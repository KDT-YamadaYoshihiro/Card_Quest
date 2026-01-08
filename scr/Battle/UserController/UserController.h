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

class UserController
{
public:

    // コンストラクタ
    UserController();

    // アクションキャラのセット
    void SetActionCharacter(const std::shared_ptr<Character>& character);
    // ターゲット候補のセット
    void SetTargetCandidates(const std::vector<std::shared_ptr<Character>>& targets);
    // 選択カードインデックスの取得
	int GetSelectedCardIndex() const { return m_selectedCardIndex; }
    // 更新
    void Update(sf::RenderWindow& window);
    // 
    bool HasAction() const;
    Action PopAction();
    // ターン終了ボタン用
    bool IsTurnEndRequested() const;
    void ResetTurnEndRequest();
private:
    // 現在のフェーズ
    PlayerSelectPhase m_phase;
    // アクションキャラ
    std::shared_ptr<Character> m_actionCharacter;
    // 選択カード
    std::optional<CardData> m_selectedCard;
    // 選択カードのindex
    int m_selectedCardIndex = -1;
    // ターゲット候補
    std::vector<std::shared_ptr<Character>> m_targetCandidates;
    // 選択ターゲット
    std::vector<std::shared_ptr<Character>> m_selectedTargets;
    // アクションデータ
    std::optional<Action> m_action;
    // ターン修了確認用
    bool m_requestTurnEnd = false;

// 内部処理
    // カード選択
    void SelectCard(sf::RenderWindow& window);
    // ターゲット選択
    void SelectTarget(sf::RenderWindow& window);
    //  クリック判定
    int GetClickHandIndex(sf::RenderWindow& window) const;
    // 決定ボタンがクリックされたか
    bool IsDecisionButtonClicked(sf::RenderWindow& window) const;
    // 終了ボタンが押されているか確認
    bool IsTurnEndButtonClicked(sf::RenderWindow& window) const;
};
