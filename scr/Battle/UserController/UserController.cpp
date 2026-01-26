#include "UserController.h"
#include "../../System/InPutManager/InPutMouseManager.h"
#include "../BattleContex/BattleContext.h"
#include "../BattleView/BattleView.h"
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

UserController::UserController(BattleContext& context,BattleView& battleView)
    : m_context(context),
    m_battleView(battleView)
{
}

void UserController::Update(sf::RenderWindow& window)
{
    InPutMouseManager::GetInstance().Update(window);

    switch (m_phase)
    {
    case PlayerSelectPhase::SELECT_PLAYER:
        std::cout << "キャラクター選択中" << std::endl;
        UpdateSelectPlayer(window);
        break;
    case PlayerSelectPhase::SELECT_CARD:
        std::cout << "カード選択中" << std::endl;
        UpdateSelectCard(window);
        break;
    case PlayerSelectPhase::CREATE_TARGETS:
        std::cout << "ターゲット候補作成" << std::endl;
        UpdateCreateTargets();
        break;
    case PlayerSelectPhase::SELECT_TARGET:
        std::cout << "ターゲット選択中" << std::endl;
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
    m_phase = PlayerSelectPhase::SELECT_PLAYER;
    m_selectedActor.reset();
    m_selectedCardIndex = -1;
    m_selectedTargets.clear();
    m_targetCandidates.clear();

    return result;
}

// ================= 選択処理 =================

void UserController::UpdateSelectPlayer(sf::RenderWindow& window)
{
    const auto& players = m_context.GetPlayers();
    UpdateCharacterRects(players);

    if (!InPutMouseManager::GetInstance().IsLeftClicked())
    {
        return;
    }
    int index = HitTestCharacter(GetMousePos(window), players);
    if (index < 0)
    {
        return;
    }

    m_selectedActor = players[index];
    m_battleView.SetSelectedActor(players[index]);
    std::cout << "選択キャラ:" << players[index]->GetData().name << std::endl;
    m_phase = PlayerSelectPhase::SELECT_CARD;
}

void UserController::UpdateSelectCard(sf::RenderWindow& window)
{
    
    UpdateHandCardRects(*m_selectedActor);

    if (!InPutMouseManager::GetInstance().IsLeftClicked())
    {
        return;
    }
    int index = HitTestHandCard(GetMousePos(window));

    if (index < 0)
    {
        return;
    }

    m_selectedCardIndex = index;
    m_selectCardId = m_selectedActor->GetHeldCardId(m_selectedCardIndex);
    m_battleView.SetSelectedCard(m_selectCardId);
    std::cout << "選択カード:" << m_selectCardId << std::endl;
    m_phase = PlayerSelectPhase::CREATE_TARGETS;
}

void UserController::UpdateCreateTargets()
{
    // カードID、情報の取得
    const CardData& card =CardManager::GetInstance().GetCardData(m_selectCardId);

    // ターゲット候補の取得
    m_targetCandidates = m_context.CreateTargetCandidates(card.targetType,m_selectedActor->GetFaction(),m_selectedActor);

    m_selectedTargets.clear();

    if (card.targetType == TargetType::ALLY_ALL || card.targetType == TargetType::OPPONENT_ALL)
    {

        UserAction action;
        action.actor = m_selectedActor;
        action.cardId = m_selectCardId;
        action.targets = m_targetCandidates;

        m_confirmedAction = action;
        m_battleView.SetTargetIndices(m_targetCandidates);
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

    if (!InPutMouseManager::GetInstance().IsLeftClicked())
    {
        return;
    }

    int index = HitTestCharacter(GetMousePos(window), m_targetCandidates);
    if (index < 0)
    {
        return;
    }

    m_selectedTargets.push_back(m_targetCandidates[index]);
    m_phase = PlayerSelectPhase::CONFIRM;

    const CardData& card = CardManager::GetInstance().GetCardData(m_selectCardId);

    // 確定
    UserAction action;
    action.actor = m_selectedActor;
    action.cardId = m_selectCardId;

    if (card.targetType == TargetType::ALLY_ALL || card.targetType == TargetType::OPPONENT_ALL)
    {
        action.targets = m_targetCandidates;
    }
    else
    {
        action.targets = m_targetCandidates;
    }

    m_selectedTargets = m_targetCandidates;
    m_battleView.SetTargetIndices(action.targets);
    m_battleView.SetSelectedCard(m_selectCardId);
    m_confirmedAction = action;
    m_phase = PlayerSelectPhase::DONE;
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
    for (size_t i = 0; i < m_handCardRects.size(); ++i)
    {
        if (m_handCardRects[i].contains(mousePos))
        {
            return static_cast<int>(i);
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
        sf::Vector2f pos = c->GetPosition();
        m_characterRects.emplace_back(sf::Vector2f{ pos.x - CHAR_W * 0.5f,pos.y - CHAR_H * 0.5f }, sf::Vector2f{ CHAR_W, CHAR_H });
    }
}

/// <summary>
/// カードクリック判定
/// </summary>
/// <param name="actor"></param>
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

/// <summary>
/// マウス座標
/// </summary>
/// <param name="window"></param>
/// <returns></returns>
sf::Vector2f UserController::GetMousePos(sf::RenderWindow& window) const
{
    auto pos = sf::Mouse::getPosition(window);
    return { static_cast<float>(pos.x), static_cast<float>(pos.y) };
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
