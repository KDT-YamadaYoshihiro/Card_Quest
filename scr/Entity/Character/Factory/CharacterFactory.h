#pragma once
#include <memory>
#include "Entity/Character/Character.h"
#include "Entity/Character/Player/Player.h"
#include "Entity/Character/Enemy/Enemy.h"
#include "CSVLoad/CharacterLoader.h"
#include "View/ConsoleView/ConsoleView.h"

class CharacterFactory : public Singleton<CharacterFactory>
{
private:

    friend class Singleton<CharacterFactory>;

public:

    // ID から Character を生成
    template <typename T>
    std::shared_ptr<Character> CreateCharacter(int id,int maxCardSlot)
    {
        const CharacterData* src = CharacterLoader::GetInstance().GetData(id);
        if (!src)
        {
#ifdef _DEBUG
            ConsoleView::GetInstance().Add("存在しないIDです : " + std::to_string(id) + "\n");
#endif // _DEBUG

            return nullptr; // 存在しない ID
        }

        // CharacterData の deep copy
        CharacterData data = *src;

        // dead フラグは必ず初期化
        data.dead = false;

#ifdef _DEBUG
        std::cout << id << "を生成" << std::endl;
        ConsoleView::GetInstance().Add("キャラクター" + data.name + "を生成" + "\n");

#endif // _DEBUG

        return std::make_shared<T>(data, maxCardSlot);
       
    }

};
