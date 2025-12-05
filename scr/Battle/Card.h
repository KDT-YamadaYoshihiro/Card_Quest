#pragma once
#include "../Character/Character.h"

class Card {
public:
    int cost;
    ActionType type;   // ダメージ、回復、バフなど
    int value;
    int ownerID;       // どのキャラのカードか

    void Execute(Character* target);
};
