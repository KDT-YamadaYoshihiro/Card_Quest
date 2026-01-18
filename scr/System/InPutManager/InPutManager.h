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
        // 毎フレームリセット
        m_wheelDelta = 0.f;
    }

    void HandleEvent(const sf::Event& event)
    {
        if (event.is<sf::Event::MouseWheelScrolled>())
        {
            const auto& wheel = event.getIf<sf::Event::MouseWheelScrolled>();
            m_wheelDelta += wheel->delta;
        }
    }

    bool IsLeftClicked() const
    {
        return m_current && !m_prev;
    }

    bool IsLeftPressed() const
    {
        return m_current;
    }

    float GetWheelDelta() const
    {
        return m_wheelDelta;
	}
private:
    bool m_prev = false;
    bool m_current = false;
    float m_wheelDelta = 0.f;
};


