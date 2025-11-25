#include "GameMain.h"
#include "../Screen/ScreenManager.h"
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


bool GameMain::Init()
{
    bool windowMode = WindowSetting::Instance().GetWindowMode();
    unsigned int w = WindowSetting::Instance().GetWindowSizeW();
    unsigned int h = WindowSetting::Instance().GetWindowSizeH();
    std::string title = WindowSetting::Instance().GetWindowTitle();

    sf::RenderWindow window(sf::VideoMode({ w, h }), title);

    m_window.setFramerateLimit(60);

    ScreenManager::Instance().Init();

    return true;
}

void GameMain::Run()
{
    while (m_window.isOpen())
    {
        sf::Clock fpsClock;
        sf::Time frameDuration = sf::seconds(1.f / 60.f);

        //sf::Event event();

        //while (m_window.pollEvent(event()))
        //{
        //    if (std::holds_alternative<sf::Event::Closed>(event.kind))
        //    {
        //        m_window.close();
        //    }
        //}

        // Escape ‰Ÿ‰º
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
            m_window.close();

        m_window.clear(sf::Color::Black);

        Update();
        Render();

        sf::Time elapsed = fpsClock.getElapsedTime();
        if (elapsed < frameDuration)
            sf::sleep(frameDuration - elapsed);

        m_window.display();
    }
}

void GameMain::Update()
{
    ScreenManager::Instance().Update();
}

void GameMain::Render()
{
    ScreenManager::Instance().Render(m_window);
}


ENTRY_POINT{

    GameMain::Instance().Init();
    GameMain::Instance().Run();

    return 0;
}