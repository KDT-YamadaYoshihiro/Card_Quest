#pragma once
#include "../Character.h"
#include "../ChracterSprite/CharacterSprite.h"
#include <random>

class EnemyCharacter : public Character
{
public:

    // ‰Šú‰»
    EnemyCharacter(CharacterData& data, int maxCardSlot);

    // XV
    void Update() override;
    // •`‰æ
    void Render(RenderSystem& render) override;

    // s“®”»’f
    int DecideActionCardIndex() override;
    int DecideTargetIndex(const std::vector<Character*>& targets) override;

private:
    std::mt19937 m_rng;
    std::shared_ptr<CharacterSprite> m_sprite;
};
