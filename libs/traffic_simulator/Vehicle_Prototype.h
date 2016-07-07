#pragma once
#include "User_Space_Includes.h"

namespace Vehicle_Components
{
	namespace Types
	{
		enum Powertrain_Type_Keys
		{
			CONVENTIONAL=0,
			HEV,
			PHEV,
			BEV,
			FCEV
		};
		enum Fuel_Type_Keys
		{
			GASOLINE=0,
			DIESEL,
			CNG,
			H2,
			ELECTRIC
		};
		enum EPA_Vehicle_Class_Keys
		{
			CAR_MINI_COMPACT=0,
			CAR_SUB_COMPACT,
			CAR_COMPACT,
			CAR_MID_SIZE,
			CAR_FULL_SIZE,
			WAGON_COMPACT,
			WAGON_MID_SIZE,
			WAGON_FULL_SIZE,
			SUV_MID_SIZE,
			SUV_FULL_SIZE,
			TRUCK_MID_SIZE,
			TRUCK_FULL_SIZE,
			VAN_MID_SIZE,
			VAN_FULL_SIZE
		};

		enum Vehicle_Type_Keys
		{
			SOV=0,
			HOV,
			TRUCK,
			BUS,
			RAIL,
			BICYCLE,
			WALK
		};

		enum Route_Choice_Use_Class_Keys
		{
			UNRESPONSIVE=0,
			ENROUTE_SWITCHING,
			USER_EQUILIBRIUM,
			SYSTEM_OPTIMAL,
		};
		
		enum Enroute_Information_Keys
		{
			NO_REALTIME_INFORMATION = 0,
			WITH_REALTIME_INFORMATION,
		};

		enum Vehicle_Status_Keys
		{
			UNLOADED=0,
			IN_ENTRY_QUEUE,
			IN_NETWORK,
			OUT_NETWORK,
		};

		struct Load_To_Entry_Queue{};
		struct Load_To_Origin_Link{};
	}

	namespace Concepts
	{
	}
	
	namespace Prototypes
	{

		prototype struct Switch_Decision_Data ADD_DEBUG_INFO
		{
			tag_as_prototype;
			accessor(switch_decision_index, NONE, NONE);
			accessor(route_links_container, NONE, NONE);
		};

		prototype struct Vehicle_Characteristics ADD_DEBUG_INFO
		{
			tag_as_prototype;
			typed_accessor(Types::EPA_Vehicle_Class_Keys, vehicle_class);
			typed_accessor(Types::Powertrain_Type_Keys, powertrain_type);
			typed_accessor(Types::Fuel_Type_Keys, fuel_type);
			typed_accessor(int, ID);
			typed_accessor(int,capacity);
			typed_accessor(bool,has_connectivity);
			typed_accessor(bool,has_cacc);
			typed_accessor(bool,has_acc);
			typed_accessor(bool,has_connected_signal);
			typed_accessor(bool,has_full_automation);
			accessor(length,check(TargetType,Basic_Units::Concepts::Is_Length_Value),check(TargetType,Basic_Units::Concepts::Is_Length_Value));
			accessor(max_speed,check(TargetType,Basic_Units::Concepts::Is_Speed_Value),check(TargetType,Basic_Units::Concepts::Is_Speed_Value));
			accessor(max_accel,check(TargetType,Basic_Units::Concepts::Is_Acceleration_Value),check(TargetType,Basic_Units::Concepts::Is_Acceleration_Value));
			accessor(max_decel,check(TargetType,Basic_Units::Concepts::Is_Acceleration_Value),check(TargetType,Basic_Units::Concepts::Is_Acceleration_Value));
			template<typename TargetType> void initialize(TargetType db_itr)
			{
				this_component()->initialize(db_itr);
			}
		};

