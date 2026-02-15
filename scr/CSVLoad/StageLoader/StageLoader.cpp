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
        if (line.empty()) continue;
        auto rows = Split(line);

        // 必要な列数（ID, Name, Text, BG, Enemy...）が揃っているかチェック
        if (rows.size() < 5) continue;

        try {
            StageData data;
            data.stageId = std::stoi(rows[0]);
            data.name = rows[1];
            data.enemyIds = {
                std::stoi(rows[2]),
                std::stoi(rows[3]),
                std::stoi(rows[4])
            };

            m_stageTable[data.stageId] = data;
        }
        catch (const std::exception& e) {
            // エラーをログに出力して次の行へ
            std::cerr << "StageLoader Error: " << e.what() << " in line: " << line << std::endl;
        }
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

