#include "TitleScene.h"
#include "../../Screen/SceneManager.h"
#include "../IngameScene/IngameScene.h"
#include <SFML/Graphics.hpp>


void TitleScene::Init()
{
    std::cout << "Title" << "\n";
}

void TitleScene::handleEvent()
{
	
    // キーボード判定
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
    {
        SceneManager::GetInstance().ChangeScreen<IngameScene>();
    }
    


}

void TitleScene::Update(sf::RenderWindow& arg_window)
{
	
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
