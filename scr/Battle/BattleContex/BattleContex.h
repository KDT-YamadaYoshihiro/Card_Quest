#pragma once
#include <vector>
#include <memory>
#include "../../Character/Character.h"
#include "../../Card/CardManager/CardManager.h"
#include "../Cost/CostManager.h"

class BattleContext
{
private:

    std::vector<std::shared_ptr<Character>> m_players;
    std::vector<std::shared_ptr<Character>> m_enemies;

public:

    enum class CharacterSide
    {
        Player,
        Enemy
    };

    /// <summary>
    /// 初期化
    /// </summary>
    /// <returns></returns>
    bool Init(const std::vector<std::shared_ptr<Character>>& arg_players,
                const std::vector<std::shared_ptr<Character>>& arg_enemies);


// 取得系 ------------------------------
    /// <summary>
    /// 全プレイヤーの取得
    /// </summary>
    /// <returns></returns>
    const std::vector<std::shared_ptr<Character>>& GetPlayers();

    /// <summary>
    /// 全エネミーの取得
    /// </summary>
    /// <returns></returns>
    const std::vector<std::shared_ptr<Character>>& GetEnemies();
    
    /// <summary>
    /// 生存しているキャラクターの取得
    /// </summary>
    /// <returns></returns>
    std::vector<std::shared_ptr<Character>> GetAlivePlayers() const;

    /// <summary>
    /// 生存しているエネミーの取得
    /// </summary>
    /// <returns></returns>
    std::vector<std::shared_ptr<Character>> GetAliveEnemies() const;

// 判定系 ------------------------------

    /// <summary>
    /// プレイヤーの全滅判定
    /// </summary>
    /// <returns></returns>
    bool IsPlayerAllDead() const;

    /// <summary>
    /// エネミーの全滅判定
    /// </summary>
    /// <returns></returns>
    bool IsEnemyAllDead() const;

    /// <summary>
    /// ターゲット候補の作成
    /// </summary>
    /// <param name="arg_type"></param>
    /// <param name="arg_requesterSide"></param>
    /// <returns></returns>
    std::vector<std::shared_ptr<Character>> CreateTargetCandidates(TargetType arg_type, CharacterSide arg_requesterSide)const;

};