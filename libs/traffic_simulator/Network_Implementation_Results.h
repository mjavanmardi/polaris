#pragma once
#include "Network_Prototype.h"

namespace Network_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{

		template<typename MasterType,typename InheritanceList>
		template<typename TargetType>
		 void Network_Implementation<MasterType,InheritanceList>::write_vehicle_trajectory()
		{
			typedef  Intersection_Components::Prototypes::Intersection<typename remove_pointer<typename  type_of(intersections_container)::value_type>::type>  _Intersection_Interface;
			typedef  Random_Access_Sequence< type_of(intersections_container), _Intersection_Interface*> _Intersections_Container_Interface;

			typedef  Link_Components::Prototypes::Link<typename remove_pointer<typename  type_of(links_container)::value_type>::type>  _Link_Interface;
			typedef  Random_Access_Sequence< type_of(links_container), _Link_Interface*> _Links_Container_Interface;

			typedef  Turn_Movement_Components::Prototypes::Movement<typename remove_pointer<typename  type_of(turn_movements_container)::value_type>::type>  _Turn_Movement_Interface;
			typedef  Random_Access_Sequence< type_of(turn_movements_container), _Turn_Movement_Interface*> _Turn_Movements_Container_Interface;

			typedef  Intersection_Components::Prototypes::Outbound_Inbound_Movements<typename remove_pointer< typename _Intersection_Interface::get_type_of(outbound_inbound_movements)::value_type>::type>  _Outbound_Inbound_Movements_Interface;
			typedef  Random_Access_Sequence< typename _Intersection_Interface::get_type_of(outbound_inbound_movements), _Outbound_Inbound_Movements_Interface*> _Outbound_Inbound_Movements_Container_Interface;

			typedef  Intersection_Components::Prototypes::Inbound_Outbound_Movements<typename remove_pointer< typename _Intersection_Interface::get_type_of(inbound_outbound_movements)::value_type>::type>  _Inbound_Outbound_Movements_Interface;
			typedef  Random_Access_Sequence< typename _Intersection_Interface::get_type_of(inbound_outbound_movements), _Inbound_Outbound_Movements_Interface*> _Inbound_Outbound_Movements_Container_Interface;

			typedef  Activity_Location_Components::Prototypes::Activity_Location<typename remove_pointer<typename  type_of(activity_locations_container)::value_type>::type>  _Activity_Location_Interface;
			typedef  Random_Access_Sequence< type_of(activity_locations_container), _Activity_Location_Interface*> _Activity_Locations_Container_Interface;

			typedef  Zone_Components::Prototypes::Zone<typename remove_pointer<typename  type_of(zones_container)::value_type>::type>  _Zone_Interface;
			typedef  Random_Access_Sequence< type_of(zones_container), _Zone_Interface*> _Zones_Container_Interface;

			typedef  Vehicle_Components::Prototypes::Vehicle<typename remove_pointer< typename _Link_Interface::get_type_of(link_destination_vehicle_queue)::value_type>::type>  _Vehicle_Interface;
			typedef  Back_Insertion_Sequence< typename _Link_Interface::get_type_of(link_destination_vehicle_queue), _Vehicle_Interface*> _Vehicles_Container_Interface;

			typedef  Movement_Plan_Components::Prototypes::Movement_Plan< typename _Vehicle_Interface::get_type_of(movement_plan)> _Movement_Plan_Interface;
			typedef  Movement_Plan_Components::Prototypes::Trajectory_Unit<typename remove_pointer< typename _Movement_Plan_Interface::get_type_of(trajectory_container)::value_type>::type>  _Trajectory_Unit_Interface;
			typedef  Random_Access_Sequence< typename _Movement_Plan_Interface::get_type_of(trajectory_container), _Trajectory_Unit_Interface*> _Trajectory_Container_Interface;

			typedef Scenario<typename MasterType::scenario_type> _Scenario_Interface;
			fstream& vehicle_trajectory_file = scenario_reference<_Scenario_Interface*>()->template vehicle_trajectory_file<fstream&>();
				
			typename _Links_Container_Interface::iterator link_itr;

			// output link moe to database
			shared_ptr<odb::database> db_ptr = ((_Scenario_Interface*)_global_scenario)->template demand_db_ptr<shared_ptr<odb::database>>();
			odb::transaction t(db_ptr->begin());

			
			for(link_itr = _links_container.begin(); link_itr != _links_container.end(); link_itr++)
			{
				_Link_Interface* destination_link = (_Link_Interface*)(*link_itr);

				int num_arrived_vehicls_of_a_link = (int)destination_link->template link_destination_vehicle_queue<_Vehicles_Container_Interface&>().size();
				if (num_arrived_vehicls_of_a_link > 0)
				{
					//output vehicle trajectory
					while(num_arrived_vehicls_of_a_link)
					{
						// check whether to sampel this vehicle
						_Vehicle_Interface* vehicle = destination_link->template link_destination_vehicle_queue<_Vehicles_Container_Interface&>().front();
						_Movement_Plan_Interface* movement_plan = vehicle->template movement_plan<_Movement_Plan_Interface*>();

						//============================================================
						// JA: Added 1/17/18 - use every vehicle for gap calculation
						// update the network relative gap calculations with the current vehicle
						network_moe_data.network_relative_gap += abs((movement_plan->template arrived_time<Time_Seconds>() - movement_plan->template departed_time<Time_Seconds>()) - movement_plan->template estimated_travel_time_when_departed<float>());
						network_moe_data.network_routed_ttime += movement_plan->template estimated_travel_time_when_departed<float>();
						//===============================================================

						if (vehicle->write_trajectory())
						{	
							// Fill the PATH DB record
							shared_ptr<polaris::io::Path> path_db_record(new polaris::io::Path());

							//float travel_time_ratio = travel_time / estimated_travel_time_when_departed;
							//float trip_length = movement_plan->template route_length<float>();
							//int entry_time = movement_plan->template entry_time<int>();
							//int origin_loading_time = trajectory_unit->template enter_time<int>();
							//int loading_delay = origin_loading_time - entry_time;

							path_db_record->setVehicle(vehicle->vehicle_ptr<shared_ptr<polaris::io::Vehicle>>());
							path_db_record->setTraveler_ID(movement_plan->traveler_id<int>());
							//path_db_record->setOrigin_Zone(movement_plan->template origin<_Zone_Interface*>()->template uuid<int>());
							//path_db_record->setDestination_Zone(movement_plan->template destination<_Zone_Interface*>()->template uuid<int>());
							path_db_record->setOrigin_Activity_Location(movement_plan->template origin<_Activity_Location_Interface*>()->template uuid<int>());
							path_db_record->setDestination_Activity_Location(movement_plan->template destination<_Activity_Location_Interface*>()->template uuid<int>());
							path_db_record->setOrigin_Link(movement_plan->template origin<_Link_Interface*>()->template uuid<int>());
							path_db_record->setDestination_Link(movement_plan->template destination<_Link_Interface*>()->template uuid<int>());
							path_db_record->setNum_Links(movement_plan->template trajectory_container<_Trajectory_Container_Interface&>().size());
							path_db_record->setDeparture_Time(movement_plan->template departed_time<Time_Seconds>());
							path_db_record->setTravel_Time(movement_plan->template arrived_time<Time_Seconds>() - movement_plan->template departed_time<Time_Seconds>());
							path_db_record->setRouted_Time(movement_plan->template estimated_travel_time_when_departed<float>());
							
							
							_Trajectory_Container_Interface& trajectory = ((_Movement_Plan_Interface*)movement_plan)->template trajectory_container<_Trajectory_Container_Interface&>();
							float start = 0;
							int route_link_counter = 0;
							for (auto link_itr = trajectory.begin(); link_itr != trajectory.end(); ++link_itr, ++route_link_counter)
							{
								// FIll the path link DB record for each step of the path
								polaris::io::link_travel path_link_record;

								//_Trajectory_Unit_Interface* trajectory_unit = movement_plan->template trajectory_container<_Trajectory_Container_Interface&>()[route_link_counter];
								_Trajectory_Unit_Interface* trajectory_unit = (_Trajectory_Unit_Interface*)(*link_itr);
								_Link_Interface* route_link = trajectory_unit->template link<_Link_Interface*>();

								int route_link_id = route_link->template uuid<int>();
								int route_link_enter_time = trajectory_unit->template enter_time<int>();
								float route_link_delayed_time = float(trajectory_unit->template intersection_delay_time<float>());		
								int route_link_exit_time = /*trajectory_unit->exit_time<int>(); //*/movement_plan->template get_route_link_exit_time<NULLTYPE>(route_link_counter);
								float route_link_travel_time = float((route_link_exit_time - route_link_enter_time));
					
								if (route_link->template link_type<Link_Components::Types::Link_Type_Keys>() != Link_Components::Types::EXTERNAL)
								{
									path_link_record.setLink(route_link->dbid<int>());
									path_link_record.setDir(route_link->direction<int>());
									path_link_record.setEntering_Time(route_link_enter_time);
									path_link_record.setTravel_Time(route_link_travel_time);
									path_link_record.setDelayed_Time(route_link_delayed_time);
									path_link_record.setExit_Position(start += GLOBALS::Convert_Units<Feet, Meters>(route_link->template length<float>()));

									path_db_record->setLink(path_link_record);

									//vehicle_trajectory_file
									//	<< vehicle_id << ","
									//	<<route_link_counter + 1 << ","
									//	<<(route_link_id - route_link_id%2)/2<< ","
									//	<<route_link_id%2<< ","
									//	<<convert_seconds_to_hhmmss(route_link_enter_time) << ","
									//	<<route_link_travel_time << ","
									//	//<<route_link_delayed_time << ","
									//	<<start<<","
									//	<<route_link->template length<float>()<<","
									//	<<(route_link->template length<float>()/5280.0)/((route_link_travel_time - trajectory_unit->template intersection_delay_time<float>())/3600.0) << ","
									//	<<route_link->template free_flow_speed<float>() << ","
									//	<<trajectory_unit->template intersection_delay_time<float>() << ","
									//	<<start + route_link->template length<float>()<<","
									//	<<endl;
								}
								
							}
							db_ptr->persist(path_db_record);
						}

						destination_link->template link_destination_vehicle_queue<_Vehicles_Container_Interface&>().pop_front();

						//deallocate vehicle
						//vehicle->template clear_trajectory<NT>();
						num_arrived_vehicls_of_a_link--;
					}
				}
			}

			t.commit();

		};

