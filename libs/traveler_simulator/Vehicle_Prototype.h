#pragma once
#include "Traveler_Simulator_Includes.h"

namespace Vehicle_Components
{

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
			accessor(db_ptr,NONE,NONE);
			accessor(vehicle_class,NONE,NONE);
			accessor(powertrain_type, NONE, NONE);
			accessor(fuel_type, NONE, NONE);
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
			template<typename TargetType> void initialize(TargetType db_itr);
		};

		template<typename ComponentType>
		template<typename TargetType>
		void Vehicle_Characteristics<ComponentType>::initialize(TargetType db_itr)
		{
			this_component()->initialize(db_itr);
		}

		prototype struct Vehicle ADD_DEBUG_INFO
		{
			tag_as_prototype;
			accessor(simulation_status, NONE, NONE);
			accessor(uuid, NONE, NONE);
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
			accessor(willingness_to_pay, NONE, NONE);
			accessor(vehicle_ptr, NONE, NONE);
			
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

			bool available();

			template<typename TargetType> void advance();

			template<typename TargetType> void load();

			template<typename TargetType> void transfer_to_link(TargetType link);

			template<typename TargetType> void unload();
			
			template<typename TargetType> void enroute_switching(int cause_for_switching);

			template<typename TargetType> bool exploit_events_set(TargetType events_set);

			template<typename TargetType> void update_vehicle_position();
			template<typename TargetType> void display_vehicle_position();

			template<typename TargetType> void initialize();
			template<typename TargetType> void initialize(TargetType characteristics, int hh_id);
			template<typename TargetType> void initialize(int num_vehicles, float* data);

			template<typename PersonItfType> void Assign_To_Person(PersonItfType person_ptr, requires(PersonItfType, check(PersonItfType, is_pointer)));
			void Unassign_From_Person();

			template<typename TargetType> void update_eta();

			template<typename TargetType> void clear_trajectory();

			template<typename TargetType> void update_vht();
			template<typename TargetType> void make_suggestion(int action);
			template<typename TargetType> void take_action();
			template<typename TargetType> void move_to_next_link();
			template<typename TargetType> void move_to_origin_link();
		};

		template<typename ComponentType>
		bool Vehicle<ComponentType>::available()
		{
			return this_component()->available();
		}

		template<typename ComponentType>
		template<typename TargetType>
		void Vehicle<ComponentType>::advance()
		{
			typedef  Movement_Plan_Components::Prototypes::Movement_Plan< typename get_type_of(movement_plan)> _Movement_Plan_Interface;
			movement_plan<_Movement_Plan_Interface*>()->template advance_trajectory<NULLTYPE>();
		}

		template<typename ComponentType>
		template<typename TargetType>
		void Vehicle<ComponentType>::load()
		{
			typedef  Movement_Plan_Components::Prototypes::Movement_Plan< typename get_type_of(movement_plan)> _Movement_Plan_Interface;
			movement_plan<_Movement_Plan_Interface*>()->template is_integrated<bool>(is_integrated<bool>());
			this_component()->template load< TargetType>();
		}

		template<typename ComponentType>
		template<typename TargetType>
		void Vehicle<ComponentType>::transfer_to_link(TargetType link)
		{
			this_component()->template transfer_to_link< TargetType>(link);
		}

		template<typename ComponentType>
		template<typename TargetType>
		void Vehicle<ComponentType>::unload()
		{
			this_component()->template unload< TargetType>();

			typedef  Movement_Plan_Components::Prototypes::Movement_Plan< typename get_type_of(movement_plan)> _Movement_Plan_Interface;
			movement_plan<_Movement_Plan_Interface*>()->template is_integrated<bool>(is_integrated<bool>());
			movement_plan<_Movement_Plan_Interface*>()->arrive_to_destination();
		}

		template<typename ComponentType>
		template<typename TargetType>
		void Vehicle<ComponentType>::enroute_switching(int cause_for_switching)
		{
			this_component()->template enroute_switching< TargetType>(cause_for_switching);
		}

		template<typename ComponentType>
		template<typename TargetType>
		bool Vehicle<ComponentType>::exploit_events_set(TargetType events_set)
		{
			// TODO: this call doesn't really make sense to me
			return this_component()->template exploit_events< boost::container::set< TargetType> >(events_set);
		}

		template<typename ComponentType>
		template<typename TargetType>
		void Vehicle<ComponentType>::update_vehicle_position()
		{
			return this_component()->template update_vehicle_position< TargetType>();
		}
		
		template<typename ComponentType>
		template<typename TargetType>
		void Vehicle<ComponentType>::display_vehicle_position()
		{
			return this_component()->template update_vehicle_position< TargetType>();
		}

		template<typename ComponentType>
		template<typename TargetType>
		void Vehicle<ComponentType>::initialize()
		{
			this_component()->template initialize< TargetType>();
		}
		
		template<typename ComponentType>
		template<typename TargetType>
		void Vehicle<ComponentType>::initialize(TargetType characteristics, int hh_id)
		{
			this_component()->initialize(characteristics, hh_id);
		}
		
		template<typename ComponentType>
		template<typename TargetType>
		void Vehicle<ComponentType>::initialize(int num_vehicles, float* data)
		{
			this_component()->template initialize< TargetType>(num_vehicles, data);
		}

		template<typename ComponentType>
		template<typename PersonItfType>
		void Vehicle<ComponentType>::Assign_To_Person(PersonItfType person_ptr, requires(PersonItfType, check(PersonItfType, is_pointer)))
		{
			this->traveler(person_ptr);
			person_ptr->vehicle(this);
		}
		
		template<typename ComponentType>
		void Vehicle<ComponentType>::Unassign_From_Person()
		{
				this_component()->Unassign_From_Person();
		}

		template<typename ComponentType>
		template<typename TargetType>
		void Vehicle<ComponentType>::update_eta()
		{
			this_component()->template update_eta<TargetType>();
		}

		template<typename ComponentType>
		template<typename TargetType>
		void Vehicle<ComponentType>::clear_trajectory()
		{
			this_component()->template clear_trajectory<TargetType>();
		}

		template<typename ComponentType>
		template<typename TargetType>
		void Vehicle<ComponentType>::update_vht()
		{
			this_component()->template update_vht<TargetType>();
		}
		
		template<typename ComponentType>
		template<typename TargetType>
		void Vehicle<ComponentType>::make_suggestion(int action)
		{
			suggested_action<int>(action);
		}
		
		template<typename ComponentType>
		template<typename TargetType>
		void Vehicle<ComponentType>::take_action()
		{
			this_component()->template take_action<TargetType>();
		}
		
		template<typename ComponentType>
		template<typename TargetType>
		void Vehicle<ComponentType>::move_to_next_link()
		{
			this_component()->template move_to_next_link<TargetType>();
		}
		
		template<typename ComponentType>
		template<typename TargetType>
		void Vehicle<ComponentType>::move_to_origin_link()
		{
			this_component()->template move_to_origin_link<TargetType>();
		}
	}

}

using namespace Vehicle_Components::Prototypes;
using namespace Vehicle_Components::Types;
