#include "DeckBuildSystem.h"
#include "../CardBuildPool/CardBuildPool.h"
#include <iostream>

// プールからデッキに追加
bool DeckBuildSystem::AddFromPool(int poolIndex)
{
    if (m_deckCards.size() >= MAX_DECK_SIZE)
    {
		std::cout << "デッキの上限枚数に達しています" << std::endl;
        return false;
    }

	// プールからカードを取得    
    auto card = CardBuildPool::GetInstance().TakeCard(poolIndex);

    if (!card)
    {
		std::cout << "プールからカードを取得できませんでした: " << poolIndex << std::endl;
        return false;
    }

    m_deckCards.emplace_back(std::move(card));

    return true;
}

// デッキからプールに戻す
bool DeckBuildSystem::RemoveFromPool(int deckIndex)
{
    if (deckIndex >= m_deckCards.size())
    {
		std::cout << "指定indexが範囲外です:" << deckIndex << std::endl;
        return false;
    }

	// デッキからカードを取り出す
    auto card = std::move(m_deckCards[deckIndex]);
	// デッキから削除
    m_deckCards.erase(m_deckCards.begin() + deckIndex);

	// カードをプールに戻す
    CardBuildPool::GetInstance().ReturnCard(std::move(card));

    return true;
}

// デッキ内カード取得
const std::vector<std::unique_ptr<Card>>& DeckBuildSystem::GetDeckCards() const
{
	return m_deckCards;
}

// デッキ内枚数取得
int DeckBuildSystem::GetDeckSize() const
{
	return static_cast<int>(m_deckCards.size());
}

// デッキ編成上限枚数取得
int DeckBuildSystem::GetMaxDeckSize() const
{
	return MAX_DECK_SIZE;
}

// デッキ編成完了判定
bool DeckBuildSystem::IsComlete() const
{
	return m_deckCards.size() == MAX_DECK_SIZE;
}
