#include "EffectAnimetion.h"

EffectAnimation::EffectAnimation(const EffectData& config, const sf::Texture& texture)
	: m_config(config), m_sprite(texture), m_isPlaying(false), m_currentFrame(0), m_elapsedTime(sf::Time::Zero)
{
    // 画像サイズから1フレームあたりのサイズを計算 (Rectサイズ計算)
    sf::Vector2u texSize = texture.getSize();
    m_frameWidth = texSize.x / m_config.xDivision;
    m_frameHeight = texSize.y / m_config.yDivision;

    // 初期フレームの設定
    UpdateTextureRect();
}

void EffectAnimation::Play(sf::Vector2f position, sf::Vector2f scale)
{

    m_sprite.setPosition(position);
    m_sprite.setScale(scale);
    m_isPlaying = true;
    m_currentFrame = 0;
    m_elapsedTime = sf::Time::Zero;

}

void EffectAnimation::Update(sf::Time deltaTime)
{
	// 再生中でなければ更新しない
    if (!m_isPlaying)
    {
        return;
    }

	// 経過時間を更新
    m_elapsedTime += deltaTime;

    if (m_elapsedTime.asSeconds() >= m_config.frameDuration) {
		// フレーム切り替え
        m_elapsedTime = sf::Time::Zero;
        m_currentFrame++;

        // 全フレーム再生終了判定
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
	// 現在のフレームに基づいてテクスチャの表示領域を計算
    int tx = (m_currentFrame % m_config.xDivision) * m_frameWidth;
    int ty = (m_currentFrame / m_config.yDivision) * m_frameHeight;
	// テクスチャの表示領域を更新
    m_sprite.setTextureRect(sf::IntRect({ tx, ty }, { m_frameWidth, m_frameHeight }));
}
