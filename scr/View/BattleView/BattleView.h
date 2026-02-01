#pragma once
#include <SFML/Graphics.hpp>
#include "System/Battle/BattleContex/BattleContext.h"
#include "View/CardRenderer/CardRenderer.h"
#include "View/ChracterSprite/CharacterSprite.h"
#include "System/Battle/UserController/UserController.h"

// バトルビューフェーズ
enum class BattleViewPhase
{
	Default,		// ターン開始・終了・エネミーターン
	SelectPlayer,	// キャラクター選択
	SelectCard,		// カード選択
	SelectTarget	// ターゲット選択
};

// ダメージ・回復量の表示用
struct DamagePopup
{
	sf::Vector2f position;
	int value;
	bool isHeal;
	float lifeTime;
};


// 
class BattleView
{
	// 
	BattleContext& m_context;
	//
	RenderSystem& m_render;
	// キャラクターと画像状態の紐づけ
	std::unordered_map<Character*, std::unique_ptr<CharacterSprite>> m_spriteTable;
	// 
	std::shared_ptr<CardRenderer> m_cardRenderer;
	// 進行管理
	BattleViewPhase m_phase = BattleViewPhase::Default;
	// 
	std::shared_ptr<Character> m_selectedActor;
	// 
	std::vector<std::shared_ptr<Character>> m_targets;
	//
	std::vector<DamagePopup> m_popups;

	const sf::Font& m_font;
	// 
	int m_selectedCard = -1;
	//
	int m_costGain = 0;

public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="context"></param>
	BattleView(BattleContext& context, RenderSystem& render);

	/// <summary>
	/// フェーズの変更
	/// </summary>
	/// <param name="arg_phase">変更先フェーズ</param>
	void SetPhase(BattleViewPhase arg_phase);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="dt"></param>
	void Update(float dt);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="window"></param>
	void Render(sf::RenderWindow& arg_window);

	/// <summary>
	/// アクションキャラ設定
	/// </summary>
	/// <param name="arg_index"></param>
	void SetSelectedActor(const std::shared_ptr<Character>& actor);
	/// <summary>
	/// 選択カードのインデックス設定
	/// </summary>
	/// <param name="arg_index"></param>
	void SetSelectedCard(int arg_index);
	/// <summary>
	/// ターゲット候補設定
	/// </summary>
	/// <param name="atg_target"></param>
	void SetTargetIndices(const std::vector<std::shared_ptr<Character>>& arg_target);

	/// <summary>
	/// 追加コスト設定
	/// </summary>
	/// <param name="arg_value"></param>
	void ShowCostGain(int arg_value);

	/// <summary>
	/// 情報リセット
	/// </summary>
	void ResetTransientView();

	/// <summary>
	/// 効果量の設定
	/// </summary>
	/// <param name="arg_pos"></param>
	/// <param name="arg_value"></param>
	/// <param name="arg_isHeal"></param>
	void AddDamagePopup(const sf::Vector2f& arg_pos, int arg_value, bool arg_isHeal);

	/// <summary>
	/// ダメージ表示座標（キャラクターの中心座標の適用）
	/// </summary>
	/// <param name="c"></param>
	/// <returns></returns>
	sf::Vector2f CalcDamagePopupPos(const std::shared_ptr<Character>& c);

private:

	/// <summary>
	/// キャラクター描画
	/// </summary>
	/// <param name="window"></param>
	void DrawCharacters();

	/// <summary>
	/// カード描画
	/// </summary>
	/// <param name="window"></param>
	void DrawCards(sf::RenderWindow& arg_window);

	/// <summary>
	/// ターゲットフォーカス（円を表示）
	/// </summary>
	/// <param name="window"></param>
	void DrawFocus(sf::RenderWindow& arg_window);

	/// <summary>
	/// コスト表示
	/// </summary>
	void DrawCost(sf::RenderWindow& arg_window);

	/// <summary>
	/// 追加コスト表示
	/// </summary>
	/// <param name="window"></param>
	void DrawCostGain(sf::RenderWindow& arg_window);

	/// <summary>
	/// ステージ名の表示
	/// </summary>
	/// <param name="arg_window"></param>
	void DrawStageName(sf::RenderWindow& arg_window);

	/// <summary>
	/// ターンバーナー表示
	/// </summary>
	/// <param name="window"></param>
	void DrawTurnBanner(sf::RenderWindow& window);

	/// <summary>
	/// ターゲット情報のリセット
	/// </summary>
	void ClearTargets();
	/// <summary>
	/// 行動須数の増加値リセット
	/// </summary>
	void ClearCostGain();

	/// <summary>
	/// キャラクターの中心座標計算
	/// </summary>
	/// <param name="c"></param>
	/// <returns></returns>
	sf::Vector2f GetCharacterCenter(const std::shared_ptr<Character>& c);

	/// <summary>
	/// カメラの更新
	/// </summary>
	/// <param name="dt"></param>
	void UpdateCamera(float dt);

};