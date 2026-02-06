#include "BattleView.h"
#include "CSVLoad/TextureLoader/TextureLoader.h"
#include "View/Font/FontManager.h"
#include "Entity/Card/CardManager/CardManager.h"
#include "System/Battle/Cost/CostManager.h"
#include "CSVLoad/StageLoader/StageLoader.h"


// 座標系
namespace
{
    // UI配置（画像イメージ準拠）
    constexpr sf::Vector2f DECK_POS{ 50.f, 120.f };
    constexpr sf::Vector2f GRAVE_POS{ 150.f, 520.f };

    constexpr sf::Vector2f HAND_START{ 200.f, 520.f };
    constexpr float HAND_SPACING = 180.f;
    constexpr float SELECT_OFFSET_Y = 50.f;
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="context"></param>
BattleView::BattleView(BattleContext& context, RenderSystem& render)
    : m_context(context),
    m_render(render),
    m_font(FontManager::GetInstance().GetFont()),
	m_clearBannerText(m_font, "")
{
    m_cardRenderer = std::make_shared<CardRenderer>();
}

/// <summary>
/// フェーズの設定
/// </summary>
/// <param name="phase"></param>
void BattleView::SetPhase(BattleViewPhase phase)
{
    m_phase  = phase;
}

/// <summary>
/// 更新
/// </summary>
/// <param name="dt"></param>
void BattleView::Update(float dt)
{
    UpdateCamera(dt);

    for (auto& p : m_popups)
    {
        p.lifeTime -= dt;
        p.position.y -= 30.f * dt; // 上に浮かせる
    }

    m_popups.erase(
        std::remove_if(m_popups.begin(), m_popups.end(),
            [](const DamagePopup& p) { return p.lifeTime <= 0.f; }),
        m_popups.end());
}

/// <summary>
///  アクションキャラ設定
/// </summary>
/// <param name="index"></param>
void BattleView::SetSelectedActor(const std::shared_ptr<Character>& actor)
{
    m_selectedActor = actor;
}

/// <summary>
///  選択カード設定
/// </summary
void BattleView::SetSelectedCard(int index)
{
    m_selectedCard = index;
}

/// <summary>
/// ターゲット設定
/// </summary>
void BattleView::SetTargetIndices(const std::vector<std::shared_ptr<Character>>& arg_target)
{
    m_targets = arg_target;
}


/// <summary>
/// コスト設定
/// </summary>
/// <param name="value"></param>
void BattleView::ShowCostGain(int value)
{
    m_costGain = value;
}

/// <summary>
/// 情報リセット
/// </summary>
void BattleView::ResetTransientView()
{
    ClearTargets();
    ClearCostGain();
    m_selectedCard = -1;
}

/// <summary>
/// 効果量の設定
/// </summary>
/// <param name="arg_pos"></param>
/// <param name="arg_value"></param>
/// <param name="arg_isHeal"></param>
void BattleView::AddDamagePopup(const sf::Vector2f& arg_pos, int arg_value, bool arg_isHeal)
{
    DamagePopup p;
    p.position = arg_pos;
    p.value = arg_value;
    p.isHeal = arg_isHeal;
    p.lifeTime = 1.0f;

    m_popups.push_back(p);
}

/// <summary>
/// 描画
/// </summary>
/// <param name="window"></param>
void BattleView::Render(sf::RenderWindow& arg_window)
{
    // カメラ機能ON
    m_render.ApplyCamera();

    // 背景
    auto tex = TextureLoader::GetInstance().GetTextureID("bg");
    if (tex)
    {
        sf::Sprite sprite(*tex);
        sprite.setPosition({ -300.0f,-250.0f });
        sprite.setScale({ 0.9f, 0.9f });
        arg_window.draw(sprite);
    }


    DrawCharacters();

    // ダメージや回復量の表示
    for (auto& p : m_popups)
    {
        sf::Text text(FontManager::GetInstance().GetFont(), "");
        text.setFont(FontManager::GetInstance().GetFont()); // 既存のフォント管理
        text.setString((p.isHeal ? "+" : "-") + std::to_string(p.value));
        text.setCharacterSize(24);
        text.setPosition(p.position);
        text.setFillColor(p.isHeal ? sf::Color::Green : sf::Color::Red);

        arg_window.draw(text);
    }
    // ターゲットのサークル
    DrawFocus(arg_window);

    // カメラ機能OFF
    m_render.ResetCamera();
    DrawCards(arg_window);
    DrawCost(arg_window);
    DrawCostGain(arg_window);
    DrawTurnBanner(arg_window);
    DrawStageName(arg_window);
}

/// <summary>
/// キャラクター描画
/// </summary>
/// <param name="window"></param>
void BattleView::DrawCharacters()
{

    auto drawList = [&](const auto& list)
        {
            for (auto& c : list)
            {
                if (!c) continue;

                // ===== Sprite生成（初回のみ）=====
                if (!m_spriteTable.contains(c.get()))
                {
                    auto sprite = std::make_unique<CharacterSprite>(c->GetData().textureKey);
                    sprite->Init(c->GetData().textureKey);
                    m_spriteTable[c.get()] = std::move(sprite);
                }

                auto& sprite = m_spriteTable[c.get()];

                // ===== 状態決定 =====
                CharacterAnimState state = CharacterAnimState::WAIT;

                if (c->IsDead())
                {
                    state = CharacterAnimState::DEAD;
                }
                else if (c == m_selectedActor)
                {
                    state = CharacterAnimState::WAIT;
                }
                else
                {
                    state = CharacterAnimState::WAIT;
                }

                // プレイヤーなら画像を反転
                if (c->GetFaction() == Faction::Player)
                {
                    sprite->SetSpriteWidthMirror();
                }

                sprite->SetState(state);
                sprite->SetPosition(c->GetPosition());

                sprite->Draw(m_render,c->GetData(),true);
            }
        };

    drawList(m_context.GetPlayers());
    drawList(m_context.GetEnemies());
}

/// <summary>
/// カード描画
/// </summary>
/// <param name="window"></param>
void BattleView::DrawCards(sf::RenderWindow& arg_window)
{
    // ===== 山札 =====
    m_cardRenderer->DrawDeck(m_font, m_render.GetWindow(), DECK_POS, CardManager::GetInstance().GetDeckCount());


    // ===== 手札 =====
    sf::Vector2f pos = HAND_START;

    for (auto& p : m_context.GetAlivePlayers())
    {
        const int cardCount = p->GetCardCount();

        for (int i = 0; i < cardCount; ++i)
        {
            sf::Vector2f drawPos = pos;

            // 選択カードを少し上に
            if (i == m_selectedCard)
            {
                drawPos.y -= SELECT_OFFSET_Y;
            }

            // カードデータ取得
            const CardData& data = p->GetCardData(i);
            // 描画
            m_cardRenderer->DrawSingleCard(m_font, m_render.GetWindow(), drawPos, data,p->GetData().iconKey);

            pos.x += HAND_SPACING;
        }
    }
}

/// <summary>
/// ターゲットに円表示
/// </summary>
/// <param name="window"></param>
void BattleView::DrawFocus(sf::RenderWindow& arg_window)
{

    // Contextから現在フォーカスすべき対象（1体または複数）を取得
    const auto& focusTargets = m_context.GetFocusTargets();

    if (m_context.GetFocusDraw()) {
        for (const auto& target : focusTargets)
        {
            if (!target || target->IsDead()) continue;

            // 円のサイズ設定
            float circleRadius = 60.f;
            sf::CircleShape circle(circleRadius);
            circle.setOutlineColor(sf::Color::Yellow);
            circle.setOutlineThickness(3.f);
            circle.setFillColor(sf::Color::Transparent);

           
            circle.setOrigin({ circleRadius, circleRadius });
            // キャラクターの中心座標を取得して設定
            sf::Vector2f centerPos = GetCharacterCenter(target);
            circle.setPosition(centerPos);
            // --------------------

            arg_window.draw(circle);
        }
    }
}

/// <summary>
/// コスト表示
/// </summary>
/// <param name="arg_window"></param>
void BattleView::DrawCost(sf::RenderWindow& arg_window)
{
    auto tex = TextureLoader::GetInstance().GetTextureID("CostFrame");
    sf::Sprite sprite(*tex);
    sprite.setPosition(sf::Vector2f(550.0f, 100.0f));
    sprite.setScale({0.13f, 0.08f});
    arg_window.draw(sprite);

    sf::Text text(m_font, "");
    int cost = CostManager::GetInstance().GetCurrentCost();
    text.setString({"AP / " +  std::to_string(cost)});
    auto pos = sprite.getPosition();
    text.setPosition({ pos.x + 55 , pos.y + 30 });
    arg_window.draw(text);
}


/// <summary>
/// コスト表示
/// </summary>
/// <param name="window"></param>
void BattleView::DrawCostGain(sf::RenderWindow& arg_window)
{
    if (m_costGain <= 0)
    {
        return;
    }

    sf::Text text(m_font, "");
    text.setString("+" + std::to_string(m_costGain));
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::Yellow);
    text.setPosition({600.0f, 450.0f});

