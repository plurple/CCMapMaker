#include "Transform.h"

Transform::Transform() :
	transformType{ TransformType::Turn },
	apply{ ApplyTo::Self },
	inc{ IncType::Plus },
	amount{ -1 },
	upper{ -1 },
	lower{ -1 },
	percentage{ false }
{
}

//-----------------------------------------------------------

ConditionData::ConditionData() :
	condType{ ConditionType::Territory },
	index{ -1 },
	oper{ Operators::Equals },
	valueOption{ ApplyTo::Self }
{
	values.push_back(-1);
}
