#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "SceneBase.h"

class TitleScene : public SceneBase
{

	//sf::Font font;
	//sf::Text title;

public:

	TitleScene() = default;
	virtual ~TitleScene() = default;

	void Init() override;
	void handleEvent(const sf::Event& event) override;
	void Update() override;
	void Render() override;
	void End() override;


};