    arg_window.draw(text);
}

/// <summary>
/// ステージ名前の表示
/// </summary>
/// <param name="arg_window"></param>
void BattleView::DrawStageName(sf::RenderWindow& arg_window)
{

    // ステージIDの取得
    int stageId = m_context.GetStageId();

    const auto& stageData = StageLoader::GetInstance().GetStageData(stageId);

    if (!stageData)
    {
        return;
    }
    // ステージ名
    sf::Text stageText(m_font, "");
    // UTF8に対応させる
    sf::String unicodeString = sf::String::fromUtf8(stageData->name.begin(), stageData->name.end());
    stageText.setString(unicodeString);
    stageText.setCharacterSize(30);
    stageText.setFillColor(sf::Color::White);
    stageText.setOutlineColor(sf::Color::Black); 
    stageText.setOutlineThickness(2.f);
    stageText.setPosition({ 20.0f, 50.0f });

    // 下地
    sf::FloatRect textBounds = stageText.getGlobalBounds();
    sf::RectangleShape bgBox;

    // ボックスのサイズ：テキストの範囲より少し大きくする（パディング）
    float paddingX = 20.f;
    float paddingY = 10.f;
    bgBox.setSize({ textBounds.size.x + paddingX * 2, textBounds.size.y + paddingY * 2 });

    // ボックスの色設定（半透明の青にすると画面に馴染みやすいです）
    bgBox.setFillColor(sf::Color(0, 50, 150, 180)); // 暗めの青、少し透過
    bgBox.setOutlineColor(sf::Color::Cyan);        // 枠線に明るい青
    bgBox.setOutlineThickness(2.f);

    // ボックスの位置：テキストを包むように配置
    bgBox.setPosition({ textBounds.position.x - paddingX, textBounds.position.y - paddingY });

    // 4. 描画（先にボックス、その後にテキストを重ねる）
    arg_window.draw(bgBox);
    arg_window.draw(stageText);
}

