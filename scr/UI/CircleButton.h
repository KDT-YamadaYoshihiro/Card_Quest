#pragma once
#include <SFML/Graphics.hpp>

class CircleButton
{
private:
    // 円形の形状
    sf::CircleShape m_shape;

public:

    CircleButton(float radius, sf::Vector2f pos)
    {
        m_shape.setRadius(radius);
        m_shape.setOrigin({ radius, radius });
        m_shape.setPosition(pos);
        m_shape.setFillColor(sf::Color(200, 200, 200));
    }

	// 描画
    void Draw(sf::RenderWindow& window) const
    {
        window.draw(m_shape);
    }

	// ホバーしているか
    bool IsHovered(sf::Vector2f mousePos) const
    {
        sf::Vector2f diff = mousePos - m_shape.getPosition();
        return (diff.x * diff.x + diff.y * diff.y) <= m_shape.getRadius() * m_shape.getRadius();
    }

	// クリックされたか
    bool IsClicked(sf::Vector2f mousePos, bool mousePressed) const
    {
        return mousePressed && IsHovered(mousePos);
    }

};
