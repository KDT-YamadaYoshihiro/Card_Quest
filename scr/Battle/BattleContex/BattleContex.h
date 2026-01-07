#pragma once
#include <vector>
#include <memory>
#include "../../Character/Character.h"
#include "../../Card/CardManager/CardManager.h"
#include "../Cost/CostManager.h"

class BattleContext
{
public:
    // --- キャラ ---
    const std::vector<std::shared_ptr<Character>>& GetPlayers() const { return m_players; }
    const std::vector<std::shared_ptr<Character>>& GetEnemies() const { return m_enemies; }

    void SetPlayers(const std::vector<std::shared_ptr<Character>>& players)
    {
        m_players = players;
    }

    void SetEnemies(const std::vector<std::shared_ptr<Character>>& enemies)
    {
        m_enemies = enemies;
    }

    // --- 行動中 ---
    void SetActionCharacter(const std::shared_ptr<Character>& c) { m_actionCharacter = c; }
    std::shared_ptr<Character> GetActionCharacter() const { return m_actionCharacter; }

    // --- ターゲット候補 ---
    void SetTargetCandidates(const std::vector<std::shared_ptr<Character>>& targets)
    {
        m_targetCandidates = targets;
    }

    const std::vector<std::shared_ptr<Character>>& GetTargetCandidates() const
    {
        return m_targetCandidates;
    }

private:
    std::vector<std::shared_ptr<Character>> m_players;
    std::vector<std::shared_ptr<Character>> m_enemies;

    std::shared_ptr<Character> m_actionCharacter;
    std::vector<std::shared_ptr<Character>> m_targetCandidates;
};
