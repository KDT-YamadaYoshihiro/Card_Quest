#include "BattleSystem.h"
#include "System/Calculation/Calculation.h"
#include "Entity/Character/Factory/CharacterFactory.h"
#include "CSVLoad/CardLoader.h"
#include "Scene/SceneManager/SceneManager.h"
#include "View/Font/FontManager.h"
#include "System/InPutManager/InPutMouseManager.h"
#include "View/Effect/EffectManager/EffectManager.h"

#include "Scene/StageBuildScene/StageBulidScene.h"
#include "Scene/PartyBuildScene/PartyBuildScene.h"
#include <SFML/System.hpp>
/// <summary>
/// 初期化処理
/// </summary>
BattleSystem::BattleSystem(sf::RenderWindow& arg_window)
	:m_phase(TurnPhase::StartTurn),
	m_userPhase(UserTurnPhase::Start),
	m_enemyPhase(EnemyTurnPhase::Start),
	m_turnCount(0)
{

}

/// <summary>
/// 生成
/// </summary>
/// <returns>成功：失敗</returns>
bool BattleSystem::Init(sf::RenderWindow& arg_window)
{
	// すべてのカード情報を取得
	CardManager::GetInstance().InitCardMaster(CardLoader::GetInstance().GetAll());
	std::cout << CardManager::GetInstance().GetDeckCount() << std::endl;

	// 描画システム
	m_render = std::make_unique<RenderSystem>(arg_window);
	if (!m_render)
	{
		ConsoleView::GetInstance().Add("BattleSystem/m_render:nullptr\n");
		return false;
	}

	// コンテックス
	auto& session = SceneManager::GetInstance().GetSession();
	m_context = session.battleContext;
	if (!m_context) {
		ConsoleView::GetInstance().Add("BattleSystem/ m_context:nullptr\n");
		return false;
	}
	// キャラクターの作成
	m_players = m_context->GetPlayers();
	for (auto& id : m_context->GetEnemyIds())
	{
		auto enemy = CharacterFactory::GetInstance().CreateCharacter<EnemyCharacter>(id, 2);
		enemy->AddCard(COMMON_CARDID);
		enemy->AddCard(UNIQUE_CARDID + id);
		m_enemies.push_back(enemy);
	}

	// キャラ座標の初期化
	CharaInitPosition();


	// 生成確認
	if (m_players.empty()) {
		ConsoleView::GetInstance().Add("BattleSystem/m_players:emptyr\n");
		return false;
	}
	if (m_enemies.empty())
	{
		ConsoleView::GetInstance().Add("BattleSystem/m_enemies:emptyr\n");
		return false;
	}

	// コンテックスの初期化
	if (!m_context->Init(m_enemies))
	{
		ConsoleView::GetInstance().Add("BattleSystem / m_context->Init() : false\n");
		return false;
	}


	// バトル画面描画
	m_battleView = std::make_unique<BattleView>(*m_context, *m_render);
	if (!m_battleView)
	{
		ConsoleView::GetInstance().Add("BattleSystem / m_battleView : nullptr\n");
		return false;
	}

	// コントローラー
	m_userController = std::make_unique<UserController>(*m_context, *m_battleView);
	if (!m_userController)
	{
		ConsoleView::GetInstance().Add("BattleSystem / m_userController : null_ptr\n");
		return false;
	}

	// ボタン系
	m_toPartySceneButton = std::make_unique<BoxButton>(sf::Vector2f(200.f, 50.f), sf::Vector2f(300.f, 500.f), FontManager::GetInstance().GetFont(), "SelectParty");
	m_toStageSelectButton = std::make_unique<BoxButton>(sf::Vector2f(200.f, 50.f), sf::Vector2f(950.f, 500.f), FontManager::GetInstance().GetFont(), "SelectStage");

	if (!m_toPartySceneButton)
	{
		ConsoleView::GetInstance().Add("BattleSystem / m_toPartySceneButton : null_ptr\n");
		return false;
	}
	if (!m_toStageSelectButton)
	{
		ConsoleView::GetInstance().Add("BattleSystem / m_toStageSelectButton : null_ptr\n");
		return false;
	}

	ConsoleView::GetInstance().Add("BattleSystem /Init() : true\n");
	return true;
}

