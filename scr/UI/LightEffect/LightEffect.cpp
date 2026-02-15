#include "LightEffect.h"
#include "EffectAnimetion.h"

LightEffect::LightEffect(size_t count, sf::Vector2u windowSize)
    : m_windowSize(windowSize), m_gen(std::random_device{}()) {

    m_lights.resize(count);
    for (auto& light : m_lights) {
        light.shape.setRadius(2.0f + getRandom(0.0f, 3.0f));
        light.shape.setFillColor(sf::Color::White);
        resetLight(light, true); // 最初は画面内にランダム配置
    }
}

void LightEffect::resetLight(Light& light, bool randomY) {
    light.xOffset = getRandom(0.0f, static_cast<float>(m_windowSize.x));
    float yPos = randomY ? getRandom(0.0f, static_cast<float>(m_windowSize.y))
        : static_cast<float>(m_windowSize.y) + 20.0f;

    light.shape.setPosition({ light.xOffset, yPos });
    light.speed = getRandom(30.0f, 80.0f);
    light.amplitude = getRandom(10.0f, 30.0f);
    light.phase = getRandom(0.0f, 6.28f); // 0 ~ 2PI
}

float LightEffect::getRandom(float min, float max) {
    std::uniform_real_distribution<float> dist(min, max);
    return dist(m_gen);
}

void LightEffect::Update(float deltaTime) {
    for (auto& light : m_lights) {
        // 上へ移動
        sf::Vector2f pos = light.shape.getPosition();
        pos.y -= light.speed * deltaTime;

        // 左右の揺れ (sin関数)
        light.phase += deltaTime * 2.0f;
        pos.x = light.xOffset + std::sin(light.phase) * light.amplitude;

        // 透明度の更新 (sin関数で0~255を往復)
        int alpha = static_cast<int>((std::sin(light.phase) * 0.5f + 0.5f) * 200);
        light.shape.setFillColor(sf::Color(255, 255, 255, alpha));

        light.shape.setPosition(pos);

        // 画面外（上）に出たらリセット
        if (pos.y < -20.0f) {
            resetLight(light, false);
        }
    }
}


void LightEffect::Draw(sf::RenderWindow& window) {
    for (const auto& light : m_lights) {
        window.draw(light.shape);
    }
}

