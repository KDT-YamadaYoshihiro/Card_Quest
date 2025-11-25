#pragma once
#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>
#include "ScreenBase.h"

class ScreenManager {

    std::unique_ptr<ScreenBase> m_screen;

    ScreenManager();
    virtual ~ScreenManager() = default;

public:

    // コピー、代入禁止
    ScreenManager(const ScreenManager&) = delete;
    ScreenManager& operator = (const ScreenManager&) = delete;
    static ScreenManager& Instance() {
        static ScreenManager instance;
        return instance;
    }


    // 更新・描画・イベント
    void Init();
    void Update();
    void Render(sf::RenderWindow& window);
    void HandleEvent(const sf::Event& ev);

    template <typename T>
    void ChangeScreen()
    {
        m_screen = std::make_unique<T>();
    }

};
