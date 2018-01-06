#pragma once
#include "Routing_Prototype.h"
//#include "Routable_Link_Implementation.h"
//#include "Routable_Intersection_Implementation.h"

#include "Network_Skimming_Prototype.h"

namespace Routing_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{
		implementation struct Routing_Implementation:public Polaris_Component<MasterType,INHERIT(Routing_Implementation),Execution_Object>
		{
			typedef Polaris_Component<MasterType,INHERIT(Routing_Implementation),Execution_Object> Base_t;
			typedef typename Base_t::ComponentType ComponentType;
			typedef typename Base_t::Component_Type Component_Type;

			m_prototype(Movement_Plan,typename MasterType::movement_plan_type,movement_plan,NONE,NONE);
			m_static_prototype(Network,typename MasterType::network_type,network,NONE,NONE);
			m_data(Simulation_Timestep_Increment, departure_time, NONE, NONE);

			typedef Movement_Plan<type_of(movement_plan)> movement_plan_interface;
			typedef Link_Components::Prototypes::Link<typename movement_plan_interface::get_type_of(origin)> Link_Interface;
			typedef Activity_Location_Components::Prototypes::Activity_Location<typename movement_plan_interface::get_type_of(origin_location)> Activity_Location_Interface;
			typedef Random_Access_Sequence<typename Activity_Location_Interface::get_type_of(destination_links)> Link_Container_Interface;


			typedef Network<typename MasterType::network_type> Network_Interface;
			typedef Random_Access_Sequence<typename Network_Interface::get_type_of(routable_networks)> Routable_Networks_Container_Interface;
			typedef Routable_Network<typename get_component_type(Routable_Networks_Container_Interface)> Routable_Network_Interface;
			typedef Intersection<typename remove_pointer<typename Network_Interface::get_type_of(intersections_container)::value_type>::type> Intersection_Interface;

			typedef Pair_Associative_Container< typename Network_Interface::get_type_of(zones_container)> _Zones_Container_Interface;
			typedef Zone_Components::Prototypes::Zone<get_mapped_component_type(_Zones_Container_Interface)> _Zone_Interface;

			template<typename Movement_Plan_Type>
			void Attach_New_Movement_Plan(Movement_Plan<Movement_Plan_Type>* mp)
			{
				_movement_plan = mp;
			}


			void Schedule_Route_Computation(Simulation_Timestep_Increment time_to_depart, Simulation_Timestep_Increment planning_time)
			{
				// if planning time is not set, use the departure time as the time to route
				if (planning_time==0) planning_time = time_to_depart;
				_departure_time = time_to_depart;

				typedef Scenario<typename Component_Type::Master_Type::scenario_type> _Scenario_Interface;

				if (((_Scenario_Interface*)_global_scenario)->template routing_with_snapshots<bool>())
				{
					this_component()->template Load_Event<ComponentType>(&Compute_Route_Condition,planning_time,Scenario_Components::Types::Type_Sub_Iteration_keys::ROUTING_SUB_ITERATION);
				}
				else
				{
					this_component()->template Load_Event<ComponentType>(&Compute_Route_Condition,planning_time,Scenario_Components::Types::Type_Sub_Iteration_keys::ROUTING_SUB_ITERATION);
				}
			}

			static void Compute_Route_Condition(ComponentType* _this,Event_Response& response)
			{
				if(sub_iteration() == Scenario_Components::Types::Type_Sub_Iteration_keys::ROUTING_SUB_ITERATION)
				{
					response.next._iteration=END;
					response.next._sub_iteration=Scenario_Components::Types::Type_Sub_Iteration_keys::ROUTING_SUB_ITERATION;
					_this->Compute_Route();
				}
				else
				{
					assert(false);
					cout << "Should never reach here in routing conditional!" << endl;
				}
			}

			void Compute_Route()
			{
				if (_network == nullptr)
				{
					THROW_EXCEPTION("Network is undefined.");
				}
				else if(((typename MasterType::network_type*)_network)->routable_networks<Routable_Networks_Container_Interface&>().size() == 0)
				{
					THROW_EXCEPTION("_routable_networks is undefined.");
				}
				else if(thread_id() >= ((typename MasterType::network_type*)_network)->routable_networks<Routable_Networks_Container_Interface&>().size())
				{
					THROW_EXCEPTION("_routable_networks is not large enough.");
				}
				else if (_movement_plan == nullptr)
				{
					THROW_EXCEPTION("Movement plan is undefined.");
				}
				else if (_movement_plan->template origin<Link_Interface*>() == nullptr)
				{
					THROW_EXCEPTION("Origin is undefined.");
				}

				// get a routable network; routable_network know what thread you are
				Routable_Network<typename MasterType::routable_network_type>* routable_network = _network->template routable_network<typename MasterType::routable_network_type>();
				

				// Get the current origin/destination information
				unsigned int origin_id = _movement_plan->template origin<Link_Interface*>()->template uuid<unsigned int>();
				unsigned int destination_id = _movement_plan->template destination<Link_Interface*>()->template uuid<unsigned int>();
				Activity_Location_Interface* origin_loc = _movement_plan->template origin<Activity_Location_Interface*>();
				Activity_Location_Interface* destination_loc = _movement_plan->template destination<Activity_Location_Interface*>();
				unsigned int origin_loc_id = origin_loc->template uuid<unsigned int>();
				unsigned int destination_loc_id = destination_loc->template uuid<unsigned int>();
				Link_Container_Interface* origin_links = origin_loc->template origin_links<Link_Container_Interface*>();
				Link_Container_Interface* destination_links = destination_loc->template destination_links<Link_Container_Interface*>();
				Link_Container_Interface* origin_walk_links = origin_loc->template origin_walk_links<Link_Container_Interface*>();
				Link_Container_Interface* destination_walk_links = destination_loc->template destination_walk_links<Link_Container_Interface*>();
				
				//TODO: Remove when done testing routing execution time
				__int64 astar_time = -1;

				// Debug_route is false, set to true under certain conditions to print the routing output
				bool debug_route = Routing_Components::Implementations::Routable_Network_Implementation<MasterType>::debug_route<bool>();

				// Fill the origin ids list from the origin location (in case there is more than one possible origin link)
				std::vector<unsigned int> origin_ids;
				for (auto itr = origin_links->begin(); itr != origin_links->end(); ++itr)
				{
					Link_Interface* link = (Link_Interface*)(*itr);
					origin_ids.push_back(link->template uuid<unsigned int>());
				}

				// Fill the destination ids list from the destination location (in case there is more than one possible destination link)
				std::vector<unsigned int> destination_ids;
				//std::vector<unsigned int> destination_tr_ids;
				for (auto itr = destination_links->begin(); itr != destination_links->end(); ++itr)
				{
					Link_Interface* link = (Link_Interface*)(*itr);
					destination_ids.push_back(link->template uuid<unsigned int>());
				}
				
				typedef Scenario_Components::Prototypes::Scenario< typename MasterType::scenario_type> _Scenario_Interface;
				std::vector<unsigned int> origin_walk_ids;
				std::vector<unsigned int> destination_walk_ids;
				//std::vector<unsigned int> destination_tr_ids;
				if (((_Scenario_Interface*)_global_scenario)->template multimodal_routing<bool>())
				{
					
					for (auto itr = origin_walk_links->begin(); itr != origin_walk_links->end(); ++itr)
					{
						Link_Interface* link = (Link_Interface*)(*itr);
						origin_walk_ids.push_back(link->template uuid<unsigned int>());
					}

					// Fill the destination ids list from the destination location (in case there is more than one possible destination link)
					
					for (auto itr = destination_walk_links->begin(); itr != destination_walk_links->end(); ++itr)
					{
						Link_Interface* link = (Link_Interface*)(*itr);
						destination_walk_ids.push_back(link->template uuid<unsigned int>());

						/*Intersection_Interface* up_node = link->_upstream_intersection;
						Link_Container_Interface& inbound_links = up_node->template inbound_links<Link_Container_Interface&>();
						typename Link_Container_Interface::iterator in_links_itr;
						for (in_links_itr = inbound_links.begin(); in_links_itr != inbound_links.end(); in_links_itr++)
						{
							Link_Interface* inbound_link = (Link_Interface*)(*in_links_itr);
							Link_Components::Types::Link_Type_Keys in_facility_type = inbound_link->template link_type<Link_Components::Types::Link_Type_Keys>();
							if (in_facility_type == Link_Components::Types::Link_Type_Keys::TRANSIT)
							{
								destination_tr_ids.push_back(inbound_link->template uuid<unsigned int>());
							}
						}*/
					}
				}

				//list of edgeid, graph_id tuples; internal edge ids
				std::deque<global_edge_id> path_container;
				//cost of traversing each of the edges
				std::deque<float> cost_container;
				std::deque<Link_Components::Types::Link_Type_Keys> out_type;
				std::deque<int> out_trip;
				std::deque<int> out_seq;
				std::deque<float> out_time;
				std::deque<float> out_arr_time;
				std::deque<float> out_wait_time;
				std::deque<float> out_walk_time;
				std::deque<float> out_ivt_time;
				std::deque<float> out_car_time;
				std::deque<int> out_wait_count;
				std::deque<float> out_transfer_pen;
				std::deque<float> out_heur_cost;
				std::string summary_paragraph = "";
				std::string detail_paragraph = "";

				Vehicle_Components::Types::Vehicle_Type_Keys mode = _movement_plan->mode<Vehicle_Components::Types::Vehicle_Type_Keys>();

				float best_route_time_to_destination = 0.0f;
				
				// Call path finder with current list of origin/destination possibilities - list will be trimmed to final od pair in compute_network_path
				if(!((_Scenario_Interface*)_global_scenario)->template time_dependent_routing<bool>())
				{
					//TODO: Remove when done testing routing execution time
					if(((_Scenario_Interface*)_global_scenario)->template multimodal_routing<bool>() && !origin_walk_ids.empty() && !destination_walk_ids.empty() && (mode == Vehicle_Components::Types::Vehicle_Type_Keys::BUS || mode == Vehicle_Components::Types::Vehicle_Type_Keys::RAIL || mode == Vehicle_Components::Types::Vehicle_Type_Keys::WALK || mode == Vehicle_Components::Types::Vehicle_Type_Keys::BICYCLE) )
					{
						best_route_time_to_destination = routable_network->compute_multimodal_network_path(origin_walk_ids, destination_walk_ids, /*destination_tr_ids,*/ _departure_time, path_container, cost_container, out_type, out_trip, out_seq, out_time, out_arr_time, out_wait_time, out_walk_time, out_ivt_time, out_car_time, out_wait_count, out_transfer_pen, out_heur_cost, astar_time, origin_loc_id, destination_loc_id, debug_route, summary_paragraph, detail_paragraph, mode);
					}
					else if (((_Scenario_Interface*)_global_scenario)->template multimodal_routing<bool>() && !destination_walk_ids.empty() && (mode == Vehicle_Components::Types::Vehicle_Type_Keys::PARK_AND_RIDE || mode == Vehicle_Components::Types::Vehicle_Type_Keys::KISS_AND_RIDE) )
					{
						best_route_time_to_destination = routable_network->compute_multimodal_network_path(origin_ids, destination_walk_ids, /*destination_tr_ids,*/ _departure_time, path_container, cost_container, out_type, out_trip, out_seq, out_time, out_arr_time, out_wait_time, out_walk_time, out_ivt_time, out_car_time, out_wait_count, out_transfer_pen, out_heur_cost, astar_time, origin_loc_id, destination_loc_id, debug_route, summary_paragraph, detail_paragraph, mode);
					}
					else
					{
						best_route_time_to_destination = routable_network->compute_static_network_path(origin_ids, destination_ids, _departure_time, path_container, cost_container, origin_loc_id, destination_loc_id, debug_route, summary_paragraph);
					}
					
				}
				else
				{
					//TODO: Remove when done testing routing execution time
					if (((_Scenario_Interface*)_global_scenario)->template multimodal_routing<bool>() && !origin_walk_ids.empty() && !destination_walk_ids.empty() && (mode == Vehicle_Components::Types::Vehicle_Type_Keys::BUS || mode == Vehicle_Components::Types::Vehicle_Type_Keys::RAIL || mode == Vehicle_Components::Types::Vehicle_Type_Keys::WALK || mode == Vehicle_Components::Types::Vehicle_Type_Keys::BICYCLE) )
					{
						best_route_time_to_destination = routable_network->compute_multimodal_network_path(origin_walk_ids, destination_walk_ids, /*destination_tr_ids,*/ _departure_time, path_container, cost_container, out_type, out_trip, out_seq, out_time, out_arr_time, out_wait_time, out_walk_time, out_ivt_time, out_car_time, out_wait_count, out_transfer_pen, out_heur_cost, astar_time, origin_loc_id, destination_loc_id, debug_route, summary_paragraph, detail_paragraph, mode);
					}
					else if (((_Scenario_Interface*)_global_scenario)->template multimodal_routing<bool>() && !destination_walk_ids.empty() && (mode == Vehicle_Components::Types::Vehicle_Type_Keys::PARK_AND_RIDE || mode == Vehicle_Components::Types::Vehicle_Type_Keys::KISS_AND_RIDE) )
					{
						best_route_time_to_destination = routable_network->compute_multimodal_network_path(origin_ids, destination_walk_ids, /*destination_tr_ids,*/ _departure_time, path_container, cost_container, out_type, out_trip, out_seq, out_time, out_arr_time, out_wait_time, out_walk_time, out_ivt_time, out_car_time, out_wait_count, out_transfer_pen, out_heur_cost, astar_time, origin_loc_id, destination_loc_id, debug_route, summary_paragraph, detail_paragraph, mode);
					}
					else
					{ 
						best_route_time_to_destination = routable_network->compute_time_dependent_network_path(origin_ids,destination_ids,_departure_time/*iteration()*/,path_container,cost_container, origin_loc_id, destination_loc_id, debug_route, summary_paragraph);
					}

				}


				if(path_container.size())
				{
					float routed_travel_time = best_route_time_to_destination; //cost_container.back();

					_movement_plan->template valid_trajectory<bool>(true);
					_movement_plan->template routed_travel_time<float>(routed_travel_time);
					_movement_plan->template estimated_time_of_arrival<Simulation_Timestep_Increment>(_movement_plan->template absolute_departure_time<int>() + routed_travel_time);
					_movement_plan->template estimated_travel_time_when_departed<float>(routed_travel_time);
					_movement_plan->set_trajectory(path_container, cost_container);
					_movement_plan->routing_execution_time(astar_time);
					_movement_plan->summary_string(summary_paragraph);
					_movement_plan->detail_string(detail_paragraph);
					
					// update movement plan O/D based on returned routing results
					Link_Interface* olink = nullptr;
					for (auto itr = origin_links->begin(); itr != origin_links->end(); ++itr)
					{
						Link_Interface* link = (Link_Interface*)(*itr);
						if (link->template uuid<unsigned int>()  == origin_ids.front()) olink=link;
					}
					Link_Interface* dlink = nullptr;
					for (auto itr = destination_links->begin(); itr != destination_links->end(); ++itr)
					{
						Link_Interface* link = (Link_Interface*)(*itr);
						if (link->template uuid<unsigned int>()  == destination_ids.front()) dlink=link;
					}

					if (olink != nullptr && dlink != nullptr)
					{
						_movement_plan->template origin<Link_Interface*>(olink);
						_movement_plan->template destination<Link_Interface*>(dlink);
					}
				}
				else
				{
					//cout << "Unable to route: " << origin_id << "," << destination_id << endl;
					//if (astar_time >= 0)
					//{
						_movement_plan->routing_execution_time(astar_time);
						_movement_plan->summary_string(summary_paragraph);
					//}
				}

				path_container.clear();
				//cost of traversing each of the edges
				cost_container.clear();
				out_type.clear();
				out_trip.clear();
				out_seq.clear();
				out_time.clear();
				out_arr_time.clear();
				out_wait_time.clear();
				out_walk_time.clear();
				out_ivt_time.clear();
				out_car_time.clear();
				out_wait_count.clear();
				out_transfer_pen.clear();
				out_heur_cost.clear();
				summary_paragraph = "";
				detail_paragraph = "";
			}

		};

