#pragma once
#include "Entity/Character/Character.h"
#include "View/ChracterSprite/CharacterSprite.h"
#include <random>

enum class EnemyType
{
    None,
    Slime,
    Bird,
    LittleDragon
};


class EnemyCharacter : public Character
{
private:

    enum enemy
    {
        Slime = 7,
        Bird = 8,
        LittleDragon = 9
    };

    std::mt19937 m_rng;
    std::shared_ptr<CharacterSprite> m_sprite;

    EnemyType m_enemyType;

public:

    // 初期化
    EnemyCharacter(CharacterData& data, int maxCardSlot);

    // タイプの設定
    void InitEnemyType() override;

    // 所持カードの設定
    void InitEnemyCards() override;
    
    // 更新
    void Update() override;
    // 描画
    void Render(RenderSystem& render) override;

    // 行動判断
    int DecideActionCardIndex() override;
    int DecideTargetIndex(const std::vector<Character*>& targets) override;

};
