#include "BattleContex.h"

BattleContext::BattleContext(
    const std::vector<std::shared_ptr<Character>>& players,
    const std::vector<std::shared_ptr<Character>>& enemies,
    CardManager& cardManager,
    CostManager& costManager
)
    : m_players(players)
    , m_enemies(enemies)
    , m_cardManager(cardManager)
    , m_costManager(costManager)
{
}

// プレイヤー取得
const std::vector<std::shared_ptr<Character>>&
BattleContext::GetPlayers() const
{
    return m_players;
}

// エネミー取得
const std::vector<std::shared_ptr<Character>>&
BattleContext::GetEnemies() const
{
    return m_enemies;
}

// カードマネージャー取得
CardManager&
BattleContext::GetCardManager() const
{
    return m_cardManager;
}

// コストマネージャー取得
CostManager&
BattleContext::GetCostManager() const
{
    return m_costManager;
}
