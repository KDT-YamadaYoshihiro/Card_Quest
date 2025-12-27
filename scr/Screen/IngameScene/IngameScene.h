#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "../SceneBase.h"
#include "../../Battle/BattleSystem.h"

class IngameScene : public SceneBase
{

	std::shared_ptr<BattleSystem> battle;

public:

	IngameScene()
	{
		std::cout << "Ingame" << std::endl;
		battle = std::make_shared<BattleSystem>();
	};
	virtual ~IngameScene() = default;


	void Init() override;
	void handleEvent() override;
	void Update(sf::RenderWindow& arg_window) override;
	void Render(sf::RenderWindow& arg_window) override;
	void End() override;

};