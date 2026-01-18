#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "../SceneBase.h"
#include "../../Battle/BattleSystem.h"

class IngameScene : public SceneBase
{

	//std::shared_ptr<BattleSystem> battle;

public:

	IngameScene(sf::RenderWindow& arg_window):SceneBase(arg_window)
	{
		std::cout << "Ingame" << std::endl;
		//battle = std::make_shared<BattleSystem>(arg_window);
	};
	virtual ~IngameScene() = default;


	void Init(sf::RenderWindow& arg_window) override;
	void handleEvent(const sf::Event& event) override;
	void Update(sf::RenderWindow& arg_window) override;
	void Render(sf::RenderWindow& arg_window) override;
	void End() override;

};