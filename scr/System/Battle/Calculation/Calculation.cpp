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

float Calculation::GetDamage(float arg_atk, float arg_card, float arg_def)
{
	float damage = (arg_atk * arg_card * K) - (arg_def * D);
	return damage;
}
