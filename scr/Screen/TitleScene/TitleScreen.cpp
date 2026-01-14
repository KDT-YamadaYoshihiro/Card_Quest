#include "TitleScene.h"
#include "../../Screen/SceneManager.h"
#include "../IngameScene/IngameScene.h"
#include <SFML/Graphics.hpp>


void TitleScene::Init(sf::RenderWindow& arg_window)
{
    std::cout << "Title" << "\n";
}

void TitleScene::handleEvent()
{
}

void TitleScene::Update(sf::RenderWindow& arg_window)
{
    // キーボード判定
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
    {
        SceneManager::GetInstance().ChangeScreen<IngameScene>(arg_window);
    }

}

void TitleScene::Render(sf::RenderWindow& window)
{
    auto tex = TextureLoader::GetInstance().GetTextureID("test");

	if (tex)
	{
		sf::Sprite sprite(*tex);
        sprite.setPosition({ 10.0f, 50.0f });
        window.draw(sprite);
	}
}

void TitleScene::End()
{
}