/// <summary>
///	更新処理
/// </summary>
void BattleSystem::Update(sf::RenderWindow& arg_window)
{
	// フェーズ進行
	switch (m_phase)
	{
	case BattleSystem::TurnPhase::StartTurn:
		ConsoleView::GetInstance().Add("TurnPhase::StartTurn\n");
		this->StartTurn();
		break;
	case BattleSystem::TurnPhase::UserTurn:
		ConsoleView::GetInstance().Add("TurnPhase::UserTurn\n");

		this->UserTurn(arg_window);
		break;
	case BattleSystem::TurnPhase::EnemyTurn:
		ConsoleView::GetInstance().Add("TurnPhase::EnemyTurn\n");

		this->EnemyTurn();
		break;
	case BattleSystem::TurnPhase::EndTurn:
		ConsoleView::GetInstance().Add("TurnPhase::EndTurn\n");

		this->EndTurn();
		break;
	case BattleSystem::TurnPhase::Result:
		this->ResultEvent(arg_window);
		break;
	default:
		break;
	} // switch(m_phase)

	if (this->IsBattleEnd())
	{
		// バトル終了判定
		if (m_phase != TurnPhase::Result && IsBattleEnd())
		{
			const auto& players = m_context->GetPlayers();
			for (auto& player : players)
			{
				if (!player) continue;

				// キャラクターから全カードを回収
				std::vector<int> cards = player->ClearAndReturnCards();

				// 回収したIDをすべて墓地へ
				for (int id : cards)
				{
					CardManager::GetInstance().SendCardIdToCemetery(id);
				}
				// 勝利アニメーションへ
				player->SetAnimation(CharacterAnimState::WIN, 20.0f);

			}
		}
			
		ConsoleView::GetInstance().Reset();
		m_phase = TurnPhase::Result;
	}

	// Animation更新のためのデルタタイム計測
	float dt = m_clock.restart().asSeconds();
	// キャラクター系の更新
	for (auto& p : m_players)
	{
		p->Update();
		p->UpdateAnimTimer(dt);
	}
	for (auto& e : m_enemies)
	{
		e->Update();
		e->UpdateAnimTimer(dt);
	}

	// コンテキストの更新
	m_context->SetTurnPhase(static_cast<int>(m_phase));
	// 描画系の更新
	m_battleView->Update(dt);
	// エフェクトの更新
	EffectManager::GetInstance().Update(dt);
}

/// <summary>
/// 描画
/// </summary>
/// <param name="window"></param>
void BattleSystem::Render(sf::RenderWindow& arg_window)
{
	// バトル系の描画
	m_battleView->Render(arg_window);

	EffectManager::GetInstance().Draw(arg_window);

	// リザルト時のみ表示
	if (m_phase == TurnPhase::Result)
	{
		this->ResultView(arg_window);
	}

}

/// <summary>
/// バトル終了判定
/// </summary>
/// <returns></returns>
bool BattleSystem::IsBattleEnd() const
{
	// 敵が全滅しているか
	if (m_context->GetAliveEnemies().empty())
	{
		return true;
	}

	// プレイヤーが全滅しているか
	auto alivePlayers = m_context->GetAlivePlayers();
	if (alivePlayers.empty())
	{
		return true;
	}

	// 行動不能状態の判定
	// 山札が空、かつ生存している全プレイヤーの手札が0枚の場合
	if (CardManager::GetInstance().GetDeckCount() == 0)
	{
		bool hasAnyCard = false;
		for (const auto& player : alivePlayers)
		{
			if (player->GetCardCount() > 0)
			{
				hasAnyCard = true;
				break;
			}
		}

		if (!hasAnyCard)
		{
			// デッキも手札も尽きたので、これ以上行動できない
			return true;
		}
	}

	return false;
}

/// <summary>
/// ユーザーの勝利か
/// </summary>
/// <returns></returns>
bool BattleSystem::IsUserWin() const
{
	return m_context->IsEnemyAllDead() && !m_context->IsPlayerAllDead();
}

/// <summary>
/// パーティー編成画面への遷移判定
/// </summary>
/// <returns></returns>
bool BattleSystem::IsToPartyScene() const
{
	 return m_toPartyScene; 
}

