#pragma once
#ifndef LIGHT_EFFECT_HPP
#define LIGHT_EFFECT_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <random>

class LightEffect {
public:
    LightEffect(size_t count, sf::Vector2u windowSize);
    void Update(float deltaTime);
    void Draw(sf::RenderWindow& window);

private:
    struct Light {
        sf::CircleShape shape;
        float speed;
        float amplitude; // ¶‰E‚É—h‚ê‚é•
        float phase;     // —h‚ê‚Æ“§–¾“x‚ÌˆÊ‘Š
        float xOffset;   // Šî€‚Æ‚È‚éXÀ•W
    };

    std::vector<Light> m_lights;
    sf::Vector2u m_windowSize;

    // —”¶¬Ší
    std::mt19937 m_gen;
    float getRandom(float min, float max);
    void resetLight(Light& light, bool randomY);
};

#endif