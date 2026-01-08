#pragma once
#include "../Character.h"
#include "../../View/Render/Animetion/Animation.h"

class Player : public Character {


public:

	Player(CharacterData& arg_data) :Character(arg_data)
	{
		m_faction = Faction::Player;
	}

	void Update() override;
	void Render(RenderSystem& render) override;
	void Action() override;
	sf::FloatRect GetHitCircle() const override;

};