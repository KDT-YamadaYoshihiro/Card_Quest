#pragma once
#include "../Character.h"

class Player : public Character {

public:

	Player(CharacterData& arg_data) :Character(arg_data) 
	{

	}

	void Update() override;
	void Render() override;
	void Action() override;

};