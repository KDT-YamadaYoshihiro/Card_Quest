#include "GameMain.h"
#include "../Screen/SceneManager.h"
#include "../Render/CameraManager.h"
#include "WindowSetting.h"
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Sleep.hpp>
#include "../CSVLoad/TextureLoader/TextureLoader.h"


#ifdef _DEBUG
#define ENTRY_POINT int main()
#else
#include <Windows.h>
#include "CameraManager.h"
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
    CameraManager::Instance().ViewInit({ 0.0f, 0.0f }, 
        { WindowSetting::Instance().GetWindowSizeW(), WindowSetting::Instance().GetWindowSizeH()});

	TextureLoader::Instance().LoadTextures("data/CSV/TextureData.csv");

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

        SceneManager::Instance().HandleEvent();

        if (event->is<sf::Event::Closed>())
        {
            m_window.close();
        }
    }
}

void GameMain::Update(float dt)
{
    SceneManager::Instance().Update();
    CameraManager::Instance().ViewUpdate(dt);
}


void GameMain::Render()
{
    m_window.clear(sf::Color::Black);
    SceneManager::Instance().Render(m_window);
    m_window.display();
}


ENTRY_POINT{

    GameMain::Instance().Run();

    return 0;
}