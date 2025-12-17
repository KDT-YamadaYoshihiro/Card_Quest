#pragma once
#include <string>
#include <vector>

// 座標
struct Position {
	int x;
	int y;
};

struct BuffData
{
	float power;
	int turn;
};


// キャラクターステータス
struct CharacterData
{
	int charaId;
	int lv;
	std::string name;
	int hp;
	int maxHp;
	int magicAtk;
	int atk;
	int def;
	int exp;
	bool dead;
	


	BuffData buff;
	std::vector<int> cardIds;

};