		template<typename MasterType,typename InheritanceList>
		Network<typename MasterType::network_type>* Routing_Implementation<MasterType,InheritanceList>::_network;

		implementation struct Skim_Routing_Implementation: public Routing_Implementation<MasterType,INHERIT(Skim_Routing_Implementation)>
		{
			using Routing_Implementation<MasterType,INHERIT(Skim_Routing_Implementation)>::_network;
			typedef typename  Routing_Implementation<MasterType,INHERIT(Skim_Routing_Implementation)>::Component_Type ComponentType;
			
			m_prototype(Network_Skimming_Components::Prototypes::Network_Skimming, typename MasterType::network_skim_type, parent_skimmer,NONE,NONE);

			m_prototype(Link,typename MasterType::link_type,origin_link,NONE,NONE);
			m_container(std::vector<float>,travel_times_to_link_container, NONE, NONE);

			//member_component_and_feature_accessor(update_increment,Value,Basic_Units::Prototypes::Time,Basic_Units::Implementations::Time_Implementation<MasterType>);
			
			member_component_and_feature_accessor(start_time,Value,Basic_Units::Prototypes::Time,Basic_Units::Implementations::Time_Implementation<MasterType>);
			member_component_and_feature_accessor(end_time,Value,Basic_Units::Prototypes::Time,Basic_Units::Implementations::Time_Implementation<MasterType>);

			tag_getter_as_available(update_increment);
			template <typename TargetType> TargetType update_increment()
			{
				return this->_parent_skimmer->template update_increment<TargetType>();
			}

			//============================================================================================
			/// Events and event handling
			void Schedule_Route_Computation(Simulation_Timestep_Increment time_to_depart, Simulation_Timestep_Increment planning_time)
			{
				this_component()->template Load_Event<ComponentType>(&Compute_Route_Event_Controller,time_to_depart,Network_Skimming_Components::Types::SUB_ITERATIONS::PATH_BUILDING);
			}
			
			static void Compute_Route_Event_Controller(ComponentType* _this,Event_Response& response)
			{
				typedef Routing_Components::Prototypes::Routing<ComponentType> _Routing_Interface;
				
				_Routing_Interface* _this_ptr=(_Routing_Interface*)_this;
				if(sub_iteration() == Network_Skimming_Components::Types::SUB_ITERATIONS::PATH_BUILDING)
				{
					if (iteration() >= (int)_this_ptr->template start_time<Simulation_Timestep_Increment>() && iteration() < (int)_this_ptr->template end_time<Simulation_Timestep_Increment>())
					{
						_this->Compute_Tree();
						//response.result=true;
						//response.next._iteration=Simulation_Time.Future_Time<Simulation_Timestep_Increment,Simulation_Timestep_Increment>(_this_ptr->update_increment<Simulation_Timestep_Increment>());
						response.next._iteration=_this_ptr->template update_increment<Simulation_Timestep_Increment>();
						response.next._sub_iteration=Network_Skimming_Components::Types::SUB_ITERATIONS::PATH_BUILDING;
					}
					else
					{
						//response.result=false;
						//response.next._iteration=Simulation_Time.Future_Time<Simulation_Timestep_Increment,Simulation_Timestep_Increment>(_this_ptr->update_increment<Simulation_Timestep_Increment>());
						response.next._iteration=_this_ptr->template update_increment<Simulation_Timestep_Increment>();
						response.next._sub_iteration=Network_Skimming_Components::Types::SUB_ITERATIONS::PATH_BUILDING;
					}
				}
				else
				{
					assert(false);
					cout << "Should never reach here in routing conditional!" << endl;
				}
			}


			void Compute_Tree()
			{
				// get a routable network
				Routable_Network<typename MasterType::routable_network_type>* routable_network = _network->template routable_network<typename MasterType::routable_network_type>();

				unsigned int origin_id = _origin_link->template uuid<unsigned int>();

				_travel_times_to_link_container.clear();

				//routable_network->compute_static_network_tree(origin_id,_travel_times_to_link_container);

				typedef Scenario_Components::Prototypes::Scenario< typename MasterType::scenario_type> _Scenario_Interface;
				if (!((_Scenario_Interface*)_global_scenario)->template time_dependent_routing<bool>())
				{
					routable_network->compute_static_network_tree(origin_id, _travel_times_to_link_container);
				}
				else
				{
					routable_network->compute_time_dependent_network_tree(origin_id, _travel_times_to_link_container);
				}
			}
		};


