#pragma once
#include <vector>
#include <memory>
#include "../../Character/Character.h"
#include "../../Character/CharacterData.h"
#include "../../Card/CardDate.h"


class BattleContext
{
private:
    std::vector<std::shared_ptr<Character>> m_players;
    std::vector<std::shared_ptr<Character>> m_enemies;

public:

    // 初期化
    bool Init(const std::vector<std::shared_ptr<Character>>& players,
        const std::vector<std::shared_ptr<Character>>& enemies);
    
    // キャラクターの取得
    const std::vector<std::shared_ptr<Character>>& GetPlayers() const;
    const std::vector<std::shared_ptr<Character>>& GetEnemies() const;

    // 生存しているキャラクターの取得
    std::vector<std::shared_ptr<Character>> GetAlivePlayers() const;
    std::vector<std::shared_ptr<Character>> GetAliveEnemies() const;

    // 全滅判定
    bool IsPlayerAllDead() const;
    bool IsEnemyAllDead() const;

    // ターゲット候補作作成
    std::vector<std::shared_ptr<Character>> CreateTargetCandidates(TargetType targetType, Faction actorFaction, const std::shared_ptr<Character>& actor) const;

};
