#pragma once
#include "../Character.h"
#include "../../View/Render/Animetion/Animation.h"

class RenderSystem;

class Player : public Character {

	Animation* animetion;

public:

	Player(CharacterData& arg_data) :Character(arg_data),animetion(nullptr)
	{
		m_faction = Faction::Player;
	}

	void Update() override;
	void Render(RenderSystem& render) override;
	void Action() override;

};