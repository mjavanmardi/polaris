#pragma once
#include "User_Space.h"
#include "../File_IO/utilities.h"

#ifndef WINDOWS
// for hash_map
using namespace __gnu_cxx;
#endif

/*------------------------------------------------*/

namespace Routing_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Prototypes
	{

		prototype struct Routing_Prototype
		{
			tag_as_prototype;

			feature_accessor(network, none, none);
			feature_accessor(traveler, none, none);
			feature_accessor(vehicle, none, none);
			feature_accessor(routable_network, none, none);
			feature_accessor(routable_origin, none, none);
			feature_accessor(routable_destination, none, none);

			declare_feature_conditional(Compute_Route_Condition)
			{
				typedef Routing_Components::Prototypes::Routing_Prototype<ComponentType, ComponentType> _Routing_Interface;
				define_component_interface(_Traveler_Interface, get_type_of(traveler), Traveler_Components::Prototypes::Traveler_Prototype, ComponentType);
				_Routing_Interface* _this_ptr=(_Routing_Interface*)_this;
				if(_sub_iteration == Scenario_Components::Types::Type_Sub_Iteration_keys::ROUTING_SUB_ITERATION)
				{
					response.result=true;
					response.next._iteration=END;
					response.next._sub_iteration=Scenario_Components::Types::Type_Sub_Iteration_keys::ROUTING_SUB_ITERATION;
				}
				else
				{
					assert(false);
					cout << "Should never reach here in routing conditional!" << endl;
				}
			}

			feature_prototype bool one_to_one_link_based_least_time_path_a_star()
			{

				define_component_interface(_Routable_Network_Interface, get_type_of(routable_network), Network_Components::Prototypes::Network_Prototype, ComponentType);
				define_component_interface(_Regular_Network_Interface, get_type_of(network), Network_Components::Prototypes::Network_Prototype, ComponentType);
				define_container_and_value_interface(_Routable_Links_Container_Interface, _Routable_Link_Interface, _Routable_Network_Interface::get_type_of(links_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
				//typedef Link_Interface<typename ComponentType::routable_link_type, ComponentType> _Routable_Link_Interface;
				define_component_interface(_Routable_Intersection_Interface, ComponentType::routable_link_type::upstream_intersection_type, Intersection_Components::Prototypes::Intersection_Prototype, ComponentType);
				define_container_and_value_interface(_Inbound_Outbound_Movements_Container_Interface, _Inbound_Outbound_Movements_Interface, _Routable_Intersection_Interface::get_type_of(inbound_outbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Inbound_Outbound_Movements_Prototype, ComponentType);
				define_container_and_value_interface(_Movements_Container_Interface, _Movement_Interface, _Inbound_Outbound_Movements_Interface::get_type_of(outbound_movements), Random_Access_Sequence_Prototype, Turn_Movement_Components::Prototypes::Movement_Prototype, ComponentType);
				define_container_and_value_interface(_Reversed_Path_Container_Interface, _Regular_Link_Interface, _Routable_Network_Interface::get_type_of(reversed_path_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
				typedef typename _Routable_Network_Interface::get_type_of(scan_list) ScanListType;

				_Routable_Network_Interface* routable_net=routable_network<_Routable_Network_Interface*>();
				routable_net->template reset_routable_network<NULLTYPE>();
				
				//RoutableNetworkInterface* routable_network=routable_network<RoutableNetworkInterface*>();
				float max_free_flow_speed=routable_net->template max_free_flow_speed<float>();
				ScanListType& scan_list=routable_net->template scan_list<ScanListType&>();
				//NumSearchesType& num_searches=routable_network->template num_searches<NumSearchesType&>();

				_Routable_Link_Interface* origin_link_ptr=routable_origin<_Routable_Link_Interface*>();
				_Routable_Link_Interface* destination_link_ptr=routable_destination<_Routable_Link_Interface*>();
				_Regular_Link_Interface* destination_reference=destination_link_ptr->template network_link_reference<_Regular_Link_Interface*>();
				
				_Regular_Link_Interface* net_origin_link=origin_link_ptr->template network_link_reference<_Regular_Link_Interface*>();
				_Movements_Container_Interface& turn_mvmt_container=net_origin_link->template outbound_turn_movements<_Movements_Container_Interface&>();
				int outbound_turn_movement_size = (int)turn_mvmt_container.size();
				if (origin_link_ptr != destination_link_ptr && outbound_turn_movement_size == 0)
				{
					cerr << "Origin link must have outbount turn movement" << endl;
					assert(false);
				}

				float next_cost,new_cost;
				float dx,dy;
				float destination_x,destination_y;

				_Routable_Intersection_Interface* destination_upstream_intersection=destination_link_ptr->template upstream_intersection<_Routable_Intersection_Interface*>();

				destination_x = destination_upstream_intersection->template x_position<float>();
				destination_y = destination_upstream_intersection->template y_position<float>();
				
				next_cost=origin_link_ptr->template network_link_reference<_Regular_Link_Interface*>()->template travel_time<float>();
				new_cost = next_cost;
				_Routable_Link_Interface* current_link=origin_link_ptr;
				
				current_link->template label_cost<float>(new_cost); // g - label_cost

				_Routable_Intersection_Interface* current_intersection;

				if(current_link->template network_link_reference<_Regular_Link_Interface*>() != destination_reference)
				{
					current_intersection=current_link->template upstream_intersection<_Routable_Intersection_Interface*>();
					dx = destination_x - current_intersection->template x_position<float>();
					dy = destination_y - current_intersection->template y_position<float>();
					current_link->template h_cost<float>(sqrt(dx*dx+dy*dy)/max_free_flow_speed); // h - estimated = sqrt(dx*dx+dy*dy)/max_free_flow_speed 
					current_link->template f_cost<float>(current_link->template label_cost<float>()+current_link->template h_cost<float>()); // f = g + h;
				}
				else
				{
					return true;
				}

				current_link->template label_pointer<_Routable_Link_Interface*>(current_link);

				scan_list.insert(make_pair(current_link->template f_cost<float>(),current_link));
				current_link->template scan_list_status<Network_Components::Types::Scan_List_Status_Keys>(Network_Components::Types::INSELIST);
				int cur_link_id;
				while(!scan_list.empty())
				{ 

					current_link = (_Routable_Link_Interface*)(scan_list.begin()->second);
					cur_link_id=current_link->template network_link_reference<_Regular_Link_Interface*>()->template uuid<int>();
					if(current_link == destination_link_ptr)
					{
						break;
					}

					scan_list.erase(scan_list.begin());	// no need to find minimum since set is ordered by its first element, i.e. label cost.
					current_link->template scan_list_status<Network_Components::Types::Scan_List_Status_Keys>(Network_Components::Types::SCANNED);

					current_intersection=current_link->template downstream_intersection<_Routable_Intersection_Interface*>();

					
					//for all outbound turn movements
					_Inbound_Outbound_Movements_Container_Interface& inbound_outbound_movements_container = current_intersection->template inbound_outbound_movements<_Inbound_Outbound_Movements_Container_Interface&>();
					typename _Inbound_Outbound_Movements_Container_Interface::iterator inbound_itr;
					for(inbound_itr=inbound_outbound_movements_container.begin(); inbound_itr!=inbound_outbound_movements_container.end(); inbound_itr++)
					{
						_Inbound_Outbound_Movements_Interface* inbound_outbound_movements = (_Inbound_Outbound_Movements_Interface*)(*inbound_itr);
						_Routable_Link_Interface* inbound_link = inbound_outbound_movements->template inbound_link_reference<_Routable_Link_Interface*>();
						int inbound_link_id=inbound_link->template network_link_reference<_Regular_Link_Interface*>()->template internal_id<int>();

						if (inbound_link == current_link)
						{
							_Movements_Container_Interface& outbound_movements_container = inbound_outbound_movements->template outbound_movements<_Movements_Container_Interface&>();
							typename _Movements_Container_Interface::iterator outbound_itr;

							for(outbound_itr=outbound_movements_container.begin(); outbound_itr!=outbound_movements_container.end(); outbound_itr++)
							{
								_Movement_Interface* outbound_movement = (_Movement_Interface*)(*outbound_itr);
								_Routable_Link_Interface* next_link=outbound_movement->template outbound_link<_Routable_Link_Interface*>();
								int next_link_id=next_link->template network_link_reference<_Regular_Link_Interface*>()->template internal_id<int>();


								next_cost=outbound_movement->template forward_link_turn_travel_time<float>();
								new_cost=current_link->template label_cost<float>() + next_cost;

								if(next_link->template label_cost<float>()>new_cost)
								{
									if(next_link->template scan_list_status<Network_Components::Types::Scan_List_Status_Keys>()==Network_Components::Types::INSELIST)
									{
										scan_list.erase(make_pair(next_link->template f_cost<float>(),next_link)); // delete the old cost
									}

									next_link->template label_cost<float>(new_cost);
									next_link->template label_pointer<_Routable_Link_Interface*>(current_link);

									if(next_link->template network_link_reference<_Regular_Link_Interface*>()!=destination_reference)
									{
										dx=destination_x - next_link->template upstream_intersection<_Routable_Intersection_Interface*>()->template x_position<float>();
										dy=destination_y - next_link->template upstream_intersection<_Routable_Intersection_Interface*>()->template y_position<float>();
										next_link->template h_cost<float>(sqrt(dx*dx+dy*dy)/max_free_flow_speed);
									}
									else
									{
										next_link->template h_cost<float>(0.0);
									}

									next_link->template f_cost<float>(next_link->template label_cost<float>() + next_link->template h_cost<float>());

									scan_list.insert(make_pair(next_link->template f_cost<float>(),next_link)); // update with the new cos

									next_link->template scan_list_status<Network_Components::Types::Scan_List_Status_Keys>(Network_Components::Types::INSELIST);
								}
							}
							break;
						}
					}
				}

				if (destination_link_ptr->template label_pointer<_Routable_Link_Interface*>() == destination_link_ptr)
				{
					return false;
				}
				else
				{

					_Reversed_Path_Container_Interface& reversed_path_container=routable_network<_Routable_Network_Interface*>()->template reversed_path_container<_Reversed_Path_Container_Interface&>();

					current_link=routable_destination<_Routable_Link_Interface*>();
				
					while (true)
					{
						reversed_path_container.push_back(current_link->template network_link_reference<_Regular_Link_Interface*>());

						if (current_link->label_pointer<_Routable_Link_Interface*>() != current_link)
						{
							current_link=current_link->template label_pointer<_Routable_Link_Interface*>();
						}
						else
						{
							break;
						}
						
					}

					return true;
				}

			};

			feature_prototype bool one_to_all_link_based_least_time_path_label_setting()
			{

				define_component_interface(_Routable_Network_Interface, get_type_of(routable_network), Network_Components::Prototypes::Network_Prototype, ComponentType);
				define_component_interface(_Regular_Network_Interface, get_type_of(network), Network_Components::Prototypes::Network_Prototype, ComponentType);
				define_container_and_value_interface(_Routable_Links_Container_Interface, _Routable_Link_Interface, _Routable_Network_Interface::get_type_of(links_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
				//typedef Link_Interface<typename ComponentType::routable_link_type, ComponentType> _Routable_Link_Interface;
				define_component_interface(_Routable_Intersection_Interface, ComponentType::routable_link_type::upstream_intersection_type, Intersection_Components::Prototypes::Intersection_Prototype, ComponentType);
				define_container_and_value_interface(_Inbound_Outbound_Movements_Container_Interface, _Inbound_Outbound_Movements_Interface, _Routable_Intersection_Interface::get_type_of(inbound_outbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Inbound_Outbound_Movements_Prototype, ComponentType);
				define_container_and_value_interface(_Movements_Container_Interface, _Movement_Interface, _Inbound_Outbound_Movements_Interface::get_type_of(outbound_movements), Random_Access_Sequence_Prototype, Turn_Movement_Components::Prototypes::Movement_Prototype, ComponentType);
				define_container_and_value_interface(_Reversed_Path_Container_Interface, _Regular_Link_Interface, _Routable_Network_Interface::get_type_of(reversed_path_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
				typedef typename _Routable_Network_Interface::get_type_of(scan_list) ScanListType;

				_Routable_Network_Interface* routable_net=routable_network<_Routable_Network_Interface*>();
				routable_net->template reset_routable_network<NULLTYPE>();
				
				//RoutableNetworkInterface* routable_network=routable_network<RoutableNetworkInterface*>();
				float max_free_flow_speed=routable_net->template max_free_flow_speed<float>();
				ScanListType& scan_list=routable_net->template scan_list<ScanListType&>();
				//NumSearchesType& num_searches=routable_network->template num_searches<NumSearchesType&>();

				_Routable_Link_Interface* origin_link_ptr=routable_origin<_Routable_Link_Interface*>();
				
				_Regular_Link_Interface* net_origin_link=origin_link_ptr->template network_link_reference<_Regular_Link_Interface*>();
				_Movements_Container_Interface& turn_mvmt_container=net_origin_link->template outbound_turn_movements<_Movements_Container_Interface&>();
				int outbound_turn_movement_size = (int)turn_mvmt_container.size();
				
				if (outbound_turn_movement_size == 0)
				{
					cerr << "Origin link must have outbount turn movement" << endl;
					assert(false);
				}

				float next_cost,new_cost;


				next_cost=origin_link_ptr->template network_link_reference<_Regular_Link_Interface*>()->template travel_time<float>();
				new_cost = next_cost;
				_Routable_Link_Interface* current_link=origin_link_ptr;
				
				current_link->template label_cost<float>(new_cost); // g - label_cost

				_Routable_Intersection_Interface* current_intersection;

				current_link->template label_pointer<_Routable_Link_Interface*>(current_link);

				scan_list.insert(make_pair(current_link->template label_cost<float>(),current_link));
				current_link->template scan_list_status<Network_Components::Types::Scan_List_Status_Keys>(Network_Components::Types::INSELIST);
				int cur_link_id;
				while(!scan_list.empty())
				{ 

					current_link = (_Routable_Link_Interface*)(scan_list.begin()->second);
					cur_link_id=current_link->template network_link_reference<_Regular_Link_Interface*>()->template uuid<int>();

					scan_list.erase(scan_list.begin());	// no need to find minimum since set is ordered by its first element, i.e. label cost.
					current_link->template scan_list_status<Network_Components::Types::Scan_List_Status_Keys>(Network_Components::Types::SCANNED);

					current_intersection=current_link->template downstream_intersection<_Routable_Intersection_Interface*>();

					
					//for all outbound turn movements
					_Inbound_Outbound_Movements_Container_Interface& inbound_outbound_movements_container = current_intersection->template inbound_outbound_movements<_Inbound_Outbound_Movements_Container_Interface&>();
					typename _Inbound_Outbound_Movements_Container_Interface::iterator inbound_itr;
					for(inbound_itr=inbound_outbound_movements_container.begin(); inbound_itr!=inbound_outbound_movements_container.end(); inbound_itr++)
					{
						_Inbound_Outbound_Movements_Interface* inbound_outbound_movements = (_Inbound_Outbound_Movements_Interface*)(*inbound_itr);
						_Routable_Link_Interface* inbound_link = inbound_outbound_movements->template inbound_link_reference<_Routable_Link_Interface*>();
						int inbound_link_id=inbound_link->template network_link_reference<_Regular_Link_Interface*>()->template internal_id<int>();

						if (inbound_link == current_link)
						{
							_Movements_Container_Interface& outbound_movements_container = inbound_outbound_movements->template outbound_movements<_Movements_Container_Interface&>();
							typename _Movements_Container_Interface::iterator outbound_itr;

							for(outbound_itr=outbound_movements_container.begin(); outbound_itr!=outbound_movements_container.end(); outbound_itr++)
							{
								_Movement_Interface* outbound_movement = (_Movement_Interface*)(*outbound_itr);
								_Routable_Link_Interface* next_link=outbound_movement->template outbound_link<_Routable_Link_Interface*>();
								int next_link_id=next_link->template network_link_reference<_Regular_Link_Interface*>()->template internal_id<int>();


								next_cost=outbound_movement->template forward_link_turn_travel_time<float>();
								new_cost=current_link->template label_cost<float>() + next_cost;

								if(next_link->template label_cost<float>()>new_cost)
								{
									if(next_link->template scan_list_status<Network_Components::Types::Scan_List_Status_Keys>()==Network_Components::Types::INSELIST)
									{
										scan_list.erase(make_pair(next_link->template label_cost<float>(),next_link)); // delete the old cost
									}

									next_link->template label_cost<float>(new_cost);
									next_link->template label_pointer<_Routable_Link_Interface*>(current_link);

									scan_list.insert(make_pair(next_link->template label_cost<float>(),next_link)); // update with the new cos

									next_link->template scan_list_status<Network_Components::Types::Scan_List_Status_Keys>(Network_Components::Types::INSELIST);
								}
							}
							break;
						}
					}
				}
				return true;
			};

			feature_prototype void Schedule_Route_Computation(int departed_time)
			{
				define_component_interface(_Regular_Network_Interface, get_type_of(network), Network_Components::Prototypes::Network_Prototype, ComponentType);
				define_component_interface(_Scenario_Interface, _Regular_Network_Interface::get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
				load_event(ComponentType,Compute_Route_Condition,Compute_Route,departed_time,Scenario_Components::Types::Type_Sub_Iteration_keys::ROUTING_SUB_ITERATION,NULLTYPE);
			}

			//first event
			declare_feature_event(Compute_Route)
			{
				typedef Routing_Components::Prototypes::Routing_Prototype<ComponentType, ComponentType> _Routing_Interface;
				typedef Link_Components::Prototypes::Link_Prototype<typename ComponentType::routable_link_type, ComponentType> _Routable_Link_Interface;
				_Routing_Interface* _this_ptr=(_Routing_Interface*)_this;
				typedef Network_Components::Prototypes::Network_Prototype<typename ComponentType::routable_network_type, ComponentType> _Routable_Network_Interface;
				define_component_interface(_Regular_Network_Interface, get_type_of(network), Network_Components::Prototypes::Network_Prototype, ComponentType);
				define_component_interface(_Scenario_Interface, _Regular_Network_Interface::get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
				define_component_interface(_Traveler_Interface, get_type_of(traveler), Traveler_Components::Prototypes::Traveler_Prototype, ComponentType);
				define_component_interface(_Vehicle_Interface, _Traveler_Interface::get_type_of(vehicle), Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);
				define_component_interface(_Movement_Plan_Interface, _Vehicle_Interface::get_type_of(movement_plan), Movement_Plan_Components::Prototypes::Movement_Plan_Prototype, ComponentType);

				define_container_and_value_interface(_Reversed_Path_Container_Interface, _Regular_Link_Interface, ComponentType::routable_network_type::type_of(reversed_path_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);

				_Vehicle_Interface* veh = _this_ptr->template vehicle<_Vehicle_Interface*>();
				_Movement_Plan_Interface* mp= veh->template movement_plan<_Movement_Plan_Interface*>();
				_Regular_Link_Interface* origin_link=mp->template origin<_Regular_Link_Interface*>();
				_Regular_Link_Interface* destination_link=mp->template destination<_Regular_Link_Interface*>();
				
				_this_ptr->template routable_origin<_Regular_Link_Interface*>(origin_link);
				_this_ptr->template routable_destination<_Regular_Link_Interface*>(destination_link);

				bool pathFound = _this_ptr->template one_to_one_link_based_least_time_path_a_star<NULLTYPE>();
				if (pathFound)
				{
					_Routable_Network_Interface* routable_network_ptr=_this_ptr->template routable_network<_Routable_Network_Interface*>();
					mp->set_trajectory<_Reversed_Path_Container_Interface>(routable_network_ptr->template reversed_path_container<_Reversed_Path_Container_Interface&>());
					origin_link->push_vehicle(veh);
				}
//*** Below is an example to calculate one-to-all shortest path tree and to access the tree. The unit of cost is second.
//*** How to calculate one-to-all shortest path tree
//				_this_ptr->template one_to_all_link_based_least_time_path_label_setting<NULLTYPE>();

//*** How to access the cost tree				
//				define_container_and_value_interface(_Routable_Links_Container_Interface, _Routable_Link_Interface, _Routable_Network_Interface::get_type_of(links_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
//
//				typename _Routable_Links_Container_Interface::iterator link_itr;
//cout << "origin_link = " << origin_link->internal_id<int>() << endl;
//				for(link_itr=_this_ptr->routable_network<_Routable_Network_Interface*>()->links_container<_Routable_Links_Container_Interface&>().begin();link_itr!=_this_ptr->routable_network<_Routable_Network_Interface*>()->links_container<_Routable_Links_Container_Interface&>().end();link_itr++)
//				{
//					_Routable_Link_Interface* link_ptr = (_Routable_Link_Interface*)(*link_itr);
//					cout << "link " << link_ptr->internal_id<int>() << ", path_cost=" << link_ptr->label_cost<float>() << endl;
//				}
			}
		};
	}
}

using namespace Routing_Components::Prototypes;