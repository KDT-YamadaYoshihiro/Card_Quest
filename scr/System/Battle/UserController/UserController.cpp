#include "UserController.h"
#include "System/Constants.h"
#include "System/InPutManager/InPutMouseManager.h"
#include "System/Battle/BattleContex/BattleContext.h"
#include "View/BattleView/BattleView.h"
#include "Entity/Character/Character.h"
#include "Entity/Card/Card.h"
#include "Entity/Card/CardManager/CardManager.h"
#include "View/ConsoleView/ConsoleView.h"
namespace UserControllerConstants
{
    // UI 配置定数 (Constants.h と重複するためコメントアウト)
    constexpr sf::Vector2f HAND_START{ 200.f, 520.f };
    constexpr float HAND_SPACING = Constants::HAND_SPACING;
    constexpr float SELECT_OFFSET_Y = Constants::SELECT_OFFSET_Y;
}

/// <summary>
/// ������
/// </summary>
/// <param name="context"></param>
UserController::UserController(BattleContext& context,BattleView& battleView)
    : m_context(context),
    m_battleView(battleView)
{
}

void UserController::Update(sf::RenderWindow& window)
{
    sf::Vector2f mousePos = InPutMouseManager::GetInstance().GetMousePosition(window);

    switch (m_phase)
    {
    case PlayerSelectPhase::SELECT_CARD:

        ConsoleView::GetInstance().Add("�J�[�h�I��\n");
        // ���W�n�̍X�V
        UpdateHandCardRects();
        // �I���X�V
        UpdateSelectCard(window, mousePos);
        break;
    case PlayerSelectPhase::CREATE_TARGETS:
        ConsoleView::GetInstance().Add("�^�[�Q�b�g���쐬\n");
        // ���쐬
        UpdateCreateTargets();
        break;
    case PlayerSelectPhase::SELECT_TARGET:
        ConsoleView::GetInstance().Add("�^�[�Q�b�g�I��\n");
        // ���W�n�̍X�V
        UpdateCharacterRects(m_targetCandidates);
        // �I���X�V
        UpdateSelectTarget(window);
        break;
    case PlayerSelectPhase::DONE:
		m_context.SetFocusDraw(false);
        return;
        break;
    }
}

bool UserController::HasConfirmedAction() const
{
    return m_confirmedAction.has_value();
}

UserAction UserController::ConsumeAction()
{
    UserAction result = *m_confirmedAction;
    m_confirmedAction.reset();

    // ���̓��͂ɔ����ă��Z�b�g
    m_phase = PlayerSelectPhase::SELECT_CARD;
    m_selectedActor.reset();
    m_selectedCardIndex = -1;
    m_selectedTargets.clear();
    m_targetCandidates.clear();

    return result;
}

// ================= �I������ =================


/// <summary>
/// �J�[�h�I��
/// </summary>
/// <param name="window"></param>
/// <param name="mousePos"></param>
void UserController::UpdateSelectCard(sf::RenderWindow& window, const sf::Vector2f& mousePos)
{
    
    m_hoveredCardIndex = HitTestHandCard(mousePos);

    int cardIndex = HitTestHandCard(mousePos);
    m_hoveredCardIndex = cardIndex;

    if (InPutMouseManager::GetInstance().IsLeftClicked())
    {
        if (cardIndex != -1)
        {
            // �J�[�h�I�������̎��s
            SelectCard(cardIndex);
        }
    }
}

void UserController::SelectCard(int cardIdx)
{
    m_selectedCardIndex = cardIdx;
    m_selectCardId = m_context.GetCardIdByGlobalIndex(cardIdx);
	m_context.SetSelectedCardIndex(m_selectedCardIndex);
    m_selectedActor = m_context.GetCharacterByCardId(m_selectCardId);
    if(!m_selectedActor) {
        ConsoleView::GetInstance().Add("Error: Actor not found for card ID: " + std::to_string(m_selectCardId));
        return;
    }

    auto cardData = CardManager::GetInstance().GetCardData(m_selectCardId);

    UpdateCreateTargets();
	int predctionCost = cardData.actionPlus - 1;
    m_context.SetPredictedCost(predctionCost);
    m_context.ClearFocusTargets();

    if (!m_targetCandidates.empty())
    {
        // �^�[�Q�b�g�^�C�v�ɂ��t�H�[�J�X����
        if (cardData.targetType == TargetType::ALLY_ALL || cardData.targetType == TargetType::OPPONENT_ALL) // �S�̑Ώۂ̏ꍇ
        {
            // �S�����t�H�[�J�X
            m_context.SetFocusTargets(m_targetCandidates);
            
            m_preSelectedTarget = nullptr;
        }
        else // �P�̑Ώۂ̏ꍇ�i�f�t�H���g�j
        {
            // 0�Ԗڂ��f�t�H���g�t�H�[�J�X
            m_preSelectedTarget = m_targetCandidates[0];
            m_context.SetFocusTargets({ m_preSelectedTarget });
        }
    }


    m_phase = PlayerSelectPhase::SELECT_TARGET;
}


