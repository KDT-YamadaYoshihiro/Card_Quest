#pragma once
#include <vector>
#include <memory>
#include "../Character/Character.h"
#include "../Battle/Cost/CostManager.h"


class Battle {

public:

    enum class TurnPhase {
        StartTurn,
        PlayerTurn,
        EnemyTurn,
        End
    };

private:
    std::vector<std::shared_ptr<Character>> m_playerMembers;
    std::vector<std::shared_ptr<Character>> m_enemyMembers;

    // コスト管理
    std::shared_ptr<CostManager> m_cost;

    TurnPhase m_phase;
    int m_turnCount;

public:

	Battle();
	virtual ~Battle() = default;

    void Init(const std::vector<std::shared_ptr<Character>>&,
              const std::vector<std::shared_ptr<Character>>&,
              const std::vector<Card>& allCards);

    void Update();

    // プレイヤーの勝利
    bool CheckWin() const {

        for (auto e : m_enemyMembers) {
            if (e->GetStatus().dead) {
                return true;
            }
        }
        return false;
    }

    // プレイヤーの敗北
    bool CheckLose() const {
        for (auto p : m_playerMembers) {
            if (p->GetStatus().dead)
            {
                return true;
            }
        }
        return false;
    }

    TurnPhase GetTurnPhase() const { return m_phase; }
    int GetCost() const { return m_cost->GetCost(); }

private:
    void StartTurn();
    void PlayerUpdate();
    void EnemyUpdate();
    void EndTurn();
};
