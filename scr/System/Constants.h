#pragma once

// ============================================================================
// 共通定数 (Common Constants)
// ============================================================================

namespace Constants {

// --- ウィンドウ設定 (Window Settings) ---
constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 720;
constexpr float WINDOW_CENTER_X = 640.f;
constexpr float WINDOW_CENTER_Y = 360.f;
constexpr int TARGET_FPS = 60;           // ターゲットフレームレート
constexpr float FRAME_TIME = 1.0f / TARGET_FPS;  // フレーム時間 (TARGET_FPS)

// --- キャラクター・カードサイズ (Character & Card Sizes) ---
constexpr float CHAR_WIDTH = 165.f;   // キャラクター幅 [px]
constexpr float CHAR_HEIGHT = 150.f;  // キャラクター高さ [px]
constexpr float CARD_WIDTH = 120.f;   // カード幅 [px]
constexpr float CARD_HEIGHT = 160.f;  // カード高さ [px]

// --- ハンド・配置 (Hand & Positioning) ---
constexpr sf::Vector2f HAND_START{ 200.f, 520.f };
constexpr float HAND_SPACING = 180.f;    // ハンド間隔 [px]
constexpr float SELECT_OFFSET_Y = 50.f;  // セレクト時のオフセット Y [px]

// --- モンスター配置 (Monster Positioning) ---
constexpr float MONSTER_Y = 320.f;   // モンスター配置 Y [px]

// --- スクロール・オフセット (Scroll & Offset) ---
constexpr float STAGE_SCROLL_OFFSET = 800.f;  // ステージ選択スクロールオフセット
constexpr float WHEEL_SCROLL_MULTIPLIER = 40.f;  // スクロールホイール倍率

// --- アニメーション・物理値 (Animation & Physics) ---
constexpr float PI = 3.14159265f;           // 円周率
constexpr float DEFAULT_ALPHA_SPEED = 200.f;  // デフォルトアルファ変化速度 [ms]
constexpr float FRAME_TIME = 1.0f / 60.0f;   // フレーム時間 (60fps)

// --- UI・ボタンサイズ (UI & Button Sizes) ---
constexpr float BUTTON_WIDTH = 200.f;      // ボタン幅 [px]
constexpr float BUTTON_HEIGHT = 50.f;      // ボタン高さ [px]
constexpr sf::Vector2f DEFAULT_BUTTON_SIZE{ BUTTON_WIDTH, BUTTON_HEIGHT };

// --- UI・配置位置 (UI & Positioning) ---
constexpr float HAND_START_X = 200.f;      // ハンド開始 X 座標 [px]
constexpr float HAND_START_Y = 520.f;      // ハンド開始 Y 座標 [px]
constexpr sf::Vector2f HAND_START{ HAND_START_X, HAND_START_Y };

// --- ドローコスト表示位置 (Draw Cost Position) ---
constexpr float DRAW_COST_OFFSET_X = 110.f;   // センターからの X オフセット [px]
constexpr float DRAW_COST_OFFSET_Y = 130.f;   // Y 座標 [px]

// --- フォントサイズ (Font Sizes) ---
constexpr int FONT_SIZE_SMALL = 20;      // 小文字 [pt]
constexpr int FONT_SIZE_MEDIUM = 14;     // 中文字 [pt]
constexpr int FONT_SIZE_LARGE = 12;      // 大文字 [pt]
constexpr int FONT_SIZE_TITLE = 25;      // タイトル [pt]
constexpr sf::Vector2f BENCH_SIZE{ 1200.0f, 220.0f };
constexpr sf::Vector2f PARTY_POS{ 180.0f, 100.0f };
constexpr sf::Vector2f PARTY_SIZE{ 900.0f, 250.0f };
constexpr float PARTY_ICON_SIZE_X = 125.0f;
constexpr float PARTY_ICON_SIZE_Y = 110.0f;

// --- ステージビルド (Stage Build) ---
constexpr sf::Vector2f DETAIL_PANEL_SIZE{ 300.f, 400.f };
constexpr float DETAIL_PANEL_X_OFFSET = 640.f;  // センターからのオフセット

// --- カメラ・ビュー (Camera & View) ---
constexpr sf::Vector2f CAMERA_TARGET{ 640.0f, 360.0f };
constexpr float CAMERA_ZOOM_SPEED = 4.0f;

// --- デッキビルド (Deck Build) ---
constexpr int MAX_DECK_SIZE = 30;
constexpr sf::Vector2f DECK_START{ 50.f, 160.f };
constexpr sf::Vector2f POOL_START{ 50.f, 420.f };
constexpr float CARD_SPACING = 160.f;

// --- カード ID (Card IDs) ---
constexpr int COMMON_CARD_ID = 1000;
constexpr int UNIQUE_CARD_ID = 1100;

} // namespace Constants
