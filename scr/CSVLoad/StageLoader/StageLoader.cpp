#include "StageLoader.h"

bool StageLoader::LoadCSV(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        ConsoleView::GetInstance().Add("指定したファイルが見つかりません :" + path + "\n");
        return false;
    }

    std::string line;
    std::getline(file, line); // ヘッダスキップ

    while (std::getline(file, line))
    {
        auto cols = Split(line);
        if (cols.size() < 4)
        {
            continue;
        }

        StageData data;
        data.stageId = std::stoi(cols[0]);
        data.name = cols[1];
        data.enemyIds = {
            std::stoi(cols[2]),
            std::stoi(cols[3]),
            std::stoi(cols[4])
        };
#ifdef _DEBUG
        ConsoleView::GetInstance().Add("Loaded stageID: " + std::to_string(data.stageId) + "\n");
#endif

        m_stageTable[data.stageId] = data;
    }
    return true;
}


const StageData* StageLoader::GetStageData(int stageId) const
{
	auto it = m_stageTable.find(stageId);
	if (it == m_stageTable.end())
	{
		return nullptr;
	}

	return &it->second;
}

std::vector<int> StageLoader::GetAllStageIds() const
{
    std::vector<int> ids;
    for (auto& [id, _] : m_stageTable)
    {
        ids.push_back(id);
    }

    return ids;
}

