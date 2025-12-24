#pragma once
#include <memory>
#include "../Character.h"
#include "../Player/Player.h"
#include "../Enemy/Enemy.h"
#include "../../CSVLoad/CharacterLoader.h"

class CharacterFactory
{


public:

    static CharacterFactory& Instance()
    {
        static CharacterFactory instance;
        return instance;
    }

    // ID から Character を生成
    template <typename T>
    std::shared_ptr<Character> CreateCharacter(int id)
    {
        const CharacterData* src = CharacterLoader::GetInstance().Get(id);
        if (!src)
        {
#ifdef _DEBUG
            std::cout << id << "は存在しないIDです。" << std::endl;
#endif // _DEBUG

            return nullptr; // 存在しない ID
        }

        // CharacterData の deep copy
        CharacterData data = *src;

        // dead フラグは必ず初期化
        data.dead = false;

#ifdef _DEBUG
        std::cout << id << "を生成" << std::endl;
#endif // _DEBUG

        return std::make_unique<T>(data);
       
    }

private:
    CharacterFactory() = default;
    ~CharacterFactory() = default;

    CharacterFactory(const CharacterFactory&) = delete;
    CharacterFactory& operator=(const CharacterFactory&) = delete;
};
