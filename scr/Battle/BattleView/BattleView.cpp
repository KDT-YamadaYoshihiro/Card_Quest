#include "BattleView.h"
#include "../../CSVLoad/TextureLoader/TextureLoader.h"
#include "../../View/Font/FontManager.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="context"></param>
BattleView::BattleView(BattleContext& context, RenderSystem& render)
    : m_context(context),
    m_render(render)
{
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
/// 描画
/// </summary>
/// <param name="window"></param>
void BattleView::Render(sf::RenderWindow& window)
{
    DrawCharacters();
    DrawCards(window);
    DrawFocus(window);
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
    if (!m_selectedActor)
    {
        return;
    }

    const int count = m_selectedActor->GetCardCount();
    sf::Vector2f pos(300.f, 520.f);

    for (int i = 0; i < count; ++i)
    {
        sf::Vector2f drawPos = pos;
        if (i == m_selectedCard)
        {
            drawPos.y -= 20.f;
        }

        const CardData& data = m_selectedActor->GetCardData(i);
        m_cardRenderer->DrawHand(FontManager::GetInstance().GetFont(), window, drawPos, data);

        pos.x += 140.f;
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
