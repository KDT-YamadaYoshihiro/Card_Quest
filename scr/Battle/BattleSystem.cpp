#include "../Battle/BattleSystem.h"
#include "../Battle/Calculation/Calculation.h"
#include "../Character/Factory/CharacterFactory.h"
#include "../Card/CardFactory/CardFactory.h"

// 初期化
BattleSystem::BattleSystem()
    :m_cost(std::make_unique<CostManager>()),
    m_phase(TurnPhase::StartTurn),
    m_turnCount(1)
{
    CreateEntity();
    m_cost->Init(3);

    if (!m_font.openFromFile("data/Font/genkai-mincho.ttf"))
    {
        // エラー処理（assert / ログなど）
        throw std::runtime_error("Font load failed");
    }
    m_renderer = std::make_unique<CardRenderer>();

}

void BattleSystem::Init()
{
}

void BattleSystem::Update()
{
    //if (CheckWin() || CheckLose()) {
    //    m_phase = TurnPhase::EndTurn;
    //    return;
    //}

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

void BattleSystem::Render(sf::RenderWindow& window)
{

    // 山札
    m_renderer->DrawDeck(m_font,window, { 50.0f,300.0f }, CardManager::GetInstance().GetDeckCount());

    // 墓地
    m_renderer->DrawGrave(m_font, window,{150.0f, 300.0f}, CardManager::GetInstance().GetCemeteryCount());

    // 手札
    float x = 300.0f;
    for (const auto& card : CardManager::GetInstance().GetHandCard())
    {
        m_renderer->DrawHand(m_font, window, { x, 300.0f }, *card);
        x += 110.0f;
    }
}

void BattleSystem::OnUseCard(size_t arg_handIndex,size_t arg_targetIndex)
{
    auto result = CardManager::GetInstance().UseCard(arg_handIndex);
    auto& owner = m_players[result.ownerID];
    auto& target = m_players[arg_targetIndex];
    // フォーカス演出

    // 効果適用
    ApplyCardAction(result,owner,target);
}

void BattleSystem::ApplyCardAction(const CardUseResult& result, std::shared_ptr<Character> arg_owner, std::shared_ptr<Character> arg_target)
{

    switch (result.effect.actionType)
    {

    case ActionType::ATTCK:
        /*arg_target->TakeDamage();*/
        break;

    case ActionType::MAGIC:

        /*arg_target->TakeDamage();*/

        break;

    case ActionType::HEAL:
        
        /*arg_target->TakeHeal();*/

        break;

    case ActionType::BUFF:

        //arg_target->TakeHeal();

        break;

    default:
        break;
    }

}

void BattleSystem::CreateEntity()
{

    // プレイヤー、カード作成
    for (int i = 1; i < 5; i++) {
        auto player = CharacterFactory::Instance().CreateCharacter<Player>(i);
        m_players.push_back(player);

        const std::vector<int>& cardIds = player->GetStatus().cardIds;
        auto cards = CardFactory::GetInstance().CreateDeck(cardIds);

        if (i == 1) {
            CardManager::GetInstance().InitDeck(std::move(cards));
        }
        else
        {
            CardManager::GetInstance().AddDeckCard(std::move(cards));
        }
    }

}

void BattleSystem::StartTurn()
{
    // 手札補充
    CardManager::GetInstance().DeckToHand(5);

    // コスト回復
    m_cost->ResetCost();

	// フェーズをプレイヤーターンに変更
    m_phase = TurnPhase::PlayerTurn;
}

void BattleSystem::PlayerUpdate()
{
    // カードを選択


    // 選択したカードでプレイヤーがアクション
    m_players[/*ここに選択カード者指定*/1]->Update();

    

    // 終了時
    // ターン終了ボタンが押されたとき
    m_phase = TurnPhase::EnemyTurn;
}

void BattleSystem::EnemyUpdate()
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

void BattleSystem::EndTurn()
{
	// ターン数を増やす
    m_turnCount++;

    // 生存確認後続行か判定
    for (auto& p : m_players) {
        for (auto& e : m_enemies) {
            if (!p->GetStatus().dead && !e->GetStatus().dead) {

                p->UpdateBuff();
                e->UpdateBuff();

                m_phase = TurnPhase::StartTurn;
            }
        }
    }
}
