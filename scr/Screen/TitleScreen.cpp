#include "TitleScene.h"
#include "../Screen/SceneManager.h"
#include "IngameScene.h"
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
        SceneManager::Instance().ChangeScreen<IngameScene>();
    }
    


}

void TitleScene::Update()
{
	
}

void TitleScene::Render(sf::RenderWindow& window)
{
    sf::Sprite sprite(test);
    sprite.setPosition({ 10.0f, 50.0f }); 
    window.draw(sprite);
}

void TitleScene::End()
{
}
