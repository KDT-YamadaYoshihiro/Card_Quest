#pragma once
#include <unordered_map>
#include "../CharacterData.h"
#include "CSVLoader.h"

class CardLoader : public CSVLoader
{

    std::unordered_map<int, CardData> m_cards;

public:

    bool Load(const std::string& path)
    {
        std::ifstream ifs(path);
        if (!ifs.is_open()) return false;

        std::string line;
        bool first = true;
        while (std::getline(ifs, line))
        {
            if (first) { first = false; continue; } // ヘッダスキップ

            auto cols = Split(line);
            if (cols.size() < 6) continue;

            CardData data;
            data.cardId = std::stoi(cols[0]);
            data.name = cols[1];
            int actionTypeValue = std::stoi(cols[2]);
            data.actionType = static_cast<ActionType>(actionTypeValue);
            data.power = std::stoi(cols[3]);
            int targetTypeValue = std::stoi(cols[4]);
            data.targetType = static_cast<TargetType>(targetTypeValue);
            data.turn = std::stoi(cols[5]);
            data.description = cols[6];

            m_cards[data.cardId] = data;
        }
        return true;
    }

    const CardData* Get(int id) const
    {
        auto it = m_cards.find(id);
        return (it != m_cards.end()) ? &it->second : nullptr;
    }

};
