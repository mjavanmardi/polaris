#pragma once
#include "Advisory_Radio_Prototype.h"

namespace Advisory_Radio_Components
{
	namespace Types{}

	namespace Concepts{}
	
	namespace Implementations
	{
		implementation struct Highway_Advisory_Radio:public Polaris_Component<APPEND_CHILD(Highway_Advisory_Radio),MasterType,Data_Object>
		{
			member_data(float, x_position, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(float, y_position, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));

			member_data(vector<string>, active_properties, none, none);
		};
	}
}

using namespace Advisory_Radio_Components::Implementations;