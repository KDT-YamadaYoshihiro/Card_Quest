#pragma once
#include <vector>
#include "Card.h"

class Hand
{
private:
    std::vector<Card> m_hand;

public:

    void Add(const std::vector<Card>& cards) {
        for (auto& c : cards) {
            m_hand.push_back(c);
        }
    }

    void Remove(int cardID) {
        m_hand.erase(
            std::remove_if(m_hand.begin(), m_hand.end(),
                [cardID](const Card& c) { return c.m_data.cardId == cardID; }),
            m_hand.end()
        );
    }

    const std::vector<Card>& Get() const { return m_hand; }

    bool Empty() const { return m_hand.empty(); }
};
