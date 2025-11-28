#pragma once
#include <string>
#include <vector>

// 座標
struct Position {
	int x;
	int y;
};

// ターゲットタイプ
enum class TargetType
{
	ENEMY,
	ENEMY_ALL,
	SELF,
	ALLY,
	ALLY_ALL
};

// アクションタイプ
enum class ActionType {
	ATTCK,
	MAGIC,
	HEAL,
	BUFF,
	UTILITY

};


// カード情報
struct CardData
{
	int cardId;
	std::string name;
	int cost;
	ActionType actionType;
	float power;
	TargetType targetType;
	int turn;
	std::string description;
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