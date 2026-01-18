//#pragma once
//#include <SFML/Graphics.hpp>
//#include <vector>
//#include <memory>
//#include "../View/Render/RenderSystem.h"
//#include "../Character/Character.h"
//#include "../Battle/Cost/CostManager.h"
//#include "../Card/CardManager/CardManager.h"
//#include "../Card/CardRenderer/CardRenderer.h"
//#include "../Battle/BattleContex/BattleContex.h"
//#include "../Battle/UserController/UserController.h"
//
//
//class BattleSystem {
//
//public:
//
//    // フェーズ
//    enum class TurnPhase {
//        StartTurn,
//        UserTurn,
//        EnemyTurn,
//        EndTurn
//    };
//
//    // プレイヤー選択フェーズ
//    enum class PlayerSelectPhase
//    {
//        SELECT_CARD,
//		CREATE_TARGET_CANDIDATES,
//        SELECT_TARGET,
//        CONFIRM
//    };
//
//
//private:
//
//	// 描画システム
//	std::unique_ptr<RenderSystem> m_renderSystem;
//    // キャラクター
//    std::vector<std::shared_ptr<Character>> m_players;
//    std::vector<std::shared_ptr<Character>> m_enemies;
//    // アクションキャラクター
//    std::shared_ptr<Character> m_actionCharacter;
//	// ターゲット候補と選択ターゲット
//    std::vector<std::shared_ptr<Character>> m_targetCandidates;
//    std::vector<std::shared_ptr<Character>> m_selectedTargets;
//	// 選択中カード
//	std::optional<CardData> m_selectedCard;
//	// プレイヤーコントローラー
//    std::unique_ptr<UserController> m_userController;
//    // コスト管理
//    std::unique_ptr<CostManager> m_cost;
//    // カード描画
//    std::unique_ptr<CardRenderer> m_cardRenderer;
//    // フェーズ
//    TurnPhase m_phase;
//	// プレイヤーターンフェーズ
//	PlayerSelectPhase m_userPhase;
//
//    // ターン数
//    int m_turnCount;
//
//	// 現在のプレイヤーindex
//    size_t m_currentPlayerIndex = 0;
//    // 選択中カードindex
//    int m_choiceCardIndex;
//
//public:
//
//    BattleSystem(sf::RenderWindow& arg_window);
//    virtual ~BattleSystem() = default;
//
//    /// <summary>
//    /// 初期化
//    /// </summary>
//    void Init(sf::RenderWindow& arg_window);
//
//    /// <summary>
//    /// 更新処理
//    /// </summary>
//    void Update(sf::RenderWindow& arg_window);
//    // 描画
//    void Render(sf::RenderWindow& arg_window);
//
//    // プレイヤーの勝利確認
//    bool CheckWin() const {
//
//        for (auto& e : m_enemies) {
//            if (!e->GetStatus().dead)
//                return false;
//        }
//        return true;
//    }
//
//    // プレイヤーの敗北確認
//    bool CheckLose() const {
//        for (auto p : m_players) {
//            if (!p->GetStatus().dead)
//            {
//                return false;
//            }
//        }
//        return true;
//    }
//
//    // カードとキャラクターの結びつけ
//    void OnUseCard(const Action& action);
//
//	// アクション適用
//    void ApplyAction(const Action& action);
//
//    // 現在のフェーズの取得
//    TurnPhase GetTurnPhase() const { return m_phase; }
//    // 現在のコスト取得
//    int GetCost() const { return m_cost->GetCost(); }
//
//private:
//    // プレイヤー,カード生成
//    void CreateEntity(sf::RenderWindow& window);
//    // フォーカス
//    void UpdateCardOwnerFocus();
//    // アクションキャラの取得
//    std::shared_ptr<Character> GetActionCharacterFromCard(const Card& arg_card);
//    // ターゲット候補作成
//    std::vector<std::shared_ptr<Character>> MakeTargetCandidates(const std::shared_ptr<Character>& actionChara, TargetType targetType) const;
//    // ユーザーターンの初期化
//    void ResetUserTurn();
//	// ターン開始時
//    void StartTurn();
//	// ユーザー更新
//    void UserUpdate(sf::RenderWindow& arg_window);
//	// エネミー更新
//    void EnemyUpdate();
//	// ターン終了時
//    void EndTurn();
//};
