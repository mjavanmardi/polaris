#pragma once
#include "ITS_Simulator_Includes.h"

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
		prototype struct Sensor ADD_DEBUG_INFO
		{
			tag_as_prototype;
			
			template<typename TargetType> static void Initialize_Type(TargetType configuration)
			{
				ComponentType::template Initialize_Type<TargetType>(configuration);
			}

			template<typename TargetType> static void Initialize_Type()
			{
				ComponentType::template Initialize_Type<TargetType>();
			}	

			template<typename TargetType> void Initialize(TargetType configuration)
			{
				this_component()->template Initialize<TargetType>(configuration);
			}
			
			template<typename TargetType> void Attach_Detector(TargetType detector_configuration)
			{
				return this_component()->template Attach_Detector<TargetType>(detector_configuration);
			}

			template<typename TargetType> bool Check_Outlier()
			{
				return this_component()->template Check_Outlier<TargetType>();
			}

			accessor(covered_link, NONE, NONE);
		};

	}
}

using namespace Sensor_Components::Prototypes;
