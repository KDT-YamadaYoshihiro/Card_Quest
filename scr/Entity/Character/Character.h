#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "CharacterData.h"
#include "View/Render/RenderSystem.h"
#include "CSVLoad/TextureLoader/TextureLoader.h"
#include "Entity/Card/CardDate.h"

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
	// カードスロット数
	int m_maxCardSlot;
	// 所持カードIDリスト
	std::vector<int> m_cardIds;
	// 座標
	sf::Vector2f m_pos;
	// フォーカス状態
	bool m_focused;

public:
	virtual ~Character() = default;

	// 更新
	virtual void Update() = 0;
	// 描画
	virtual void Render(RenderSystem& render) = 0;

	// データ参照
	CharacterData& GetData() { return m_data; }
	const CharacterData& GetData() const { return m_data; }
	BuffData& GetBuffData() { return m_buff; }
	const BuffData& GetBuffData() const { return m_buff; }

	// プレイヤーかエネミーか
	Faction GetFaction() const { return m_faction; }

	// 生存判定
	bool IsDead() const { return m_data.dead || m_data.hp <= 0; }

	// ===== カード管理 =====

	// 所持カード数
	int GetCardCount() const;

	// cardId 取得（index指定）
	int GetHeldCardId(int index) const;

	// CardData 取得（index指定）
	const CardData& GetCardData(int index) const;

	// カードの保持枠が空いているか
	bool CanDrawCard() const;

	// カード追加
	void AddCard(int cardId);

	// カードの仕様
	int DiscardCard(int index);

	// 全破棄
	void ClearCards();

	std::vector<int> ClearAndReturnCards();

	// カードドロー
	bool DrawCard();

	// 所持カードを cardId で取得
	int GetHeldCardById(int cardId) const;

	// 
	int DiscardCardById(int cardId);

	// 行動判断
	virtual int DecideActionCardIndex() = 0;
	virtual int DecideTargetIndex(const std::vector<Character*>& targets) = 0;

	// 座標管理
	void SetPosition(const sf::Vector2f& pos) { m_pos = pos; }
	sf::Vector2f GetPosition() const { return m_pos; }

	// フォーカス管理
	void SetFocused(bool focused) { m_focused = focused; }
	bool IsFocused() const { return m_focused; }

	// ダメージを受ける
	void TakeDamage(int damage);
	// 回復する
	void TakeHeal(int heal);
	// バフを受ける
	void TakeBuff(float power, int turn);
	// バフ更新
	void UpdateBuff();
	// バフリセット
	void ResetBuff();
	// レベルアップ
	void LevelUp();
	// バトル開始ステータスリセット
	void ResetBattleStatus();


	//　エネミー用
	virtual void InitEnemyCards() = 0;
	virtual void InitEnemyType() = 0;

};