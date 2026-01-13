#include "Enemy.h"
#include "../../View/Render/Animetion/Animation.h"

void Enemy::Init(const std::vector<sf::Texture>& arg_textures, const sf::Vector2f& arg_startPos)
{
    for (const auto& tex : arg_textures)
    {
        m_animation->AddFrame(tex);
    }
    SetPosition(arg_startPos);
}

void Enemy::Update()
{
}

// •`‰æ
void Enemy::Render(RenderSystem& render)
{
}

// ƒAƒNƒVƒ‡ƒ“
void Enemy::Action()
{
}

// “–‚½‚è”»’èŽæ“¾
sf::FloatRect Enemy::GetHitCircle() const
{
	return sf::FloatRect();
}
