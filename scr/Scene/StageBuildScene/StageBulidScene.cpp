#include "StageBulidScene.h"
#include "Scene/SceneManager/SceneManager.h"
#include "System/InPutManager/InPutMouseManager.h"
#include "System/StageBulid/StageData.h"
#include "System/Battle/BattleContex/BattleContext.h"
#include "Scene/PartyBuildScene/PartyBuildScene.h"
#include "Scene/TitleScene/TitleScene.h"
#include "View/Font/FontManager.h"
#include "UI/BoxButton.h"
#include "UI/TriangleButton.h"

StageBulidScene::StageBulidScene()
{
    // 
    ConsoleView::GetInstance().Add("StageBulidScene\n");

}

bool StageBulidScene::Init(sf::RenderWindow& arg_window)
{
    // RenderSystem
    m_render = std::make_unique<RenderSystem>(arg_window);
    if(!m_render)
    {
        ConsoleView::GetInstance().Add("StageBulidScene/m_render:nullptr\n");
        return false;
	}

    // Context 初期化
    m_context.Init();

    // View / Controller
    m_view = std::make_unique<StageBuildView>(*m_render);
    if(!m_view)
    {
        ConsoleView::GetInstance().Add("StageBulidScene/m_view:nullptr\n");
        return false;
	}
    m_controller = std::make_unique<StageBulidController>(m_context);
    if (!m_controller)
    {
        ConsoleView::GetInstance().Add("StageBulidScene/m_controller:nullptr\n");
        return false;
    }

	// ボタン
    m_nextButton = std::make_unique<BoxButton>(sf::Vector2f(200.f, 50.f), sf::Vector2f(1000.f, 680.f), FontManager::GetInstance().GetFont(), "NEXT");
    if (!m_nextButton)
    {
        ConsoleView::GetInstance().Add("StageBulidScene/m_nextButton:nullptr\n");
        return false;
    }
    m_backButton = std::make_unique<BoxButton>(sf::Vector2f(200.f, 50.f), sf::Vector2f(200.f, 680.f), FontManager::GetInstance().GetFont(), "BACK");
    if(!m_backButton)
    {
        ConsoleView::GetInstance().Add("StageBulidScene/m_backButton:nullptr\n");
        return false;
	}
    m_leftArrow = std::make_unique<TriangleButton>(sf::Vector2f(50, 70), sf::Vector2f(100, 300), TriangleButton::Direction::Left);
    if (!m_leftArrow)
    {
        ConsoleView::GetInstance().Add("StageBulidScene/m_leftArrow:nullptr\n");
        return false;
    }
	m_rightArrow = std::make_unique<TriangleButton>(sf::Vector2f(50, 70), sf::Vector2f(1200, 300), TriangleButton::Direction::Right);
    if(!m_rightArrow)
    {
        ConsoleView::GetInstance().Add("StageBulidScene/m_rightArrow:nullptr\n");
        return false;
	}

	// Contextにボタンを登録
    m_context.SetNavigationButtons(std::move(m_leftArrow), std::move(m_rightArrow));
	m_context.SetSceneButton(std::move(m_nextButton), std::move(m_backButton)); 

	return true;
}

void StageBulidScene::handleEvent(const sf::Event& event)
{
    // ホイール等のイベントは InputManager に流す
    InPutMouseManager::GetInstance().HandleEvent(event);
}

void StageBulidScene::Update(sf::RenderWindow& arg_window)
{
    auto& input = InPutMouseManager::GetInstance();
    auto mousePos = input.GetMousePosition(arg_window);
    // 入力更新
    input.Update(arg_window);

    // 選択処理（左リスト）
    m_controller->Update(arg_window);

    // View 更新（スクロール・詳細スライド）
    m_view->Update(arg_window, m_context);

	// ボタンのカーソルが合ったら色を変える
	// 次へボタン
	if (m_context.GetNextButton()->IsHovered(mousePos))
	{
		m_context.GetNextButton()->SetColor(sf::Color::Yellow);
	}
	else
	{
		m_context.GetNextButton()->SetColor(sf::Color::White);
	}
	// 戻るボタン
	if (m_context.GetBackButton()->IsHovered(mousePos))
	{
		m_context.GetBackButton()->SetColor(sf::Color::Yellow);
	}
	else
	{
		m_context.GetBackButton()->SetColor(sf::Color::White);
	}


    // クリックしていなければ処理しない
    if (!input.IsLeftClicked())
    {
        return;
    }

	// シーン切り替え
	if (m_context.GetNextButton()->IsClicked(input.GetMousePosition(arg_window), input.IsLeftClicked()))
	{
        DecideStage(arg_window);
        return;
	}
    if (m_context.GetBackButton()->IsClicked(input.GetMousePosition(arg_window), input.IsLeftClicked()))
    {
        ConsoleView::GetInstance().Reset();
        SceneManager::GetInstance().ChangeScreen<TitleScene>(arg_window);
        return;
    }

}

void StageBulidScene::Render(sf::RenderWindow& arg_window)
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

    m_view->Draw(m_context);
}

void StageBulidScene::End()
{
}

void StageBulidScene::DecideStage(sf::RenderWindow& arg_window)
{
    const StageData* stage = m_context.GetSelectedStage();
    if (!stage) return;

    auto& session = SceneManager::GetInstance().GetSession();

    session.battleContext = std::make_shared<BattleContext>();
    session. battleContext->SetStageId(stage->stageId);

    // vector<int>に変換
    std::vector<int> vec(stage->enemyIds.begin(), stage->enemyIds.end());
    session.battleContext->SetEnemyIds(vec); // std::array<int,3>

    ConsoleView::GetInstance().Reset();

    SceneManager::GetInstance().ChangeScreen<PartyBuildScene>(arg_window);
}
