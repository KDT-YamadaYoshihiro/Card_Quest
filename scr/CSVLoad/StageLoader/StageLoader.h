#pragma once
#include <unordered_map>
#include <vector>
#include "CSVLoad/CardLoader.h"
#include "System/StageBulid/StageData.h"

class StageLoader : public CSVLoader
{
private:

    std::unordered_map<int, StageData> m_stageTable;

	StageLoader() = default;
	virtual ~StageLoader() = default;

public:

    /// <summary>
    /// コピー禁止
    /// </summary>
    /// <param name=""></param>
    StageLoader(const StageLoader&) = delete;
    StageLoader& operator = (const StageLoader&) = delete;
    static StageLoader& GetInstance()
    {
        static StageLoader instance;
        return instance;
    }

    /// <summary>
    /// 読み込み
    /// </summary>
    /// <param name="path">ファイルパス</param>
    /// <returns>ture:読み込み成功</returns>
    bool LoadCSV(const std::string& path);

    /// <summary>
    /// ステージデータの取得
    /// </summary>
    /// <param name="stageId"></param>
    /// <returns></returns>
    const StageData* GetStageData(int stageId) const;
    
    /// <summary>
    /// すべてのステージIDの取得
    /// </summary>
    /// <returns></returns>
    std::vector<int> GetAllStageIds() const;
};