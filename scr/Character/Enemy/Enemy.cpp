#include "Enemy.h"
#include "../../View/Render/Animetion/Animation.h"

void Enemy::Init(const std::vector<sf::Texture>& arg_textures, const sf::Vector2f& arg_startPos)
{
    SetPosition(arg_startPos);
}

void Enemy::Update()
{
}

// 描画
void Enemy::Render(RenderSystem& render)
{
	// テクスチャ取得
	auto tex = TextureLoader::GetInstance().GetTextureID(m_status.textureKey);
	if (tex)
	{
		sf::Sprite sprite(*tex);
		sprite.setPosition(m_postion);
		render.Draw(sprite);
	}
	else {
#ifdef _DEBUG
		// テクスチャがない場合は何もしない
		std::cout << "テクスチャーが見つかりません：" << m_status.textureKey << std::endl;
#endif // _DEBUG
	}

}

// アクション
void Enemy::Action()
{
}

// 当たり判定取得
sf::FloatRect Enemy::GetHitCircle() const
{
	return sf::FloatRect();
}
