#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "../Card.h"

class CardRenderer
{
public:
    CardRenderer()
    { }

    void DrawDeck(
        const sf::Font arg_font,
        sf::RenderWindow& window,
        sf::Vector2f pos,
        int count
    );

    void DrawGrave(
        const sf::Font arg_font,
        sf::RenderWindow& window,
        sf::Vector2f pos,
        int count
    );

    void DrawHand(
        const sf::Font arg_font,
        sf::RenderWindow& window,
        sf::Vector2f pos,
        const Card& card
    );
};
