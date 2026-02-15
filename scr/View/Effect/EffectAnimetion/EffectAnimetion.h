#pragma once
#include "CSVLoad/EffectDataLoder/EffectDataLoder.h"
#include <SFML/Graphics.hpp>

class EffectAnimation
{
private:

    const EffectData& m_config;
    sf::Sprite m_sprite;
    float m_elapsedTime;
    int m_currentFrame;
    int m_frameWidth;
    int m_frameHeight;
    bool m_isPlaying;

public:

    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name="config"></param>
    /// <param name="texture"></param>
    EffectAnimation(const EffectData& config, const sf::Texture& texture);

    /// <summary>
	/// 指定座標でエフェクトを再生
    /// </summary>
    /// <param name="position"></param>
    /// <param name="scale"></param>
    void Play(sf::Vector2f position, sf::Vector2f scale = { 1.0f, 1.0f });

    /// <summary>
	/// エフェクト再生の更新
    /// </summary>
    /// <param name="deltaTime"></param>
    void Update(float deltaTime);


    /// <summary>
    /// 描画
    /// </summary>
    /// <param name="window"></param>
    void Draw(sf::RenderWindow& window);


    /// <summary>
    /// エフェクトが再生中か
    /// </summary>
    /// <returns></returns>
    bool IsPlaying() const;

private:

    /// <summary>
    /// テクスチャの表示領域の更新
    /// </summary>
    void UpdateTextureRect();

};

