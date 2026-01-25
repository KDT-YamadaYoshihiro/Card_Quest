#pragma once
#include <SFML/Graphics.hpp>

class InPutMouseManager
{
private:
    bool m_prev = false;
    bool m_current = false;
    bool m_dragging = false;
    sf::Vector2f m_pressPos;
    float m_wheelDelta = 0.f;

public:

    static InPutMouseManager& GetInstance()
    {
        static InPutMouseManager instance;
        return instance;
    }

    void Update(const sf::RenderWindow& window)
    {
        m_prev = m_current;
        m_current = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

        sf::Vector2f mousePos(
            static_cast<float>(sf::Mouse::getPosition(window).x),
            static_cast<float>(sf::Mouse::getPosition(window).y)
        );

        // âüÇµÇΩèuä‘
        if (m_current && !m_prev)
        {
            m_pressPos = mousePos;
            m_dragging = false;
        }

        // âüÇµÇƒÇ¢ÇÈä‘ Å® à⁄ìÆó Ç≈ÉhÉâÉbÉOîªíË
        if (m_current && m_prev)
        {
            const float DRAG_THRESHOLD = 5.0f;
            float dx = std::abs(mousePos.x - m_pressPos.x);
            float dy = std::abs(mousePos.y - m_pressPos.y);

            if (dx + dy > DRAG_THRESHOLD)
            {
                m_dragging = true;
            }
        }

        // ó£ÇµÇΩèuä‘
        if (!m_current && m_prev)
        {
            m_dragging = false;
        }

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

    bool IsDragging() const
    {
        return m_dragging;
    }

    bool IsLeftReleased() const
    {
        return !m_current && m_prev;
    }

    float GetWheelDelta() const
    {
        return m_wheelDelta;
	}

    sf::Vector2f GetMousePosition(const sf::RenderWindow& window) const
    {
        return sf::Vector2f(
            static_cast<float>(sf::Mouse::getPosition(window).x),
            static_cast<float>(sf::Mouse::getPosition(window).y)
        );
    }

    bool IsLeftClickConfirmed() const
    {
        return IsLeftClicked() && !IsDragging();
    }

    bool IsCancelTriggered() const
    {
        return sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape);
    }

    bool IsConfirmTriggered() const
    {
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter) || IsLeftClickConfirmed();
    }

};



