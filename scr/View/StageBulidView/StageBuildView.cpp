#include "StageBuildView.h"
#include "CSVLoad/StageLoader/StageLoader.h"
#include "CSVLoad/CharacterLoader.h"
#include "CSVLoad//TextureLoader/TextureLoader.h"
#include "View/Font/FontManager.h"
#include "System/InPutManager/InPutMouseManager.h"
#include "UI/TriangleButton.h"
#include "UI/BoxButton.h"
#include <cmath>

/// <summary>
/// 初期化
/// </summary>
/// <param name="render"></param>
StageBuildView::StageBuildView(RenderSystem& render)
    : m_render(render),
    m_font(FontManager::GetInstance().GetFont())
{
}

/// <summary>
/// 更新
/// </summary>
/// <param name="window"></param>
/// <param name="context"></param>
void StageBuildView::Update(sf::RenderWindow& window, const StageBulidContext& context)
{
    const auto* stage = context.GetSelectedStage();
	// 選択ステージなしの時は処理しない
    if (!stage)
    {
        return;
    }

    const auto& allIds = context.GetStageIds();
	// ステージIDリストが空の場合も処理しない
    if (allIds.empty())
    {
        return;
    }

    // ステージ切り替え検知とスライド方向の決定
    if (stage->stageId != m_currentStageId)
    {
        if (m_currentStageId != -1)
        {
            int firstId = allIds.front();
            int lastId = allIds.back();

            if (m_currentStageId == firstId && stage->stageId == lastId)
            {
                // 1ページ目から最終ページへ
                m_currentOffsetX = -800.f; // 左からスライド
            }
            else if (m_currentStageId == lastId && stage->stageId == firstId)
            {
                // 最終ページから1ページ目へ
                m_currentOffsetX = 800.f;  // 右からスライド
            }
            // 通常の前後判定
            else
            {
                m_currentOffsetX = (stage->stageId > m_currentStageId) ? 800.f : -800.f;
            }
        }
        m_currentStageId = stage->stageId;
    }

    // 2. 座標を 0 (中心) に向かって補間（変更なし）
    m_currentOffsetX += (0.f - m_currentOffsetX) * SLIDE_SPEED;

    UpdateScroll(window, allIds.size());
}

/// <summary>
/// 描画
/// </summary>
/// <param name="context"></param>
void StageBuildView::Draw(const StageBulidContext& context)
{
    const auto* stage = context.GetSelectedStage();
    if (!stage) return;

    // 計算されたオフセットを渡して描画
    DrawStageContent(stage, m_currentOffsetX);

    // ボタン類はスライドさせない（固定位置）
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
/// ステージ内容描画
/// </summary>
/// <param name="stage"></param>
/// <param name="offsetX"></param>
void StageBuildView::DrawStageContent(const StageData* stage, float offsetX)
{
    // 画面中央(640) + オフセット
    float centerX = 640.f + offsetX;

    // 背景パネル
    sf::RectangleShape detailPanel({ PANEL_WIDTH, 400.f });
    detailPanel.setOrigin({ PANEL_WIDTH / 2.f, 200.f });
    detailPanel.setPosition({ centerX, 350.f });
    detailPanel.setFillColor(sf::Color(100, 149, 237, 200));
    m_render.Draw(detailPanel);

    // ステージタイトル
    sf::Text title(m_font, sf::String::fromUtf8(stage->name.begin(), stage->name.end()));
    title.setCharacterSize(36);
    // パネル内の相対位置に配置
    title.setPosition({ centerX - 300.f, 180.f });
    m_render.Draw(title);

    // モンスター表示
    DrawStageMonsters(stage, offsetX);

    // テキスト情報
    sf::Text info(m_font, std::to_string(stage->enemyIds.size()) + " Enemies\nSpecial Rules: None");
    info.setCharacterSize(24);
    info.setPosition({ centerX - 300.f, 450.f });
    m_render.Draw(info);
}

/// <summary>
/// 出現モンスター表示
/// </summary>
/// <param name="data"></param>
/// <param name="offsetX"></param>
void StageBuildView::DrawStageMonsters(const StageData* data, float offsetX)
{
    constexpr float MONSTER_Y = 320.f;
    constexpr float SPACING = 160.f;
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

        // アイコンを横並びに配置
        sprite.setPosition({ centerX - SPACING + (i * SPACING), MONSTER_Y });
        m_render.Draw(sprite);
    }
}

/// <summary>
/// スクロール用の更新メソッド
/// </summary>
/// <param name="window"></param>
/// <param name="stageCount"></param>
void StageBuildView::UpdateScroll(sf::RenderWindow& window, size_t stageCount)
{
    auto& input = InPutMouseManager::GetInstance();
    m_scrollOffsetY += input.GetWheelDelta() * 30.f;

    // スクロール制限 (リスト表示を復活させる場合に使用)
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
