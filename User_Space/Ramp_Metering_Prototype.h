#pragma once
#include "User_Space_Includes.h"
#include "Link_Prototype.h"
#include "Scenario_Prototype.h"
extern void* _global_scenario;

namespace Ramp_Metering_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Prototypes
	{
		prototype struct Ramp_Metering_Prototype
		{
			tag_as_prototype;

			//ramp
			feature_accessor(internal_id, none, none);
			feature_accessor(on_ramp_link, none, none);
			feature_accessor(downstream_freeway_link, none, none);

			//detector
			feature_accessor(position_first_detector_on_freeway, none, none);
			feature_accessor(position_second_detector_on_freeway, none, none);
			feature_accessor(downstream_freeway_detector_length, none, none);

			//parameter
			feature_accessor(starting_time, none, none);
			feature_accessor(ending_time, none, none);
			feature_accessor(metering_updating_interval_length, none, none);
			feature_accessor(alpha, none, none);
			feature_accessor(beta, none, none);
			feature_accessor(downstream_freeway_link_occupancy, none, none);

			feature_prototype void ramp_metering_update()
			{
				this_component()->template ramp_metering_update<ComponentType,CallerType,TargetType>();
			}

			feature_prototype void Initialize()
			{
				this_component()->template Initialize<ComponentType,CallerType,TargetType>();			
			}
		};
	}
}

using namespace Ramp_Metering_Components::Prototypes;