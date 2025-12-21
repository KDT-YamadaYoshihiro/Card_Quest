#include "CardManager.h"
#include "../../Character/Character.h"

// 初期追加
void CardManager::InitDeck(std::vector<std::unique_ptr<Card>>&& arg_cards)
{
	m_deck = std::move(arg_cards);
}

// デッキにカードを追加
void CardManager::AddDeckCard(std::vector<std::unique_ptr<Card>>&& cards)
{
	for (auto& card : cards)
	{
		// 状態変更
		card->SetZone(CardZone::Deck);
		// 山札に移動
		m_deck.push_back(std::move(card));
	}
}

// デッキをシャッフルする
void CardManager::DeckShuffle()
{
	std::shuffle(m_deck.begin(), m_deck.end(), m_rng);
}

// デッキから手札に
void CardManager::DeckToHand(int arg_drawnum)
{
	// ドローできる環境かチェック
	if (m_deck.empty()) {
		CemeteryToDeck();
		DeckShuffle();
	}
	
	// 状態変化とドロー
	for (int i = 0; i < 5; i++) {
		auto card = std::move(m_deck.back());
		m_deck.pop_back();

		card->SetZone(CardZone::Hand);
		m_hand.push_back(std::move(card));
	}
}

// カードの使用
CardUseResult CardManager::UseCard(std::size_t arg_handIndex)
{
	auto& card = m_hand[arg_handIndex];

	CardUseResult result;
	result.effect = card->GetCardState();
	result.ownerID = card->GetOwnerId();

	AddCemeteryCard(std::move(card));
	m_hand.erase(m_hand.begin() + arg_handIndex);

	return result;

}

// 墓地にカードの移動
void CardManager::AddCemeteryCard(std::unique_ptr<Card>&& arg_card)
{
	// 状態変更
	arg_card->SetZone(CardZone::Grave);
	// 墓地に移動する
	m_cemetery.push_back(std::move(arg_card));
}

// 墓地からデッキに移動
void CardManager::CemeteryToDeck()
{
	// 墓地からデッキに移動
	AddDeckCard(std::move(m_cemetery));
	// クリアする
	m_cemetery.clear();
}

// デッキ枚数の取得
int CardManager::GetDeckCount() const
{
	return m_deck.size();
}

// 手札枚数の取得
int CardManager::GetHandCount() const
{
	return m_hand.size();
}

// 墓地枚数の取得
int CardManager::GetCemeteryCount() const
{
	return m_cemetery.size();
}

// デッキカードの取得
const std::vector<std::unique_ptr<Card>>& CardManager::GetDeckCard() const
{
	return m_deck;
}

// 墓地カードを取得
const std::vector<std::unique_ptr<Card>>& CardManager::GetCemetyeryCard() const
{
	return m_cemetery;
}

// 手札カードを取得
const std::vector<std::unique_ptr<Card>>& CardManager::GetHandCard() const
{
	return m_hand;
}
