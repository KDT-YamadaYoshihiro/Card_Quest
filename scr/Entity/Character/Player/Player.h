#pragma once
#include "Entity/Character/Character.h"
#include "View/ChracterSprite/CharacterSprite.h"

class PlayerCharacter : public Character
{
private:

	// 
	std::shared_ptr<CharacterSprite> m_sprite;

public:
	PlayerCharacter(CharacterData& data, int maxCardSlot);

	// 更新
	void Update() override;
	// 描画
	void Render(RenderSystem& render) override;


    int DecideActionCardIndex() override { return 0; }
    int DecideTargetIndex(const std::vector<Character*>&) override { return 0; }

	// エネミー用なので使用しない
	void InitEnemyCards() override {};
	void InitEnemyType() override {};
};