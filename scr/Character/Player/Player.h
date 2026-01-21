#pragma once
#include "../Character.h"
#include "../../View/Render/Animetion/Animation.h"

class PlayerCharacter : public Character
{
public:
    PlayerCharacter(CharacterData& data, int maxCardSlot = 1)
        : Character(data, Faction::Player, maxCardSlot) {
    }

	// XV
	void Update() override;
	// •`‰æ
	void Render(RenderSystem& render) override;


    int DecideActionCardIndex() override { return 0; }
    int DecideTargetIndex(const std::vector<Character*>&) override { return 0; }
};