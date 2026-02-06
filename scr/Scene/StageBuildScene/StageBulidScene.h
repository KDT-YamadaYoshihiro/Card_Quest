#pragma once
#include <iostream>
#include "Scene/SceneBase.h"
#include "System/StageBulid/StageBulidContext/StageBulidContext.h"
#include "System/StageBulid/StageBulidController/StageBulidController.h"
#include "View/StageBulidView/StageBuildView.h"
#include "View/Render/RenderSystem.h"

class TriangleButton;
class BoxButton;

class StageBulidScene : public SceneBase
{
private:

	std::unique_ptr<RenderSystem> m_render;

	StageBulidContext m_context;
	std::unique_ptr<StageBulidController> m_controller;
	std::unique_ptr<StageBuildView> m_view;

	std::unique_ptr<TriangleButton> m_leftArrow;
	std::unique_ptr<TriangleButton> m_rightArrow;
	std::unique_ptr<BoxButton> m_backButton;
	std::unique_ptr<BoxButton> m_nextButton;

public:

	StageBulidScene();
	virtual ~StageBulidScene() = default;

	bool Init(sf::RenderWindow& arg_window) override;
	void handleEvent(const sf::Event& event) override;
	void Update(sf::RenderWindow& arg_window) override;
	void Render(sf::RenderWindow& arg_window) override;
	void End() override;

	/// <summary>
	/// BattleContext‚Ö‚ÌŽó‚¯“n‚µ
	/// </summary>
	void DecideStage(sf::RenderWindow& arg_window);


};

