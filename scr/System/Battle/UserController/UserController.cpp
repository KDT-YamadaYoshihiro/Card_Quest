#include "UserController.h"
#include "System/InPutManager/InPutMouseManager.h"
#include "System/Battle/BattleContex/BattleContext.h"
#include "View/BattleView/BattleView.h"
#include "Entity/Character/Character.h"
#include "Entity/Card/Card.h"
#include "Entity/Card/CardManager/CardManager.h"
#include "View/ConsoleView/ConsoleView.h"
namespace
{
    constexpr float CHAR_W = 165.f;
    constexpr float CHAR_H = 150.f;

    constexpr float CARD_W = 120.f;
    constexpr float CARD_H = 160.f;

    constexpr sf::Vector2f HAND_START{ 200.f, 520.f };
    constexpr float HAND_SPACING = 180.f;
}

UserController::UserController(BattleContext& context,BattleView& battleView)
    : m_context(context),
    m_battleView(battleView)
{
}

void UserController::Update(sf::RenderWindow& window)
{

    sf::Vector2f mousePos = this->GetScreenMousePos(window);
    //sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window), window.getDefaultView());


    switch (m_phase)
    {
    case PlayerSelectPhase::SELECT_CARD:

        ConsoleView::GetInstance().Add("カード選択中\n");
        // 座標系の更新
        UpdateHandCardRects();
        // 選択更新
        UpdateSelectCard(window, mousePos);
        break;
    case PlayerSelectPhase::CREATE_TARGETS:
        ConsoleView::GetInstance().Add("ターゲット候補作成\n");
        // 候補作成
        UpdateCreateTargets();
        break;
    case PlayerSelectPhase::SELECT_TARGET:
        ConsoleView::GetInstance().Add("ターゲット選択中\n");
        // 座標系の更新
        UpdateCharacterRects(m_targetCandidates);
        // 選択更新
        UpdateSelectTarget(window);
        break;
    case PlayerSelectPhase::DONE:
        return;
        break;
    }
}

bool UserController::HasConfirmedAction() const
{
    return m_confirmedAction.has_value();
}

UserAction UserController::ConsumeAction()
{
    UserAction result = *m_confirmedAction;
    m_confirmedAction.reset();

    // 次の入力に備えてリセット
    m_phase = PlayerSelectPhase::SELECT_CARD;
    m_selectedActor.reset();
    m_selectedCardIndex = -1;
    m_selectedTargets.clear();
    m_targetCandidates.clear();

    return result;
}

// ================= 選択処理 =================


/// <summary>
/// カード選択
/// </summary>
/// <param name="window"></param>
/// <param name="mousePos"></param>
void UserController::UpdateSelectCard(sf::RenderWindow& window, const sf::Vector2f& mousePos)
{
    
    m_hoveredCardIndex = HitTestHandCard(mousePos);

    if (InPutMouseManager::GetInstance().IsLeftClicked()) {
        if (m_hoveredCardIndex == -1) {
            // indexが空なら何もしない
            return;
        }
        // 同じカードを2回クリック、または既に選択中なら確定
        if (m_preSelectedCardIndex == m_hoveredCardIndex) {
            // インデックの同期
            m_selectedCardIndex = m_hoveredCardIndex;
            //  UIインデックスからカードIDを特定
            int cardId = m_context.GetCardIdByGlobalIndex(m_selectedCardIndex);
            // カードIDから行動者を特定
            m_selectedActor = m_context.GetCharacterByCardId(cardId);
            if (m_selectedActor) 
            {
                m_selectCardId = cardId;
                
                m_selectedCardIndex = m_context.GetLocalCardIndex(m_selectedActor, cardId);

                m_battleView.SetSelectedCard(m_selectCardId);
                ConsoleView::GetInstance().Add("選択カード" + std::to_string(m_selectCardId) +" 使用者:" + m_selectedActor->GetData().name + "\n");

                m_phase = PlayerSelectPhase::CREATE_TARGETS;
            }
            m_preSelectedCardIndex = -1;
        }
        else {
            //　選択状態にする
            m_preSelectedCardIndex = m_hoveredCardIndex;
        }
        
    }
}

