#pragma once
#include <SFML/Graphics.hpp>
#include "System/PartyBulid/PartyBuildContext/PartyBuildContext.h"
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
	std::vector<IconView> m_characterIcons;
	std::vector<IconView> m_partyIcons;

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
	void Draw();

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


private:

	/// <summary>
	/// キャラクターのアイコン描画
	/// </summary>
	/// <param name="context"></param>
	void BulidCharacterIcons(const PartyBuildContext& context);

	/// <summary>
	/// パーティメンバーのアイコン描画
	/// </summary>
	/// <param name="context"></param>
	void BulidPartyIcons(const PartyBuildContext& context);

};

