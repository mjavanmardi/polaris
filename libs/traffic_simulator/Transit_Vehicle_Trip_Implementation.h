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
			typedef Person_Components::Prototypes::Person_Mover<typename _Person_Interface::get_type_of(Moving_Faculty)> _Person_Mover_Interface;
			typedef Movement_Plan_Components::Prototypes::Movement_Plan< typename _Person_Mover_Interface::get_type_of(Movement)> _Movement_Interface;
			typedef  Movement_Plan_Components::Prototypes::Multimodal_Trajectory_Unit<typename remove_pointer< typename _Movement_Interface::get_type_of(multimodal_trajectory_container)::value_type>::type>  _Multimodal_Trajectory_Unit_Interface;
			typedef  Random_Access_Sequence< typename _Movement_Interface::get_type_of(multimodal_trajectory_container), _Multimodal_Trajectory_Unit_Interface*> _Multimodal_Trajectory_Container_Interface;
			typedef Network_Components::Prototypes::Network<typename MasterType::network_type> _Network_Interface;
			typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;
			typedef Transit_Pattern_Components::Prototypes::Transit_Pattern<typename MasterType::transit_pattern_type> _Transit_Pattern_Interface;
			typedef Link_Components::Prototypes::Link<typename MasterType::link_type> _Link_Interface;
			typedef Demand_Components::Prototypes::Demand<typename MasterType::demand_type> _Demand_Interface;
			typedef typename std::list<_Person_Interface*>::iterator queue_iterator_type;
			

			m_data(int, internal_id, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(std::string, dbid, NONE, NONE);
			m_data(int, uuid, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(int, direction, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_prototype(Transit_Pattern_Components::Prototypes::Transit_Pattern, typename MasterType::transit_pattern_type, pattern, NONE, NONE);
			m_prototype(Network_Components::Prototypes::Network, typename MasterType::network_type, network_reference, NONE, NONE);
			m_container(std::vector<int>, arrival_seconds, NONE, NONE);
			m_container(std::vector<int>, departure_seconds, NONE, NONE);
			m_data(int, number_of_stops, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));

			m_container(std::vector<int>, act_arrival_seconds, NONE, NONE);
			m_container(std::vector<int>, act_departure_seconds, NONE, NONE);
			m_container(std::vector<int>, seated_load, NONE, NONE);
			m_container(std::vector<int>, standing_load, NONE, NONE);
			m_data(int, seated_capacity, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(int, standing_capacity, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));

			member_component_and_feature_accessor(Next_Simulation_Time, Value, Basic_Units::Prototypes::Time, Basic_Units::Implementations::Time_Implementation<NT>);
			
			//Simulation related
			m_container(std::list<_Person_Interface*>, people_standing, NONE, NONE);
			m_container(std::list<_Person_Interface*>, people_seated, NONE, NONE);			
			m_data(int, current_position, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			//----------------------------------------------------------------------

			typedef  Intersection_Components::Prototypes::Intersection<typename remove_pointer< typename type_of(network_reference)::get_type_of(intersections_container)::value_type>::type>  _Intersection_Interface;
			typedef  Random_Access_Sequence< typename type_of(network_reference)::get_type_of(intersections_container), _Intersection_Interface*> _Intersections_Container_Interface;

			typedef  typename _Transit_Pattern_Interface::get_type_of(pattern_stops) _Pattern_Stops_Container_Interface;
			typedef  typename _Transit_Pattern_Interface::get_type_of(pattern_links) _Pattern_Links_Container_Interface;
			

			template<typename TargetType> void schdeule_vehicle_movements_in_transit_network(void* network)
			{
				_network_reference = (network_reference_type)network;				

				this->template current_position<int>((int)-1);

				int arrival_time = this->template arrival_seconds<std::vector<int>>()[0];

				this->template Load_Event<ComponentType>(&ComponentType::move_transit_vehicles_in_transit_network_conditional, arrival_time, Scenario_Components::Types::Transit_Sub_Iteration_keys::TRANSIT_VEHICLE_ARRIVING_SUBITERATION);
			}

			static void move_transit_vehicles_in_transit_network_conditional(ComponentType* _this, Event_Response& response) 
			{
				int cur_iter = iteration();
				int cur_sub = sub_iteration();

				if (sub_iteration() == Scenario_Components::Types::Transit_Sub_Iteration_keys::TRANSIT_VEHICLE_ARRIVING_SUBITERATION)
				{
					_this->transit_vehicle_arriving();
					response.next._iteration = iteration();
					response.next._sub_iteration = Scenario_Components::Types::Transit_Sub_Iteration_keys::TRANSIT_VEHICLE_ALIGHTING_SUBITERATION;
				}
				
				else if (sub_iteration() == Scenario_Components::Types::Transit_Sub_Iteration_keys::TRANSIT_VEHICLE_ALIGHTING_SUBITERATION)
				{
					_this->alight_travelers();
					
					int position = _this->template current_position<int>();
					int number_of_links = _this->template number_of_stops<int>() - 1;

					if (position < number_of_links)
					{
						response.next._iteration = iteration();
						response.next._sub_iteration = Scenario_Components::Types::Transit_Sub_Iteration_keys::TRANSIT_VEHICLE_BOARDING_SUBITERATION;
					}
					else
					{
						response.next._iteration = iteration();
						response.next._sub_iteration = Scenario_Components::Types::Transit_Sub_Iteration_keys::TRANSIT_VEHICLE_DEPOT_SUBITERATION;
					}
				}
				
				else if (sub_iteration() == Scenario_Components::Types::Transit_Sub_Iteration_keys::TRANSIT_VEHICLE_BOARDING_SUBITERATION)
				{
					_this->board_travelers();
					response.next._iteration = _this->template Next_Simulation_Time<Simulation_Timestep_Increment>();
					response.next._sub_iteration = Scenario_Components::Types::Transit_Sub_Iteration_keys::TRANSIT_VEHICLE_DEPARTING_SUBITERATION;
				}
				
				else if (sub_iteration() == Scenario_Components::Types::Transit_Sub_Iteration_keys::TRANSIT_VEHICLE_DEPARTING_SUBITERATION)
				{
					_this->transit_vehicle_departing();
					response.next._iteration = _this->template Next_Simulation_Time<Simulation_Timestep_Increment>();
					response.next._sub_iteration = Scenario_Components::Types::Transit_Sub_Iteration_keys::TRANSIT_VEHICLE_ARRIVING_SUBITERATION;
				}
				
				else if (sub_iteration() == Scenario_Components::Types::Transit_Sub_Iteration_keys::TRANSIT_VEHICLE_DEPOT_SUBITERATION)
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

				int cur_iter = iteration();
				int cur_sub = sub_iteration();				

				std::string trip_ID = this->template dbid<std::string>();
				int position = this->template current_position<int>();

				position++;
				this->template current_position<int>(position);
				int arrival_time = this->template arrival_seconds<std::vector<int>>()[position];
				
				this->template act_arrival_seconds<std::vector<int>&>()[position] = cur_iter;

				if (position < pattern_links.size())
				{
					_Link_Interface* pattern_link = (_Link_Interface*)(pattern_links[position]);			
					std::string stop_ID = pattern_link->template upstream_intersection<_Intersection_Interface*>()->template dbid<std::string>();

					trajectory_stream << "I am trip:\t" << trip_ID << "\t";
					trajectory_stream << "At position:\t" << position << "\t";
					trajectory_stream << "Time is:\t" << arrival_time << "\t";
					trajectory_stream << "Simulation time is:\t" << cur_iter << "\t";
					trajectory_stream << "Subiteration is:\t" << cur_sub << "\t";
					trajectory_stream << "Pattern is:\t" << pattern_ID << "\t";
					trajectory_stream << "Stop is:\t" << stop_ID << "\t";
					trajectory_stream << "I arrived" << endl;
				}
				else
				{
					_Link_Interface* pattern_link = (_Link_Interface*)(pattern_links[position - 1]);
					std::string stop_ID = pattern_link->template downstream_intersection<_Intersection_Interface*>()->template dbid<std::string>();

					trajectory_stream << "I am trip:\t" << trip_ID << "\t";
					trajectory_stream << "At position:\t" << position << "\t";
					trajectory_stream << "Time is:\t" << arrival_time << "\t";
					trajectory_stream << "Simulation time is:\t" << cur_iter << "\t";
					trajectory_stream << "Subiteration is:\t" << cur_sub << "\t";
					trajectory_stream << "Pattern is:\t" << pattern_ID << "\t";
					trajectory_stream << "Last stop is:\t" << stop_ID << "\t";
					trajectory_stream << "I arrived" << endl;
				}

				fw_transit_vehicle_trajectory.Write_NoDelim(trajectory_stream);
			}

			void alight_travelers()
			{
				stringstream trajectory_stream;

				_Transit_Pattern_Interface* pattern = this->template pattern<_Transit_Pattern_Interface*>();
				std::string pattern_ID = pattern->template dbid<std::string>();
				_Pattern_Links_Container_Interface& pattern_links = pattern->template pattern_links<_Pattern_Links_Container_Interface&>();

				int cur_iter = iteration();
				int cur_sub = sub_iteration();

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
					trajectory_stream << "Simulation time is:\t" << cur_iter << "\t";
					trajectory_stream << "Subiteration is:\t" << cur_sub << "\t";
					trajectory_stream << "Pattern is:\t" << pattern_ID << "\t";
					trajectory_stream << "Stop is:\t" << stop_ID << "\t";
					trajectory_stream << "I am rearranging seats! Persons who got a seat are:";
				}
				else
				{
					_Link_Interface* pattern_link = (_Link_Interface*)(pattern_links[position - 1]);
					std::string stop_ID = pattern_link->template downstream_intersection<_Intersection_Interface*>()->template dbid<std::string>();

					trajectory_stream << "I am trip:\t" << trip_ID << "\t";
					trajectory_stream << "At position:\t" << position << "\t";
					trajectory_stream << "Time is:\t" << arrival_time << "\t";
					trajectory_stream << "Simulation time is:\t" << cur_iter << "\t";
					trajectory_stream << "Subiteration is:\t" << cur_sub << "\t";
					trajectory_stream << "Pattern is:\t" << pattern_ID << "\t";
					trajectory_stream << "Last stop is:\t" << stop_ID << "\t";
					trajectory_stream << "I am checking for drunk people! Persons who are kicked out are:";
				}

				//Get the list of people standing in the vehicle
				std::list<_Person_Interface*>* people_standing = this->template people_standing<std::list<_Person_Interface*>*>();
				//Get the list of people seated in the vehicle
				std::list<_Person_Interface*>* people_seated = this->template people_seated<std::list<_Person_Interface*>*>();

				queue_iterator_type position_in_vehicle_seated_queue;
				queue_iterator_type position_in_vehicle_standing_queue = people_standing->begin();
				//Loop over the standing list
				while (people_seated->size() < this->template seated_capacity<int>() && position_in_vehicle_standing_queue != people_standing->end())
				{					
					//Get the person
					_Person_Interface* person = (_Person_Interface*)*position_in_vehicle_standing_queue;
					//Delete person from the standing queue and increment the iterator
					people_standing->erase(position_in_vehicle_standing_queue++);
					//Set the waiting standing position of the person to null
					person->position_in_vehicle_standing_queue(people_standing->end());
					//Push the person into the seated people list
					people_seated->push_back(person);
					//Get the person's position in the list
					queue_iterator_type position_in_vehicle_seated_queue = --people_seated->end();
					//Set the person's position as a person property
					person->position_in_vehicle_seated_queue(position_in_vehicle_seated_queue);
					//Update the person's status
					person->template simulation_status<Person_Components::Types::Movement_Status_Keys>(Person_Components::Types::Movement_Status_Keys::ON_BOARD_SEATED);
					//Report the person
					trajectory_stream << "\t" << person->template uuid<int>();
				}

				trajectory_stream << endl;
				fw_transit_vehicle_trajectory.Write_NoDelim(trajectory_stream);
			}			

			void board_travelers()
			{
				stringstream trajectory_stream;				

				//Get the pattern of transit vehicle trip
				_Transit_Pattern_Interface* pattern = this->template pattern<_Transit_Pattern_Interface*>();
				//Get the pattern ID
				std::string pattern_ID = pattern->template dbid<std::string>();
				//Get the pattern links
				_Pattern_Links_Container_Interface& pattern_links = pattern->template pattern_links<_Pattern_Links_Container_Interface&>();

				int cur_iter = iteration();
				int cur_sub = sub_iteration();

				//Get the trip ID
				std::string trip_ID = this->template dbid<std::string>();
				//Get vehicle's position
				int position = this->template current_position<int>();
				//Get the arrival time
				int arrival_time = this->template arrival_seconds<std::vector<int>>()[position];

				//Get the current link
				_Link_Interface* pattern_link = (_Link_Interface*)(pattern_links[position]);
				//Get the stop ID
				std::string stop_ID = pattern_link->template upstream_intersection<_Intersection_Interface*>()->template dbid<std::string>();

				//Get the list of people standing in the vehicle
				std::list<_Person_Interface*>* people_standing = this->template people_standing<std::list<_Person_Interface*>*>();
				//Get the list of people seated in the vehicle
				std::list<_Person_Interface*>* people_seated = this->template people_seated<std::list<_Person_Interface*>*>();
				//Get the list of people waiting at the stop
				std::list<_Person_Interface*>* people_waiting = pattern_link->template people_waiting<std::list<_Person_Interface*>*>();

				//Get the departure time
				int departure_time = this->template departure_seconds<std::vector<int>>()[position];
				//Set the next response time
				this->template Next_Simulation_Time<Simulation_Timestep_Increment>(departure_time);

				trajectory_stream << "I am trip:\t" << trip_ID << "\t";
				trajectory_stream << "At position:\t" << position << "\t";
				trajectory_stream << "Time is:\t" << arrival_time << "\t";
				trajectory_stream << "Simulation time is:\t" << cur_iter << "\t";
				trajectory_stream << "Subiteration is:\t" << cur_sub << "\t";
				trajectory_stream << "Pattern is:\t" << pattern_ID << "\t";
				trajectory_stream << "Stop is:\t" << stop_ID << "\t";
				trajectory_stream << "I am accepting some travelers on board:";

				//Loop over the people waiting in the link
				queue_iterator_type position_in_link_waiting_queue = people_waiting->begin();
				while (position_in_link_waiting_queue != people_waiting->end())
				{
					//Get the person
					_Person_Interface* person = (_Person_Interface*) *position_in_link_waiting_queue;
					//Get the movement faculty
					_Person_Mover_Interface* person_mover = person->template Moving_Faculty<_Person_Mover_Interface*>();
					//Get the movement plan
					_Movement_Interface* movement = person_mover->Movement<_Movement_Interface*>();
					//Get the current position of person
					int person_position = movement->template current_multimodal_trajectory_position<int>();
					//Get the planned trajectory
					_Multimodal_Trajectory_Container_Interface& trajectory = movement->template multimodal_trajectory_container<_Multimodal_Trajectory_Container_Interface&>();
					//Get the trajectory members at that position
					_Multimodal_Trajectory_Unit_Interface* trajectory_unit = (_Multimodal_Trajectory_Unit_Interface*)trajectory[person_position];
					//Get the transit vehicle trip to be boarded
					ComponentType* person_vehicle_trip = trajectory_unit->template transit_vehicle_trip<ComponentType*>();

					//Check if the person is waiting for this vehicle
					if (person_vehicle_trip == this)
					{
						if (people_seated->size() < this->template seated_capacity<int>())
						{
							//Delete person from the waiting queue and increment the iterator
							people_waiting->erase(position_in_link_waiting_queue++);
							//Set the waiting queue position of the person to null
							person->position_in_link_waiting_queue(people_waiting->end());
							//Call the related person function
							person_mover->template person_boarded_transit_vehicle<NT>(departure_time);
							//Push the person into the seated people list
							people_seated->push_back(person);
							//Get the person's position in the list
							queue_iterator_type position_in_vehicle_seated_queue = --people_seated->end();
							//Set the person's position as a person property
							person->position_in_vehicle_seated_queue(position_in_vehicle_seated_queue);
							//Update the person's status
							person->template simulation_status<Person_Components::Types::Movement_Status_Keys>(Person_Components::Types::Movement_Status_Keys::ON_BOARD_SEATED);
							//Report the person
							trajectory_stream << "\tSIT\t" << person->template uuid<int>();
						}
						else if (people_standing->size() < this->template standing_capacity<int>())
						{
							//Delete person from the waiting queue and increment the iterator
							people_waiting->erase(position_in_link_waiting_queue++);	
							//Set the waiting queue position of the person to null
							person->position_in_link_waiting_queue(people_waiting->end());
							//Call the related person function
							person_mover->template person_boarded_transit_vehicle<NT>(departure_time);
							//Push the person into the seated people list
							people_standing->push_back(person);
							//Get the person's position in the list
							queue_iterator_type position_in_vehicle_standing_queue = --people_standing->end();
							//Set the person's position as a person property
							person->position_in_vehicle_standing_queue(position_in_vehicle_standing_queue);
							//Update the person's status
							person->template simulation_status<Person_Components::Types::Movement_Status_Keys>(Person_Components::Types::Movement_Status_Keys::ON_BOARD_STANDING);
							//Report the person
							trajectory_stream << "\tSTAND\t" << person->template uuid<int>();
						}
						else
						{
							position_in_link_waiting_queue++;
						}
					}
					else
					{
						position_in_link_waiting_queue++;
					}

					this->template seated_load<std::vector<int>&>()[position] = people_seated->size();
					this->template standing_load<std::vector<int>&>()[position] = people_standing->size();
				}

				trajectory_stream << endl;
				fw_transit_vehicle_trajectory.Write_NoDelim(trajectory_stream);
			}

			void transit_vehicle_departing()
			{
				stringstream trajectory_stream;

				_Transit_Pattern_Interface* pattern = this->template pattern<_Transit_Pattern_Interface*>();
				std::string pattern_ID = pattern->template dbid<std::string>();
				_Pattern_Links_Container_Interface& pattern_links = pattern->template pattern_links<_Pattern_Links_Container_Interface&>();

				int cur_iter = iteration();
				int cur_sub = sub_iteration();

				

				std::string trip_ID = this->template dbid<std::string>();
				int position = this->template current_position<int>();
				int departure_time = this->template departure_seconds<std::vector<int>>()[position];

				this->template act_departure_seconds<std::vector<int>&>()[position] = cur_iter;

				_Link_Interface* pattern_link = (_Link_Interface*)(pattern_links[position]);
				std::string stop_ID = pattern_link->template upstream_intersection<_Intersection_Interface*>()->template dbid<std::string>();

				trajectory_stream << "I am trip:\t" << trip_ID << "\t";
				trajectory_stream << "At position:\t" << position << "\t";
				trajectory_stream << "Time is:\t" << departure_time << "\t";
				trajectory_stream << "Simulation time is:\t" << cur_iter << "\t";
				trajectory_stream << "Subiteration is:\t" << cur_sub << "\t";
				trajectory_stream << "Pattern is:\t" << pattern_ID << "\t";
				trajectory_stream << "Stop is:\t" << stop_ID << "\t";
				trajectory_stream << "I am moving to my next stop:" << "\t";

				//this->template current_position<int>(position);
				int arrival_time = this->template arrival_seconds<std::vector<int>>()[position + 1];
				this->template Next_Simulation_Time<Simulation_Timestep_Increment>(arrival_time);

				//Get the list of people standing in the vehicle
				std::list<_Person_Interface*>* people_standing = this->template people_standing<std::list<_Person_Interface*>*>();
				//Get the list of people seated in the vehicle
				std::list<_Person_Interface*>* people_seated = this->template people_seated<std::list<_Person_Interface*>*>();

				trajectory_stream << "There are:\t" << people_seated->size() << "\tpeople seated:";

				//Write out the seated people
				queue_iterator_type position_in_vehicle_seated_queue = people_seated->begin();
				while (position_in_vehicle_seated_queue != people_seated->end())
				{
					//Get the person
					_Person_Interface* person = (_Person_Interface*)*position_in_vehicle_seated_queue;
					//Report the person
					trajectory_stream << "\t" << person->template uuid<int>();
					position_in_vehicle_seated_queue++;
				}

				trajectory_stream << "\tand:\t" << people_standing->size() << "\tpeople standing:";
				//Write out the standing people
				queue_iterator_type position_in_vehicle_standing_queue = people_standing->begin();
				while (position_in_vehicle_standing_queue != people_standing->end())
				{
					//Get the person
					_Person_Interface* person = (_Person_Interface*)*position_in_vehicle_standing_queue;
					//Report the person
					trajectory_stream << "\t" << person->template uuid<int>();
					position_in_vehicle_standing_queue++;
				}
				trajectory_stream << endl;

				fw_transit_vehicle_trajectory.Write_NoDelim(trajectory_stream);
			}

			void transit_vehicle_depot()
			{
				stringstream trajectory_stream;

				_Transit_Pattern_Interface* pattern = this->template pattern<_Transit_Pattern_Interface*>();
				std::string pattern_ID = pattern->template dbid<std::string>();
				_Pattern_Links_Container_Interface& pattern_links = pattern->template pattern_links<_Pattern_Links_Container_Interface&>();

				int cur_iter = iteration();
				int cur_sub = sub_iteration();

				std::string trip_ID = this->template dbid<std::string>();
				int position = this->template current_position<int>();
				int depature_time = this->template departure_seconds<std::vector<int>>()[position];

				this->template act_departure_seconds<std::vector<int>&>()[position] = cur_iter;

				((_Demand_Interface*)_global_demand)->Add_Transit_Vehicle_Trip_Record(this);
				
				_Link_Interface* pattern_link = (_Link_Interface*)(pattern_links[position - 1]);
				std::string stop_ID = pattern_link->template downstream_intersection<_Intersection_Interface*>()->template dbid<std::string>();

				trajectory_stream << "I am trip:\t" << trip_ID << "\t";
				trajectory_stream << "At position:\t" << position << "\t";
				trajectory_stream << "Time is:\t" << depature_time << "\t";
				trajectory_stream << "Simulation time is:\t" << cur_iter << "\t";
				trajectory_stream << "Subiteration is:\t" << cur_sub << "\t";
				trajectory_stream << "Pattern is:\t" << pattern_ID << "\t";
				trajectory_stream << "Last stop is:\t" << stop_ID << "\t";
				trajectory_stream << "I am going to my depot" << endl;

				fw_transit_vehicle_trajectory.Write_NoDelim(trajectory_stream);
			}
		};

	}
}

