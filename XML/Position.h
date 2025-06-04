#pragma once
#include <vector>

struct PositionData
{
public:
	int territory;
	int startSize;

	PositionData() : territory{ -1 }, startSize{ 3 } {};
};

class Position
{
public:
	std::vector<PositionData> positions;

	Position();
};

