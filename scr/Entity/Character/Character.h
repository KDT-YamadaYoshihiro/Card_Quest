#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "CharacterData.h"
#include "View/Render/RenderSystem.h"
#include "CSVLoad/TextureLoader/TextureLoader.h"
#include "Entity/Card/CardDate.h"
#include "View/ChracterSprite/CharacterSprite.h"


class RenderSystem;

class Character
{
protected:

	// 初期化
	Character(CharacterData& data, Faction faction, int maxCardSlot);

	// データ
	CharacterData m_data;
	BuffData m_buff;
	Faction m_faction;
	// 所持カードIDリスト
	std::vector<int> m_cardIds;
	// 座標
	sf::Vector2f m_pos;
	// Animationステート
	CharacterAnimState m_animState = CharacterAnimState::WAIT;
	// フォーカス状態
	bool m_focused;
	// 残り時間
	float m_animTimer = 0.0f;
	// 総時間（進捗計算用）
	float m_animDuration = 0.0f;
	// 累計経過時間（待機揺れ用）
	float m_totalTime = 0.0f;
	// カードスロット数
	int m_maxCardSlot;


public:
	virtual ~Character() = default;

	// 更新
	virtual void Update() = 0;
	// 描画
	virtual void Render(RenderSystem& render) = 0;

	/// <summary>
	/// データ参照
	/// </summary>
	/// <returns></returns>
	CharacterData& GetData() { return m_data; }
	const CharacterData& GetData() const { return m_data; }
	BuffData& GetBuffData() { return m_buff; }
	const BuffData& GetBuffData() const { return m_buff; }

	/// <summary>
	/// プレイヤーかエネミーか
	/// </summary>
	/// <returns></returns>
	Faction GetFaction() const { return m_faction; }

	/// <summary>
	/// 生存判定
	/// </summary>
	/// <returns></returns>
	bool IsDead() const { return m_data.dead || m_data.hp <= 0; }

	// ===== カード管理 =====

	/// <summary>
	/// 所持カード数
	/// </summary>
	/// <returns></returns>
	int GetCardCount() const;

	/// <summary>
	/// cardIdの取得
	/// </summary>
	/// <param name="index">要素数</param>
	/// <returns></returns>
	int GetHeldCardId(int index) const;

	/// <summary>
	/// CardDataの取得
	/// </summary>
	/// <param name="index">要素数</param>
	/// <returns></returns>
	const CardData& GetCardData(int index) const;

	/// <summary>
	/// カードの保存枠が空いているか
	/// </summary>
	/// <returns></returns>
	bool CanDrawCard() const;

	/// <summary>
	/// カードの追加
	/// </summary>
	/// <param name="cardId"></param>
	void AddCard(int cardId);

	/// <summary>
	/// カードの使用
	/// </summary>
	/// <param name="index"></param>
	/// <returns></returns>
	int DiscardCard(int index);

	/// <summary>
	/// 全破棄
	/// </summary>
	void ClearCards();

	/// <summary>
	/// 所持カードを返却
	/// </summary>
	/// <returns></returns>
	std::vector<int> ClearAndReturnCards();

	/// <summary>
	/// カードドロー
	/// </summary>
	/// <returns></returns>
	bool DrawCard();

	/// <summary>
	/// 所持カードを取得
	/// </summary>
	/// <param name="cardId">カードID</param>
	/// <returns></returns>
	int GetHeldCardById(int cardId) const;

	/// <summary>
	/// カードの破棄
	/// </summary>
	/// <param name="cardId">カードID</param>
	/// <returns></returns>
	int DiscardCardById(int cardId);

	/// <summary>
	/// 使用カードの決定
	/// </summary>
	/// <returns></returns>
	virtual int DecideActionCardIndex() = 0;
	/// <summary>
	/// ターゲットの決定
	/// </summary>
	/// <param name="targets"></param>
	/// <returns></returns>
	virtual int DecideTargetIndex(const std::vector<Character*>& targets) = 0;

	/// <summary>
	/// 座標管理
	/// </summary>
	/// <param name="pos"></param>
	void SetPosition(const sf::Vector2f& pos) { m_pos = pos; }
	sf::Vector2f GetPosition() const { return m_pos; }

	/// <summary>
	/// フォーカス管理
	/// </summary>
	/// <param name="focused"></param>
	void SetFocused(bool focused) { m_focused = focused; }
	bool IsFocused() const { return m_focused; }

	/// <summary>
	/// Animationステートセット
	/// </summary>
	void SetAnimation(CharacterAnimState state, float duration);
	/// <summary>
	/// Animationステートの取得
	/// </summary>
	/// <returns></returns>
	CharacterAnimState GetAnimState() const { return m_animState; }

	/// <summary>
	/// Animationフレーム更新
	/// </summary>
	/// <param name="arg_dt"></param>
	void UpdateAnimTimer(float arg_dt);

	/// <summary>
	/// 描画用座標の計算
	/// </summary>
	/// <returns></returns>
	sf::Vector2f GetVisualPosition
	() const;


	/// <summary>
	/// ダメージ受け取る
	/// </summary>
	/// <param name="damage">ダメージ量</param>
	void TakeDamage(int damage);
	/// <summary>
	/// 回復する
	/// </summary>
	/// <param name="heal">回復量</param>
	void TakeHeal(int heal);
	/// <summary>
	/// バフを受ける
	/// </summary>
	/// <param name="power">バフ量</param>
	/// <param name="turn">バフ継続機関</param>
	void TakeBuff(float power, int turn);
	/// <summary>
	/// バフ更新
	/// </summary>
	void UpdateBuff();
	/// <summary>
	/// バフリセット
	/// </summary>
	void ResetBuff();
	/// <summary>
	/// レベルアップ
	/// </summary>
	void LevelUp();
	/// <summary>
	/// バトル開始時のリセット
	/// </summary>
	void ResetBattleStatus();

	//　エネミー用
	virtual void InitEnemyCards() = 0;
	virtual void InitEnemyType() = 0;

};