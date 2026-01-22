#pragma once
#include <vector>
#include <optional>
#include "ActionData.h"
#include "../../Card/CardDate.h"

class BattleContext;
class Character;

class UserController
{
public:

    UserController(BattleContext& context);

    void Update(sf::RenderWindow& window);
    bool HasConfirmedAction() const;
    UserAction ConsumeAction();

    void DrawDebug(sf::RenderWindow& window);

private:
    // ===== 内部更新 =====
    void UpdateSelectPlayer(sf::RenderWindow& window);
    void UpdateSelectCard(sf::RenderWindow& window);
    void UpdateCreateTargets();
    void UpdateSelectTarget(sf::RenderWindow& window);

    // ===== HitTest =====
    int HitTestCharacter(const sf::Vector2f& mousePos,
        const std::vector<std::shared_ptr<Character>>& list) const;

    int HitTestHandCard(const sf::Vector2f& mousePos) const;

    // ===== Rect 更新 =====
    void UpdateCharacterRects(const std::vector<std::shared_ptr<Character>>& list);
    void UpdateHandCardRects(const Character& actor);

    // ===== 補助 =====
    sf::Vector2f GetMousePos(sf::RenderWindow& window) const;

private:
    BattleContext& m_context;

    PlayerSelectPhase m_phase = PlayerSelectPhase::SELECT_PLAYER;

    // Rect 管理
    std::vector<sf::FloatRect> m_characterRects;
    std::vector<sf::FloatRect> m_handCardRects;

    // 選択状態
    std::shared_ptr<Character> m_selectedActor;
    int m_selectedCardIndex = -1;
    std::vector<std::shared_ptr<Character>> m_targetCandidates;
    std::vector<int> m_selectedTargetIndices;

    // 確定行動
    std::optional<UserAction> m_confirmedAction;
};