#include "UserController.h"
#include "../../Card/CardManager/CardManager.h"

UserController::UserController()
	:m_phase(PlayerSelectPhase::SELECT_CARD)
{
}

// アクションキャラのセット
void UserController::SetActionCharacter(const std::shared_ptr<Character>& character)
{
    m_actionCharacter = character;
}

// ターゲット候補のセット
void UserController::SetTargetCandidates(const std::vector<std::shared_ptr<Character>>& targets)
{
    m_targetCandidates = targets;
}

// 更新
void UserController::Update(sf::RenderWindow& window)
{
    switch (m_phase)
    {
    case PlayerSelectPhase::SELECT_CARD:
        SelectCard(window);
        break;

    case PlayerSelectPhase::SELECT_TARGET:
        SelectTarget(window);
        break;

    case PlayerSelectPhase::CONFIRM:
        // アクション作成
        m_action = Action{
            m_actionCharacter,
            m_selectedTargets,
            *m_selectedCard,
        };
        break;
    }
}

// アクションがあるか
bool UserController::HasAction() const
{
    return m_action.has_value();
}

// アクションを取り出す
Action UserController::PopAction()
{
    Action act = *m_action;
    m_action.reset();

    m_selectedTargets.clear();
    m_selectedCard.reset();
    m_phase = PlayerSelectPhase::SELECT_CARD;

    return act;
}

// カード選択
void UserController::SelectCard(sf::RenderWindow& window)
{
    // 左クリック
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        int index = GetClickHandIndex(window);

        if (index >= 0)
        {
            m_selectedCardIndex = index;

            const auto& hand = CardManager::GetInstance().GetHandCard();
            m_selectedCard = hand[index]->GetCardState();

            // デバッグ
            std::cout << "選択中カード index: " << index << std::endl;
        }
    }

    // 決定ボタン
    if (IsDecisionButtonClicked(window) && m_selectedCard.has_value())
    {
        m_phase = PlayerSelectPhase::SELECT_TARGET;
    }
}

// ターゲット選択
void UserController::SelectTarget(sf::RenderWindow& window)
{

    // --- 単体選択 ---
    if (m_selectedCard->targetType == TargetType::OPPONENT ||
        m_selectedCard->targetType == TargetType::ALLY)
    {
        auto target = TargetSelect::SelectSingle(m_targetCandidates, window);

        if (target)
        {
            m_selectedTargets.clear();
            m_selectedTargets.push_back(target);

            m_phase = PlayerSelectPhase::CONFIRM;
        }
    }
    // --- 全体 ---
    else if (m_selectedCard->targetType == TargetType::OPPONENT_ALL ||
        m_selectedCard->targetType == TargetType::ALLY_ALL)
    {
        // 誰かをクリックしたら確定、という仕様
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            m_selectedTargets =
                TargetSelect::SelectAll(m_targetCandidates);

            m_phase = PlayerSelectPhase::CONFIRM;
        }
    }
    // --- 自分 ---
    else if (m_selectedCard->targetType == TargetType::SELF)
    {
        auto self =
            TargetSelect::SelectSelf(m_player);

        if (self)
        {
            m_selectedTargets.clear();
            m_selectedTargets.push_back(self);

            m_phase = PlayerSelectPhase::CONFIRM;
        }
    }
}

// クリックされた手札のインデックス取得
int UserController::GetClickHandIndex(sf::RenderWindow& window) const
{
	// カードサイズ
    constexpr float CARD_W = 120.0f;
    constexpr float CARD_H = 160.0f;

	// 手札のベース座標
    float startX = 300.0f;
    float y = 300.0f;

	// マウス座標取得
    auto mouse = sf::Mouse::getPosition(window);
    sf::Vector2f mousePos(static_cast<float>(mouse.x), static_cast<float>(mouse.y));

	// 手札取得
    const auto& hand = CardManager::GetInstance().GetHandCard();

	// 各カードと当たり判定
    for (size_t i = 0; i < hand.size(); i++)
    {
        sf::FloatRect rect(
            { startX + i * 130.0f, y },
            { CARD_W, CARD_H }
        );

        if (rect.contains(mousePos))
        {
            return static_cast<int>(i);
        }
    }

    return -1;
}

// 決定ボタンがクリックされたか
bool UserController::IsDecisionButtonClicked(sf::RenderWindow& window) const
{
    constexpr sf::FloatRect decisionButton(
        { 800.f, 500.f },   // 座標
        { 120.f, 50.f }     // サイズ
    );

    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        auto mouse = sf::Mouse::getPosition(window);
        sf::Vector2f pos(static_cast<float>(mouse.x), static_cast<float>(mouse.y));
        return decisionButton.contains(pos);
    }
    return false;
}
