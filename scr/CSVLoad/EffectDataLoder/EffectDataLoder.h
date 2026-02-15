#pragma once
#include "CSVLoad/CSVLoader.h"
#include "System/Singleton/Singleton.h"
#include <map>

enum class PositionType
{
	Target,
	PlayerSide,
	EnemySide,
	WindowCenter
};

struct EffectData
{
	std::string key;
	PositionType positionType;		// エフェクトの位置タイプ
	float frameDuration;			// フレームの切り替え時間
	int xDivision;				// エフェクトのX分割数
	int yDivision;				// エフェクトのY分割数
};


class EffectDataLoder : public CSVLoader, public Singleton<EffectDataLoder>
{
private:

	friend class Singleton<EffectDataLoder>;

	// エフェクトデータの格納用マップ
	std::map<std::string, EffectData> m_configs;

public:

	/// <summary>
	/// エフェクトデータをCSVファイルから読み込む
	/// </summary>
	/// <param name="filePath"></param>
	void LoadCSV(const std::string& filePath);

	/// <summary>
	/// エフェクトデータの取得
	/// </summary>
	/// <param name="key"></param>
	/// <returns></returns>
	const EffectData& GetConfig(const std::string& key) const;
};

