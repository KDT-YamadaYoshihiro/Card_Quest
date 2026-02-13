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

    // フォーカスターゲット
    std::vector<std::shared_ptr<Character>> m_focusTargets;
    // フォーカスの表示有無
    bool IsFocusDraw = false;
    // フェーズ
    int m_currentTurnPhase = 0;
    // 追加行動数
    int m_uiPredictedCost = 0;
    // 選択カード
	int m_selectedCardId = -1;

public:

    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name="arg_enemies"></param>
    /// <returns></returns>
    bool Init(const std::vector<std::shared_ptr<Character>>& arg_enemies);

    /// <summary>
    /// Partyメンバーのセット
    /// </summary>
    /// <param name="arg_players"></param>
    void SetPlayers(const std::vector<std::shared_ptr<Character>>& arg_players);

    /// <summary>
    /// 生成エネミーのセット
    /// </summary>
    /// <param name="arg_enemyIds"></param>
    void SetEnemyIds(std::vector<int> arg_enemyIds);
    std::vector<int> GetEnemyIds();

    /// <summary>
    /// stageIDのセット・取得
    /// </summary>
    /// <param name="arg_stageId"></param>
    void SetStageId(const int arg_stageId);
    int GetStageId() const;

    /// <summary>
    /// キャラクターのセット
    /// </summary>
    /// <returns></returns>
    const std::vector<std::shared_ptr<Character>>& GetPlayers() const;
    const std::vector<std::shared_ptr<Character>>& GetEnemies() const;

    /// <summary>
    /// 生存しているキャラクターのセット
    /// </summary>
    /// <returns></returns>
    std::vector<std::shared_ptr<Character>> GetAlivePlayers() const;
    std::vector<std::shared_ptr<Character>> GetAliveEnemies() const;


    /// <summary>
    /// 全滅判定
    /// </summary>
    /// <returns></returns>
    bool IsPlayerAllDead() const;
    bool IsEnemyAllDead() const;

    /// <summary>
    /// ターゲット候補の作成
    /// </summary>
    /// <param name="targetType"></param>
    /// <param name="actorFaction"></param>
    /// <param name="actor"></param>
    /// <returns></returns>
    std::vector<std::shared_ptr<Character>> CreateTargetCandidates(TargetType targetType, Faction actorFaction, const std::shared_ptr<Character>& actor) const;


    /// <summary>
    /// コントローラーの現状ホーバー・選択情報の取得
    /// </summary>
    /// <returns></returns>
    int GetHoveredCardIndex() const;
    std::shared_ptr<Character> GetHoveredTarget() const;

    /// <summary>
    /// フォーカスターゲットのセット
    /// </summary>
    /// <param name="targets"></param>
    void SetFocusTargets(const std::vector<std::shared_ptr<Character>>& targets);

    /// <summary>
    /// フォーカスターゲットのリセット
    /// </summary>
    void ClearFocusTargets();

    /// <summary>
    /// フォーカスターゲットの取得
    /// </summary>
    /// <returns></returns>
    const std::vector<std::shared_ptr<Character>>& GetFocusTargets() const;

    /// <summary>
    /// 追加行動数のセット
    /// </summary>
    /// <param name="arg_cost"></param>
    void SetPredictedCost(int arg_cost);

    /// <summary>
    /// 追加行動数の取得
    /// </summary>
    /// <returns></returns>
    int GetPredictedCost() const;

    /// <summary>
    /// UI上の通し番号からカードIDを取得する
    /// </summary>
    int GetCardIdByGlobalIndex(int globalIndex) const;

    /// <summary>
    /// カードIDから所持しているキャラクターを検索する
    /// </summary>
    std::shared_ptr<Character> GetCharacterByCardId(int cardId) const;

    /// <summary>
    /// カードIDからそのキャラ内でのローカルインデックスを取得する
    /// </summary>
    int GetLocalCardIndex(const std::shared_ptr<Character>& actor, int cardId) const;


    /// <summary>
    /// 表示・非表示の取得
    /// </summary>
    /// <returns></returns>
    bool GetFocusDraw() const;

    /// <summary>
    /// 表示・非表示設定
    /// </summary>
    /// <param name="arg_draw"></param>
    void SetFocusDraw(bool arg_draw);

    /// <summary>
    /// フェーズのセット
    /// </summary>
    /// <param name="phase"></param>
    void SetTurnPhase(int phase);

    /// <summary>
    /// フェーズの主得
    /// </summary>
    /// <returns></returns>
    int GetTurnPhase() const;

    /// <summary>
    /// 選択カードのセット
    /// </summary>
    /// <param name="cardId"></param>
	void SetSelectedCardId(int cardId);

    /// <summary>
    /// 選択カードの取得
    /// </summary>
    /// <returns></returns>
	int GetSelectedCardId() const;

};