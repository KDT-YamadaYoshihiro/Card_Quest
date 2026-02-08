#pragma once
#include <SFML/Graphics.hpp>
#include "View/Render/RenderSystem.h"
#include "CSVLoad/TextureLoader/TextureLoader.h"
#include "View/ConsoleView/ConsoleView.h"
#include "System/InPutManager/InPutMouseManager.h"
#include "UI/LightEffect/LightEffect.h"
#include "GameMain/WindowSetting.h"

class SceneBase
{

protected:

	std::unique_ptr<LightEffect> m_lightEffect;

public:

	SceneBase() 
	{
		sf::Vector2u windowSize = {WindowSetting::GetInstance().GetWindowSize()};
		// ëSÇƒÇÃÉVÅ[ÉìÇ≈ã§í ÇµÇƒê∂
		m_lightEffect = std::make_unique<LightEffect>(50, windowSize);
	};
	virtual ~SceneBase() = default;

	virtual bool Init(sf::RenderWindow& arg_window) = 0;
	virtual void handleEvent(const sf::Event& event) = 0;
	virtual void Update(sf::RenderWindow& arg_window, float dt) = 0;
	virtual void Render(sf::RenderWindow& arg_window) = 0;
	virtual void End() = 0;

};