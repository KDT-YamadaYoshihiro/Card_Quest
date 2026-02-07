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

    int cardIdx = HitTestHandCard(mousePos);
    m_hoveredCardIndex = cardIdx;

    if (InPutMouseManager::GetInstance().IsLeftClicked())
    {
        if (cardIdx != -1)
        {
            // カード選択処理の実行
            SelectCard(cardIdx);
        }
    }
}

void UserController::SelectCard(int cardIdx)
{
    m_selectedCardIndex = cardIdx;
    m_selectCardId = m_context.GetCardIdByGlobalIndex(cardIdx);

    m_selectedActor = m_context.GetCharacterByCardId(m_selectCardId);
    if(!m_selectedActor) {
        ConsoleView::GetInstance().Add("Error: Actor not found for card ID: " + std::to_string(m_selectCardId));
        return;
    }

    auto cardData = CardManager::GetInstance().GetCardData(m_selectCardId);

    UpdateCreateTargets();
	int predctionCost = cardData.actionPlus - 1;
    m_context.SetPredictedCost(predctionCost);
    m_context.ClearFocusTargets();

    if (!m_targetCandidates.empty())
    {
        // ターゲットタイプによるフォーカス分岐
        if (cardData.targetType == TargetType::ALLY_ALL || cardData.targetType == TargetType::OPPONENT_ALL) // 全体対象の場合
        {
            // 全員をフォーカス
            m_context.SetFocusTargets(m_targetCandidates);
            
            m_preSelectedTarget = nullptr;
        }
        else // 単体対象の場合（デフォルト）
        {
            // 0番目をデフォルトフォーカス
            m_preSelectedTarget = m_targetCandidates[0];
            m_context.SetFocusTargets({ m_preSelectedTarget });
        }
    }

    m_phase = PlayerSelectPhase::SELECT_TARGET;
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
    sf::Vector2f mousePos = GetScreenMousePos(window);

    // --- 追加仕様：ターゲット選択中もカードのクリックをチェック ---
    int cardIdx = HitTestHandCard(mousePos);
    if (InPutMouseManager::GetInstance().IsLeftClicked() && cardIdx != -1)
    {
        // 別のカード（または同じカード）がクリックされたら、カード選択をやり直す
        SelectCard(cardIdx);
        return; // 以降のターゲット判定はスキップ
    }

    int targetIdx = HitTestCharacter(mousePos, m_targetCandidates);
    m_hoveredTarget = (targetIdx != -1) ? m_targetCandidates[targetIdx] : nullptr;

    if (InPutMouseManager::GetInstance().IsLeftClicked())
    {
        if (targetIdx != -1)
        {
            auto clickedTarget = m_targetCandidates[targetIdx];
            auto cardData = CardManager::GetInstance().GetCardData(m_selectCardId);

            // --- 仕様変更：全体攻撃と単体攻撃で決定ロジックを分ける ---
            if (cardData.targetType == TargetType::ALLY_ALL || cardData.targetType == TargetType::OPPONENT_ALL)
            {
                // 全員ターゲット時
                ConfirmAction(m_targetCandidates);
            }
            else if (m_preSelectedTarget == clickedTarget)
            {
                // 2回目：決定
                std::vector<std::shared_ptr<Character>> targets = { clickedTarget };
                ConfirmAction(targets);
            }
            else
            {
                // 1回目：フォーカス更新
                m_preSelectedTarget = clickedTarget;
                m_context.ClearFocusTargets();
                m_context.SetFocusTargets({ m_preSelectedTarget });
            }
        }
    }

}

void UserController::ConfirmAction(const std::vector<std::shared_ptr<Character>>& targets)
{
    UserAction action;
    action.actor = m_selectedActor;
    action.cardId = m_selectCardId;
    action.targets = targets;
    m_confirmedAction = action;

    m_context.ClearFocusTargets();
    m_context.SetPredictedCost(0);
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
