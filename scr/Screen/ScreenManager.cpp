#include "SceneManager.h"
#include "TitleScene.h"
#include <iostream>

SceneManager::SceneManager()
{

    SceneManager::ChangeScreen<TitleScene>();

    if (m_screen == nullptr) {
        std::cout << "m_screen‚ªnull‚Å‚·" << std::endl;
        return;
    }

}

void SceneManager::Init()
{
    if (m_screen == nullptr) {
        std::cout << "m_screen‚ªnull‚Å‚·" << std::endl;
        return;
    }

    m_screen->Init();
}

void SceneManager::Update()
{
    if (m_screen == nullptr) {
        std::cout << "m_screen‚ªnull‚Å‚·" << std::endl;
        return;
    }

    m_screen->Update();
}

void SceneManager::Render(sf::RenderWindow& window)
{
    if (m_screen == nullptr) {
        std::cout << "m_screen‚ªnull‚Å‚·" << std::endl;
        return;
    }
    m_screen->Render();
}

void SceneManager::HandleEvent(const sf::Event& ev)
{
    if (m_screen == nullptr) {
        std::cout << "m_screen‚ªnull‚Å‚·" << std::endl;
        return;
    }
}

