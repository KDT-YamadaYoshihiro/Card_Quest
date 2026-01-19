#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include "../Card.h"
#include "../CardFactory/CardFactory.h"
#include "../../CSVLoad/CardLoader.h"

class CardBuildPool
{

	std::vector<std::unique_ptr<Card>> m_poolCards;
	bool m_build = false;

	CardBuildPool() = default;
	virtual ~CardBuildPool() = default;

public:


	// コピー禁止
	CardBuildPool(const CardBuildPool&) = delete;
	CardBuildPool& operator = (const CardBuildPool&) = delete;
	static CardBuildPool& GetInstance()
	{
		static CardBuildPool instance;
		return instance;
	}

	void Build();

	// プールカードの取得
	const std::vector<std::unique_ptr<Card>>& GetPoolCards() const;

	// 指定indexのカードを取り出す
	std::unique_ptr<Card> TakeCard(int id);

	// カードをプールに戻す
	void ReturnCard(std::unique_ptr<Card>&& card);

	// 現在の所持数
	size_t GetCount() const;

};

