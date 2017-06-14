#pragma once
#include "Traffic_Simulator_Includes.h"
//#include "../File_IO/utilities.h"
#ifndef EXCLUDE_DEMAND
#include "Network_Skimming_Prototype.h"
#endif
#ifndef _MSC_VER
// for hash_map
using namespace __gnu_cxx;
#endif

/*------------------------------------------------*/

namespace Routing_Components
{
	namespace Types
	{
	}

	namespace Prototypes
	{
		prototype struct Routing ADD_DEBUG_INFO
		{
			tag_as_prototype;

			void Schedule_Route_Computation(Simulation_Timestep_Increment time_to_depart, Simulation_Timestep_Increment planning_time = 0)
			{
				departure_time<Simulation_Timestep_Increment>(time_to_depart);
				this_component()->Schedule_Route_Computation(time_to_depart, planning_time);
			}

			template<typename Movement_Plan_Type>
			void Attach_New_Movement_Plan(Movement_Plan<Movement_Plan_Type>* mp)
			{
				this_component()->Attach_New_Movement_Plan<Movement_Plan_Type>(mp);
			}

			template<typename T> T Get_Tree_Results_For_Destination(int destination_internal_id, requires(T,check(strip_modifiers(T),Basic_Units::Concepts::Is_Time_Value)))
			{
				// get reference to travel times
				typedef Random_Access_Sequence<typename get_type_of(travel_times_to_link_container)> travel_times_itf;

				travel_times_itf* travel_times = this->travel_times_to_link_container<travel_times_itf*>();

				// return travel time to destion in requested time units
				return Time<Basic_Time>::Convert_Value<Simulation_Timestep_Increment, T>(travel_times->at(destination_internal_id));
			}
			
			accessor(parent_skimmer, NONE, NONE);
			accessor(network, NONE, NONE);
			accessor(origin_link, NONE,NONE);
			accessor(movement_plan, NONE, NONE);
			accessor(departure_time, NONE, NONE); // the time at which routing is triggered.
			accessor(update_increment, NONE, NONE);
			accessor(start_time, NONE, NONE);
			accessor(end_time, NONE, NONE);
			accessor(travel_times_to_link_container, NONE, NONE);
		};

//		prototype struct Routing ADD_DEBUG_INFO
//		{
//			tag_as_prototype;
//
//			accessor(network, NONE, NONE);
//			accessor(traveler, NONE, NONE);
//			accessor(vehicle, NONE, NONE);
//			accessor(movement_plan, NONE, NONE);
//			accessor(routable_network, NONE, NONE);
//			accessor(realtime_routable_network, NONE, NONE);
//			accessor(routable_origin, NONE, NONE);
//			accessor(routable_destination, NONE, NONE);
//			accessor(routable_origin_index, NONE, NONE);
//			accessor(routable_destination_index, NONE, NONE);
//			accessor(update_increment, NONE, NONE);
//			accessor(start_time, NONE, NONE);
//			accessor(end_time, NONE, NONE);
//			accessor(travel_times_to_link_container, NONE, NONE);
//			accessor(departure_time, NONE, NONE); // the time at which routing is triggered.
///*#ifndef EXCLUDE_DEMAND
//			template<typename TargetType> void Evaluate_Condition(Event_Response& response, requires(TargetType,check(strip_modifiers(TargetType),Concepts::Is_One_To_One_Router)))
//			{
//				response.result=true;
//				response.next._iteration=END;
//				response.next._sub_iteration=Scenario_Components::Types::Type_Sub_Iteration_keys::ROUTING_SUB_ITERATION;
//			}
//			template<typename TargetType> void Evaluate_Condition(Event_Response& response, requires(TargetType,check(strip_modifiers(TargetType),Concepts::Is_One_To_All_Router)))
//			{
//				if (iteration() >= _this_ptr->template start_time<Simulation_Timestep_Increment>() && iteration() < _this_ptr->end_time<Simulation_Timestep_Increment>())
//				{
//					response.result=true;
//					response.next._iteration=Simulation_Time.Future_Time<Simulation_Timestep_Increment,Simulation_Timestep_Increment>(_this_ptr->update_increment<Simulation_Timestep_Increment>());
//					response.next._sub_iteration=Network_Skimming_Components::Types::SUB_ITERATIONS::PATH_BUILDING;
//				}
//				else
//				{
//					response.result=false;
//					response.next._iteration=Simulation_Time.Future_Time<Simulation_Timestep_Increment,Simulation_Timestep_Increment>(_this_ptr->update_increment<Simulation_Timestep_Increment>());
//					response.next._sub_iteration=Network_Skimming_Components::Types::SUB_ITERATIONS::PATH_BUILDING;
//				}
//			}
//			template<typename TargetType> void Evaluate_Condition(Event_Response& response, requires(TargetType,check(strip_modifiers(TargetType),!Concepts::Is_One_To_All_Router) && check(strip_modifiers(TargetType),!Concepts::Is_One_To_One_Router)))
//			{
//				assert_check(strip_modifiers(TargetType),Concepts::Is_One_To_All_Router, "ControlType is not a one-to-all router");
//				assert_check(strip_modifiers(TargetType),Concepts::Is_One_To_One_Router, "ControlType is not a one-to-one router");
//				assert_sub_check(strip_modifiers(TargetType),Concepts::Is_One_To_One_Router,is_one_to_one_prototype, "ControlType has no one-to-one prototype");
//				assert_sub_check(strip_modifiers(TargetType),Concepts::Is_One_To_One_Router,has_routable_origin, "ControlType has no routable_origin");
//				assert_sub_check(strip_modifiers(TargetType),Concepts::Is_One_To_One_Router,has_routable_destination, "ControlType has no routable_destination");
//				assert_sub_check(strip_modifiers(TargetType),Concepts::Is_One_To_One_Router,has_network, "ControlType has no network");
//				assert_sub_check(strip_modifiers(TargetType),Concepts::Is_One_To_One_Router,has_vehicle, "ControlType has no vehicle");
//			}
//#endif*/
//			template<typename TargetType> float one_to_one_link_based_least_time_path_a_star(TargetType routable_net, std::vector<float>& reversed_arrival_time_container)
//			{
//
//				typedef  Network_Components::Prototypes::Network< typename get_type_of(routable_network)> _Routable_Network_Interface;
//				typedef  Network_Components::Prototypes::Network< typename get_type_of(network)> _Regular_Network_Interface;
//				typedef  Link_Components::Prototypes::Link<typename remove_pointer< typename _Routable_Network_Interface::get_type_of(links_container)::value_type>::type>  _Routable_Link_Interface;
//				typedef  Random_Access_Sequence< typename _Routable_Network_Interface::get_type_of(links_container), _Routable_Link_Interface*> _Routable_Links_Container_Interface;
//
//				//typedef Link_Interface<typename ComponentType::routable_link_type> _Routable_Link_Interface;
//				typedef  Intersection_Components::Prototypes::Intersection< typename ComponentType::routable_link_type::upstream_intersection_type> _Routable_Intersection_Interface;
//				typedef  Intersection_Components::Prototypes::Inbound_Outbound_Movements<typename remove_pointer< typename _Routable_Intersection_Interface::get_type_of(inbound_outbound_movements)::value_type>::type>  _Inbound_Outbound_Movements_Interface;
//				typedef  Random_Access_Sequence< typename _Routable_Intersection_Interface::get_type_of(inbound_outbound_movements), _Inbound_Outbound_Movements_Interface*> _Inbound_Outbound_Movements_Container_Interface;
//
//				typedef  Turn_Movement_Components::Prototypes::Movement<typename remove_pointer< typename _Inbound_Outbound_Movements_Interface::get_type_of(outbound_movements)::value_type>::type>  _Movement_Interface;
//				typedef  Random_Access_Sequence< typename _Inbound_Outbound_Movements_Interface::get_type_of(outbound_movements), _Movement_Interface*> _Movements_Container_Interface;
//
//				typedef  Link_Components::Prototypes::Link<typename remove_pointer< typename _Routable_Network_Interface::get_type_of(reversed_path_container)::value_type>::type>  _Regular_Link_Interface;
//				typedef  Random_Access_Sequence< typename _Routable_Network_Interface::get_type_of(reversed_path_container), _Regular_Link_Interface*> _Reversed_Path_Container_Interface;
//
//				typedef typename _Routable_Network_Interface::get_type_of(scan_list) ScanListType;
//
//				//_Routable_Network_Interface* routable_net=routable_network<_Routable_Network_Interface*>();
//				routable_net->template reset_routable_network<NULLTYPE>();
//				typedef  Vehicle_Components::Prototypes::Vehicle< typename get_type_of(vehicle)> _Vehicle_Interface;
//					
//				//RoutableNetworkInterface* routable_network=routable_network<RoutableNetworkInterface*>();
//				float max_free_flow_speed=routable_net->template max_free_flow_speed<float>();
//				ScanListType& scan_list=routable_net->template scan_list<ScanListType&>();
//				//NumSearchesType& num_searches=routable_network->template num_searches<NumSearchesType&>();
//				int origin_index = routable_origin_index<int>();
//				int destination_index = routable_destination_index<int>();
//				_Routable_Link_Interface* origin_link_ptr=routable_net->template links_container<_Routable_Links_Container_Interface&>()[origin_index];
//				_Routable_Link_Interface* destination_link_ptr=routable_net->template links_container<_Routable_Links_Container_Interface&>()[destination_index];
//				_Regular_Link_Interface* destination_reference=destination_link_ptr->template network_link_reference<_Regular_Link_Interface*>();
//				
//				_Regular_Link_Interface* net_origin_link=origin_link_ptr->template network_link_reference<_Regular_Link_Interface*>();
//				_Movements_Container_Interface& turn_mvmt_container=net_origin_link->template outbound_turn_movements<_Movements_Container_Interface&>();
//				int outbound_turn_movement_size = (int)turn_mvmt_container.size();
//				if (origin_link_ptr != destination_link_ptr && outbound_turn_movement_size == 0)
//				{
//					THROW_WARNING("Origin link 'index="<<net_origin_link->template internal_id<int>() <<" id="<<net_origin_link->template uuid<int>()<<"' has no outbound turn movements, trip not able to be routed.");
//					return -1.0;
//				}
//
//				float next_cost,new_cost;
//				float dx,dy;
//				float destination_x,destination_y;
//
//				_Routable_Intersection_Interface* destination_upstream_intersection=destination_link_ptr->template upstream_intersection<_Routable_Intersection_Interface*>();
//
//				destination_x = destination_upstream_intersection->template x_position<float>();
//				destination_y = destination_upstream_intersection->template y_position<float>();
//				
//				//next_cost=origin_link_ptr->template network_link_reference<_Regular_Link_Interface*>()->template travel_time<float>();
//				next_cost= 0.0f;
//				new_cost = next_cost;
//				_Routable_Link_Interface* current_link=origin_link_ptr;
//
//
//				std::vector<_Routable_Link_Interface*>& reset_links_container=routable_net->template reset_links< std::vector<_Routable_Link_Interface*>& >();
//				if(current_link->template reset_list_status<bool>()==0)
//				{
//					reset_links_container.push_back(current_link);
//					current_link->template reset_list_status<bool>(1);
//				}
//
//				current_link->template label_cost<float>(new_cost); // g - label_cost
//
//				if(current_link->template network_link_reference<_Regular_Link_Interface*>() != destination_reference)
//				{
//					_Routable_Intersection_Interface* current_intersection=current_link->template upstream_intersection<_Routable_Intersection_Interface*>();
//					dx = destination_x - current_intersection->template x_position<float>();
//					dy = destination_y - current_intersection->template y_position<float>();
//					current_link->template h_cost<float>(sqrt(dx*dx+dy*dy)/max_free_flow_speed); // h - estimated = sqrt(dx*dx+dy*dy)/max_free_flow_speed 
//					current_link->template f_cost<float>(current_link->template label_cost<float>()+current_link->template h_cost<float>()); // f = g + h;
//				}
//				else
//				{
//					return -1.0;
//				}
//
//				current_link->template label_pointer<_Routable_Link_Interface*>(current_link);
//				scan_list.insert(make_pair(current_link->template f_cost<float>(),current_link));
//				current_link->template scan_list_status<Network_Components::Types::Scan_List_Status_Keys>(Network_Components::Types::INSELIST);
//
//				while(!scan_list.empty())
//				{ 
//
//					current_link = (_Routable_Link_Interface*)(scan_list.begin()->second);
//
//					if(current_link->template reset_list_status<bool>()==0)
//					{
//						reset_links_container.push_back(current_link);
//						current_link->template reset_list_status<bool>(1);
//					}
//
//					if(current_link == destination_link_ptr)
//					{
//						break;
//					}
//
//					scan_list.erase(scan_list.begin());	// no need to find minimum since set is ordered by its first element, i.e. label cost.
//					current_link->template scan_list_status<Network_Components::Types::Scan_List_Status_Keys>(Network_Components::Types::SCANNED);
//
//					_Movements_Container_Interface& outbound_movements_container = current_link->template outbound_turn_movements<_Movements_Container_Interface&>();
//					typename _Movements_Container_Interface::iterator outbound_itr;
//
//					for(outbound_itr=outbound_movements_container.begin(); outbound_itr!=outbound_movements_container.end(); outbound_itr++)
//					{
//						_Movement_Interface* outbound_movement = (_Movement_Interface*)(*outbound_itr);
//						_Routable_Link_Interface* next_link=outbound_movement->template outbound_link<_Routable_Link_Interface*>();
//						int next_link_id=next_link->template network_link_reference<_Regular_Link_Interface*>()->template internal_id<int>();
//
//
//						next_cost=outbound_movement->template forward_link_turn_travel_time<float>();
//						new_cost=current_link->template label_cost<float>() + next_cost;
//
//						if(next_link->template label_cost<float>()>new_cost)
//						{
//							if(next_link->template scan_list_status<Network_Components::Types::Scan_List_Status_Keys>()==Network_Components::Types::INSELIST)
//							{
//								scan_list.erase(make_pair(next_link->template f_cost<float>(),next_link)); // delete the old cost
//							}
//
//							if(next_link->template reset_list_status<bool>()==0)
//							{
//								reset_links_container.push_back(next_link);
//								next_link->template reset_list_status<bool>(1);
//							}
//
//
//							next_link->template label_cost<float>(new_cost);
//							next_link->template label_pointer<_Routable_Link_Interface*>(current_link);
//
//							if(next_link->template network_link_reference<_Regular_Link_Interface*>()!=destination_reference)
//							{
//								dx=destination_x - next_link->template upstream_intersection<_Routable_Intersection_Interface*>()->template x_position<float>();
//								dy=destination_y - next_link->template upstream_intersection<_Routable_Intersection_Interface*>()->template y_position<float>();
//								next_link->template h_cost<float>(sqrt(dx*dx+dy*dy)/max_free_flow_speed);
//							}
//							else
//							{
//								next_link->template h_cost<float>(0.0);
//							}
//
//							next_link->template f_cost<float>(next_link->template label_cost<float>() + next_link->template h_cost<float>());
//
//							scan_list.insert(make_pair(next_link->template f_cost<float>(),next_link)); // update with the new cos
//
//							next_link->template scan_list_status<Network_Components::Types::Scan_List_Status_Keys>(Network_Components::Types::INSELIST);
//						}
//					}
//				}
//				if (destination_link_ptr->template label_pointer<_Routable_Link_Interface*>() == destination_link_ptr)
//				{
//					return -1.0;
//				}
//				else
//				{
//
//					_Reversed_Path_Container_Interface& reversed_path_container=routable_net->template reversed_path_container<_Reversed_Path_Container_Interface&>();
//
//					current_link=destination_link_ptr;
//				
//					while (true)
//					{
//						reversed_path_container.push_back(current_link->template network_link_reference<_Regular_Link_Interface*>());
//
//						reversed_arrival_time_container.push_back(current_link->template label_cost<float>());
//						if (current_link->template label_pointer<_Routable_Link_Interface*>() != current_link)
//						{
//							current_link=current_link->template label_pointer<_Routable_Link_Interface*>();
//						}
//						else
//						{
//							break;
//						}
//						
//					}
//
//					float route_travel_time = destination_link_ptr->template label_cost<float>();// + destination_link_ptr->template network_link_reference<_Regular_Link_Interface*>()->template travel_time<float>();
//					return route_travel_time; 
//				}
//
//			};
//
//			template<typename TargetType> bool one_to_all_link_based_least_time_path_label_setting()
//			{
//
//				typedef  Network_Components::Prototypes::Network< typename get_type_of(routable_network)> _Routable_Network_Interface;
//				typedef  Network_Components::Prototypes::Network< typename get_type_of(network)> _Regular_Network_Interface;
//				typedef  Link_Components::Prototypes::Link<typename remove_pointer< typename _Routable_Network_Interface::get_type_of(links_container)::value_type>::type>  _Routable_Link_Interface;
//				typedef  Random_Access_Sequence< typename _Routable_Network_Interface::get_type_of(links_container), _Routable_Link_Interface*> _Routable_Links_Container_Interface;
//
//				//typedef Link_Interface<typename ComponentType::routable_link_type> _Routable_Link_Interface;
//				typedef  Intersection_Components::Prototypes::Intersection< typename ComponentType::routable_link_type::upstream_intersection_type> _Routable_Intersection_Interface;
//				typedef  Intersection_Components::Prototypes::Inbound_Outbound_Movements<typename remove_pointer< typename _Routable_Intersection_Interface::get_type_of(inbound_outbound_movements)::value_type>::type>  _Inbound_Outbound_Movements_Interface;
//				typedef  Random_Access_Sequence< typename _Routable_Intersection_Interface::get_type_of(inbound_outbound_movements), _Inbound_Outbound_Movements_Interface*> _Inbound_Outbound_Movements_Container_Interface;
//
//				typedef  Turn_Movement_Components::Prototypes::Movement<typename remove_pointer< typename _Inbound_Outbound_Movements_Interface::get_type_of(outbound_movements)::value_type>::type>  _Movement_Interface;
//				typedef  Random_Access_Sequence< typename _Inbound_Outbound_Movements_Interface::get_type_of(outbound_movements), _Movement_Interface*> _Movements_Container_Interface;
//
//				typedef  Link_Components::Prototypes::Link<typename remove_pointer< typename _Routable_Network_Interface::get_type_of(reversed_path_container)::value_type>::type>  _Regular_Link_Interface;
//				typedef  Random_Access_Sequence< typename _Routable_Network_Interface::get_type_of(reversed_path_container), _Regular_Link_Interface*> _Reversed_Path_Container_Interface;
//
//				typedef typename _Routable_Network_Interface::get_type_of(scan_list) ScanListType;
//
//				_Routable_Network_Interface* routable_net=routable_network<_Routable_Network_Interface*>();
//				routable_net->template reset_routable_network<NULLTYPE>();
//				
//				//RoutableNetworkInterface* routable_network=routable_network<RoutableNetworkInterface*>();
//				float max_free_flow_speed=routable_net->template max_free_flow_speed<float>();
//				ScanListType& scan_list=routable_net->template scan_list<ScanListType&>();
//				//NumSearchesType& num_searches=routable_network->template num_searches<NumSearchesType&>();
//
//				_Routable_Link_Interface* origin_link_ptr=routable_origin<_Routable_Link_Interface*>();
//				
//				_Regular_Link_Interface* net_origin_link=origin_link_ptr->template network_link_reference<_Regular_Link_Interface*>();
//				_Movements_Container_Interface& turn_mvmt_container=net_origin_link->template outbound_turn_movements<_Movements_Container_Interface&>();
//				int outbound_turn_movement_size = (int)turn_mvmt_container.size();
//				
//				if (outbound_turn_movement_size == 0)
//				{
//					THROW_WARNING("Origin link 'index="<<net_origin_link->template internal_id<int>() <<" id="<<net_origin_link->template uuid<int>()<<"' has no outbound turn movements, trip not able to be routed.");
//					return false;
//				}
//
//				float next_cost,new_cost;
//
//
//				//next_cost=origin_link_ptr->template network_link_reference<_Regular_Link_Interface*>()->template travel_time<float>();
//				next_cost= 0.0f;
//				new_cost = next_cost;
//				_Routable_Link_Interface* current_link=origin_link_ptr;
//
//				std::vector<_Routable_Link_Interface*>& reset_links_container=routable_net->template reset_links< std::vector<_Routable_Link_Interface*>& >();
//				if(current_link->template reset_list_status<bool>()==0)
//				{
//					reset_links_container.push_back(current_link);
//					current_link->template reset_list_status<bool>(1);
//				}
//				current_link->template label_cost<float>(new_cost); // g - label_cost
//
//				current_link->template label_pointer<_Routable_Link_Interface*>(current_link);
//
//				scan_list.insert(make_pair(current_link->template label_cost<float>(),current_link));
//				current_link->template scan_list_status<Network_Components::Types::Scan_List_Status_Keys>(Network_Components::Types::INSELIST);
//				
//				while(!scan_list.empty())
//				{ 
//
//					current_link = (_Routable_Link_Interface*)(scan_list.begin()->second);
//
//					if(current_link->template reset_list_status<bool>()==0)
//					{
//						reset_links_container.push_back(current_link);
//						current_link->template reset_list_status<bool>(1);
//					}
//
//					scan_list.erase(scan_list.begin());	// no need to find minimum since set is ordered by its first element, i.e. label cost.
//					current_link->template scan_list_status<Network_Components::Types::Scan_List_Status_Keys>(Network_Components::Types::SCANNED);
//
//					//for all outbound turn movements
//					_Movements_Container_Interface& outbound_movements_container = current_link->template outbound_turn_movements<_Movements_Container_Interface&>();
//					typename _Movements_Container_Interface::iterator outbound_itr;
//
//					for(outbound_itr=outbound_movements_container.begin(); outbound_itr!=outbound_movements_container.end(); outbound_itr++)
//					{
//						_Movement_Interface* outbound_movement = (_Movement_Interface*)(*outbound_itr);
//						_Routable_Link_Interface* next_link=outbound_movement->template outbound_link<_Routable_Link_Interface*>();
//						int next_link_id=next_link->template network_link_reference<_Regular_Link_Interface*>()->template internal_id<int>();
//
//						if(next_link->template reset_list_status<bool>()==0)
//						{
//							reset_links_container.push_back(next_link);
//							next_link->template reset_list_status<bool>(1);
//						}
//						next_cost=outbound_movement->template forward_link_turn_travel_time<float>();
//						new_cost=current_link->template label_cost<float>() + next_cost;
//
//						if(next_link->template label_cost<float>()>new_cost)
//						{
//							if(next_link->template scan_list_status<Network_Components::Types::Scan_List_Status_Keys>()==Network_Components::Types::INSELIST)
//							{
//								scan_list.erase(make_pair(next_link->template label_cost<float>(),next_link)); // delete the old cost
//							}
//
//							next_link->template label_cost<float>(new_cost);
//							next_link->template label_pointer<_Routable_Link_Interface*>(current_link);
//
//							scan_list.insert(make_pair(next_link->template label_cost<float>(),next_link)); // update with the new cos
//
//							next_link->template scan_list_status<Network_Components::Types::Scan_List_Status_Keys>(Network_Components::Types::INSELIST);
//						}
//					}
//				}
//				return true;
//			};
//
//			template<typename TargetType> void Schedule_Route_Computation(int time_to_depart)
//			{
//				typedef Scenario<typename Component_Type::Master_Type::scenario_type> _Scenario_Interface;
//				if (((_Scenario_Interface*)_global_scenario)->template routing_with_snapshots<bool>())
//				{
//					departure_time<int>(time_to_depart);
//					//TODO
////load_event(ComponentType,ComponentType::template Compute_Route_Condition,Compute_Route_Using_Snapshot,time_to_depart,Scenario_Components::Types::Type_Sub_Iteration_keys::ROUTING_SUB_ITERATION,NULLTYPE);
//				}
//				else
//				{
//					//TODO
////load_event(ComponentType,ComponentType::template Compute_Route_Condition,Compute_Route,time_to_depart,Scenario_Components::Types::Type_Sub_Iteration_keys::ROUTING_SUB_ITERATION,NULLTYPE);
//					
//					this_component()->template Load_Event<ComponentType>(&ComponentType::Compute_Route_Condition,time_to_depart,Scenario_Components::Types::Type_Sub_Iteration_keys::ROUTING_SUB_ITERATION);
//				}
//			}
//
//			template<typename TargetType> void Schedule_Route_Computation(int time_to_depart, int planning_time, bool use_snapshot)
//			{
//				departure_time<int>(time_to_depart);
//				if (use_snapshot) //TODO
////load_event(ComponentType,ComponentType::template Compute_Route_Condition,Compute_Route_Using_Snapshot,planning_time,Scenario_Components::Types::Type_Sub_Iteration_keys::ROUTING_SUB_ITERATION,NULLTYPE);
//				else
//				{
//					//TODO
////load_event(ComponentType,ComponentType::template Compute_Route_Condition,Compute_Route,planning_time,Scenario_Components::Types::Type_Sub_Iteration_keys::ROUTING_SUB_ITERATION,NULLTYPE);
//					
//					this_component()->template Load_Event<ComponentType>(&ComponentType::Compute_Route_Condition,time_to_depart,Scenario_Components::Types::Type_Sub_Iteration_keys::ROUTING_SUB_ITERATION);
//				}
//			}
//#ifndef EXCLUDE_DEMAND
//			template<typename TargetType> void Initialize_Tree_Computation(int departed_time)
//			{
//				// initialize the containers for skimmed travel times to links
//				typedef  Network_Components::Prototypes::Network< typename get_type_of(routable_network)> _Routable_Network_Interface;
//				typedef  Network_Components::Prototypes::Network< typename get_type_of(network)> _Regular_Network_Interface;
//				typedef  Link_Components::Prototypes::Link<typename remove_pointer< typename _Routable_Network_Interface::get_type_of(links_container)::value_type>::type>  _Routable_Link_Interface;
//				typedef  Random_Access_Sequence< typename _Routable_Network_Interface::get_type_of(links_container), _Routable_Link_Interface*> _Routable_Links_Container_Interface;
//
//				typedef (travel_times_itf, typename get_type_of(travel_times_to_link_container),Random_Access_Sequence, typename get_type_of(travel_times_to_link_container)::value_type);
//
//				travel_times_itf* travel_times = this->template travel_times_to_link_container<travel_times_itf*>();
//				_Routable_Network_Interface* routable_net= this->template routable_network<_Routable_Network_Interface*>();
//				travel_times->resize(routable_net->template links_container<_Routable_Links_Container_Interface*>()->size(),0);
//
//				typedef  Network_Components::Prototypes::Network< typename get_type_of(network)> _Regular_Network_Interface;
//				typedef  Scenario_Components::Prototypes::Scenario< typename _Regular_Network_Interface::get_type_of(scenario_reference)> _Scenario_Interface;
//				//TODO
////load_event(ComponentType,ComponentType::template Compute_Route_Condition,Compute_Tree,departed_time,Network_Skimming_Components::Types::SUB_ITERATIONS::PATH_BUILDING,NULLTYPE);
//			}
//
//			template<typename TargetType> TargetType Get_Tree_Results_For_Destination(int destination_internal_id, requires(TargetType,check(strip_modifiers(TargetType),Basic_Units::Concepts::Is_Time_Value)))
//			{
//				// get reference to travel times
//				typedef (travel_times_itf, typename get_type_of(travel_times_to_link_container),Random_Access_Sequence, typename get_type_of(travel_times_to_link_container)::value_type);
//				travel_times_itf* travel_times = this->travel_times_to_link_container<travel_times_itf*>();
//
//				// return travel time to destion in requested time units
//				return GLOBALS::Time_Converter.Convert_Value<Simulation_Timestep_Increment,TargetType>(travel_times->at(destination_internal_id));
//			}
//#endif
//			//first event
//			template<typename TargetType> void Compute_Route()
//			{
//				typedef Routing_Components::Prototypes::Routing<ComponentType> _Routing_Interface;
//				typedef Link_Components::Prototypes::Link<typename ComponentType::routable_link_type> _Routable_Link_Interface;
//				_Routing_Interface* _this_ptr=(_Routing_Interface*)this;
//				typedef Network_Components::Prototypes::Network<typename ComponentType::routable_network_type> _Routable_Network_Interface;
//				typedef  Network_Components::Prototypes::Network< typename get_type_of(network)> _Regular_Network_Interface;
//				typedef  Scenario_Components::Prototypes::Scenario< typename _Regular_Network_Interface::get_type_of(scenario_reference)> _Scenario_Interface;
//				typedef  Traveler_Components::Prototypes::Traveler< typename get_type_of(traveler)> _Traveler_Interface;
//				typedef  Vehicle_Components::Prototypes::Vehicle< typename _Traveler_Interface::get_type_of(vehicle)> _Vehicle_Interface;
//				//typedef  Movement_Plan_Components::Prototypes::Movement_Plan< typename _Vehicle_Interface::get_type_of(movement_plan)> _Movement_Plan_Interface;
//				typedef  Movement_Plan_Components::Prototypes::Movement_Plan< typename get_type_of(movement_plan)> _Movement_Plan_Interface;
//				typedef  Link_Components::Prototypes::Link<typename remove_pointer< typename ComponentType::routable_network_type::type_of(reversed_path_container)::value_type>::type>  _Regular_Link_Interface;
//				typedef  Random_Access_Sequence< typename ComponentType::routable_network_type::type_of(reversed_path_container), _Regular_Link_Interface*> _Reversed_Path_Container_Interface;
//
//				
//				//_Vehicle_Interface* veh = _this_ptr->template vehicle<_Vehicle_Interface*>();
//				//_Movement_Plan_Interface* mp= veh->template movement_plan<_Movement_Plan_Interface*>();
//
//				_Movement_Plan_Interface* mp= _this_ptr->template movement_plan<_Movement_Plan_Interface*>();
//
//				_Regular_Link_Interface* origin_link=mp->template origin<_Regular_Link_Interface*>();
//				_Regular_Link_Interface* destination_link=mp->template destination<_Regular_Link_Interface*>();
//
//				_this_ptr->template routable_origin<_Regular_Link_Interface*>(origin_link);
//				_this_ptr->template routable_destination<_Regular_Link_Interface*>(destination_link);
//				
//				_Routable_Network_Interface* routable_network_ptr=_this_ptr->template routable_network<_Routable_Network_Interface*>();
//				std::vector<float> reversed_arrival_time_container;
//				float routed_travel_time = _this_ptr->template one_to_one_link_based_least_time_path_a_star<_Routable_Network_Interface*>(routable_network_ptr, reversed_arrival_time_container);
//
//				if (routed_travel_time >= 0.0)
//				{	
//
//					mp->template valid_trajectory<bool>(true);
//					mp->template routed_travel_time<float>(routed_travel_time);
//					mp->template estimated_time_of_arrival<Simulation_Timestep_Increment>(mp->template absolute_departure_time<int>() + routed_travel_time);
//					mp->template estimated_travel_time_when_departed<float>(routed_travel_time);
//					// print out and break when scheduling departure for veh_id 101319
//					if (routable_network_ptr->template reversed_path_container<_Reversed_Path_Container_Interface&>().size() == 0)
//					{
//						THROW_WARNING(endl << "Error: path size is: " << routable_network_ptr->template reversed_path_container<_Reversed_Path_Container_Interface&>().size() << endl);
//						THROW_EXCEPTION(endl << "no path between origin link uuid " << origin_link->template uuid<int>() << " and destination link uuid " << destination_link->template uuid<int>());
//					}
//
//					mp->template set_trajectory<_Reversed_Path_Container_Interface>(routable_network_ptr->template reversed_path_container<_Reversed_Path_Container_Interface&>(), reversed_arrival_time_container);
//
//				}
//			}
//
//			template<typename TargetType> void write_path()
//			{
//				typedef  Traveler_Components::Prototypes::Traveler< typename get_type_of(traveler)> _Traveler_Interface;
//				typedef  Vehicle_Components::Prototypes::Vehicle< typename _Traveler_Interface::get_type_of(vehicle)> _Vehicle_Interface;
//				typedef  Movement_Plan_Components::Prototypes::Movement_Plan< typename get_type_of(movement_plan)> _Movement_Plan_Interface;
//				typedef  Movement_Plan_Components::Prototypes::Trajectory_Unit<typename remove_pointer< typename _Movement_Plan_Interface::get_type_of(trajectory_container)::value_type>::type>  _Trajectory_Unit_Interface;
//				typedef  Random_Access_Sequence< typename _Movement_Plan_Interface::get_type_of(trajectory_container), _Trajectory_Unit_Interface*> _Trajecotry_Container_Interface;
//
//                typedef  Link_Components::Prototypes::Link< typename _Trajectory_Unit_Interface::get_type_of(link)> _Link_Interface;
//				typedef Scenario<typename Component_Type::Master_Type::scenario_type> _Scenario_Interface;
//				_Movement_Plan_Interface* mp= movement_plan<_Movement_Plan_Interface*>();
//								
//						_Vehicle_Interface* v_ptr = vehicle<_Vehicle_Interface*>();
//
//						int vehicle_id = v_ptr->template uuid<int>();
//						int origin_zone_index = 0;
//						int destination_zone_index = 0;
//						int origin_activity_location_index = 0;
//						int destination_activity_location_index = 0;
//						int origin_link_index = mp->template origin<_Link_Interface*>()->template uuid<int>();
//						int destination_link_index = mp->template destination<_Link_Interface*>()->template uuid<int>();
//						int num_links = (int)mp->template trajectory_container<_Trajecotry_Container_Interface&>().size();
//
//						int departure_time = mp->template departed_time<Time_Seconds>();
//						int arrival_time = mp->template arrived_time<Time_Seconds>();
//						float travel_time = float ((arrival_time - departure_time)/60.0f);
//
//						fstream& routed_path_file = ((_Scenario_Interface*)_global_scenario)->template routed_path_file<fstream&>();			
//						routed_path_file
//							<< vehicle_id << ","
//							<< 0 << ","
//							<< 0 << ","
//							<< 0 << ","
//							<< 0 << ","
//							<< origin_link_index << ","
//							<< destination_link_index << ","
//							<< num_links << ","
//							<< convert_seconds_to_hhmmss(departure_time)<< ","
//							<< convert_seconds_to_hhmmss(arrival_time)<< ","
//							<< travel_time
//							<<endl;
//
//						float path_delayed_time = 0;
//						for (int route_link_counter=0;route_link_counter<num_links;route_link_counter++)
//						{
//
//							_Trajectory_Unit_Interface* trajectory_unit = mp->template trajectory_container<_Trajecotry_Container_Interface&>()[route_link_counter];
//							_Link_Interface* route_link = trajectory_unit->template link<_Link_Interface*>();
//							int route_link_id = route_link->template uuid<int>();
//							int route_link_enter_time = trajectory_unit->template enter_time<int>();
//							float route_link_delayed_time = float(trajectory_unit->template delayed_time<float>()/60.0f);
//								
//							int route_link_exit_time = mp->template get_route_link_exit_time<NULLTYPE>(route_link_counter);
//							float route_link_travel_time = float((route_link_exit_time - route_link_enter_time)/60.0f);
//
//							path_delayed_time+=route_link_delayed_time;
//					
//							routed_path_file
//								<<route_link_counter + 1 << ","
//								<<route_link_id << ","
//								<<convert_seconds_to_hhmmss(route_link_enter_time) << ","
//								<<route_link_travel_time << ","
//								<<route_link_delayed_time
//								<<endl;
//						}
//			}
//
//			declare_event(Compute_Route_Using_Snapshot)
//			{
//				typedef Routing_Components::Prototypes::Routing<ComponentType> _Routing_Interface;
//				typedef Link_Components::Prototypes::Link<typename ComponentType::routable_link_type> _Routable_Link_Interface;
//				typedef Network_Components::Prototypes::Network<typename ComponentType::network_type> _Regular_Network_Interface;
//				_Routing_Interface* _this_ptr=(_Routing_Interface*)_this;
//				typedef Network_Components::Prototypes::Network<typename ComponentType::routable_network_type> _Routable_Network_Interface;
//				typedef  Network_Components::Prototypes::Network< typename get_type_of(network)> _Regular_Network_Interface;
//				typedef  Scenario_Components::Prototypes::Scenario< typename _Regular_Network_Interface::get_type_of(scenario_reference)> _Scenario_Interface;
//				typedef  Traveler_Components::Prototypes::Traveler< typename get_type_of(traveler)> _Traveler_Interface;
//				typedef  Vehicle_Components::Prototypes::Vehicle< typename _Traveler_Interface::get_type_of(vehicle)> _Vehicle_Interface;
//				typedef  Movement_Plan_Components::Prototypes::Movement_Plan< typename get_type_of(movement_plan)> _Movement_Plan_Interface;
//				typedef  Link_Components::Prototypes::Link<typename remove_pointer< typename ComponentType::routable_network_type::type_of(reversed_path_container)::value_type>::type>  _Regular_Link_Interface;
//				typedef  Random_Access_Sequence< typename ComponentType::routable_network_type::type_of(reversed_path_container), _Regular_Link_Interface*> _Reversed_Path_Container_Interface;
//
//				
//				_Movement_Plan_Interface* mp= _this_ptr->template movement_plan<_Movement_Plan_Interface*>();
//
//				_Regular_Link_Interface* origin_link=mp->template origin<_Regular_Link_Interface*>();
//				_Regular_Link_Interface* destination_link=mp->template destination<_Regular_Link_Interface*>();
//				
//				_this_ptr->template routable_origin<_Regular_Link_Interface*>(origin_link);
//				_this_ptr->template routable_destination<_Regular_Link_Interface*>(destination_link);
//				
//				_Routable_Network_Interface* routable_network_ptr=((_Regular_Network_Interface*)_global_network)->template get_routable_network_from_snapshots<_Routable_Network_Interface*>(_this_ptr->template departure_time<int>());
//				std::vector<float> reversed_arrival_time_container;
//				float routed_travel_time = _this_ptr->template one_to_one_link_based_least_time_path_a_star<_Routable_Network_Interface*>(routable_network_ptr, reversed_arrival_time_container);
//				
//				if (routed_travel_time >= 0.0)
//				{	
//
//					mp->template valid_trajectory<bool>(true);
//					mp->template routed_travel_time<float>(routed_travel_time);
//					mp->template estimated_time_of_arrival<Simulation_Timestep_Increment>(mp->template absolute_departure_time<int>() + routed_travel_time);
//
//					// print out and break when scheduling departure for veh_id 101319
//					if (routable_network_ptr->template reversed_path_container<_Reversed_Path_Container_Interface&>().size() == 0)
//					{
//						THROW_WARNING(endl << "Error: path size is: " << routable_network_ptr->template reversed_path_container<_Reversed_Path_Container_Interface&>().size() << endl);
//						THROW_EXCEPTION(endl << "no path between origin link uuid " << origin_link->template uuid<int>() << " and destination link uuid " << destination_link->template uuid<int>());
//					}
//
//					mp->template set_trajectory<_Reversed_Path_Container_Interface>(routable_network_ptr->template reversed_path_container<_Reversed_Path_Container_Interface&>(), reversed_arrival_time_container);
//					//_this_ptr->template write_path<NULLTYPE>();
//				}
//			}
//
//			declare_event(Compute_Tree)
//			{
//				typedef Routing_Components::Prototypes::Routing<ComponentType> _Routing_Interface;
//				//typedef Link_Components::Prototypes::Link<typename ComponentType::routable_link_type> _Routable_Link_Interface;
//				
//				_Routing_Interface* _this_ptr=(_Routing_Interface*)_this;
//				typedef Network_Components::Prototypes::Network<typename ComponentType::routable_network_type> _Routable_Network_Interface;
//				typedef  Link_Components::Prototypes::Link<typename remove_pointer< typename _Routable_Network_Interface::get_type_of(links_container)::value_type>::type>  _Routable_Link_Interface;
//				typedef  Random_Access_Sequence< typename _Routable_Network_Interface::get_type_of(links_container), _Routable_Link_Interface*> _Routable_Links_Container_Interface;
//
//				typedef  Network_Components::Prototypes::Network< typename get_type_of(network)> _Regular_Network_Interface;
//				typedef  Scenario_Components::Prototypes::Scenario< typename _Regular_Network_Interface::get_type_of(scenario_reference)> _Scenario_Interface;
//				
//				_Routable_Link_Interface* origin_link = _this_ptr->routable_origin<_Routable_Link_Interface*>();
//
//				bool pathFound = _this_ptr->template one_to_all_link_based_least_time_path_label_setting<NULLTYPE>();
//
//				// reference to store travel times
//				typedef (travel_times_itf, typename get_type_of(travel_times_to_link_container),Random_Access_Sequence, typename get_type_of(travel_times_to_link_container)::value_type);
//				travel_times_itf* travel_times = _this_ptr->travel_times_to_link_container<travel_times_itf*>();
//
//				// reference to links in tree
//				//typedef  Link_Components::Prototypes::Link<typename remove_pointer< typename _Routable_Network_Interface::get_type_of(links_container)::value_type>::type>  _Routable_Link_Interface;
//				//typedef  Random_Access_Sequence< typename _Routable_Network_Interface::get_type_of(links_container), _Routable_Link_Interface*> _Routable_Links_Container_Interface;
//
//				typename _Routable_Links_Container_Interface::iterator link_itr;
//				//cout << "origin_link = " << origin_link->template internal_id<int>() << endl;
//
//				// for each link, store its travel time
//				for(link_itr=_this_ptr->template routable_network<_Routable_Network_Interface*>()->template links_container<_Routable_Links_Container_Interface&>().begin();link_itr!=_this_ptr->template routable_network<_Routable_Network_Interface*>()->template links_container<_Routable_Links_Container_Interface&>().end();link_itr++)
//				{
//					_Routable_Link_Interface* link_ptr = (_Routable_Link_Interface*)(*link_itr);
//					travel_times->at(link_ptr->template internal_id<int>()) = link_ptr->template label_cost<float>();
//					//cout << "link " << link_ptr->template internal_id<int>() << ", path_cost=" << link_ptr->template label_cost<float>() << endl;
//				}				
//			}
//		};
	}
}

using namespace Routing_Components::Prototypes;
