#pragma once
#include <memory>
#include "../Character.h"
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
    std::shared_ptr<Character> CreateCharacter(int id)
    {
        const CharacterData* src = CharacterLoader::Instance().Get(id);
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

        // Character をヒープ上に生成して Shared_ptr に包む
        return std::make_shared<Character>(data);
    }

private:
    CharacterFactory() = default;
    ~CharacterFactory() = default;

    CharacterFactory(const CharacterFactory&) = delete;
    CharacterFactory& operator=(const CharacterFactory&) = delete;
};
