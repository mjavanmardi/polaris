#pragma once
#include "User_Space_Includes.h"

namespace Sensor_Components
{
	namespace Types
	{
		struct Reading
		{
			int time;
			double speed;
			double occupancy;
			int volume;
		};
	}

	namespace Concepts
	{
	}
	
	namespace Prototypes
	{
		prototype struct Sensor:public ComponentType
		{
			tag_as_prototype;
			
			feature static void Initialize_Type(TargetType configuration)
			{
				ComponentType::template Initialize_Type<ComponentType,CallerType,TargetType>(configuration);
			}

			feature static void Initialize_Type()
			{
				ComponentType::template Initialize_Type<ComponentType,CallerType,TargetType>();
			}	

			feature void Initialize(TargetType configuration)
			{
				this_component()->template Initialize<ComponentType,CallerType,TargetType>(configuration);
			}
		};

	}
}

using namespace Sensor_Components::Prototypes;