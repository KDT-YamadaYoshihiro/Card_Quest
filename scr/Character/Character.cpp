#include "Character.h"
#include "../Card/CardManager/CardManager.h"

// 所持カード数
int Character::GetCardCount() const
{
    return static_cast<int>(m_cardIds.size());
}

// cardId 取得
int Character::GetHeldCardId(int index) const
{
    if (index < 0 || index >= static_cast<int>(m_cardIds.size()))
    {
        std::cout << "無効なカード index\n";
        return -1;
    }
    return m_cardIds[index];
}

// CardData 取得
const CardData& Character::GetCardData(int index) const
{
    int cardId = m_cardIds[index];
    return CardManager::GetInstance().GetCardData(cardId);
}

bool Character::CanDrawCard() const
{
    return static_cast<int>(m_cardIds.size()) < m_maxCardSlot;
}

// カード追加
void Character::AddCard(int cardId)
{
    if (static_cast<int>(m_cardIds.size()) >= m_maxCardSlot)
    {
        std::cout << "カードスロット上限\n";
        return;
    }
    m_cardIds.push_back(cardId);
}

// カード破棄
int Character::DiscardCard(int index)
{
    // カード確認
    if (index < 0 || index >= static_cast<int>(m_cardIds.size()))
    {
        std::cout << "破棄失敗：無効 index\n";
        return -1;
    }

    int cardId = m_cardIds[index];
    m_cardIds.erase(m_cardIds.begin() + index);
    return cardId;
}

// 全破棄
void Character::ClearCards()
{
    m_cardIds.clear();
}

bool Character::DrawCard()
{
    // 先に手札の空きを確認
    if (!CanDrawCard())
    {
        return false;
    }

    int cardId;
    if (!CardManager::GetInstance().DrawCard(cardId))
    {
        return false;
    }

    m_cardIds.push_back(cardId);
    return true;
}

// 
int Character::GetHeldCardById(int cardId) const
{
    auto it = std::find(m_cardIds.begin(), m_cardIds.end(), cardId);

    if (it == m_cardIds.end())
    {
        std::cout << "Chracter/GetHeldCardById/無効なIDです" << std::endl;
        return -1;
    }

    return static_cast<int>(std::distance(m_cardIds.begin(), it));
}

// 
int Character::DiscardCardById(int cardId)
{
    auto it = std::find(m_cardIds.begin(), m_cardIds.end(), cardId);
    if (it == m_cardIds.end())
    {
        std::cout << "Chracter/DiscardCardById/無効なIDです" << std::endl;
        return -1;
    }

    int id = *it;
    m_cardIds.erase(it);
    return id;
}

// ダメージ
void Character::TakeDamage(int damage)
{
    m_data.hp -= damage;
    if (m_data.hp <= 0)
    {
        m_data.hp = 0;
        m_data.dead = true;
    }
}

// 回復
void Character::TakeHeal(int heal)
{
    m_data.hp += heal;
    if (m_data.hp > m_data.maxHp)
    {
        m_data.hp = m_data.maxHp;
    }
}

// バフ
void Character::TakeBuff(float power)
{
    m_buff.power += power;
}

// バフ更新
void Character::UpdateBuff()
{
    if (m_buff.turn > 0)
    {
        m_buff.turn--;
        if (m_buff.turn <= 0)
        {
            m_buff.power = 1.0f;
        }
    }
}

// バフリセット
void Character::ResetBuff()
{
    m_buff.power = 1.0f;
    m_buff.turn = 0;
}

// レベルアップ
void Character::LevelUp()
{
    m_data.lv++;
    m_data.exp = 0;

    m_data.maxHp = static_cast<int>(m_data.maxHp * 1.3f);
    m_data.atk = static_cast<int>(m_data.atk * 1.3f);
    m_data.def = static_cast<int>(m_data.def * 1.3f);
    m_data.magicAtk = static_cast<int>(m_data.magicAtk * 1.3f);
}

// バトル開始時リセット
void Character::ResetBattleStatus()
{
    m_data.hp = m_data.maxHp;
    m_data.dead = false;
    ResetBuff();
}

// 初期化
Character::Character(CharacterData& data, Faction faction, int maxCardSlot)
    : m_data(data),
    m_buff(1.0f,0),
    m_faction(faction),
    m_maxCardSlot(maxCardSlot)
{
}
