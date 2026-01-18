#pragma once
#include "../SceneBase.h"

class DeckBuildingScene : public SceneBase
{

	// デッキ編成システム


public:
	// 初期化
	DeckBuildingScene(sf::RenderWindow& arg_window);
	virtual ~DeckBuildingScene() = default;
	// シーン基本関数
	virtual void Init(sf::RenderWindow& arg_window) override;
	virtual void handleEvent() override;
	virtual void Update(sf::RenderWindow& arg_window) override;
	virtual void Render(sf::RenderWindow& arg_window) override;
	virtual void End() override;
};