void BattleView::DrawTurnBanner(sf::RenderWindow& window)
{
    // BattleSystem.h の TurnPhase 定義と合わせる
    // 0: StartTurn, 1: UserTurn, 2: EnemyTurn ... 
    int phase = m_context.GetTurnPhase();

    std::string turnStr = "";
    sf::Color textColor = sf::Color::White;

    if (phase == 1) { // UserTurn
        turnStr = "PLAYER TURN";
        textColor = sf::Color::Cyan; // 青系
    }
    else if (phase == 2) { // EnemyTurn
        turnStr = "ENEMY TURN";
        textColor = sf::Color(255, 100, 100); // 赤系
    }
    else {
        return; // それ以外のフェーズ（Start等）では表示しない、または別の表示
    }

    sf::Text text(m_font, "");
    text.setString(turnStr);
    text.setCharacterSize(50); // 大きめに表示
    text.setFillColor(textColor);
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(4.f);

    // 画面中央に配置
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin({ textRect.position.x + textRect.size.x / 2.0f,textRect.position.y + textRect.size.y / 2.0f });

    // ウィンドウサイズを取得して中央へ
    sf::Vector2u windowSize = window.getSize();
    text.setPosition({ windowSize.x / 2.0f, 50.f }); 

    window.draw(text);
}

/// <summary>
/// ターゲット情報のリセット
/// </summary>
void BattleView::ClearTargets()
{
    m_targets.clear();
}

/// <summary>
/// 行動須数の増加値リセット
/// </summary>
void BattleView::ClearCostGain()
{
    m_costGain = 0;
}

