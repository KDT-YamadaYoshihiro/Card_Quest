#include "SceneManager.h"
#include "../Screen/TitleScene/TitleScene.h"
#include <iostream>


void SceneManager::Init(sf::RenderWindow& arg_window)
{

    SceneManager::GetInstance().ChangeScreen<TitleScene>(arg_window);
    // エラー確認
    if (m_screen == nullptr) {
        std::cout << "m_screenがnullです" << std::endl;
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

