#pragma once
#include "Traffic_Simulator_Includes.h"
#include "Activity_Location_Prototype.h"

namespace Zone_Components
{
	namespace Types
	{
	}

	namespace Prototypes
	{
		prototype struct Graphical_Zone_Group ADD_DEBUG_INFO
		{
			tag_as_prototype;

			template<typename CoordType, typename CountType> void accept_zone_information(CoordType coordinates, void* this_ptr, CountType productions, CountType attractions)
			{
				this_component()->template accept_zone_information<CoordType, CountType>(coordinates, this_ptr, productions, attractions);
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

			//For A* star Dijkstra skimming
			accessor(origin_links, NONE, NONE);
			accessor(destination_links, NONE, NONE);
			accessor(origin_walk_links, NONE, NONE);
			accessor(destination_walk_links, NONE, NONE);
			//For A* star Dijkstra skimming

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
			accessor(areatype, NONE, NONE);

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
			accessor(avg_wait_transit, NONE, NONE);
			accessor(avg_ovtt_transit, NONE, NONE);
			accessor(avg_fare_transit, NONE, NONE);
			accessor(avg_distance, NONE, NONE);
			accessor(min_ovtt_transit, NONE, NONE);
			//accessor(avg_ttime_auto_to_transit_accessible_zones, NONE, NONE);
			accessor(avg_ttime_auto_ampeak, NONE, NONE);
			accessor(avg_ttime_auto_pmpeak, NONE, NONE);
			accessor(avg_ttime_auto_offpeak, NONE, NONE);
			accessor(avg_ttime_auto_night, NONE, NONE);
			accessor(avg_ttime_var_auto_ampeak, NONE, NONE);
			accessor(avg_ttime_var_auto_pmpeak, NONE, NONE);
			accessor(avg_ttime_var_auto_offpeak, NONE, NONE);
			accessor(avg_ttime_var_auto_night, NONE, NONE);

			accessor(zone_is_available, NONE, NONE);
			accessor(graphical_zone_group, NONE, NONE);
			accessor(update_increment, NONE, NONE);
			template<typename TargetType> void Initialize()
			{
				this_component()->template Initialize<TargetType>();
			}
			template<typename TargetType> TargetType Get_Random_Location(TargetType excluded_location=nullptr, requires(TargetType,check(TargetType,is_pointer) && check(strip_modifiers(TargetType), Activity_Location_Components::Concepts::Is_Activity_Location)))
			{
				typedef  Activity_Location_Components::Prototypes::Activity_Location<typename remove_pointer< typename get_type_of(origin_activity_locations)::value_type>::type> activity_location_itf;
				typedef Random_Access_Sequence< typename get_type_of(origin_activity_locations),activity_location_itf*> activity_locations_itf;

				activity_locations_itf* locations = this->template origin_activity_locations<activity_locations_itf*>();
				
				int size = (int)locations->size();

                int loc_index = (int)((GLOBALS::Uniform_RNG.template Next_Rand<float>()*0.9999999) * size);

				TargetType return_loc = locations->at(loc_index);

				if (excluded_location==nullptr)	return return_loc;
				else
				{
					int num_attempts=0;
					while (return_loc == excluded_location)
					{
						if (num_attempts == 10)
						{
							THROW_EXCEPTION("ERROR: could not choose a unique random location after 10 attempts.");
						}
						return_loc = Get_Random_Location<TargetType>();
						++num_attempts;
					}
					return return_loc;
				}
			}
			template<typename TargetType> TargetType Get_Random_Work_Location(requires(TargetType,check(TargetType,is_pointer) && check(strip_modifiers(TargetType), Activity_Location_Components::Concepts::Is_Activity_Location)))
			{
				typedef Activity_Location_Components::Prototypes::Activity_Location<typename remove_pointer< typename get_type_of(work_locations)::value_type>::type> activity_location_itf;
				typedef Random_Access_Sequence< typename get_type_of(work_locations),activity_location_itf*> activity_locations_itf;
				
				activity_locations_itf* locations = this->template work_locations<activity_locations_itf*>();
				
				int size = (int)locations->size();
				if (size == 0) return nullptr;

                int loc_index = (int)((GLOBALS::Uniform_RNG.template Next_Rand<float>()*0.9999999) * size);
				return locations->at(loc_index);
			}
			template<typename TargetType> TargetType Get_Random_School_Location(requires(TargetType,check(TargetType,is_pointer) && check(strip_modifiers(TargetType), Activity_Location_Components::Concepts::Is_Activity_Location)))
			{
				typedef Activity_Location_Components::Prototypes::Activity_Location<typename remove_pointer< typename get_type_of(school_locations)::value_type>::type> activity_location_itf;
				typedef Random_Access_Sequence< typename get_type_of(school_locations),activity_location_itf*> activity_locations_itf;
				
				activity_locations_itf* locations = this->template school_locations<activity_locations_itf*>();
				
				int size = (int)locations->size();
				if (size == 0) return nullptr;

                int loc_index = (int)((GLOBALS::Uniform_RNG.template Next_Rand<float>()*0.9999999) * size);
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
				int id = this->template uuid<int>();
				if (areatype<int>() == 1) auto_parking_cost = 15.0;
				else if (areatype<int>() == 2) auto_parking_cost = 5.0;
				else if (areatype<int>() == 3) auto_parking_cost = 1.5;
				else auto_parking_cost = 0.0;

				return GLOBALS::Currency_Converter.Convert_Value<Dollars,TargetType>(auto_parking_cost);
			}

			// features for counting productions and attractions, use TargetType as a reference to set for a specific thread and as a value to return the sum total
            template<typename T> void production_count(T count)
			{
				this_component()->template production_count<T>(count);
			}
			template<typename T> T production_count()
			{
				return this_component()->template production_count<T>();
			}
			template<typename T> void attraction_count(T count)
			{
				this_component()->template attraction_count<T>(count);
			}
			template<typename T> T attraction_count()
			{
				return this_component()->template attraction_count<T>();
			}
			template<typename AreaType, typename ReturnType> ReturnType population_density()
			{
				return this_component()->template population_density<AreaType,ReturnType>();
			}
			accessor(employment_simulated, NONE, NONE);
		};
	}
}

using namespace Zone_Components::Prototypes;
