#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "View/Render/RenderSystem.h"
#include "Entity/Character/Character.h"
#include "System/Battle/Cost/CostManager.h"
#include "Entity/Card/CardManager/CardManager.h"
#include "View/CardRenderer/CardRenderer.h"
#include "System/Battle/BattleContex/BattleContext.h"
#include "System/Battle/UserController/UserController.h"
#include "View/BattleView/BattleView.h"
#include "UI/BoxButton.h"
#include "System/Battle/MenuWindow/MenuWindow.h"


class BattleSystem
{

private:
	
	// ターンフェーズ
	enum class TurnPhase
	{
		StartTurn,
		UserTurn,
		EnemyTurn,
		EndTurn,
		Result,
	};

	// ユーザーターンフェーズ
	enum class UserTurnPhase
	{
		Start,
		Select,
		Action,
		EndCheck,
		EndUserTurn
	};

	// エネミーターンフェーズ
	enum class EnemyTurnPhase
	{
		Start,
		Select,
		Action,
		NextEnemy,
		End
	};
	
	// 定数
	const int  COMMON_CARDID = 1000;
	const int  UNIQUE_CARDID = 1100;
	
	// コンテキスト
	std::shared_ptr<BattleContext> m_context;
	// バトル描画
	std::unique_ptr<BattleView> m_battleView;
	// ユーザー入力系
	std::unique_ptr<UserController> m_userController;
	// キャラクター
	std::vector<std::shared_ptr<Character>> m_players;
	std::vector<std::shared_ptr<Character>> m_enemies;
	// 描画システム
	std::unique_ptr<RenderSystem> m_render;
	
	// リザルト用
	// 編成画面遷移ボタン
	std::unique_ptr<BoxButton> m_toPartySceneButton;

	// ステージ選択遷移ボタン
	std::unique_ptr<BoxButton> m_toStageSelectButton;

	// ターンフェーズ
	TurnPhase m_phase;
	// ユーザーターンフェーズ
	UserTurnPhase m_userPhase;
	// エネミーターンフェーズ
	EnemyTurnPhase m_enemyPhase;

	// エネミー用
	// 行動エネミー一覧
	std::vector<std::shared_ptr<Character>> m_actingEnemies;
	// 行動エネミー
	std::shared_ptr<Character> m_currentEnemy = nullptr;
	// ターゲット確定枠
	std::vector<std::shared_ptr<Character>> m_enemyFinalTargets;

	// メニューウィンドウ
	std::unique_ptr<MenuWindow> m_menuWindow;

	// Animation更新用
	sf::Clock m_clock;


	// ターン数
	int m_turnCount;

	// 行動エネミーのindex
	int m_currentEnemyIndex = 0;

	// シーン遷移用フラグ (あきらめる等で使用)
	bool m_isToStageSelect = false;


	// シーン切り替え判定用
	bool m_toPartyScene = false;
	bool m_toStageSelectScene = false;


public:

	/// <summary>
	/// 初期化
	/// </summary>
	BattleSystem(sf::RenderWindow& arg_window);
	/// <summary>
	/// 終了処理
	/// </summary>
	virtual ~BattleSystem() = default;

	/// <summary>
	/// 生成
	/// </summary>
	/// <returns>成功：失敗</returns>
	bool Init(sf::RenderWindow& arg_window);

	/// <summary>
	///	更新処理
	/// </summary>
	void Update(sf::RenderWindow& window);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="window"></param>
	void Render(sf::RenderWindow& window);

	/// <summary>
	/// バトル終了判定
	/// </summary>
	/// <returns></returns>
	bool IsBattleEnd() const ;

	/// <summary>
	/// ユーザーの勝利か
	/// </summary>
	/// <returns></returns>
	bool IsUserWin() const;

	/// <summary>
	/// シーン切り替え判定
	/// </summary>
	/// <returns></returns>
	bool IsToPartyScene() const;
	bool IsToStageSelectScene() const;

	/// <summary>
	/// メニューの状態取得
	/// </summary>
	/// <returns></returns>
	bool GetStageSelectScene() const;

	/// <summary>
	/// 再挑戦リクエストがあるか
	/// </summary>
	/// <returns></returns>
	bool IsRetryRequested() const;

private:

	/// <summary>
	/// 座標の初期化
	/// </summary>
	void CharaInitPosition();

	/// <summary>
	/// メニューの更新メソッド
	/// </summary>
	void MenuUpdate(sf::RenderWindow& arg_window);

	/// <summary>
	/// ターン開始
	/// </summary>
	void StartTurn();

	/// <summary>
	/// ユーザーターン
	/// </summary>
	void UserTurn(sf::RenderWindow& window);

	/// <summary>
	/// エネミーターン
	/// </summary>
	void EnemyTurn();

	/// <summary>
	/// ターン終了
	/// </summary>
	void EndTurn();

	/// <summary>
	/// リザルト画面のクリック判定
	/// </summary>
	void ResultEvent(sf::RenderWindow& arg_woindow);

	/// <summary>
	/// リザルト描画
	/// </summary>
	/// <param name="arg_window"></param>
	void ResultView(sf::RenderWindow& arg_window);


	/// <summary>
	/// カードの使用時の効果
	/// </summary>
	/// <param name="actor"></param>
	/// <param name="targets"></param>
	/// <param name="card"></param>
	void ApplyAction(const std::shared_ptr<Character>& actor, const std::vector<std::shared_ptr<Character>>& targets, const CardData& card);

};