		template<typename MasterType,typename InheritanceList>
		template<typename TargetType>
		 void Network_Implementation<MasterType,InheritanceList>::write_ttime_distribution()
		{
			typedef Scenario<typename MasterType::scenario_type> _Scenario_Interface;	
			typedef Network<typename MasterType::network_type> _Network_Interface;

			fstream& ttime_distribution_file = ((_Scenario_Interface*)_global_scenario)->template ttime_distribution_file<fstream&>();
			int current_time = ((_Network_Interface*)this)->template start_of_current_simulation_interval_absolute<int>();
			ttime_distribution_file << current_time;
			for (int j=0; j < (int)ttime_distribution.size(); j++)
			{
				ttime_distribution_file <<","<< ttime_distribution[j];
			}
			ttime_distribution_file << endl;
		}

		template<typename MasterType,typename InheritanceList>
		template<typename TargetType>
		 void Network_Implementation<MasterType,InheritanceList>::write_network_link_flow()
		{
			typedef  Link_Components::Prototypes::Link<typename remove_pointer<typename  type_of(links_container)::value_type>::type>  _Link_Interface;
			typedef  Random_Access_Sequence< type_of(links_container), _Link_Interface*> _Links_Container_Interface;

			typedef  Vehicle_Components::Prototypes::Vehicle<typename remove_pointer< typename _Link_Interface::get_type_of(link_destination_vehicle_queue)::value_type>::type>  _Vehicle_Interface;
			typedef  Back_Insertion_Sequence< typename _Link_Interface::get_type_of(link_destination_vehicle_queue), _Vehicle_Interface*> _Vehicles_Container_Interface;

			typedef Scenario<typename MasterType::scenario_type> _Scenario_Interface;
			typedef Network<typename MasterType::network_type> _Network_Interface;
			int simulation_interval_length = ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>();
			int simulation_interval_index = ((_Network_Interface*)this)->template current_simulation_interval_index<int>();
				
			fstream& network_link_flow_file = scenario_reference<_Scenario_Interface*>()->template network_link_flow_file<fstream&>();
				
			typename _Links_Container_Interface::iterator link_itr;
			for(link_itr = _links_container.begin(); link_itr != _links_container.end(); link_itr++)
			{
				//flow
				_Link_Interface* link = (_Link_Interface*)(*link_itr);
				float bwtt = (float) (link->template length<float>()/(link->template backward_wave_speed<float>()*5280.0/3600.0)); // in seconds
				float fftt = (float) (link->template length<float>()/(link->template free_flow_speed<float>()*5280.0/3600.0)); //in seconds

				int link_fftt_cached_simulation_interval_size = max(1, int(ceil(float(fftt/((float)simulation_interval_length)))));
				int link_bwtt_cached_simulation_interval_size = max(1, int(ceil(float(bwtt/((float)simulation_interval_length)))));
					
				network_link_flow_file
					<< convert_seconds_to_hhmmss(((_Network_Interface*)this)->template start_of_current_simulation_interval_absolute<int>()) <<  ","
					<< link->template uuid<int>() <<  ","
					<< link_fftt_cached_simulation_interval_size <<  ","
					<< link_bwtt_cached_simulation_interval_size <<  ","
					<< link->template link_origin_cumulative_arrived_vehicles<int>() << ","
					<< link->template link_origin_cumulative_departed_vehicles<int>() << ","
					<< link->template link_destination_cumulative_arrived_vehicles<int>() << ","
					<< link->template link_upstream_cumulative_arrived_vehicles<int>() << ","
					<< link->template link_upstream_cumulative_vehicles<int>() << ","
					<< link->template link_downstream_cumulative_arrived_vehicles<int>() << ","
					<< link->template link_downstream_cumulative_vehicles<int>() << ","
					<< link->template link_num_vehicles_in_queue<int>()
					<<endl;		
			}
		};

