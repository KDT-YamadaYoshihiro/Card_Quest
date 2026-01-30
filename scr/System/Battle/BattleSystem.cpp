#include "BattleSystem.h"
#include "System/Calculation/Calculation.h"
#include "Entity/Character/Factory/CharacterFactory.h"
#include "CSVLoad/CardLoader.h"
#include "Scene/SceneManager/SceneManager.h"
//#include "../View/Font/FontManager.h"
//#include "../Battle/UserController/UserController.h"
//#include "../Battle/UserController/ActionData.h"
//#include "../System/InPutManager/InPutManager.h"

/// <summary>
/// 初期化処理
/// </summary>
BattleSystem::BattleSystem(sf::RenderWindow& arg_window)
	:m_phase(TurnPhase::StartTurn),
	m_userPhase(UserTurnPhase::Start),
	m_enemyPhase(EnemyTurnPhase::Start),
	m_turnCount(0)
{
	// すべてのカード情報を取得
	CardManager::GetInstance().InitCardMaster(CardLoader::GetInstance().GetAll());
	std::cout << CardManager::GetInstance().GetDeckCount() << std::endl;
	// 生成・初期化
	if (!Init(arg_window))
	{
		std::cout << "BattleSystem/Init():false" << std::endl;
	}

	// 座標の初期化
	InitPosition();
}

/// <summary>
/// 生成
/// </summary>
/// <returns>成功：失敗</returns>
bool BattleSystem::Init(sf::RenderWindow& arg_window)
{
	// 描画システム
	m_render = std::make_unique<RenderSystem>(arg_window);
	if (!m_render)
	{
		std::cout << "BattleSysytem/m_render:nullptr" << std::endl;
		return false;
	}

	// コンテックス
	auto& session = SceneManager::GetInstance().GetSession();
	m_context = session.battleContext;
	if (!m_context) {
		std::cout << "BattleSystem/m_context:nullptr" << std::endl;
		return false;
	}
	// コスト
	m_costManager = std::make_unique<CostManager>();
	if (!m_costManager)
	{
		std::cout << "BattleSystem/m_costManager:nullptr" << std::endl;;
		return false;
	}
	// キャラクターの作成
	m_players = m_context->GetPlayers();
	for (auto& id : m_context->GetEnemyIds())
	{
		m_enemies.push_back(CharacterFactory::GetInstance().CreateCharacter<EnemyCharacter>(id, 2));
	}


	// 生成確認
	if (m_players.empty()) {
		std::cout << "BattleSystem/m_players:empty" << std::endl;
		return false;
	}
	if (m_enemies.empty())
	{
		std::cout << "BattleSysyte/m_enemis:empty" << std::endl;
		return false;
	}

	// コンテックスの初期化
	if (!m_context->Init(m_enemies))
	{
		std::cout << "BattleSystem/m_context->Init() : false" << std::endl;
		return false;
	}


	// バトル画面描画
	m_battleView = std::make_unique<BattleView>(*m_context, *m_render);
	if (!m_battleView)
	{
		std::cout << "BattleSysytem/m_battleView:nullptr" << std::endl;
		return false;
	}

	// コントローラー
	m_userController = std::make_unique<UserController>(*m_context, *m_battleView);
	if (!m_userController)
	{
		std::cout << "BattleSystem/m_userController:nullptr" << std::endl;
		return false;
	}

	std::cout << "BattleSystem/Init():成功" << std::endl;
	return true;
}

/// <summary>
///	更新処理
/// </summary>
void BattleSystem::Update(sf::RenderWindow& window)
{
	// フェーズ進行
	switch (m_phase)
	{
	case BattleSystem::TurnPhase::StartTurn:
		StartTurn();
		break;
	case BattleSystem::TurnPhase::UserTurn:
		UserTurn(window);
		break;
	case BattleSystem::TurnPhase::EnemyTurn:
		EnemyTurn();
		break;
	case BattleSystem::TurnPhase::EndTurn:
		EndTurn();
		break;
	default:
		break;
	}

	// キャラクター系の更新
	for (auto& p : m_players)
	{
		p->Update();
	}
	for (auto& e : m_enemies)
	{
		e->Update();
	}

	m_battleView->Update(1.0f / 60.0f);
}

/// <summary>
/// 描画
/// </summary>
/// <param name="window"></param>
void BattleSystem::Render(sf::RenderWindow& window)
{
	m_battleView->Render(window);
}

