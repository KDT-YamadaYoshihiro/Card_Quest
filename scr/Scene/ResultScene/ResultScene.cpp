#include "ResultScene.h"

void ResultScene::Init(sf::RenderWindow& arg_window)
{
}

void ResultScene::handleEvent(const sf::Event& event)
{
}

void ResultScene::Update(sf::RenderWindow& arg_window)
{
}

void ResultScene::Render(sf::RenderWindow& arg_window)
{
    // îwåi
    auto tex = TextureLoader::GetInstance().GetTextureID("bg");
    if (tex)
    {
        sf::Sprite sprite(*tex);
        sprite.setPosition({ 0.0f,0.0f });
        sprite.setScale({ 0.7f,0.7f });
        arg_window.draw(sprite);
    }

}

void ResultScene::End()
{
}
