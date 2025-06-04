#pragma once
#include <vector>

enum class TransformType
{
	Turn,
	Round,
	NumTypes
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
	NumApplys
};

enum class IncType
{
	Plus,
	Minus,
	Independent,
	NumIncs
};

enum class ConditionType
{
	Round,
	Player,
	Territory,
	ArmyCount,
	NumTypes
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
	NumOperators
};

struct ConditionData
{
public:
	ConditionType condType;
	int index;
	Operators oper;
	ApplyTo valueOption;
	std::vector<int> values;

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
	std::vector<ConditionData> conditions;

	Transform();
};

