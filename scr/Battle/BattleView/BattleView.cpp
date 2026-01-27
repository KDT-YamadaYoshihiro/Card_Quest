#include "BattleView.h"
#include "../../CSVLoad/TextureLoader/TextureLoader.h"
#include "../../View/Font/FontManager.h"
#include "../../Card/CardManager/CardManager.h"

// 座標系
namespace
{
    // UI配置（画像イメージ準拠）
    constexpr sf::Vector2f DECK_POS{ 50.f, 520.f };
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
    // カメラ機能N
    m_render.ApplyCamera();
    DrawCharacters();

    // カメラ機能OFF
    m_render.ResetCamera();
    DrawCards(window);
    DrawFocus(window);
    DrawCostGain(window);
    for (auto& p : m_popups)
    {
        sf::Text text(FontManager::GetInstance().GetFont(),"");
        text.setFont(FontManager::GetInstance().GetFont()); // 既存のフォント管理
        text.setString((p.isHeal ? "+" : "-") + std::to_string(p.value));
        text.setCharacterSize(24);
        text.setPosition(p.position);
        text.setFillColor(p.isHeal ? sf::Color::Green : sf::Color::Red);

        window.draw(text);
    }
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

                sprite->SetState(state);
                sprite->SetPosition(c->GetPosition());

                sprite->Draw(m_render);
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

    // ===== 墓地 =====
    m_cardRenderer->DrawGrave(m_font, m_render.GetWindow(), GRAVE_POS, CardManager::GetInstance().GetCemeteryCount());

    // ===== 手札 =====
    if (!m_selectedActor)
    {
        return;
    }

    const int cardCount = m_selectedActor->GetCardCount();
    sf::Vector2f pos = HAND_START;

    for (int i = 0; i < cardCount; ++i)
    {
        sf::Vector2f drawPos = pos;

        // 選択カードを少し上に
        if (i == m_selectedCard)
        {
            drawPos.y -= SELECT_OFFSET_Y;
        }

        // カードデータ取得
        const CardData& data = m_selectedActor->GetCardData(i);
        // 描画
        m_cardRenderer->DrawSingleCard(m_font,m_render.GetWindow(),drawPos,data);

        pos.x += HAND_SPACING;
    }
}

/// <summary>
/// ターゲットに円表示
/// </summary>
/// <param name="window"></param>
void BattleView::DrawFocus(sf::RenderWindow& window)
{

    sf::CircleShape circle(80.f);
    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineThickness(4.f);
    circle.setOutlineColor(sf::Color::Yellow);

    for (auto& target : m_targets)
    {
        if (!target)
        {
            continue;
        }

        auto pos = target->GetPosition();
        circle.setPosition({ pos.x - 80.0f, pos.y - 80.0f });
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
