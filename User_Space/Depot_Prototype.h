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
			
			feature static void Initialize_Type(TargetType configuration)
			{
				ComponentType::Initialize_Type<ComponentType,CallerType,TargetType>(configuration);
			}
			
			feature void Initialize(TargetType configuration)
			{
				ComponentType::Initialize<ComponentType,CallerType,TargetType>(configuration);
			}

			feature_accessor(covered_area, none, none);
			
			feature_accessor(active_properties, none, none);
		};
	}
}

using namespace Depot_Components::Prototypes;