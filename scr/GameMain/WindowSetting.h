#pragma once
#include <string>
#include "System/Singleton/Singleton.h"

class WindowSetting : public Singleton<WindowSetting>
{

    friend class Singleton<WindowSetting>;

    bool m_windowMode;          // true = windowed, false = fullscreen
	sf::Vector2u m_windowSize; // window size
    std::string m_windowTitle;  // window title

    WindowSetting() : m_windowMode(true), m_windowSize({ 1280,720 }), m_windowTitle("Card_Quest") {}
    ~WindowSetting() {}

public:

    // コピー禁止、代入禁止
    WindowSetting(const WindowSetting&) = delete;
    WindowSetting& operator=(const WindowSetting&) = delete;
    static WindowSetting& GetInstance()
    {
        static WindowSetting instance;
        return instance;
    }

    //　表示するウィンドウモード（full , windowのセットゲット関数）
    void SetWindowMode(bool isWindowed) { m_windowMode = isWindowed; }
    bool GetWindowMode() const { return m_windowMode; }

    // ウィンドウサイズの取得
	const sf::Vector2u& GetWindowSize() const { return m_windowSize; }

    // ウィンドウタイトルの取得
    const std::string& GetWindowTitle() const { return m_windowTitle; }

};
