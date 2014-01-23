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
			check_template_method_name(has_origin_activity_locations, Component_Type::origin_activity_locations);
			check_template_method_name(has_destination_activity_locations, Component_Type::destination_activity_locations);
			check_template_method_name(has_uuid, Component_Type::uuid); 
			check_template_method_name(has_internal_id, Component_Type::internal_id); 
			define_default_check(has_origin_activity_locations && has_destination_activity_locations && has_uuid && has_internal_id);
		};
		concept struct Is_Zone
		{
			check_template_method_name(has_origin_activity_locations, origin_activity_locations);
			check_template_method_name(has_destination_activity_locations, destination_activity_locations);
			check_template_method_name(has_uuid, uuid); 
			check_template_method_name(has_internal_id, internal_id); 

			check_concept(is_zone_prototype, Is_Zone_Prototype, T, V);
			define_sub_check(is_zone_component, has_origin_activity_locations && has_destination_activity_locations && has_uuid && has_internal_id);
			define_default_check(is_zone_component || is_zone_prototype);
		};
	}
	
	namespace Prototypes
	{
		prototype struct Graphical_Zone_Group ADD_DEBUG_INFO
		{
			tag_as_prototype;

			template<typename TargetType> void push_zone_information(typename TargetType::ParamType coordinates, void* this_ptr, typename TargetType::Param2Type productions, typename TargetType::Param2Type attractions)
			{
				this_component()->template accept_zone_information<TargetType>(coordinates, this_ptr, productions, attractions);
			}
			template<typename TargetType> void configure_zones_layer()
			{
				this_component()->template configure_zones_layer<TargetType>();
			}
			accessor(input_offset, NONE, NONE);
			accessor(network_bounds, NONE, NONE);
			accessor(zone_centroids, NONE, NONE);
			//accessor(canvas, NONE, NONE);
		};
	
		prototype struct Zone ADD_DEBUG_INFO
		{
			tag_as_prototype;

			accessor(origin_activity_locations, NONE, NONE);
			accessor(destination_activity_locations, NONE, NONE);

			accessor(home_locations, NONE, NONE);
			accessor(work_locations, NONE, NONE);
			accessor(discretionary_locations, NONE, NONE);
			accessor(school_locations, NONE, NONE);

			accessor(origin_activity_location_choice_cdfs, NONE, NONE);
			accessor(destination_activity_location_choice_cdfs, NONE, NONE);
			accessor(uuid, NONE, NONE);
			accessor(internal_id, NONE, NONE);
			accessor(X, NONE, NONE);
			accessor(Y, NONE, NONE);

			accessor(average_household_income, NONE, NONE);
			accessor(race_percent_white, NONE, NONE);
			accessor(race_percent_black, NONE, NONE);
			accessor(area, NONE, NONE);
			accessor(entertainment_area, NONE, NONE);
			accessor(industrial_area, NONE, NONE);
			accessor(institutional_area, NONE, NONE);
			accessor(mixed_use_area, NONE, NONE);
			accessor(office_area, NONE, NONE);
			accessor(other_area, NONE, NONE);
			accessor(residential_area, NONE, NONE);
			accessor(retail_area, NONE, NONE);
			accessor(school_area, NONE, NONE);
			accessor(pop_households, NONE, NONE);
			accessor(pop_persons, NONE, NONE);
			accessor(pop_group_quarters, NONE, NONE);
			accessor(employment_total, NONE, NONE);
			accessor(employment_retail, NONE, NONE);
			accessor(employment_government, NONE, NONE);
			accessor(employment_manufacturing, NONE, NONE);
			accessor(employment_services, NONE, NONE);
			accessor(employment_industrial, NONE, NONE);
			accessor(employment_other, NONE, NONE);
			accessor(accessibility_employment_retail, NONE, NONE);
			accessor(accessibility_employment_government, NONE, NONE);
			accessor(accessibility_employment_manufacturing, NONE, NONE);
			accessor(accessibility_employment_services, NONE, NONE);
			accessor(accessibility_employment_industrial, NONE, NONE);
			accessor(accessibility_employment_other, NONE, NONE);

			accessor(avg_ttime_transit, NONE, NONE);
			accessor(avg_ttime_auto_to_transit_accessible_zones, NONE, NONE);
			accessor(avg_ttime_auto_peak, NONE, NONE);
			accessor(avg_ttime_auto_offpeak, NONE, NONE);

			accessor(zone_is_available, NONE, NONE);
			accessor(graphical_zone_group, NONE, NONE);
			accessor(update_increment, NONE, NONE);
			template<typename TargetType> void Initialize()
			{
				this_component()->Initialize<TargetType>();
			}
			template<typename TargetType> TargetType Get_Random_Location(requires(TargetType,check(strip_modifiers(TargetType),is_pointer) && check(strip_modifiers(TargetType), Activity_Location_Components::Concepts::Is_Activity_Location)))
			{
				typedef  Activity_Location_Components::Prototypes::Activity_Location<typename remove_pointer< typename get_type_of(origin_activity_locations)::value_type>::type> activity_location_itf;
				typedef Random_Access_Sequence< typename get_type_of(origin_activity_locations),activity_location_itf*> activity_locations_itf;

				activity_locations_itf* locations = this->template origin_activity_locations<activity_locations_itf*>();
				
				int size = (int)locations->size();

#ifndef EXCLUDE_DEMAND
                int loc_index = (int)((GLOBALS::Uniform_RNG.Next_Rand<float>()*0.9999999) * size);
#else
                int loc_index = 0;
                cout << "error: cannot reach here when demand is not enabled!!!" << endl;
                exit(0);
#endif

				return locations->at(loc_index);
			}
			template<typename TargetType> void Push_To_Zone_Display()
			{
				this_component()->template Push_To_Zone_Display<TargetType>();
			}
			template<typename TargetType> void reset_counters()
			{
				this_component()->template reset_counters<TargetType>();
			}

			//TODO: move to zone properties file at some point, this is a temporary parking cost measure which is Chicago Specific
			template<typename TargetType> TargetType Parking_Cost(requires(TargetType,check(strip_modifiers(TargetType),Basic_Units::Concepts::Is_Currency_Value)))
			{
				Basic_Units::Currency_Variables::Dollars auto_parking_cost;
				int id = this->uuid<int>();
				if (id <= 47) auto_parking_cost = 15.0;
				else if (id <= 77) auto_parking_cost = 5.0;
				else if (id <= 350) auto_parking_cost = 1.5;
				else auto_parking_cost = 0.0;

//TODO
//				return Basic_Units::Prototypes::Currency<Basic_Units::Implementations::Currency_Implementation<NT>,NT>::Convert_Value<Target_Type<NT,TargetType,Dollars>>(auto_parking_cost);
			}

			// features for counting productions and attractions, use TargetType as a reference to set for a specific thread and as a value to return the sum total
            accessor(production_count, NONE, NONE);
            accessor(attraction_count, NONE, NONE);
			accessor(employment_simulated, NONE, NONE);
		};
	}
}

using namespace Zone_Components::Prototypes;
