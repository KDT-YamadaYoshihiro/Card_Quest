#include "character.h"


// ダメージを与える。
void Character::TakeDamage(int damage)
{
	m_status.hp -= damage;
}

// 回復する。
void Character::TakeHeal(float arg_heal)
{
	m_status.hp += arg_heal;
}

// バフ
void Character::TakeBuff(float arg_power)
{
	m_status.buff.power += arg_power;
}

// バフ更新
void Character::UpdateBuff()
{
	m_status.buff.turn--;
	if (m_status.buff.turn <= 0) {
		m_status.buff.power = 1.0f;
	}
}

// バフリセット
void Character::ResetBuff()
{
	m_status.buff.power = 1.0f;
	m_status.buff.turn = 0;
}

// レベルアップ
void Character::LevelUp()
{
	m_status.lv++;
	m_status.exp = 0;

	m_status.maxHp *= 1.3f;
	m_status.atk *= 1.3f;
	m_status.def *= 1.3f;
	m_status.magicAtk *= 1.3f;
}
