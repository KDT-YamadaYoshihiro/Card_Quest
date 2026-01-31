#include "CharacterSprite.h"


CharacterSprite::CharacterSprite(const std::string& arg_textureKey)
    :m_sprite(*TextureLoader::GetInstance().GetTextureID(arg_textureKey))
{
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="arg_textureKey">テクスチャキー</param>
void CharacterSprite::Init(const std::string& arg_textureKey)
{

    m_textureKey = arg_textureKey;

    auto tex = TextureLoader::GetInstance().GetTextureID(arg_textureKey);
    if (!tex) return;

    m_sprite.setTexture(*tex);

    const sf::Vector2u texSize = tex->getSize();

    constexpr int DIV_X = 9;
    constexpr int DIV_Y = 6;

    m_cellSize.x = texSize.x / DIV_X;
    m_cellSize.y = texSize.y / DIV_Y;

    const int W = m_cellSize.x;
    const int H = m_cellSize.y;

    // ===== 状態ごとの Rect 定義 =====
    // ※ 数値は「左上 x, y, 横, 縦」
    // 実際の画像に合わせて調整してOK

    m_rectTable[CharacterAnimState::WAIT] = sf::IntRect(sf::Vector2i{ W*0, H*0 }, sf::Vector2i{ W, H });
    m_rectTable[CharacterAnimState::ATTACK] = sf::IntRect(sf::Vector2i{ W*3, H*0 }, sf::Vector2i{ W, H });
    m_rectTable[CharacterAnimState::MAGIC] = sf::IntRect(sf::Vector2i{ W*3, H*3 }, sf::Vector2i{ W, H });
    m_rectTable[CharacterAnimState::DAMAGE] = sf::IntRect(sf::Vector2i{ W*0, H*4 }, sf::Vector2i{ W, H });
    m_rectTable[CharacterAnimState::DEAD] = sf::IntRect(sf::Vector2i{ W*6, H*3 }, sf::Vector2i{ W, H });
    m_rectTable[CharacterAnimState::WIN] = sf::IntRect(sf::Vector2i{ W*6,H*2}, sf::Vector2i{ W, H });

    SetState(CharacterAnimState::WAIT);
}

/// <summary>
/// 座標設定
/// </summary>
/// <param name="arg_pos">設定座標</param>
void CharacterSprite::SetPosition(const sf::Vector2f& arg_pos)
{
    m_sprite.setPosition(arg_pos);
    m_fill.setPosition({ arg_pos.x + 10.0f, arg_pos.y - 5.0f });
    m_frame.setPosition({ arg_pos.x + 10.0f, arg_pos.y - 5.0f});
}

/// <summary>
/// 水平方向の画像反転
/// </summary>
void CharacterSprite::SetSpriteWidthMirror()
{
    m_sprite.setOrigin({ m_sprite.getLocalBounds().size.x, 0.0f });
    m_sprite.setScale(sf::Vector2f(-1.0f, 1.0f));
}

/// <summary>
/// 垂直方向の画像反転
/// </summary>
void CharacterSprite::SetSpriteHeightMirror()
{
    m_sprite.setOrigin({ m_sprite.getLocalBounds().size.y, 0.0f });
    m_sprite.setScale(sf::Vector2f(1.0f, -1.0f));
}

/// <summary>
/// アニメーションステート設定
/// </summary>
/// <param name="arg_state">設定アニメーションステート</param>
void CharacterSprite::SetState(CharacterAnimState arg_state)
{

    m_currentState = arg_state;

    auto it = m_rectTable.find(arg_state);
    if (it != m_rectTable.end())
    {
        m_sprite.setTextureRect(it->second);
    }
}

/// <summary>
/// 描画
/// </summary>
/// <param name="arg_render">レンダーシステム</param>
void CharacterSprite::Draw(RenderSystem& arg_render, const CharacterData& arg_data, bool hpDrawFlag)
{
    arg_render.Draw(m_sprite);
    if (hpDrawFlag)
    {
        // HPの描画
        // --- 塗りつぶしゲージ ---
        float ratio = static_cast<float>(arg_data.hp) / arg_data.maxHp;
        m_fill.setSize({ 100.0f * ratio, 10.0f });
        m_fill.setFillColor(sf::Color::Green);

        // --- 枠線 ---
        m_frame.setSize({ 100.0f, 10.0f });
        m_frame.setFillColor(sf::Color::Transparent);
        m_frame.setOutlineColor(sf::Color::White);
        m_frame.setOutlineThickness(2.0f);

        arg_render.Draw(m_fill);
        arg_render.Draw(m_frame);

    }
}
