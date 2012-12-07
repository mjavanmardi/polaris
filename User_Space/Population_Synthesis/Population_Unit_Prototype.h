#pragma once

#include "User_Space\User_Space_Includes.h"

using namespace std;

//---------------------------------------------------------
//	POP_UNIT OBJECT CLASS (i.e. HH, PERSON, VEHICLE, FIRM, etc.)
//---------------------------------------------------------

namespace PopSyn
{
	prototype struct Population_Unit_Prototype
	{
		feature_accessor(ID,check(ReturnValueType,is_arithmetic),check(SetValueType,is_arithmetic));
		feature_accessor(Weight,check(ReturnValueType,is_arithmetic),check(SetValueType,is_arithmetic) && check_2(CallerType,ComponentType,Is_Same_Entity));
		feature_accessor(Index,check(ReturnValueType,is_arithmetic),check(SetValueType,is_arithmetic) && check_2(CallerType,ComponentType,Is_Same_Entity));
		feature_accessor(Characteristics,none,none);
		feature_prototype void Normalize_Weight(TargetType normalization_factor, requires(check(TargetType,is_arithmetic))){_weight = _weight/normalization_factor;}
	};

	implementation struct ADAPTS_Population_Unit_Implementation
	{
		member_data(double,ID,none,none);
		member_data(double,Weight,none,none);
		member_data(uint,index,none,none);				 //index into the joint-distribution matrix of the region (convert using region.get_index())
		member_container(vector<double>, Characteristics,none,none);
	};
}
