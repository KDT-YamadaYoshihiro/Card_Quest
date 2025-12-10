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
		// Player か Enemy かを判定して生成
		if (data.charaId <= 4)
		{
			return std::make_shared<Player>(data);
		}
		else if (data.charaId > 4 && data.charaId <= 6)
		{
			return std::make_shared<Enemy>(data);
        }
        else {
			// 存在しないID
            return nullptr;
        }

    }

private:
    CharacterFactory() = default;
    ~CharacterFactory() = default;

    CharacterFactory(const CharacterFactory&) = delete;
    CharacterFactory& operator=(const CharacterFactory&) = delete;
};
