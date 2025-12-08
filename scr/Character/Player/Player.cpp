#include "Player.h"
#include "../../Render/Animetion/Animation.h"

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
