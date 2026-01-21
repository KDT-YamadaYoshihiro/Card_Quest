#pragma once
#include <vector>
#include <optional>
#include "ActionData.h"
#include "../../Card/CardDate.h"


class UserController
{
public:
    // 初期化
    UserController();

    // ターン制御
    void ResetTurn();
    void Update();          // 入力状態更新
    void HandleInput();     // UI/選択処理

// 入力
	// キャラクター選択
    void SelectCharacter(int characterId);
	// カード選択（保持スロット）
    void SelectCard(int cardSlotIndex);
	// ターゲット選択
    void SelectTarget(int targetId);
	// 確定・キャンセル
    void Confirm();
    void Cancel();

// 状態取得
	// 現在フェーズ取得
    PlayerSelectPhase GetPhase() const;
	// 確定アクションの有無
    bool HasConfirmedAction() const;
	// 確定アクション取得（消費）
    UserAction ConsumeAction();
	// 選択キャラクター取得
    int GetSelectedCharacter() const;
	// 選択カードスロット取得
    int GetSelectedCardSlot() const;
	// 選択ターゲット取得
    const std::vector<int>& GetSelectedTargets() const;

private:

	// 現在フェーズ
    PlayerSelectPhase m_phase;
	// 選択キャラクターID
    std::optional<int> m_selectedCharacterId;
	// 選択カードスロット
    std::optional<int> m_selectedCardSlot;
	// 選択ターゲットIDリスト
    std::vector<int> m_selectedTargets;

	// 確定アクション
    std::optional<UserAction> m_confirmedAction;

private:

	// 選択情報クリア
    void ClearSelections();
};