/// <summary>
/// ステージ選択Sceneへの遷移判定
/// </summary>
/// <returns></returns>
bool BattleSystem::IsToStageSelectScene() const
{
	return m_toStageSelectScene; 
}

/// <summary>
/// キャラクターの座標設定
/// </summary>
void BattleSystem::CharaInitPosition()
{
	// プレイヤー側：画面中央より少し左下に配置
	for (int i = 0; i < m_players.size(); i++)
	{
		sf::Vector2f pos({ 50.0f + i * 140.0f, 300.0f });
		m_players[i]->SetPosition(pos);
	}
	// エネミー側：画面右上のエリアに配置
	for (int i = 0; i < m_enemies.size(); i++)
	{
		sf::Vector2f pos({ 750.0f + i * 165.0f, 200.0f });
		m_enemies[i]->SetPosition(pos);
	}
}

/// <summary>
/// ターン開始
/// </summary>
void BattleSystem::StartTurn()
{
	// コスト回復
	CostManager::GetInstance().ResetCost();

	// 生存判定
	if (this->IsBattleEnd())
	{
		return;
	}

	// 次のフェーズ
	m_phase = TurnPhase::UserTurn;
}

/// <summary>
/// ユーザーターン
/// </summary>
void BattleSystem::UserTurn(sf::RenderWindow& window)
{
	UserAction action;
	int discardId;

	switch (m_userPhase)
	{
	case BattleSystem::UserTurnPhase::Start:

		ConsoleView::GetInstance().Add("UserTurnPhase::Start\n");
		// 各プレイヤーにカード配布
		for (auto& p : m_context->GetAlivePlayers())
		{
			p->DrawCard();
		}

		m_userPhase = UserTurnPhase::Select;

		break;
	case BattleSystem::UserTurnPhase::Select:

		ConsoleView::GetInstance().Add("UserTurnPhase::Select\n");

		// 選択系
		// キャラクター、カード、ターゲット
		m_userController->Update(window);

		if(!m_userController->HasConfirmedAction())	
		{ 
			return;
		}
		// 状態遷移
		m_userPhase = UserTurnPhase::Action;

		break;
	case BattleSystem::UserTurnPhase::Action:
	{
		ConsoleView::GetInstance().Add("UserTurnPhase::Action\n");

		
		action = m_userController->ConsumeAction();
		// コスト消費可能か
		if (!CostManager::GetInstance().CanConsume(1))
		{
			return;
		}
		// コスト消費
		CostManager::GetInstance().Consume(1);

		// データ取得
		const CardData& card = CardManager::GetInstance().GetCardData(action.cardId);

		// アクション
		ApplyAction(action.actor, action.targets, card);

		// 使用カードは墓地へ
		discardId = action.actor->DiscardCardById(action.cardId);
		CardManager::GetInstance().SendCardIdToCemetery(discardId);

		// 描画系をリセット
		m_battleView->ResetTransientView();
		// 状態遷移
		m_userPhase = UserTurnPhase::EndCheck;

		break;
	}
	case BattleSystem::UserTurnPhase::EndCheck:

		ConsoleView::GetInstance().Add("UserTurnPhase::EndCheck\n");

		// 行動数0 
		// true:エネミーターンへ / false:ユーザーターンの続行
		if (CostManager::GetInstance().IsEmpty())
		{
			m_userPhase = UserTurnPhase::EndUserTurn;
		}
		else {
			m_userPhase = UserTurnPhase::Start;
		}

		break;

	case BattleSystem::UserTurnPhase::EndUserTurn:

		// 
		ConsoleView::GetInstance().Add("UserTurnPhase::EndUserTurn\n");

		// ユーザーフェーズのリセット
		m_userPhase = UserTurnPhase::Start;
		// エネミーターンへ
		m_phase = TurnPhase::EnemyTurn;
		break;
	} // switch()
}


