#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Scene/SceneBase.h"


class ResultScene : public SceneBase
{

public:

	ResultScene():SceneBase()
	{
		ConsoleView::GetInstance().Add("ResultScene\n");
	}
	virtual ~ResultScene() = default;

	void Init(sf::RenderWindow& arg_window) override;
	void handleEvent(const sf::Event& event) override;
	void Update(sf::RenderWindow& arg_window) override;
	void Render(sf::RenderWindow& arg_window) override;
	void End() override;

};