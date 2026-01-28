#include "PartyBuildView.h"
#include "View/Render/RenderSystem.h"
#include "System/PartyBulid/PartyBuildContext/PartyBuildContext.h"
#include "CSVLoad/TextureLoader/TextureLoader.h"
#include "Entity/Character/Character.h"
#include <iostream>


/// <summary>
/// 初期化
/// </summary>
/// <param name="render"></param>
PartyBuildView::PartyBuildView(RenderSystem& arg_render)
	:m_render(arg_render)
{

}

/// <summary>
/// 更新
/// </summary>
/// <param name="context"></param>
void PartyBuildView::Update(const PartyBuildContext& arg_context)
{
	BulidCharacterIcons(arg_context);
	BulidPartyIcons(arg_context);
}

/// <summary>
/// 描画
/// </summary>
void PartyBuildView::Draw()
{
	for (auto& icon : m_characterIcons)
	{
		m_render.Draw(icon.sprite);
	}

	for (auto& icon : m_partyIcons)
	{
		m_render.Draw(icon.sprite);
	}
}

/// <summary>
/// キャラクターアイコンView情報の取得
/// </summary>
/// <returns></returns>
const std::vector<IconView>& PartyBuildView::GetCharacterIcons() const
{
	return m_characterIcons;
}

/// <summary>
/// パーティーキャラアイコンView情報の取得
/// </summary>
/// <returns></returns>
const std::vector<IconView>& PartyBuildView::GetPartyIcons() const
{
	return m_partyIcons;
}

/// <summary>
/// キャラクターのアイコン描画
/// </summary>
/// <param name="context"></param>
void PartyBuildView::BulidCharacterIcons(const PartyBuildContext& arg_context)
{

	m_characterIcons.clear();

	// スタート座標と描画間隔
	sf::Vector2f start{ 50.0f,50.0f };
	float spacingY = 120.0f;

	int index = 0;
	for (auto& ch : arg_context.GetAllCharacters())
	{

		if (arg_context.IsInParty(ch))
		{
			continue;
		}

		auto tex = TextureLoader::GetInstance().GetTextureID(ch->GetData().iconKey);

		if (!tex)
		{
			std::cout << "テクスチャーが見つかりませんでした" << std::endl;
			continue;
		}

		IconView icon(*tex, { start.x , start.y + index * spacingY }, { SIZE_X, SIZE_Y });
		icon.sprite.setPosition({ start.x , start.y + index * spacingY });
		icon.rect = sf::FloatRect(icon.sprite.getPosition(),sf::Vector2(SIZE_X, SIZE_Y));

		m_characterIcons.push_back(icon);
		++index;
	}

}

/// <summary>
/// パーティメンバーのアイコン描画
/// </summary>
/// <param name="context"></param>
void PartyBuildView::BulidPartyIcons(const PartyBuildContext& arg_context)
{
	m_partyIcons.clear();

	sf::Vector2f start{ 500.0f, 120.0f };
	float spacingX = 140.0f;

	int index = 0;
	for (auto& ch : arg_context.GetParty())
	{
		auto tex = TextureLoader::GetInstance().GetTextureID(ch->GetData().iconKey);
		if (!tex) continue;

		
		IconView icon( *tex, {start.x + index * spacingX, start.y},{SIZE_X,SIZE_Y});
		icon.sprite.setPosition({ start.x + index * spacingX, start.y });

		icon.rect = sf::FloatRect(icon.sprite.getPosition(),sf::Vector2f(SIZE_X, SIZE_Y));

		m_partyIcons.push_back(icon);
		++index;
	}
}
