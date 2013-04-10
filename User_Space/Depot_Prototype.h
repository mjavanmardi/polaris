#pragma once
#include "User_Space_Includes.h"

namespace Depot_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Prototypes
	{
		prototype struct Depot:public ComponentType
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

			feature_accessor(covered_area, none, none);
			
			feature_accessor(active_properties, none, none);
		};
	}
}

using namespace Depot_Components::Prototypes;