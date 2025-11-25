#pragma once
#include "SceneBase.h"
#include <SFML/Graphics.hpp>
#include <iostream>

class TitleScene : public SceneBase
{

	sf::Font font;
	sf::Text title;

public:

	TitleScene() = default;
	virtual ~TitleScene() = default;

	void Init() override;
	void Update() override;
	void Render() override;
	void End() override;


};