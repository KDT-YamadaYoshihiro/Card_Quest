#pragma once
#include <memory>
#include <vector>
#include "../../Character/Character.h"
#include "../../Card/CardDate.h"

struct UserAction
{
    // çsìÆé“
    std::shared_ptr<Character> actor;
    int cardId;
    std::vector<int> targetIds;
};

enum class PlayerSelectPhase
{
    SELECT_CHARACTER,
    SELECT_CARD,
    SELECT_TARGET,
    CONFIRM,
    NONE
};