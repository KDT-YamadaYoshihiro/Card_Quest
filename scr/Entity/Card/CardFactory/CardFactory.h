#pragma once
#include <iostream>
#include "Entity/Card/CardDate.h"
#include "Entity/Card/Card.h"
#include "CSVLoad/CardLoader.h"

class CardFactory
{

	CardFactory() = default;
	~CardFactory() = default;

public:


	// ÉRÉsÅ[ã÷é~
	CardFactory(const CardFactory&) = delete;
	CardFactory& operator = (const CardFactory&) = delete;
	static CardFactory& GetInstance()
	{
		static CardFactory instance;
		return instance;
	}

	std::unique_ptr<Card> CreateCard(int cardId);

	std::vector<std::unique_ptr<Card>> CreateBuildCards(int cardId);

};