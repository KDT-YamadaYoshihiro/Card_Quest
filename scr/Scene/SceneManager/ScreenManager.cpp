#include "SceneManager.h"
#include "Scene/TitleScene/TitleScene.h"
#include <iostream>


void SceneManager::Init(sf::RenderWindow& arg_window)
{

    SceneManager::GetInstance().ChangeScreen<TitleScene>(arg_window);
    // エラー確認
    if (m_scene == nullptr) 
    {
        ConsoleView::GetInstance().Add("m_screenがnullです\n");
        return;
    }
}

void SceneManager::Update(sf::RenderWindow& arg_window)
{
    m_scene->Update(arg_window);
}

void SceneManager::Render(sf::RenderWindow& window)
{
    m_scene->Render(window);
}

void SceneManager::HandleEvent(const sf::Event& event)
{
    m_scene->handleEvent(event);
}

GameSession& SceneManager::GetSession()
{
    return m_session; 
}

