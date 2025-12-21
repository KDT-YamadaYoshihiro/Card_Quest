#include "CardRenderer.h"

/// <summary>
/// 山札の描画
/// </summary>
/// <param name="window"></param>
/// <param name="pos">座標</param>
/// <param name="count">枚数</param>
void CardRenderer::DrawDeck(const sf::Font arg_font, sf::RenderWindow& window, sf::Vector2f pos, int count)
{
    sf::RectangleShape rect({ 80.f, 120.f });
    rect.setFillColor(sf::Color(160, 160, 160));
    rect.setOutlineColor(sf::Color::Black);
    rect.setOutlineThickness(2.f);
    rect.setPosition(pos);
    window.draw(rect);

    sf::Text text(arg_font,std::to_string(count));
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::White);
    text.setPosition({ pos.x + 30.0f, pos.y + 45.0f });
    window.draw(text);
}

/// <summary>
/// 墓地の描画
/// </summary>
/// <param name="window"></param>
/// <param name="pos">座標</param>
/// <param name="count">枚数</param>
void CardRenderer::DrawGrave(const sf::Font arg_font, sf::RenderWindow& window, sf::Vector2f pos, int count)
{
    DrawDeck(arg_font,window, pos, count); // 今回は同じ見た目
}

/// <summary>
/// 手札の描画
/// </summary>
/// <param name="window"></param>
/// <param name="pos">座標</param>
/// <param name="card">カード</param>
void CardRenderer::DrawHand(const sf::Font arg_font, sf::RenderWindow& window, sf::Vector2f pos, const Card& card)
{
    sf::RectangleShape rect({ 100.f, 140.f });
    rect.setFillColor(sf::Color(230, 230, 230));
    rect.setOutlineColor(sf::Color::Black);
    rect.setOutlineThickness(2.f);
    rect.setPosition(pos);
    window.draw(rect);

    const auto& data = card.GetCardState();

    sf::Text nameText(arg_font, data.name);
    nameText.setCharacterSize(14);
    nameText.setFillColor(sf::Color::Black);
    nameText.setPosition({ pos.x + 5.0f, pos.y + 5.0f });
    window.draw(nameText);

    sf::Text descText(arg_font, data.description);
    descText.setCharacterSize(12);
    descText.setFillColor(sf::Color::Black);
    descText.setPosition({ pos.x + 5.0f, pos.y + 30.0f });
    window.draw(descText);
}
