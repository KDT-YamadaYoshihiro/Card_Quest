#include "BattleContext.h"
#include <iostream>

bool BattleContext::Init(const std::vector<std::shared_ptr<Character>>& arg_enemies)
{
    m_enemies = arg_enemies;

    for (const auto& p : m_players)
    {
        if (!p)
        {
            std::cout << "BattleContext: player nullptr" << std::endl;
            return false;
        }
    }

    for (const auto& e : m_enemies)
    {
        if (!e)
        {
            std::cout << "BattleContext: enemy nullptr" << std::endl;
            return false;
        }
    }

    return true;
}

void BattleContext::SetPlayers(const std::vector<std::shared_ptr<Character>>& arg_players)
{
    m_players = arg_players;
}

void BattleContext::SetEnemyIds(std::vector<int> arg_enemyIds)
{
    m_enemyIds = arg_enemyIds;
}

std::vector<int> BattleContext::GetEnemyIds()
{
    return m_enemyIds;
}


void BattleContext::SetStageId(const int arg_stageId)
{
    m_stageId = arg_stageId;
}

const std::vector<std::shared_ptr<Character>>& BattleContext::GetPlayers() const
{
    return m_players;
}

const std::vector<std::shared_ptr<Character>>& BattleContext::GetEnemies() const
{
    return m_enemies;
}

std::vector<std::shared_ptr<Character>> BattleContext::GetAlivePlayers() const
{
    std::vector<std::shared_ptr<Character>> result;
    for (const auto& p : m_players)
    {
        if (!p->GetData().dead)
            result.push_back(p);
    }
    return result;
}

std::vector<std::shared_ptr<Character>> BattleContext::GetAliveEnemies() const
{
    std::vector<std::shared_ptr<Character>> result;
    for (const auto& e : m_enemies)
    {
        if (!e->GetData().dead)
            result.push_back(e);
    }
    return result;
}

bool BattleContext::IsPlayerAllDead() const
{
    for (const auto& p : m_players)
    {
        if (!p->GetData().dead)
        {
            return false;
        }
    }
    return true;
}

bool BattleContext::IsEnemyAllDead() const
{
    for (const auto& e : m_enemies)
    {
        if (!e->GetData().dead)
            return false;
    }
    return true;
}

std::vector<std::shared_ptr<Character>> BattleContext::CreateTargetCandidates(TargetType targetType, Faction actorFaction, const std::shared_ptr<Character>& actor) const
{
    std::vector<std::shared_ptr<Character>> result;

    auto pushAlive = [&](const std::vector<std::shared_ptr<Character>>& list)
        {
            for (auto& c : list)
            {

                if (!c || c->IsDead())
                {
                    continue;
                }
                if (c == actor)
                {
                    continue;
                }

                result.push_back(c);
            }
        };

    switch (targetType)
    {
    case TargetType::SELF:

        if (actor && !actor->IsDead())
        {
            result.push_back(actor);
        }
        break;

    case TargetType::ALLY:
    case TargetType::ALLY_ALL:

        if (actorFaction == Faction::Player)
        {
            pushAlive(m_players);
        }
        else
        {
            pushAlive(m_enemies);
        }

        break;

    case TargetType::OPPONENT:
    case TargetType::OPPONENT_ALL:

        if (actorFaction == Faction::Player)
        {
            pushAlive(m_enemies);
        }
        else
        {
            pushAlive(m_players);
        }

        break;
    }

    return result;
}

