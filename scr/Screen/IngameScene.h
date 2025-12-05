#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "SceneBase.h"

class IngameScene : public SceneBase
{

public:

	IngameScene() = default;
	virtual ~IngameScene() = default;

	void Init() override;
	void handleEvent() override;
	void Update() override;
	void Render(sf::RenderWindow& window) override;
	void End() override;

};