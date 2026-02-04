#include "CardManager.h"
#include "Entity/Character/Character.h"
#include <cassert>

// 初期化
CardManager::CardManager()
{
    std::random_device rd;
    m_rng.seed(rd());
}

// カードマスタ登録
void CardManager::InitCardMaster(const std::unordered_map<int, CardData>& cards)
{
    m_cardMaster = cards;
}

// デッキ初期化
void CardManager::InitDeck(const std::vector<int>& deckCardIds)
{
    m_deck = deckCardIds;
    ShuffleDeck();
}

const std::vector<int>& CardManager::GetCardMasterIds() const
{
	return m_deck;
}

// シャッフル
void CardManager::ShuffleDeck()
{
    std::shuffle(m_deck.begin(), m_deck.end(), m_rng);
}

// ドロー
bool CardManager::DrawCard(int& cardId)
{
    // デッキの枚数確認
    if (m_deck.empty())
    {
        if (m_deck.empty())
        {
            return false;
        }
    }

    cardId = m_deck.back();
    m_deck.pop_back();
    return true;
}

// 墓地送り
void CardManager::SendCardIdToCemetery(int cardId)
{
    m_cemetery.push_back(cardId);
}

// 墓地 → デッキ
void CardManager::RecycleCemeteryToDeck()
{
    if (m_cemetery.empty()) return;

    m_deck.insert(m_deck.end(), m_cemetery.begin(), m_cemetery.end());
    m_cemetery.clear();
    ShuffleDeck();
}

// CardData 取得
const CardData& CardManager::GetCardData(int cardId) const
{
    auto it = m_cardMaster.find(cardId);
    assert(it != m_cardMaster.end());
    return it->second;
}

// 枚数取得
int CardManager::GetDeckCount() const
{
    return static_cast<int>(m_deck.size());
}

int CardManager::GetCemeteryCount() const
{
    return static_cast<int>(m_cemetery.size());
}
