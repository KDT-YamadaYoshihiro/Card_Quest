#pragma once
#include "../../Battle/Card/CardDate.h"
#include "../../Battle/Card/Card.h"
#include "../../CSVLoad/CardLoader.h"

class CardFactory
{

	CardFactory() = default;
	~CardFactory() = default;

public:


	// コピー禁止
	CardFactory(const CardFactory&) = delete;
	CardFactory& operator = (const CardFactory&) = delete;
	static CardFactory& GetInstance()
	{
		static CardFactory instance;
		return instance;
	}

	// 単体生成
	std::unique_ptr<Card> Create(int arg_id)
	{
		const CardData* src = CardLoader::Instance().Get(arg_id);

		if (!src)
		{
			// エラー処理
			return nullptr;
		}

		CardData data = *src;
		if (arg_id == data.cardId) {
#ifdef _DEBUG
			std::cout << data.cardId << "を生成" << std::endl;
#endif // _DEBUG

			return std::make_unique<Card>(data);
		}
	}

	// 複数生成
	std::vector<std::unique_ptr<Card>> CreateDeck(const std::vector<int>& arg_id) {

		std::vector<std::unique_ptr<Card>> deck;
		deck.reserve(arg_id.size());

		for (int id : arg_id)
		{
			deck.emplace_back(Create(id));
		}

		return deck;
	}
};