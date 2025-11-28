#pragma once
#include "../Character.h"

class Enemy : public Character
{

public:

	Enemy(CharacterData& arg_data) :Character(arg_data)
	{

	}

	void Update() override;
	void Render() override;
	void Action() override;

};