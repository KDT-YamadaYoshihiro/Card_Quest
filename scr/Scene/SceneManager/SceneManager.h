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

class SceneManager : public Singleton<SceneManager>
{
    friend class Singleton<SceneManager>;

    std::unique_ptr<SceneBase> m_scene;
    GameSession m_session;

public:



    // 更新・描画・イベント
    void Init(sf::RenderWindow& arg_window);
    void Update(sf::RenderWindow& arg_window);
    void Render(sf::RenderWindow& arg_window);
    void HandleEvent(const sf::Event& event);

    GameSession& GetSession();

    template <typename T,  typename... Args>
    void ChangeScreen(sf::RenderWindow& arg_window, Args&&... args)
    {
        auto newScene = std::make_unique<T>();

        m_scene.reset();
        m_scene = std::move(newScene);

        if (m_scene)
        {
            if (static_cast<T*>(m_scene.get())->Init(arg_window, std::forward<Args>(args)...))
            {
                ConsoleView::GetInstance().Add("SceneManager: Init Success\n");
            }
            else
            {
                ConsoleView::GetInstance().Add("SceneManager: Init Failed\n");
            }
        }
    }



};
