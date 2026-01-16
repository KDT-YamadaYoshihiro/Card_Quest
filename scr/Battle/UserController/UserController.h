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
#include "../../UI/CircleButton.h"

class UserController
{
public:

    // コンストラクタ
    UserController();

	// 描画
    void Draw(sf::RenderWindow& window) const;
	// アクションがあるか
    bool HasAction() const;
    // ターン終了ボタン用
    bool IsTurnEndRequested() const;
	// ターン終了要求リセット
    void ResetTurnEndRequest();

private:

    // 選択カード
	std::shared_ptr<Card> m_selectedCard;
    // アクションデータ
    std::optional<Action> m_action;
	// 決定ボタン
	CircleButton m_decisionButton;
	// ターン終了ボタン
	CircleButton m_turnEndButton;
    // ターン終了確認用
    bool m_requestTurnEnd;

// 内部処理
    // カード選択
    void SelectCard(sf::RenderWindow& window,const std::vector<Card> arg_hand);
    // ターゲット選択
    void SelectTarget(sf::RenderWindow& window, const std::vector<std::shared_ptr<Character>> arg_target);
    //  クリック判定
    int GetClickHandIndex(sf::RenderWindow& window) const;
    // 決定ボタンがクリックされたか
    bool IsDecisionButtonClicked(sf::RenderWindow& window) const;
    // 終了ボタンが押されているか確認
    bool IsTurnEndButtonClicked(sf::RenderWindow& window) const;
};
