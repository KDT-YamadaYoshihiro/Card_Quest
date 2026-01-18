#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "../SceneBase.h"


class ResultScene : public SceneBase
{

public:

	ResultScene(sf::RenderWindow& arg_window):SceneBase(arg_window)
	{
		std::cout << "Result" << std::endl;
	}
	virtual ~ResultScene() = default;

	void Init(sf::RenderWindow& arg_window) override;
	void handleEvent(const sf::Event& event) override;
	void Update(sf::RenderWindow& arg_window) override;
	void Render(sf::RenderWindow& arg_window) override;
	void End() override;

};