#include "Player.h"
#include "../../View/Render/Animetion/Animation.h"

// 更新
void Player::Update()
{
}

// 描画
void Player::Render(RenderSystem& render)
{
	animetion->Draw(render);
}

// アクション
void Player::Action()
{
}
