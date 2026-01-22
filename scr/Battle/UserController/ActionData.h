#pragma once
#include <memory>
#include <vector>
#include "../../Character/Character.h"
#include "../../Card/CardDate.h"

struct UserAction
{
    // 行動者
    std::shared_ptr<Character> actor;
    int cardId;
    std::vector<int> targetIds;
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