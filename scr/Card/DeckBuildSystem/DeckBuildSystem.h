#pragma once
#include <vector>
#include <memory>
#include "../../Card/Card.h"


class DeckBuildSystem
{

	// デッキ編成上限枚数
	static const int MAX_DECK_SIZE = 30;
	// デッキ内カード
	std::vector<std::unique_ptr<Card>> m_deckCards;

public:

	// プールからデッキに追加
	bool AddFromPool(int poolIndex);

	// デッキからプールに戻す
	bool RemoveFromPool(int deckIndex);

	// デッキ内カード取得
	const std::vector<std::unique_ptr<Card>>& GetDeckCards() const;

	// デッキ内枚数取得
	int GetDeckSize() const;

	// デッキ編成上限枚数取得
	int GetMaxDeckSize() const;

	// デッキ編成完了判定
	bool IsComlete() const;

};

