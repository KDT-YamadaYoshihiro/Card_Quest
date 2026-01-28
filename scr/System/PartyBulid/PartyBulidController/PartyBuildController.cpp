#include "PartyBuildController.h"
#include "System/InPutManager/InPutMouseManager.h"

PartyBuildController::PartyBuildController(PartyBuildContext& context, PartyBuildView& view)
	: m_context(context), m_view(view)
{
}

void PartyBuildController::Update(sf::RenderWindow& window)
{

    if (!InPutMouseManager::GetInstance().IsLeftClicked())
        return;

    auto mouse = GetMousePos(window);

    // 左：キャラ一覧 → パーティ追加
    const auto& charIcons = m_view.GetCharacterIcons();
    for (size_t i = 0; i < charIcons.size(); ++i)
    {
        if (charIcons[i].rect.contains(mouse))
        {
            auto ch = GetCharacterByIconIndex(i);
            m_context.AddToParty(ch);
            return;
        }
    }

    // 右：パーティ → 除外
    const auto& partyIcons = m_view.GetPartyIcons();
    for (size_t i = 0; i < partyIcons.size(); ++i)
    {
        if (partyIcons[i].rect.contains(mouse))
        {
            auto ch = m_context.GetParty()[i];
            m_context.RmoveParty(ch);
            return;
        }
    }
}

sf::Vector2f PartyBuildController::GetMousePos(sf::RenderWindow& window)
{
    auto pos = sf::Mouse::getPosition(window);
    return { static_cast<float>(pos.x), static_cast<float>(pos.y) };
}

std::shared_ptr<Character> PartyBuildController::GetCharacterByIconIndex(size_t index)
{
    size_t count = 0;
    for (auto& ch : m_context.GetAllCharacters())
    {
        if (m_context.IsInParty(ch)) continue;
        if (count == index) return ch;
        ++count;
    }
    return nullptr;
}
