#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Scene/SceneBase.h"
#include "System/Battle/BattleSystem.h"

class IngameScene : public SceneBase
{

	std::shared_ptr<BattleSystem> m_battle;

public:

	/// <summary>
	/// èâä˙âª
	/// </summary>
	/// <param name="arg_window"></param>
	IngameScene();
	virtual ~IngameScene() = default;


	bool Init(sf::RenderWindow& arg_window) override;
	void handleEvent(const sf::Event& event) override;
	void Update(sf::RenderWindow& arg_window, float dt) override;
	void Render(sf::RenderWindow& arg_window) override;
	void End() override;

};