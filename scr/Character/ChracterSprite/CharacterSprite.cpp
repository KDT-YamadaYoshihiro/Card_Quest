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

    // ===== 状態ごとの Rect 定義 =====
    // ※ 数値は「左上 x, y, 横, 縦」
    // 実際の画像に合わせて調整してOK

    m_rectTable[CharacterAnimState::WAIT] = sf::IntRect(sf::Vector2i{ 0, 0 }, sf::Vector2i{ 165, 150 });
    m_rectTable[CharacterAnimState::ATTACK] = sf::IntRect(sf::Vector2i{ 495, 0 }, sf::Vector2i{ 165, 150 });
    m_rectTable[CharacterAnimState::MAGIC] = sf::IntRect(sf::Vector2i{ 495, 450 }, sf::Vector2i{ 165, 150 });
    m_rectTable[CharacterAnimState::DAMAGE] = sf::IntRect(sf::Vector2i{ 0, 600 }, sf::Vector2i{ 165, 150 });
    m_rectTable[CharacterAnimState::DEAD] = sf::IntRect(sf::Vector2i{ 990, 450 }, sf::Vector2i{ 165, 150 });
    m_rectTable[CharacterAnimState::WIN] = sf::IntRect(sf::Vector2i{990,300}, sf::Vector2i{ 165, 150 });

    SetState(CharacterAnimState::WAIT);
}

/// <summary>
/// 座標設定
/// </summary>
/// <param name="arg_pos">設定座標</param>
void CharacterSprite::SetPosition(const sf::Vector2f& arg_pos)
{
    m_sprite.setPosition(arg_pos);
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
void CharacterSprite::Draw(RenderSystem& arg_render)
{
    arg_render.Draw(m_sprite);
}
