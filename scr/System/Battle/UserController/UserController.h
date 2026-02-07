#pragma once
#include <vector>
#include <optional>
#include "ActionData.h"
#include "Entity/Card/CardDate.h"
#include "System/Battle/BattleContex/BattleContext.h"

class BattleContext;
class BattleView;
class Character;

class UserController
{

private:
    BattleContext& m_context;
    BattleView& m_battleView;

    PlayerSelectPhase m_phase = PlayerSelectPhase::SELECT_CARD;

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

    // ダブルクリック判定用の仮選択状態
    int m_preSelectedCardIndex = -1;
    std::shared_ptr<Character> m_preSelectedTarget = nullptr;

    // マウスホバー中の対象（描画用）
    int m_hoveredCardIndex = -1;
    std::shared_ptr<Character> m_hoveredTarget = nullptr;

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
    int GetHoveredCardIndex() const { return m_hoveredCardIndex; }
    std::shared_ptr<Character> GetHoveredTarget() const { return m_hoveredTarget; }


private:
    // ===== 内部更新 =====
    void UpdateSelectCard(sf::RenderWindow& window , const sf::Vector2f& mousePos);
    void SelectCard(int cardIdx);
    void UpdateCreateTargets();
    void UpdateSelectTarget(sf::RenderWindow& window);
    void ConfirmAction(const std::vector<std::shared_ptr<Character>>& targets);

    // ===== HitTest =====
    int HitTestCharacter(const sf::Vector2f& mousePos,
        const std::vector<std::shared_ptr<Character>>& list) const;

    int HitTestHandCard(const sf::Vector2f& mousePos) const;

    // ===== Rect 更新 =====
    void UpdateCharacterRects(const std::vector<std::shared_ptr<Character>>& list);
    void UpdateHandCardRects();

    // ===== 補助 =====
    sf::Vector2f GetWorldMousePos(sf::RenderWindow& window) const;
    sf::Vector2f GetScreenMousePos(sf::RenderWindow& window) const;

};