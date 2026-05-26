#include "GameMain.h"
#include "CSVLoad/TextureLoader/TextureLoader.h"
#include "CSVLoad/CharacterLoader.h"
#include "CSVLoad/CardLoader.h"
#include "CSVLoad/StageLoader/StageLoader.h"
#include "CSVLoad/EffectDataLoder/EffectDataLoder.h"
#include "Scene/Scenemanager/SceneManager.h"
#include "View/CameraManager/CameraManager.h"
#include "WindowSetting.h"
#include "View/Font/FontManager.h"
#include "View/ConsoleView/ConsoleView.h"
#include "System/Constants.h"
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Sleep.hpp>
#include<SFML/Window.hpp>
#include<SFML/Window/WindowEnums.hpp>

#ifdef _DEBUG
#define ENTRY_POINT int main()
#else
#include <Windows.h>
#define ENTRY_POINT int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
#endif // _DEBUG

GameMain::GameMain() :
    m_window(sf::VideoMode({ Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT }), "Card Quest")
{

}

bool GameMain::Init()
{

    m_window.setFramerateLimit(Constants::TARGET_FPS);

#ifdef _DEBUG
    // �f�o�b�O���� WindowSetting �ɕۑ�����Ă���T�C�Y�ŃE�B���h�E�\\��
    m_window.create(sf::VideoMode({ Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT }), "Card Quest");
#else
    m_window.create(
        sf::VideoMode::getDesktopMode(),
        "Card Quest",
        sf::State::Fullscreen
    );
#endif // _DEBUG


    sf::Vector2f size = sf::Vector2f(m_window.getSize());
    sf::View view(sf::FloatRect({ 0.f, 0.f }, { Constants::WINDOW_WIDTH.f, Constants::WINDOW_HEIGHT.f }));
    m_window.setView(view);

    SceneManager::GetInstance().Init(m_window);
    CameraManager::GetInstance().ViewInit({ 0.0f, 0.0f }, static_cast<sf::Vector2f>( WindowSetting::GetInstance().GetWindowSize()));

    // CSV�̓ǂݍ���
	TextureLoader::GetInstance().LoadTextures("data/CSV/TextureData.csv");
	CharacterLoader::GetInstance().LoadCSV("data/CSV/CharacterData.csv");
    CardLoader::GetInstance().LoadCSV("data/CSV/CardData.csv");
    StageLoader::GetInstance().LoadCSV("data/CSV/StageData.csv");
	EffectDataLoder::GetInstance().LoadCSV("data/CSV/EffectData.csv");
    
    // �t�H���g�ǂݍ���
    FontManager::GetInstance().FontLoad();


    return true;
}

void GameMain::Run()
{
    if (!Init())
    {
        return;
    }

	m_clock.restart();

    while (m_isRunning && m_window.isOpen())
    {
        float dt = m_clock.restart().asSeconds();

        if (dt <= 0.0f)
        {
            dt = Constants::FRAME_TIME;
        }

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
    SceneManager::GetInstance().Update(m_window, dt);
    CameraManager::GetInstance().ViewUpdate(dt);
}


void GameMain::Render()
{
    m_window.clear(sf::Color::Black);
    SceneManager::GetInstance().Render(m_window);
    ConsoleView::GetInstance().RenderFrame();
    m_window.display();
}


ENTRY_POINT{

    GameMain::GetInstance().Run();

    return 0;
}