#include "ResultScene.h"

ResultScene::ResultScene()
	: SceneBase()
{
}

bool ResultScene::Init(sf::RenderWindow& arg_window)
{
	return true;
}

void ResultScene::handleEvent(const sf::Event& event)
{
}

void ResultScene::Update(sf::RenderWindow& arg_window, float dt)
{
    // ライトエフェクト更新
    m_lightEffect->Update(dt);

}

void ResultScene::Render(sf::RenderWindow& arg_window)
{
    // 背景
    auto tex = TextureLoader::GetInstance().GetTextureID("bg");
    if (tex)
    {
        sf::Sprite sprite(*tex);
        sprite.setPosition({ 0.0f,0.0f });
        sprite.setScale({ 0.7f,0.7f });
        arg_window.draw(sprite);
    }

}

void ResultScene::End()
{
}
