#pragma once
#include <unordered_map>
#include "Entity/Character/CharacterData.h"
#include "Entity/Card/CardDate.h"
#include "CSVLoader.h"

class CardLoader : public CSVLoader, public Singleton<CardLoader>
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


    bool LoadCSV(const std::string& path)
    {
        std::ifstream ifs(path);
        if (!ifs.is_open())
        {
            ConsoleView::GetInstance().Add("指定のファイルが見つかりません " + path + "\n");
            return false;
        }

        std::string line;
        bool first = true;
        while (std::getline(ifs, line))
        {
            if (first) 
            {
                first = false; continue; 
            } // ヘッダスキップ

            // もし行の中にダブルクォート付き改行がある場合、閉じるまで読み続ける
            while (std::count(line.begin(), line.end(), '"') % 2 != 0)
            {
                std::string nextLine;
                if (!std::getline(ifs, nextLine)) break; // EOF
                line += "\n" + nextLine; // 改行を含めて結合
            }

            auto cols = Split(line);
            if (cols.size() < 8)
            {
                continue;
            }

            CardData data;
            data.cardId = std::stoi(cols[0]);
            data.name = cols[1];
            data.actionPlus = std::stoi(cols[2]);
            int actionTypeValue = std::stoi(cols[3]);
            data.actionType = static_cast<ActionType>(actionTypeValue);
            data.power = std::stof(cols[4]);
            int targetTypeValue = std::stoi(cols[5]);
            data.targetType = static_cast<TargetType>(targetTypeValue);
            data.turn = std::stoi(cols[6]);
            data.createMax = std::stoi(cols[7]);

           std::string desc = cols[8];
            if (!desc.empty() && desc.front() == '"' && desc.back() == '"')
            {
                desc = desc.substr(1, desc.size() - 2);
            }

            // \n を本当の改行に置換する場合
            size_t pos = 0;
            while ((pos = desc.find("\\n", pos)) != std::string::npos)
            {
                desc.replace(pos, 2, "\n");
            }

            data.description = desc;
            m_cards[data.cardId] = data;

#ifdef _DEBUG
            ConsoleView::GetInstance().Add("Loaded Card ID: " + std::to_string(data.cardId) + "\n");
#endif

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

    const std::unordered_map<int, CardData>& GetAll() const
    {
        return m_cards;
	}

};
