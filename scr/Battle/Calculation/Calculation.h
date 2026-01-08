#pragma once
// 計算クラス
class Calculation
{
	// 定数
	constexpr static float K = 1.5f; // ダメージ補正定数
	constexpr static float D = 0.8f; // 防御補正定数
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