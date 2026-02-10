#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Scene/SceneBase.h"


class TitleScene : public SceneBase
{
private:

	// タイトルテキストの透明度
	float m_titleAlpha = 255.0f;
	// 透明度の変化速度
	float m_alphaChangeSpeed = 200.0f;

public:

	TitleScene();

	virtual ~TitleScene() = default;

	bool Init(sf::RenderWindow& arg_window) override;
	void handleEvent(const sf::Event& event) override;
	void Update(sf::RenderWindow& arg_window, float dt) override;
	void Render(sf::RenderWindow& arg_window) override;
	void End() override;


private:

	// 透明度の更新
	void UpdateAlpha(float dt);

};