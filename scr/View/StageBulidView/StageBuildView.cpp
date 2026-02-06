#include "StageBuildView.h"
#include "CSVLoad/StageLoader/StageLoader.h"
#include "CSVLoad/CharacterLoader.h"
#include "CSVLoad//TextureLoader/TextureLoader.h"
#include "View/Font/FontManager.h"
#include "System/InPutManager/InPutMouseManager.h"
#include "UI/TriangleButton.h"
#include "UI/BoxButton.h"
#include <cmath>

namespace
{
    constexpr float LIST_START_Y = 300.f;
    constexpr float LIST_INTERVAL = 40.f;

    constexpr sf::Vector2f DETAIL_BASE_POS{ 920.f, 100.f };
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
        //UpdateDetailAnimation(selected);
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
	//DrawStageList(context); 　// debug用
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

    const auto* stage = context.GetSelectedStage();
    if (!stage)
    {
        return;
    }

    // 背景パネル
    sf::RectangleShape detailPanel({ 700.f, 400.f });
    detailPanel.setOrigin({ 200.f, 200.f });
    detailPanel.setPosition({ 500.f, 300.f }); 
    detailPanel.setFillColor(sf::Color(100, 149, 237)); // 落ち着いた青
    m_render.Draw(detailPanel);

    // ステージタイトル 
    sf::Text title(m_font, "");
    sf::String titleText = sf::String::fromUtf8(stage->name.begin(), stage->name.end());
	title.setString(titleText);
	title.setCharacterSize(32);
	title.setPosition({ 320.f, 120.f });
	m_render.Draw(title);

    // モンスター表示
    DrawStageMonsters(stage);

    // 4. テキスト情報（敵の数、特殊制限）
    sf::Text info(m_font, "");
	std::string infoStr = std::to_string(stage->enemyIds.size()) + "Enemies\n";
	infoStr += "Special Rules: None"; // 仮
	info.setString(infoStr);
    info.setCharacterSize(24);
    info.setPosition({ 320.f, 420.f });
    m_render.Draw(info);

	// ボタン描画
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

/// <summary>
/// エネミーアイコンの描画
/// </summary>
/// <param name="data"></param>
void StageBuildView::DrawStageMonsters(const StageData* data)
{
	// 詳細ビューの位置取得
    constexpr float MONSTER_Y = 250.f;
    constexpr float SPACING = 150.f;

    // エネミーアイコン表示
    constexpr sf::Vector2f ENEMY_ICON_SIZE{ 128.f, 128.f };
    constexpr float ENEMY_ICON_SPACING = 72.f;


    for (int i = 0; i < 3; ++i) {
		int enemyId = data->enemyIds[i];
        // キャラデータ取得
        const CharacterData* enemyData = CharacterLoader::GetInstance().GetData(enemyId);
        if (!enemyData)
        {
            continue;
        }

        // テクスチャ取得
        auto tex = TextureLoader::GetInstance().GetTextureID(enemyData->iconKey);
        if (!tex) continue;

        sf::Sprite sprite(*tex);
        // 配置位置を中央揃えに調整
            // サイズ調整
        auto size = tex->getSize();
        sprite.setScale({ ENEMY_ICON_SIZE.x / size.x,ENEMY_ICON_SIZE.y / size.y });
        sprite.setPosition({ 650.f - SPACING + (i * SPACING) - (ENEMY_ICON_SIZE.x / 2), MONSTER_Y });
        m_render.Draw(sprite);

    }
}
