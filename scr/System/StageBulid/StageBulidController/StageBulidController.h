#pragma once
#include <SFML/Graphics.hpp>
#include "System/StageBulid/StageBulidContext/StageBulidContext.h"

class StageBulidController
{
private:

	StageBulidContext& m_context;

public:

	StageBulidController(StageBulidContext& context)
		:m_context(context) 
	{
	}

	void Update(sf::RenderWindow& window);

};

