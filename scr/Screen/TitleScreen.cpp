#include "TitleScene.h"
#include "../Screen/SceneManager.h"
#include "IngameScene.h"
#include <SFML/Graphics.hpp>


void TitleScene::Init()
{

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
    sprite.setPosition({ 10.f, 50.f }); 
    window.draw(sprite);

    std::cout << "Title" << "\n";
}

void TitleScene::End()
{
}
