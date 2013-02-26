#pragma once
#include "User_Space.h"

namespace Zone_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Prototypes
	{
		prototype struct Graphical_Zone_Group ADD_DEBUG_INFO
		{
			tag_as_prototype;

			feature_prototype void push_zone_information(typename TargetType::ParamType coordinates, typename TargetType::Param2Type productions, typename TargetType::Param2Type attractions)
			{
				this_component()->accept_zone_information<ComponentType,CallerType,TargetType>(coordinates, productions, attractions);
			}
			feature_prototype void configure_zones_layer()
			{
				this_component()->configure_zones_layer<ComponentType,CallerType,TargetType>();
			}
			feature_accessor(input_offset,none,none);
			feature_accessor(network_bounds,none,none);
			feature_accessor(zone_centroids,none,none);
			feature_accessor(canvas,none,none);
		};
	
		prototype struct Zone_Prototype ADD_DEBUG_INFO
		{
			tag_as_prototype;

			feature_accessor(origin_activity_locations, none, none);
			feature_accessor(destination_activity_locations, none, none);
			feature_accessor(origin_activity_location_choice_cdfs, none, none);
			feature_accessor(destination_activity_location_choice_cdfs, none, none);
			feature_accessor(uuid, none, none);
			feature_accessor(internal_id, none, none);
			feature_accessor(X,none, none);
			feature_accessor(Y,none, none);
			feature_accessor(population,none, none);

			feature_accessor(graphical_zone_group,none,none);
			feature_accessor(update_increment,none,none);
			feature_prototype void Initialize()
			{
				this_component()->Initialize<ComponentType,CallerType,TargetType>();
			}
			feature_prototype void Push_To_Zone_Display()
			{
				this_component()->Push_To_Zone_Display<ComponentType,CallerType,TargetType>();
			}

			// features for counting productions and attractions, use TargetType as a reference to set for a specific thread and as a value to return the sum total
			feature_accessor(production_count,none,not_available);
			feature_accessor(attraction_count,none,not_available);
		};
	}
}

using namespace Zone_Components::Prototypes;