/// <summary>
/// エネミーターン
/// </summary>
void BattleSystem::EnemyTurn()
{

	switch (m_enemyPhase)
	{
	case BattleSystem::EnemyTurnPhase::Start:
	{
		// ターンの最初に「今から動く敵」を確定させる
		m_actingEnemies = m_context->GetAliveEnemies();
		m_currentEnemyIndex = 0;

		if (m_actingEnemies.empty()) {
			m_enemyPhase = EnemyTurnPhase::End;
		}
		else {
			m_enemyPhase = EnemyTurnPhase::Select;
		}
		break;
	}
	case BattleSystem::EnemyTurnPhase::Select:
	{
		// 行動予定リストから取得
		m_currentEnemy = m_actingEnemies[m_currentEnemyIndex];

		// もし直前の味方の反撃などで死んでいた場合の安全策
		if (!m_currentEnemy || m_currentEnemy->IsDead()) {
			m_enemyPhase = EnemyTurnPhase::NextEnemy;
			break;
		}

		m_enemyFinalTargets.clear();
		m_enemyPhase = EnemyTurnPhase::Action;
		break;
	}
	case BattleSystem::EnemyTurnPhase::Action:
	{
		// 行動予定の敵が行う行動を決定する
		int cardIndex = m_currentEnemy->DecideActionCardIndex();
		if (cardIndex < 0)
		{
			m_enemyPhase = EnemyTurnPhase::NextEnemy;
			break;
		}

		// カード情報の取得
		int cardId = m_currentEnemy->GetHeldCardId(cardIndex);
		const CardData& card = CardManager::GetInstance().GetCardData(cardId);

		// ターゲットの決定
		auto targets = m_context->CreateTargetCandidates(card.targetType,m_currentEnemy->GetFaction(),m_currentEnemy);

		// ターゲットがいない場合は行動できないので次の敵へ
		if (targets.empty())
		{
			m_enemyPhase = EnemyTurnPhase::NextEnemy;
			break;
		}
		// ターゲットの確定
		if (card.targetType == TargetType::SELF)
		{
			m_enemyFinalTargets.push_back(m_currentEnemy);
		}
		else if (card.targetType == TargetType::OPPONENT || card.targetType == TargetType::ALLY)
		{
			std::vector<Character*> rawTargets;
			for (auto& t : targets) {
				rawTargets.push_back(t.get());
			}

			int targetIndex = m_currentEnemy->DecideTargetIndex(rawTargets);
			if (targetIndex >= 0)
			{
				m_enemyFinalTargets.push_back(targets[targetIndex]);
			}
		}
		else
		{
			m_enemyFinalTargets = targets;
		}

		this->ApplyAction(m_currentEnemy, m_enemyFinalTargets, card);
		m_enemyPhase = EnemyTurnPhase::NextEnemy;
		break;
	}
	case BattleSystem::EnemyTurnPhase::NextEnemy:
	{
		++m_currentEnemyIndex;

		// 行動予定リストのサイズと比較する
		if (m_currentEnemyIndex < static_cast<int>(m_actingEnemies.size())) {
			m_enemyPhase = EnemyTurnPhase::Select;
		}
		else {
			m_enemyPhase = EnemyTurnPhase::End;
			m_actingEnemies.clear(); // リストを空にする
		}
		break;
	}
	case BattleSystem::EnemyTurnPhase::End:
		m_enemyPhase = EnemyTurnPhase::Start;
		m_phase = TurnPhase::EndTurn;
		break;
	default:
		break;
	} // switch()

}

/// <summary>
/// ターン終了
/// </summary>
void BattleSystem::EndTurn()
{
	// ターン数増加
	m_turnCount++;

	// バフ更新
	for (auto& c : m_context->GetAlivePlayers())
	{
		c->UpdateBuff();
	}

	for (auto& e : m_context->GetAliveEnemies())
	{
		e->UpdateBuff();
	}

	// 次ターンへ
	m_phase = TurnPhase::StartTurn;
}