/// <summary>
/// �^�[�Q�b�g���̎擾
/// </summary>
void UserController::UpdateCreateTargets()
{
    // �J�[�hID�A���̎擾
    const CardData& card = CardManager::GetInstance().GetCardData(m_selectCardId);

    // �^�[�Q�b�g���̎擾
    m_targetCandidates = m_context.CreateTargetCandidates(card.targetType,m_selectedActor->GetFaction(),m_selectedActor);

    // �^�[�Q�b�g����łȂ����m�F
    if (m_targetCandidates.empty()) {
        m_phase = PlayerSelectPhase::SELECT_CARD;
        return;
    }

    // �^�[�Q�b�g�I���t�F�[�Y�ڍs���̏����ݒ�
    if (card.targetType == TargetType::OPPONENT_ALL || card.targetType == TargetType::ALLY_ALL) {
        // �S�����t�H�[�J�X
        m_context.SetFocusTargets(m_targetCandidates);
    }
    else {
        // �f�t�H��0�Ԗڂ����I���E�t�H�[�J�X
        m_preSelectedTarget = m_targetCandidates[0];
        m_context.SetFocusTargets({ m_preSelectedTarget });
    }

    // Focus�\��
    m_context.SetFocusDraw(true);

    m_phase = PlayerSelectPhase::SELECT_TARGET;
}

/// <summary>
/// �^�[�Q�b�g�I��
/// </summary>
/// <param name="window"></param>
void UserController::UpdateSelectTarget(sf::RenderWindow& window)
{
	sf::Vector2f mousePos = InPutMouseManager::GetInstance().GetMousePosition(window);

    // --- �ǉ��d�l�F�^�[�Q�b�g�I�𒆂��J�[�h�̃N���b�N���`�F�b�N ---
    int cardIdx = HitTestHandCard(mousePos);
    if (InPutMouseManager::GetInstance().IsLeftClicked() && cardIdx != -1)
    {
        // �ʂ̃J�[�h�i�܂��͓����J�[�h�j���N���b�N���ꂽ��A�J�[�h�I������蒼��
        SelectCard(cardIdx);
        return; // �ȍ~�̃^�[�Q�b�g����̓X�L�b�v
    }

    int targetIdx = HitTestCharacter(mousePos, m_targetCandidates);
    m_hoveredTarget = (targetIdx != -1) ? m_targetCandidates[targetIdx] : nullptr;

    if (InPutMouseManager::GetInstance().IsLeftClicked())
    {
        if (targetIdx != -1)
        {
            auto clickedTarget = m_targetCandidates[targetIdx];
            auto cardData = CardManager::GetInstance().GetCardData(m_selectCardId);

            // --- �d�l�ύX�F�S�̍U���ƒP�̍U���Ō��胍�W�b�N�𕪂��� ---
            if (cardData.targetType == TargetType::ALLY_ALL || cardData.targetType == TargetType::OPPONENT_ALL)
            {
                // �S���^�[�Q�b�g��
                ConfirmAction(m_targetCandidates);
                m_context.SetFocusDraw(false);

            }
            else if (m_preSelectedTarget == clickedTarget)
            {
                // 2��ځF����
                std::vector<std::shared_ptr<Character>> targets = { clickedTarget };
                ConfirmAction(targets);
                m_context.SetFocusDraw(false);
            }
            else
            {
                // 1��ځF�t�H�[�J�X�X�V
                m_preSelectedTarget = clickedTarget;
                m_context.ClearFocusTargets();
                m_context.SetFocusTargets({ m_preSelectedTarget });
            }
        }
    }

}

void UserController::ConfirmAction(const std::vector<std::shared_ptr<Character>>& targets)
{
    UserAction action;
    action.actor = m_selectedActor;
    action.cardId = m_selectCardId;
    action.targets = targets;
    m_confirmedAction = action;

    m_context.ClearFocusTargets();
    m_context.SetPredictedCost(0);
    m_context.SetSelectedCardIndex(-1);
}

