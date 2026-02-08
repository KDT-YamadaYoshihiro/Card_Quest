#pragma once
#include "Scene/SceneBase.h"
#include "System/PartyBulid/PartyBuildContext/PartyBuildContext.h"
#include "System/PartyBulid/PartyBulidController/PartyBuildController.h"
#include "View/PartyBulidView/PartyBuildView.h"
#include "UI/BoxButton.h"
#include <iostream>

class PartyBuildScene : public SceneBase
{
private:

	PartyBuildContext m_context;
	std::unique_ptr<RenderSystem> m_render;
	std::unique_ptr<PartyBuildView> m_view;
	std::unique_ptr<PartyBuildController> m_controller;
	std::unique_ptr<BoxButton> m_nextButton;
	std::unique_ptr<BoxButton> m_backButton;

public:

	PartyBuildScene();
	virtual ~PartyBuildScene() = default;

	bool Init(sf::RenderWindow& arg_window) override;
	void handleEvent(const sf::Event& event) override;
	void Update(sf::RenderWindow& arg_window, float dt) override;
	void Render(sf::RenderWindow& arg_window) override;
	void End() override;

private:

	/// <summary>
	/// ÉfÉbÉLï“ê¨âÊñ Ç÷à⁄ìÆ
	/// </summary>
	/// <param name="arg_window"></param>
	void StartDeckBulid(sf::RenderWindow& arg_window);


};

