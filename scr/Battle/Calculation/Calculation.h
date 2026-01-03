#pragma once

// 計算クラス
class Calculation
{
	// 定数
	const float K = 1.50f;		// 攻撃力補正値
	const float D = 0.80f;		// 防御力補正値
	
public:

	// 加法
	static float GetAddition(float arg_a, float arg_b);

	// 減法
	static float GetSubtraction(float arg_a, float arg_b);

	// 乗法
	static float GetMultiplicative(float arg_a, float arg_b);

	// 除法
	static float GetDivision(float arg_a, float arg_b);

	// ダメージ計算
	static float GetDamage(float arg_atk, float arg_card, float arg_def);

};