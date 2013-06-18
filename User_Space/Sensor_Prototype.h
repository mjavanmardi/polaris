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
			
			feature void Attach_Detector(TargetType detector_configuration)
			{
				return this_component()->template Attach_Detector<ComponentType,CallerType,TargetType>(detector_configuration);
			}

			feature bool Check_Outlier()
			{
				return this_component()->template Check_Outlier<ComponentType,CallerType,TargetType>();
			}

			feature_accessor(covered_link,none,none);
		};

	}
}

using namespace Sensor_Components::Prototypes;