/// <summary>
/// ターゲット候補の取得
/// </summary>
void UserController::UpdateCreateTargets()
{
    // カードID、情報の取得
    const CardData& card = CardManager::GetInstance().GetCardData(m_selectCardId);

    // ターゲット候補の取得
    m_targetCandidates = m_context.CreateTargetCandidates(card.targetType,m_selectedActor->GetFaction(),m_selectedActor);

    // ターゲットが空でないか確認
    if (m_targetCandidates.empty()) {
        m_phase = PlayerSelectPhase::SELECT_CARD;
        return;
    }

    // ターゲット選択フェーズ移行時の初期設定
    if (card.targetType == TargetType::OPPONENT_ALL || card.targetType == TargetType::ALLY_ALL) {
        // 全員をフォーカス
        m_context.SetFocusTargets(m_targetCandidates);
    }
    else {
        // デフォで0番目を仮選択・フォーカス
        m_preSelectedTarget = m_targetCandidates[0];
        m_context.SetFocusTargets({ m_preSelectedTarget });
    }

    // Focus表示
    m_context.SetFocusDraw(true);

    m_phase = PlayerSelectPhase::SELECT_TARGET;
}

/// <summary>
/// ターゲット選択
/// </summary>
/// <param name="window"></param>
void UserController::UpdateSelectTarget(sf::RenderWindow& window)
{
    // 現在のView設定に基づいたワールド座標を取得
    sf::Vector2f worldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    UpdateCharacterRects(m_targetCandidates);

    const auto& cardData = m_selectedActor->GetCardData(m_selectedCardIndex);
    bool isAreaAttack = (cardData.targetType == TargetType::OPPONENT_ALL || cardData.targetType == TargetType::ALLY_ALL);

    // マウス直下のキャラ判定
    int hoverIdx = HitTestCharacter(worldPos, m_targetCandidates);
    std::shared_ptr<Character> hovered = (hoverIdx != -1) ? m_targetCandidates[hoverIdx] : nullptr;

    // クリック処理
    if (InPutMouseManager::GetInstance().IsLeftClicked()) {

        bool isDone = false;

        if (isAreaAttack) {
            // 全体系：誰かをクリックすれば確定
            if (hovered) {
                m_selectedTargets = m_targetCandidates; // 全員をターゲットに
                isDone = true;
            }
        }
        else {
            // 単体系：2クリック確定ロジック
            if (hovered) {
                if (m_preSelectedTarget == hovered) {
                    m_selectedTargets.push_back(m_preSelectedTarget);
                    isDone = true;
                }
                else {
                    m_preSelectedTarget = hovered;
                    // フォーカス対象を更新
                    m_context.SetFocusTargets({ m_preSelectedTarget });
                }
            }
        }

        if (isDone) {
            // --- 行動情報の確定 ---
            UserAction action;
            action.actor = m_selectedActor;      // 特定済みの使用者
            action.cardId = m_selectCardId;      // 特定済みのカードID
            action.targets = m_selectedTargets;  // 決定したターゲットリスト

            m_confirmedAction = action;          // optionalに値をセット

            // Focus表示
            m_context.SetFocusDraw(false);

            m_phase = PlayerSelectPhase::DONE;
        }

    }

}

/// <summary>
/// キャラクタークリック判定
/// </summary>
/// <param name="mousePos"></param>
/// <param name="list"></param>
/// <returns></returns>
int UserController::HitTestCharacter(
    const sf::Vector2f& mousePos,
    const std::vector<std::shared_ptr<Character>>& list) const
{
    for (size_t i = 0; i < m_characterRects.size(); ++i)
    {
        if (m_characterRects[i].contains(mousePos))
        {
            return static_cast<int>(i);
        }
    }
    return -1;
}

