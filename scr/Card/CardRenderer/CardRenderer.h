#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "../Card.h"

class CardRenderer
{
public:
    CardRenderer()
    { }

    // RD•`‰æ
    void DrawDeck( const sf::Font& arg_font, sf::RenderWindow& window, sf::Vector2f pos, int count);
    // •æ’n•`‰æ
    void DrawGrave(const sf::Font& arg_font, sf::RenderWindow& window, sf::Vector2f pos, int count);
    // èD•`‰æ
    void DrawHand(const sf::Font& arg_font, sf::RenderWindow& window, sf::Vector2f pos, const Card& card);

    // ‰üsˆ—
    sf::String WarpText(const sf::String& arg_scr, const sf::Font& arg_font, unsigned int arg_charSize, float arg_maxWidth);
};
