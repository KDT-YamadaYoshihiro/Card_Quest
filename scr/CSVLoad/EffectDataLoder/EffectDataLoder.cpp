#include "EffectDataLoder.h"

void EffectDataLoder::LoadCSV(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open()) return;

    std::string line;

    // ヘッダー行を読み飛ばす
    std::getline(file, line);

    while (std::getline(file, line)) {
        // 空行をスキップ
        if (line.empty())
        {
            continue;
        }

        auto rows = Split(line);

        // 列数が足りているかチェック
        if (rows.size() < 5)
        {
            continue;
        }

        try {
            EffectData config;
            config.key = rows[0];
            config.positionType = static_cast<PositionType>(std::stoi(rows[1]));
            config.frameDuration = std::stof(rows[2]);
            config.xDivision = std::stoi(rows[3]);
            config.yDivision = std::stoi(rows[4]);

            m_configs[config.key] = config;
        }
        catch (const std::exception& e) {
            // 変換に失敗した行があればスキップしてログを出す
            std::cerr << "CSV変換エラー (line: " << line << "): " << e.what() << std::endl;
            continue;
        }
    }
}

const EffectData& EffectDataLoder::GetConfig(const std::string& key) const
{
    return m_configs.at(key);
}