		template<typename MasterType,typename InheritanceList>
		template<typename TargetType>
		 void Network_Implementation<MasterType,InheritanceList>::write_network_link_turn_time()
		{
			typedef  Link_Components::Prototypes::Link<typename remove_pointer<typename  type_of(links_container)::value_type>::type>  _Link_Interface;
			typedef  Random_Access_Sequence< type_of(links_container), _Link_Interface*> _Links_Container_Interface;

			typedef  Vehicle_Components::Prototypes::Vehicle<typename remove_pointer< typename _Link_Interface::get_type_of(link_destination_vehicle_queue)::value_type>::type>  _Vehicle_Interface;
			typedef  Back_Insertion_Sequence< typename _Link_Interface::get_type_of(link_destination_vehicle_queue), _Vehicle_Interface*> _Vehicles_Container_Interface;

			typedef  Turn_Movement_Components::Prototypes::Movement<typename remove_pointer<typename  type_of(turn_movements_container)::value_type>::type>  _Turn_Movement_Interface;
			typedef  Random_Access_Sequence< type_of(turn_movements_container), _Turn_Movement_Interface*> _Turn_Movements_Container_Interface;

			typedef Scenario<typename MasterType::scenario_type> _Scenario_Interface;
			typedef  Intersection_Components::Prototypes::Intersection<typename remove_pointer<typename  type_of(intersections_container)::value_type>::type>  _Intersection_Interface;
			typedef  Random_Access_Sequence< type_of(intersections_container), _Intersection_Interface*> _Intersections_Container_Interface;

			typedef Network<typename MasterType::network_type> _Network_Interface;
			_Scenario_Interface* scenario = (_Scenario_Interface*)_scenario_reference;
			int simulation_interval_length = scenario->template simulation_interval_length<int>();
			int simulation_interval_index = ((_Network_Interface*)this)->template current_simulation_interval_index<int>();
			int simulation_start_time = scenario->template simulation_start_time<int>();
			int current_starting_time = simulation_start_time + simulation_interval_length*simulation_interval_index;

			fstream& network_link_turn_time_file = scenario->template network_link_turn_time_file<fstream&>();
			typename _Turn_Movements_Container_Interface::iterator movement_itr;
			for (movement_itr = _turn_movements_container.begin(); movement_itr != _turn_movements_container.end(); movement_itr++)
			{
				_Turn_Movement_Interface* movement = (_Turn_Movement_Interface*)(*movement_itr);
				_Link_Interface* link = movement->template inbound_link<_Link_Interface*>();
					
				float fftt = (float) (movement->template inbound_link<_Link_Interface*>()->template length<float>()/(movement->template inbound_link<_Link_Interface*>()->template free_flow_speed<float>()*5280.0/3600.0)); //in seconds
				int link_fftt_cached_simulation_interval_size = max(1,int(ceil(float(fftt/(simulation_interval_length*1.0)))));
					
					
				network_link_turn_time_file
					<< convert_seconds_to_hhmmss(((_Network_Interface*)this)->template start_of_current_simulation_interval_absolute<int>()) <<  ","
					<< current_starting_time << ","
					<< movement->template inbound_link<_Link_Interface*>()->template downstream_intersection<_Intersection_Interface*>()->template uuid<int>() << ","
					<< movement->template uuid<int>() <<  ","
					<< movement->template inbound_link<_Link_Interface*>()->template uuid<int>() <<  ","
					<< movement->template outbound_link<_Link_Interface*>()->template uuid<int>() <<  ","
					<< fftt << ","
					<< link_fftt_cached_simulation_interval_size*simulation_interval_length << ","

					<< movement->template inbound_link<_Link_Interface*>()->template link_supply<float>() << ","
					<< movement->template outbound_link<_Link_Interface*>()->template link_supply<float>() << ","
					<< movement->template inbound_link<_Link_Interface*>()->template link_origin_arrived_vehicles<int>() << ","
					<<movement->template outbound_link<_Link_Interface*>()->template link_origin_arrived_vehicles<int>() << ","
					<< movement->template inbound_link<_Link_Interface*>()->template link_origin_departed_vehicles<int>() << ","
					<<movement->template outbound_link<_Link_Interface*>()->template link_origin_departed_vehicles<int>() << ","
					<< movement->template outbound_link_arrived_time_based_experienced_link_turn_travel_delay<float>() << ","
					<< movement->template green_time<float>() << ","
					<< movement->template movement_capacity<float>() << ","
					<< movement->template movement_demand<float>() << ","
					<< movement->template movement_supply<float>() << ","
					<< movement->template movement_flow<float>() << ","
					<< movement->template movement_transferred<float>() << ","
					//<< movement->template vehicles_container<_Vehicles_Container_Interface&>().size() << ","
					<< movement->template turn_travel_penalty<float>() << ","
					<< movement->template forward_link_turn_travel_time<float>()
					<<endl;

			}

		};

		template<typename MasterType,typename InheritanceList>
		template<typename TargetType>
		 void Network_Implementation<MasterType,InheritanceList>::write_node_control_state()
		{

			typedef  Intersection_Components::Prototypes::Intersection<typename remove_pointer<typename  type_of(intersections_container)::value_type>::type>  _Intersection_Interface;
			typedef  Random_Access_Sequence< type_of(intersections_container), _Intersection_Interface*> _Intersections_Container_Interface;

			typedef  Intersection_Control_Components::Prototypes::Intersection_Control< typename _Intersection_Interface::get_type_of(intersection_control)> _Intersection_Control_Interface;
			typedef  Intersection_Control_Components::Prototypes::Control_Plan<typename remove_pointer< typename _Intersection_Control_Interface::get_type_of(control_plan_data_array)::value_type>::type>  _Control_Plan_Interface;
			typedef  Random_Access_Sequence< typename _Intersection_Control_Interface::get_type_of(control_plan_data_array), _Control_Plan_Interface*> _Control_Plans_Container_Interface;

			typedef  Intersection_Control_Components::Prototypes::Phase<typename remove_pointer< typename _Control_Plan_Interface::get_type_of(phase_data_array)::value_type>::type>  _Phase_Interface;
			typedef  Random_Access_Sequence< typename _Control_Plan_Interface::get_type_of(phase_data_array), _Phase_Interface*> _Phases_Container_Interface;

			typedef  Intersection_Control_Components::Prototypes::Phase_Movement<typename remove_pointer< typename _Phase_Interface::get_type_of(turn_movements_in_the_phase_array)::value_type>::type>  _Phase_Movement_Interface;
			typedef  Random_Access_Sequence< typename _Phase_Interface::get_type_of(turn_movements_in_the_phase_array), _Phase_Movement_Interface*> _Phase_Movements_Container_Interface;

			typedef  Turn_Movement_Components::Prototypes::Movement<typename remove_pointer<typename  type_of(turn_movements_container)::value_type>::type>  _Movement_Interface;
			typedef  Random_Access_Sequence< type_of(turn_movements_container), _Movement_Interface*> _Movements_Container_Interface;

			typedef  Intersection_Control_Components::Prototypes::Approach<typename remove_pointer< typename _Control_Plan_Interface::get_type_of(approach_data_array)::value_type>::type>  _Approach_Interface;
			typedef  Random_Access_Sequence< typename _Control_Plan_Interface::get_type_of(approach_data_array), _Approach_Interface*> _Approaches_Container_Interface;

			typedef  Link_Components::Prototypes::Link<typename remove_pointer<typename  type_of(links_container)::value_type>::type>  _Link_Interface;
			typedef  Random_Access_Sequence< type_of(links_container), _Link_Interface*> _Links_Container_Interface;

			typedef Network<typename MasterType::network_type> _Network_Interface;
			typedef  Scenario_Components::Prototypes::Scenario< type_of(scenario_reference)> _Scenario_Interface;
			_Network_Interface* _this_ptr = (_Network_Interface*)this;				
			_Scenario_Interface* scenario = (_Scenario_Interface*)_global_scenario;

			fstream& network_node_control_state_file = scenario_reference<_Scenario_Interface*>()->template network_node_control_state_file<fstream&>();
			for (int i = 0; i < (int)_intersections_container.size(); i++)
			{
				_Intersection_Interface* intersection = (_Intersection_Interface*)_intersections_container[i];
				_Intersection_Control_Interface* intersection_control = intersection->template intersection_control<_Intersection_Control_Interface*>();
				_Control_Plan_Interface* current_control_plan = intersection_control->template current_control_plan<_Control_Plan_Interface*>();
				int starting_time = current_control_plan->template starting_time<int>();
				int ending_time = current_control_plan->template ending_time<int>();
				Intersection_Control_Components::Types::Intersection_Type_Keys control_type = current_control_plan->template control_type<Intersection_Control_Components::Types::Intersection_Type_Keys>();

				network_node_control_state_file
					<< convert_seconds_to_hhmmss(_this_ptr->template start_of_current_simulation_interval_absolute<int>()) <<  ","
					<< _this_ptr->template current_simulation_interval_index<int>() <<  ","
					<< _this_ptr->template start_of_current_simulation_interval_relative<int>() <<  ","
					<< intersection->template uuid<int>() <<  ","
					<< current_control_plan->template control_plan_index<int>() <<  ","
					<< current_control_plan->template control_type<int>() <<  ","
					<< convert_seconds_to_hhmmss(starting_time) <<  ","
					<< convert_seconds_to_hhmmss(ending_time) <<  ","
					<< current_control_plan->template approach_data_array<_Approaches_Container_Interface&>().size() <<  ",";
		
				if (control_type == Intersection_Control_Components::Types::PRE_TIMED_SIGNAL_CONTROL || control_type == Intersection_Control_Components::Types::ACTUATED_SIGNAL_CONTROL)
				{
					int num_phases = (int)current_control_plan->template phase_data_array<_Phases_Container_Interface&>().size();
					int last_phase = num_phases - 1;
					int cycle_index = current_control_plan->template cycle_index<int>();
					int cycle_starting_time = current_control_plan->template cycle_starting_time<int>();
					int cycle_ending_time = current_control_plan->template cycle_ending_time<int>();
					int cycle_length = cycle_ending_time - cycle_starting_time;

					network_node_control_state_file
						<< cycle_index <<  ","
						<< cycle_length <<  ","
						<< convert_seconds_to_hhmmss(cycle_starting_time) <<  ","
						<< convert_seconds_to_hhmmss(cycle_ending_time) <<  ","
						<< num_phases << ",";

					for (int iphase=0;iphase<num_phases;iphase++)
					{
						_Phase_Interface* phase = current_control_plan->template phase_data_array<_Phases_Container_Interface&>()[iphase];
						int green_starting_time = phase->template green_starting_time<int>();
						int yellow_starting_time = cycle_starting_time +  phase->template yellow_starting_time<int>();
						int red_starting_time = cycle_starting_time + phase->template red_start_time<int>();
						int phase_end_time = red_starting_time + phase->template all_red_time<int>();

						network_node_control_state_file
							<< iphase <<  ","
							<< convert_seconds_to_hhmmss(green_starting_time) <<  ","
							<< convert_seconds_to_hhmmss(yellow_starting_time) <<  ","
							<< convert_seconds_to_hhmmss(red_starting_time) <<  ","
							<< convert_seconds_to_hhmmss(phase_end_time) <<  ",";
					}
				}
				network_node_control_state_file <<endl;
			}
		}

