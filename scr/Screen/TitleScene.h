#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "SceneBase.h"

class TitleScene : public SceneBase
{

	sf::Texture test;

public:

	TitleScene()
	{
		if (!test.loadFromFile("data/Test/test.jpg", false, sf::IntRect({ 0, 0 }, { 210, 280 })))
		{
			// error...
			std::cout << "error" << "\n";
		}
	};


	virtual ~TitleScene() = default;

	void Init() override;
	void handleEvent() override;
	void Update() override;
	void Render(sf::RenderWindow& window) override;
	void End() override;


};