/// <summary>
/// バトル終了判定
/// </summary>
/// <returns></returns>
bool BattleSystem::IsBattleEnd() const
{
	return m_context->IsPlayerAllDead() || m_context->IsEnemyAllDead();
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
/// キャラクターの座標設定
/// </summary>
void BattleSystem::InitPosition()
{
	for (int i = 0; i < m_players.size(); i++)
	{
		sf::Vector2f pos({ 200.0f,200.0f + i * 150.0f });
		m_players[i]->SetPosition(sf::Vector2f(pos));
	}
	for (int i = 0; i < m_enemies.size(); i++)
	{
		sf::Vector2f pos({ 600.0f + i * 165.0f,200.0f });
		m_enemies[i]->SetPosition(pos);
	}

}

/// <summary>
/// ターン開始
/// </summary>
void BattleSystem::StartTurn()
{

	// コスト回復
	m_costManager->ResetCost();

	// 生存判定
	if (IsBattleEnd())
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

		// 各プレイヤーにカード配布
		for (auto& p : m_context->GetAlivePlayers())
		{
			p->DrawCard();
		}

		m_userPhase = UserTurnPhase::Select;

		break;
	case BattleSystem::UserTurnPhase::Select:

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

		action = m_userController->ConsumeAction();

		if (!m_costManager->CanConsume(1))
		{
			return;
		}

		m_costManager->Consume(1);

		// データ取得
		const CardData& card = CardManager::GetInstance().GetCardData(action.cardId);

		// アクション
		ApplyAction(action.actor, action.targets, card);

		// コストの増減
		m_costManager->AddCost(card.actionPlus);
		m_battleView->ShowCostGain(card.actionPlus);

		// ここに問題あるかも？
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

		// 行動数0 
		// true:エネミーターンへ / false:ユーザーターンの続行
		if (m_costManager->IsEmpty())
		{
			m_userPhase = UserTurnPhase::EndUserTurn;
		}
		else {
			m_userPhase = UserTurnPhase::Start;
		}

		break;

	case BattleSystem::UserTurnPhase::EndUserTurn:
		// エネミーターンへ
		m_userPhase = UserTurnPhase::Start;
		m_phase = TurnPhase::EnemyTurn;
		break;
	}
}


/// <summary>
/// エネミーターン
/// </summary>
void BattleSystem::EnemyTurn()
{


	switch (m_enemyPhase)
	{
	case BattleSystem::EnemyTurnPhase::Start:
		
		m_currentEnemyIndex = 0;
		m_currentEnemy.reset();
		m_enemyFinalTargets.clear();
		m_enemyPhase = EnemyTurnPhase::Select;

		break;
	case BattleSystem::EnemyTurnPhase::Select:
	{
		const auto& enemies = m_context->GetAliveEnemies();

		if (m_currentEnemyIndex >= enemies.size())
		{
			m_enemyPhase = EnemyTurnPhase::End;
			break;
		}

		m_currentEnemy = enemies[m_currentEnemyIndex];
		m_enemyFinalTargets.clear();
		m_enemyPhase = EnemyTurnPhase::Action;

		break;
	}
	case BattleSystem::EnemyTurnPhase::Action:
	{
		int cardIndex = m_currentEnemy->DecideActionCardIndex();
		if (cardIndex < 0)
		{
			m_enemyPhase = EnemyTurnPhase::NextEnemy;
			break;
		}

		int cardId = m_currentEnemy->GetHeldCardId(cardIndex);
		const CardData& card = CardManager::GetInstance().GetCardData(cardId);

		auto targets = m_context->CreateTargetCandidates(card.targetType,m_currentEnemy->GetFaction(),m_currentEnemy);

		if (targets.empty())
		{
			m_enemyPhase = EnemyTurnPhase::NextEnemy;
			break;
		}

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

		ApplyAction(m_currentEnemy, m_enemyFinalTargets, card);
		m_enemyPhase = EnemyTurnPhase::NextEnemy;
		break;
	}
	case BattleSystem::EnemyTurnPhase::NextEnemy:
		
		++m_currentEnemyIndex;
		m_enemyPhase = EnemyTurnPhase::Select;
		
		break;

	case BattleSystem::EnemyTurnPhase::End:
		m_enemyPhase = EnemyTurnPhase::Start;
		m_phase = TurnPhase::EndTurn;
		break;
	default:
		break;
	}


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

//	カード使用時効果
void BattleSystem::ApplyAction(const std::shared_ptr<Character>& actor, const std::vector<std::shared_ptr<Character>>& targets, const CardData& card)
{
	if (!actor)
	{
		return;
	}

	// アクション効果
	for (auto& target : targets)
	{
		// 生存判定
		if (!target || target->IsDead())
		{
			continue;
		}

		switch (card.actionType)
		{
		case ActionType::ATTCK:
		{
			int damage = Calculation::GetDamage(actor->GetData().magicAtk, card.power, target->GetData().def);
			target->TakeDamage(damage);
			// ダメージ表示
			m_battleView->AddDamagePopup(target->GetPosition(),damage,false);
			// 確認用ログ
			std::cout << actor->GetData().name << "が" << target->GetData().name << "に" << damage << "与えた" << std::endl;
			std::cout << target->GetData().maxHp << "/" << target->GetData().hp << std::endl;
			break;
		}
		case ActionType::MAGIC:
		{
			int damage = Calculation::GetDamage(actor->GetData().atk, card.power, target->GetData().def);
			target->TakeDamage(damage);
			// ダメージ表示
			m_battleView->AddDamagePopup(target->GetPosition(),damage,false);
			// 確認用ログ
			std::cout << actor->GetData().name << "が" << target->GetData().name << "に" << damage << "与えた" << std::endl;
			std::cout << target->GetData().maxHp << "/" << target->GetData().hp << std::endl;
			break;
		}

		case ActionType::HEAL:
		{
			int heal = Calculation::GetMultiplicative(actor->GetData().maxHp, card.power);
			target->TakeHeal(heal);
			// ダメージ表示
			m_battleView->AddDamagePopup(target->GetPosition(),heal,true);
			// 確認用ログ
			std::cout << actor->GetData().name << "が" << target->GetData().name << "に" << heal << "回復させた" << std::endl;
			std::cout << target->GetData().maxHp << "/" << target->GetData().hp << std::endl;

			break;
		}
		case ActionType::BUFF:
			target->TakeBuff(card.power);
			break;
		}
	}
}
