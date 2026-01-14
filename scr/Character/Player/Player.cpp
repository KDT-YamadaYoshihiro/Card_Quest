#include "Player.h"
#include "../../View/Render/Animetion/Animation.h"

void Player::Init(const std::vector<sf::Texture>& arg_textures, const sf::Vector2f& arg_startPos)
{
	// 初期位置設定
	SetPosition(arg_startPos);
}

void Player::Update()
{
}

// 描画
void Player::Render(RenderSystem& render)
{
	auto tex = TextureLoader::GetInstance().GetTextureID(m_status.textureKey);

	if (tex)
	{
		sf::Sprite sprite(*tex);
		sprite.setPosition(m_postion);
		render.Draw(sprite);
	}
	else
	{
#ifdef _DEBUG
		// テクスチャがない場合は何もしない
		std::cout << "テクスチャーが見つかりません：" << m_status.textureKey << std::endl; 
#endif // _DEBUG
	}

}

// アクション
void Player::Action()
{
}

// 当たり判定取得
sf::FloatRect Player::GetHitCircle() const
{
	return sf::FloatRect();
}
