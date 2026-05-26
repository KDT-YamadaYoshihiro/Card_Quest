#include "StageBuildView.h"
#include "System/Constants.h"
#include "CSVLoad/StageLoader/StageLoader.h"
#include "CSVLoad/CharacterLoader.h"
#include "CSVLoad//TextureLoader/TextureLoader.h"
#include "View/Font/FontManager.h"
#include "System/InPutManager/InPutMouseManager.h"
#include "UI/TriangleButton.h"
#include "UI/BoxButton.h"
#include <cmath>

/// <summary>
/// ������
/// </summary>
/// <param name="render"></param>
StageBuildView::StageBuildView(RenderSystem& render)
    : m_render(render),
    m_font(FontManager::GetInstance().GetFont())
{
}

/// <summary>
/// �X�V
/// </summary>
/// <param name="window"></param>
/// <param name="context"></param>
void StageBuildView::Update(sf::RenderWindow& window, const StageBulidContext& context)
{
    const auto* stage = context.GetSelectedStage();
	// �I���X�e�[�W�Ȃ��̎��͏������Ȃ�
    if (!stage)
    {
        return;
    }

    const auto& allIds = context.GetStageIds();
	// �X�e�[�WID���X�g����̏ꍇ���������Ȃ�
    if (allIds.empty())
    {
        return;
    }

    // �X�e�[�W�؂�ւ����m�ƃX���C�h�����̌���
    if (stage->stageId != m_currentStageId)
    {
        if (m_currentStageId != -1)
        {
            int firstId = allIds.front();
            int lastId = allIds.back();

            if (m_currentStageId == firstId && stage->stageId == lastId)
            {
                // 1�y�[�W�ڂ���ŏI�y�[�W��
                m_currentOffsetX = -800.f; // ������X���C�h
            }
            else if (m_currentStageId == lastId && stage->stageId == firstId)
            {
                // �ŏI�y�[�W����1�y�[�W�ڂ�
                m_currentOffsetX = 800.f;  // �E����X���C�h
            }
            // �ʏ�̑O�㔻��
            else
            {
                m_currentOffsetX = (stage->stageId > m_currentStageId) ? 800.f : -800.f;
            }
        }
        m_currentStageId = stage->stageId;
    }

    // 2. ���W�� 0 (���S) �Ɍ������ĕ�ԁi�ύX�Ȃ��j
    m_currentOffsetX += (0.f - m_currentOffsetX) * SLIDE_SPEED;

    UpdateScroll(window, allIds.size());
}

/// <summary>
/// �`��
/// </summary>
/// <param name="context"></param>
void StageBuildView::Draw(const StageBulidContext& context)
{
    const auto* stage = context.GetSelectedStage();
    if (!stage) return;

    // �v�Z���ꂽ�I�t�Z�b�g��n���ĕ`��
    DrawStageContent(stage, m_currentOffsetX);

    // �{�^���ނ̓X���C�h�����Ȃ��i�Œ�ʒu�j
    if (auto* left = context.GetLeftArrow())
    {
        left->Draw(m_render.GetWindow());
    }
    if (auto* right = context.GetRightArrow()) 
    {
        right->Draw(m_render.GetWindow());
    }
    if (auto* nextBtn = context.GetNextButton())
    {
        nextBtn->Draw(m_render.GetWindow());
    }
    if (auto* backBtn = context.GetBackButton())
    {
        backBtn->Draw(m_render.GetWindow());
    }
}

/// <summary>
/// �X�e�[�W���e�`��
/// </summary>
/// <param name="stage"></param>
/// <param name="offsetX"></param>
void StageBuildView::DrawStageContent(const StageData* stage, float offsetX)
{
    // ��ʒ���(640) + �I�t�Z�b�g
    float centerX = 640.f + offsetX;

    // �w�i�p�l��
    sf::RectangleShape detailPanel({ PANEL_WIDTH, 400.f });
    detailPanel.setOrigin({ PANEL_WIDTH / 2.f, 200.f });
    detailPanel.setPosition({ centerX, 350.f });
    detailPanel.setFillColor(sf::Color(100, 149, 237, 200));
    m_render.Draw(detailPanel);

    // �X�e�[�W�^�C�g��
    sf::Text title(m_font, sf::String::fromUtf8(stage->name.begin(), stage->name.end()));
    title.setCharacterSize(36);
    // �p�l�����̑��Έʒu�ɔz�u
    title.setPosition({ centerX - 300.f, 180.f });
    m_render.Draw(title);

    // �����X�^�[�\��
    DrawStageMonsters(stage, offsetX);

    // �e�L�X�g���
    sf::Text info(m_font, std::to_string(stage->enemyIds.size()) + " Enemies\nSpecial Rules: None");
    info.setCharacterSize(24);
    info.setPosition({ centerX - 300.f, 450.f });
    m_render.Draw(info);
}

/// <summary>
/// �o�������X�^�[�\��
/// </summary>
/// <param name="data"></param>
/// <param name="offsetX"></param>
void StageBuildView::DrawStageMonsters(const StageData* data, float offsetX)
// ステージビルド定数 (Constants.h と重複するためコメントアウト)
constexpr float MONSTER_Y = Constants::MONSTER_Y;
constexpr float SPACING = Constants::SPACING;
    constexpr sf::Vector2f ICON_SIZE{ 120.f, 120.f };

    float centerX = 640.f + offsetX;

    for (int i = 0; i < 3 && i < data->enemyIds.size(); ++i)
    {
        const CharacterData* enemyData = CharacterLoader::GetInstance().GetData(data->enemyIds[i]);
        if (!enemyData)
        {
            continue;
        }

        auto tex = TextureLoader::GetInstance().GetTextureID(enemyData->iconKey);
        if (!tex)
        {
            continue;
        }

        sf::Sprite sprite(*tex);
        auto size = tex->getSize();
        sprite.setScale({ ICON_SIZE.x / size.x, ICON_SIZE.y / size.y });
        sprite.setOrigin({ ICON_SIZE.x / 2.f, ICON_SIZE.y / 2.f });

        // �A�C�R���������тɔz�u
        sprite.setPosition({ centerX - SPACING + (i * SPACING), MONSTER_Y });
        m_render.Draw(sprite);
    }
}

/// <summary>
/// �X�N���[���p�̍X�V���\�b�h
/// </summary>
/// <param name="window"></param>
/// <param name="stageCount"></param>
void StageBuildView::UpdateScroll(sf::RenderWindow& window, size_t stageCount)
{
    auto& input = InPutMouseManager::GetInstance();
    m_scrollOffsetY += input.GetWheelDelta() * 30.f;

    // �X�N���[������ (���X�g�\���𕜊�������ꍇ�Ɏg�p)
    float minY = -(stageCount * ITEM_HEIGHT);
    if (m_scrollOffsetY < minY)
    {
        m_scrollOffsetY = minY;
    }
    if (m_scrollOffsetY > 0.f)
    {
        m_scrollOffsetY = 0.f;
    }
}
