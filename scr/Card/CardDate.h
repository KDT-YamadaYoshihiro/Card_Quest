#pragma once
#include <string>

// カードの状態	
enum class CardZone
{
	None,		// 無属性
	Deck,		// 山札
	Hand,		// 手札
	Grave		// 墓地
};

// ターゲットタイプ
enum class TargetType
{
	OPPONENT,
	OPPONENT_ALL,
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
