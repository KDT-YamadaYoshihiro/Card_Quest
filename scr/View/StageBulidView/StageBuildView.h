#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_map>
#include "View/Render/RenderSystem.h"
#include "System/StageBulid/StageBulidContext/StageBulidContext.h"

class StageBuildView
{
private:

	// レンダラー参照
    RenderSystem& m_render;
	// フォント
    sf::Font m_font;
    
    // 現在の表示ステージＩＤ
    int   m_currentStageId = -1;
	// 現在の表示オフセットＸ
    float m_currentOffsetX = 0.f;

    // スクロール（リスト用）
    float m_scrollOffsetY = 0.f;

    // 定数
	static constexpr float SLIDE_SPEED = 0.003f;     // スライド速度
	static constexpr float ITEM_HEIGHT = 40.f;      // アイテム高さ
	static constexpr float PANEL_WIDTH = 700.f;     // パネル幅

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
	/// ステージ内容描画
    /// </summary>
    /// <param name="stage"></param>
    /// <param name="offsetX"></param>
    void DrawStageContent(const StageData* stage, float offsetX);

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
    void DrawStageMonsters(const StageData* data, float offsetX);

};
