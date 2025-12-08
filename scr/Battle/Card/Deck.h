#pragma once
#include <vector>
#include <random>
#include <algorithm>
#include "Card.h"

class Character;

class Deck
{
private:
    std::vector<Card> m_cards;
    std::mt19937 m_rng{ std::random_device{}() };

public:

    void Init(const std::vector<Card>& cards) {
        m_cards = cards;
        Shuffle();
    }

    void Shuffle() {
        std::shuffle(m_cards.begin(), m_cards.end(), m_rng);
    }

    std::vector<Card> Draw(int num) {
        std::vector<Card> result;

        for (int i = 0; i < num && !m_cards.empty(); i++) {
            result.push_back(m_cards.back());
            m_cards.pop_back();
        }

        return result;
    }

    bool Empty() const { return m_cards.empty(); }
};
