#pragma once
#include <vector>
#include "../Character/Character.h"
#include "Deck.h"
#include "Hand.h"
#include "CostManager.h"


class Battle {

public:

    enum class TurnPhase {
        PlayerTurn,
        EnemyTurn,
    };

private:
    std::vector<Character*> m_playerMembers;
    std::vector<Character*> m_enemyMembers;

    Deck m_deck;             // 16枚の山札
    Hand m_hand;             // 現在の手札
    CostManager m_cost;      // コスト管理

    TurnPhase m_phase;
    int m_turnCount;

public:
    void Init();
    void Update();

    bool IsBattleEnd() const;
    bool IsPlayerWin() const;
    bool IsEnemyWin() const;

private:
    void StartTurn();
    void PlayerUpdate();
    void EnemyUpdate();
    void EndTurn();
};
