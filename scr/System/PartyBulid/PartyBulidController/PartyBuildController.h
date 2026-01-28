#pragma once
#include "System/PartyBulid/PartyBuildContext/PartyBuildContext.h"
#include "View/PartyBulidView/PartyBuildView.h"


class PartyBuildController
{
private:

	PartyBuildContext& m_context;
	PartyBuildView& m_view;

public:

	PartyBuildController(PartyBuildContext& context, PartyBuildView& view);

	void Update(sf::RenderWindow& window);

private:

	sf::Vector2f GetMousePos(sf::RenderWindow& window);

	std::shared_ptr<Character> GetCharacterByIconIndex(size_t index);
};

