#include "UserController.h"
#include "../../System/InPutManager/InPutManager.h"
#include "../BattleContex/BattleContext.h"
#include "../../Character/Character.h"
#include "../../Card/Card.h"
#include "../../Card/CardManager/CardManager.h"

namespace
{
    constexpr float CHAR_W = 165.f;
    constexpr float CHAR_H = 150.f;

    constexpr float CARD_W = 120.f;
    constexpr float CARD_H = 160.f;

    constexpr sf::Vector2f HAND_START{ 300.f, 520.f };
    constexpr float HAND_SPACING = 140.f;
}

UserController::UserController(BattleContext& context)
    : m_context(context)
{
}

void UserController::Update(sf::RenderWindow& window)
{
    InputManager::GetInstance().Update(window);

    switch (m_phase)
    {
    case PlayerSelectPhase::SELECT_PLAYER:
        UpdateSelectPlayer(window);
        break;
    case PlayerSelectPhase::SELECT_CARD:
        UpdateSelectCard(window);
        break;
    case PlayerSelectPhase::CREATE_TARGETS:
        UpdateCreateTargets();
        break;
    case PlayerSelectPhase::SELECT_TARGET:
        UpdateSelectTarget(window);
        break;
    case PlayerSelectPhase::DONE:
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
    m_phase = PlayerSelectPhase::SELECT_PLAYER;
    m_selectedActor.reset();
    m_selectedCardIndex = -1;
    m_selectedTargetIndices.clear();
    m_targetCandidates.clear();

    return result;
}

// ================= 選択処理 =================

void UserController::UpdateSelectPlayer(sf::RenderWindow& window)
{
    const auto& players = m_context.GetPlayers();
    UpdateCharacterRects(players);

    if (!InputManager::GetInstance().IsLeftClicked())
        return;

    int index = HitTestCharacter(GetMousePos(window), players);
    if (index < 0) return;

    m_selectedActor = players[index];
    m_phase = PlayerSelectPhase::SELECT_CARD;
}

void UserController::UpdateSelectCard(sf::RenderWindow& window)
{
    
    UpdateHandCardRects(*m_selectedActor);

    if (!InputManager::GetInstance().IsLeftClicked())
    {
        return;
    }
    int index = HitTestHandCard(GetMousePos(window));

    if (index < 0)
    {
        return;
    }

    m_selectedCardIndex = index;
    m_phase = PlayerSelectPhase::CREATE_TARGETS;
}

void UserController::UpdateCreateTargets()
{
    // カードID、情報の取得
    int cardId = m_selectedActor->GetHeldCardId(m_selectedCardIndex);
    const CardData& card =CardManager::GetInstance().GetCardData(cardId);

    // ターゲット候補の取得
    m_targetCandidates = m_context.CreateTargetCandidates(card.targetType,m_selectedActor->GetFaction(),m_selectedActor);

    m_selectedTargetIndices.clear();

    if (card.targetType == TargetType::ALLY_ALL || card.targetType == TargetType::OPPONENT_ALL)
    {
        for (size_t i = 0; i < m_targetCandidates.size(); ++i)
        {
            m_selectedTargetIndices.push_back(static_cast<int>(i));
        }

        UserAction action;
        action.actor = m_selectedActor;
        action.cardId = cardId;
        action.targetIds = m_selectedTargetIndices;

        m_confirmedAction = action;
        m_phase = PlayerSelectPhase::DONE;
    }
    else
    {
        m_phase = PlayerSelectPhase::SELECT_TARGET;
    }
}

void UserController::UpdateSelectTarget(sf::RenderWindow& window)
{
    UpdateCharacterRects(m_targetCandidates);

    if (!InputManager::GetInstance().IsLeftClicked())
    {
        return;
    }

    int index = HitTestCharacter(GetMousePos(window), m_targetCandidates);
    if (index < 0)
    {
        return;
    }

    m_selectedTargetIndices.push_back(index);
    m_phase = PlayerSelectPhase::CONFIRM;

    int cardId = m_selectedActor->GetHeldCardId(m_selectedCardIndex);

    // 確定
    UserAction action;
    action.actor = m_selectedActor;
    action.cardId = cardId;
    action.targetIds = m_selectedTargetIndices;

    m_confirmedAction = action;
    m_phase = PlayerSelectPhase::DONE;
}

// ================= HitTest =================

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

int UserController::HitTestHandCard(const sf::Vector2f& mousePos) const
{
    for (size_t i = 0; i < m_handCardRects.size(); ++i)
    {
        if (m_handCardRects[i].contains(mousePos))
        {
            return static_cast<int>(i);
        }
    }
    return -1;
}

// ================= Rect 更新 =================

void UserController::UpdateCharacterRects(
    const std::vector<std::shared_ptr<Character>>& list)
{
    m_characterRects.clear();

    for (auto& c : list)
    {
        sf::Vector2f pos = c->GetPosition();
        m_characterRects.emplace_back(pos.x - CHAR_W * 0.5f,pos.y - CHAR_H * 0.5f,CHAR_W,CHAR_H);
    }
}

void UserController::UpdateHandCardRects(const Character& actor)
{
    m_handCardRects.clear();

    const int cardCount = actor.GetCardCount();

    const float cardWidth = 120.f;
    const float cardHeight = 160.f;
    const float spacing = 20.f;

    sf::Vector2f startPos(300.f, 500.f);

    for (int i = 0; i < cardCount; ++i)
    {
        sf::FloatRect rect(
            sf::Vector2f(startPos.x + i * (cardWidth + spacing),startPos.y),
            sf::Vector2f(cardWidth, cardHeight)
        );

        m_handCardRects.push_back(rect);
    }
}

// ================= 補助 =================

sf::Vector2f UserController::GetMousePos(sf::RenderWindow& window) const
{
    auto pos = sf::Mouse::getPosition(window);
    return { static_cast<float>(pos.x), static_cast<float>(pos.y) };
}

// デバッグ用（必要なければ削除可）
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