/// <summary>
/// リザルトEvent処理
/// </summary>
void BattleSystem::ResultEvent(sf::RenderWindow& arg_woindow)
{
	// ボタンが存在しなければ処理しない
	if (!m_toPartySceneButton || !m_toStageSelectButton)
	{
		return;
	}

	// ボタンがホバーしていたら色の変更
	// 編成画面
	if (m_toPartySceneButton->IsHovered(InPutMouseManager::GetInstance().GetMousePosition(arg_woindow)))
	{
		m_toPartySceneButton->SetColor(sf::Color::Yellow);
	}
	else
	{
		m_toPartySceneButton->SetColor(sf::Color::White);
	}
	// ステージ選択ボタン
	if (m_toStageSelectButton->IsHovered(InPutMouseManager::GetInstance().GetMousePosition(arg_woindow)))
	{
		m_toStageSelectButton->SetColor(sf::Color::Yellow);
	}
	else
	{
		m_toStageSelectButton->SetColor(sf::Color::White);
	}

	// クリックしていなければ処理しない
	if (!InPutMouseManager::GetInstance().IsLeftClicked())
	{
		return;
	}

	// クリックしている場合は、クリック位置を取得
	bool isClickTriggered = InPutMouseManager::GetInstance().IsLeftClicked();
	// マウス座標の取得
	sf::Vector2f mousePos = InPutMouseManager::GetInstance().GetMousePosition(arg_woindow);

	// ボタンのクリック判定
	// 編成画面に
	if (m_toPartySceneButton->IsClicked(mousePos,isClickTriggered))
	{
		m_toPartyScene = true;
	}
	// ステージ選択画面に
	if (m_toStageSelectButton->IsClicked(mousePos, isClickTriggered))
	{
		m_toStageSelectScene = true;
	}


}

/// <summary>
/// リザルトの描画系
/// </summary>
/// <param name="arg_window"></param>
void BattleSystem::ResultView(sf::RenderWindow& arg_window)
{

	// 黒のベース（半透明)
	sf::RectangleShape bgBox;
	bgBox.setSize(static_cast<sf::Vector2f>(arg_window.getSize()));
	bgBox.setFillColor(sf::Color(0, 0, 0, 150)); // 黒、少し透過
	arg_window.draw(bgBox);

	// クリアかオーバーか
	if (this->IsUserWin())
	{
		m_battleView->DrawClearBanner(arg_window);
	}
	else
	{
		m_battleView->DrawGameOverBanner(arg_window);
	}

	// ボタンの描画
	m_toPartySceneButton->Draw(arg_window);
	m_toStageSelectButton->Draw(arg_window);

}

//	カード使用時効果
void BattleSystem::ApplyAction(const std::shared_ptr<Character>& actor, const std::vector<std::shared_ptr<Character>>& targets, const CardData& card)
{
	// アクターが存在しない、もしくは死んでいる場合は処理しない
	if (!actor)
	{
		return;
	}

	// 全体ターゲットのエフェクト生成
	const auto& effectData = EffectDataLoder::GetInstance().GetConfig(std::to_string(card.cardId));
	if (effectData.positionType == PositionType::PlayerSide || effectData.positionType == PositionType::EnemySize) {

		// プレイヤー側かエネミー側かでエフェクトの位置を変える
		if (targets[0]->GetFaction() == Faction::Player)
		{
			sf::Vector2f basePos = m_context->GetPlayers()[0]->GetPosition();
			EffectManager::GetInstance().CreateEffect(std::to_string(card.cardId), basePos);
		}
		else
		{
			sf::Vector2f basePos = m_context->GetEnemies()[0]->GetPosition();
			EffectManager::GetInstance().CreateEffect(std::to_string(card.cardId), basePos);
		}
	}

	// 単体ターゲットのエフェクト生成
	for (auto& target : targets) {
		if (effectData.positionType == PositionType::PlayerChara || effectData.positionType == PositionType::EnemyChara) {
			EffectManager::GetInstance().CreateEffect(std::to_string(card.cardId), target->GetPosition());
		}
	}


	// アクション効果
	for (auto& target : targets)
	{
		// 生存確認
		if (!target || target->IsDead())
		{
			continue;
		}
		// カードの種類ごとの処理
		HandleActionType(actor, target, card);

	} //for(auto& target : targets)
}

