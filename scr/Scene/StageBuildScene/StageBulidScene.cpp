#include "StageBulidScene.h"
#include "Scene/SceneManager/SceneManager.h"
#include "System/InPutManager/InPutMouseManager.h"
#include "System/StageBulid/StageData.h"
#include "System/Battle/BattleContex/BattleContext.h"
#include "Scene/PartyBuildScene/PartyBuildScene.h"

StageBulidScene::StageBulidScene(sf::RenderWindow& arg_window)
{
    // 
    ConsoleView::GetInstance().Add("StageBulidScene\n");

    Init(arg_window);
}

void StageBulidScene::Init(sf::RenderWindow& arg_window)
{
    // RenderSystem
    m_render = std::make_unique<RenderSystem>(arg_window);

    // Context 初期化
    m_context.Init();

    // View / Controller
    m_view = std::make_unique<StageBuildView>(*m_render);
    m_controller = std::make_unique<StageBulidController>(m_context);

    // 決定ボタン
    m_button = std::make_unique<CircleButton>(50.0f, sf::Vector2f(700.0f,600.0f));
}

void StageBulidScene::handleEvent(const sf::Event& event)
{
    // ホイール等のイベントは InputManager に流す
    InPutMouseManager::GetInstance().HandleEvent(event);
}

void StageBulidScene::Update(sf::RenderWindow& arg_window)
{
    auto& input = InPutMouseManager::GetInstance();
    // 入力更新
    input.Update(arg_window);

    // 選択処理（左リスト）
    m_controller->Update(arg_window);

    // View 更新（スクロール・詳細スライド）
    m_view->Update(arg_window, m_context);

    //　ボタン
    // マウス座標取得
    sf::Vector2f mousePos = input.GetMousePosition(arg_window);
    float wheel = input.GetWheelDelta();

    // ステージ選択終了
    if (m_button->IsClicked(mousePos, input.IsLeftClicked()))
    {
        DecideStage(arg_window);
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
    m_button->Draw(arg_window);
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
