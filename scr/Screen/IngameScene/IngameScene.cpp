#include "IngameScene.h"
#include "../SceneManager.h"
#include "../ResultScene/ResultScene.h"


void IngameScene::Init(sf::RenderWindow& arg_window)
{
	//battle->Init(arg_window);
}

void IngameScene::handleEvent(const sf::Event& event)
{
	if(m_battle->IsBattleEnd())
	{
		SceneManager::GetInstance().ChangeScreen<ResultScene>();
	}
}

void IngameScene::Update(sf::RenderWindow& arg_window)
{
	m_battle->Update(arg_window);
}

void IngameScene::Render(sf::RenderWindow& arg_window)
{	

}

void IngameScene::End()
{
}
