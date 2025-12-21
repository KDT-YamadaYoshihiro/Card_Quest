#include "Card.h"
#include "../Character/Character.h"

void Card::UseCard(Character* target)
{

	if (!target)
	{
		return;
	}

	switch (m_data.actionType) {

	case ActionType::ATTCK:

		target->TakeDamage(m_value);

		break;

	case ActionType::MAGIC:

		target->TakeDamage(m_value);

		break;

	case ActionType::HEAL:

		target->TakeHeal(m_value);

		break;

	case ActionType::BUFF:

		target->TakeBuff(m_value);

		break;

	}
}
