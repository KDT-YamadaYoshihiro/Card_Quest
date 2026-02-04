#include <SFML/Graphics.hpp>
#include <string>

class BoxButton
{
private:
    sf::RectangleShape m_shape;
	sf::Font 	       m_font;
    sf::Text           m_text;

public:
    // コンストラクタ：サイズ、位置、フォント、表示文字列
    BoxButton(sf::Vector2f arg_size, sf::Vector2f arg_pos, const sf::Font& arg_font, const std::string& arg_label)
		:m_font(arg_font),
        m_text(arg_font, "")
    {
        // 1. ボタン本体の設定（サイズ、座標、色）
        m_shape.setSize(arg_size);
        m_shape.setOrigin(arg_size / 2.0f);
        m_shape.setPosition(arg_pos);
        m_shape.setFillColor(sf::Color(200, 200, 200));

        // 2. テキストの設定（文字化け対策込み）
        m_text.setString(sf::String::fromUtf8(arg_label.begin(),arg_label.end()));
        m_text.setCharacterSize(20);
        m_text.setFillColor(sf::Color::Black);

        // 3. テキスト描画（中央揃えの計算）
        // SFML 3 では getLocalBounds().position (x, y) のオフセット考慮が必須
        sf::FloatRect textRect = m_text.getLocalBounds();
        m_text.setOrigin({
            textRect.position.x + textRect.size.x / 2.0f,
            textRect.position.y + textRect.size.y / 2.0f
            });
        m_text.setPosition(arg_pos);
    }

    // 描画
    void Draw(sf::RenderWindow& window) const
    {
        window.draw(m_shape);
        window.draw(m_text);
    }

    // ホバーしているか
    bool IsHovered(sf::Vector2f mousePos) const
    {
        return m_shape.getGlobalBounds().contains(mousePos);
    }

    // クリックされたか
    bool IsClicked(sf::Vector2f mousePos, bool mousePressed) const
    {
        return mousePressed && IsHovered(mousePos);
    }

    // テキストの内容を後から変更したい場合
    void SetString(const std::string& label)
    {
        m_text.setString(label);
        sf::FloatRect textRect = m_text.getLocalBounds();
        m_text.setOrigin({ textRect.position.x + textRect.size.x / 2.0f, textRect.position.y + textRect.size.y / 2.0f });
    }
};