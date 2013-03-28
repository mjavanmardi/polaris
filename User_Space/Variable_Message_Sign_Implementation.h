#pragma once
#include "Variable_Message_Sign_Prototype.h"

namespace Variable_Message_Sign_Components
{
	namespace Types{}

	namespace Concepts{}
	
	namespace Implementations
	{
		implementation struct Variable_Speed_Sign:public Polaris_Component<APPEND_CHILD(Variable_Speed_Sign),MasterType,Data_Object>
		{
			member_data(float, x_position, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(float, y_position, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));

			member_data(vector<string>, active_properties, none, none);
		};

		implementation struct Variable_Word_Sign:public Polaris_Component<APPEND_CHILD(Variable_Word_Sign),MasterType,Data_Object>
		{
			member_data(float, x_position, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(float, y_position, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));

			member_data(vector<string>, active_properties, none, none);
		};
	}
}

using namespace Variable_Message_Sign_Components::Implementations;