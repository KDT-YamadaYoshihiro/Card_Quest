#pragma once
#include "../Character/Character.h"

class Card {


    int cost;
    ActionType type;   // ダメージ、回復、バフなど
    int value;
    int ownerID;       // どのキャラのカードか

public:

    void Execute(Character* target);
};