/// <summary>
/// キャラクターの中心座標の取得
/// </summary>
/// <param name="c"></param>
/// <returns></returns>
sf::Vector2f BattleView::GetCharacterCenter(const std::shared_ptr<Character>& c)
{
    constexpr float CHAR_W = 165.f;
    constexpr float CHAR_H = 150.f;

    // キャラクターの左上座標を取得
    sf::Vector2f pos = c->GetPosition();

    // 矩形サイズ（CHAR_W, CHAR_H）の半分を足して中心を計算
    return { pos.x + (CHAR_W * 0.5f), pos.y + (CHAR_H * 0.5f) };
}

void BattleView::UpdateCamera(float dt)
{

    auto& camera = CameraManager::GetInstance();

    switch (m_phase)
    {
    case BattleViewPhase::SelectPlayer:
    case BattleViewPhase::SelectCard:
    {
        // 全プレイヤーが収まる中心点を計算
        const auto& players = m_context.GetPlayers();
        if (players.empty()) break;

        sf::Vector2f averageCenter{ 0.f, 0.f };
        for (auto& p : players)
        {
            averageCenter += GetCharacterCenter(p);
        }
        averageCenter /= static_cast<float>(players.size());

        camera.SetMove(averageCenter, 5.0f);
        camera.SetZoom(1.0f, 2.0f);
        camera.ViewStopFollow();
        break;
    }

    case BattleViewPhase::SelectTarget:
    {
        if (m_targets.empty())
        {
            break;
        }

        sf::Vector2f targetCenter{ 0.f, 0.f };
        for (auto& t : m_targets)
        {
            targetCenter += GetCharacterCenter(t);
        }

        targetCenter /= static_cast<float>(m_targets.size());

        // ターゲット選択時は少しズームインして注目させる
        camera.SetMove(targetCenter, 6.0f);
        camera.SetZoom(0.7f, 3.0f);
        break;
    }
    case BattleViewPhase::Default:
        // 初期位置に戻る
        camera.SetMove({ 640.0f, 360.0f }, 4.0f);
        camera.SetZoom(1.0f, 2.0f);
        break;
    }

    camera.ViewUpdate(dt);
}

sf::Vector2f BattleView::CalcDamagePopupPos(const std::shared_ptr<Character>& c)
{
    auto center = GetCharacterCenter(c);
    center.y -= 40.f; // 頭上に浮かせる
    return center;
}

void BattleView::DrawClearBanner(sf::RenderWindow& arg_window)
{

	m_clearBannerText = sf::Text(m_font, "GAME CLEAR!");

	// バナー文字列設定
	m_clearBannerText.setString("STAGE CLEAR!");
	m_clearBannerText.setCharacterSize(60);
	m_clearBannerText.setFillColor(sf::Color::Green);
	m_clearBannerText.setOutlineColor(sf::Color::Black);
	m_clearBannerText.setOutlineThickness(4.f);
	// 画面中央に配置
	sf::FloatRect textRect = m_clearBannerText.getLocalBounds();
	m_clearBannerText.setOrigin({ textRect.position.x + textRect.size.x / 2.0f,textRect.position.y + textRect.size.y / 2.0f });
	// ウィンドウサイズを取得して中央へ
	sf::Vector2u windowSize = arg_window.getSize();
	m_clearBannerText.setPosition({ windowSize.x / 2.0f, windowSize.y / 2.0f });
	arg_window.draw(m_clearBannerText);


}

void BattleView::DrawGameOverBanner(sf::RenderWindow& arg_window)
{

    m_clearBannerText = sf::Text(m_font, "GAME OVER");


	// バナー文字列設定
	m_clearBannerText.setString("GAME OVER");
	m_clearBannerText.setCharacterSize(60);
	m_clearBannerText.setFillColor(sf::Color::Red);
	m_clearBannerText.setOutlineColor(sf::Color::Black);
	m_clearBannerText.setOutlineThickness(4.f);
	// 画面中央に配置
	sf::FloatRect textRect = m_clearBannerText.getLocalBounds();
	m_clearBannerText.setOrigin({ textRect.position.x + textRect.size.x / 2.0f,textRect.position.y + textRect.size.y / 2.0f });
	// ウィンドウサイズを取得して中央へ
	sf::Vector2u windowSize = arg_window.getSize();
	m_clearBannerText.setPosition({ windowSize.x / 2.0f, windowSize.y / 2.0f });
	arg_window.draw(m_clearBannerText);

}
