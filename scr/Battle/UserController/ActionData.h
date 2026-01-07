#pragma once
#include <memory>
#include <vector>
#include "../../Character/Character.h"
#include "../../Card/CardDate.h"

struct Action
{
    std::shared_ptr<Character> user;
    std::vector<std::shared_ptr<Character>> targets;
    CardData card;
};
