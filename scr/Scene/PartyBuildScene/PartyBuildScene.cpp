#include "PartyBuildScene.h"
#include "Entity/Character/Factory/CharacterFactory.h"
#include "System/Battle/BattleContex/BattleContext.h"
#include "Scene/SceneManager/SceneManager.h"
#include "Scene/DeckBuildingScene/DeckBuildingScene.h"
#include "System/InPutManager/InPutMouseManager.h"

/// <summary>
/// 初期化
/// </summary>
PartyBuildScene::PartyBuildScene(sf::RenderWindow& window)
	:SceneBase()
{
	Init(window);
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="arg_window"></param>
void PartyBuildScene::Init(sf::RenderWindow& arg_window)
{
    m_render = std::make_unique<RenderSystem>(arg_window);

    std::vector<std::shared_ptr<Character>> allChars;
    for (int i = 1; i <= 6; ++i)
    {
        allChars.push_back(CharacterFactory::GetInstance().CreateCharacter<PlayerCharacter>(i, 1));
    }

    m_context.Init(allChars);
    m_view = std::make_unique<PartyBuildView>(*m_render);
    m_controller = std::make_unique<PartyBuildController>(m_context, *m_view);
    m_button = std::make_unique<CircleButton>(50,sf::Vector2f(700,700));
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
void PartyBuildScene::Update(sf::RenderWindow& arg_window)
{
    m_view->Update(m_context);
    m_controller->Update(m_render->GetWindow());
    
    // 入力更新
    auto& input = InPutMouseManager::GetInstance();
    input.Update(arg_window);

    // マウス座標取得
    sf::Vector2f mousePos = input.GetMousePosition(arg_window);
    float wheel = input.GetWheelDelta();


    if (m_button->IsClicked(mousePos, input.IsLeftClicked()))
    {
        StartDeckBulid();
    }
}

/// <summary>
/// 描画
/// </summary>
/// <param name="arg_window"></param>
void PartyBuildScene::Render(sf::RenderWindow& arg_window)
{
    m_view->Draw();
    m_button->Draw(arg_window);
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
void PartyBuildScene::StartDeckBulid()
{
    // パーティー情報をセット
    auto& session = SceneManager::GetInstance().GetSession();

    session.battleContext = std::make_shared<BattleContext>();
    session.battleContext->SetPlayers(m_context.GetParty());

    // シーンの切り替え
    SceneManager::GetInstance().ChangeScreen<DeckBuildingScene>();
}
