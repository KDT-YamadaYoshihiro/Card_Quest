#include "Calculation.h"

float Calculation::GetAddition(float arg_a, float arg_b)
{
	float answer = arg_a + arg_b;
	return answer;
}

float Calculation::GetSubtraction(float arg_a, float arg_b)
{
	float answer = arg_a - arg_b;
	return answer;
}

float Calculation::GetMultiplicative(float arg_a, float arg_b)
{
	float answer = arg_a * arg_b;
	return answer;
}

float Calculation::GetDivision(float arg_a, float arg_b)
{
	float answer = arg_a / arg_b;
	return answer;
}

float Calculation::GetDamage(float arg_atk, float arg_atkBuff, float arg_card, float arg_def, float arg_defBuff)
{
	float damage = (arg_atk * arg_atkBuff * arg_card * K) - (arg_def * arg_defBuff * D);

	// É_ÉÅÅ[ÉWÇÃç≈í·ï€èÿ
	if (damage <= 0)
	{
		damage = 1;
	}

	return damage;
}
