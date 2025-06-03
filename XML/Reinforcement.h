#pragma once
#include "XMLData.h"

class Reinforcement
{
public:
	int lower;
	int upper;
	int divisor;

	Reinforcement(XMLData& xmlData);
};

