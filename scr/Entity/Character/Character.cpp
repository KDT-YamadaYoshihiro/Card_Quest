#include "Character.h"
#include "entity/Card/CardManager/CardManager.h"

// �����J�[�h��
int Character::GetCardCount() const
{
    return static_cast<int>(m_cardIds.size());
}

// cardId �擾
int Character::GetHeldCardId(int index) const
{
    if (index < 0 || index >= static_cast<int>(m_cardIds.size()))
    {
        std::cout << "�����ȃJ�[�h index\n";
        return -1;
    }
    return m_cardIds[index];
}

// CardData �擾
const CardData& Character::GetCardData(int index) const
{
    int cardId = m_cardIds[index];
    return CardManager::GetInstance().GetCardData(cardId);
}

bool Character::CanDrawCard() const
{
    return static_cast<int>(m_cardIds.size()) < m_maxCardSlot;
}

// �J�[�h�ǉ�
void Character::AddCard(int cardId)
{
    if (static_cast<int>(m_cardIds.size()) >= m_maxCardSlot)
    {
        std::cout << "�J�[�h�X���b�g���\n";
        return;
    }
    m_cardIds.push_back(cardId);
}

// �J�[�h�j��
int Character::DiscardCard(int index)
{
    // �J�[�h�m�F
    if (index < 0 || index >= static_cast<int>(m_cardIds.size()))
    {
        std::cout << "�j�����s�F���� index\n";
        return -1;
    }

    int cardId = m_cardIds[index];
    m_cardIds.erase(m_cardIds.begin() + index);
    return cardId;
}

// �S�j��
void Character::ClearCards()
{
    m_cardIds.clear();
}

// �S�j������ID�ԋp
std::vector<int> Character::ClearAndReturnCards()
{
    std::vector<int> discardedIds = m_cardIds;
    m_cardIds.clear();
    return discardedIds;
}

bool Character::DrawCard()
{
    // ��Ɏ�D�̋󂫂��m�F
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
        std::cout << "Chracter/GetHeldCardById/������ID�ł�" << std::endl;
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
        std::cout << "Chracter/DiscardCardById/������ID�ł�" << std::endl;
        return -1;
    }

    int id = *it;
    m_cardIds.erase(it);
    return id;
}

void Character::SetAnimation(CharacterAnimState state, float duration)
{
    m_animState = state;
    m_animTimer = duration;
    m_animDuration = duration;
}

void Character::UpdateAnimTimer(float arg_dt)
{
    m_totalTime += arg_dt;
    if (m_animTimer > 0.0f) {
        m_animTimer -= arg_dt;
        if (m_animTimer <= 0.0f) {
            m_animState = CharacterAnimState::WAIT; 
        }
    }
}

sf::Vector2f Character::GetVisualPosition() const
{
    sf::Vector2f offset(0.f, 0.f);

    // �ҋ@���̏㉺�h�� (��ɓK�p)
    if (m_animState == CharacterAnimState::WAIT && !IsDead()) {
        offset.y = sin(m_totalTime * 2.0f) * 2.0f; // ���x3.0, �U��5px
    }

    // �A�N�V�������̓���ړ�
    if (m_animTimer > 0.0f) {
        float progress = 1.0f - (m_animTimer / m_animDuration); // 0.0 �` 1.0
        float dir = (m_faction == Faction::Player) ? 1.0f : -1.0f; // ����

        if (m_animState == CharacterAnimState::ATTACK) {
            // �E�֓��ݍ���(30px) �{ �����݂ȗh��(10px)
            float moveX = 30.0f * sin(progress * 3.14f);
            float shake = SHAKE_INTENSITY * sin(progress * 20.0f);
            offset.x = (moveX + shake) * dir;
        }
        else if (m_animState == CharacterAnimState::MAGIC) {
            // ��ւӂ���ƕ���(20px)
            offset.y = -20.0f * sin(progress * 3.14f);
        }
        else if (m_animState == CharacterAnimState::DAMAGE) {
            // ���ւ̂�����
            offset.x = -15.0f * sin(progress * 3.14f) * dir;
        }
    }
    return m_pos + offset;
}

// �_���[�W
void Character::TakeDamage(int damage)
{
    m_data.hp -= damage;
    if (m_data.hp <= 0)
    {
        m_data.hp = 0;
        m_data.dead = true;
    }
}

// ��
void Character::TakeHeal(int heal)
{
    m_data.hp += heal;
    if (m_data.hp > m_data.maxHp)
    {
        m_data.hp = m_data.maxHp;
    }
}

// �o�t
void Character::TakeBuff(float power, int arg_turn)
{
    m_buff.power += power;
	m_buff.turn = arg_turn; 
}

// �o�t�X�V
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

// �o�t���Z�b�g
void Character::ResetBuff()
{
    m_buff.power = 1.0f;
    m_buff.turn = 0;
}

// ���x���A�b�v
void Character::LevelUp()
{
    m_data.lv++;
    m_data.exp = 0;

    m_data.maxHp = static_cast<int>(m_data.maxHp * 1.3f);
    m_data.atk = static_cast<int>(m_data.atk * 1.3f);
    m_data.def = static_cast<int>(m_data.def * 1.3f);
    m_data.magicAtk = static_cast<int>(m_data.magicAtk * 1.3f);
}

// �o�g���J�n�����Z�b�g
void Character::ResetBattleStatus()
{
    m_data.hp = m_data.maxHp;
    m_data.dead = false;
    ResetBuff();
}

// ������
Character::Character(CharacterData& data, Faction faction, int maxCardSlot)
    : m_data(data),
    m_buff(1.0f,0),
    m_faction(faction),
    m_maxCardSlot(maxCardSlot)
{
}
