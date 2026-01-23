#include "BattleSystem.h"
//#include "../Battle/Calculation/Calculation.h"
//#include "../Character/Factory/CharacterFactory.h"
//#include "../View/Font/FontManager.h"
//#include "../Battle/UserController/UserController.h"
//#include "../Battle/UserController/ActionData.h"
//#include "../System/InPutManager/InPutManager.h"

/// <summary>
/// 終了処理
/// </summary>
BattleSystem::BattleSystem(sf::RenderWindow& arg_window)
	:m_phase(TurnPhase::StartTurn),
	m_turnCount(0)
{
	// 生成・初期化
	Init();
}

/// <summary>
/// 生成
/// </summary>
/// <returns>成功：失敗</returns>
bool BattleSystem::Init()
{

	// コンテックス
	m_context = std::make_unique<BattleContext>();
	if (!m_context) {
		std::cout << "BattleSystem/m_context:nullptr" << std::endl;
		return false;
	}
	// コントローラー
	m_userController = std::make_unique<UserController>(*m_context);
	if (!m_userController)
	{
		std::cout << "BattleSystem/m_userController:nullptr" << std::endl;
		return false;
	}
	// コスト
	m_costManager = std::make_unique<CostManager>();
	if (!m_costManager)
	{
		std::cout << "BattleSystem/m_costManager:nullptr" << std::endl;;
		return false;
	}

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

}

/// <summary>
/// 描画
/// </summary>
/// <param name="window"></param>
void BattleSystem::Render(sf::RenderWindow& window)
{
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

	// ユーザー入力更新
	m_userController->Update(window);

	// 行動が確定したか？
	if (!m_userController->HasConfirmedAction())
	{
		return;
	}

	// 確定アクション取得
	UserAction action = m_userController->ConsumeAction();

	// コスト不足なら無効
	if (!m_costManager->CanConsume(1))
	{
		return;
	}

	// コスト消費
	m_costManager->Consume(1);

	// 行動キャラ取得
	auto actor = action.actor;

	if (!actor || actor->IsDead())
	{
		return;
	}

	// 使用カード取得（キャラ保持カード）
	const CardData& card = actor->GetCardData(action.cardId);

	// ターゲット候補生成
	auto targets = m_context->CreateTargetCandidates(card.targetType,actor->GetFaction(), actor);

	// 単体/全体補完
	std::vector<std::shared_ptr<Character>> finalTargets;

	if (card.targetType == TargetType::SELF)	// 自分自身
	{
		finalTargets.push_back(actor);
	}
	else if (card.targetType == TargetType::OPPONENT || card.targetType == TargetType::ALLY)	// 単体
	{
		finalTargets.push_back(targets[action.targetIds[0]]);
	}
	else
	{
		// 全体
		finalTargets = targets;
	}

	// アクション適用
	ApplyAction(actor, finalTargets, card);

	// 使用カード破棄
	int discardedId = actor->DiscardCard(action.cardId);
	CardManager::GetInstance().SendCardIdToCemetery(discardedId);

	// 行動数が尽きたらエネミーターン
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
		// 
		if (enemy->GetCardCount() == 0)
		{
			continue;
		}

		// 
		int cardIndex = enemy->DecideActionCardIndex();
		int cardId = enemy->GetHeldCardId(cardIndex);
		const CardData& card = CardManager::GetInstance().GetCardData(cardId);

		auto targets = m_context->CreateTargetCandidates(card.targetType,enemy->GetFaction(), enemy);

		if (targets.empty()) 
		{
			continue;
		}

		std::vector<std::shared_ptr<Character>> finalTargets;

		// 
		if (card.targetType == TargetType::SELF)
		{
			finalTargets.push_back(enemy);
		}
		else if (card.targetType == TargetType::OPPONENT || card.targetType == TargetType::ALLY)
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
		else
		{
			finalTargets = targets;
		}

		// 
		ApplyAction(enemy, finalTargets, card);

		// 
		int discardedId = enemy->DiscardCard(cardIndex);
		CardManager::GetInstance().SendCardIdToCemetery(discardedId);
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
			int damage = static_cast<int>(actor->GetData().atk * card.power * actor->GetData().buff.power);

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
