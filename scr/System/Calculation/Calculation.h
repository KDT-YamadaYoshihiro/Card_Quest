#pragma once
#include <array>

// 計算クラス
class Calculation
{
	// 定数
	constexpr static float K = 1.5f; // ダメージ補正定数
	constexpr static float D = 0.6f; // 防御補正定数
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
	static float GetDamage(float arg_atk, float arg_atkBuff, float arg_card, float arg_def);

	// 平均値
	template<size_t N>
	static int CalcAverageLevel(const std::array<int, N>& levels);
};

template<size_t N>
inline int Calculation::CalcAverageLevel(const std::array<int, N>& levels)
{
	int sum = 0;
	int count = 0;

	for (int lv : levels)
	{
		if (lv > 0)   // 有効レベルのみ
		{
			sum += lv;
			++count;
		}
	}

	if (count == 0)
	{
		return 0;
	}

	return sum / count;
}
