#pragma once
#include <vector>
#include <memory>
#include "../Character/Character.h"
#include "../Battle/Cost/CostManager.h"
#include "../Battle/Card/CardManager/CardManager.h"


class BattleSystem {

public:
    // フェーズ
    enum class TurnPhase {
        StartTurn,
        PlayerTurn,
        EnemyTurn,
        EndTurn
    };

private:

    // キャラクター
    std::vector<std::shared_ptr<Character>> m_players;
    std::vector<std::shared_ptr<Character>> m_enemies;
    // コスト管理
    std::unique_ptr<CostManager> m_cost;
    // フェーズ
    TurnPhase m_phase;
    // ターン数
    int m_turnCount;

public:

	BattleSystem();
	virtual ~BattleSystem() = default;

    /// <summary>
    /// 初期化
    /// </summary>
    void Init();

    /// <summary>
    /// 更新処理
    /// </summary>
    void Update();

    // 描画
    void Render(RenderSystem& render);

    // プレイヤーの勝利確認
    bool CheckWin() const {

        for (auto e : m_enemies) {
            if (e->GetStatus().dead) {
                return true;
            }
        }
        return false;
    }

    // プレイヤーの敗北確認
    bool CheckLose() const {
        for (auto p : m_players) {
            if (p->GetStatus().dead)
            {
                return true;
            }
        }
        return false;
    }

    // カードとキャラクターの結びつけ
    void OnUseCard(size_t arg_handIndex, size_t arg_targetIndex);

    void ApplyCardAction(const CardUseResult& result, std::shared_ptr<Character> arg_owner, std::shared_ptr<Character> arg_target);

    // 現在のフェーズの取得
    TurnPhase GetTurnPhase() const { return m_phase; }
    // 現在のコスト取得
    int GetCost() const { return m_cost->GetCost(); }

private:
    // プレイヤー,カード生成
    void CreateEntity();

    void StartTurn();
    void PlayerUpdate();
    void EnemyUpdate();
    void EndTurn();
};
