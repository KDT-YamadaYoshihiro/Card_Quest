#pragma once
#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>
#include "SceneBase.h"

class SceneManager {

    std::unique_ptr<SceneBase> m_screen;

    SceneManager();
    virtual ~SceneManager() = default;

public:

    // コピー、代入禁止
    SceneManager(const SceneManager&) = delete;
    SceneManager& operator = (const SceneManager&) = delete;
    static SceneManager& Instance() {
        static SceneManager instance;
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
