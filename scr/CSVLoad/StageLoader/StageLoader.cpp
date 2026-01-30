#include "StageLoader.h"

bool StageLoader::LoadCSV(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
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
        data.enemyIds = {
            std::stoi(cols[1]),
            std::stoi(cols[2]),
            std::stoi(cols[3])
        };

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