		prototype struct Vehicle ADD_DEBUG_INFO
		{
			tag_as_prototype;
			accessor(simulation_status, NONE, NONE);
			accessor(uuid, NONE, NONE);
			accessor(trip_id, NONE, NONE);
			accessor(internal_id, NONE, NONE);
			accessor(movement_plan, NONE, NONE);
			accessor(vehicle_characteristics, NONE, NONE);
			accessor(traveler, NONE, NONE);
			accessor(router, NONE, NONE);
			accessor(distance_to_stop_bar, NONE, NONE);
			accessor(local_speed, NONE, NONE);
			accessor(is_integrated, NONE, NONE);
			accessor(downstream_preferred_departure_time, NONE, NONE);
			accessor(is_autonomous, NONE, NONE);
			accessor(vehicle_ptr, NONE, NONE);

			accessor(cacc_vmt, NONE, NONE);
			
			//enroute swtiching
			accessor(rng_stream, NONE, NONE);
			accessor(enroute_information_type, NONE, NONE);
			accessor(information_compliance_rate, NONE, NONE);
			accessor(relative_indifference_band_route_choice, NONE, NONE);
			accessor(minimum_travel_time_saving, NONE, NONE);
			//accessor(enroute_updated, NONE, NONE);
			accessor(last_enroute_switching_route_check_time, NONE, NONE);
			accessor(entry_queue_length, NONE, NONE);
			accessor(suggested_action, NONE, NONE);

			typed_accessor(bool,write_trajectory);

			template<typename TargetType> void advance()
			{
				typedef  Movement_Plan_Components::Prototypes::Movement_Plan< typename get_type_of(movement_plan)> _Movement_Plan_Interface;
				movement_plan<_Movement_Plan_Interface*>()->template advance_trajectory<NULLTYPE>();
			}

			template<typename TargetType> void load()
			{
				typedef  Movement_Plan_Components::Prototypes::Movement_Plan< typename get_type_of(movement_plan)> _Movement_Plan_Interface;
				movement_plan<_Movement_Plan_Interface*>()->template is_integrated<bool>(is_integrated<bool>());
				this_component()->template load< TargetType>();
			}

			template<typename TargetType> void transfer_to_link(TargetType link)
			{
				this_component()->template transfer_to_link< TargetType>(link);
			}

			template<typename TargetType> void unload()
			{
				this_component()->template unload< TargetType>();

				typedef  Movement_Plan_Components::Prototypes::Movement_Plan< typename get_type_of(movement_plan)> _Movement_Plan_Interface;
				movement_plan<_Movement_Plan_Interface*>()->template is_integrated<bool>(is_integrated<bool>());
				movement_plan<_Movement_Plan_Interface*>()->template arrive_to_destination<NULLTYPE>();	
			}
			
			template<typename TargetType> void enroute_switching(int cause_for_switching)
			{
				this_component()->template enroute_switching< TargetType>(cause_for_switching);
			}

			template<typename TargetType> bool exploit_events_set(TargetType events_set)
			{
				return this_component()->template exploit_events_set< TargetType>(events_set);
			}

			template<typename TargetType> void update_vehicle_position()
			{
				return this_component()->template update_vehicle_position< TargetType>();
			}
			template<typename TargetType> void display_vehicle_position()
			{
				return this_component()->template update_vehicle_position< TargetType>();
			}

			template<typename TargetType> void initialize()
			{
				this_component()->template initialize< TargetType>();
			}
			template<typename TargetType> void initialize(int num_vehicles, float* data)
			{
				this_component()->template initialize< TargetType>(num_vehicles,data);
			}

			template<typename TargetType> void update_eta()
			{
				this_component()->template update_eta<TargetType>();
			}

			template<typename TargetType> void clear_trajectory()
			{
				this_component()->template clear_trajectory<TargetType>();
			}

			template<typename TargetType> void update_vht()
			{
				this_component()->template update_vht<TargetType>();
			}
			template<typename TargetType> void make_suggestion(int action)
			{
				suggested_action<int>(action);
			}
			template<typename TargetType> void take_action()
			{
				this_component()->template take_action<TargetType>();
			}
			template<typename TargetType> void move_to_next_link()
			{
				this_component()->template move_to_next_link<TargetType>();
			}
			template<typename TargetType> void move_to_origin_link()
			{
				this_component()->template move_to_origin_link<TargetType>();
			}
		};
	}
}

using namespace Vehicle_Components::Prototypes;
using namespace Vehicle_Components::Types;
