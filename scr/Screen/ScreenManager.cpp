#include "SceneManager.h"
#include "TitleScene.h"
#include <iostream>

SceneManager::SceneManager()
{
    SceneManager::ChangeScreen<TitleScene>();
#ifdef  _DEBUG
    if (m_screen == nullptr) {
        std::cout << "m_screen‚ªnull‚Å‚·" << std::endl;
        return;
    }
#endif //  _DEBUG

}

void SceneManager::Init()
{
#ifdef  _DEBUG
    if (m_screen == nullptr) {
        std::cout << "m_screen‚ªnull‚Å‚·" << std::endl;
        return;
    }
#endif //  _DEBUG

    m_screen->Init();
}

void SceneManager::Update()
{
#ifdef  _DEBUG
    if (m_screen == nullptr) {
        std::cout << "m_screen‚ªnull‚Å‚·" << std::endl;
        return;
    }
#endif //  _DEBUG

    m_screen->Update();
}

void SceneManager::Render(sf::RenderWindow& window)
{
#ifdef  _DEBUG
    if (m_screen == nullptr) {
        std::cout << "m_screen‚ªnull‚Å‚·" << std::endl;
        return;
    }
#endif //  _DEBUG
    m_screen->Render();
}

void SceneManager::HandleEvent(const sf::Event& ev)
{
#ifdef  _DEBUG
    if (m_screen == nullptr) {
        std::cout << "m_screen‚ªnull‚Å‚·" << std::endl;
        return;
    }
#endif //  _DEBUG
}

