#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "../SceneBase.h"
#include "../../Battle/Battle.h"
#include "../../Character/Character.h"
#include "../../Character/Factory/ChracterFactory.h"
#include "../../Battle/Card/Card.h"

class IngameScene : public SceneBase
{

	std::shared_ptr<Battle> battle;
	std::vector<std::shared_ptr<Character>> m_players;
	std::vector<std::shared_ptr<Character>> m_enemies;
	std::vector<Card> m_card;

public:

	IngameScene()
	{
		battle = std::make_shared<Battle>();

		for (int i = 1; i < 5; i++) {
			m_players.push_back(CharacterFactory::Instance().CreateCharacter(i));
		}

	};
	virtual ~IngameScene() = default;

	void Init() override;
	void handleEvent() override;
	void Update() override;
	void Render(sf::RenderWindow& window) override;
	void End() override;

};