#include "DeckBuildSystem.h"
#include "System/DeckBulid/CardBuildPool/CardBuildPool.h"
#include "Entity/Card/CardFactory/CardFactory.h"
#include "View/ConsoleView/ConsoleView.h"
#include "Entity/Card/CardManager/CardManager.h"

void DeckBuildSystem::LoadDeckFromManager()
{

    m_deckCards.clear();

    // CardManager ���猻�݂̃f�b�L ID ���X�g���擾
    const auto& currentDeckIds = CardManager::GetInstance().GetCardMasterIds();

    for (int id : currentDeckIds)
    {
        // ID ���� Card �N���X�̃C���X�^���X�𐶐����Ċi�[
        auto card = CardFactory::GetInstance().CreateCard(id);
        if (card)
        {
            m_deckCards.push_back(std::move(card));
        }
    }
}

// ������
void DeckBuildSystem::Init()
{

    LoadDeckFromManager();

	RebuildDisplayPool();

    RebuildDisplayDeck();
}

// �X�V
void DeckBuildSystem::Update(sf::Vector2f mousePos, bool isClick, bool isDragging, bool released, float wheelDelta)
{

    // �z�C�[��
    if (wheelDelta != 0.f)
    {
        m_poolScrollX += wheelDelta * 40.f;
        ClampPoolScroll();
    }

    // �������u��
    if (isClick && !m_prevClick)
    {
        m_mouseDownPos = mousePos;
        m_lastMouseX = mousePos.x;
        m_dragStarted = false;
        m_poolDragging = false;
        m_deckDragging = false;
    }

    // �h���b�O����
    if (isDragging)
    {
        float dragDist = std::abs(mousePos.x - m_mouseDownPos.x);

        if (dragDist > 5.f) // �� �N���b�N/�h���b�O����臒l
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

    // �h���b�O��
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

    // �������u��
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

// �`��
void DeckBuildSystem::Draw(sf::RenderWindow& window, const sf::Font& font)
{

	// シーンタイトル
	sf::Text titleText(font, "Deck Build");
	titleText.setCharacterSize(ui_font_sizes::TITLE_SIZE);
	titleText.setFillColor(sf::Color::White);
	titleText.setPosition({ 50.f, ui_layout::DECK_X_START });
	window.draw(titleText);

    // 指示テキスト
    sf::Text instructionText(font, "-Please select the cards.-");
    instructionText.setCharacterSize(ui_font_sizes::INSTRUCTION_SIZE);
    instructionText.setFillColor(sf::Color::White);
    instructionText.setPosition({ ui_layout::INSTRUCTION_TEXT_X, 80.f });
	window.draw(instructionText);


    // ===== �f�b�L���i��i�j =====

     //�@�w�i�`��
    sf::RectangleShape deckBG({ 1500.f, 250.f });
    deckBG.setPosition({ 0.f, m_deckStartPos.y - 40.f });
    deckBG.setFillColor(sf::Color(20, 20, 20, 200)); // �������̍�
    window.draw(deckBG);

    // デッキラベル
    sf::Text deckText(font, "Deck");
    deckText.setCharacterSize(ui_font_sizes::LABEL_SIZE);
    deckText.setFillColor(sf::Color::White);
    deckText.setPosition({ ui_layout::DECK_X_START, ui_layout::DECK_LABEL_RELATIVE_Y_OFFSET });

    // カード数表示

    for (size_t i = 0; i < m_displayDeck.size(); ++i)
    {
        sf::Vector2f pos = m_deckStartPos + sf::Vector2f(i * m_cardSpacing + m_deckScrollX, 0.f);

        // カード数表示 (*n)
        if (m_displayDeck[i].count > 1)
        {
            sf::Text countText(font, "x" + std::to_string(m_displayDeck[i].count));
            countText.setCharacterSize(ui_font_sizes::DECK_COUNTER_SIZE);
            countText.setFillColor(sf::Color::White);
            countText.setOutlineColor(sf::Color::Black);
            countText.setOutlineThickness(2.f);
            countText.setPosition(pos + sf::Vector2f(80.f, ui_layout::CARD_COUNT_Y_OFFSET)); // カード数表示位置
            window.draw(countText);
        }
        }
    }

    // ===== �v�[�����i���i�j =====

	//�@�w�i�`��
    sf::RectangleShape poolBG({ 1500.f, 250.f });
    poolBG.setPosition({ 0.f,m_poolStartPos.y - 40.f });
    poolBG.setFillColor(sf::Color(20, 20, 20, 200)); // �������̍�
    window.draw(poolBG);
	window.draw(deckBG);

	// デッキラベル
	sf::Text deckText(font, "Deck");
	deckText.setCharacterSize(ui_font_sizes::LABEL_SIZE);
	deckText.setFillColor(sf::Color::White);
	deckText.setPosition({ ui_layout::DECK_X_START, ui_layout::DECK_LABEL_RELATIVE_Y_OFFSET });

	// カード数表示

	const auto& pool = CardBuildPool::GetInstance().GetPoolCards();
    for (size_t i = 0; i < m_displayPool.size(); ++i)
    {
        sf::Vector2f pos = { m_poolStartPos.x + m_cardSpacing * i + m_poolScrollX, m_poolStartPos.y };

        m_renderer->DrawSingleCard(font, window, pos, m_displayPool[i].card->GetCardState(),"");

        // デッキラベル
        sf::Text deckText(font, "Deck");
        deckText.setCharacterSize(ui_font_sizes::LABEL_SIZE);
        deckText.setFillColor(sf::Color::White);
        deckText.setPosition({ ui_layout::DECK_X_START, ui_layout::DECK_LABEL_RELATIVE_Y_OFFSET });

        // カード数表示

        sf::Text countText(font, "x" + std::to_string(m_displayPool[i].count));
        countText.setCharacterSize(ui_font_sizes::DECK_COUNTER_SIZE);
        countText.setFillColor(sf::Color::White);
        countText.setPosition({ pos.x + 85.f, pos.y + ui_layout::CARD_COUNT_Y_OFFSET });
        window.draw(countText);
}

// �v�[������f�b�L�ɒǉ�
bool DeckBuildSystem::AddFromPool(int poolIndex)
{
    if (m_deckCards.size() >= MAX_DECK_SIZE)
    {
        ConsoleView::GetInstance().Add("�f�b�L�̏�������ɒB���Ă��܂�\n");

        return false;
    }

    int targetId = m_displayPool[poolIndex].card->GetCardState().cardId;
	// �v�[������J�[�h���擾    
    auto card = CardBuildPool::GetInstance().TakeCard(targetId);

    if (!card)
    {
        ConsoleView::GetInstance().Add("�v�[������J�[�h���擾�ł��܂���ł���: " + std::to_string(targetId) + "\n");
        return false;
    }

    m_deckCards.emplace_back(std::move(card));

    // �f�b�L����v�[���ɖ߂�
    RebuildDisplayPool();
    RebuildDisplayDeck();

    return true;
}

// �f�b�L����v�[���ɖ߂�
bool DeckBuildSystem::RemoveFromPool(int deckIndex)
{
    if (deckIndex >= m_deckCards.size())
    {
        ConsoleView::GetInstance().Add("�w��index���͈͊O�ł�:"  +  std::to_string(deckIndex) + "\n");
        return false;
    }

	// �f�b�L����J�[�h�����o��
    auto card = std::move(m_deckCards[deckIndex]);
	// �f�b�L����폜
    m_deckCards.erase(m_deckCards.begin() + deckIndex);

	// �J�[�h���v�[���ɖ߂�
    CardBuildPool::GetInstance().ReturnCard(std::move(card));

    return true;
}

// �f�b�L���J�[�h�擾
const std::vector<std::unique_ptr<Card>>& DeckBuildSystem::GetDeckCards() const
{
	return m_deckCards;
}

// �f�b�L�������擾
int DeckBuildSystem::GetDeckSize() const
{
	return static_cast<int>(m_deckCards.size());
}

// �f�b�L�Ґ���������擾
int DeckBuildSystem::GetMaxDeckSize() const
{
	return MAX_DECK_SIZE;
}

// �f�b�L�Ґ���������
bool DeckBuildSystem::IsComplete() const
{
	return m_deckCards.size() == MAX_DECK_SIZE;
}

// �f�b�L�������n��
std::vector<int> DeckBuildSystem::TakeDeck()
{
    std::vector<int> result;
    for (auto& card : m_deckCards)
    {
        result.push_back(card->GetCardState().cardId);
    }
    return result;
}

// �v�[���N���b�N����
bool DeckBuildSystem::HandlePoolClick(sf::Vector2f mousePos)
{
    // �h���b�O���̓N���b�N�������Ȃ�
    if (m_poolDragging || m_deckDragging)
    {
        return false;
    }
	// �f�b�L����`�F�b�N
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

// �f�b�L�N���b�N����
bool DeckBuildSystem::HandleDeckClick(sf::Vector2f mousePos)
{
    // �h���b�O���̓N���b�N�������Ȃ�
    if (m_poolDragging || m_deckDragging)
    {
        return false;
    }

    for (size_t i = 0; i < m_displayDeck.size(); ++i)
    {
        sf::FloatRect rect({ m_deckStartPos.x + m_cardSpacing * i + m_deckScrollX, m_deckStartPos.y }, { 120.0f, 160.0f });

        if (rect.contains(mousePos))
        {
            // �N���b�N���ꂽ�\����̃J�[�hID�����
            int targetId = m_displayDeck[i].card->GetCardState().cardId;

            // ���f�[�^(m_deckCards)���炻��ID��1�����T���č폜
            for (auto it = m_deckCards.begin(); it != m_deckCards.end(); ++it)
            {
                if ((*it)->GetCardState().cardId == targetId)
                {
                    // �v�[��(CardBuildPool)�Ɏ��̂�߂�
                    CardBuildPool::GetInstance().ReturnCard(std::move(*it));
                    // ���X�g���珜��
                    m_deckCards.erase(it);
                    break;
                }
            }

            // �\�����č\�z
            RebuildDisplayDeck();
            RebuildDisplayPool();
            return true;
        }
    }
	return false;
}

// �h���b�O����
void DeckBuildSystem::HandleDrag(sf::Vector2f mousePos, bool isClick)
{

    if (!isClick)
    {
        return;
    }

    float deltaX = mousePos.x - m_lastMouseX;
    m_lastMouseX = mousePos.x;

    // �v�[���h���b�O
    if (m_poolDragging)
    {
        m_poolScrollX += deltaX;
        ClampPoolScroll();
    }

    // �f�b�L�h���b�O
    if (m_deckDragging)
    {
        m_deckScrollX += deltaX;
        ClampDeckScroll();
    }

}

// �v�[���č\�z
void DeckBuildSystem::RebuildDisplayPool()
{
	// �N���A
    m_displayPool.clear();
	// �J�E���g�}�b�v�쐬
    std::unordered_map<int, int> countMap;
	// �ŏ��̏o��index�}�b�v�쐬
    std::unordered_map<int, size_t> firstIndexMap;
	// �v�[�����J�[�h�擾
    const auto& pool = CardBuildPool::GetInstance().GetPoolCards();

    for (size_t i = 0; i < pool.size(); ++i)
    {
		// �J�[�hID�擾
        int id = pool[i]->GetCardState().cardId;
        countMap[id]++;

        if (!firstIndexMap.contains(id))
        {
			// �ŏ��̏o��index�ۑ�
            firstIndexMap[id] = i;
        }
    }

    for (const auto& [id, count] : countMap)
    {
		// �J�[�h����
        auto card = CardFactory::GetInstance().CreateCard(id);
		// �v�[���\���p�ɒǉ�
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
    std::vector<int> idOrder; // �ǉ����ꂽ�������ێ��������ꍇ

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

// �X�N���[������
void DeckBuildSystem::ClampPoolScroll()
{
	// �v�[�����X�N���[������
	// �v�[�����J�[�h���Ɋ�Â��ő啝�v�Z
    float maxWidth = static_cast<float>(m_displayPool.size()) * m_cardSpacing;
    float viewWidth = 800.f;
	// �ŏ��X�N���[���ʒu�v�Z
    float minScroll = std::min(0.f, viewWidth - maxWidth);
	// �X�N���[���ʒu����
    m_poolScrollX = std::clamp(m_poolScrollX, minScroll, 0.f);
}

void DeckBuildSystem::ClampDeckScroll()
{
	// �f�b�L���X�N���[������
	// �f�b�L���J�[�h���Ɋ�Â��ő啝�v�Z
    float maxWidth = static_cast<float>(m_deckCards.size()) * m_cardSpacing;
    float viewWidth = 800.f;
	// �ŏ��X�N���[���ʒu�v�Z
    float minScroll = std::min(0.f, viewWidth - maxWidth);
	// �X�N���[���ʒu����
    m_deckScrollX = std::clamp(m_deckScrollX, minScroll, 0.f);
}
