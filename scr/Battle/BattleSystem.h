#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "../View/Render/RenderSystem.h"
#include "../Character/Character.h"
#include "../Battle/Cost/CostManager.h"
#include "../Card/CardManager/CardManager.h"
#include "../Card/CardRenderer/CardRenderer.h"
#include "../Battle/BattleContex/BattleContext.h"
#include "../Battle/UserController/UserController.h"

class BattleSystem
{

private:
	
	// ターンフェーズ
	enum class TurnPhase
	{
		StartTurn,
		UserTurn,
		EnemyTurn,
		EndTurn
	};
	
	// 依存システム
	
	// コンテキスト
	std::unique_ptr<BattleContext> m_context;
	// ユーザー入力系
	std::unique_ptr<UserController> m_userController;
	// コスト管理
	std::unique_ptr<CostManager> m_costManager;
	// キャラクター
	std::vector<std::shared_ptr<Character>> m_players;
	std::vector<std::shared_ptr<Character>> m_enemies;
	// 描画システム
	std::unique_ptr<RenderSystem> m_render;

	// ターンフェーズ
	TurnPhase m_phase;
	// ターン数
	int m_turnCount;

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

private:

	/// <summary>
	/// 座標の初期化
	/// </summary>
	void InitPosition();

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
	/// カードの使用時の効果
	/// </summary>
	/// <param name="actor"></param>
	/// <param name="targets"></param>
	/// <param name="card"></param>
	void ApplyAction(const std::shared_ptr<Character>& actor, const std::vector<std::shared_ptr<Character>>& targets, const CardData& card);

};


