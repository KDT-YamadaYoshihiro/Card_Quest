#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Scene/SceneBase.h"
#include "System/Battle/BattleSystem.h"

class IngameScene : public SceneBase
{

	std::shared_ptr<BattleSystem> m_battle;

public:

	IngameScene(sf::RenderWindow& arg_window):SceneBase()
	{
		ConsoleView::GetInstance().Add("IngameScene\n");
		m_battle = std::make_shared<BattleSystem>(arg_window);
	};
	virtual ~IngameScene() = default;


	void Init(sf::RenderWindow& arg_window) override;
	void handleEvent(const sf::Event& event) override;
	void Update(sf::RenderWindow& arg_window) override;
	void Render(sf::RenderWindow& arg_window) override;
	void End() override;

};