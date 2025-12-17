#include "../Battle/Battle.h"
#include "../Battle/Calculation/Calculation.h"

Battle::Battle() :
	m_cost(std::make_shared<CostManager>()),
	m_phase(TurnPhase::StartTurn),
	m_turnCount(0)
{
}

void Battle::Init(const std::vector<std::shared_ptr<Character>>& players, const std::vector<std::shared_ptr<Character>>& enemies, const std::vector<Card>& allCards)
{
	m_playerMembers = players;
	m_enemyMembers = enemies;
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

    // コスト回復
    m_cost->ResetCost();

	// フェーズをプレイヤーターンに変更
    m_phase = TurnPhase::PlayerTurn;
}

void Battle::PlayerUpdate()
{
    // カードを選択


    // 選択したカードでプレイヤーがアクション
    m_playerMembers[/*ここに選択カード者指定*/1]->Update();

    

    // 終了時
    // ターン終了ボタンが押されたとき
    m_phase = TurnPhase::EnemyTurn;
}

void Battle::EnemyUpdate()
{
    // エネミーはカードをランダムで選択一回だけアクションを起こして終了
    for (auto& e : m_enemyMembers)
    {
        e->Update();
    }

    // 終了時
    // メンバー全員がアクションを行ったら終了
    m_phase = TurnPhase::End;
}

void Battle::EndTurn()
{
	// ターン数を増やす
    m_turnCount++;

    // 生存確認後続行か判定
    for (auto& p : m_playerMembers) {
        for (auto& e : m_enemyMembers) {
            if (p->GetStatus().dead && e->GetStatus().dead) {

                p->UpdateBuff();
                e->UpdateBuff();

                m_phase = TurnPhase::StartTurn;
            }
        }
    }
}
