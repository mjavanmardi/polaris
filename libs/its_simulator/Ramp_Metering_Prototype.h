#pragma once
#include "ITS_Simulator_Includes.h"
//#include "Link_Prototype.h"
//#include "Scenario_Prototype.h"
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
		prototype struct Ramp_Metering ADD_DEBUG_INFO
		{
			tag_as_prototype;

			//ramp
			accessor(internal_id, NONE, NONE);
			accessor(on_ramp_link, NONE, NONE);
			accessor(downstream_freeway_link, NONE, NONE);
			accessor(operation_status, NONE, NONE);

			//detector
			accessor(position_first_detector_on_freeway, NONE, NONE);
			accessor(position_second_detector_on_freeway, NONE, NONE);
			accessor(downstream_freeway_detector_length, NONE, NONE);

			//parameter
			accessor(starting_time, NONE, NONE);	
			accessor(ending_time, NONE, NONE);		
			accessor(metering_updating_interval_length, NONE, NONE);
			accessor(alpha, NONE, NONE); 
			accessor(beta, NONE, NONE);	
			accessor(downstream_freeway_link_occupancy, NONE, NONE);
			

			template<typename TargetType> void ramp_metering_update()
			{
				this_component()->template ramp_metering_update<TargetType>();
			}

			template<typename TargetType> void Initialize()
			{
				this_component()->template Initialize<TargetType>();			
			}
			
			template<typename TargetType> static void Initialize_Type()
			{
				ComponentType::template Initialize_Type<TargetType>();
			}

			template<typename TargetType> bool is_enabled()
			{
				return this_component()->template is_enabled<TargetType>();			
			}
			template<typename TargetType> void enable(bool status)
			{
				this_component()->template enable<TargetType>(status);			
			}
			template<typename TargetType> float meter_flow_ratio()
			{
				return this_component()->template meter_flow_ratio<TargetType>();			
			}
		};
	}
}

using namespace Ramp_Metering_Components::Prototypes;
