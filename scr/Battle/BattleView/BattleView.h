#pragma once
#include <SFML/Graphics.hpp>
#include "../../Battle/BattleContex/BattleContext.h"
#include "../../Card/CardRenderer/CardRenderer.h"
#include "../../Character/ChracterSprite/CharacterSprite.h"


// バトルビューフェーズ
enum class BattleViewPhase
{
	Default,		// ターン開始・終了・エネミーターン
	SelectPlayer,	// キャラクター選択
	SelectCard,		// カード選択
	SelectTarget	// ターゲット選択
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
	int m_selectedCard = -1;
	// 
	std::vector<std::shared_ptr<Character>> m_targets;

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
	/// 描画
	/// </summary>
	/// <param name="window"></param>
	void Render(sf::RenderWindow& window);

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
	/// コスト設定
	/// </summary>
	/// <param name="arg_value"></param>
	void ShowCostGain(int arg_value);


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
	void DrawCards(sf::RenderWindow& window);

	/// <summary>
	/// ターゲットフォーカス（円を表示）
	/// </summary>
	/// <param name="window"></param>
	void DrawFocus(sf::RenderWindow& window);

	/// <summary>
	/// コスト表示
	/// </summary>
	/// <param name="window"></param>
	void DrawCostGain(sf::RenderWindow& window);

};