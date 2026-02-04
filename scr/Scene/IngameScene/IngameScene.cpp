#include "IngameScene.h"
#include "Scene/SceneManager/SceneManager.h"
#include "Scene/PartyBuildScene/PartyBuildScene.h"
#include "Scene/StageBuildScene/StageBulidScene.h"


void IngameScene::Init(sf::RenderWindow& arg_window)
{
	//battle->Init(arg_window);
}

void IngameScene::handleEvent(const sf::Event& event)
{

}

void IngameScene::Update(sf::RenderWindow& arg_window)
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


	m_battle->Update(arg_window);
}

void IngameScene::Render(sf::RenderWindow& arg_window)
{	
	m_battle->Render(arg_window);
}

void IngameScene::End()
{
}
