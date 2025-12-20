#pragma once
#include "CardDate.h"

class Character;


class Card {

protected:

    const CardData m_data;
    int m_value;
    int m_ownerId;       // どのキャラのカードか

public:
    Card() = default;
    // 初期化
    Card(const CardData& arg_data, int arg_ownerId)
        :m_data(arg_data),
        m_value(-1),
        m_ownerId(arg_ownerId)
    {

    }

    // カードの情報取得
    CardData GetCardState() const { return m_data; }

    // カードの所持者のID
    int GetOwnerId() const { return m_ownerId; }

    // デストラクタ
    virtual ~Card() = default;

    // カードの使用
    void UseCard(Character* target);

    // カードの削除

};
