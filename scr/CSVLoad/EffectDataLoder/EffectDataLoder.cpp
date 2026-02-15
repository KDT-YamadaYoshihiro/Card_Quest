#include "EffectDataLoder.h"

void EffectDataLoder::Load(const std::string& filePath)
{
    std::ifstream file(filePath);
    std::string line;
    while (std::getline(file, line)) {
        auto rows = Split(line);
        if (rows.empty()) continue;

        EffectData config;
        config.key = rows[0];
        config.xDivision = std::stoi(rows[1]);
        config.yDivision = std::stoi(rows[2]);
        config.positionType = static_cast<PositionType>(std::stoi(rows[3]));

        m_configs[config.key] = config;
    }
}

const EffectData& EffectDataLoder::GetConfig(const std::string& key) const
{
    return m_configs.at(key);
}
