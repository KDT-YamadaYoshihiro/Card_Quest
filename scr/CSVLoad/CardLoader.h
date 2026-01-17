#pragma once
#include <unordered_map>
#include "../Character/CharacterData.h"
#include "../Card/CardDate.h"
#include "CSVLoader.h"

class CardLoader : public CSVLoader
{

    std::unordered_map<int, CardData> m_cards;

    CardLoader() = default;
    virtual ~CardLoader() = default;


public:

    // コピー禁止
    CardLoader(const CardLoader&) = delete;
    CardLoader& operator = (const CardLoader&) = delete;
    static CardLoader& GetInstance()
    {
        static CardLoader instance;
        return instance;
    }


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
            if (cols.size() < 8) continue;

            CardData data;
            data.cardId = std::stoi(cols[0]);
            data.name = cols[1];
            data.cost = std::stoi(cols[2]);
            int actionTypeValue = std::stoi(cols[3]);
            data.actionType = static_cast<ActionType>(actionTypeValue);
            data.power = std::stof(cols[4]);
            int targetTypeValue = std::stoi(cols[5]);
            data.targetType = static_cast<TargetType>(targetTypeValue);
            data.turn = std::stoi(cols[6]);
            data.description = cols[7];

            m_cards[data.cardId] = data;
        }
        return true;
    }

    const CardData* Get(int id) const
    {
        auto it = m_cards.find(id);
        if (it == m_cards.end())
        {
            return nullptr;
        }
        return &it->second;
    }

};
