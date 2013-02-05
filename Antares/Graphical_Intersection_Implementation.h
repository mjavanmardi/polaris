//*********************************************************
//	Graphical_Intersection_Implementation.h - Graphical Intersection Variant
//*********************************************************

#pragma once
#include "Antares_Includes.h"
#include "User_Space\Intersection_Prototype.h"

namespace Intersection_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{
		implementation struct Graphical_Intersection_Implementation:public Polaris_Component_Class<Graphical_Intersection_Implementation,MasterType,Data_Object,ParentType>
		{
			member_data(int, uuid, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, internal_id, none, none);
			member_data(float, x_position, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(float, y_position, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(float, z_position, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
		};
	}
}

using namespace Intersection_Components::Implementations;