		implementation struct Dijkstra_for_Heuristics_Implementation : public Routing_Implementation<MasterType, INHERIT(Dijkstra_for_Heuristics_Implementation)>
		{
			using Routing_Implementation<MasterType, INHERIT(Dijkstra_for_Heuristics_Implementation)>::_network;
			typedef typename  Routing_Implementation<MasterType, INHERIT(Dijkstra_for_Heuristics_Implementation)>::Component_Type ComponentType;
			
			m_prototype(Zone, typename MasterType::zone_type, origin_zone, NONE, NONE);
			
			// Events and event handling
			void Schedule_Route_Computation(Simulation_Timestep_Increment start, Simulation_Timestep_Increment planning_time)
			{
				this_component()->template Load_Event<ComponentType>(&Compute_Route_Event_Controller, start + 1, 0);
			}

			static void Compute_Route_Event_Controller(ComponentType* _this, Event_Response& response)
			{
				if (sub_iteration() == 0)
				{
					response.next._iteration = END;
					response.next._sub_iteration = 0;
					_this->Compute_MM_Dijkstra_Tree();
				}
				else
				{
					assert(false);
					cout << "Should never reach here in routing conditional!" << endl;
				}
			}


			void Compute_MM_Dijkstra_Tree()
			{
				if (_network == nullptr)
				{
					THROW_EXCEPTION("Network is undefined.");
				}
				else if (((typename MasterType::network_type*)_network)->routable_networks<Routable_Networks_Container_Interface&>().size() == 0)
				{
					THROW_EXCEPTION("_routable_networks is undefined.");
				}
				else if (thread_id() >= ((typename MasterType::network_type*)_network)->routable_networks<Routable_Networks_Container_Interface&>().size())
				{
					THROW_EXCEPTION("_routable_networks is not large enough.");
				}	
				else if (_origin_zone == nullptr)
				{
					THROW_EXCEPTION("Origin zone is undefined.");
				}

				// Debug_route is false, set to true under certain conditions to print the routing output
				bool debug_route = Routing_Components::Implementations::Routable_Network_Implementation<MasterType>::debug_route<bool>();

				std::string summary_paragraph = "";

				// get a routable network; routable_network know what thread you are
				Routable_Network<typename MasterType::routable_network_type>* routable_network = _network->template routable_network<typename MasterType::routable_network_type>();
								
								
				//_Zone_Interface* origin_zone = origin_zone;
				int origin_zone_index = _origin_zone->internal_id<int>();
				Link_Container_Interface* origin_links = _origin_zone->origin_links<Link_Container_Interface*>();

				// Fill the origin ids list from the origin location (in case there is more than one possible origin link)
				std::vector<unsigned int> origin_ids;
				for (auto itr = origin_links->begin(); itr != origin_links->end(); ++itr)
				{
					Link_Interface* origin_link = (Link_Interface*)(*itr);
					origin_ids.push_back(origin_link->template uuid<unsigned int>());
				}				

				routable_network->compute_dijkstra_network_tree(origin_ids, origin_zone_index, debug_route, summary_paragraph);

			}
		};
	}

}
