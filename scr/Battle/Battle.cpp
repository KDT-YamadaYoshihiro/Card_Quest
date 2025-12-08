#include "../Battle/Battle.h"

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

        StartTurn();

        break;

    case TurnPhase::PlayerTurn:
        
        PlayerUpdate();

        break;

    case TurnPhase::EnemyTurn:

        // エネミーアップデートを呼ぶ
        EnemyUpdate();

        break;

    case TurnPhase::End:
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
    m_turnCount++;
    m_phase = (m_phase == TurnPhase::PlayerTurn)
        ? TurnPhase::EnemyTurn : TurnPhase::PlayerTurn;

    if (m_phase == TurnPhase::PlayerTurn) {
        StartTurn();
    }
}
