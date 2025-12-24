#include "SceneManager.h"
#include "../Screen/TitleScene/TitleScene.h"
#include <iostream>


void SceneManager::Init()
{

    SceneManager::GetInstance().ChangeScreen<TitleScene>();
    // エラー確認
    if (m_screen == nullptr) {
        std::cout << "m_screenがnullです" << std::endl;
        return;
    }

    m_screen->Init();
}

void SceneManager::Update()
{
    m_screen->Update();
}

void SceneManager::Render(sf::RenderWindow& window)
{
    m_screen->Render(window);
}

void SceneManager::HandleEvent()
{
    m_screen->handleEvent();
}

