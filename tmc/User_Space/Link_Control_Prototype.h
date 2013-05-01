#pragma once
#include "User_Space_Includes.h"

namespace Link_Control_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Prototypes
	{
		prototype struct Link_Control:public ComponentType
		{
			tag_as_prototype;
			
			feature static void Initialize_Type(TargetType configuration)
			{
				ComponentType::Initialize_Type<ComponentType,CallerType,TargetType>(configuration);
			}

			feature static void Initialize_Type()
			{
				ComponentType::Initialize_Type<ComponentType,CallerType,TargetType>();
			}	

			feature void Initialize(TargetType configuration)
			{
				this_component()->Initialize<ComponentType,CallerType,TargetType>(configuration);
			}

			feature_accessor(x_position, none, none);
			feature_accessor(y_position, none, none);

			feature_accessor(active_properties, none, none);
		};
	}
}

using namespace Link_Control_Components::Prototypes;