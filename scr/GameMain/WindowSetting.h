#pragma once
#include <string>

class WindowSetting
{
    bool m_windowMode;          // true = windowed, false = fullscreen
    float m_windowWidth;          // width
    float m_windowHeight;         // height
    std::string m_windowTitle;  // window title

    WindowSetting() : m_windowMode(true), m_windowWidth(1280), m_windowHeight(720), m_windowTitle("Game") {}
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
    float GetWindowSizeW() const { return m_windowWidth; }
    float GetWindowSizeH() const { return m_windowHeight; }

    // ウィンドウタイトルの取得
    const std::string& GetWindowTitle() const { return m_windowTitle; }

};
