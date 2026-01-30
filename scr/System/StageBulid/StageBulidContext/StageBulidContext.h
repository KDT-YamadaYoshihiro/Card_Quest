#pragma once
#include <vector>
#include "System/StageBulid/StageData.h"

class StageBulidContext
{
private:

	std::vector<int> m_stageIds;
	int m_selectedStageId = -1;

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// ステージIDの取得
	/// </summary>
	/// <returns></returns>
	const std::vector<int>& GetStageIds() const;

	/// <summary>
	/// 選択ステージ
	/// </summary>
	/// <param name="stageId"></param>
	void SelectStage(int stageId);

	/// <summary>
	/// 選択したステージ情報を取得
	/// </summary>
	/// <returns></returns>
	const StageData* GetSelectedStage() const;
	

};

