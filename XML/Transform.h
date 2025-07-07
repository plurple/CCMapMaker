#pragma once
#include <vector>
#include <unordered_map>

enum class TransformOptionType
{
	When,
	Who,
	INC,
	ConditionType,
	Operator,
	COUNT
};

enum class TransformType
{
	Turn,
	Round,
	COUNT
};

enum class ApplyTo
{
	Self,
	Neutral,
	Players,
	Opponents,
	Others,
	Team,
	NonTeam,
	TeamMates,
	All,
	COUNT
};

enum class IncType
{
	Plus,
	Minus,
	Independent,
	COUNT
};

enum class ConditionType
{
	Round,
	Player,
	Territory,
	ArmyCount,
	COUNT
};

enum class Operators
{
	Equals,
	NotEquals,
	GreaterThan,
	GreaterEquals,
	LessThan,
	LessEquals,
	In,
	NotIn,
	COUNT
};

struct ConditionData
{
public:
	ConditionType condType;
	int index;
	Operators oper;
	ApplyTo valueOption;
	std::unordered_map<int, int> values;

	ConditionData();
};

class Transform
{
public:
	TransformType transformType;
	ApplyTo apply;
	IncType inc;
	int amount;
	int upper;
	int lower;
	bool percentage;
	std::unordered_map<int, ConditionData> conditions;

	Transform();
};

