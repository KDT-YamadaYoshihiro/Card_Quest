#include "BattleContex.h"

/// <summary>
/// 初期化
/// </summary>
/// <returns></returns>
bool BattleContext::Init(const std::vector<std::shared_ptr<Character>>& arg_players,
    const std::vector<std::shared_ptr<Character>>& arg_enemies) 
{
    m_players = arg_players;
    m_enemies = arg_enemies;
}

// 取得系 ------------------------------
/// <summary>
/// 全プレイヤーの取得
/// </summary>
/// <returns></returns>
const std::vector<std::shared_ptr<Character>>& BattleContext::GetPlayers()
{
    return m_players;
}

/// <summary>
/// 全エネミーの取得
/// </summary>
/// <returns></returns>
const std::vector<std::shared_ptr<Character>>& BattleContext::GetEnemies()
{
    return m_enemies;
}

/// <summary>
/// 生存しているキャラクターの取得
/// </summary>
/// <returns></returns>
std::vector<std::shared_ptr<Character>> BattleContext::GetAlivePlayers() const
{
    std::vector<std::shared_ptr<Character>> result;
    for (const auto& p : m_players)
    {
        if (!p->GetStatus().dead)
        {
            result.push_back(p);
        }
    }
    return result;
}

/// <summary>
/// 生存しているエネミーの取得
/// </summary>
/// <returns></returns>
std::vector<std::shared_ptr<Character>> BattleContext::GetAliveEnemies() const
{
    std::vector<std::shared_ptr<Character>> result;
    for (const auto& e : m_enemies)
    {
        if (!e->GetStatus().dead) 
        {
            result.push_back(e);
        }
    }
    return result;
}

// 判定系 ------------------------------

    /// <summary>
    /// プレイヤーの全滅判定
    /// </summary>
    /// <returns></returns>
bool BattleContext::IsPlayerAllDead() const
{ 
    for (const auto& p : m_players)
    {
        if (p->GetStatus().dead)
        {
            std::cout << "プレイヤーが全滅しました" << std::endl;
            return true;
        }
    }
    return false;
}

/// <summary>
/// エネミーの全滅判定
/// </summary>
/// <returns></returns>
bool BattleContext::IsEnemyAllDead() const
{
    for (const auto& e : m_enemies)
    {
        if (e->GetStatus().dead)
        {
            std::cout << "エネミーが全滅しました" << std::endl;
            return true;
        }
    }
    return false;
}

/// <summary>
/// ターゲット候補の作成
/// </summary>
/// <param name="arg_type"></param>
/// <param name="arg_requesterSide"></param>
/// <returns></returns>
std::vector<std::shared_ptr<Character>> BattleContext::CreateTargetCandidates(TargetType arg_type, CharacterSide arg_requesterSide)const
{
    switch (arg_type)
    {
    case TargetType::SELF:
        return {}; // ← 呼び出し側で actor を使う

    case TargetType::ALLY:
    case TargetType::ALLY_ALL:

        // ターゲット候補を欲しているのがどっちか判定後
        if (arg_requesterSide == CharacterSide::Player)
        {
            return GetAliveEnemies();
        }
        else {
            return GetAlivePlayers();
        }

    case TargetType::OPPONENT:
    case TargetType::OPPONENT_ALL:

        // ターゲット候補を欲しているのがどっちか判定後
        if(arg_requesterSide == CharacterSide::Player)
        {
            return GetAliveEnemies();
        }
        else {
            return GetAlivePlayers();
        }

    default:
        return {};
    }
}
