#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_map>
#include "View/Render/RenderSystem.h"
#include "System/StageBulid/StageBulidContext/StageBulidContext.h"

class TriangleButton;
class BoxButton;

struct StageDetailView
{
    int stageId;
    sf::Vector2f pos;
    sf::Vector2f targetPos;
};

class StageBuildView
{
private:

    RenderSystem& m_render;


    sf::Font m_font;

    std::unordered_map<int, StageDetailView> m_detailViews;


    int m_prevSelectedStageId = -1;

    // スクロール
    float m_scrollOffsetY = 0.f;
    float m_scrollVelocity = 0.f;

    // 定数
    static constexpr float LIST_START_Y = 80.f;
    static constexpr float ITEM_HEIGHT = 40.f;
    static constexpr float LIST_X = 40.f;

public:

    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name="render"></param>
    StageBuildView(RenderSystem& render);

    /// <summary>
    /// 更新
    /// </summary>
    /// <param name="context"></param>
    void Update(sf::RenderWindow& window, const StageBulidContext& context);

    /// <summary>
    /// 描画
    /// </summary>
    /// <param name="context"></param>
    void Draw(const StageBulidContext& context);


private:

    /// <summary>
    /// リスト表示
    /// </summary>
    /// <param name="context"></param>
    void DrawStageList(const StageBulidContext& context);

    /// <summary>
    /// 詳細表示
    /// </summary>
    /// <param name="context"></param>
    void DrawStageDetails(const StageBulidContext& context);

    /// <summary>
    /// 詳細表示の移動更新
    /// </summary>
    /// <param name="selectedStageId"></param>
    //void UpdateDetailAnimation(int selectedStageId);

    /// <summary>
    /// スクロール用の更新メソッド
    /// </summary>
    /// <param name="window"></param>
    /// <param name="stageCout"></param>
    void UpdateScroll(sf::RenderWindow& window, size_t stageCount);

    /// <summary>
	/// 出現モンスター表示
    /// </summary>
    /// <param name="data"></param>
    void DrawStageMonsters(const StageData* data);

};
