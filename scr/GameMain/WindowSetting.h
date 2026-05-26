#pragma once
#include <string>
#include "System/Singleton/Singleton.h"

class WindowSetting : public Singleton<WindowSetting>
{

    friend class Singleton<WindowSetting>;

    bool m_windowMode;          // true = windowed, false = fullscreen
	sf::Vector2u m_windowSize; // window size
    std::string m_windowTitle;  // window title

    WindowSetting() : m_windowMode(true), m_windowSize({ Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT }), m_windowTitle("Card_Quest") {}
    ~WindowSetting() {}

public:

    // �R�s�[�֎~�A����֎~
    WindowSetting(const WindowSetting&) = delete;
    WindowSetting& operator=(const WindowSetting&) = delete;
    static WindowSetting& GetInstance()
    {
        static WindowSetting instance;
        return instance;
    }

    //�@�\������E�B���h�E���[�h�ifull , window�̃Z�b�g�Q�b�g�֐��j
    void SetWindowMode(bool isWindowed) { m_windowMode = isWindowed; }
    bool GetWindowMode() const { return m_windowMode; }

    // �E�B���h�E�T�C�Y�̎擾
	const sf::Vector2u& GetWindowSize() const { return m_windowSize; }

    // �E�B���h�E�^�C�g���̎擾
    const std::string& GetWindowTitle() const { return m_windowTitle; }

};
