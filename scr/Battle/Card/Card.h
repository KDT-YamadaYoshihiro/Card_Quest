#pragma once
#include "CardDate.h"

class Character;


class Card {

public:

    CardData m_data;
    int value;
    int ownerID;       // どのキャラのカードか


    Card(CardData& data)
        :m_data(data),
        value(-1),
        ownerID(-1)
    {

    }

    void Execute(Character* target);
};
