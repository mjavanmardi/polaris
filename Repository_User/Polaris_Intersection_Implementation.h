#pragma once
#include "Repository_User_Includes.h"
#include "Link_Prototype.h"
#include "Vehicle_Prototype.h"
#include "Repository_User\Polaris_Link_Implementation.h"
#include "Intersection_Prototype.h"

namespace Intersection_Components
{
	namespace Types
	{
	
	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{

		implementation struct Polaris_Movement_Implementation
		{
		public:


			member_data_basic(float, turn_travel_penalty);

			member_data_basic(float, movement_capacity);
			member_data_basic(float, movement_supply);
			member_data_basic(float, movement_demand);
			member_data_basic(float, movement_flow);
			member_data_basic(int, movement_transferred);

			member_data_basic(Intersection_Components::Types::Turn_Movement_Type_Keys, movement_type);
			member_data_basic(Intersection_Components::Types::Turn_Movement_Rule_Keys, movement_rule);

			member_data_basic(float, inbound_link_departed_time_based_experienced_link_turn_travel_delay);
			member_data_basic(float, outbound_link_arrived_time_based_experienced_link_turn_travel_delay);
			
			member_data_basic(int, turn_movement_cumulative_arrived_vehicles);
			member_data_basic(int, turn_movement_cumulative_vehicles);

			member_data_basic(int, turn_movement_cumulative_shifted_arrived_vehicles);
			
			member_data_basic(float, minimum_merge_rate);

			member_container_basic(vector<typename MasterType::routable_movement_type*>, replicas_container);


		//==================================================================================================================
		/// forward_link_turn_travel_time
		//------------------------------------------------------------------------------------------------------------------
			typedef Link_Prototype<typename MasterType::routable_link_type, NULLTYPE> replica_interface;
			template<typename ComponentType, typename CallerType, typename TargetType>
			TargetType forward_link_turn_travel_time(){return (TargetType)(_forward_link_turn_travel_time);} tag_getter(forward_link_turn_travel_time);
			template<typename ComponentType, typename CallerType, typename TargetType>
			void forward_link_turn_travel_time(TargetType set_value)
			{
				_forward_link_turn_travel_time = (float)set_value;
				// update replicas
				replicas_container_type::iterator replica_itr;
				for (replica_itr=_replicas_container.begin(); replica_itr!=_replicas_container.end(); replica_itr++)
				{
					replica_interface* replica = (replica_interface*)(*replica_itr);
					//replica->forward_link_turn_travel_time<float>(_forward_link_turn_travel_time);
				}
			}
			tag_setter(forward_link_turn_travel_time);

			float _forward_link_turn_travel_time;

			member_container_basic(vector<float>, cached_outbound_link_arrived_time_based_experienced_link_turn_travel_delay_array);
			member_container_basic(vector<float>, cached_inbound_link_departed_time_based_experienced_link_turn_travel_delay_array);
			member_container_basic(vector<float>, cached_outbound_link_departed_time_based_experienced_link_turn_travel_delay_array);
			member_container_basic(vector<int>, cached_turn_movement_cumulative_shifted_arrived_vehicles_array);

			//typedef deque<typename MasterType::vehicle_type*> vehicles_container_type;
			member_container_basic(deque<typename MasterType::vehicle_type*>, vehicles_container);

			member_component_basic(typename MasterType::link_type, inbound_link);

			member_component_basic(typename MasterType::link_type, outbound_link);

			member_data_basic(int, id);
		};
		


		implementation struct Polaris_Outbound_Inbound_Movements_Implementation
		{


			member_component_basic(typename MasterType::link_type, outbound_link_reference);

			member_container_basic(vector<typename MasterType::movement_type*>, inbound_movements);

		};
		
		implementation struct Polaris_Inbound_Outbound_Movements_Implementation
		{

			member_component_basic(typename MasterType::link_type, inbound_link_reference);

			member_container_basic(vector<typename MasterType::movement_type*>, outbound_movements);

		};
		
		implementation struct Polaris_Intersection_Implementation
		{

			
			//typedef Signal_Components::Interfaces::Signal_Interface<typename MasterType::SIGNAL_TYPE,NULLTYPE> signal_interface;


			//signal_interface* _signal;
			feature_implementation TargetType signal(){return (TargetType)_signal;} tag_getter(signal);
			feature_implementation void signal(TargetType set_value){_signal = (signal_interface*)set_value;} tag_setter(signal);

			member_data_basic(int, uuid);
			member_data_basic(float, x_position);
			member_data_basic(float, y_position);
			member_data_basic(float, z_position);

			member_data_basic(Intersection_Components::Types::Intersection_Type_Keys, intersection_type);

			member_data_basic(Types::Intersection_Simulation_Status, intersection_simulation_status);


			member_container_basic(vector<typename MasterType::link_type*>, inbound_links);


			member_container_basic(vector<typename MasterType::link_type*>, outbound_links);

			member_container_basic(vector<typename MasterType::outbound_inbound_movements_type*>, outbound_inbound_movements);
			member_container_basic(vector<typename MasterType::inbound_outbound_movements_type*>, inbound_outbound_movements);

			member_component_basic(typename MasterType::scenario_type, scenario_reference);


			member_data_basic(RngStream, rng_stream);

			typedef typename MasterType::vehicle_type vehicle_type;

		};
	}

	namespace Components
	{
		template<typename MasterType>
		struct Polaris_Intersection_Component{typedef Polaris_Component_Execution<Implementations::Polaris_Intersection_Implementation, MasterType> type; };

		template<typename MasterType>
		struct Polaris_Movement_Component{typedef Polaris_Component<Implementations::Polaris_Movement_Implementation, MasterType> type; };
		
		template<typename MasterType>
		struct Polaris_Outbound_Inbound_Movements_Component{typedef Polaris_Component<Implementations::Polaris_Outbound_Inbound_Movements_Implementation, MasterType> type; };
		
		template<typename MasterType>
		struct Polaris_Inbound_Outbound_Movements_Component{typedef Polaris_Component<Implementations::Polaris_Inbound_Outbound_Movements_Implementation, MasterType> type; };

	}	

}