/// <summary>
/// �L�����N�^�[�N���b�N����
/// </summary>
/// <param name="mousePos"></param>
/// <param name="list"></param>
/// <returns></returns>
int UserController::HitTestCharacter(
    const sf::Vector2f& mousePos,
    const std::vector<std::shared_ptr<Character>>& list) const
{
    for (size_t i = 0; i < m_characterRects.size(); ++i)
    {
        if (m_characterRects[i].contains(mousePos))
        {
            return static_cast<int>(i);
        }
    }
    return -1;
}

/// <summary>
/// �J�[�h�N���b�N����
/// </summary>
/// <param name="mousePos"></param>
/// <returns></returns>
int UserController::HitTestHandCard(const sf::Vector2f& mousePos) const
{
    for (int i = 0; i < m_handCardRects.size(); ++i)
    {
        if (m_handCardRects[i].contains(mousePos))
        {
            return i; 
        }
    }
    return -1;
}

/// <summary>
/// �L�����N�^�[�N���b�N�͈�
/// </summary>
/// <param name="list"></param>
void UserController::UpdateCharacterRects(
    const std::vector<std::shared_ptr<Character>>& list)
{
    m_characterRects.clear();

    for (auto& c : list)
    {
        if (!c)
        {
            continue;
        }

        sf::Vector2f pos = c->GetPosition();

        constexpr float SPRITE_W = 165.f; 
        constexpr float SPRITE_H = 150.f;

        m_characterRects.emplace_back(sf::FloatRect({ pos.x,pos.y }, { SPRITE_W, SPRITE_H }));
    }
}

/// <summary>
/// �J�[�h�N���b�N����
/// </summary>
/// <param name="actor"></param>
void UserController::UpdateHandCardRects()
{
    m_handCardRects.clear();

    int cardCount = 0;

    for (auto& p : m_context.GetAlivePlayers())
    {
        cardCount += p->GetCardCount();
    }

    for (int i = 0; i < cardCount; ++i)
    {
        // �`��(BattleView)�Ɠ����v�Z����Rect���쐬
        sf::Vector2f pos = HAND_START;
        pos.x += i * HAND_SPACING;

        // �J�[�h�̋�`��o�^
        m_handCardRects.push_back(sf::FloatRect({ pos.x, pos.y }, { CARD_W, CARD_H }));
    }
}

// ================= �⏕ =================

/// <summary>
/// �}�E�X�́u���[���h���W�v���擾�i�L�����N�^�[����p�j
/// </summary>
sf::Vector2f UserController::GetWorldMousePos(sf::RenderWindow& window) const
{
    // ���݂̃E�B���h�E���̃}�E�X�s�N�Z���ʒu���擾
    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);

    // ���݂̃J�����iView�j�̐ݒ�Ɋ�Â��āA���[���h���W�ɕϊ�
    // ����ɂ��A�Y�[����X�N���[���̏�Ԃ����f���ꂽ���W���Ԃ�܂�
    return window.mapPixelToCoords(pixelPos);
}

/// <summary>
/// �}�E�X�́u�X�N���[�����W�v���擾�iUI/�J�[�h����p�j
/// </summary>
sf::Vector2f UserController::GetScreenMousePos(sf::RenderWindow& window) const
{
    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);

    // UI�̓J�����̉e�����󂯂Ȃ����߁A�f�t�H���gView�i��ʂ��̂܂܂̍��W�j�ŕϊ�
    return window.mapPixelToCoords(pixelPos, window.getDefaultView());
}
// �f�o�b�O�p�`��
void UserController::DrawDebug(sf::RenderWindow& window)
{
    sf::RectangleShape r;
    r.setFillColor(sf::Color::Transparent);
    r.setOutlineColor(sf::Color::Green);
    r.setOutlineThickness(2.f);

    for (auto& rect : m_characterRects)
    {
        r.setPosition({ rect.position.x, rect.position.y });
        r.setSize({ rect.size.x, rect.size.y });
        window.draw(r);
    }

    r.setOutlineColor(sf::Color::Blue);
    for (auto& rect : m_handCardRects)
    {
        r.setPosition({ rect.position.x, rect.position.y });
        r.setSize({ rect.size.x, rect.size.y });
        window.draw(r);
    }
}

PlayerSelectPhase UserController::GetSelectPhase() const
{
    return m_phase;
}

std::shared_ptr<Character> UserController::GetSelectActor() const
{
    return m_selectedActor;
}

int UserController::GetSelectCardId() const
{
    return m_selectCardId;
}


const std::vector<std::shared_ptr<Character>>& UserController::GetSelectTargetIndices() const
{
    return m_selectedTargets;
}
