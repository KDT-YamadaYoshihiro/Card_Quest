#pragma once
#include <vector>
#include "Card.h"

class Hand {
private:
    std::vector<Card> m_cards;

public:
    void Add(const std::vector<Card>& cards);
    void Remove(CardID id);
    const std::vector<Card>& GetCards() const;
};
