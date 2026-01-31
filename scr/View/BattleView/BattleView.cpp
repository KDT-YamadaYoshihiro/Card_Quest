#include "BattleView.h"
#include "CSVLoad/TextureLoader/TextureLoader.h"
#include "View/Font/FontManager.h"
#include "Entity/Card/CardManager/CardManager.h"

// 座標系
namespace
{
    // UI配置（画像イメージ準拠）
    constexpr sf::Vector2f DECK_POS{ 50.f, 120.f };
    constexpr sf::Vector2f GRAVE_POS{ 150.f, 520.f };

    constexpr sf::Vector2f HAND_START{ 300.f, 520.f };
    constexpr float HAND_SPACING = 140.f;
    constexpr float SELECT_OFFSET_Y = 20.f;
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="context"></param>
BattleView::BattleView(BattleContext& context, RenderSystem& render)
    : m_context(context),
    m_render(render),
    m_font(FontManager::GetInstance().GetFont())
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
void BattleView::Render(sf::RenderWindow& window)
{
    // カメラ機能ON
    m_render.ApplyCamera();

    // 背景
    auto tex = TextureLoader::GetInstance().GetTextureID("test");
    if (tex)
    {
        sf::Sprite sprite(*tex);
        sprite.setPosition({ 0.0f,0.0f });
        window.draw(sprite);
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

        window.draw(text);
    }
    // ターゲットのサークル
    DrawFocus(window);

    // カメラ機能OFF
    m_render.ResetCamera();
    DrawCards(window);
    DrawCostGain(window);
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
void BattleView::DrawCards(sf::RenderWindow& window)
{
    // ===== 山札 =====
    m_cardRenderer->DrawDeck(m_font, m_render.GetWindow(), DECK_POS, CardManager::GetInstance().GetDeckCount());

    //// ===== 墓地 =====
    //m_cardRenderer->DrawGrave(m_font, m_render.GetWindow(), GRAVE_POS, CardManager::GetInstance().GetCemeteryCount());

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
void BattleView::DrawFocus(sf::RenderWindow& window)
{

    constexpr float RADIUS = 80.f;

    sf::CircleShape circle(RADIUS);
    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineThickness(4.f);
    circle.setOutlineColor(sf::Color::Yellow);

    for (auto& target : m_targets)
    {
        if (!target)
        {
            continue;
        }

        sf::Vector2f center = GetCharacterCenter(target);

        // CircleShape は左上基準なので半径分引く
        circle.setPosition({center.x - RADIUS,center.y - RADIUS});

        window.draw(circle);
    }
}

/// <summary>
/// コスト表示
/// </summary>
/// <param name="window"></param>
void BattleView::DrawCostGain(sf::RenderWindow& window)
{
    if (m_costGain <= 0) return;

    sf::Text text(FontManager::GetInstance().GetFont(), "");
    text.setString("+" + std::to_string(m_costGain));
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::Yellow);
    text.setPosition({600.0f, 450.0f});

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

    auto pos = c->GetPosition(); // 左上
    return {pos.x + CHAR_W * 0.5f,pos.y + CHAR_H * 0.5f};
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
