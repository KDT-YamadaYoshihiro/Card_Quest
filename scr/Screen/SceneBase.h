#pragma once
#include <SFML/Graphics.hpp>

class SceneBase
{

public:

	SceneBase() = default;
	virtual ~SceneBase() = default;

	virtual void Init() = 0;
	virtual void handleEvent() = 0;
	virtual void Update() = 0;
	virtual void Render(sf::RenderWindow& window) = 0;
	virtual void End() = 0;

};