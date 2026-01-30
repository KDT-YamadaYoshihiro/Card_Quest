#pragma once
#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Scene/SceneBase.h"
#include "System/Battle/BattleContex/BattleContext.h"


class GameSession {
public:
    std::shared_ptr<BattleContext> battleContext;
};

class SceneManager {

    std::unique_ptr<SceneBase> m_screen;
    GameSession m_session;

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

    GameSession& GetSession();

    template <typename T,  typename... Args>
    void ChangeScreen(Args&&... args)
    {
        m_screen = std::make_unique<T>(std::forward<Args>(args)...);
    }

};