		template<typename MasterType,typename InheritanceList>
		template<typename TargetType>
		 void Network_Implementation<MasterType,InheritanceList>::write_output_summary()
		{

			typedef  Intersection_Components::Prototypes::Intersection<typename remove_pointer<typename  type_of(intersections_container)::value_type>::type>  _Intersection_Interface;
			typedef  Random_Access_Sequence< type_of(intersections_container), _Intersection_Interface*> _Intersections_Container_Interface;

			typedef  Intersection_Control_Components::Prototypes::Intersection_Control< typename _Intersection_Interface::get_type_of(intersection_control)> _Intersection_Control_Interface;
			typedef  Intersection_Control_Components::Prototypes::Control_Plan<typename remove_pointer< typename _Intersection_Control_Interface::get_type_of(control_plan_data_array)::value_type>::type>  _Control_Plan_Interface;
			typedef  Random_Access_Sequence< typename _Intersection_Control_Interface::get_type_of(control_plan_data_array), _Control_Plan_Interface*> _Control_Plans_Container_Interface;

			typedef  Intersection_Control_Components::Prototypes::Phase<typename remove_pointer< typename _Control_Plan_Interface::get_type_of(phase_data_array)::value_type>::type>  _Phase_Interface;
			typedef  Random_Access_Sequence< typename _Control_Plan_Interface::get_type_of(phase_data_array), _Phase_Interface*> _Phases_Container_Interface;

			typedef  Intersection_Control_Components::Prototypes::Phase_Movement<typename remove_pointer< typename _Phase_Interface::get_type_of(turn_movements_in_the_phase_array)::value_type>::type>  _Phase_Movement_Interface;
			typedef  Random_Access_Sequence< typename _Phase_Interface::get_type_of(turn_movements_in_the_phase_array), _Phase_Movement_Interface*> _Phase_Movements_Container_Interface;

			typedef  Turn_Movement_Components::Prototypes::Movement<typename remove_pointer<typename  type_of(turn_movements_container)::value_type>::type>  _Movement_Interface;
			typedef  Random_Access_Sequence< type_of(turn_movements_container), _Movement_Interface*> _Movements_Container_Interface;

			typedef  Intersection_Control_Components::Prototypes::Approach<typename remove_pointer< typename _Control_Plan_Interface::get_type_of(approach_data_array)::value_type>::type>  _Approach_Interface;
			typedef  Random_Access_Sequence< typename _Control_Plan_Interface::get_type_of(approach_data_array), _Approach_Interface*> _Approaches_Container_Interface;

			typedef  Link_Components::Prototypes::Link<typename remove_pointer<typename  type_of(links_container)::value_type>::type>  _Link_Interface;
			typedef  Random_Access_Sequence< type_of(links_container), _Link_Interface*> _Links_Container_Interface;

			typedef Network<typename MasterType::network_type> _Network_Interface;
			typedef  Scenario_Components::Prototypes::Scenario< type_of(scenario_reference)> _Scenario_Interface;

			_Network_Interface* _this_ptr = (_Network_Interface*)this;
			_Scenario_Interface* scenario = scenario_reference<_Scenario_Interface*>();

			fstream& output_summary_file = scenario->template output_summary_file<fstream&>();

            _current_cpu_time_in_seconds = (long)get_current_cpu_time_in_seconds();

			long long totalPhysicalMemory = 0;
			long long physicalMemoryUsedByProcess = 0;
			//mem_info(totalPhysicalMemory, physicalMemoryUsedByProcess);

            long elapsed_time = _current_cpu_time_in_seconds - _start_cpu_time_in_seconds;
			output_summary_file
                    << convert_seconds_to_hhmmss(_this_ptr->template start_of_current_simulation_interval_absolute<int>()).c_str() << ","
                    << scenario->template network_cumulative_loaded_vehicles<int>() <<  ","
                    << scenario->template network_cumulative_departed_vehicles<int>() << ","
                    << scenario->template network_cumulative_arrived_vehicles<int>() << ","
                    << scenario->template network_in_network_vehicles<int>() << ","
					<< scenario->template network_cumulative_switched_decisions<int>() << ","
					<< _network_vmt << ","
					<< _network_vht_in_network_based << ","
					<< scenario->template network_average_trip_travel_time<float>() << ","
					<< scenario->template network_cumulative_switched_decisions_excessive_delay<int>() << ","
					<< scenario->template network_cumulative_switched_decisions_realtime_informed<int>() << ","
					<< scenario->template network_cumulative_switched_decisions_ITS_informed<int>() << ","
                    << convert_seconds_to_hhmmss(elapsed_time).c_str() << ","
					<< _this_ptr->template start_of_current_simulation_interval_absolute<int>() << ","
					<< getCurrentRSS()/1000000 << "," 
					<< network_moe_data.network_relative_gap / network_moe_data.network_routed_ttime << ","
					//<< physicalMemoryUsedByProcess/1000000 << ","
					//<< int(float(physicalMemoryUsedByProcess)/float(totalPhysicalMemory)*100.0) << ","
                    <<endl;
		}

