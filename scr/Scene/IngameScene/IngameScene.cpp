#include "IngameScene.h"
#include "Scene/SceneManager/SceneManager.h"
#include "Scene/PartyBuildScene/PartyBuildScene.h"
#include "Scene/StageBuildScene/StageBulidScene.h"
#include "System/InPutManager/InPutMouseManager.h"

IngameScene::IngameScene()
	:SceneBase()
{
}

bool IngameScene::Init(sf::RenderWindow& arg_window)
{
	ConsoleView::GetInstance().Add("IngameScene\n");
	m_battle = std::make_shared<BattleSystem>(arg_window);
	if (!m_battle)
	{
		ConsoleView::GetInstance().Add("IngameScene/m_battle:nullptr\n");
		return false; 
	}

	if (m_battle->Init(arg_window))
	{
		ConsoleView::GetInstance().Add("IngameScene/m_battle->Init():true\n");
		return true;
	}
	else
	{
		ConsoleView::GetInstance().Add("IngameScene/m_battle->Init():false\n");
		return false;
	}

}

void IngameScene::handleEvent(const sf::Event& event)
{

}

void IngameScene::Update(sf::RenderWindow& arg_window, float dt)
{

	InPutMouseManager::GetInstance().Update(arg_window);

	if (m_battle->IsToPartyScene())
	{
		CardManager::GetInstance().RecycleCemeteryToDeck();
		SceneManager::GetInstance().ChangeScreen<PartyBuildScene>(arg_window);
		return;
	}

	if (m_battle->IsToStageSelectScene())
	{
		CardManager::GetInstance().RecycleCemeteryToDeck();
		SceneManager::GetInstance().ChangeScreen<StageBulidScene>(arg_window);
		return;
	}

	// ライトエフェクト更新
	m_lightEffect->Update(dt);

	// バトル更新
	m_battle->Update(arg_window);
}

void IngameScene::Render(sf::RenderWindow& arg_window)
{
	// 背景
	auto tex = TextureLoader::GetInstance().GetTextureID("bg");
	if (tex)
	{
		sf::Sprite sprite(*tex);
		sprite.setPosition({ -300.0f,-250.0f });
		sprite.setScale({ 0.9f, 0.9f });
		arg_window.draw(sprite);
	}

	// ライトエフェクト描画
	m_lightEffect->Draw(arg_window);
	// バトル描画
	m_battle->Render(arg_window);
}

void IngameScene::End()
{
}
