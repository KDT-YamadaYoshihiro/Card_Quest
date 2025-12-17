#include "IngameScene.h"
#include "../SceneManager.h"
#include "../ResultScene/ResultScene.h"


void IngameScene::Init()
{
	battle->Init();
}

void IngameScene::handleEvent()
{
	if(battle->CheckWin() || battle->CheckLose())
	{
		SceneManager::Instance().ChangeScreen<ResultScene>();
	}
}

void IngameScene::Update()
{
	battle->Update();
}

void IngameScene::Render(sf::RenderWindow& window)
{	
}

void IngameScene::End()
{
}
