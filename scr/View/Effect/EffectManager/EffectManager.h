#pragma once
#include <vector>
#include <memory>
#include "System/Singleton/Singleton.h"
#include "View/Effect/EffectAnimetion/EffectAnimetion.h"

class EffectManager : public Singleton<EffectManager>
{
private:

	friend class Singleton<EffectManager>;
	// エフェクトのリスト
	std::vector<std::unique_ptr<EffectAnimation>> m_effects;

public:

	/// <summary>
	/// エフェクトを作成して再生
	/// </summary>
	/// <param name="arg_data"></param>
	/// <param name="arg_texture"></param>
	/// <param name="arg_pos"></param>
	void CreateEffect(const std::string& arg_key, sf::Vector2f arg_pos);

	/// <summary>
	/// 全エフェクトの更新
	/// </summary>
	/// <param name="arg_dt"></param>
	void Update(float arg_dt);

	/// <summary>
	/// 全エフェクトの描画
	/// </summary>
	/// <param name="arg_window"></param>
	void Draw(sf::RenderWindow& arg_window);

};

