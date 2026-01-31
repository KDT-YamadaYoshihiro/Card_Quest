#include "GameMain.h"
#include "CSVLoad/TextureLoader/TextureLoader.h"
#include "CSVLoad/CharacterLoader.h"
#include "CSVLoad/CardLoader.h"
#include "CSVLoad/StageLoader/StageLoader.h"
#include "Scene/Scenemanager/SceneManager.h"
#include "View/CameraManager/CameraManager.h"
#include "WindowSetting.h"
#include "View/Font/FontManager.h"
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
    m_window(sf::VideoMode({ 1280, 720 }),"Card Quest")
{

}

bool GameMain::Init()
{

    m_window.setFramerateLimit(60);

    
    SceneManager::GetInstance().Init(m_window);
    CameraManager::GetInstance().ViewInit({ 0.0f, 0.0f }, 
        { WindowSetting::GetInstance().GetWindowSizeW(), WindowSetting::GetInstance().GetWindowSizeH()});

    // CSV‚Ì“Ç‚Ýž‚Ý
	TextureLoader::GetInstance().LoadTextures("data/CSV/TextureData.csv");
	CharacterLoader::GetInstance().LoadCSV("data/CSV/CharacterData.csv");
    CardLoader::GetInstance().LoadCSV("data/CSV/CardData.csv");
    StageLoader::GetInstance().LoadCSV("data/CSV/StageData.csv");
    // ƒtƒHƒ“ƒg“Ç‚Ýž‚Ý
    FontManager::GetInstance().FontLoad();


    return true;
}

void GameMain::Run()
{
    if (!Init())
    {
        return;
    }

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
    while (auto optevent = m_window.pollEvent())
    {
		const sf::Event& event = *optevent;
        SceneManager::GetInstance().HandleEvent(event);

        if (optevent->is<sf::Event::Closed>())
        {
            m_window.close();
        }
    }
}

void GameMain::Update(float dt)
{
    SceneManager::GetInstance().Update(m_window);
    CameraManager::GetInstance().ViewUpdate(dt);
}


void GameMain::Render()
{
    m_window.clear(sf::Color::Black);
    SceneManager::GetInstance().Render(m_window);
    m_window.display();
}


ENTRY_POINT{

    GameMain::GetInstance().Run();

    return 0;
}