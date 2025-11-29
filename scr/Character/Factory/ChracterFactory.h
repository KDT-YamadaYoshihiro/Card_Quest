#pragma once
#include <memory>
#include "../CharacterData.h"
#include "../CSVLoad/CharacterLoader.h"

class CharacterFactory
{
public:
    static CharacterFactory& Instance()
    {
        static CharacterFactory instance;
        return instance;
    }

    // IDから新しくキャラクターを生成
    std::unique_ptr<CharacterData> CreateCharacter(int id)
    {
        const CharacterData* src = CharacterLoader::Instance().Get(id);
        if (!src)
        {
            return nullptr; // 存在しないID
        }

        // ディープコピーで新規作成
        auto character = std::make_unique<CharacterData>();

        character->charaId = src->charaId;
        character->name = src->name;
        character->hp = src->hp;
        character->maxHp = src->maxHp;
        character->atk = src->atk;
        character->mp = src->mp;
        character->def = src->def;
        character->cardIds = src->cardIds;  // vectorコピー

        // ObjectPool 用フラグを使用するならここで初期化
        character->dead = true;

        return character;
    }

private:
    CharacterFactory() = default;
    ~CharacterFactory() = default;

    CharacterFactory(const CharacterFactory&) = delete;
    CharacterFactory& operator=(const CharacterFactory&) = delete;
};
