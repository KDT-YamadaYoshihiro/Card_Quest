#include "../Battle/Battle.h"
#include "../Battle/Calculation/Calculation.h"

Battle::Battle() :
	m_deck(std::make_shared<Deck>()),
	m_cost(std::make_shared<CostManager>()),
	m_phase(TurnPhase::StartTurn),
	m_turnCount(0)
{
}

void Battle::Init(const std::vector<std::shared_ptr<Character>>& players, const std::vector<std::shared_ptr<Character>>& enemies, const std::vector<Card>& allCards)
{
	m_playerMembers = players;
	m_enemyMembers = enemies;
	m_deck->Init(allCards);
	m_cost->Init(3);
}

void Battle::Update()
{
    if (CheckWin() || CheckLose()) {
        m_phase = TurnPhase::End;
        return;
    }

    switch (m_phase) {

    case TurnPhase::StartTurn:

		// ターン開始処理
        StartTurn();

        break;

    case TurnPhase::PlayerTurn:
        
		// プレイヤーアップデートを呼ぶ
        PlayerUpdate();

        break;

    case TurnPhase::EnemyTurn:

        // エネミーアップデートを呼ぶ
        EnemyUpdate();

        break;

    case TurnPhase::End:

		// ターン終了処理
        EndTurn();
        
        break;

    default:
        break;
    }
}


void Battle::StartTurn()
{
    // 手札補充
    auto drawn = m_deck->Draw(5);
    m_hand.Add(drawn);

    // コスト回復
    m_cost->ResetCost();

	// フェーズをプレイヤーターンに変更
    m_phase = TurnPhase::PlayerTurn;
}

void Battle::PlayerUpdate()
{


}

void Battle::EnemyUpdate()
{
}

void Battle::EndTurn()
{
	// ターン数を増やす
    m_turnCount++;
	// フェーズを切り替える
    m_phase = (m_phase == TurnPhase::PlayerTurn)
        ? TurnPhase::EnemyTurn : TurnPhase::PlayerTurn;

	// 次のターン開始処理
    if (m_phase == TurnPhase::PlayerTurn) {
        StartTurn();
    }
}
