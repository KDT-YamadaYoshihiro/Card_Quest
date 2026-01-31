#pragma once
#include <SFML/Graphics.hpp>
#include "System/Singleton/Singleton.h"

class GameMain : public Singleton<GameMain>
{

    friend class Singleton<GameMain>;

    sf::RenderWindow m_window;
    sf::Clock m_clock;
    bool m_isRunning = true;

    GameMain();

public:

    bool Init();           // 初期化
    void Run();            // メインループ開始
    void ProcessEvents();  // イベント処理
    void Update(float dt); // ゲーム更新
    void Render();         // 描画

    sf::RenderWindow& GetWindow() { return m_window; }


};
