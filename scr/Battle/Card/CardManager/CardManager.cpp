#include "CardManager.h"

void CardManager::AddDeckCard(const std::vector<Card>& cards)
{
	m_deck = cards;
}

void CardManager::DeckShuffle()
{
	std::shuffle(m_deck.begin(), m_deck.end(), m_rng);
}

void CardManager::DeckToHand(int arg_drawnum)
{
}

void CardManager::UseCard()
{
}

void CardManager::AddCemeteryCard()
{
}

void CardManager::CemeteryToDeck()
{
	AddDeckCard(m_cemetery);
}

std::vector<Card> CardManager::GetDeckCount() const
{
	return m_deck;
}

std::vector<Card> CardManager::GetHandCard() const
{
	return m_hand;
}

std::vector<Card> CardManager::GetCemeteryCard() const
{
	return m_cemetery;
}
