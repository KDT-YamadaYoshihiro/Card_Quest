#pragma once
#include <SFML/Graphics.hpp>

class GameMain
{
    sf::RenderWindow m_window;
    sf::Clock m_clock;
    bool m_isRunning = true;

    GameMain();
    virtual ~GameMain() = default;

public:

    GameMain(const GameMain&) = delete;
    GameMain& operator=(const GameMain&) = delete;
    static GameMain& Instance()
    {
        static GameMain instance;
        return instance;
    }

    bool Init();           // 初期化
    void Run();            // メインループ開始
    void ProcessEvents();  // イベント処理
    void Update(float dt); // ゲーム更新
    void Render();         // 描画

    sf::RenderWindow& GetWindow() { return m_window; }


};
