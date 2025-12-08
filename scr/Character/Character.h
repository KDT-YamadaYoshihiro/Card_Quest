#pragma once
#include "CharacterData.h"
#include "../Battle/Card/CardPool.h"

class RenderSystem;

class Character
{
	
	CharacterData m_status;
	Position m_pos;
	CardPool m_cardPool;

public:

	// 初期化
	Character(CharacterData& arg_data)
		:m_status(arg_data),
		m_pos({0,0})
	{

	}


	// 更新
	virtual void Update() = 0;
	// 描画
	virtual void Render(RenderSystem& render) = 0;
	// アクションメソッド
	virtual void Action() = 0;

	// 死亡
	void CharaDead() {
		// ０以上ならreturnする
		if (m_status.hp > 0)
		{
			return;
		}
		// 死亡
		m_status.dead = true;
	}

	// ダメージを与える。
	void TakeDamage(int damage);

	// 回復する。
	void TakeHeal(float arg_heal);

	// バフ
	void TakeBuff(float arg_power);

	// バフ更新
	void UpdateBuff();

	// バフリセット
	void ResetBuff();

	// レベルアップ
	void LevelUp();

	// バトル開始ステータス
	void StartStatus() {
		m_status.hp = m_status.maxHp;
		ResetBuff();
	}

	// ステータス取得
	CharacterData GetStatus() const { return m_status; }
	// 座標の取得
	Position GetPos() const { return m_pos; }
	CardPool GetCardPool() const { return m_cardPool; }
};