		template<typename MasterType,typename InheritanceList>
		template<typename TargetType>
		 void Network_Implementation<MasterType,InheritanceList>::output_moe_for_simulation_interval()
		{
			using namespace polaris::io;
			typedef  Scenario_Components::Prototypes::Scenario< type_of(scenario_reference)> _Scenario_Interface;
			typedef  Link_Components::Prototypes::Link<typename remove_pointer<typename  type_of(links_container)::value_type>::type>  _Link_Interface;
			typedef  Random_Access_Sequence< type_of(links_container), _Link_Interface*> _Links_Container_Interface;

			typedef  Turn_Movement_Components::Prototypes::Movement<typename remove_pointer<typename  type_of(turn_movements_container)::value_type>::type>  _Turn_Movement_Interface;
			typedef  Random_Access_Sequence< type_of(turn_movements_container), _Turn_Movement_Interface*> _Turn_Movements_Container_Interface;


			typedef Network<typename MasterType::network_type> _Network_Interface;
			int time = ((_Network_Interface*)this)->template start_of_current_simulation_interval_absolute<int>();
			typedef typename MasterType::intersection_type _intersection_component_type;
			typedef typename MasterType::link_type _link_component_type;
			
			if (((_Scenario_Interface*)_global_scenario)->template output_link_moe_for_simulation_interval<bool>())
			{
				try
				{
					// output link moe to database
					shared_ptr<odb::database> db_ptr = ((_Scenario_Interface*)_global_scenario)->template result_db_ptr<shared_ptr<odb::database>>();
					odb::transaction t(db_ptr->begin());

					typename _Links_Container_Interface::iterator link_itr;
					RealtimeLinkMOE link_moe_db_record;
					for (link_itr = _links_container.begin(); link_itr != _links_container.end(); link_itr++)
					{
						_link_component_type* link = (_link_component_type*)(*link_itr);
						shared_ptr<polaris::io::RealtimeLinkMOE> link_moe_db_record(new polaris::io::RealtimeLinkMOE());

						link_moe_db_record->setLink_Uid(link->uuid<int>());
						link_moe_db_record->setLink_Type(link->link_type<Link_Components::Types::Link_Type_Keys>());
						link_moe_db_record->setLink_Length(GLOBALS::Length_Converter.Convert_Value<Feet, Meters>(link->length<float>()));											// output in SI (m), currently stored internally in feet -> replace eventually with use of units library
						link_moe_db_record->setStart_Time(time);
						link_moe_db_record->setEnd_Time(time + ((_Scenario_Interface*)_global_scenario)->template assignment_interval_length<int>());
						link_moe_db_record->setLink_Travel_Time(GLOBALS::Convert_Units<Time_Minutes, Time_Seconds>(link->realtime_link_moe_data.link_travel_time));					// output in SI (s), currently stored internally in minutes -> replace eventually with use of units library
						link_moe_db_record->setLink_Travel_Time_Standard_Deviation(GLOBALS::Convert_Units<Time_Minutes, Time_Seconds>(link->realtime_link_moe_data.link_travel_time_standard_deviation));
						link_moe_db_record->setLink_Queue_Length(link->realtime_link_moe_data.link_queue_length);
						link_moe_db_record->setLink_Travel_Delay(GLOBALS::Convert_Units<Time_Minutes, Time_Seconds>(link->realtime_link_moe_data.link_travel_delay));
						link_moe_db_record->setLink_Travel_Delay_Standard_Deviation(GLOBALS::Convert_Units<Time_Minutes, Time_Seconds>(link->realtime_link_moe_data.link_travel_delay_standard_deviation));
						link_moe_db_record->setLink_Speed(GLOBALS::Convert_Units<Miles_Per_Hour, Meters_Per_Second>(link->realtime_link_moe_data.link_speed));
						link_moe_db_record->setLink_Density(link->realtime_link_moe_data.link_density / GLOBALS::Convert_Units<Miles, Kilometers>(1.0));									// output in SI (veh / km), currently stored internally in Veh / mile -> replace eventually with use of units library
						link_moe_db_record->setLink_In_Flow_Rate(link->realtime_link_moe_data.link_in_flow_rate);
						link_moe_db_record->setLink_Out_Flow_Rate(link->realtime_link_moe_data.link_out_flow_rate);
						link_moe_db_record->setLink_In_Volume(link->realtime_link_moe_data.link_in_volume);
						link_moe_db_record->setLink_Out_Volume(link->realtime_link_moe_data.link_out_volume);
						link_moe_db_record->setLink_Speed_Ratio(link->realtime_link_moe_data.link_speed_ratio);
						link_moe_db_record->setLink_In_Flow_Ratio(link->realtime_link_moe_data.link_in_flow_ratio);
						link_moe_db_record->setLink_Out_Flow_Ratio(link->realtime_link_moe_data.link_out_flow_ratio);
						link_moe_db_record->setLink_Density_Ratio(link->realtime_link_moe_data.link_density_ratio);
						link_moe_db_record->setLink_Travel_Time_Ratio(link->realtime_link_moe_data.link_travel_time_ratio);

						try
						{
							db_ptr->persist(link_moe_db_record);
						}
						catch (odb::sqlite::database_exception ex)
						{
							cout << ex.message() << ". DB error in network implementation results, line 585." << endl;
						}
					}
					t.commit();
				}
				catch (odb::sqlite::database_exception ex)
				{
					cout << ex.message() << ". DB error in network implementation results, line 585." << endl;
				}
			}

			//if (((_Scenario_Interface*)_global_scenario)->template output_link_moe_for_simulation_interval<bool>())
			//{	
			//	// output link moe
			//	
			//	typename _Links_Container_Interface::iterator link_itr;
			//	for(link_itr = _links_container.begin(); link_itr != _links_container.end(); link_itr++)
			//	{
			//		_link_component_type* link = (_link_component_type*)(*link_itr);
			//		((_Scenario_Interface*)_global_scenario)->template out_realtime_link_moe_file<fstream&>()
			//			<< convert_seconds_to_hhmmss(time).c_str() << ","
			//			<< time << ","
			//			<< link->_uuid << ","
			//			<< link->_dbid << ","
			//			<< link->_direction << ","
			//			<< ((_intersection_component_type*)link->_upstream_intersection)->_uuid << ","
			//			<< ((_intersection_component_type*)link->_downstream_intersection)->_uuid << ","
			//			<< link->_link_type << ","	
			//			<< link->realtime_link_moe_data.link_travel_time << ","
			//			<< link->realtime_link_moe_data.link_travel_delay << ","
			//			<< link->realtime_link_moe_data.link_queue_length << ","
			//			<< link->realtime_link_moe_data.link_speed << ","
			//			<< link->realtime_link_moe_data.link_density << ","
			//			<< link->realtime_link_moe_data.link_in_volume << ","
			//			<< link->realtime_link_moe_data.link_out_volume << ","
			//			<< link->realtime_link_moe_data.link_travel_time_ratio << ","
			//			<< link->realtime_link_moe_data.link_speed_ratio << ","
			//			<< link->realtime_link_moe_data.link_density_ratio << ","
			//			<< endl;

			//	}
			//}

			if (((_Scenario_Interface*)_global_scenario)->template output_turn_movement_moe_for_simulation_interval<bool>())
			{
				try
				{
					// output link moe to database
					shared_ptr<odb::database> db_ptr = ((_Scenario_Interface*)_global_scenario)->template result_db_ptr<shared_ptr<odb::database>>();
					odb::transaction t(db_ptr->begin());

					// output turn movement moe
					typedef MasterType::turn_movement_type _movement_component_type;
					typename _Turn_Movements_Container_Interface::iterator movement_itr;
					for (movement_itr = _turn_movements_container.begin(); movement_itr != _turn_movements_container.end(); movement_itr++)
					{
						_movement_component_type* movement = (_movement_component_type*)(*movement_itr);
						shared_ptr<polaris::io::RealtimeTurnMOE> turn_moe_rec(new polaris::io::RealtimeTurnMOE());
						turn_moe_rec->setTurn_Uid(movement->uuid<int>());
						turn_moe_rec->setStart_Time(time);
						turn_moe_rec->setEnd_Time(time + ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>());
						turn_moe_rec->setInbound_Link_Uid(movement->inbound_link<_Link_Interface*>()->uuid<int>());
						turn_moe_rec->setOutbound_Link_Uid(movement->outbound_link<_Link_Interface*>()->uuid<int>());
						turn_moe_rec->setNode_Uid(movement->inbound_link<_Link_Interface*>()->downstream_intersection<_intersection_component_type*>()->uuid<int>());
						turn_moe_rec->setTurn_Penalty(GLOBALS::Convert_Units<Time_Minutes, Time_Seconds>(movement->realtime_movement_moe_data.turn_penalty));							// output in SI (s), currently stored internally in minutes -> replace eventually with use of units library
						turn_moe_rec->setTurn_Penalty_SD(GLOBALS::Convert_Units<Time_Minutes, Time_Seconds>(movement->realtime_movement_moe_data.turn_penalty_standard_deviation));		// output in SI (s), currently stored internally in minutes -> replace eventually with use of units library
						turn_moe_rec->setInbound_Turn_Travel_Time(GLOBALS::Convert_Units<Time_Minutes, Time_Seconds>(movement->realtime_movement_moe_data.inbound_link_turn_time));		// output in SI (s), currently stored internally in minutes -> replace eventually with use of units library
						turn_moe_rec->setOutbound_Turn_Travel_Time(GLOBALS::Convert_Units<Time_Minutes, Time_Seconds>(movement->realtime_movement_moe_data.outbound_link_turn_time));	// output in SI (s), currently stored internally in minutes -> replace eventually with use of units library
						turn_moe_rec->setTurn_Flow_Rate(movement->realtime_movement_moe_data.movement_flow_rate);
						db_ptr->persist(turn_moe_rec);
					}
					t.commit();
				}
				catch (odb::sqlite::database_exception ex)
				{
					cout << ex.message() << ". DB error in network implementation results, line 684." << endl;
				}
			}

			//if (((_Scenario_Interface*)_global_scenario)->template output_turn_movement_moe_for_simulation_interval<bool>())
			//{	
			//	// output turn movement moe
			//	typedef typename MasterType::turn_movement_type _movement_component_type;
			//	typename _Turn_Movements_Container_Interface::iterator movement_itr;
			//	for(movement_itr = _turn_movements_container.begin(); movement_itr != _turn_movements_container.end(); movement_itr++)
			//	{
			//		_movement_component_type* movement = (_movement_component_type*)(*movement_itr);
			//		((_Scenario_Interface*)_global_scenario)->template out_realtime_movement_moe_file<fstream&>() 
			//			<< convert_seconds_to_hhmmss(time).c_str() << ","
			//			<< time << ","
			//			<< movement->_uuid << ","
			//			<< ((_link_component_type*)movement->_inbound_link)->_uuid << ","
			//			<< ((_link_component_type*)movement->_outbound_link)->_uuid << ","
			//			<< ((_intersection_component_type*)((_link_component_type*)movement->_inbound_link)->_downstream_intersection)->_uuid << ","
			//			<< movement->realtime_movement_moe_data.turn_penalty << ","
			//			<< movement->realtime_movement_moe_data.inbound_link_turn_time << ","
			//			<< movement->realtime_movement_moe_data.outbound_link_turn_time << ","
			//			<< movement->realtime_movement_moe_data.movement_flow_rate
			//			<<endl;

			//	}
			//}

			if (((_Scenario_Interface*)_global_scenario)->template output_network_moe_for_simulation_interval<bool>())
			{	
				// output network moe
				((_Scenario_Interface*)_global_scenario)->template out_realtime_network_moe_file<fstream&>()
					<< convert_seconds_to_hhmmss(time).c_str() << ","
					<< time << ","
					<< realtime_network_moe_data.num_loaded_vehicles << ","
					<< realtime_network_moe_data.num_departed_vehicles << ","
					<< realtime_network_moe_data.num_arrived_vehicles << ","
					<< realtime_network_moe_data.network_avg_link_travel_time << ","
					<< realtime_network_moe_data.network_avg_link_speed << ","
					<< realtime_network_moe_data.network_avg_link_density << ","
					<< realtime_network_moe_data.network_avg_link_in_volume << ","
					<< realtime_network_moe_data.network_avg_link_out_volume << ","
					<< realtime_network_moe_data.network_avg_link_travel_time_ratio << ","
					<< realtime_network_moe_data.network_avg_link_speed_ratio << ","
					<< realtime_network_moe_data.network_avg_link_density_ratio << ","
					<< realtime_network_moe_data.network_avg_link_queue_length << ","
					<< _network_vmt << ","
					//TODO: swap this back
					//<< _network_vht_in_network_based << ","
					<< _network_vht << ","
					<< ((_Scenario_Interface*)_global_scenario)->template network_cumulative_loaded_vehicles<int>() << ","
					<< ((_Scenario_Interface*)_global_scenario)->template network_cumulative_departed_vehicles<int>() << ","
					<< ((_Scenario_Interface*)_global_scenario)->template network_in_network_vehicles<int>() << ","
					<< ((_Scenario_Interface*)_global_scenario)->template network_cumulative_arrived_vehicles<int>()
					<< endl;
			}
		}

