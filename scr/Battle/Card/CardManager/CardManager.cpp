#include "CardManager.h"
#include "../../../Character/Character.h"

void CardManager::AddDeckCard(const std::vector<Card>& cards)
{
	m_deck =std::move(cards);
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
	m_hand[arg_handIndex].UseCard(arg_chara);
	// 墓地に移動
	AddCemeteryCard(m_hand[arg_handIndex]);
	// 手札から削除
	m_hand.erase(m_hand.begin() + arg_handIndex);
}

void CardManager::AddCemeteryCard(Card arg_card)
{
	// 墓地に移動
	m_cemetery.push_back(std::move(arg_card));
}

void CardManager::CemeteryToDeck()
{
	// 墓地からデッキに移動
	AddDeckCard(m_cemetery);
	// クリアする
	m_cemetery.clear();
}

std::size_t CardManager::GetDeckCount() const
{
	return m_deck.size();
}

std::size_t CardManager::GetHandCard() const
{
	return m_hand.size();
}

std::size_t CardManager::GetCemeteryCard() const
{
	return m_cemetery.size();
}
