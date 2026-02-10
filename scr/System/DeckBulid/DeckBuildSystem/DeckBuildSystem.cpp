#include "DeckBuildSystem.h"
#include "System/DeckBulid/CardBuildPool/CardBuildPool.h"
#include "Entity/Card/CardFactory/CardFactory.h"
#include "View/ConsoleView/ConsoleView.h"
#include "Entity/Card/CardManager/CardManager.h"

void DeckBuildSystem::LoadDeckFromManager()
{

    m_deckCards.clear();

    // CardManager から現在のデッキ ID リストを取得
    const auto& currentDeckIds = CardManager::GetInstance().GetCardMasterIds();

    for (int id : currentDeckIds)
    {
        // ID から Card クラスのインスタンスを生成して格納
        auto card = CardFactory::GetInstance().CreateCard(id);
        if (card)
        {
            m_deckCards.push_back(std::move(card));
        }
    }
}

// 初期化
void DeckBuildSystem::Init()
{

    LoadDeckFromManager();

	RebuildDisplayPool();

    RebuildDisplayDeck();
}

// 更新
void DeckBuildSystem::Update(sf::Vector2f mousePos, bool isClick, bool isDragging, bool released, float wheelDelta)
{

    // ホイール
    if (wheelDelta != 0.f)
    {
        m_poolScrollX += wheelDelta * 40.f;
        ClampPoolScroll();
    }

    // 押した瞬間
    if (isClick && !m_prevClick)
    {
        m_mouseDownPos = mousePos;
        m_lastMouseX = mousePos.x;
        m_dragStarted = false;
        m_poolDragging = false;
        m_deckDragging = false;
    }

    // ドラッグ判定
    if (isDragging)
    {
        float dragDist = std::abs(mousePos.x - m_mouseDownPos.x);

        if (dragDist > 5.f) // ← クリック/ドラッグ分離閾値
        {
            m_dragStarted = true;

            sf::FloatRect poolArea(m_poolStartPos, { 800.f, 200.f });
            sf::FloatRect deckArea(m_deckStartPos, { 800.f, 200.f });

            if (poolArea.contains(m_mouseDownPos))
                m_poolDragging = true;
            else if (deckArea.contains(m_mouseDownPos))
                m_deckDragging = true;
        }
    }

    // ドラッグ中
    if (m_dragStarted)
    {
        float deltaX = mousePos.x - m_lastMouseX;
        m_lastMouseX = mousePos.x;

        if (m_poolDragging)
        {
            m_poolScrollX += deltaX;
            ClampPoolScroll();
        }

        if (m_deckDragging)
        {
            m_deckScrollX += deltaX;
            ClampDeckScroll();
        }
    }

    // 離した瞬間
    if (released)
    {
        if (!m_dragStarted)
        {
            if (HandleDeckClick(mousePos))
            {
                m_prevClick = isClick;
                return;
            }
            HandlePoolClick(mousePos);
        }

        m_poolDragging = false;
        m_deckDragging = false;
        m_dragStarted = false;
    }

    m_prevClick = isClick;
}

