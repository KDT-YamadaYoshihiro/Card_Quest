#include "PlayerController.h"
#include "../../Card/CardManager/CardManager.h"

PlayerController::PlayerController(std::shared_ptr<Character> player)
	:m_player(player),
	m_phase(PlayerSelectPhase::SELECT_CARD)
{
}


void PlayerController::Update()
{
    switch (m_phase)
    {
    case PlayerSelectPhase::SELECT_CARD:
        SelectCard();
        break;

    case PlayerSelectPhase::SELECT_TARGET:
        SelectTarget();
        break;

    case PlayerSelectPhase::CONFIRM:
        m_action = Action{
            m_player,
            m_selectedTargets,
            *m_selectedCard
        };
        break;
    }
}

bool PlayerController::HasAction() const
{
    return m_action.has_value();
}

Action PlayerController::PopAction()
{
    Action act = *m_action;
    m_action.reset();

    m_selectedTargets.clear();
    m_selectedCard.reset();
    m_phase = PlayerSelectPhase::SELECT_CARD;

    return act;
}

void PlayerController::SelectCard(sf::RenderWindow& window)
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

void PlayerController::SelectTarget(sf::RenderWindow& window)
{
	// 次のフェーズへ
    m_phase = PlayerSelectPhase::CONFIRM;
}

int PlayerController::GetClickHandIndex(sf::RenderWindow& window) const
{
    constexpr float CARD_W = 120.0f;
    constexpr float CARD_H = 160.0f;

    float startX = 300.0f;
    float y = 300.0f;

    auto mouse = sf::Mouse::getPosition(window);
    sf::Vector2f mousePos(static_cast<float>(mouse.x), static_cast<float>(mouse.y));

    const auto& hand = CardManager::GetInstance().GetHandCard();

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


bool PlayerController::IsDecisionButtonClicked(sf::RenderWindow& window) const
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
