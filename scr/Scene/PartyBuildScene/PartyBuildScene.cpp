#include "PartyBuildScene.h"
#include "Entity/Character/Factory/CharacterFactory.h"
#include "System/Battle/BattleContex/BattleContext.h"
#include "Scene/SceneManager/SceneManager.h"
#include "Scene/DeckBuildingScene/DeckBuildingScene.h"
#include "System/InPutManager/InPutMouseManager.h"
#include "View/Font/FontManager.h"
#include "Scene/StageBuildScene/StageBulidScene.h"

/// <summary>
/// 初期化
/// </summary>
PartyBuildScene::PartyBuildScene()
	:SceneBase()
{

    ConsoleView::GetInstance().Add("PartyBuildScene\n");
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="arg_window"></param>
bool PartyBuildScene::Init(sf::RenderWindow& arg_window)
{
    m_render = std::make_unique<RenderSystem>(arg_window);
    if (!m_render)
    {
        ConsoleView::GetInstance().Add("PartyBuildScene/m_render:nullptr\n");
        return false;
	}

    std::vector<std::shared_ptr<Character>> allChars;
    for (int i = 1; i <= 6; ++i)
    {
        allChars.push_back(CharacterFactory::GetInstance().CreateCharacter<PlayerCharacter>(i, 1));
    }

    m_context.Init(allChars);

    if(m_context.GetParty().empty())
    {
        ConsoleView::GetInstance().Add("PartyBuildScene/m_context.GetParty():empty\n");
	}

    m_view = std::make_unique<PartyBuildView>(*m_render);
    if (!m_view)
    {
        ConsoleView::GetInstance().Add("PartyBuildScene/m_view:nullptr\n");
        return false;
    }

    m_controller = std::make_unique<PartyBuildController>(m_context, *m_view);
    if (!m_controller)
    {
        ConsoleView::GetInstance().Add("PartyBuildScene/m_controller:nullptr\n");
        return false;
    }

	// ボタン
    m_nextButton = std::make_unique<BoxButton>(sf::Vector2f(200.f, 50.f), sf::Vector2f(1000.f, 680.f), FontManager::GetInstance().GetFont(), "NEXT");
    if(!m_nextButton)
    {
        ConsoleView::GetInstance().Add("PartyBuildScene/m_nextButton:nullptr\n");
        return false;
	}

    m_backButton = std::make_unique<BoxButton>(sf::Vector2f(200.f, 50.f), sf::Vector2f(200.f, 680.f), FontManager::GetInstance().GetFont(), "BACK");
    if (!m_backButton)
    {
        ConsoleView::GetInstance().Add("PartyBuildScene/m_backButton:nullptr\n");
        return false;
    }

	return true;
}

/// <summary>
/// イベント処理
/// </summary>
/// <param name="event"></param>
void PartyBuildScene::handleEvent(const sf::Event& event)
{
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="arg_window"></param>
void PartyBuildScene::Update(sf::RenderWindow& arg_window, float dt)
{
    m_view->Update(m_context);
    m_controller->Update(m_render->GetWindow());
    
    // 入力更新
    auto& input = InPutMouseManager::GetInstance();
    input.Update(arg_window);

    // マウス座標取得
    sf::Vector2f mousePos = input.GetMousePosition(arg_window);
    float wheel = input.GetWheelDelta();

    // ライトエフェクト更新
    m_lightEffect->Update(dt);


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


	// 戻るボタン処理
    if (m_backButton->IsClicked(mousePos, input.IsLeftClicked()))
    {
        // Consoleのリセット
        ConsoleView::GetInstance().Reset();
        SceneManager::GetInstance().ChangeScreen<StageBulidScene>(arg_window);
        return;
	}
	// 次へボタン処理
    if (m_nextButton->IsClicked(mousePos, input.IsLeftClicked()))
    {
        if (!m_context.GetParty().empty()) {
            StartDeckBulid(arg_window);
			return;
        }
    }
}

/// <summary>
/// 描画
/// </summary>
/// <param name="arg_window"></param>
void PartyBuildScene::Render(sf::RenderWindow& arg_window)
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
    
	// ライトエフェクト描画
    m_lightEffect->Draw(arg_window);
	// パーティー編成画面描画
    m_view->Draw(m_context);
	// ボタン描画
    m_nextButton->Draw(arg_window);
    m_backButton->Draw(arg_window);

}

/// <summary>
/// 終了処理
/// </summary>
void PartyBuildScene::End()
{
}

/// <summary>
/// デッキ編成画面に移行
/// </summary>
void PartyBuildScene::StartDeckBulid(sf::RenderWindow& arg_window)
{
    // パーティー情報をセット
    auto& session = SceneManager::GetInstance().GetSession();

    session.battleContext->SetPlayers(m_context.GetParty());

    // Consoleのリセット
    ConsoleView::GetInstance().Reset();

    // シーンの切り替え
    SceneManager::GetInstance().ChangeScreen<DeckBuildingScene>(arg_window);
}
