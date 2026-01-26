#include "BattleSystem.h"
//#include "../Battle/Calculation/Calculation.h"
#include "../Character/Factory/CharacterFactory.h"
#include "../CSVLoad/CardLoader.h"
//#include "../View/Font/FontManager.h"
//#include "../Battle/UserController/UserController.h"
//#include "../Battle/UserController/ActionData.h"
//#include "../System/InPutManager/InPutManager.h"

/// <summary>
/// 初期化処理
/// </summary>
BattleSystem::BattleSystem(sf::RenderWindow& arg_window)
	:m_phase(TurnPhase::StartTurn),
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

	// コンテックス
	m_context = std::make_unique<BattleContext>();
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
	for (int i = 1; i < 5; i++)
	{
		auto player = CharacterFactory::GetInstance().CreateCharacter<PlayerCharacter>(i,1);
		m_players.push_back(player);
	}
	for (int i = 0; i < 3; i++) 
	{
		auto enemy = CharacterFactory::GetInstance().CreateCharacter<EnemyCharacter>(7,1);
		m_enemies.push_back(enemy);
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
	if (!m_context->Init(m_players, m_enemies))
	{
		std::cout << "BattleSystem/m_context->Init() : false" << std::endl;
		return false;
	}

	// 描画システム
	m_render = std::make_unique<RenderSystem>(arg_window);
	if (!m_render)
	{
		std::cout << "BattleSysytem/m_render:nullptr" << std::endl;
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

	// 各プレイヤーにカード配布
	for (auto& p : m_context->GetAlivePlayers())
	{
		p->DrawCard(); // Character責務
	}

	// 次のフェーズ
	m_phase = TurnPhase::UserTurn;
}

/// <summary>
/// ユーザーターン
/// </summary>
void BattleSystem::UserTurn(sf::RenderWindow& window)
{

	m_userController->Update(window);

	if (!m_userController->HasConfirmedAction())
	{
		return;
	}

	UserAction action = { m_userController->GetSelectActor(),m_userController->GetSelectCardId(),m_userController->GetSelectTargetIndices() };

	if (!m_costManager->CanConsume(1))
	{
		return;
	}

	m_costManager->Consume(1);

	const CardData& card = CardManager::GetInstance().GetCardData(action.cardId);

	ApplyAction(action.actor, action.targets, card);

	m_costManager->AddCost(card.actionPlus);
	m_battleView->ShowCostGain(card.actionPlus);

	int discardId = action.actor->DiscardCard(action.actor->GetHeldCardById(action.cardId));
	CardManager::GetInstance().SendCardIdToCemetery(discardId);

	if (m_costManager->IsEmpty())
	{
		m_phase = TurnPhase::EnemyTurn;
	}
}

/// <summary>
/// エネミーターン
/// </summary>
void BattleSystem::EnemyTurn()
{
	for (auto& enemy : m_context->GetAliveEnemies())
	{
		// エネミーの手札確認
		if (enemy->GetCardCount() == 0)
		{
			continue;
		}

		// カードデータの取得
		int cardIndex = enemy->DecideActionCardIndex();
		int cardId = enemy->GetHeldCardId(cardIndex);
		const CardData& card = CardManager::GetInstance().GetCardData(cardId);

		// ターゲット候補の取得
		auto targets = m_context->CreateTargetCandidates(card.targetType,enemy->GetFaction(), enemy);

		// ターゲット候補がからでないことを確認
		if (targets.empty()) 
		{
			continue;
		}

		// ターゲット確定枠
		std::vector<std::shared_ptr<Character>> finalTargets;

		// カードのタイプに合わせてアクション
		if (card.targetType == TargetType::SELF)	// セルフ
		{
			finalTargets.push_back(enemy);
		}
		else if (card.targetType == TargetType::OPPONENT || card.targetType == TargetType::ALLY)	// 単体
		{
			std::vector<Character*> rawTargets;
			for (auto& t : targets) 
			{
				rawTargets.push_back(t.get());
			}
			int targetIndex = enemy->DecideTargetIndex(rawTargets);

			if (targetIndex >= 0)
			{
				finalTargets.push_back(targets[targetIndex]);
			}
		}
		else																						// 全体
		{
			finalTargets = targets;
		}

		// アクション
		ApplyAction(enemy, finalTargets, card);

		// エネミーはカード破棄しない

	}

	m_phase = TurnPhase::EndTurn;
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
		if (!target || target->IsDead())
		{
			continue;
		}

		switch (card.actionType)
		{
		case ActionType::ATTCK:
		case ActionType::MAGIC:
		{
			int damage = static_cast<int>(actor->GetData().atk * card.power * actor->GetBuffData().power);
			target->TakeDamage(damage);
			break;
		}

		case ActionType::HEAL:
			target->TakeHeal(static_cast<int>(card.power));
			break;

		case ActionType::BUFF:
			target->TakeBuff(card.power);
			break;
		}
	}
}
