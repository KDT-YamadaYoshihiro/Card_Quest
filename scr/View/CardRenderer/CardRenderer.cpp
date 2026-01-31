#include "CardRenderer.h"
#include "CSVLoad/TextureLoader/TextureLoader.h"

#include <iostream>
/// <summary>
/// 山札の描画
/// </summary>
/// <param name="window"></param>
/// <param name="pos">座標</param>
/// <param name="count">枚数</param>
void CardRenderer::DrawDeck(const sf::Font& arg_font, sf::RenderWindow& window, sf::Vector2f pos, int count)
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
void CardRenderer::DrawGrave(const sf::Font& arg_font, sf::RenderWindow& window, sf::Vector2f pos, int count)
{
    DrawDeck(arg_font,window, pos, count); // 今回は同じ見た目

}

/// <summary>
/// 単体カード描画
/// </summary>
/// <param name="window"></param>
/// <param name="pos">座標</param>
/// <param name="card">カード</param>
void CardRenderer::DrawSingleCard(const sf::Font& arg_font, sf::RenderWindow& window, sf::Vector2f pos, const CardData& cardData, const std::string& IconKey)
{
    // カードのベース（白地）
    sf::RectangleShape rect({ 120.f, 160.f });
    rect.setFillColor(sf::Color(230, 230, 230));
    rect.setOutlineColor(sf::Color::Black);
    rect.setOutlineThickness(2.0f);
    rect.setPosition(pos);
    window.draw(rect);


    // 技名
    sf::Text nameText(arg_font, sf::String::fromUtf8(cardData.name.begin(), cardData.name.end()));
    nameText.setCharacterSize(14);
    nameText.setFillColor(sf::Color::Black);
    nameText.setPosition({ pos.x + 5.0f, pos.y + 5.0f });
    window.draw(nameText);

    // 説明
    sf::String desc = sf::String::fromUtf8(cardData.description.begin(), cardData.description.end());
    // 改行処理
    desc = WarpText(desc, arg_font, 12, 100.0f);

    sf::Text descText(arg_font,desc);
    descText.setCharacterSize(12);
    descText.setFillColor(sf::Color::Black);
    descText.setPosition({ pos.x + 5.0f, pos.y + 30.0f });
    window.draw(descText);

    // 所持キャラクターのアイコン表示
    auto tex = TextureLoader::GetInstance().GetTextureID(IconKey);
    if (!tex)
    {
        // アイコンが見つからないとき何もしない
        return;
    }
    sf::Sprite icon(*tex);
    icon.setPosition({pos.x, pos.y + 100.0f});
    icon.setOrigin({ icon.getLocalBounds().size.x, 0.0f });
    icon.setScale(sf::Vector2f(-0.7f, 0.7f));
    window.draw(icon);
}


/// <summary>
/// 改行処理
/// </summary>
/// <param name="arg_src"></param>
/// <param name="arg_font">フォント</param>
/// <param name="arg_charSize"></param>
/// <param name="arg_maxWidth"></param>
/// <returns></returns>
sf::String CardRenderer::WarpText(const sf::String& arg_src, const sf::Font& arg_font, unsigned int arg_charSize, float arg_maxWidth)
{
    sf::String result;
    float lineWidth = 0.f;

    for (std::size_t i = 0; i < arg_src.getSize(); ++i)
    {
        char32_t c = arg_src[i];  // ← SFML3ではこれ

        // 明示的な改行
        if (c == U'\n')
        {
            result += U'\n';
            lineWidth = 0.f;
            continue;
        }

        const sf::Glyph& glyph = arg_font.getGlyph(c, arg_charSize, false);
        float advance = glyph.advance;

        if (lineWidth + advance > arg_maxWidth)
        {
            result += U'\n';
            lineWidth = 0.f;
        }

        result += c;
        lineWidth += advance;
    }

    return result;
}
