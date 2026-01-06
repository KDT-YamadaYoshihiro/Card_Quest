#pragma once
#include<SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <vector>
#include <optional>
#include "ActionData.h"
#include "../../Character/Character.h"
#include "../../Card/Card.h"
#include "../SelectTarget/TargetSelect.h"

// プレイヤー選択フェーズ
enum class PlayerSelectPhase
{
    SELECT_CARD,
    SELECT_TARGET,
    CONFIRM
};

class PlayerController
{
public:
    PlayerController(std::shared_ptr<Character> player);

    // 更新
    void Update();
   
    bool HasAction() const;
    Action PopAction();

private:
    // プレイヤー
    std::shared_ptr<Character> m_player;
    // 現在のフェーズ
    PlayerSelectPhase m_phase;
    // 選択カードのindex
    int m_selectedCardIndex = -1;
    // 選択カード
    std::optional<CardData> m_selectedCard;
    // 選択ターゲット
    std::vector<std::shared_ptr<Character>> m_selectedTargets;
    // アクションデータ
    std::optional<Action> m_action;


// 内部処理
    // カード選択
    void SelectCard(sf::RenderWindow& window);
    // ターゲット選択
    void SelectTarget(sf::RenderWindow& window);
    //  クリック判定
    int GetClickHandIndex(sf::RenderWindow& window) const;
    // 
    bool IsDecisionButtonClicked(sf::RenderWindow& window) const;
};
