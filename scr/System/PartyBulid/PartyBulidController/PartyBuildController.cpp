#include "PartyBuildController.h"
#include "System/InPutManager/InPutMouseManager.h"

PartyBuildController::PartyBuildController(PartyBuildContext& context, PartyBuildView& view)
	: m_context(context), m_view(view)
{
}

void PartyBuildController::Update(sf::RenderWindow& window)
{

    if (!InPutMouseManager::GetInstance().IsLeftClicked())
    {
        return;
    }

    auto mouse = GetMousePos(window);

    // 全キャラ一覧（ベンチ） → パーティ追加
    const auto& charIcons = m_view.GetCharacterIcons();
    for (size_t i = 0; i < charIcons.size(); ++i)
    {
        if (charIcons[i].rect.contains(mouse))
        {
            // アイコンに対応するキャラを直接取得
            auto ch = GetCharacterByIconIndex(i);
            if (ch)
            {
                m_context.AddToParty(ch);
            }
            return;
        }
    }

    // パーティメンバー → 除外
    const auto& partyIcons = m_view.GetPartyIcons();
    // パーティリストのサイズとアイコン数が一致している前提
    const auto& partyList = m_context.GetParty();

    for (size_t i = 0; i < partyIcons.size(); ++i)
    {
        if (partyIcons[i].rect.contains(mouse))
        {
            if (i < partyList.size())
            {
                auto ch = partyList[i];
                m_context.RmoveParty(ch);
            }
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

    auto allChars = m_context.GetAllCharacters();
    if (index < allChars.size())
    {
        return allChars[index];
    }
    return nullptr;
}
