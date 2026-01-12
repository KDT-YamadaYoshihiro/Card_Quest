#pragma once
#include "../Character.h"

class RenderSystem;

class Enemy : public Character
{

public:

	Enemy(CharacterData& arg_data) :Character(arg_data)
	{
		m_faction = Faction::Enemy;
	}

	// 更新
	void Update(float dt) override;
	// 描画
	void Render(RenderSystem& render) override;
	// アクション
	void Action() override;
	// 当たり判定取得
	sf::FloatRect GetHitCircle() const override;

};