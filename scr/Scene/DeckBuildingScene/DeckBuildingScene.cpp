#include "DeckBuildingScene.h"
#include "Scene/SceneManager/SceneManager.h"
#include "Scene/IngameScene/IngameScene.h"
#include "System/DeckBulid/CardBuildPool/CardBuildPool.h"
#include "Entity/Card/CardManager/CardManager.h"
#include "View/Font/FontManager.h"
#include "Scene/PartyBuildScene/PartyBuildScene.h"
#include "UI/BoxButton.h"

DeckBuildingScene::DeckBuildingScene()
    : SceneBase()
{

}

bool DeckBuildingScene::Init(sf::RenderWindow& arg_window)
{
    m_nextButton = std::make_unique<BoxButton>(sf::Vector2f(200.f, 50.f), sf::Vector2f(1000.f, 680.f), FontManager::GetInstance().GetFont(), "BATTLE START");
    if(!m_nextButton)
    {
        ConsoleView::GetInstance().Add("DeckBuildingScene/m_nextButton:nullptr\n");
        return false;
	}

    m_backButton = std::make_unique<BoxButton>(sf::Vector2f(200.f, 50.f), sf::Vector2f(200.f, 680.f), FontManager::GetInstance().GetFont(), "BACK");
    if (!m_backButton)
    {
        ConsoleView::GetInstance().Add("DeckBuildingScene/m_backButton:nullptr\n");
        return false;
    }

    ConsoleView::GetInstance().Add("DeckBuildingScene\n");
    // カードプール構築
    CardBuildPool::GetInstance().Build();
    m_deckBuildSystem.Init();

	return true;
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


	// ボタンのカーソルが合ったら色を変える
	// 完了ボタン
	if(m_nextButton->IsHovered(mousePos))
    {
        m_nextButton->SetColor(sf::Color::Yellow);
    }
    else
    {
        m_nextButton->SetColor(sf::Color::White);
	}
	// 戻るボタン
    if(m_backButton->IsHovered(mousePos))
    {
        m_backButton->SetColor(sf::Color::Yellow);
    }
    else
    {
        m_backButton->SetColor(sf::Color::White);
    }

	// デッキ編成システム更新
    m_deckBuildSystem.Update(mousePos, input.IsLeftClicked(),input.IsDragging(),input.IsLeftReleased(), wheel);
    // 編成完了ボタン(デッキ枚数が30枚以上の時)
    if (m_nextButton->IsClicked(mousePos, input.IsLeftClicked()))
    {
        if (m_deckBuildSystem.IsComplete())
        {
			// CardManager にデッキをセット
            CardManager::GetInstance().InitDeck(m_deckBuildSystem.TakeDeck());
            // Consoleのリセット
            ConsoleView::GetInstance().Reset();
			// シーン切り替え
			SceneManager::GetInstance().ChangeScreen<IngameScene>(arg_window);

            return;
        }
        
    }


	// 戻るボタン
	if (m_backButton->IsClicked(mousePos, input.IsLeftClicked()))
	{
        // CardManager にデッキをセット
        CardManager::GetInstance().InitDeck(m_deckBuildSystem.TakeDeck());
		// Consoleのリセット
		ConsoleView::GetInstance().Reset();
		// シーン切り替え
		SceneManager::GetInstance().ChangeScreen<PartyBuildScene>(arg_window);

        return;
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

    // パーティの描画
    auto& session = SceneManager::GetInstance().GetSession();
	const auto& party = session.battleContext->GetPlayers();
	float posX = 850.f;
    for (auto& member : party)
    {
		auto tex = TextureLoader::GetInstance().GetTextureID(member->GetData().iconKey);
        if (!tex)
        {
            return;
        }

        sf::Sprite sprite(*tex);
        sprite.setPosition({ posX, 20.f });
        sprite.setOrigin({ sprite.getLocalBounds().size.x, 0.0f });
        sprite.setScale({ -0.8f, 0.8f });
        arg_window.draw(sprite);

		posX += 100.f;
    }



     // --- 完了ボタン ---
    m_nextButton->Draw(arg_window);
	m_backButton->Draw(arg_window);
}

void DeckBuildingScene::End()
{
}
