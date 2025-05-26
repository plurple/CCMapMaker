#pragma once
#include <vector>

enum class TransformType
{
	turn,
	round
};

enum class ApplyTo
{
	self,
	neutral,
	players,
	opponents,
	others,
	team,
	nonTeam,
	teamMates,
	all
};


enum class ConditionType
{
	round,
	player,
	territory,
	armyCount
};

enum class Operators
{
	equals,
	notEquals,
	greaterThan,
	greaterEquals,
	lessThan,
	lessEquals,
	in,
	notIn
};

struct ConditionData
{
	ConditionType condType;
	unsigned int index;
	Operators oper;
	std::vector<unsigned int> values;
};

class Transform
{
public:
	TransformType transType;
	ApplyTo apply;
	int inc;
	unsigned int amount;
	bool percentage;
	std::vector<ConditionData> conditions;
};