		template<typename MasterType,typename InheritanceList>
		template<typename TargetType>
		void Network_Implementation<MasterType,InheritanceList>::output_moe_for_assignment_interval()
		{
			using namespace polaris::io;
			typedef  Scenario_Components::Prototypes::Scenario< type_of(scenario_reference)> _Scenario_Interface;
			typedef  Link_Components::Prototypes::Link<typename remove_pointer<typename  type_of(links_container)::value_type>::type>  _Link_Interface;
			typedef  Random_Access_Sequence< type_of(links_container), _Link_Interface*> _Links_Container_Interface;

			typedef  Turn_Movement_Components::Prototypes::Movement<typename remove_pointer<typename  type_of(turn_movements_container)::value_type>::type>  _Turn_Movement_Interface;
			typedef  Random_Access_Sequence< type_of(turn_movements_container), _Turn_Movement_Interface*> _Turn_Movements_Container_Interface;


			typedef Network<typename MasterType::network_type> _Network_Interface;
			int time = ((_Network_Interface*)this)->template start_of_current_simulation_interval_absolute<int>() + ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>() - ((_Scenario_Interface*)_global_scenario)->template assignment_interval_length<int>();
			
			typedef typename MasterType::link_type _link_component_type;
			typedef typename MasterType::intersection_type _intersection_component_type;
			
			if (((_Scenario_Interface*)_global_scenario)->template output_link_moe_for_assignment_interval<bool>())
			{
				try
				{
					// output link moe to database
					shared_ptr<odb::database> db_ptr = ((_Scenario_Interface*)_global_scenario)->template result_db_ptr<shared_ptr<odb::database>>();
					odb::transaction t(db_ptr->begin());

					typename _Links_Container_Interface::iterator link_itr;
					LinkMOE link_moe_db_record;
					for(link_itr = _links_container.begin(); link_itr != _links_container.end(); link_itr++)
					{
						_link_component_type* link = (_link_component_type*)(*link_itr);
						shared_ptr<polaris::io::LinkMOE> link_moe_db_record(new polaris::io::LinkMOE());
				
						link_moe_db_record->setLink_Uid(link->uuid<int>());
						link_moe_db_record->setLink_Type(link->link_type<Link_Components::Types::Link_Type_Keys>());
						link_moe_db_record->setLink_Length(GLOBALS::Length_Converter.Convert_Value<Feet,Meters>(link->length<float>()));											// output in SI (m), currently stored internally in feet -> replace eventually with use of units library
						link_moe_db_record->setStart_Time(time);
						link_moe_db_record->setEnd_Time(time + ((_Scenario_Interface*)_global_scenario)->template assignment_interval_length<int>());
						link_moe_db_record->setLink_Travel_Time(GLOBALS::Convert_Units<Time_Minutes,Time_Seconds>(link->link_moe_data.link_travel_time));					// output in SI (s), currently stored internally in minutes -> replace eventually with use of units library
						link_moe_db_record->setLink_Travel_Time_Standard_Deviation(GLOBALS::Convert_Units<Time_Minutes, Time_Seconds>(link->link_moe_data.link_travel_time_standard_deviation));
						link_moe_db_record->setLink_Queue_Length(link->link_moe_data.link_queue_length);
						link_moe_db_record->setLink_Travel_Delay(GLOBALS::Convert_Units<Time_Minutes, Time_Seconds>(link->link_moe_data.link_travel_delay));
						link_moe_db_record->setLink_Travel_Delay_Standard_Deviation(GLOBALS::Convert_Units<Time_Minutes, Time_Seconds>(link->link_moe_data.link_travel_delay_standard_deviation));
						link_moe_db_record->setLink_Speed(GLOBALS::Convert_Units<Miles_Per_Hour,Meters_Per_Second>(link->link_moe_data.link_speed));
						link_moe_db_record->setLink_Density(link->link_moe_data.link_density/GLOBALS::Convert_Units<Miles,Kilometers>(1.0));									// output in SI (veh / km), currently stored internally in Veh / mile -> replace eventually with use of units library
						link_moe_db_record->setLink_In_Flow_Rate(link->link_moe_data.link_in_flow_rate);
						link_moe_db_record->setLink_Out_Flow_Rate(link->link_moe_data.link_out_flow_rate);
						link_moe_db_record->setLink_In_Volume(link->link_moe_data.link_in_volume);
						link_moe_db_record->setLink_Out_Volume(link->link_moe_data.link_out_volume);
						link_moe_db_record->setLink_Speed_Ratio(link->link_moe_data.link_speed_ratio);
						link_moe_db_record->setLink_In_Flow_Ratio(link->link_moe_data.link_in_flow_ratio);
						link_moe_db_record->setLink_Out_Flow_Ratio(link->link_moe_data.link_out_flow_ratio);
						link_moe_db_record->setLink_Density_Ratio(link->link_moe_data.link_density_ratio);
						link_moe_db_record->setLink_Travel_Time_Ratio(link->link_moe_data.link_travel_time_ratio);
						
						try
						{				
							db_ptr->persist(link_moe_db_record);
						}
						catch (odb::sqlite::database_exception ex)
						{
							cout << ex.message()<<". DB error in network implementation results, line 585."<<endl;
						}
					}
					t.commit();
				}
				catch (odb::sqlite::database_exception ex)
				{
					cout << ex.message()<<". DB error in network implementation results, line 585."<<endl;
				}
			}

			//===============================================================================================================
			// WRITE TURN MOVEMENT MOE TO RESULT DATABASE
			//---------------------------------------------------------------------------------------------------------------
			if (((_Scenario_Interface*)_global_scenario)->template output_turn_movement_moe_for_assignment_interval<bool>())
			{
				try
				{
					// output link moe to database
					shared_ptr<odb::database> db_ptr = ((_Scenario_Interface*)_global_scenario)->template result_db_ptr<shared_ptr<odb::database>>();
					odb::transaction t(db_ptr->begin());

					// output turn movement moe
					typedef MasterType::turn_movement_type _movement_component_type;
					typename _Turn_Movements_Container_Interface::iterator movement_itr;
					for (movement_itr = _turn_movements_container.begin(); movement_itr != _turn_movements_container.end(); movement_itr++)
					{
						_movement_component_type* movement = (_movement_component_type*)(*movement_itr);
						shared_ptr<polaris::io::TurnMOE> turn_moe_rec(new polaris::io::TurnMOE());
						turn_moe_rec->setTurn_Uid(movement->uuid<int>());
						turn_moe_rec->setStart_Time(time);
						turn_moe_rec->setEnd_Time(time + ((_Scenario_Interface*)_global_scenario)->template assignment_interval_length<int>());
						turn_moe_rec->setInbound_Link_Uid(movement->inbound_link<_Link_Interface*>()->uuid<int>());
						turn_moe_rec->setOutbound_Link_Uid(movement->outbound_link<_Link_Interface*>()->uuid<int>());
						turn_moe_rec->setNode_Uid(movement->inbound_link<_Link_Interface*>()->downstream_intersection<_intersection_component_type*>()->uuid<int>());
						turn_moe_rec->setTurn_Penalty(GLOBALS::Convert_Units<Time_Minutes, Time_Seconds>(movement->movement_moe_data.turn_penalty));							// output in SI (s), currently stored internally in minutes -> replace eventually with use of units library
						turn_moe_rec->setTurn_Penalty_SD(GLOBALS::Convert_Units<Time_Minutes, Time_Seconds>(movement->movement_moe_data.turn_penalty_standard_deviation));		// output in SI (s), currently stored internally in minutes -> replace eventually with use of units library
						turn_moe_rec->setInbound_Turn_Travel_Time(GLOBALS::Convert_Units<Time_Minutes, Time_Seconds>(movement->movement_moe_data.inbound_link_turn_time));		// output in SI (s), currently stored internally in minutes -> replace eventually with use of units library
						turn_moe_rec->setOutbound_Turn_Travel_Time(GLOBALS::Convert_Units<Time_Minutes, Time_Seconds>(movement->movement_moe_data.outbound_link_turn_time));	// output in SI (s), currently stored internally in minutes -> replace eventually with use of units library
						turn_moe_rec->setTurn_Flow_Rate(movement->movement_moe_data.movement_flow_rate);						
						db_ptr->persist(turn_moe_rec);
					}
					t.commit();
				}
				catch (odb::sqlite::database_exception ex)
				{
					cout << ex.message() << ". DB error in network implementation results, line 684." << endl;
				}
			}

			//===============================================================================================================
			//NOT SURE WHAT THIS IS/DOES - JAA
			//---------------------------------------------------------------------------------------------------------------
			if (((_Scenario_Interface*)_global_scenario)->template output_analzye_link_group_moe_for_assignment_interval<bool>())
			{
				typedef  Analyze_Link_Group_Components::Prototypes::Analyze_Link_Group<typename remove_pointer<typename  type_of(analyze_link_groups_container)::value_type>::type>  _Analyze_Link_Group_Interface;
				typedef  Random_Access_Sequence< type_of(analyze_link_groups_container), _Analyze_Link_Group_Interface*> _Analyze_Link_Groups_Container_Interface;

				typedef  Network_Event_Components::Prototypes::Network_Event< typename _Analyze_Link_Group_Interface::get_type_of(event)> _Network_Event_Interface;
				typename _Analyze_Link_Groups_Container_Interface::iterator analyze_link_group_itr;
				for(analyze_link_group_itr = _analyze_link_groups_container.begin(); analyze_link_group_itr != _analyze_link_groups_container.end(); analyze_link_group_itr++)
				{
					_Analyze_Link_Group_Interface* analyze_link_group = (_Analyze_Link_Group_Interface*)(*analyze_link_group_itr);
					_Network_Event_Interface* event = analyze_link_group->template event<_Network_Event_Interface*>();
					//if (time <= event->template end_time<int>() + ((_Scenario_Interface*)_global_scenario)->template event_impact_time<int>())
					{
						analyze_link_group->template output_moe<NT>(time);
					}
				}
			}

			// DEPRECATED - output_link_moe_for_assignment_interval now defaults to outputing to DB
			//if (((_Scenario_Interface*)_global_scenario)->template DB_output_link_moe_for_assignment_interval<bool>())
			//{
			//	shared_ptr<LinkMOE> moe_rec(nullptr);
			//	// output link moe to database
			//	using namespace odb;
			//	using namespace polaris::io;
			//	typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;
			//	string name(((_Scenario_Interface*)_global_scenario)->template database_name<string&>());
			//	unique_ptr<database> db (open_sqlite_database (name));
			//	transaction t(db->begin());


			//	typename _Links_Container_Interface::iterator link_itr;
			//	for(link_itr = _links_container.begin(); link_itr != _links_container.end(); link_itr++)
			//	{
			//		_link_component_type* link = (_link_component_type*)(*link_itr);
			//		moe_rec.reset(new LinkMOE());
			//		moe_rec->setLink_Uid(link->_uuid);
			//		moe_rec->setLink_Density(link->link_moe_data.link_density);
			//		moe_rec->setLink_Density_Ratio(link->link_moe_data.link_density_ratio);
			//		moe_rec->setLink_Speed(link->link_moe_data.link_speed);
			//		moe_rec->setLink_In_Volume(link->link_moe_data.link_in_volume);
			//		moe_rec->setLink_Out_Volume(link->link_moe_data.link_out_volume);
			//		db->persist(moe_rec);
			//	}
			//	t.commit();
			//}

			// DEPRECATED - USE THE RESULTS DB - see above
			//if (((_Scenario_Interface*)_global_scenario)->template output_turn_movement_moe_for_assignment_interval<bool>())
			//{
			//	// output turn movement moe
			//	typedef typename MasterType::turn_movement_type _movement_component_type;
			//	typename _Turn_Movements_Container_Interface::iterator movement_itr;
			//	for(movement_itr = _turn_movements_container.begin(); movement_itr != _turn_movements_container.end(); movement_itr++)
			//	{
			//		_movement_component_type* movement = (_movement_component_type*)(*movement_itr);
			//		((_Scenario_Interface*)_global_scenario)->template out_movement_moe_file<fstream&>() 
			//			<< convert_seconds_to_hhmmss(time).c_str() << ","
			//			<< time << ","
			//			<< movement->_uuid << ","
			//			<< ((_link_component_type*)movement->_inbound_link)->_uuid << ","
			//			<< ((_link_component_type*)movement->_outbound_link)->_uuid << ","
			//			<< ((_intersection_component_type*)((_link_component_type*)movement->_inbound_link)->_downstream_intersection)->_uuid << ","
			//			<< movement->movement_moe_data.turn_penalty << ","
			//			<< movement->movement_moe_data.turn_penalty_standard_deviation << ","
			//			<< movement->movement_moe_data.inbound_link_turn_time << ","
			//			<< movement->movement_moe_data.outbound_link_turn_time << ","
			//			<< movement->movement_moe_data.movement_flow_rate
			//			<<endl;

			//	}
			//}
			if (((_Scenario_Interface*)_global_scenario)->template output_network_moe_for_assignment_interval<bool>())
			{
				((_Scenario_Interface*)_global_scenario)->template out_network_moe_file<fstream&>()
					<< convert_seconds_to_hhmmss(time).c_str() << ","
					<< time << ","
					<< network_moe_data.num_loaded_vehicles << ","
					<< network_moe_data.num_departed_vehicles << ","
					<< network_moe_data.num_arrived_vehicles << ","
					<< network_moe_data.network_avg_link_travel_time << ","
					<< network_moe_data.network_avg_link_speed << ","
					<< network_moe_data.network_avg_link_density << ","
					<< network_moe_data.network_avg_link_in_flow_rate << ","
					<< network_moe_data.network_avg_link_out_flow_rate << ","
					<< network_moe_data.network_avg_link_in_volume << ","
					<< network_moe_data.network_avg_link_out_volume << ","
					<< network_moe_data.network_avg_link_travel_time_ratio << ","
					<< network_moe_data.network_avg_link_speed_ratio << ","
					<< network_moe_data.network_avg_link_density_ratio << ","
					<< network_moe_data.network_avg_link_in_flow_ratio << ","
					<< network_moe_data.network_avg_link_out_flow_ratio << ","
					<< _network_vmt << ","
					<< _network_vht_in_network_based 
					//<< network_moe_data.assignment_calculation_time << ","
					//<< network_moe_data.simulation_calculation_time << ","
					//<< network_moe_data.operation_calculation_time << ","
					//<< network_moe_data.output_calculation_time 
					<< endl;
				}
			}

