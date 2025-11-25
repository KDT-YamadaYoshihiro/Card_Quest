#include "ScreenManager.h"
#include "TitleScreen.h"
#include <iostream>

ScreenManager::ScreenManager()
{
    ScreenManager::ChangeScreen<TitleScreen>();
    if (m_screen == nullptr) {
        std::cout << "m_screen‚ªnull‚Å‚·" << std::endl;
        return;
    }

}

void ScreenManager::Init()
{
    if (m_screen == nullptr) {
        std::cout << "m_screen‚ªnull‚Å‚·" << std::endl;
        return;
    }

    m_screen->Init();
}

void ScreenManager::Update()
{
    if (m_screen == nullptr) {
        std::cout << "m_screen‚ªnull‚Å‚·" << std::endl;
        return;
    }

    m_screen->Update();
}

void ScreenManager::Render(sf::RenderWindow& window)
{
    if (m_screen == nullptr) {
        std::cout << "m_screen‚ªnull‚Å‚·" << std::endl;
        return;
    }
    m_screen->Render();
}

void ScreenManager::HandleEvent(const sf::Event& ev)
{
    if (m_screen == nullptr) {
        std::cout << "m_screen‚ªnull‚Å‚·" << std::endl;
        return;
    }
}

