#pragma once
#include <memory>
#include <vector>
#include "Entity/Character/Character.h"
#include "Entity/Card/CardDate.h"

struct UserAction
{
    // 行動者
    std::shared_ptr<Character> actor;
    int cardId;
    std::vector<std::shared_ptr<Character>> targets;
};

enum class PlayerSelectPhase
{
    SELECT_PLAYER,
    SELECT_CARD,
    CREATE_TARGETS,
    SELECT_TARGET,
    CONFIRM,
    DONE
};

// クリック判定用
struct HitRect
{
    sf::FloatRect rect;
    int index;
};

struct HandCardRect
{
    sf::FloatRect rect;
    int cardIndex;   // 手札配列の index
};