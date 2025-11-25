#include "GameMain.h"
#include "../Screen/SceneManager.h"
#include "WindowSetting.h"
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Sleep.hpp>

#ifdef _DEBUG
#define ENTRY_POINT int main()
#else
#include <Windows.h>
#define ENTRY_POINT int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
#endif // _DEBUG

GameMain::GameMain() :
    m_window(sf::VideoMode({ 1280, 720 }),"Game")
{

}

bool GameMain::Init()
{

    m_window.setFramerateLimit(60);

    SceneManager::Instance().Init();

    return true;
}

void GameMain::Run()
{
    if (!Init()) return;

    while (m_isRunning && m_window.isOpen())
    {
        float dt = m_clock.restart().asSeconds();

        ProcessEvents();
        Update(dt);
        Render();
    }
}

void GameMain::ProcessEvents()
{
    while (auto event = m_window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            m_window.close();
    }
}

void GameMain::Update(float dt)
{
    SceneManager::Instance().Update();
}

void GameMain::Render()
{
    SceneManager::Instance().Render(m_window);
}


ENTRY_POINT{

    GameMain::Instance().Init();
    GameMain::Instance().Run();

    return 0;
}