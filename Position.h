#pragma once
#include <vector>

struct PositionData
{
	unsigned int territory;
	unsigned int startSize;
};

class Position
{
	std::vector<PositionData> territories;
};

