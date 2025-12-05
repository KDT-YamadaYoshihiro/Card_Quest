#pragma once
#include <vector>
#include "Card.h"

class Deck {
private:

	// デッキ内のカード群
    std::vector<Card> m_cards;

public:

	// デッキ初期化
    void Init(const std::vector<Character*>& allChars);
    // シャッフル
    void Shuffle();
    // ドロー
    std::vector<Card> Draw(int num);
};
