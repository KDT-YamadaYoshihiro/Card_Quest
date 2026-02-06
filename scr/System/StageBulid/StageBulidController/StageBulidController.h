#pragma once
#include <SFML/Graphics.hpp>
#include "System/StageBulid/StageBulidContext/StageBulidContext.h"

class StageBulidController
{
private:

	StageBulidContext& m_context;

public:

	/// <summary>
	/// ‰Šú‰»
	/// </summary>
	/// <param name="context"></param>
	StageBulidController(StageBulidContext& context)
		:m_context(context) 
	{
	}

	/// <summary>
	/// XV
	/// </summary>
	/// <param name="window"></param>
	void Update(sf::RenderWindow& window);

};

