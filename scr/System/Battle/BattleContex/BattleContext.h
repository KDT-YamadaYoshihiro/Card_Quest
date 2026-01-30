#pragma once
#include <vector>
#include <memory>
#include "Entity/Character/Character.h"
#include "Entity/Character/CharacterData.h"
#include "Entity/Card/CardDate.h"


class BattleContext
{
private:

    // キャラクター
    std::vector<std::shared_ptr<Character>> m_players;
    std::vector<std::shared_ptr<Character>> m_enemies;
    
    // エネミーID
    std::vector<int> m_enemyIds;

    // ステージID
    int m_stageId;

public:

    // 初期化
    bool Init(const std::vector<std::shared_ptr<Character>>& arg_enemies);
    
    // プレイヤーメンバーのセット
    void SetPlayers(const std::vector<std::shared_ptr<Character>>& arg_players);

    // エネミーのセット
    void SetEnemyIds(std::vector<int> arg_enemyIds);
    std::vector<int> GetEnemyIds();

    void SetStageId(const int arg_stageId);

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
