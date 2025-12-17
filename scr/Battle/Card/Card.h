#pragma once
#include "CardDate.h"

class Character;


class Card {

protected:

    CardData m_data;
    int m_value;
    int m_ownerID;       // どのキャラのカードか

public:

    Card(CardData& data)
        :m_data(data),
        m_value(-1),
        m_ownerID(-1)
    {

    }

    // カードの使用
    void UseCard(Character* target);

    // カードの削除

};
