#pragma once
#include "Link_Control_Prototype.h"

namespace Link_Control_Components
{
	namespace Types{}

	namespace Concepts{}
	
	namespace Implementations
	{
		implementation struct Lane_Link_Control:public Polaris_Component<APPEND_CHILD(Lane_Link_Control),MasterType,Data_Object>
		{
			member_data(float, x_position, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(float, y_position, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));

			member_data(vector<string>, active_properties, none, none);
		};
	}
}

using namespace Link_Control_Components::Implementations;