// src/System/Constants/AnimationConstants.hpp
#pragma once

namespace ui_layout {

    // --- Position Constants (Coordinates) ---
    constexpr float DECK_X_START = 10.f;              // デッキラベルの左端X座標
    constexpr float INFO_TEXT_Y_OFFSET = 80.f;        // 指示テキストの基準Y座標
    constexpr float INSTRUCTION_TEXT_X = 430.f;       // 指示テキストの固定X座標

    // --- Offset Constants (Relative Positioning) ---
    constexpr float DECK_LABEL_RELATIVE_Y_OFFSET = -35.f; // デッキラベルの基準からの相対Yオフセット
    constexpr float CARD_COUNT_Y_OFFSET = 130.f;        // カウントテキストのY座標オフセット

    // --- Fixed Timing/Duration (If any more are found, they will be added here) ---

} // namespace ui_layout