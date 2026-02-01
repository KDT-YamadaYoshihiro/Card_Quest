#pragma once
#include <string>
#include <array>

struct  StageData
{
	int stageId;
	std::string name;
	std::array<int, 3> enemyIds{};
};