			template<typename MasterType,typename InheritanceList>
		template<typename TargetType>
		 void Network_Implementation<MasterType,InheritanceList>::read_analyze_link_groups()
			{
				typedef Scenario<typename MasterType::scenario_type> _Scenario_Interface;
				typedef Network_Event_Components::Prototypes::Network_Event<typename MasterType::base_network_event_type> _Network_Event_Interface;
				typedef  Link_Components::Prototypes::Link<typename remove_pointer<typename  type_of(links_container)::value_type>::type>  _Link_Interface;
				typedef  Random_Access_Sequence< type_of(links_container), _Link_Interface*> _Links_Container_Interface;

				typedef  Analyze_Link_Group_Components::Prototypes::Analyze_Link_Group<typename remove_pointer<typename  type_of(analyze_link_groups_container)::value_type>::type>  _Analyze_Link_Group_Interface;
				typedef  Random_Access_Sequence< type_of(analyze_link_groups_container), _Analyze_Link_Group_Interface*> _Analyze_Link_Groups_Container_Interface;


				string line;
				std::vector<string> tokens;

				fstream& _analyze_link_groups_file = ((_Scenario_Interface*)_global_scenario)->template analyze_link_groups_file<fstream&>();
				getline(_analyze_link_groups_file, line); // skip "number_of_groups" label
				getline(_analyze_link_groups_file, line); // get number of groups
				int num_of_groups = atoi(line.c_str());
				for (int i = 0; i < num_of_groups; i++)
				{
					_Analyze_Link_Group_Interface* analyze_link_group = (_Analyze_Link_Group_Interface*)Allocate<typename MasterType::analyze_link_group_type>();
					_Network_Event_Interface* event = (_Network_Event_Interface*)Allocate<typename MasterType::base_network_event_type>();
					
					getline(_analyze_link_groups_file, line); // skip "event_start_time,event_end_time" label
					// read in start time and end time
					getline(_analyze_link_groups_file, line);
					
					string_split(tokens, line);
					event->template start_time<int>(atoi(tokens[0].c_str()));
					event->template end_time<int>(atoi(tokens[1].c_str()));

					analyze_link_group->template event<_Network_Event_Interface*>(event);
					analyze_link_group->template initialize<NT>(i);

					getline(_analyze_link_groups_file, line); // skip "affected_links" label

					// read affected links
					getline(_analyze_link_groups_file, line);
					string_split(tokens, line);
					for (int j = 0; j < (int)tokens.size(); j++)
					{
						int link_id = atoi(tokens[j].c_str());
						std::unordered_map<int,std::vector<typename MasterType::link_type*>>& db_map=((Network<typename MasterType::network_type>*)_global_network)->template db_id_to_links_map<std::unordered_map<int,std::vector<typename MasterType::link_type*>>&>();

						if(db_map.count(link_id))
						{
							std::vector<typename MasterType::link_type*>& links=db_map[link_id];

							typename std::vector<typename MasterType::link_type*>::iterator vitr;

							for(vitr=links.begin();vitr!=links.end();vitr++)
							{
								_Link_Interface* link = (_Link_Interface*)(*vitr);
								analyze_link_group->template links_container<_Links_Container_Interface&>().push_back(link);
							}
						}
					}
					_analyze_link_groups_container.push_back((typename MasterType::analyze_link_group_type*)analyze_link_group);
				}
			}
	}
}

