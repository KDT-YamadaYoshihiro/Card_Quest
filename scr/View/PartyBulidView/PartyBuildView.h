#pragma once
#include <SFML/Graphics.hpp>
#include "System/PartyBulid/PartyBuildContext/PartyBuildContext.h"

class CharacterSprite;
class RenderSystem;

struct IconView
{
	sf::Sprite sprite;
	sf::FloatRect rect;

	IconView(const sf::Texture& tex, const sf::Vector2f& pos, const sf::Vector2f& size)
		:sprite(tex),
		rect(pos,size)
	{ }

};

class PartyBuildView
{
private:

	RenderSystem& m_render;
	// キャラクターと画像状態の紐づけ
	std::unordered_map<const Character*, std::unique_ptr<CharacterSprite>> m_spriteTable;
	// キャラクターアイコンView情報
	std::vector<IconView> m_characterIcons;
	std::vector<IconView> m_partyIcons;

	sf::RectangleShape m_benchBackground; // ベンチエリア用
	sf::RectangleShape m_partyBackground; // パーティエリア用

	// エリアの定義（定数）
	const sf::Vector2f BENCH_POS = { 50.0f, 400.0f };
	const sf::Vector2f BENCH_SIZE = { 1200.0f, 220.0f };
	const sf::Vector2f PARTY_POS = { 180.0f, 100.0f };
	const sf::Vector2f PARTY_SIZE = { 900.0f, 250.0f };

	const float SIZE_X = 125.0f;
	const float SIZE_Y = 110.0f;

public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="render"></param>
	PartyBuildView(RenderSystem& arg_render);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="context"></param>
	void Update(const PartyBuildContext& context);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="context"></param>
	void Draw(const PartyBuildContext& context);

	/// <summary>
	/// キャラクターアイコンView情報の取得
	/// </summary>
	/// <returns></returns>
	const std::vector<IconView>& GetCharacterIcons() const;

	/// <summary>
	/// パーティーキャラアイコンView情報の取得
	/// </summary>
	/// <returns></returns>
	const std::vector<IconView>& GetPartyIcons() const;

};

