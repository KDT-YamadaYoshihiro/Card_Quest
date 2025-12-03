#pragma once
#include <SFML/Graphics.hpp>
#include "SceneBase.h"


class ResultScene : public SceneBase
{

public:

	ResultScene() = default;
	virtual ~ResultScene() = default;

	void Init() override;
	void handleEvent() override;
	void Update() override;
	void Render(sf::RenderWindow& window) override;
	void End() override;

};