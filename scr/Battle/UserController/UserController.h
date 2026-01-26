#pragma once
#include <vector>
#include <optional>
#include "ActionData.h"
#include "../../Card/CardDate.h"

class BattleContext;
class BattleView;
class Character;

class UserController
{

private:
    BattleContext& m_context;
    BattleView& m_battleView;

    PlayerSelectPhase m_phase = PlayerSelectPhase::SELECT_PLAYER;

    // Rect 管理
    std::vector<sf::FloatRect> m_characterRects;
    std::vector<sf::FloatRect> m_handCardRects;

    // 選択状態
    std::shared_ptr<Character> m_selectedActor;
    int m_selectedCardIndex = -1;
    int m_selectCardId = -1;
    std::vector<std::shared_ptr<Character>> m_targetCandidates;
    std::vector<std::shared_ptr<Character>> m_selectedTargets;

    // 確定行動
    std::optional<UserAction> m_confirmedAction;

public:

    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name="context"></param>
    /// <param name="battleView"></param>
    UserController(BattleContext& context, BattleView& battleView);

    /// <summary>
    /// 更新
    /// </summary>
    /// <param name="window"></param>
    void Update(sf::RenderWindow& window);

    /// <summary>
    /// 操作が確定しているか確認
    /// </summary>
    /// <returns></returns>
    bool HasConfirmedAction() const;
    /// <summary>
    /// データのリセット
    /// </summary>
    /// <returns></returns>
    UserAction ConsumeAction();
    
    /// <summary>
    /// 
    /// </summary>
    /// <param name="window"></param>
    void DrawDebug(sf::RenderWindow& window);

    // 取得系
    PlayerSelectPhase GetSelectPhase() const;
    std::shared_ptr<Character> GetSelectActor() const;
    int GetSelectCardId() const;
    const std::vector<std::shared_ptr<Character>>& GetSelectTargetIndices() const;

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

};