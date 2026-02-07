#include "BattleContext.h"
#include <iostream>
#include "View/ConsoleView/ConsoleView.h"

bool BattleContext::Init(const std::vector<std::shared_ptr<Character>>& arg_enemies)
{
    m_enemies = arg_enemies;

    for (const auto& p : m_players)
    {
        if (!p)
        {
            ConsoleView::GetInstance().Add("BattleContext: player nullptr\n");
            
            return false;
        }
    }

    for (const auto& e : m_enemies)
    {
        if (!e)
        {
            ConsoleView::GetInstance().Add("BattleContext: enemy nullptr\n");

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

int BattleContext::GetStageId() const
{
    return m_stageId;
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
        {
            result.push_back(e);
        }
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

// 現状未使用------------------------------------------------------
int BattleContext::GetHoveredCardIndex() const
{
    return 0;
}
std::shared_ptr<Character> BattleContext::GetHoveredTarget() const
{
    return std::shared_ptr<Character>();
}
// ----------------------------------------------------------------


void BattleContext::SetFocusTargets(const std::vector<std::shared_ptr<Character>>& targets)
{
    m_focusTargets = targets;
}

void BattleContext::ClearFocusTargets()
{
    m_focusTargets.clear();
}

const std::vector<std::shared_ptr<Character>>& BattleContext::GetFocusTargets() const
{
    return m_focusTargets;
}

void BattleContext::SetPredictedCost(int arg_cost)
{
    m_uiPredictedCost = arg_cost;
}

int BattleContext::GetPredictedCost() const
{
    return m_uiPredictedCost;
}

int BattleContext::GetCardIdByGlobalIndex(int globalIndex) const
{
    int currentIndex = 0;
    for (const auto& p : GetAlivePlayers()) {
        int count = p->GetCardCount();
        if (globalIndex < currentIndex + count) {
            // そのプレイヤーのカード範囲内なら、ローカルインデックスでIDを返す
            return p->GetHeldCardId(globalIndex - currentIndex);
        }
        currentIndex += count;
    }
    return -1; // 見つからない場合
}

std::shared_ptr<Character> BattleContext::GetCharacterByCardId(int cardId) const
{
    for (const auto& p : GetAlivePlayers()) {
        for (int i = 0; i < p->GetCardCount(); ++i) {
            if (p->GetHeldCardId(i) == cardId) {
                return p;
            }
        }
    }
    return nullptr;
}

int BattleContext::GetLocalCardIndex(const std::shared_ptr<Character>& actor, int cardId) const
{
    if (!actor)
    {
        return -1;
    }

    for (int i = 0; i < actor->GetCardCount(); ++i) {
        if (actor->GetHeldCardId(i) == cardId)
        {
            return i;
        }
    }
    return -1;
}

bool BattleContext::GetFocusDraw() const
{
    return IsFocusDraw;
}

void BattleContext::SetFocusDraw(bool arg_draw)
{
    IsFocusDraw = arg_draw;
}

void BattleContext::SetTurnPhase(int phase)
{
     m_currentTurnPhase = phase; 
}

int BattleContext::GetTurnPhase() const
{
     return m_currentTurnPhase;
}