// 描画
void DeckBuildSystem::Draw(sf::RenderWindow& window, const sf::Font& font)
{

	// Scene の案内文字
	sf::Text titleText(font, "Deck Build");
	titleText.setCharacterSize(50);
	titleText.setFillColor(sf::Color::White);
	titleText.setPosition({ 50.f, 10.f });
	window.draw(titleText);

    // 案内文字
	sf::Text instructionText(font, "-Please select the cards.-");
	instructionText.setCharacterSize(30);
	instructionText.setFillColor(sf::Color::White);
	instructionText.setPosition({ 430.f, 80.f });
	window.draw(instructionText);


    // ===== デッキ側（上段） =====

     //　背景描画
    sf::RectangleShape deckBG({ 1500.f, 250.f });
    deckBG.setPosition({ 0.f, m_deckStartPos.y - 40.f });
    deckBG.setFillColor(sf::Color(20, 20, 20, 200)); // 半透明の黒
    window.draw(deckBG);

	// 文字描画
    sf::Text deckText(font, "Deck");
    deckText.setCharacterSize(24);
    deckText.setFillColor(sf::Color::White);
    deckText.setPosition({ 10.f, m_deckStartPos.y - 35.f });
	window.draw(deckText);

    // 枚数表示
    sf::Text countText(font, std::to_string(m_deckCards.size()) + " / 30");
    countText.setCharacterSize(20);
    countText.setFillColor(sf::Color::White);
    countText.setPosition({ 100.f,  m_deckStartPos.y - 35.f });
    window.draw(countText);


    for (size_t i = 0; i < m_displayDeck.size(); ++i)
    {
        sf::Vector2f pos = m_deckStartPos + sf::Vector2f(i * m_cardSpacing + m_deckScrollX, 0.f);

        // カード本体の描画
        m_renderer->DrawSingleCard(font, window, pos,m_displayDeck[i].card->GetCardState(),"");

        // 枚数表示 (*n)
        if (m_displayDeck[i].count > 1)
        {
            sf::Text countText(font, "x" + std::to_string(m_displayDeck[i].count));
            countText.setCharacterSize(20);
            countText.setFillColor(sf::Color::Yellow);
            countText.setOutlineColor(sf::Color::Black);
            countText.setOutlineThickness(2.f);
            countText.setPosition(pos + sf::Vector2f(80.f, 130.f)); // 右下付近
            window.draw(countText);
        }
    }

    // ===== プール側（下段） =====

	//　背景描画
    sf::RectangleShape poolBG({ 1500.f, 250.f });
    poolBG.setPosition({ 0.f,m_poolStartPos.y - 40.f });
    poolBG.setFillColor(sf::Color(20, 20, 20, 200)); // 半透明の黒
    window.draw(poolBG);


	// 文字描画
	sf::Text poolText(font, "Card");
	poolText.setCharacterSize(24);
	poolText.setFillColor(sf::Color::White);
	poolText.setPosition({ 10.f, m_poolStartPos.y - 35.f });
	window.draw(poolText);

	// プール内カード描画
    const auto& pool = CardBuildPool::GetInstance().GetPoolCards();
    for (size_t i = 0; i < m_displayPool.size(); ++i)
    {
        sf::Vector2f pos = { m_poolStartPos.x + m_cardSpacing * i + m_poolScrollX, m_poolStartPos.y };

        m_renderer->DrawSingleCard(font, window, pos, m_displayPool[i].card->GetCardState(),"");

        // ×n 表示
        sf::Text countText(font, "x" + std::to_string(m_displayPool[i].count));
        countText.setCharacterSize(18);
        countText.setFillColor(sf::Color::Black);
        countText.setPosition({ pos.x + 85.f, pos.y + 130.f });
        window.draw(countText);
    }

}

// プールからデッキに追加
bool DeckBuildSystem::AddFromPool(int poolIndex)
{
    if (m_deckCards.size() >= MAX_DECK_SIZE)
    {
        ConsoleView::GetInstance().Add("デッキの上限枚数に達しています\n");

        return false;
    }

    int targetId = m_displayPool[poolIndex].card->GetCardState().cardId;
	// プールからカードを取得    
    auto card = CardBuildPool::GetInstance().TakeCard(targetId);

    if (!card)
    {
        ConsoleView::GetInstance().Add("プールからカードを取得できませんでした: " + std::to_string(targetId) + "\n");
        return false;
    }

    m_deckCards.emplace_back(std::move(card));

    // 表示用リストを更新
    RebuildDisplayPool();
    RebuildDisplayDeck();

    return true;
}

// デッキからプールに戻す
bool DeckBuildSystem::RemoveFromPool(int deckIndex)
{
    if (deckIndex >= m_deckCards.size())
    {
        ConsoleView::GetInstance().Add("指定indexが範囲外です:"  +  std::to_string(deckIndex) + "\n");
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
std::vector<int> DeckBuildSystem::TakeDeck()
{
    std::vector<int> result;
    for (auto& card : m_deckCards)
    {
        result.push_back(card->GetCardState().cardId);
    }
    return result;
}

// プールクリック処理
bool DeckBuildSystem::HandlePoolClick(sf::Vector2f mousePos)
{
    // ドラッグ中はクリック処理しない
    if (m_poolDragging || m_deckDragging)
    {
        return false;
    }
	// デッキ上限チェック
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
            return AddFromPool(static_cast<int>(i));
        }
    }
    return false;

}

