#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "../SceneBase.h"


class TitleScene : public SceneBase
{


public:

	TitleScene():SceneBase()
	{
		std::cout << "Title" << std::endl;
	};


	virtual ~TitleScene() = default;

	void Init(sf::RenderWindow& arg_window) override;
	void handleEvent(const sf::Event& event) override;
	void Update(sf::RenderWindow& arg_window) override;
	void Render(sf::RenderWindow& arg_window) override;
	void End() override;


};