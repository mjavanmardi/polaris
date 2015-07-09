#pragma once
#include "Routing_Prototype.h"
#include "Routable_Link_Implementation.h"
#include "Routable_Intersection_Implementation.h"
#ifndef EXCLUDE_DEMAND
//#include "Person_Implementations.h"
#endif
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
			m_prototype(Movement_Plan,typename MasterType::movement_plan_type,movement_plan,NONE,NONE);
			static m_prototype(Network,typename MasterType::network_type,network,NONE,NONE);
			m_data(Simulation_Timestep_Increment, departure_time, NONE, NONE);

			typedef Movement_Plan<typename type_of(movement_plan)> movement_plan_interface;
			typedef Link_Components::Prototypes::Link<typename movement_plan_interface::get_type_of(origin)> Link_Interface;

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
				else if(((typename MasterType::network_type*)_network)->_routable_networks.size() == 0)
				{
					THROW_EXCEPTION("_routable_networks is undefined.");
				}
				else if(thread_id() >= ((typename MasterType::network_type*)_network)->_routable_networks.size())
				{
					THROW_EXCEPTION("_routable_networks is not large enough.");
				}
				else if (_movement_plan == nullptr)
				{
					THROW_EXCEPTION("Movement plan is undefined.");
				}
				else if (_movement_plan->origin<Link_Interface*>() == nullptr)
				{
					THROW_EXCEPTION("Origin is undefined.");
				}

				// get a routable network; routable_network know what thread you are
				Routable_Network<typename MasterType::routable_network_type>* routable_network = _network->routable_network<typename MasterType::routable_network_type>();
				
				unsigned int origin_id = _movement_plan->origin<Link_Interface*>()->uuid<unsigned int>();
				unsigned int destination_id = _movement_plan->destination<Link_Interface*>()->uuid<unsigned int>();

				//list of edgeid, graph_id tuples; internal edge ids
				boost::container::deque<global_edge_id> path_container;
				//cost of traversing each of the edges
				boost::container::deque<float> cost_container;
				
				typedef Scenario_Components::Prototypes::Scenario< typename MasterType::scenario_type> _Scenario_Interface;

				float best_route_time_to_destination = 0.0f;

				if(!((_Scenario_Interface*)_global_scenario)->template time_dependent_routing<bool>())
				{
					best_route_time_to_destination = routable_network->compute_static_network_path(origin_id,destination_id,path_container,cost_container);
				}
				else
				{
					best_route_time_to_destination = routable_network->compute_time_dependent_network_path(origin_id,destination_id,_departure_time/*iteration()*/,path_container,cost_container);
				}


				if(path_container.size())
				{
					float routed_travel_time = cost_container.back();

					_movement_plan->template valid_trajectory<bool>(true);
					_movement_plan->template routed_travel_time<float>(routed_travel_time);
					_movement_plan->template estimated_time_of_arrival<Simulation_Timestep_Increment>(_movement_plan->template absolute_departure_time<int>() + routed_travel_time);
					_movement_plan->template estimated_travel_time_when_departed<float>(routed_travel_time);
					_movement_plan->set_trajectory(path_container, cost_container);
				}
				else
				{
					//cout << "Unable to route: " << origin_id << "," << destination_id << endl;
				}
			}

		};

		template<typename MasterType,typename InheritanceList>
		Network<typename MasterType::network_type>* Routing_Implementation<MasterType,InheritanceList>::_network;

		implementation struct Skim_Routing_Implementation: public Routing_Implementation<MasterType,INHERIT(Skim_Routing_Implementation)>
		{
			typedef typename  Routing_Implementation<MasterType,INHERIT(Skim_Routing_Implementation)>::Component_Type ComponentType;
			
			m_prototype(Network_Skimming_Components::Prototypes::Network_Skimming, typename MasterType::network_skim_type, parent_skimmer,NONE,NONE);

			m_prototype(Link,typename MasterType::link_type,origin_link,NONE,NONE);
			m_container(boost::container::vector<float>,travel_times_to_link_container, NONE, NONE);

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
					if (iteration() >= (int)_this_ptr->start_time<Simulation_Timestep_Increment>() && iteration() < (int)_this_ptr->end_time<Simulation_Timestep_Increment>())
					{
						_this->Compute_Tree();
						//response.result=true;
						//response.next._iteration=Simulation_Time.Future_Time<Simulation_Timestep_Increment,Simulation_Timestep_Increment>(_this_ptr->update_increment<Simulation_Timestep_Increment>());
						response.next._iteration=_this_ptr->update_increment<Simulation_Timestep_Increment>();
						response.next._sub_iteration=Network_Skimming_Components::Types::SUB_ITERATIONS::PATH_BUILDING;
					}
					else
					{
						//response.result=false;
						//response.next._iteration=Simulation_Time.Future_Time<Simulation_Timestep_Increment,Simulation_Timestep_Increment>(_this_ptr->update_increment<Simulation_Timestep_Increment>());
						response.next._iteration=_this_ptr->update_increment<Simulation_Timestep_Increment>();
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
				Routable_Network<typename MasterType::routable_network_type>* routable_network = _network->routable_network<typename MasterType::routable_network_type>();
				
				unsigned int origin_id = _origin_link->uuid<unsigned int>();

				_travel_times_to_link_container.clear();

				routable_network->compute_static_network_tree(origin_id,_travel_times_to_link_container);
			}
		};
	}

}
