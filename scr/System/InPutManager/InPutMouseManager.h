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

    /// <summary>
    /// 更新
    /// </summary>
    /// <param name="window"></param>
    void Update(const sf::RenderWindow& window)
    {
        m_prev = m_current;
        m_current = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

        sf::Vector2f mousePos(
            static_cast<float>(sf::Mouse::getPosition(window).x),
            static_cast<float>(sf::Mouse::getPosition(window).y)
        );

        // 押した瞬間
        if (m_current && !m_prev)
        {
            m_pressPos = mousePos;
            m_dragging = false;
        }

        // 押している間 → 移動量でドラッグ判定
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

        // 離した瞬間
        if (!m_current && m_prev)
        {
            m_dragging = false;
        }

        m_wheelDelta = 0.f;
    }

    /// <summary>
    /// イベント処理
    /// </summary>
    /// <param name="event"></param>
    void HandleEvent(const sf::Event& event)
    {
        if (event.is<sf::Event::MouseWheelScrolled>())
        {
            const auto& wheel = event.getIf<sf::Event::MouseWheelScrolled>();
            m_wheelDelta += wheel->delta;
        }
    }

    /// <summary>
    /// 左クリック判定
    /// </summary>
    /// <returns></returns>
    bool IsLeftClicked() const
    {
        return m_current && !m_prev;
    }

    /// <summary>
    /// ドラック判定
    /// </summary>
    /// <returns></returns>
    bool IsDragging() const
    {
        return m_dragging;
    }

    /// <summary>
    /// 左クリックが離された判定
    /// </summary>
    /// <returns></returns>
    bool IsLeftReleased() const
    {
        return !m_current && m_prev;
    }

    /// <summary>
    /// ホイールデータの取得
    /// </summary>
    /// <returns></returns>
    float GetWheelDelta() const
    {
        return m_wheelDelta;
	}

    /// <summary>
    /// マウス座標の取得
    /// </summary>
    /// <param name="window"></param>
    /// <returns></returns>
    sf::Vector2f GetMousePosition(const sf::RenderWindow& window) const
    {
        return sf::Vector2f(
            static_cast<float>(sf::Mouse::getPosition(window).x),
            static_cast<float>(sf::Mouse::getPosition(window).y)
        );
    }

    /// <summary>
    ///  左クリックの判定確認
    /// </summary>
    /// <returns></returns>
    bool IsLeftClickConfirmed() const
    {
        return IsLeftClicked() && !IsDragging();
    }
    
    /// <summary>
    /// キャンセル用判定
    /// </summary>
    /// <returns></returns>
    bool IsCancelTriggered() const
    {
        return sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape);
    }

    /// <summary>
    /// トリガーの確認
    /// </summary>
    /// <returns></returns>
    bool IsConfirmTriggered() const
    {
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter) || IsLeftClickConfirmed();
    }

};



