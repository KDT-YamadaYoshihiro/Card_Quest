#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Entity/Card/Card.h"

class CardRenderer
{
public:
    CardRenderer()
    { }

    // 山札描画
    void DrawDeck( const sf::Font& arg_font, sf::RenderWindow& window, sf::Vector2f pos, int count);
    // 墓地描画
    void DrawGrave(const sf::Font& arg_font, sf::RenderWindow& window, sf::Vector2f pos, int count);
    // 単体カード描画
    void DrawSingleCard(const sf::Font& arg_font, sf::RenderWindow& window, sf::Vector2f pos, const CardData& cardData, const std::string& IconKey);

    // 改行処理
    sf::String WarpText(const sf::String& arg_scr, const sf::Font& arg_font, unsigned int arg_charSize, float arg_maxWidth);
};
