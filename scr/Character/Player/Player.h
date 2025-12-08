#pragma once
#include "../Character.h"
#include "../../Render/Animetion/Animation.h"

class RenderSystem;

class Player : public Character {

	Animation* animetion;

public:

	Player(CharacterData& arg_data) :Character(arg_data),animetion(nullptr)
	{

	}

	void Update() override;
	void Render(RenderSystem& render) override;
	void Action() override;

};