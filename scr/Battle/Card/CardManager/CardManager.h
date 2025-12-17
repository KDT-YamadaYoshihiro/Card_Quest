#pragma once
#include <memory>
#include <vector>
#include <random>
#include <algorithm>
#include "../Card.h"

// カード管理
class CardManager : public Card
{

private:

	// デッキカード
	std::vector<Card> m_deck;

	// 手札カード
	std::vector<Card> m_hand;

	// 墓地カード
	std::vector<Card> m_cemetery;

	std::mt19937 m_rng{ std::random_device{}() };

public:

	// カードの追加（初期化）
	void AddDeckCard(const std::vector<Card>& cards);

	// デッキをシャッフル
	void DeckShuffle();

	// デッキから引数分手札に移動
	void DeckToHand(int arg_drawnum);

	// 手札からカードを使用
	void UseCard();

	// 使用したカードを墓地に移動
	void AddCemeteryCard();

	// 墓地からデッキに戻す
	void CemeteryToDeck();

	// デッキの残り枚数を取得
	std::vector<Card> GetDeckCount() const;

	// 手札カードを取得
	std::vector<Card> GetHandCard() const;

	// 墓地にあるカードの確認	
	std::vector<Card> GetCemeteryCard() const;



};