/// <summary>
/// カード効果内容
/// </summary>
/// <param name="actor"></param>
/// <param name="targets"></param>
/// <param name="card"></param>
void BattleSystem::HandleActionType(const std::shared_ptr<Character>& arg_actor, const std::shared_ptr<Character>& arg_target, const CardData& arg_card)
{
	// カードの種類ごとに処理
	switch (arg_card.actionType)
	{
	case ActionType::ATTCK:
	{
		arg_actor->SetAnimation(CharacterAnimState::ATTACK, 1.0f);
		arg_target->SetAnimation(CharacterAnimState::DAMAGE, 0.8f);

		int damage = Calculation::GetDamage(arg_actor->GetData().atk, arg_actor->GetBuffData().power, arg_card.power, arg_target->GetData().def);
		arg_target->TakeDamage(damage);
		// ダメージ表示
		m_battleView->AddDamagePopup(m_battleView->CalcDamagePopupPos(arg_target), damage, false);
		// 確認用ログ
		ConsoleView::GetInstance().Add(arg_actor->GetData().name + "が" + arg_target->GetData().name + "に" + std::to_string(damage) + "与えた\n");
		ConsoleView::GetInstance().Add(std::to_string(arg_target->GetData().maxHp) + "/" + std::to_string(arg_target->GetData().hp) + "\n");
		ConsoleView::GetInstance().Add("ターンが" + std::to_string(arg_card.actionPlus) + "追加された\n");
		// コストの増減
		CostManager::GetInstance().AddCost(arg_card.actionPlus);
		m_battleView->ShowCostGain(arg_card.actionPlus);

		break;
	}
	case ActionType::MAGIC:
	{
		arg_actor->SetAnimation(CharacterAnimState::MAGIC, 1.0f);
		arg_target->SetAnimation(CharacterAnimState::DAMAGE, 0.8f);

		int damage = Calculation::GetDamage(arg_actor->GetData().magicAtk, arg_actor->GetBuffData().power, arg_card.power, arg_target->GetData().def);
		arg_target->TakeDamage(damage);
		// ダメージ表示
		m_battleView->AddDamagePopup(m_battleView->CalcDamagePopupPos(arg_target), damage, false);
		// 確認用ログ
		ConsoleView::GetInstance().Add(arg_actor->GetData().name + "が" + arg_target->GetData().name + "に" + std::to_string(damage) + "与えた\n");
		ConsoleView::GetInstance().Add(std::to_string(arg_target->GetData().maxHp) + "/" + std::to_string(arg_target->GetData().hp) + "\n");

		// コストの増減
		ConsoleView::GetInstance().Add("ターンが" + std::to_string(arg_card.actionPlus) + "追加された\n");
		CostManager::GetInstance().AddCost(arg_card.actionPlus);
		m_battleView->ShowCostGain(arg_card.actionPlus);

		break;
	}
	case ActionType::HEAL:
	{
		arg_actor->SetAnimation(CharacterAnimState::MAGIC, 1.0f);
		int heal = Calculation::GetMultiplicative(arg_actor->GetData().maxHp, arg_card.power);
		arg_target->TakeHeal(heal);
		// ダメージ表示
		m_battleView->AddDamagePopup(m_battleView->CalcDamagePopupPos(arg_target), heal, true);
		// 確認用ログ
		ConsoleView::GetInstance().Add(arg_actor->GetData().name + "が" + arg_target->GetData().name + "に" + std::to_string(heal) + "回復させた\n");
		ConsoleView::GetInstance().Add(std::to_string(arg_target->GetData().maxHp) + "/" + std::to_string(arg_target->GetData().hp) + "\n");

		// コストの増減
		ConsoleView::GetInstance().Add("ターンが" + std::to_string(arg_card.actionPlus) + "追加された\n");
		CostManager::GetInstance().AddCost(arg_card.actionPlus);
		m_battleView->ShowCostGain(arg_card.actionPlus);

		break;
	}
	case ActionType::BUFF:

		arg_actor->SetAnimation(CharacterAnimState::MAGIC, 1.0f);
		arg_target->TakeBuff(arg_card.power, arg_card.turn);
		// コストの増減
		ConsoleView::GetInstance().Add("ターンが" + std::to_string(arg_card.actionPlus) + "追加された\n");
		CostManager::GetInstance().AddCost(arg_card.actionPlus);
		m_battleView->ShowCostGain(arg_card.actionPlus);

		break;

	case ActionType::ACTION_NONE:

		ConsoleView::GetInstance().Add("ターンが" + std::to_string(arg_card.actionPlus) + "追加された\n");
		CostManager::GetInstance().AddCost(arg_card.actionPlus);

	} // switch()

}
