#include "Player.h"
#include "../../View/Render/Animetion/Animation.h"

void Player::Init(const std::vector<sf::Texture>& arg_textures, const sf::Vector2f& arg_startPos)
{
	// アニメーションにフレームを追加
	for (const auto& tex : arg_textures)
	{
		m_animation->AddFrame(tex);
	}
	// 初期位置設定
	SetPosition(arg_startPos);
}

void Player::Update()
{
}

// 描画
void Player::Render(RenderSystem& render)
{
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
