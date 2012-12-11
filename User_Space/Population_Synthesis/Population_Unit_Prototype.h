#pragma once

#include "User_Space\User_Space_Includes.h"

using namespace std;

//---------------------------------------------------------
//	POP_UNIT OBJECT CLASS (i.e. HH, PERSON, VEHICLE, FIRM, etc.)
//---------------------------------------------------------

namespace PopSyn
{
	namespace Prototypes
	{
		prototype struct Population_Unit_Prototype
		{
			feature_accessor(ID,check(ReturnValueType,is_arithmetic),check(SetValueType,is_arithmetic));
			feature_accessor(Weight,check(ReturnValueType,is_arithmetic),check(SetValueType,is_arithmetic));
			feature_accessor(Index,check(ReturnValueType,is_arithmetic),check(SetValueType,is_arithmetic));
			feature_accessor(Characteristics,none,none);
			feature_prototype void Normalize_Weight(TargetType normalization_factor, requires(check(TargetType,is_arithmetic)))
			{
				this->Weight<TargetType>(this->Weight<TargetType>()/normalization_factor);
			}
		};
	}
}
