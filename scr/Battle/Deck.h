#pragma once
#include <vector>
#include "Card.h"

class Deck {
private:
    std::vector<Card> m_cards;

public:
    void Init(const std::vector<Character*>& allChars);
    void Shuffle();
    std::vector<Card> Draw(int num);
};
