#include "DeckBuildingScene.h"
#include "Scene/SceneManager/SceneManager.h"
#include "Scene/IngameScene/IngameScene.h"
#include "System/DeckBulid/CardBuildPool/CardBuildPool.h"
#include "Entity/Card/CardManager/CardManager.h"
#include "System/InPutManager/InPutMouseManager.h"
#include "View/Font/FontManager.h"

DeckBuildingScene::DeckBuildingScene()
    :SceneBase(),
    m_completeButton(50.0f, { 700.0f, 500.0f })
{
	std::cout << "デッキ編成シーン" << std::endl;
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
    // 編成完了ボタン
    if (m_completeButton.IsClicked(mousePos, input.IsLeftClicked()))
    {
        if (m_deckBuildSystem.IsComplete())
        {
			// CardManager にデッキをセット
            CardManager::GetInstance().InitDeck(m_deckBuildSystem.TakeDeck());
			// シーン切り替え
			SceneManager::GetInstance().ChangeScreen<IngameScene>(arg_window);
        }
        // else: 30枚未満 → 何もしない or SE
    }
}

void DeckBuildingScene::Render(sf::RenderWindow& arg_window)
{
    // --- デッキ & プール描画 ---
	m_deckBuildSystem.Draw(arg_window, FontManager::GetInstance().GetFont());

  // --- 完了ボタン ---
    m_completeButton.Draw(arg_window);
}

void DeckBuildingScene::End()
{
}
