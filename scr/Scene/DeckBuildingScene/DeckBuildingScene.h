#pragma once
#include <SFML/Graphics.hpp>
#include "Scene/SceneBase.h"
#include "System/DeckBulid/DeckBuildSystem/DeckBuildSystem.h"

class BoxButton;

class DeckBuildingScene : public SceneBase
{

	// デッキ編成システム
	DeckBuildSystem m_deckBuildSystem;

	// デッキ完成ボタン
	std::unique_ptr<BoxButton> m_nextButton;
	std::unique_ptr<BoxButton> m_backButton;
	
public:

	// 初期化
	DeckBuildingScene();
	virtual ~DeckBuildingScene() = default;

	// シーン基本関数
	virtual void Init(sf::RenderWindow& arg_window) override;
	virtual void handleEvent(const sf::Event& event) override;
	virtual void Update(sf::RenderWindow& arg_window) override;
	virtual void Render(sf::RenderWindow& arg_window) override;
	virtual void End() override;
};

