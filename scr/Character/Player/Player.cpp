#include "Player.h"
#include "../../View/Render/Animetion/Animation.h"

/// <summary>
/// ‰Šú‰»
/// </summary>
/// <param name="data"></param>
/// <param name="maxCardSlot"></param>
PlayerCharacter::PlayerCharacter(CharacterData& data, int maxCardSlot)
	: Character(data, Faction::Player, maxCardSlot)
{
	m_sprite = std::make_shared<CharacterSprite>(data.textureKey);
	m_sprite->Init(data.textureKey);
}

/// <summary>
/// XV
/// </summary>
void PlayerCharacter::Update()
{
	if (IsDead())
	{
		m_sprite->SetState(CharacterAnimState::DEAD);
	}
	else
	{
		m_sprite->SetState(CharacterAnimState::WAIT);
	}
}

/// <summary>
/// •`‰æ
/// </summary>
/// <param name="render"></param>
void PlayerCharacter::Render(RenderSystem& render)
{
	m_sprite->SetPosition(m_pos);
	m_sprite->Draw(render);
}
