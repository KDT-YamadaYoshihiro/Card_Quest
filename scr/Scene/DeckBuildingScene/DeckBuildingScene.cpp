#include "DeckBuildingScene.h"
#include "Scene/SceneManager/SceneManager.h"
#include "Scene/IngameScene/IngameScene.h"
#include "System/DeckBulid/CardBuildPool/CardBuildPool.h"
#include "Entity/Card/CardManager/CardManager.h"
#include "System/InPutManager/InPutMouseManager.h"
#include "View/Font/FontManager.h"

DeckBuildingScene::DeckBuildingScene()
    : SceneBase(),
    m_completeButton(50.0f, { 700.0f, 600.0f })
{

    ConsoleView::GetInstance().Add("DeckBuildingScene\n");
	// カードプール構築
	CardBuildPool::GetInstance().Build();
    m_deckBuildSystem.Init();
}

void DeckBuildingScene::Init(sf::RenderWindow& arg_window)
{
}

void DeckBuildingScene::handleEvent(const sf::Event& event)
{
    InPutMouseManager::GetInstance().HandleEvent(event);
}

void DeckBuildingScene::Update(sf::RenderWindow& arg_window)
{

	// 入力更新
    auto& input = InPutMouseManager::GetInstance();
    input.Update(arg_window);

	// マウス座標取得
    sf::Vector2f mousePos = input.GetMousePosition(arg_window);

    float wheel = input.GetWheelDelta();

	// デッキ編成システム更新
    m_deckBuildSystem.Update(mousePos, input.IsLeftClicked(),input.IsDragging(),input.IsLeftReleased(), wheel);
    // 編成完了ボタン(デッキ枚数が30枚以上の時)
    if (m_completeButton.IsClicked(mousePos, input.IsLeftClicked()))
    {
        if (m_deckBuildSystem.IsComplete())
        {
			// CardManager にデッキをセット
            CardManager::GetInstance().InitDeck(m_deckBuildSystem.TakeDeck());
            // Consoleのリセット
            ConsoleView::GetInstance().Reset();
			// シーン切り替え
			SceneManager::GetInstance().ChangeScreen<IngameScene>(arg_window);
        }
        
    }
}

void DeckBuildingScene::Render(sf::RenderWindow& arg_window)
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

    // --- デッキ & プール描画 ---
	m_deckBuildSystem.Draw(arg_window, FontManager::GetInstance().GetFont());

     // --- 完了ボタン ---
    m_completeButton.Draw(arg_window);
}

void DeckBuildingScene::End()
{
}
