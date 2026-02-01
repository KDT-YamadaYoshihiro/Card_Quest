#pragma once
#include <SFML/Graphics.hpp>
#include "View/Render/RenderSystem.h"
#include "CSVLoad/TextureLoader/TextureLoader.h"
#include "View/ConsoleView/ConsoleView.h"

class SceneBase
{

protected:


public:

	SceneBase() {};
	virtual ~SceneBase() = default;

	virtual void Init(sf::RenderWindow& arg_window) = 0;
	virtual void handleEvent(const sf::Event& event) = 0;
	virtual void Update(sf::RenderWindow& arg_window) = 0;
	virtual void Render(sf::RenderWindow& arg_window) = 0;
	virtual void End() = 0;

};