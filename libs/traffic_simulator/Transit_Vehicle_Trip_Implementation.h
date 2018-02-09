#pragma once
#include "Transit_Vehicle_Trip_Prototype.h"
#include "../repository/RNG_Implementations.h"

namespace Transit_Vehicle_Trip_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}

	namespace Implementations
	{
		/*struct Movement_MOE_Data
		{
			float movement_flow_rate;
			float turn_penalty;
			float turn_penalty_standard_deviation;
			float inbound_link_turn_time;
			float outbound_link_turn_time;
		};*/

		implementation struct Transit_Vehicle_Trip_Implementation :public Polaris_Component<MasterType, INHERIT(Transit_Vehicle_Trip_Implementation), Execution_Object>
		{
			typedef typename Polaris_Component<MasterType, INHERIT(Transit_Vehicle_Trip_Implementation), Execution_Object>::Component_Type ComponentType;

			typedef Person_Components::Prototypes::Person<typename MasterType::person_type> _Person_Interface;
			typedef Network_Components::Prototypes::Network<typename MasterType::network_type> _Network_Interface;
			typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;
			typedef Transit_Pattern_Components::Prototypes::Transit_Pattern<typename MasterType::transit_pattern_type> _Transit_Pattern_Interface;
			typedef Link_Components::Prototypes::Link<typename MasterType::link_type> _Link_Interface;
			

			m_data(int, internal_id, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(std::string, dbid, NONE, NONE);
			m_data(int, uuid, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(int, direction, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_prototype(Transit_Pattern_Components::Prototypes::Transit_Pattern, typename MasterType::transit_pattern_type, pattern, NONE, NONE);
			m_prototype(Network_Components::Prototypes::Network, typename MasterType::network_type, network_reference, NONE, NONE);
			m_container(std::vector<int>, arrival_seconds, NONE, NONE);
			m_container(std::vector<int>, departure_seconds, NONE, NONE);
			
			//Simulation related
			m_container(std::vector<_Person_Interface*>, people_on_board, NONE, NONE);
			m_data(int, current_position, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			//----------------------------------------------------------------------

			typedef  Intersection_Components::Prototypes::Intersection<typename remove_pointer< typename type_of(network_reference)::get_type_of(intersections_container)::value_type>::type>  _Intersection_Interface;
			typedef  Random_Access_Sequence< typename type_of(network_reference)::get_type_of(intersections_container), _Intersection_Interface*> _Intersections_Container_Interface;

			typedef  typename _Transit_Pattern_Interface::get_type_of(pattern_stops) _Pattern_Stops_Container_Interface;
			typedef  typename _Transit_Pattern_Interface::get_type_of(pattern_links) _Pattern_Links_Container_Interface;

			//template<typename TargetType> void schdeule_vehicle_movements_in_transit_network();
			template<typename TargetType> void advance();
			template<typename TargetType> void load();
			template<typename TargetType> void transfer_to_link(TargetType link);
			template<typename TargetType> void unload();
			template<typename TargetType> void clear_trajectory();
			template<typename TargetType> void move_to_next_link();
			template<typename TargetType> void move_to_origin_link();

			

			template<typename TargetType> void schdeule_vehicle_movements_in_transit_network(void* network)
			{
				_network_reference = (network_reference_type)network;				

				this->template current_position<int>((int)0);

				int load_time = this->template arrival_seconds<std::vector<int>>()[0];


				
				this->template Load_Event<ComponentType>(&ComponentType::move_transit_vehicles_in_transit_network_conditional, load_time, Scenario_Components::Types::Transit_Sub_Iteration_keys::TRANSIT_VEHICLE_ARRIVING_SUBITERATION);

				/*typename _Pattern_Links_Container_Interface::iterator links_itr;
				_Pattern_Links_Container_Interface& pattern_links = pattern->template pattern_links<_Pattern_Links_Container_Interface&>();

				for (links_itr = pattern_links.begin(); links_itr != pattern_links.end(); ++links_itr)
				{
					_Link_Interface* pattern_link = (_Link_Interface*)(*links_itr);

					pattern_link->template Load_Event<ComponentType>(&ComponentType::move_transit_vehicles_in_transit_network_conditional, ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>() - 1, Scenario_Components::Types::Transit_Sub_Iteration_keys::TRANSIT_VEHICLE_ARRIVING_SUBITERATION);
				}*/

			}

			static void move_transit_vehicles_in_transit_network_conditional(ComponentType* _this, Event_Response& response) 
			{
				if (sub_iteration() == Scenario_Components::Types::Transit_Sub_Iteration_keys::TRANSIT_VEHICLE_ARRIVING_SUBITERATION)
				{
					_this->transit_vehicle_arriving();
					response.next._iteration = iteration();
					response.next._sub_iteration = Scenario_Components::Types::Transit_Sub_Iteration_keys::TRAVELER_ALIGHTING_SUBITERATION;					
				}
			}

			void transit_vehicle_arriving()
			{
				_Transit_Pattern_Interface* pattern = this->template pattern<_Transit_Pattern_Interface*>(); 
				
				typename _Pattern_Links_Container_Interface::iterator links_itr;
				_Pattern_Links_Container_Interface& pattern_links = pattern->template pattern_links<_Pattern_Links_Container_Interface&>();

				std::string trip_ID = this->template dbid<std::string>();
				int position = this->template current_position<int>();
				int arrival_time = this->template arrival_seconds<std::vector<int>>()[position];

				_Link_Interface* pattern_link = (_Link_Interface*)(pattern_links[position]);

				std::string pattern_ID  = pattern->template dbid<std::string>();
				std::string stop_ID = pattern_link->template upstream_intersection<_Intersection_Interface*>()->template dbid<std::string>();
				
				cout << "I am trip:\t" << trip_ID << "\t";
				cout << "At position:\t" << position << "\t";
				cout << "TIme is:\t" << arrival_time << "\t";
				cout << "Pattern is:\t" << pattern_ID << "\t";
				cout << "Stop is:\t" << stop_ID << endl;
			}


			

		};

	}
}

