#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Scene/SceneBase.h"


class TitleScene : public SceneBase
{


public:

	TitleScene();

	virtual ~TitleScene() = default;

	bool Init(sf::RenderWindow& arg_window) override;
	void handleEvent(const sf::Event& event) override;
	void Update(sf::RenderWindow& arg_window) override;
	void Render(sf::RenderWindow& arg_window) override;
	void End() override;


};