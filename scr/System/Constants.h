#pragma once

// ============================================================================
// ゲーム定数 (Magic Numbers)
// ============================================================================

namespace Constants {

// --- キャラクターサイズ ---
constexpr float CHAR_W = 165.f;   // キャラクター幅 [px]
constexpr float CHAR_H = 150.f;   // キャラクター高さ [px]

// --- カードサイズ ---
constexpr float CARD_W = 120.f;   // カード幅 [px]
constexpr float CARD_H = 160.f;   // カード高さ [px]

// --- ハンド配置 ---
constexpr float HAND_SPACING = 180.f;    // ハンド間隔 [px]
constexpr float SELECT_OFFSET_Y = 50.f;  // セレクト時のオフセット Y [px]

// --- スプライト分割数 (アニメーション用) ---
constexpr int DIV_X = 9;   // X 方向分割数
constexpr int DIV_Y = 6;   // Y 方向分割数

// --- ステージビルド ---
constexpr float MONSTER_Y = 320.f;   // モンスター配置 Y [px]
constexpr float SPACING = 160.f;     // スペース間隔 [px]

// --- UI パディング ---
constexpr float PADDING_X = 20.f;    // X 方向パディング [px]
constexpr float PADDING_Y = 10.f;    // Y 方向パディング [px]

} // namespace Constants
