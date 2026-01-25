#pragma once
#include "../Character.h"
#include "../ChracterSprite/CharacterSprite.h"

class PlayerCharacter : public Character
{
private:

	// 
	std::shared_ptr<CharacterSprite> m_sprite;

public:
	PlayerCharacter(CharacterData& data, int maxCardSlot);

	// XV
	void Update() override;
	// •`‰æ
	void Render(RenderSystem& render) override;


    int DecideActionCardIndex() override { return 0; }
    int DecideTargetIndex(const std::vector<Character*>&) override { return 0; }


};