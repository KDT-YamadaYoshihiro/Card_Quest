#include "CardFactory.h"
#include "CSVLoad/CardLoader.h"

// 単体生成
std::unique_ptr<Card> CardFactory::CreateCard(int cardId)
{
	const CardData* src = CardLoader::GetInstance().Get(cardId);

	if (!src)
	{
		std::cout << "dataが見つかりませんでした" << cardId << std::endl;
		return nullptr;
	}

	CardData data = *src;

	return std::make_unique<Card>(data, CardZone::None);

}

// 一括生成
std::vector<std::unique_ptr<Card>> CardFactory::CreateBuildCards(int cardId)
{

	std::vector<std::unique_ptr<Card>> result;

	// プレイヤー用のみ作成
	if (cardId < 100 || cardId >= 1000) {
		std::cout << "このIDはプレイヤー用ではございません" << std::endl;
		return result;
	}

	const CardData* src = CardLoader::GetInstance().Get(cardId);

	if (!src)
	{
		std::cout << "dataが見つかりませんでした" << cardId << std::endl;
		return result;
	}

	result.reserve(src->createMax);

	for (int i = 0; i < src->createMax; i++) {

		auto card = CreateCard(cardId);
		if (card)
		{
			result.emplace_back(std::move(card));
		}
	}

	return result;
}
