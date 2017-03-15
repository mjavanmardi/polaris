#pragma once
#include "Routing_Prototype.h"
#include "Routable_Link_Implementation.h"
#include "Routable_Intersection_Implementation.h"
#include "Platoon_Implementation.h"
#ifndef EXCLUDE_DEMAND
//#include "Person_Implementations.h"
#endif
#include "Network_Skimming_Prototype.h"
#include <iostream>
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
			static m_prototype(Network,typename MasterType::network_type,network,NONE,NONE);
			m_data(Simulation_Timestep_Increment, departure_time, NONE, NONE);

			typedef Movement_Plan<type_of(movement_plan)> movement_plan_interface;
			typedef Link_Components::Prototypes::Link<typename movement_plan_interface::get_type_of(origin)> Link_Interface;
			typedef Activity_Location_Components::Prototypes::Activity_Location<typename movement_plan_interface::get_type_of(origin_location)> Activity_Location_Interface;
			typedef Random_Access_Sequence<typename Activity_Location_Interface::get_type_of(destination_links)> Link_Container_Interface;

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
				else if (((typename MasterType::network_type*)_network)->_routable_networks.size() == 0)
				{
					THROW_EXCEPTION("_routable_networks is undefined.");
				}
				else if (thread_id() >= ((typename MasterType::network_type*)_network)->_routable_networks.size())
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
				//unsigned int destination_id = _movement_plan->template destination<Link_Interface*>()->template uuid<unsigned int>();
				Activity_Location_Interface* origin_loc = _movement_plan->template origin<Activity_Location_Interface*>();
				Activity_Location_Interface* destination_loc = _movement_plan->template destination<Activity_Location_Interface*>();
				Link_Container_Interface* origin_links = origin_loc->template origin_links<Link_Container_Interface*>();
				Link_Container_Interface* destination_links = destination_loc->template destination_links<Link_Container_Interface*>();

				// Debug_route is false, set to true under certain conditions to print the routing output
				bool debug_route = false;

				// Fill the origin ids list from the origin location (in case there is more than one possible origin link)
				std::vector<unsigned int> origin_ids;
				for (auto itr = origin_links->begin(); itr != origin_links->end(); ++itr)
				{
					Link_Interface* link = (Link_Interface*)(*itr);
					origin_ids.push_back(link->template uuid<unsigned int>());
				}

				// Fill the destination ids list from the destination location (in case there is more than one possible destination link)
				std::vector<unsigned int> destination_ids;
				for (auto itr = destination_links->begin(); itr != destination_links->end(); ++itr)
				{
					Link_Interface* link = (Link_Interface*)(*itr);
					destination_ids.push_back(link->template uuid<unsigned int>());
				}

				//list of edgeid, graph_id tuples; internal edge ids
				std::deque<global_edge_id> path_container;
				//cost of traversing each of the edges
				std::deque<float> cost_container;

				typedef Scenario_Components::Prototypes::Scenario< typename MasterType::scenario_type> _Scenario_Interface;
				float best_route_time_to_destination = 0.0f;


				bool platoon_member = false;
				typedef Vehicle_Components::Prototypes::Vehicle<typename MasterType::vehicle_type> Vehicle_Interface;
				auto vehicle = this->_movement_plan->parent_vehicle<Vehicle_Interface*>();
				if (vehicle != nullptr)
				{
					auto trip_id = vehicle->trip_id<int>();
					auto platoon_data_container = Platoon_Components::Implementations::Platoon_Implementation<MasterType, InheritanceList>::_platoon_data_container;
					auto platoon_data_it = platoon_data_container.find(trip_id);

					if (platoon_data_it != platoon_data_container.end())
					{
						platoon_member = true;
						vehicle->platoon_data_vec (platoon_data_it->second);
						vehicle->write_trajectory(true);
						//cout << "found a trip! trip_id = " << trip_id << endl;
						best_route_time_to_destination = Compute_Platoon_Route(routable_network, origin_ids, destination_ids, path_container, cost_container, vehicle);
					}
				}

				if(!platoon_member)
				{					
					// Call path finder with current list of origin/destination possibilities - list will be trimmed to final od pair in compute_network_path
					if (!((_Scenario_Interface*)_global_scenario)->template time_dependent_routing<bool>())
					{
						best_route_time_to_destination = routable_network->compute_static_network_path(origin_ids, destination_ids, path_container, cost_container);
					}
					else
					{
						best_route_time_to_destination = routable_network->compute_time_dependent_network_path(origin_ids, destination_ids, _departure_time/*iteration()*/, path_container, cost_container, debug_route);
					}
				}

				if(path_container.size())
				{
					float routed_travel_time = cost_container.back();

					_movement_plan->template valid_trajectory<bool>(true);
					_movement_plan->template routed_travel_time<float>(routed_travel_time);
					_movement_plan->template estimated_time_of_arrival<Simulation_Timestep_Increment>(_movement_plan->template absolute_departure_time<int>() + routed_travel_time);
					_movement_plan->template estimated_travel_time_when_departed<float>(routed_travel_time);
					_movement_plan->set_trajectory(path_container, cost_container);

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
				}
			}

			float Compute_Platoon_Route(Routable_Network<typename MasterType::routable_network_type>* routable_network, std::vector<unsigned int> &origin_ids, std::vector<unsigned int> &destination_ids, std::deque<global_edge_id> &path_container, std::deque<float> &cost_container, Vehicle_Components::Prototypes::Vehicle<typename MasterType::vehicle_type> * vehicle)
			{
				typedef Link_Components::Prototypes::Link<typename MasterType::link_type>  link_itf;
				bool debug_route = false;
				float best_route_time_to_destination = 0;

				auto platoon_data =  vehicle->template platoon_data_vec < typename std::deque< Platoon_Components::Prototypes::Platoon_Data<MasterType::platoon_data_type> *>>();
				auto first_link = platoon_data[0]->link<link_itf* >();
				auto last_link = platoon_data[platoon_data.size() - 1]->link<link_itf* >();
				
				//from origin to the beginning of platoon formation
				std::deque<global_edge_id> path_container_0;	//list of edgeid, graph_id tuples; internal edge ids
				std::deque<float> cost_container_0;			//cost of traversing each of the edges
				std::vector<unsigned int> destination_ids0;
				destination_ids0.push_back(first_link->template uuid<unsigned int>());
				float best_route_time_to_destination_0 = routable_network->compute_time_dependent_network_path(origin_ids, destination_ids0, _departure_time, path_container_0, cost_container_0, debug_route);

				//from end of platooning to the destination
				std::deque<global_edge_id> path_container_1;	//list of edgeid, graph_id tuples; internal edge ids				
				std::deque<float> cost_container_1;				//cost of traversing each of the edges
				std::vector<unsigned int> origin_ids1;
				origin_ids1.push_back(last_link->template uuid<unsigned int>());
				float best_route_time_to_destination_1 = routable_network->compute_time_dependent_network_path(origin_ids1, destination_ids, _departure_time, path_container_1, cost_container_1, debug_route);

				//convert vector of links to vector of edges to create path container!				
				auto graph_id = path_container_0[0].graph_id;
				for (auto platoon_data_it : platoon_data)
				{
					auto link =  platoon_data_it->link<link_itf* >();
					// get edge id from link id
					global_edge_id edge;
					edge.edge_id = link->template uuid<unsigned int>();			
					edge.graph_id = graph_id;
					path_container.push_back(edge);
					
					//cost_container.push_back(platoon_data_it->);
					cost_container.push_back(0.0);

					best_route_time_to_destination +=  0.0 ;
				}

				//Merge 
				auto link_it_a = path_container_0.rbegin();  link_it_a++;
				for (; link_it_a != path_container_0.rend(); link_it_a++){path_container.push_front(*link_it_a);}

				auto cost_it_a = cost_container_0.rbegin(); cost_it_a++;
				for (; cost_it_a != cost_container_0.rend(); cost_it_a++){cost_container.push_front(*cost_it_a);}
				
				auto link_it_b = path_container_1.begin(); link_it_b++;
				for (; link_it_b != path_container_1.end(); ++link_it_b)	{path_container.push_back(*link_it_b);}

				auto cost_it_b = cost_container_1.begin(); cost_it_b++;
				for (; cost_it_b != cost_container_1.end(); ++cost_it_b)	{cost_container.push_back(*cost_it_b);}

				return  best_route_time_to_destination + best_route_time_to_destination_0 + best_route_time_to_destination_1;
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

				routable_network->compute_static_network_tree(origin_id,_travel_times_to_link_container);
			}
		};
	}

}
