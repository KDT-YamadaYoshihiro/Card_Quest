#include "../Battle/BattleSystem.h"
#include "../Battle/Calculation/Calculation.h"
#include "../Character/Factory/CharacterFactory.h"
#include "../Card/CardFactory/CardFactory.h"
#include "../../View/Font/FontManager.h"

// フォントのインスタンス取得省略
#define FontMgr FontManager::GetInstance()

// 初期化
BattleSystem::BattleSystem()
    :m_cost(std::make_unique<CostManager>()),
    m_phase(TurnPhase::StartTurn),
    m_turnCount(1),
    m_choiceCardIndex(0)
{
    CreateEntity();
    m_cost->Init(3);

    m_renderer = std::make_unique<CardRenderer>();

}

// 初期化
void BattleSystem::Init()
{
}

// 更新
void BattleSystem::Update(sf::RenderWindow& arg_window)
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
        PlayerUpdate(arg_window);

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

// 描画
void BattleSystem::Render(sf::RenderWindow& window)
{

    // 山札
    m_renderer->DrawDeck(FontMgr.GetFont(), window, {50.0f,300.0f}, CardManager::GetInstance().GetDeckCount());

    // 墓地
    m_renderer->DrawGrave(FontMgr.GetFont(), window,{150.0f, 300.0f}, CardManager::GetInstance().GetCemeteryCount());

    // 手札
    // ベース座標
    float x = 300.0f;
    float baseY = 300.0f;
    // カード情報の取得
    const auto& handCard = CardManager::GetInstance().GetHandCard();
    // 個々のカード情報
    for (size_t i = 0; i < handCard.size(); i++)
    {
        // 座標の変更
        float y = baseY;

        if(static_cast<int>(i) == m_choiceCardIndex)
        {
            // 選択中カードを浮かす
            y -= 30;
        }
        // 描画
        m_renderer->DrawHand(FontMgr.GetFont(), window, { x, y }, *handCard[i]);
        // 一枚ごとにx座標をずらす
        x += 130.0f;
    }
}

// カードの使用
void BattleSystem::OnUseCard(size_t arg_handIndex,size_t arg_targetIndex)
{
    auto result = CardManager::GetInstance().UseCard(arg_handIndex);
    auto& owner = m_players[result.ownerID];
    auto& target = m_players[arg_targetIndex];
    // フォーカス演出

    // 効果適用
    ApplyCardAction(result,owner,target);
}

// カード使用時の影響
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

// 生成関数
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

// クリック判定
int BattleSystem::GetClickHandIndex(sf::RenderWindow& arrg_window)
{
    // カードのサイズ
    constexpr float CARD_W = 120.0f;
    constexpr float CARD_H = 160.0f;

    // カードの描画開始地点
    float startX = 300.0f;
    float y = 300.0f;

    // マウスの座標
    auto mouse = sf::Mouse::getPosition(arrg_window);
    sf::Vector2f mousePos(static_cast<float>(mouse.x), static_cast<float>(mouse.y));

    // カード情報
    const auto& hand = CardManager::GetInstance().GetHandCard();

    // クリック判定
    for (size_t i = 0; i < hand.size(); i++)
    {
        sf::FloatRect rect({ startX + i * 130.0f, y }, { CARD_W, CARD_H });
    
        if (rect.contains(mousePos))
        {
            // クリックしたインデックスを返す
            return i;
        }

    }

    // どれもクリックされていない
    return -1;

}

// ターン開始時
void BattleSystem::StartTurn()
{
    // 手札補充
    CardManager::GetInstance().DeckToHand(5);

    // コスト回復
    m_cost->ResetCost();

	// フェーズをプレイヤーターンに変更
    m_phase = TurnPhase::PlayerTurn;
}

// プレイヤー更新
void BattleSystem::PlayerUpdate(sf::RenderWindow& arg_window)
{
    // カードを選択
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        int index = GetClickHandIndex(arg_window);

        if (index >= 0)
        {
            m_choiceCardIndex = index;
            // ログ
            std::cout << "クリックしたカード:" << index << std::endl;
        }
    }

    // 決定ボタンを押したとき
    
    //// 選択したカードでプレイヤーがアクション
    //m_players[/*ここに選択カード者指定*/1]->Update();

    //

    //// 終了時
    //// ターン終了ボタンが押されたとき
    //m_phase = TurnPhase::EnemyTurn;
}

// エネミー更新
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

// ターン終了時
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
