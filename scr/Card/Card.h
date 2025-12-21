#pragma once
#include "CardDate.h"

class Character;


class Card {

protected:

    const CardData m_data;
    CardZone m_cardZone;
    int m_value;
    int m_ownerId;       // どのキャラのカードか

public:
    Card() = default;
    // 初期化
    Card(const CardData& arg_data, const CardZone& arg_zone, int arg_ownerId)
        :m_data(arg_data),
        m_cardZone(arg_zone),
        m_value(-1),
        m_ownerId(arg_ownerId)
    {

    }

    // カードの状態変更と取得
    void SetZone(CardZone arg_zone){ m_cardZone = arg_zone; }
    CardZone GetCardZone() const { return m_cardZone; }

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
