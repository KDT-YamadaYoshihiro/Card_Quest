#pragma once
#include <memory>
#include <vector>
#include "../../Character/Character.h"
#include "../../Card/CardDate.h"

struct Action
{
    // s“®Ò
    std::shared_ptr<Character> actor;
    std::shared_ptr<Card> card;
    std::vector<std::shared_ptr<Character>> targets;
};
