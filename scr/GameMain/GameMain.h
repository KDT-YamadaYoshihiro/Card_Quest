#pragma once
#include <SFML/Graphics.hpp>

class GameMain
{
    GameMain() = default;
    virtual ~GameMain() = default;

public:

    GameMain(const GameMain&) = delete;
    GameMain& operator=(const GameMain&) = delete;
    static GameMain& Instance()
    {
        static GameMain instance;
        return instance;
    }

    bool Init();
    void Run();
    void Update();
    void Render();

    sf::RenderWindow& GetWindow() { return m_window; }


private:
    sf::RenderWindow m_window;
};
