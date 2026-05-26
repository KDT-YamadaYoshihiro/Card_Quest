#include "EffectAnimation.h"
#include "GameMain/WindowSetting.h"

EffectAnimation::EffectAnimation(const EffectData& config, const sf::Texture& texture)
    : m_config(config), m_sprite(texture), m_isPlaying(false), m_currentFrame(0), m_elapsedTime(0.0f)
{
    // �摜�T�C�Y����1�t���[��������̃T�C�Y���v�Z (Rect�T�C�Y�v�Z)
    sf::Vector2u texSize = texture.getSize();
    m_frameWidth = texSize.x / m_config.xDivision;
    m_frameHeight = texSize.y / m_config.yDivision;

    m_sprite.setOrigin({m_frameWidth / 2.0f, m_frameHeight / 2.0f});
    // �����t���[���̐ݒ�
    UpdateTextureRect();
}

void EffectAnimation::Play(sf::Vector2f position, sf::Vector2f scale)
{

    m_sprite.setPosition(position);
    m_sprite.setScale(scale);
    m_isPlaying = true;
    m_currentFrame = 0;
    m_elapsedTime = 0.0f;

}


void EffectAnimation::Update(float deltaTime)
{
	// �Đ����łȂ���΍X�V���Ȃ�
    if (!m_isPlaying)
    {
        return;
    }

	// �o�ߎ��Ԃ��X�V
    m_elapsedTime += deltaTime;

    if (m_elapsedTime >= m_config.frameDuration) {
		//�@�t���[����i�߂�
        m_elapsedTime = 0.0f;
        m_currentFrame++;

        if (m_currentFrame >= m_config.xDivision * m_config.yDivision) {
            m_isPlaying = false;
        }
        else {
            UpdateTextureRect();
        }
    }
}

void EffectAnimation::Draw(sf::RenderWindow& window) {
    if (m_isPlaying) {
        window.draw(m_sprite);
    }
}

bool EffectAnimation::IsPlaying() const
{
    return m_isPlaying;
}

void EffectAnimation::UpdateTextureRect()
{
    // ���݂̃t���[���Ɋ�Â��ăe�N�X�`���̕\���̈���v�Z
    int tx = (m_currentFrame % m_config.xDivision) * m_frameWidth;
    int ty = (m_currentFrame / m_config.xDivision) * m_frameHeight;

    // �e�N�X�`���̕\���̈���X�V
    m_sprite.setTextureRect(sf::IntRect({ tx, ty }, { m_frameWidth, m_frameHeight }));
}
