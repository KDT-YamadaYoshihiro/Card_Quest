#include "../Battle/Battle.h"
#include "../Battle/Calculation/Calculation.h"
#include "../Character/Factory/CharacterFactory.h"
#include "../Character/Factory/CardFactory.h"

// 初期化
Battle::Battle()
    :m_cost(std::make_unique<CostManager>()),
    m_phase(TurnPhase::StartTurn),
    m_turnCount(1)
{
    CreateEntity();
    m_cost->Init(3);

}

void Battle::Init()
{
}

void Battle::Update()
{
    if (CheckWin() || CheckLose()) {
        m_phase = TurnPhase::EndTurn;
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

    case TurnPhase::EndTurn:

        // ターン終了処理
        EndTurn();

        break;

    default:
        break;
    }

}

void Battle::Render(RenderSystem& render)
{


}

void Battle::CreateEntity()
{

    // プレイヤー、カード作成
    for (int i = 1; i < 5; i++) {
        auto player = CharacterFactory::Instance().CreateCharacter<Player>(i);
        m_players.push_back(player);

        const std::vector<int>& cardIds = player->GetStatus().cardIds;
        auto cards = CardFactory::GetInstance().CreateDeck(cardIds);

        CardManager::GetInstance().InitDeck(std::move(cards));
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
    m_players[/*ここに選択カード者指定*/1]->Update();

    

    // 終了時
    // ターン終了ボタンが押されたとき
    m_phase = TurnPhase::EnemyTurn;
}

void Battle::EnemyUpdate()
{
    // エネミーはカードをランダムで選択一回だけアクションを起こして終了
    for (auto& e : m_enemies)
    {
        e->Update();
    }

    // 終了時
    // メンバー全員がアクションを行ったら終了
    m_phase = TurnPhase::EndTurn;
}

void Battle::EndTurn()
{
	// ターン数を増やす
    m_turnCount++;

    // 生存確認後続行か判定
    for (auto& p : m_players) {
        for (auto& e : m_enemies) {
            if (p->GetStatus().dead && e->GetStatus().dead) {

                p->UpdateBuff();
                e->UpdateBuff();

                m_phase = TurnPhase::StartTurn;
            }
        }
    }
}
