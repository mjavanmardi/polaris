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
			m_data(int, number_of_stops, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			
			//Simulation related
			m_container(std::vector<_Person_Interface*>, people_on_board, NONE, NONE);
			m_data(int, current_position, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			//----------------------------------------------------------------------

			typedef  Intersection_Components::Prototypes::Intersection<typename remove_pointer< typename type_of(network_reference)::get_type_of(intersections_container)::value_type>::type>  _Intersection_Interface;
			typedef  Random_Access_Sequence< typename type_of(network_reference)::get_type_of(intersections_container), _Intersection_Interface*> _Intersections_Container_Interface;

			typedef  typename _Transit_Pattern_Interface::get_type_of(pattern_stops) _Pattern_Stops_Container_Interface;
			typedef  typename _Transit_Pattern_Interface::get_type_of(pattern_links) _Pattern_Links_Container_Interface;
			
			

			template<typename TargetType> void schdeule_vehicle_movements_in_transit_network(void* network)
			{
				_network_reference = (network_reference_type)network;				

				this->template current_position<int>((int)0);

				int arrival_time = this->template arrival_seconds<std::vector<int>>()[0];

				this->template Load_Event<ComponentType>(&ComponentType::move_transit_vehicles_in_transit_network_conditional, arrival_time, Scenario_Components::Types::Transit_Sub_Iteration_keys::TRANSIT_VEHICLE_ARRIVING_SUBITERATION);
			}

			static void move_transit_vehicles_in_transit_network_conditional(ComponentType* _this, Event_Response& response) 
			{
				if (sub_iteration() == Scenario_Components::Types::Transit_Sub_Iteration_keys::TRANSIT_VEHICLE_ARRIVING_SUBITERATION)
				{
					_this->transit_vehicle_arriving();
					response.next._iteration = iteration();
					response.next._sub_iteration = Scenario_Components::Types::Transit_Sub_Iteration_keys::TRAVELER_ALIGHTING_SUBITERATION;					
				}
				
				else if (sub_iteration() == Scenario_Components::Types::Transit_Sub_Iteration_keys::TRAVELER_ALIGHTING_SUBITERATION)
				{
					_this->alight_travelers();
					
					int position = _this->template current_position<int>();
					int number_of_links = _this->template number_of_stops<int>() - 1;

					if (position < number_of_links)
					{
						response.next._iteration = iteration();
						response.next._sub_iteration = Scenario_Components::Types::Transit_Sub_Iteration_keys::TRANSIT_VEHICLE_SEATING_SUBITERTION;
					}
					else
					{
						response.next._iteration = iteration();
						response.next._sub_iteration = Scenario_Components::Types::Transit_Sub_Iteration_keys::TRANSIT_VEHICLE_DEPOT_SUBITERATION;
					}
				}
				
				else if (sub_iteration() == Scenario_Components::Types::Transit_Sub_Iteration_keys::TRANSIT_VEHICLE_SEATING_SUBITERTION)
				{
					_this->rearrange_seats();
					response.next._iteration = iteration();
					response.next._sub_iteration = Scenario_Components::Types::Transit_Sub_Iteration_keys::TRAVELER_BOARDING_SUBITERATION;
				}
				
				else if (sub_iteration() == Scenario_Components::Types::Transit_Sub_Iteration_keys::TRAVELER_BOARDING_SUBITERATION)
				{
					_this->board_travelers(); 
					
					int position = _this->template current_position<int>();
					int depart_time = _this->template departure_seconds<std::vector<int>>()[position];

					response.next._iteration = depart_time;
					response.next._sub_iteration = Scenario_Components::Types::Transit_Sub_Iteration_keys::TRANSIT_VEHICLE_DEPARTING_SUBITERATION
						;
				}
				
				else if (sub_iteration() == Scenario_Components::Types::Transit_Sub_Iteration_keys::TRANSIT_VEHICLE_DEPARTING_SUBITERATION
					)
				{
					_this->transit_vehicle_departing();

					int position = _this->template current_position<int>();
					int arrival_time = _this->template arrival_seconds<std::vector<int>>()[position];

					response.next._iteration = arrival_time;
					response.next._sub_iteration = Scenario_Components::Types::Transit_Sub_Iteration_keys::TRANSIT_VEHICLE_ARRIVING_SUBITERATION;
				}
				
				else if (sub_iteration() == Scenario_Components::Types::Transit_Sub_Iteration_keys::TRANSIT_VEHICLE_DEPOT_SUBITERATION
					)
				{
					_this->transit_vehicle_depot();
					response.next._iteration = END;
				}

				else
				{
					assert(false);
					cout << "Should never reach here in transit vehicle conditional!" << endl;
				}
			}

			void transit_vehicle_arriving()
			{
				stringstream trajectory_stream;

				_Transit_Pattern_Interface* pattern = this->template pattern<_Transit_Pattern_Interface*>();
				std::string pattern_ID = pattern->template dbid<std::string>();
				_Pattern_Links_Container_Interface& pattern_links = pattern->template pattern_links<_Pattern_Links_Container_Interface&>();

				std::string trip_ID = this->template dbid<std::string>();
				int position = this->template current_position<int>();
				int arrival_time = this->template arrival_seconds<std::vector<int>>()[position];

				if (position < pattern_links.size())
				{
					_Link_Interface* pattern_link = (_Link_Interface*)(pattern_links[position]);			
					std::string stop_ID = pattern_link->template upstream_intersection<_Intersection_Interface*>()->template dbid<std::string>();

					trajectory_stream << "I am trip:\t" << trip_ID << "\t";
					trajectory_stream << "At position:\t" << position << "\t";
					trajectory_stream << "Time is:\t" << arrival_time << "\t";
					trajectory_stream << "Pattern is:\t" << pattern_ID << "\t";
					trajectory_stream << "Stop is:\t" << stop_ID << "\t";
					trajectory_stream << "0 - I arrived" << endl;
				}
				else
				{
					_Link_Interface* pattern_link = (_Link_Interface*)(pattern_links[position-1]);
					std::string stop_ID = pattern_link->template downstream_intersection<_Intersection_Interface*>()->template dbid<std::string>();

					trajectory_stream << "I am trip:\t" << trip_ID << "\t";
					trajectory_stream << "At position:\t" << position << "\t";
					trajectory_stream << "Time is:\t" << arrival_time << "\t";
					trajectory_stream << "Pattern is:\t" << pattern_ID << "\t";
					trajectory_stream << "Last stop is:\t" << stop_ID << "\t";
					trajectory_stream << "0 - I arrived" << endl;
				}

				fw_transit_vehicle_trajectory.Write_NoDelim(trajectory_stream);
			}

			void alight_travelers()
			{
				stringstream trajectory_stream;

				_Transit_Pattern_Interface* pattern = this->template pattern<_Transit_Pattern_Interface*>();
				std::string pattern_ID = pattern->template dbid<std::string>();
				_Pattern_Links_Container_Interface& pattern_links = pattern->template pattern_links<_Pattern_Links_Container_Interface&>();

				std::string trip_ID = this->template dbid<std::string>();
				int position = this->template current_position<int>();
				int arrival_time = this->template arrival_seconds<std::vector<int>>()[position];


				if (position < pattern_links.size())
				{
					_Link_Interface* pattern_link = (_Link_Interface*)(pattern_links[position]);
					std::string stop_ID = pattern_link->template upstream_intersection<_Intersection_Interface*>()->template dbid<std::string>();

					trajectory_stream << "I am trip:\t" << trip_ID << "\t";
					trajectory_stream << "At position:\t" << position << "\t";
					trajectory_stream << "Time is:\t" << arrival_time << "\t";
					trajectory_stream << "Pattern is:\t" << pattern_ID << "\t";
					trajectory_stream << "Stop is:\t" << stop_ID << "\t";
					trajectory_stream << "1 - I am alighting some travelers" << endl;
				}
				else
				{
					_Link_Interface* pattern_link = (_Link_Interface*)(pattern_links[position - 1]);
					std::string stop_ID = pattern_link->template downstream_intersection<_Intersection_Interface*>()->template dbid<std::string>();

					trajectory_stream << "I am trip:\t" << trip_ID << "\t";
					trajectory_stream << "At position:\t" << position << "\t";
					trajectory_stream << "Time is:\t" << arrival_time << "\t";
					trajectory_stream << "Pattern is:\t" << pattern_ID << "\t";
					trajectory_stream << "Last stop is:\t" << stop_ID << "\t";
					trajectory_stream << "1 - I am alighting all remaining travelers" << endl;
				}

				fw_transit_vehicle_trajectory.Write_NoDelim(trajectory_stream);
			}

			void rearrange_seats()
			{
				stringstream trajectory_stream;

				_Transit_Pattern_Interface* pattern = this->template pattern<_Transit_Pattern_Interface*>();
				std::string pattern_ID = pattern->template dbid<std::string>();
				_Pattern_Links_Container_Interface& pattern_links = pattern->template pattern_links<_Pattern_Links_Container_Interface&>();

				std::string trip_ID = this->template dbid<std::string>();
				int position = this->template current_position<int>();
				int arrival_time = this->template arrival_seconds<std::vector<int>>()[position];

				_Link_Interface* pattern_link = (_Link_Interface*)(pattern_links[position]);
				std::string stop_ID = pattern_link->template upstream_intersection<_Intersection_Interface*>()->template dbid<std::string>();

				trajectory_stream << "I am trip:\t" << trip_ID << "\t";
				trajectory_stream << "At position:\t" << position << "\t";
				trajectory_stream << "Time is:\t" << arrival_time << "\t";
				trajectory_stream << "Pattern is:\t" << pattern_ID << "\t";
				trajectory_stream << "Stop is:\t" << stop_ID << "\t";
				trajectory_stream << "2 - I am re-arranging seats" << endl;

				fw_transit_vehicle_trajectory.Write_NoDelim(trajectory_stream);
			}

			void board_travelers()
			{
				stringstream trajectory_stream;

				_Transit_Pattern_Interface* pattern = this->template pattern<_Transit_Pattern_Interface*>();
				std::string pattern_ID = pattern->template dbid<std::string>();
				_Pattern_Links_Container_Interface& pattern_links = pattern->template pattern_links<_Pattern_Links_Container_Interface&>();

				std::string trip_ID = this->template dbid<std::string>();
				int position = this->template current_position<int>();
				int arrival_time = this->template arrival_seconds<std::vector<int>>()[position];

				_Link_Interface* pattern_link = (_Link_Interface*)(pattern_links[position]);
				std::string stop_ID = pattern_link->template upstream_intersection<_Intersection_Interface*>()->template dbid<std::string>();

				trajectory_stream << "I am trip:\t" << trip_ID << "\t";
				trajectory_stream << "At position:\t" << position << "\t";
				trajectory_stream << "Time is:\t" << arrival_time << "\t";
				trajectory_stream << "Pattern is:\t" << pattern_ID << "\t";
				trajectory_stream << "Stop is:\t" << stop_ID << "\t";
				trajectory_stream << "3 - I am accepting some travelers on board" << endl;

				fw_transit_vehicle_trajectory.Write_NoDelim(trajectory_stream);
			}

			void transit_vehicle_departing()
			{
				stringstream trajectory_stream;

				_Transit_Pattern_Interface* pattern = this->template pattern<_Transit_Pattern_Interface*>();
				std::string pattern_ID = pattern->template dbid<std::string>();
				_Pattern_Links_Container_Interface& pattern_links = pattern->template pattern_links<_Pattern_Links_Container_Interface&>();

				std::string trip_ID = this->template dbid<std::string>();
				int position = this->template current_position<int>();
				int departure_time = this->template departure_seconds<std::vector<int>>()[position];

				_Link_Interface* pattern_link = (_Link_Interface*)(pattern_links[position]);
				std::string stop_ID = pattern_link->template upstream_intersection<_Intersection_Interface*>()->template dbid<std::string>();

				trajectory_stream << "I am trip:\t" << trip_ID << "\t";
				trajectory_stream << "At position:\t" << position << "\t";
				trajectory_stream << "Time is:\t" << departure_time << "\t";
				trajectory_stream << "Pattern is:\t" << pattern_ID << "\t";
				trajectory_stream << "Stop is:\t" << stop_ID << "\t";
				trajectory_stream << "4 - I am moving to my next stop" << endl;

				position++;
				this->template current_position<int>(position);

				fw_transit_vehicle_trajectory.Write_NoDelim(trajectory_stream);
			}

			void transit_vehicle_depot()
			{
				stringstream trajectory_stream;

				_Transit_Pattern_Interface* pattern = this->template pattern<_Transit_Pattern_Interface*>();
				std::string pattern_ID = pattern->template dbid<std::string>();
				_Pattern_Links_Container_Interface& pattern_links = pattern->template pattern_links<_Pattern_Links_Container_Interface&>();

				std::string trip_ID = this->template dbid<std::string>();
				int position = this->template current_position<int>();
				int depature_time = this->template departure_seconds<std::vector<int>>()[position];
				
				_Link_Interface* pattern_link = (_Link_Interface*)(pattern_links[position - 1]);
				std::string stop_ID = pattern_link->template downstream_intersection<_Intersection_Interface*>()->template dbid<std::string>();

				trajectory_stream << "I am trip:\t" << trip_ID << "\t";
				trajectory_stream << "At position:\t" << position << "\t";
				trajectory_stream << "Time is:\t" << depature_time << "\t";
				trajectory_stream << "Pattern is:\t" << pattern_ID << "\t";
				trajectory_stream << "Last stop is:\t" << stop_ID << "\t";
				trajectory_stream << "5 - I am going to my depot" << endl;

				fw_transit_vehicle_trajectory.Write_NoDelim(trajectory_stream);
			}

		};

	}
}