// デッキクリック処理
bool DeckBuildSystem::HandleDeckClick(sf::Vector2f mousePos)
{
    // ドラッグ中はクリック処理しない
    if (m_poolDragging || m_deckDragging)
    {
        return false;
    }

    for (size_t i = 0; i < m_displayDeck.size(); ++i)
    {
        sf::FloatRect rect({ m_deckStartPos.x + m_cardSpacing * i + m_deckScrollX, m_deckStartPos.y }, { 120.0f, 160.0f });

        if (rect.contains(mousePos))
        {
            // クリックされた表示上のカードIDを特定
            int targetId = m_displayDeck[i].card->GetCardState().cardId;

            // 実データ(m_deckCards)からそのIDを1つだけ探して削除
            for (auto it = m_deckCards.begin(); it != m_deckCards.end(); ++it)
            {
                if ((*it)->GetCardState().cardId == targetId)
                {
                    // プール(CardBuildPool)に実体を戻す
                    CardBuildPool::GetInstance().ReturnCard(std::move(*it));
                    // リストから除去
                    m_deckCards.erase(it);
                    break;
                }
            }

            // 表示を再構築
            RebuildDisplayDeck();
            RebuildDisplayPool();
            return true;
        }
    }
	return false;
}

// ドラッグ処理
void DeckBuildSystem::HandleDrag(sf::Vector2f mousePos, bool isClick)
{

    if (!isClick)
    {
        return;
    }

    float deltaX = mousePos.x - m_lastMouseX;
    m_lastMouseX = mousePos.x;

    // プールドラッグ
    if (m_poolDragging)
    {
        m_poolScrollX += deltaX;
        ClampPoolScroll();
    }

    // デッキドラッグ
    if (m_deckDragging)
    {
        m_deckScrollX += deltaX;
        ClampDeckScroll();
    }

}

// プール再構築
void DeckBuildSystem::RebuildDisplayPool()
{
	// クリア
    m_displayPool.clear();
	// カウントマップ作成
    std::unordered_map<int, int> countMap;
	// 最初の出現indexマップ作成
    std::unordered_map<int, size_t> firstIndexMap;
	// プール内カード取得
    const auto& pool = CardBuildPool::GetInstance().GetPoolCards();

    for (size_t i = 0; i < pool.size(); ++i)
    {
		// カードID取得
        int id = pool[i]->GetCardState().cardId;
        countMap[id]++;

        if (!firstIndexMap.contains(id))
        {
			// 最初の出現index保存
            firstIndexMap[id] = i;
        }
    }

    for (const auto& [id, count] : countMap)
    {
		// カード生成
        auto card = CardFactory::GetInstance().CreateCard(id);
		// プール表示用に追加
        m_displayPool.push_back({ std::move(card),count});
    }
}

void DeckBuildSystem::RebuildDisplayDeck()
{

    m_displayDeck.clear();
    if (m_deckCards.empty())
    {
        return;
    }

    std::unordered_map<int, int> countMap;
    std::vector<int> idOrder; // 追加された順序を維持したい場合

    for (const auto& card : m_deckCards)
    {
        int id = card->GetCardState().cardId;
        if (countMap[id] == 0) {
            idOrder.push_back(id);
        }
        countMap[id]++;
    }

    for (int id : idOrder)
    {
        auto card = CardFactory::GetInstance().CreateCard(id);
        m_displayDeck.push_back({ std::move(card), countMap[id] });
    }
}

// スクロール制限
void DeckBuildSystem::ClampPoolScroll()
{
	// プール側スクロール制限
	// プール内カード数に基づく最大幅計算
    float maxWidth = static_cast<float>(m_displayPool.size()) * m_cardSpacing;
    float viewWidth = 800.f;
	// 最小スクロール位置計算
    float minScroll = std::min(0.f, viewWidth - maxWidth);
	// スクロール位置制限
    m_poolScrollX = std::clamp(m_poolScrollX, minScroll, 0.f);
}

void DeckBuildSystem::ClampDeckScroll()
{
	// デッキ側スクロール制限
	// デッキ内カード数に基づく最大幅計算
    float maxWidth = static_cast<float>(m_deckCards.size()) * m_cardSpacing;
    float viewWidth = 800.f;
	// 最小スクロール位置計算
    float minScroll = std::min(0.f, viewWidth - maxWidth);
	// スクロール位置制限
    m_deckScrollX = std::clamp(m_deckScrollX, minScroll, 0.f);
}
