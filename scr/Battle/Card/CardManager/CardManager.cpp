#include "CardManager.h"
#include "../../../Character/Character.h"

void CardManager::InitDeck(std::vector<std::unique_ptr<Card>>&& arg_cards)
{
	m_deck = std::move(arg_cards);
}

void CardManager::AddDeckCard(std::vector<std::unique_ptr<Card>>&& cards)
{
	for (auto& card : cards)
	{
		m_deck.push_back(std::move(card));
	}
}

void CardManager::DeckShuffle()
{
	std::shuffle(m_deck.begin(), m_deck.end(), m_rng);
}

void CardManager::DeckToHand(int arg_drawnum)
{
	// ドローできる環境かチェック
	if (m_deck.empty()) {
		CemeteryToDeck();
		DeckShuffle();
	}
	
	// ドロー
	m_hand.push_back(std::move(m_deck.back()));
	m_deck.pop_back();
	
}

void CardManager::UseCard(std::size_t arg_handIndex, Character* arg_chara)
{
	// カードを使用
	m_hand[arg_handIndex]->UseCard(arg_chara);
	// 墓地に移動
	AddCemeteryCard(std::move(m_hand[arg_handIndex]));
	// 手札から削除
	m_hand.erase(m_hand.begin() + arg_handIndex);
}

void CardManager::AddCemeteryCard(std::unique_ptr<Card>&& arg_card)
{
	// 墓地に移動
	m_cemetery.push_back(std::move(arg_card));
}

void CardManager::CemeteryToDeck()
{
	// 墓地からデッキに移動
	AddDeckCard(std::move(m_cemetery));
	// クリアする
	m_cemetery.clear();
}

int CardManager::GetDeckCount() const
{
	return m_deck.size();
}

int CardManager::GetHandCard() const
{
	return m_hand.size();
}

int CardManager::GetCemeteryCard() const
{
	return m_cemetery.size();
}
