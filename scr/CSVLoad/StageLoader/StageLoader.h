#pragma once
#include <unordered_map>
#include <vector>
#include "CSVLoad/CardLoader.h"
#include "System/StageBulid/StageData.h"

class StageLoader : public CSVLoader,public Singleton<StageLoader>
{
private:
    friend class Singleton<StageLoader>;
    std::unordered_map<int, StageData> m_stageTable;

public:

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