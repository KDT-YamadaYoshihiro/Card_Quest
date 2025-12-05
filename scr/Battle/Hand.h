#pragma once
#include <vector>
#include "Card.h"

class Hand {
private:

	// 手札のカード群
    std::vector<Card> m_cards;

public:

	// カードを手札に追加
    void Add(const std::vector<Card>& cards);
	// カードを手札から削除
    void Remove(CardID id);
	// 手札のカードを取得
    const std::vector<Card>& GetCards() const;
};
