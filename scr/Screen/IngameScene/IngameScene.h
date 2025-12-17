#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "../SceneBase.h"
#include "../../Battle/Battle.h"

class IngameScene : public SceneBase
{

	std::shared_ptr<Battle> battle;

public:

	IngameScene()
	{
		battle = std::make_shared<Battle>();
	};
	virtual ~IngameScene() = default;


	void Init() override;
	void handleEvent() override;
	void Update() override;
	void Render(sf::RenderWindow& window) override;
	void End() override;

};