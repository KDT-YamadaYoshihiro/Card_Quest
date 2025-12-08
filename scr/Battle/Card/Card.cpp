#include "Card.h"
#include "../../Character/Character.h"

void Card::Execute(Character* target)
{

	if (!target)
	{
		return;
	}

	switch (m_data.actionType) {

	case ActionType::ATTCK:

		target->TakeDamage(value);

		break;

	case ActionType::MAGIC:

		target->TakeDamage(value);

		break;

	case ActionType::HEAL:

		target->TakeHeal(value);

		break;

	case ActionType::BUFF:

		target->TakeBuff(value);

		break;

	}
}
