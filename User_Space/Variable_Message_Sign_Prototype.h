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
			
			feature static void Setup_Type(TargetType configuration)
			{
				ComponentType::Setup_Type<ComponentType,CallerType,TargetType>(configuration);
			}

			feature void Setup(TargetType configuration)
			{
				ComponentType::Setup<ComponentType,CallerType,TargetType>(configuration);
			}

			feature_accessor(x_position, none, none);
			feature_accessor(y_position, none, none);

			feature_accessor(active_properties, none, none);
		};
	}
}

using namespace Variable_Message_Sign_Components::Prototypes;