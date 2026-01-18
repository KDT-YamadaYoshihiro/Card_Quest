#include "CardBuildPool.h"

// プールの構築
void CardBuildPool::Build()
{
    if (m_build) {
        // 再生成防止
        return;
    }

    m_build = true;

	m_poolCards.clear();

	const auto& all = CardLoader::GetInstance().GetAll();

	for (const auto& [id, data] : all)
	{
        // プレイヤー用 3桁ID のみ
        if (id < 100 || id >= 1000) {
			// スキップ
            continue;
        }

        // createMax 0以下は非表示
        if (data.createMax <= 0)
        {
			// スキップ
            continue;
        }

        // createMax分生成
        auto cards =
            CardFactory::GetInstance().CreateBuildCards(id);

        for (auto& card : cards)
        {
            m_poolCards.emplace_back(std::move(card));
        }
    }

}

// プールカードの取得
const std::vector<std::unique_ptr<Card>>& CardBuildPool::GetPoolCards() const
{
	return m_poolCards;
}

//　指定indexのカードを取り出す
std::unique_ptr<Card> CardBuildPool::TakeCard(size_t index)
{
    if (index >= m_poolCards.size())
    {
		std::cout << "指定indexが範囲外です:" << index << std::endl;
        return nullptr;
    }

    auto card = std::move(m_poolCards[index]);

    m_poolCards.erase(m_poolCards.begin() + index);

    return card;
}

// カードプールに戻す
void CardBuildPool::ReturnCard(std::unique_ptr<Card>&& card)
{

    if (!card)
    {
        std::cout << "存在しないカードです" << std::endl;
        return;
    }

    m_poolCards.emplace_back(std::move(card));
}

// プールにある枚数取得
size_t CardBuildPool::GetCount() const
{
    return m_poolCards.size();
}
