#pragma once
#include <string>
#include <vector>

// 座標
struct Position {
	int x;
	int y;
};


// キャラクターステータス
struct CharacterData
{
	int charaId;
	std::string name;
	int hp;
	int maxHp;
	int mp;
	int atk;
	int def;
	bool dead;
	
	std::vector<int> cardIds;

};