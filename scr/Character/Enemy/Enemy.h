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

	void Update() override;
	void Render(RenderSystem& render) override;
	void Action() override;
	sf::FloatRect GetHitCircle() const override;

};