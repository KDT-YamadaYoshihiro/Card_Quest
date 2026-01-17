#pragma once
#include <SFML/Graphics.hpp>
class InputManager
{
public:

    static InputManager& GetInstance()
    {
        static InputManager instance;
        return instance;
    }

    void Update()
    {
        m_prev = m_current;
        m_current = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
    }

    bool IsLeftClicked() const
    {
        return m_current && !m_prev;
    }

    bool IsLeftPressed() const
    {
        return m_current;
    }

private:
    bool m_prev = false;
    bool m_current = false;
};


