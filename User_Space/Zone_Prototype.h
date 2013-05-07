#pragma once
#include "User_Space_Includes.h"
#include "Activity_Location_Prototype.h"

namespace Zone_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
		concept struct Is_Zone_Prototype
		{
			check_getter(has_origin_activity_locations, Component_Type::origin_activity_locations);
			check_getter(has_destination_activity_locations, Component_Type::destination_activity_locations);
			check_getter(has_uuid, Component_Type::uuid); 
			check_getter(has_internal_id, Component_Type::internal_id); 
			define_default_check(has_origin_activity_locations && has_destination_activity_locations && has_uuid && has_internal_id);
		};
		concept struct Is_Zone
		{
			check_getter(has_origin_activity_locations, origin_activity_locations);
			check_getter(has_destination_activity_locations, destination_activity_locations);
			check_getter(has_uuid, uuid); 
			check_getter(has_internal_id, internal_id); 

			check_concept(is_zone_prototype, Is_Zone_Prototype);
			define_sub_check(is_zone_component, has_origin_activity_locations && has_destination_activity_locations && has_uuid && has_internal_id);
			define_default_check(is_zone_component || is_zone_prototype);
		};
	}
	
	namespace Prototypes
	{
		prototype struct Graphical_Zone_Group ADD_DEBUG_INFO
		{
			tag_as_prototype;

			feature_prototype void push_zone_information(typename TargetType::ParamType coordinates, void* this_ptr, typename TargetType::Param2Type productions, typename TargetType::Param2Type attractions)
			{
				this_component()->template accept_zone_information<ComponentType,CallerType,TargetType>(coordinates, this_ptr, productions, attractions);
			}
			feature_prototype void configure_zones_layer()
			{
				this_component()->template configure_zones_layer<ComponentType,CallerType,TargetType>();
			}
			feature_accessor(input_offset,none,none);
			feature_accessor(network_bounds,none,none);
			feature_accessor(zone_centroids,none,none);
			//feature_accessor(canvas,none,none);
		};
	
		prototype struct Zone_Prototype ADD_DEBUG_INFO
		{
			tag_as_prototype;

			feature_accessor(origin_activity_locations, none, none);
			feature_accessor(destination_activity_locations, none, none);

			feature_accessor(home_locations, none, none);
			feature_accessor(work_locations, none, none);
			feature_accessor(discretionary_locations, none, none);
			feature_accessor(school_locations, none, none);

			feature_accessor(origin_activity_location_choice_cdfs, none, none);
			feature_accessor(destination_activity_location_choice_cdfs, none, none);
			feature_accessor(uuid, none, none);
			feature_accessor(internal_id, none, none);
			feature_accessor(X,none, none);
			feature_accessor(Y,none, none);
			feature_accessor(population,none, none);
			feature_accessor(households,none, none);
			feature_accessor(group_quarters_population,none, none);
			feature_accessor(employment,none, none);
			feature_accessor(retail_employment,none, none);
			feature_accessor(simulated_employment,none,none);

			feature_accessor(total_area, none, none);
			feature_accessor(low_density_residential_area, none, none);
			feature_accessor(high_density_residential_area, none, none);
			feature_accessor(commercial_area, none, none);
			feature_accessor(industrial_area, none, none);

			feature_accessor(zone_is_available,none,none);
			feature_accessor(graphical_zone_group,none,none);
			feature_accessor(update_increment,none,none);
			feature_prototype void Initialize()
			{
				this_component()->Initialize<ComponentType,CallerType,TargetType>();
			}
			feature_prototype TargetType Get_Random_Location(requires(check_as_given(TargetType,is_pointer) && check(TargetType, Activity_Location_Components::Concepts::Is_Activity_Location)))
			{
				define_container_and_value_interface(activity_locations_itf,activity_location_itf, typename get_type_of(origin_activity_locations),Containers::Random_Access_Sequence_Prototype, Activity_Location_Components::Prototypes::Activity_Location_Prototype,ComponentType);
				activity_locations_itf* locations = this->template origin_activity_locations<activity_locations_itf*>();
				
				int size = locations->size();

#ifndef EXCLUDE_DEMAND
                int loc_index = (int)((GLOBALS::Uniform_RNG.Next_Rand<float>()*0.9999999) * size);
#else
                int loc_index = 0;
                cout << "error: cannot reach here when demand is not enabled!!!" << endl;
                exit(0);
#endif

				return locations->at(loc_index);
			}
			feature_prototype void Push_To_Zone_Display()
			{
				this_component()->template Push_To_Zone_Display<ComponentType,CallerType,TargetType>();
			}
			feature_prototype void reset_counters()
			{
				this_component()->template reset_counters<ComponentType,CallerType,TargetType>();
			}

			// features for counting productions and attractions, use TargetType as a reference to set for a specific thread and as a value to return the sum total
#ifndef EXCLUDE_DEMAND
            feature_accessor(production_count,none,not_available);
            feature_accessor(attraction_count,none,not_available);
#else
            feature_accessor(production_count,none,none);
            feature_accessor(attraction_count,none,none);
#endif
		};
	}
}

using namespace Zone_Components::Prototypes;