#pragma once
#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>
#include "SceneBase.h"

class SceneManager {

    std::unique_ptr<SceneBase> m_screen;

    SceneManager() = default;
    virtual ~SceneManager() = default;

public:

    // コピー、代入禁止
    SceneManager(const SceneManager&) = delete;
    SceneManager& operator = (const SceneManager&) = delete;
    static SceneManager& GetInstance() {
        static SceneManager instance;
        return instance;
    }


    // 更新・描画・イベント
    void Init(sf::RenderWindow& arg_window);
    void Update(sf::RenderWindow& arg_window);
    void Render(sf::RenderWindow& arg_window);
    void HandleEvent(const sf::Event& event);

    template <typename T>
    void ChangeScreen(sf::RenderWindow& arg_window)
    {
        m_screen = std::make_unique<T>(arg_window);
    }

};
