#pragma once
#include <string>

struct Position {
	int x;
	int y;
};

struct CardId
{
	int card1;
	int card2;
	int card3;
	int card4;
};

struct CharacterData
{
	int charaId;
	std::string name;
	int hp;
	int maxHp;
	int atk;
	int def;
	bool dead;
	CardId cardId;
};