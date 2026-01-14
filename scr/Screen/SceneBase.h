#pragma once
#include <SFML/Graphics.hpp>
#include "../View/Render/RenderSystem.h"
#include "../CSVLoad/TextureLoader/TextureLoader.h"

class SceneBase
{

protected:

	//std::shared_ptr<RenderSystem> renderSystem = std::make_shared<RenderSystem>();

public:

	SceneBase(sf::RenderWindow& arg_window) {};
	virtual ~SceneBase() = default;

	virtual void Init(sf::RenderWindow& arg_window) = 0;
	virtual void handleEvent() = 0;
	virtual void Update(sf::RenderWindow& arg_window) = 0;
	virtual void Render(sf::RenderWindow& arg_window) = 0;
	virtual void End() = 0;

};