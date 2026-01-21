#include "Player.h"
#include "../../View/Render/Animetion/Animation.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="data"></param>
/// <param name="maxCardSlot"></param>
PlayerCharacter::PlayerCharacter(CharacterData& data, int maxCardSlot)
	: Character(data, Faction::Player, maxCardSlot)
{
}

/// <summary>
/// 更新
/// </summary>
void PlayerCharacter::Update()
{
}

/// <summary>
/// 描画
/// </summary>
/// <param name="render"></param>
void PlayerCharacter::Render(RenderSystem& render)
{
	auto tex = TextureLoader::GetInstance().GetTextureID(m_data.textureKey);

	if (tex)
	{
		sf::Sprite sprite(*tex);
		sprite.setPosition({ m_pos.x, m_pos.y });
		render.Draw(sprite);
	}
	else
	{
#ifdef _DEBUG
		// テクスチャがない場合は何もしない
		std::cout << "テクスチャーが見つかりません：" << m_data.textureKey << std::endl; 
#endif // _DEBUG
	}

}
