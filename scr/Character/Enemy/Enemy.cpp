#include "Enemy.h"
#include "../../Card/CardManager/CardManager.h"

EnemyCharacter::EnemyCharacter(CharacterData& data, int maxCardSlot)
    : Character(data, Faction::Enemy, maxCardSlot),
      m_enemyType(EnemyType::None)
{
    std::random_device rd;
    m_rng.seed(rd());
    m_sprite = std::make_shared<CharacterSprite>(m_data.textureKey);
    m_sprite->Init(m_data.textureKey);
}

void EnemyCharacter::InitEnemyType()
{

    switch (m_data.charaId)
    {

    case Slime:
        m_enemyType = EnemyType::Slime;
        break;

    case Bird:
        m_enemyType = EnemyType::Bird;
        break;

    case LittleDragon:
        m_enemyType = EnemyType::LittleDragon;
        break;

    default:
        break;
    }

}

void EnemyCharacter::InitEnemyCards()
{
    // デフォルトでとっしんを持つ
    AddCard(1001);
    // 固有技
    switch (m_enemyType)
    {
    case EnemyType::None:
        break;
    case EnemyType::Slime:
        AddCard(1001);
        break;
    case EnemyType::Bird:
        AddCard(1002);
        break;
    case EnemyType::LittleDragon:
        AddCard(1003);
        break;
    default:
        break;
    }
}

void EnemyCharacter::Update()
{
    // 敵は入力なし
    if (IsDead())
    {
        m_sprite->SetState(CharacterAnimState::DEAD);
    }
    else
    {
        m_sprite->SetState(CharacterAnimState::WAIT);
    }

}

void EnemyCharacter::Render(RenderSystem& render)
{
    m_sprite->SetPosition(m_pos);
    m_sprite->Draw(render);
}

// 使用カードをランダム選択
int EnemyCharacter::DecideActionCardIndex()
{
    if (m_cardIds.empty())
    {
        return -1;
    }

    std::uniform_int_distribution<int> dist(0, static_cast<int>(m_cardIds.size()) - 1);
    return dist(m_rng);
}

// ターゲットをランダム選択
int EnemyCharacter::DecideTargetIndex(const std::vector<Character*>& targets)
{
    if (targets.empty())
    {
        return -1;
    }

    return std::rand() % targets.size();
}
