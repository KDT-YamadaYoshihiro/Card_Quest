#pragma once
#include <vector>
#include "System/StageBulid/StageData.h"
#include "UI/TriangleButton.h"
#include "UI/BoxButton.h"


class StageBulidContext
{
private:

	// ボタンの共有インスタンス
	std::unique_ptr<TriangleButton> m_leftArrow;
	std::unique_ptr<TriangleButton> m_rightArrow;
	std::unique_ptr<BoxButton> m_nextButton;
	std::unique_ptr<BoxButton> m_backButton;

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
	
	/// <summary>
	/// 次のステージへ切り替え
	/// </summary>
	void SelectNextStage();
	
	/// <summary>
	/// 前のステージへ切り替え
	/// </summary>
	void SelectPrevStage();

	// ボタンの設定
	/// <summary>
	/// ステージ切り替え用ボタンの設定
	/// </summary>
	/// <param name="left"></param>
	/// <param name="right"></param>
	void SetNavigationButtons(std::unique_ptr<TriangleButton> left, std::unique_ptr<TriangleButton> right);

	/// <summary>
	/// シーン切り替え用ボタンの設定
	/// </summary>
	/// <param name="nextbutton"></param>
	/// <param name="backButton"></param>
	void SetSceneButton(std::unique_ptr<BoxButton> nextbutton, std::unique_ptr<BoxButton> backButton);

	// ボタンの取得
	// ステージ切り替えボタン
	TriangleButton* GetLeftArrow() const;
	TriangleButton* GetRightArrow() const;
	// シーン切り替えボタン
	BoxButton* GetNextButton() const;
	BoxButton* GetBackButton() const;


};

