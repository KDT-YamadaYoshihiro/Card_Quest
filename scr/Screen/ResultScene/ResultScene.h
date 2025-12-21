#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "../SceneBase.h"


class ResultScene : public SceneBase
{

public:

	ResultScene()
	{
		std::cout << "Result" << std::endl;
	}
	virtual ~ResultScene() = default;

	void Init() override;
	void handleEvent() override;
	void Update() override;
	void Render(sf::RenderWindow& window) override;
	void End() override;

};