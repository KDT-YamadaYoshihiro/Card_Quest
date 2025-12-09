#pragma once
#include <string>

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
	BUFF
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
