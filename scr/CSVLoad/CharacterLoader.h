#pragma once
#include <unordered_map>
#include <iostream>
#include "Entity/Character/CharacterData.h"
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
    static CharacterLoader& GetInstance()
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
            data.lv = std::stoi(cols[1]);
            data.name = cols[2];
            data.textureKey = cols[3];
            data.iconKey = cols[4];
            data.hp = std::stoi(cols[5]);
            data.maxHp = std::stoi(cols[5]);
            data.atk = std::stoi(cols[6]);
            data.magicAtk = std::stoi(cols[7]);
            data.def = std::stoi(cols[8]);
            data.exp = std::stoi(cols[9]);

            m_characters[data.charaId] = data;

#ifdef _DEBUG
            std::cout << "Loaded Character ID: " << data.charaId << std::endl;
#endif
        }
        return true;
    }

    const CharacterData* GetData(int id) const
    {
        auto it = m_characters.find(id);

        return (it != m_characters.end()) ? &it->second : nullptr;
    }

};
