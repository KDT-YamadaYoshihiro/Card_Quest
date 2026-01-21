#include "Enemy.h"
#include "../../Card/CardManager/CardManager.h"
#include "../../View/Render/RenderSystem.h"

EnemyCharacter::EnemyCharacter(CharacterData& data, int maxCardSlot)
    : Character(data, Faction::Enemy, maxCardSlot)
{
    std::random_device rd;
    m_rng.seed(rd());
}

void EnemyCharacter::Update()
{
    // 敵は入力なし
}

void EnemyCharacter::Render(RenderSystem& render)
{
    // テクスチャ取得
    auto tex = TextureLoader::GetInstance().GetTextureID(m_data.textureKey);
    if (tex)
    {
    	sf::Sprite sprite(*tex);
        sprite.setPosition({ m_pos.x, m_pos.y });
    	render.Draw(sprite);
    }
    else {
#ifdef _DEBUG
    	// テクスチャがない場合は何もしない
    	std::cout << "テクスチャーが見つかりません：" << m_data.textureKey << std::endl;
#endif // _DEBUG
    }
}

// 使用カードをランダム選択
int EnemyCharacter::DecideActionCardIndex()
{
    if (m_cardIds.empty())
        return -1;

    std::uniform_int_distribution<int> dist(0, static_cast<int>(m_cardIds.size()) - 1);
    return dist(m_rng);
}

// ターゲットをランダム選択
int EnemyCharacter::DecideTargetIndex(const std::vector<Character*>& targets)
{
    if (targets.empty())
        return -1;

    std::uniform_int_distribution<int> dist(0, static_cast<int>(targets.size()) - 1);
    return dist(m_rng);
}
