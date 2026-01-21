#include "UserController.h"

// 初期化
UserController::UserController()
{
    ResetTurn();
}

// ターン初期化
void UserController::ResetTurn()
{
    m_phase = PlayerSelectPhase::SELECT_CHARACTER;
    ClearSelections();
    m_confirmedAction.reset();
}

// 更新
void UserController::Update()
{
}

// 入力
void UserController::HandleInput()
{
}

// キャラクター選択
void UserController::SelectCharacter(int characterId)
{
    if (m_phase != PlayerSelectPhase::SELECT_CHARACTER)
        return;

    m_selectedCharacterId = characterId;
    m_phase = PlayerSelectPhase::SELECT_CARD;
}

// カード選択（保持スロット）
void UserController::SelectCard(int cardSlotIndex)
{
    if (m_phase != PlayerSelectPhase::SELECT_CARD)
        return;

    m_selectedCardSlot = cardSlotIndex;
    m_selectedTargets.clear();
    m_phase = PlayerSelectPhase::SELECT_TARGET;
}

// ターゲット選択
void UserController::SelectTarget(int targetId)
{
    if (m_phase != PlayerSelectPhase::SELECT_TARGET)
        return;

    m_selectedTargets.clear();
    m_selectedTargets.push_back(targetId);
    m_phase = PlayerSelectPhase::CONFIRM;
}

// 確定
void UserController::Confirm()
{
    if (m_phase != PlayerSelectPhase::CONFIRM)
        return;

    if (!m_selectedCharacterId || !m_selectedCardSlot)
        return;

    m_confirmedAction = UserAction{
        *m_selectedCharacterId,
        *m_selectedCardSlot,
        m_selectedTargets
    };

    m_phase = PlayerSelectPhase::NONE;
}

// キャンセル
void UserController::Cancel()
{
    switch (m_phase)
    {
    case PlayerSelectPhase::SELECT_CARD:
        m_selectedCharacterId.reset();
        m_phase = PlayerSelectPhase::SELECT_CHARACTER;
        break;

    case PlayerSelectPhase::SELECT_TARGET:
        m_selectedCardSlot.reset();
        m_phase = PlayerSelectPhase::SELECT_CARD;
        break;

    case PlayerSelectPhase::CONFIRM:
        m_selectedTargets.clear();
        m_phase = PlayerSelectPhase::SELECT_TARGET;
        break;

    default:
        break;
    }
}

// ===== 状態取得 =====

// 現在フェーズ取得
PlayerSelectPhase UserController::GetPhase() const
{
    return m_phase;
}

// 確定アクションの有無
bool UserController::HasConfirmedAction() const
{
    return m_confirmedAction.has_value();
}

// 確定アクション取得（消費）
UserAction UserController::ConsumeAction()
{
    UserAction action = *m_confirmedAction;
    m_confirmedAction.reset();
    return action;
}

// 選択情報取得
int UserController::GetSelectedCharacter() const
{
    return m_selectedCharacterId.value_or(-1);
}

// 選択カードスロット取得
int UserController::GetSelectedCardSlot() const
{
    return m_selectedCardSlot.value_or(-1);
}

// 選択ターゲット取得
const std::vector<int>& UserController::GetSelectedTargets() const
{
    return m_selectedTargets;
}

// 内部処理
void UserController::ClearSelections()
{
    m_selectedCharacterId.reset();
    m_selectedCardSlot.reset();
    m_selectedTargets.clear();
}
