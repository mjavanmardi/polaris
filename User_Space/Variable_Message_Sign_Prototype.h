#pragma once
#include "User_Space_Includes.h"

namespace Variable_Message_Sign_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Prototypes
	{
		prototype struct Variable_Message_Sign:public ComponentType
		{
			tag_as_prototype;

			feature_accessor(x_position, none, none);
			feature_accessor(y_position, none, none);

			feature_accessor(active_properties, none, none);
		};
	}
}

using namespace Variable_Message_Sign_Components::Prototypes;