/// <summary>
/// カードクリック判定
/// </summary>
/// <param name="mousePos"></param>
/// <returns></returns>
int UserController::HitTestHandCard(const sf::Vector2f& mousePos) const
{
    for (int i = 0; i < m_handCardRects.size(); ++i)
    {
        if (m_handCardRects[i].contains(mousePos))
        {
            return i; 
        }
    }
    return -1;
}

/// <summary>
/// キャラクタークリック範囲
/// </summary>
/// <param name="list"></param>
void UserController::UpdateCharacterRects(
    const std::vector<std::shared_ptr<Character>>& list)
{
    m_characterRects.clear();

    for (auto& c : list)
    {
        if (!c)
        {
            continue;
        }

        sf::Vector2f pos = c->GetPosition();

        constexpr float SPRITE_W = 165.f; 
        constexpr float SPRITE_H = 150.f;

        m_characterRects.emplace_back(sf::FloatRect({ pos.x,pos.y }, { SPRITE_W, SPRITE_H }));
    }
}

/// <summary>
/// カードクリック判定
/// </summary>
/// <param name="actor"></param>
void UserController::UpdateHandCardRects()
{
    m_handCardRects.clear();

    int cardCount = 0;

    for (auto& p : m_context.GetAlivePlayers())
    {
        cardCount += p->GetCardCount();
    }

    for (int i = 0; i < cardCount; ++i)
    {
        // 描画(BattleView)と同じ計算式でRectを作成
        sf::Vector2f pos = HAND_START;
        pos.x += i * HAND_SPACING;

        // カードの矩形を登録
        m_handCardRects.push_back(sf::FloatRect({ pos.x, pos.y }, { CARD_W, CARD_H }));
    }
}

// ================= 補助 =================

/// <summary>
/// マウスの「ワールド座標」を取得（キャラクター判定用）
/// </summary>
sf::Vector2f UserController::GetWorldMousePos(sf::RenderWindow& window) const
{
    // 現在のウィンドウ内のマウスピクセル位置を取得
    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);

    // 現在のカメラ（View）の設定に基づいて、ワールド座標に変換
    // これにより、ズームやスクロールの状態が反映された座標が返ります
    return window.mapPixelToCoords(pixelPos);
}

/// <summary>
/// マウスの「スクリーン座標」を取得（UI/カード判定用）
/// </summary>
sf::Vector2f UserController::GetScreenMousePos(sf::RenderWindow& window) const
{
    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);

    // UIはカメラの影響を受けないため、デフォルトView（画面そのままの座標）で変換
    return window.mapPixelToCoords(pixelPos, window.getDefaultView());
}
// デバッグ用描画
void UserController::DrawDebug(sf::RenderWindow& window)
{
    sf::RectangleShape r;
    r.setFillColor(sf::Color::Transparent);
    r.setOutlineColor(sf::Color::Green);
    r.setOutlineThickness(2.f);

    for (auto& rect : m_characterRects)
    {
        r.setPosition({ rect.position.x, rect.position.y });
        r.setSize({ rect.size.x, rect.size.y });
        window.draw(r);
    }

    r.setOutlineColor(sf::Color::Blue);
    for (auto& rect : m_handCardRects)
    {
        r.setPosition({ rect.position.x, rect.position.y });
        r.setSize({ rect.size.x, rect.size.y });
        window.draw(r);
    }
}

PlayerSelectPhase UserController::GetSelectPhase() const
{
    return m_phase;
}

std::shared_ptr<Character> UserController::GetSelectActor() const
{
    return m_selectedActor;
}

int UserController::GetSelectCardId() const
{
    return m_selectCardId;
}


const std::vector<std::shared_ptr<Character>>& UserController::GetSelectTargetIndices() const
{
    return m_selectedTargets;
}
