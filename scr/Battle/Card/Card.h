#pragma once
#include "CardDate.h"

class Character;


class Card {

protected:

    CardData m_data;
    int m_value;
    int m_ownerID;       // どのキャラのカードか

public:
    Card() = default;
    // 初期化
    Card(CardData& data)
        :m_data(data),
        m_value(-1),
        m_ownerID(-1)
    {

    }

    // デストラクタ
    virtual ~Card() = default;

    // カードの使用
    void UseCard(Character* target);

    // カードの削除

};
