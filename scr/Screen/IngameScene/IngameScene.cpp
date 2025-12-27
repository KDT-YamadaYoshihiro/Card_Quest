#include "IngameScene.h"
#include "../SceneManager.h"
#include "../ResultScene/ResultScene.h"


void IngameScene::Init()
{
	battle->Init();
}

void IngameScene::handleEvent()
{
	//if(battle->CheckWin() || battle->CheckLose())
	//{
	//	SceneManager::Instance().ChangeScreen<ResultScene>();
	//}
}

void IngameScene::Update(sf::RenderWindow& arg_window)
{
	battle->Update(arg_window);
}

void IngameScene::Render(sf::RenderWindow& arg_window)
{	
	battle->Render(arg_window);
}

void IngameScene::End()
{
}
