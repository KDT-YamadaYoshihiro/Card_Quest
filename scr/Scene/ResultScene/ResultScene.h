#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Scene/SceneBase.h"


class ResultScene : public SceneBase
{

public:

	ResultScene();
	virtual ~ResultScene() = default;

	bool Init(sf::RenderWindow& arg_window) override;
	void handleEvent(const sf::Event& event) override;
	void Update(sf::RenderWindow& arg_window, float dt) override;
	void Render(sf::RenderWindow& arg_window) override;
	void End() override;

};