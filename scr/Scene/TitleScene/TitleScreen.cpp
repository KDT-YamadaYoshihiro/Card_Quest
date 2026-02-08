#include "TitleScene.h"
#include "Scene/SceneManager/SceneManager.h"
#include "Scene/StageBuildScene/StageBulidScene.h"
#include "View/Font/FontManager.h"
#include "System/InPutManager/InPutMouseManager.h"
#include <SFML/Graphics.hpp>


TitleScene::TitleScene()
{
    ConsoleView::GetInstance().Add("TitleScene\n");
}

bool TitleScene::Init(sf::RenderWindow& arg_window)
{
	return true;
}

void TitleScene::handleEvent(const sf::Event& event)
{
}

void TitleScene::Update(sf::RenderWindow& arg_window, float dt)
{
    InPutMouseManager::GetInstance().Update(arg_window);

	m_lightEffect->Update(dt);

    // キーボード判定
    if (InPutMouseManager::GetInstance().IsLeftClicked())
    {
        ConsoleView::GetInstance().Reset();
        SceneManager::GetInstance().ChangeScreen<StageBulidScene>(arg_window);
    }

}

void TitleScene::Render(sf::RenderWindow& arg_window)
{
	// 背景描画
    auto bg = TextureLoader::GetInstance().GetTextureID("bg");
	if (bg)
	{
		sf::Sprite sprite(*bg);
        sprite.setPosition({ 0.0f,0.0f });
        sprite.setScale({ 0.7f,0.7f });
        arg_window.draw(sprite);
	}

	// ライトエフェクト描画
	m_lightEffect->Draw(arg_window);

	// タイトル描画
    auto title = TextureLoader::GetInstance().GetTextureID("Title");
    if (title)
    {
        sf::Sprite sprite(*title);
        sprite.setPosition({ 100.0f,0.0f });
        sprite.setScale({ 0.7f,0.7f });
        arg_window.draw(sprite);
    }

	// CLICK to START 描画
    sf::Text text(FontManager::GetInstance().GetFont(), "CLICK to START");
    text.setPosition({ 400.0f, 400.0f });
    text.setScale({ 2.0f,2.0f });
    arg_window.draw(text);

}

void TitleScene::End()
{
}
