#include "SceneManager.h"
#include "Scene/TitleScene/TitleScene.h"
#include <iostream>


void SceneManager::Init(sf::RenderWindow& arg_window)
{

    SceneManager::GetInstance().ChangeScreen<TitleScene>();
    // エラー確認
    if (m_screen == nullptr) 
    {
        ConsoleView::GetInstance().Add("m_screenがnullです\n");
        return;
    }

    m_screen->Init(arg_window);
}

void SceneManager::Update(sf::RenderWindow& arg_window)
{
    m_screen->Update(arg_window);
}

void SceneManager::Render(sf::RenderWindow& window)
{
    m_screen->Render(window);
}

void SceneManager::HandleEvent(const sf::Event& event)
{
    m_screen->handleEvent(event);
}

GameSession& SceneManager::GetSession()
{
    return m_session; 
}

