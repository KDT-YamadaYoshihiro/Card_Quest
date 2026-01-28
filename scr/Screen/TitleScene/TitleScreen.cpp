#include "TitleScene.h"
#include "Screen/SceneManager.h"
#include "Screen/PartyBuildScene/PartyBulidScene.h"
#include <SFML/Graphics.hpp>


void TitleScene::Init(sf::RenderWindow& arg_window)
{
}

void TitleScene::handleEvent(const sf::Event& event)
{
}

void TitleScene::Update(sf::RenderWindow& arg_window)
{
    // キーボード判定
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
    {
        SceneManager::GetInstance().ChangeScreen<PartyBuildScene>(arg_window);
    }

}

void TitleScene::Render(sf::RenderWindow& window)
{
    auto tex = TextureLoader::GetInstance().GetTextureID("test");

	if (tex)
	{
		sf::Sprite sprite(*tex);
        sprite.setPosition({ 0.0f,0.0f });
        window.draw(sprite);
	}
}

void TitleScene::End()
{
}
