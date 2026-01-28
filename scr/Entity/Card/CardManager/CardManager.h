#pragma once
#include <unordered_map>
#include <vector>
#include <random>
#include <algorithm>
#include "Entity/Card/CardDate.h"

class Character;

// カード管理（IDベース）
class CardManager
{
private:
    std::unordered_map<int, CardData> m_cardMaster;
    std::vector<int> m_deck;
    std::vector<int> m_cemetery;

    std::mt19937 m_rng;

    CardManager();

public:
    CardManager(const CardManager&) = delete;
    CardManager& operator=(const CardManager&) = delete;

    static CardManager& GetInstance()
    {
        static CardManager instance;
        return instance;
    }

    // 初期化
    void InitCardMaster(const std::unordered_map<int, CardData>& cards);
    void InitDeck(const std::vector<int>& deckCardIds);

    // デッキ操作
    void ShuffleDeck();
    bool DrawCard(int& cardId);

    // 墓地操作
    void SendCardIdToCemetery(int cardId);
    void RecycleCemeteryToDeck();

    // 参照
    const CardData& GetCardData(int cardId) const;

    // 取得
    int GetDeckCount() const;
    int GetCemeteryCount() const;

};
