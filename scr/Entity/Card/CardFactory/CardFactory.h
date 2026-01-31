#pragma once
#include <iostream>
#include "Entity/Card/CardDate.h"
#include "Entity/Card/Card.h"
#include "CSVLoad/CardLoader.h"

class CardFactory : public Singleton<CardFactory>
{
	friend class Singleton<CardFactory>;


public:

	std::unique_ptr<Card> CreateCard(int cardId);

	std::vector<std::unique_ptr<Card>> CreateBuildCards(int cardId);

};