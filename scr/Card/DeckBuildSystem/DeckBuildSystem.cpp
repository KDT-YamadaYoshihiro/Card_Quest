#include "DeckBuildSystem.h"
#include "../CardBuildPool/CardBuildPool.h"
#include "../CardFactory/CardFactory.h"
#include <iostream>

void DeckBuildSystem::Init()
{
	RebuildDisplayPool();
}

// 更新
void DeckBuildSystem::Update(sf::Vector2f mousePos, bool isClick, float wheelDelta)
{
    // 横スクロール（ホイール）
    m_poolScrollX += wheelDelta * 40.f;
    m_poolScrollX = std::min(0.f, m_poolScrollX);


    if (!isClick)
    {
		// クリックされていなければ何もしない
        return;
    }

    if (HandleDeckClick(mousePos))
    {
        return;
    }
    HandlePoolClick(mousePos);
}

// スクロール更新
void DeckBuildSystem::UpdateScroll(int wheelDelta)
{
    m_poolScrollX += wheelDelta * m_poolScrollSpeed;

    // 左端制限
    m_poolScrollX = std::min(m_poolScrollX, 0.0f);

    // 右端制限（カード総幅から算出）
    float maxWidth = static_cast<float>(m_displayPool.size()) * m_cardSpacing;

    float viewWidth = 800.0f; // 表示領域幅（仮）
    float minScroll = std::min(0.0f, viewWidth - maxWidth);

    m_poolScrollX = std::max(m_poolScrollX, minScroll);

}


// 描画
void DeckBuildSystem::Draw(sf::RenderWindow& window, const sf::Font& font)
{

    // ===== デッキ側（上段） =====
    for (size_t i = 0; i < m_deckCards.size(); ++i)
    {
        sf::Vector2f pos = { m_deckStartPos.x + m_cardSpacing * i , m_deckStartPos.y };

        m_renderer->DrawHand(font, window, pos, *m_deckCards[i]);
    }

    // ===== プール側（下段） =====
    const auto& pool = CardBuildPool::GetInstance().GetPoolCards();

    for (size_t i = 0; i < m_displayPool.size(); ++i)
    {
        sf::Vector2f pos = { m_poolStartPos.x + m_cardSpacing * i+ m_poolScrollX, m_poolStartPos.y };

        m_renderer->DrawHand(font, window, pos, *m_displayPool[i].card);

        // ×n 表示
        sf::Text countText(font, "x" + std::to_string(m_displayPool[i].count));
        countText.setCharacterSize(18);
        countText.setFillColor(sf::Color::White);
        countText.setPosition({ pos.x + 85.f, pos.y + 130.f });
        window.draw(countText);
    }

    // ===== 枚数表示 =====
    sf::Text countText(font,std::to_string(m_deckCards.size()) + " / 30");
    countText.setCharacterSize(20);
    countText.setFillColor(sf::Color::White);
    countText.setPosition({ 50.f, 250.f });
    window.draw(countText);

}

// プール再構築
void DeckBuildSystem::RebuildDisplayPool()
{

    m_displayPool.clear();

    std::unordered_map<int, int> countMap;
    std::unordered_map<int, size_t> firstIndexMap;
    const auto& pool = CardBuildPool::GetInstance().GetPoolCards();

    for (size_t i = 0; i < pool.size(); ++i)
    {
        int id = pool[i]->GetCardState().cardId;
        countMap[id]++;

        if (!firstIndexMap.contains(id))
        {
            firstIndexMap[id] = i;
        }
    }

    for (const auto& [id, count] : countMap)
    {
        auto card = CardFactory::GetInstance().CreateCard(id);
        m_displayPool.push_back({std::move(card),count,firstIndexMap[id]});
    }
}


// プールからデッキに追加
bool DeckBuildSystem::AddFromPool(int poolIndex)
{
    if (m_deckCards.size() >= MAX_DECK_SIZE)
    {
		std::cout << "デッキの上限枚数に達しています" << std::endl;
        return false;
    }

	// プールからカードを取得    
    auto card = CardBuildPool::GetInstance().TakeCard(poolIndex);

    if (!card)
    {
		std::cout << "プールからカードを取得できませんでした: " << poolIndex << std::endl;
        return false;
    }

    m_deckCards.emplace_back(std::move(card));

    return true;
}

// デッキからプールに戻す
bool DeckBuildSystem::RemoveFromPool(int deckIndex)
{
    if (deckIndex >= m_deckCards.size())
    {
		std::cout << "指定indexが範囲外です:" << deckIndex << std::endl;
        return false;
    }

	// デッキからカードを取り出す
    auto card = std::move(m_deckCards[deckIndex]);
	// デッキから削除
    m_deckCards.erase(m_deckCards.begin() + deckIndex);

	// カードをプールに戻す
    CardBuildPool::GetInstance().ReturnCard(std::move(card));

    return true;
}

// デッキ内カード取得
const std::vector<std::unique_ptr<Card>>& DeckBuildSystem::GetDeckCards() const
{
	return m_deckCards;
}

// デッキ内枚数取得
int DeckBuildSystem::GetDeckSize() const
{
	return static_cast<int>(m_deckCards.size());
}

// デッキ編成上限枚数取得
int DeckBuildSystem::GetMaxDeckSize() const
{
	return MAX_DECK_SIZE;
}

// デッキ編成完了判定
bool DeckBuildSystem::IsComplete() const
{
	return m_deckCards.size() == MAX_DECK_SIZE;
}

// デッキを引き渡す
std::vector<std::unique_ptr<Card>> DeckBuildSystem::TakeDeck()
{
    return std::move(m_deckCards);
}

// プールクリック処理
bool DeckBuildSystem::HandlePoolClick(sf::Vector2f mousePos)
{

    if (m_deckCards.size() >= MAX_DECK_SIZE)
    {
        return false;
    }

    auto& pool = CardBuildPool::GetInstance();

    const auto& cards = pool.GetPoolCards();

    for (size_t i = 0; i < m_displayPool.size(); ++i)
    {
        sf::FloatRect rect({ m_poolStartPos.x + m_cardSpacing * i + m_poolScrollX, m_poolStartPos.y }, { 120.0f, 160.0f });

        if (rect.contains(mousePos))
        {
			int index = m_displayPool[i].poolIndex;
            auto card = pool.TakeCard(index);
            if (card)
            {
                m_deckCards.emplace_back(std::move(card));
				RebuildDisplayPool();
                return true;
            }
        }
    }

    return false;

}

// デッキクリック処理
bool DeckBuildSystem::HandleDeckClick(sf::Vector2f mousePos)
{

    for (size_t i = 0; i < m_deckCards.size(); ++i)
    {
        sf::FloatRect rect({ m_deckStartPos.x + m_cardSpacing * i , m_deckStartPos.y }, { 120.0f, 160.0f });

        if (rect.contains(mousePos))
        {
            CardBuildPool::GetInstance().ReturnCard(std::move(m_deckCards[i]));
            m_deckCards.erase(m_deckCards.begin() + i);
            RebuildDisplayPool();
            return true;
        }
    }
	return false;
}
