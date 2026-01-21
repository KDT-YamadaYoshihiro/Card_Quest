#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "CharacterData.h"
#include  "../View/Render/RenderSystem.h"
#include "../CSVLoad/TextureLoader/TextureLoader.h"
#include "../Card/CardDate.h"

class RenderSystem;

class Character
{
public:
	virtual ~Character() = default;

	// 更新
	virtual void Update() = 0;
	// 描画
	virtual void Render(RenderSystem& render) = 0;

	// データ参照
	CharacterData& GetData() { return m_data; }
	const CharacterData& GetData() const { return m_data; }

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
	const CardData& GetCard(int index) const;

	// カード追加
	void AddCard(int cardId);

	// カード破棄（index指定）
	// 破棄した cardId を返す
	int DiscardCard(int index);

	// 全破棄
	void ClearCards();

	// カードドロー
	bool DrawCard();

	// 所持カードを cardId で取得
	int GetHeldCardById(int cardId) const;

	// 行動判断
	virtual int DecideActionCardIndex() = 0;
	virtual int DecideTargetIndex(const std::vector<Character*>& targets) = 0;

	// 座標管理
	void SetPosition(const Position& pos) { m_pos = pos; }
	Position GetPosition() const { return m_pos; }

	// フォーカス管理
	void SetFocused(bool focused) { m_focused = focused; }
	bool IsFocused() const { return m_focused; }

	// ダメージを受ける
	void TakeDamage(int damage);
	// 回復する
	void TakeHeal(int heal);
	// バフを受ける
	void TakeBuff(float power);
	// バフ更新
	void UpdateBuff();
	// バフリセット
	void ResetBuff();
	// レベルアップ
	void LevelUp();
	// バトル開始ステータスリセット
	void ResetBattleStatus();

protected:
	
	// 初期化
	Character(CharacterData& data, Faction faction, int maxCardSlot);

	// データ
	CharacterData& m_data;
	Faction m_faction;
	// カードスロット数
	int m_maxCardSlot;
	// 所持カードIDリスト
	std::vector<int> m_cardIds;
	// 座標
	Position m_pos;
	// フォーカス状態
	bool m_focused;
};