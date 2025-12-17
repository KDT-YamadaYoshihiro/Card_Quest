#pragma once
#include <unordered_map>
#include <iostream>
#include "../Character/CharacterData.h"
#include "CSVLoader.h"

class CharacterLoader : public CSVLoader
{

    std::unordered_map<int, CharacterData> m_characters;

    CharacterLoader() = default;
    virtual ~CharacterLoader() = default;

public:

    // コピー禁止
    CharacterLoader(const CharacterLoader&) = delete;
    CharacterLoader& operator = (const CharacterLoader&) = delete;
    static CharacterLoader& Instance()
    {
        static CharacterLoader instance;
        return instance;
    }

    bool Load(const std::string& path)
    {
        std::ifstream ifs(path);
        if (!ifs.is_open()) {
			std::cout << "指定するファイルが見つかりません: " << path << std::endl;
            return false;
        }
        std::string line;
        bool first = true;
        while (std::getline(ifs, line))
        {
            if (first) { first = false; continue; } // ヘッダスキップ

            auto cols = Split(line);
            if (cols.size() < 9) continue;

            CharacterData data;
            data.charaId = std::stoi(cols[0]);
            data.name = cols[1];
            data.hp = std::stoi(cols[2]);
            data.maxHp = std::stoi(cols[2]);
            data.atk = std::stoi(cols[3]);
            data.magicAtk = std::stoi(cols[4]);
            data.def = std::stoi(cols[5]);

            // cardId1〜4
            for (int i = 5; i <= 8; i++) {
                data.cardIds.push_back(std::stoi(cols[i]));
            }

            m_characters[data.charaId] = data;
        }
        return true;
    }

    const CharacterData* Get(int id) const
    {
        auto it = m_characters.find(id);
        return (it != m_characters.end()) ? &it->second : nullptr;
    }

};
