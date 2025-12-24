#include "GameMain.h"
#include "../CSVLoad/TextureLoader/TextureLoader.h"
#include "../CSVLoad/CharacterLoader.h"
#include "../CSVLoad/CardLoader.h"
#include "../Screen/SceneManager.h"
#include "../CameraManager/CameraManager.h"
#include "WindowSetting.h"
#include "../../View/Font/FontManager.h"
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Sleep.hpp>


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

    SceneManager::GetInstance().Init();
    CameraManager::GetInstance().ViewInit({ 0.0f, 0.0f }, 
        { WindowSetting::GetInstance().GetWindowSizeW(), WindowSetting::GetInstance().GetWindowSizeH()});

	TextureLoader::GetInstance().LoadTextures("data/CSV/TextureData.csv");
	CharacterLoader::GetInstance().Load("data/CSV/CharacterData.csv");
    CardLoader::GetInstance().Load("data/CSV/CardData.csv");
    FontManager::GetInstance().FontLoad();


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

        SceneManager::GetInstance().HandleEvent();

        if (event->is<sf::Event::Closed>())
        {
            m_window.close();
        }
    }
}

void GameMain::Update(float dt)
{
    SceneManager::GetInstance().Update();
    CameraManager::GetInstance().ViewUpdate(dt);
}


void GameMain::Render()
{
    m_window.clear(sf::Color::Black);
    SceneManager::GetInstance().Render(m_window);
    m_window.display();
}


ENTRY_POINT{

    GameMain::Instance().Run();

    return 0;
}