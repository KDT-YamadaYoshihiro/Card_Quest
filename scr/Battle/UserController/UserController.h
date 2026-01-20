#pragma once
#include<SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <vector>
#include <optional>
#include "ActionData.h"
#include "../../Battle/BattleContex/BattleContex.h"
#include "../../Character/Character.h"
#include "../../Card/Card.h"
#include "../SelectTarget/TargetSelect.h"
#include "../../UI/CircleButton.h"

class UserController
{
private:

	// コンテックス
	BattleContext* m_context = nullptr;

	// 選択した行動キャラ
	std::shared_ptr<Character> m_selectedActor;

	// 選択したターゲット
	std::vector<std::shared_ptr<Character>> m_selectedTargets;

	// 選択確定フラグ
	bool m_actionConfimed = false;

public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="arg_context"></param>
	/// <returns>成功：失敗</returns>
	bool Init(BattleContext* arg_context);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 行動確定確認
	/// </summary>
	/// <returns></returns>
	bool IsActionConfirmed()const;

	/// <summary>
	/// 行動確定取得
	/// </summary>
	/// <returns></returns>
	Action TakeConfirmedAction();

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

};

//class UserController
//{
//public:
//
//    // コンストラクタ
//    UserController();
//
//	// 描画
//    void Draw(sf::RenderWindow& window) const;
//    // カード選択
//    std::optional<CardData> SelectCard(sf::RenderWindow& window, const std::vector<std::unique_ptr<Card>>& arg_hand);
//    // ターゲット選択
//    std::optional<std::vector<std::shared_ptr<Character>>> SelectTarget(sf::RenderWindow& window, const std::vector<std::shared_ptr<Character>>& arg_target, const CardData& arg_card, const std::shared_ptr<Character>& arg_actionChara);
//	// 選択カードindex取得
//    int GetSelectedCardIndex() const;
//    // ターン終了ボタン用
//    bool IsTurnEndRequested() const;
//	// ターン終了要求リセット
//    void ResetTurnEndRequest();
//    // コントローラーのリセット
//    void Reset();
//
//private:
//
//    // 選択カード
//	std::optional<CardData> m_selectedCard;
//	// 選択カードインデックス
//	int m_selectedCardIndex;
//	// 決定ボタン
//	CircleButton m_decisionButton;
//	// ターン終了ボタン
//	CircleButton m_turnEndButton;
//    // ターン終了確認用
//    bool m_requestTurnEnd;
//
//// 内部処理
//    //  クリック判定
//    int GetClickHandIndex(sf::RenderWindow& window) const;
//    // 決定ボタンがクリックされたか
//    bool IsDecisionButtonClicked(sf::RenderWindow& window) const;
//    // 終了ボタンが押されているか確認
//    bool IsTurnEndButtonClicked(sf::RenderWindow& window) const;
//};
