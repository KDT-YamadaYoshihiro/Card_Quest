#pragma once
#include <vector>
#include "../Card/Card.h"

class CardPool
{
private:

    std::vector<Card> m_cards;

public:
    void AddCard(const Card& card) {
        m_cards.push_back(card);
    }

    const std::vector<Card>& GetCards() const {
        return m_cards;
    }

    void Clear() {
        m_cards.clear();
    }
};
