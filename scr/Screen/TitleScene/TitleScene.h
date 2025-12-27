#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "../SceneBase.h"


class TitleScene : public SceneBase
{

	sf::Texture test;

public:

	TitleScene()
	{
		if (!test.loadFromFile("data/Test/test.jpg"))
		{
			// error...
			std::cout << "error" << "\n";
		}
	};


	virtual ~TitleScene() = default;

	void Init() override;
	void handleEvent() override;
	void Update(sf::RenderWindow& arg_window) override;
	void Render(sf::RenderWindow& arg_window) override;
	void End() override;


};