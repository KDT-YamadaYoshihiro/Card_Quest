#pragma once
#include <string>

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
struct Card
{
	int cardId;
	std::string name;
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

	int cardId1;
	int cardId2;
	int cardId3;
	int cardId4;
};