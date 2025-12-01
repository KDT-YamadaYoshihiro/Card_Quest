#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "CameraManager.h"

class RenderManager
{

	sf::RenderWindow& m_window;

	std::vector<std::weak_ptr<sf::Drawable>> m_world;
	std::vector<std::weak_ptr<sf::Drawable>> m_ui;

public:

	RenderManager(sf::RenderWindow& arg_window)
		:m_window(arg_window)
	{
	}

	void Begin(float arg_dt);

	void DrawWorld(std::shared_ptr<sf::Drawable> arg_drawble);
	void DrawUi(std::shared_ptr<sf::Drawable> arg_drawble);
	void Render();

};