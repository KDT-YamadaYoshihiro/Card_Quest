#include "CharacterSprite.h"
#include "System/Constants.h"


CharacterSprite::CharacterSprite(const std::string& arg_textureKey)
    :m_sprite(*TextureLoader::GetInstance().GetTextureID(arg_textureKey))
{
}

/// <summary>
/// ������
/// </summary>
/// <param name="arg_textureKey">�e�N�X�`���L�[</param>
void CharacterSprite::Init(const std::string& arg_textureKey)
{

    m_textureKey = arg_textureKey;

    auto tex = TextureLoader::GetInstance().GetTextureID(arg_textureKey);
    if (!tex) return;

    m_sprite.setTexture(*tex);

    const sf::Vector2u texSize = tex->getSize();

    constexpr int DIV_X = Constants::DIV_X;
    constexpr int DIV_Y = Constants::DIV_Y;

    m_cellSize.x = texSize.x / DIV_X;
    m_cellSize.y = texSize.y / DIV_Y;

    const int W = m_cellSize.x;
    const int H = m_cellSize.y;

    // ===== ��Ԃ��Ƃ� Rect ��` =====
    // �� ���l�́u���� x, y, ��, �c�v
    // ���ۂ̉摜�ɍ��킹�Ē�������OK

    m_rectTable[CharacterAnimState::WAIT] = sf::IntRect(sf::Vector2i{ W*0, H*0 }, sf::Vector2i{ W, H });
    m_rectTable[CharacterAnimState::ATTACK] = sf::IntRect(sf::Vector2i{ W*3, H*0 }, sf::Vector2i{ W, H });
    m_rectTable[CharacterAnimState::MAGIC] = sf::IntRect(sf::Vector2i{ W*3, H*3 }, sf::Vector2i{ W, H });
    m_rectTable[CharacterAnimState::DAMAGE] = sf::IntRect(sf::Vector2i{ W*0, H*4 }, sf::Vector2i{ W, H });
    m_rectTable[CharacterAnimState::DEAD] = sf::IntRect(sf::Vector2i{ W*6, H*3 }, sf::Vector2i{ W, H });
    m_rectTable[CharacterAnimState::WIN] = sf::IntRect(sf::Vector2i{ W*6,H*1}, sf::Vector2i{ W, H });

    SetState(CharacterAnimState::WAIT);
}

/// <summary>
/// ���W�ݒ�
/// </summary>
/// <param name="arg_pos">�ݒ���W</param>
void CharacterSprite::SetPosition(const sf::Vector2f& arg_pos)
{
    m_sprite.setPosition(arg_pos);
    m_fill.setPosition({ arg_pos.x + 10.0f, arg_pos.y - 5.0f });
    m_frame.setPosition({ arg_pos.x + 10.0f, arg_pos.y - 5.0f});
}

/// <summary>
/// ���������̉摜���]
/// </summary>
void CharacterSprite::SetSpriteWidthMirror()
{
    m_sprite.setOrigin({ m_sprite.getLocalBounds().size.x, 0.0f });
    m_sprite.setScale(sf::Vector2f(-1.0f, 1.0f));
}

/// <summary>
/// ���������̉摜���]
/// </summary>
void CharacterSprite::SetSpriteHeightMirror()
{
    m_sprite.setOrigin({ m_sprite.getLocalBounds().size.y, 0.0f });
    m_sprite.setScale(sf::Vector2f(1.0f, -1.0f));
}

/// <summary>
/// �A�j���[�V�����X�e�[�g�ݒ�
/// </summary>
/// <param name="arg_state">�ݒ�A�j���[�V�����X�e�[�g</param>
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
/// �`��
/// </summary>
/// <param name="arg_render">�����_�[�V�X�e��</param>
void CharacterSprite::Draw(RenderSystem& arg_render, const CharacterData& arg_data, bool hpDrawFlag)
{
    arg_render.Draw(m_sprite);
    if (hpDrawFlag)
    {
        // HP�̕`��
        // --- �h��Ԃ��Q�[�W ---
        float ratio = static_cast<float>(arg_data.hp) / arg_data.maxHp;
        m_fill.setSize({ 100.0f * ratio, 10.0f });
        m_fill.setFillColor(sf::Color::Green);

        // --- �g�� ---
        m_frame.setSize({ 100.0f, 10.0f });
        m_frame.setFillColor(sf::Color::Transparent);
        m_frame.setOutlineColor(sf::Color::White);
        m_frame.setOutlineThickness(2.0f);

        arg_render.Draw(m_fill);
        arg_render.Draw(m_frame);

    }
}
