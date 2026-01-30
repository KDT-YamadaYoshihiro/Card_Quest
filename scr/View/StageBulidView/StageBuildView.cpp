#include "StageBuildView.h"
#include "CSVLoad/StageLoader/StageLoader.h"
#include "View/Font/FontManager.h"
#include "System/InPutManager/InPutMouseManager.h"
#include <cmath>

namespace
{
    constexpr float LIST_START_Y = 80.f;
    constexpr float LIST_INTERVAL = 40.f;

    constexpr sf::Vector2f DETAIL_BASE_POS{ 420.f, 100.f };
    constexpr float DETAIL_SLIDE_OFFSET = 900.f; // 画面外
}

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
    if (!stage)
    {
        // 選択なし → 詳細更新しない
        UpdateScroll(window, context.GetStageIds().size());
        return;
    }


    int selected = context.GetSelectedStage()->stageId;

    if (selected != m_prevSelectedStageId)
    {
        UpdateDetailAnimation(selected);
        m_prevSelectedStageId = selected;
    }

    // 補間移動
    for (auto& [_, view] : m_detailViews)
    {
        view.pos += (view.targetPos - view.pos) * 0.15f;
    }

    UpdateScroll(window, context.GetStageIds().size());
}

/// <summary>
/// 描画
/// </summary>
/// <param name="context"></param>
void StageBuildView::Draw(const StageBulidContext& context)
{
    DrawStageList(context);
    DrawStageDetails(context);
}

/// <summary>
/// ステージリスト描画
/// </summary>
/// <param name="context"></param>
void StageBuildView::DrawStageList(const StageBulidContext& context)
{
    const auto& ids = context.GetStageIds();

    for (size_t i = 0; i < ids.size(); ++i)
    {
        float y = LIST_START_Y + i * ITEM_HEIGHT + m_scrollOffsetY;

        // 画面外は描画しない（最適化）
        if (y < -ITEM_HEIGHT || y > 720) continue;

        sf::Text text(m_font, "stage" + std::to_string(ids[i]));
        text.setCharacterSize(22);
        text.setPosition({ 40.0f, LIST_START_Y + i * LIST_INTERVAL });

        // 選択中強調
        if (ids[i] == context.GetSelectedStage()->stageId)
        {
            text.setFillColor(sf::Color::Yellow);
        }
        else
        {
            text.setFillColor(sf::Color::White);
        }

        m_render.Draw(text);
    }
}

/// <summary>
/// 選択したステージ詳細
/// </summary>
/// <param name="context"></param>
void StageBuildView::DrawStageDetails(const StageBulidContext& context)
{
    for (auto& [stageId, view] : m_detailViews)
    {
        const StageData* data = StageLoader::GetInstance().GetStageData(stageId);
        if (!data) continue;

        sf::RectangleShape panel({ 320.f, 360.f });
        panel.setPosition(view.pos);
        panel.setFillColor(sf::Color(40, 40, 40, 220));
        panel.setOutlineThickness(2.f);
        panel.setOutlineColor(sf::Color::White);

        m_render.Draw(panel);

        // ステージ名
        sf::Text title(m_font, "Stage " + std::to_string(stageId));
        title.setCharacterSize(26);
        title.setPosition({ view.pos.x + 20, view.pos.y + 20 });
        m_render.Draw(title);

        // エネミーID表示（アイコン化は後でOK）
        for (int i = 0; i < 3; ++i)
        {
            sf::Text enemy(m_font, "Enemy : " + std::to_string(data->enemyIds[i]));
            enemy.setCharacterSize(18);
            enemy.setPosition({ view.pos.x + 20, view.pos.y + 80 + i * 30 });
            m_render.Draw(enemy);
        }
    }
}

/// <summary>
/// 詳細表示物の移動更新
/// </summary>
/// <param name="selectedStageId"></param>
void StageBuildView::UpdateDetailAnimation(int selectedStageId)
{
    // ステージが未登録なら追加
    if (m_detailViews.find(selectedStageId) == m_detailViews.end())
    {
        StageDetailView view;
        view.stageId = selectedStageId;
        view.pos = DETAIL_BASE_POS + sf::Vector2f(DETAIL_SLIDE_OFFSET, 0.f);
        view.targetPos = DETAIL_BASE_POS;

        m_detailViews[selectedStageId] = view;
    }

    for (auto& [id, view] : m_detailViews)
    {
        if (id == selectedStageId)
        {
            view.targetPos = DETAIL_BASE_POS;
        }
        else
        {
            view.targetPos = DETAIL_BASE_POS + sf::Vector2f(DETAIL_SLIDE_OFFSET, 0.f);
        }
    }
}

/// <summary>
/// スクロールの更新
/// </summary>
/// <param name="window"></param>
/// <param name="stageCount"></param>
void StageBuildView::UpdateScroll(sf::RenderWindow& window, size_t stageCount)
{

    auto& input = InPutMouseManager::GetInstance();

    // ホイールスクロール
    m_scrollOffsetY += input.GetWheelDelta() * 30.f;

    // ドラッグスクロール
    if (input.IsDragging())
    {
        static float prevY = input.GetMousePosition(window).y;
        float currentY = input.GetMousePosition(window).y;
        m_scrollOffsetY += (currentY - prevY);
        prevY = currentY;
    }

    // スクロール制限
    float minY = -(stageCount * ITEM_HEIGHT) + 300.f; // 表示領域想定
    if (m_scrollOffsetY < minY) m_scrollOffsetY = minY;
    if (m_scrollOffsetY > 0.f) m_scrollOffsetY = 0.f;



}
