#include "CardFactory.h"
#include "CSVLoad/CardLoader.h"
#include "View/ConsoleView/ConsoleView.h"

// 単体生成
std::unique_ptr<Card> CardFactory::CreateCard(int cardId)
{
	const CardData* src = CardLoader::GetInstance().Get(cardId);

	if (!src)
	{
		ConsoleView::GetInstance().Add("dataが見つかりませんでした" + std::to_string(cardId) + "\n");
		return nullptr;
	}

	CardData data = *src;

	ConsoleView::GetInstance().Add("カード:" + data.name + "を生成" + "\n");
	return std::make_unique<Card>(data, CardZone::None);

}

// 一括生成
std::vector<std::unique_ptr<Card>> CardFactory::CreateBuildCards(int cardId)
{

	std::vector<std::unique_ptr<Card>> result;

	// プレイヤー用のみ作成
	if (cardId < 100 || cardId >= 1000) {
		std::cout << "このIDはプレイヤー用ではございません" << std::endl;
		ConsoleView::GetInstance().Add("このIDはプレイヤー用ではございません" + std::to_string(cardId) + "\n");
		return result;
	}

	const CardData* src = CardLoader::GetInstance().Get(cardId);

	if (!src)
	{
		ConsoleView::GetInstance().Add("dataが見つかりませんでした" + std::to_string(cardId) + "\n");

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
