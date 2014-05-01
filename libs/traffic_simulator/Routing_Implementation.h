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

				typedef Scenario<typename Component_Type::Master_Type::scenario_type> _Scenario_Interface;

				if (((_Scenario_Interface*)_global_scenario)->template routing_with_snapshots<bool>())
				{
					this_component()->Load_Event<ComponentType>(&Compute_Route_Condition,planning_time,Scenario_Components::Types::Type_Sub_Iteration_keys::ROUTING_SUB_ITERATION);
				}
				else
				{
					this_component()->Load_Event<ComponentType>(&Compute_Route_Condition,planning_time,Scenario_Components::Types::Type_Sub_Iteration_keys::ROUTING_SUB_ITERATION);
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
				// get a routable network
				Routable_Network<typename MasterType::routable_network_type>* routable_network = _network->routable_network<typename MasterType::routable_network_type>();
				
				unsigned int origin_id = _movement_plan->origin<Link_Interface*>()->uuid<unsigned int>();
				unsigned int destination_id = _movement_plan->destination<Link_Interface*>()->uuid<unsigned int>();

				boost::container::deque<global_edge_id> path_container;
				boost::container::deque<float> cost_container;
				
				typedef Scenario_Components::Prototypes::Scenario< typename MasterType::scenario_type> _Scenario_Interface;

				float best_route_time_to_destination = 0.0f;

				if(!((_Scenario_Interface*)_global_scenario)->time_dependent_routing<bool>())
				{
					best_route_time_to_destination = routable_network->compute_static_network_path(origin_id,destination_id,path_container,cost_container);
				}
				else
				{
					best_route_time_to_destination = routable_network->compute_time_dependent_network_path(origin_id,destination_id,iteration(),path_container,cost_container);
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
			
			m_prototype(Link,typename MasterType::link_type,origin_link,NONE,NONE);
			m_container(boost::container::vector<float>,travel_times_to_link_container, NONE, NONE);
			
			member_component_and_feature_accessor(update_increment,Value,Basic_Units::Prototypes::Time,Basic_Units::Implementations::Time_Implementation<MasterType>);
			member_component_and_feature_accessor(start_time,Value,Basic_Units::Prototypes::Time,Basic_Units::Implementations::Time_Implementation<MasterType>);
			member_component_and_feature_accessor(end_time,Value,Basic_Units::Prototypes::Time,Basic_Units::Implementations::Time_Implementation<MasterType>);


			//============================================================================================
			/// Events and event handling
			void Schedule_Route_Computation(Simulation_Timestep_Increment time_to_depart, Simulation_Timestep_Increment planning_time)
			{
				this_component()->Load_Event<ComponentType>(&Compute_Route_Event_Controller,time_to_depart,Network_Skimming_Components::Types::SUB_ITERATIONS::PATH_BUILDING);
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
						response.next._iteration=Simulation_Time.Future_Time<Simulation_Timestep_Increment,Simulation_Timestep_Increment>(_this_ptr->update_increment<Simulation_Timestep_Increment>());
						response.next._sub_iteration=Network_Skimming_Components::Types::SUB_ITERATIONS::PATH_BUILDING;
					}
					else
					{
						//response.result=false;
						response.next._iteration=Simulation_Time.Future_Time<Simulation_Timestep_Increment,Simulation_Timestep_Increment>(_this_ptr->update_increment<Simulation_Timestep_Increment>());
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

//		implementation struct Routing_Implementation:public Polaris_Component<MasterType,INHERIT(Routing_Implementation),Execution_Object>
//		{
//			//==================================================================================================
//			// TYPEDEFS AND INTERFACES
//			//--------------------------------------------------------------------------------------------------
//			typedef boost::container::vector<typename MasterType::routable_link_type*> routable_links_container_type;
//			//typedef Container<routable_links_container_type> _Links_Container_Type;
//			//typedef Random_Access_Sequence<_Links_Container_Type, NULLTYPE, void*> _Links_Container_Interface;
//			typedef boost::container::vector<typename MasterType::routable_link_type*> _Links_Container_Interface;
//			typedef Network_Components::Prototypes::Network<typename MasterType::routable_network_type> _Routable_Network_Interface;
//			typedef typename MasterType::link_type regular_link_type;
//			typedef typename MasterType::routable_link_type routable_link_type;
//			typedef typename MasterType::vehicle_type vehicle_type;
//			typedef typename MasterType::routable_network_type routable_network_type;
//			typedef  Person_Components::Prototypes::Person< typename MasterType::person_type> _Traveler_Interface;
//			typedef  Network_Components::Prototypes::Network< typename MasterType::network_type> _Network_Interface;
//			//==================================================================================================
//
//
//			static void Compute_Route_Condition(ComponentType* _this,Event_Response& response)
//			{
//				typedef Routing_Components::Prototypes::Routing<typename MasterType::routing_type> _Routing_Interface;
//				typedef  Traveler_Components::Prototypes::Traveler< type_of(traveler)> _Traveler_Interface;
//				_Routing_Interface* _this_ptr=(_Routing_Interface*)_this;
//				if(sub_iteration() == Scenario_Components::Types::Type_Sub_Iteration_keys::ROUTING_SUB_ITERATION)
//				{
//					//response.result=true;
//					response.next._iteration=END;
//					response.next._sub_iteration=Scenario_Components::Types::Type_Sub_Iteration_keys::ROUTING_SUB_ITERATION;
//					_this_ptr->Compute_Route<NT>();
//				}
//				else
//				{
//					assert(false);
//					cout << "Should never reach here in routing conditional!" << endl;
//				}
//
//				//CHECK_CONDITIONAL
//			}
//
//			// don't need ifdef here - just change the typedef of MasterType::person_type to traveler_implemenationt in the mastertype definition
//			m_prototype(Null_Prototype,typename MasterType::person_type>, traveler, NONE, NONE);
//			m_prototype(Null_Prototype,typename MasterType::network_type>, network, NONE, NONE);
//			m_prototype(Null_Prototype,typename MasterType::movement_plan_type>, movement_plan, NONE, NONE);
//			m_data(int, departure_time, NONE, NONE);
//
//			template<typename TargetType> TargetType vehicle()
//			{
//				return ((_Traveler_Interface*)_traveler)->template vehicle<TargetType>();
//			}
//			tag_getter_as_available(vehicle);
//
//			template<typename TargetType> TargetType routable_network()
//			{
//				return ((_Network_Interface*)_network)->template routable_network<TargetType>();
//			}		
//			tag_getter_as_available(routable_network);			
//
//
//			template<typename TargetType> TargetType realtime_routable_network()
//			{
//				return ((_Network_Interface*)_network)->template realtime_routable_network<TargetType>();
//			}		
//			tag_getter_as_available(realtime_routable_network);			
//
//			template<typename TargetType> void routable_origin(TargetType set_value)
//			{
//				_routable_origin = set_value->template internal_id<int>();
//			}
//			template<typename TargetType> TargetType routable_origin()
//			{
//				return (TargetType)(((_Network_Interface*)_network)->template routable_network<_Routable_Network_Interface*>()->template links_container<_Links_Container_Interface&>()[_routable_origin]);
//			}
//			tag_getter_setter_as_available(routable_origin);
//			template<typename TargetType> TargetType routable_origin_index()
//			{
//				return (TargetType)_routable_origin;
//			}
//			tag_getter_as_available(routable_origin_index);			
//			template<typename TargetType> void routable_destination(TargetType set_value)
//			{
//				_routable_destination = set_value->template internal_id<int>();
//			}
//			template<typename TargetType> TargetType routable_destination()
//			{
//				return (TargetType)(((_Network_Interface*)_network)->template routable_network<_Routable_Network_Interface*>()->template links_container<_Links_Container_Interface&>()[_routable_destination]);
//			}
//			tag_getter_setter_as_available(routable_destination);
//			template<typename TargetType> TargetType routable_destination_index()
//			{
//				return (TargetType)_routable_destination;
//			}
//			tag_getter_as_available(routable_destination_index);			
//
//			int _routable_origin;
//			int _routable_destination;	
//		};

//		implementation struct Routable_Network_Implementation : public Polaris_Component<MasterType,INHERIT(Routable_Network_Implementation),Data_Object>
//		{
//			m_data(concat(boost::container::multiset<pair<float, void*>>), scan_list, NONE, NONE);
//
//			m_data(float, max_free_flow_speed, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
//
//			m_container(boost::container::vector<typename MasterType::routable_intersection_type*>, intersections_container, NONE, NONE);
//
//			m_container(boost::container::vector<typename MasterType::routable_link_type*>, links_container, NONE, NONE);
//			m_container(boost::container::vector<typename MasterType::routable_link_type*>, reset_links, NONE, NONE);
//
//			m_container(boost::container::vector<typename MasterType::link_type*>, reversed_path_container, NONE, NONE);
//
//			typedef typename MasterType::network_type regular_network_type;
//
//			template<typename TargetType> void reset_routable_network()
//			{
//				typedef  Link_Components::Prototypes::Link<typename remove_pointer<typename  type_of(links_container)::value_type>::type>  _Routable_Link_Interface;
//				typedef  Random_Access_Sequence< type_of(links_container), _Routable_Link_Interface*> _Routable_Links_Container_Interface;
//
//
//				typedef scan_list_type ScanListType;
//				_scan_list.clear();
//				 
//				_reversed_path_container.clear();
//				typename _Routable_Links_Container_Interface::iterator link_itr;
//
//				for(link_itr=_reset_links.begin();link_itr!=_reset_links.end();link_itr++)
//				{
//					_Routable_Link_Interface* link_ptr = (_Routable_Link_Interface*)(*link_itr);
//					link_ptr->template reset_routable_link<NULLTYPE>();
//				}
//				_reset_links.clear();
//			}
//
//			template<typename TargetType> void read_network_data(typename TargetType::ParamType regular_network, requires(TargetType,check_2(typename TargetType::NetIOType,Network_Components::Types::Regular_Network,is_same)))
//			{
//
//				typedef  Link_Components::Prototypes::Link<typename remove_pointer<typename  type_of(links_container)::value_type>::type>  _Routable_Link_Interface;
//				typedef  Random_Access_Sequence< type_of(links_container), _Routable_Link_Interface*> _Routable_Links_Container_Interface;
//
//				typedef  Intersection_Components::Prototypes::Intersection<typename remove_pointer<typename  type_of(intersections_container)::value_type>::type>  _Routable_Intersection_Interface;
//				typedef  Random_Access_Sequence< type_of(intersections_container), _Routable_Intersection_Interface*> _Routable_Intersections_Container_Interface;
//
//				typedef Outbound_Inbound_Movements<typename remove_pointer< typename _Routable_Intersection_Interface::get_type_of(outbound_inbound_movements)::value_type>::type>  _Routable_Outbound_Inbound_Movements_Interface;
//				typedef  Random_Access_Sequence< typename _Routable_Intersection_Interface::get_type_of(outbound_inbound_movements), _Routable_Outbound_Inbound_Movements_Interface*> _Routable_Outbound_Inbound_Movements_Container_Interface;
//
//				typedef  Intersection_Components::Prototypes::Inbound_Outbound_Movements<typename remove_pointer< typename _Routable_Intersection_Interface::get_type_of(inbound_outbound_movements)::value_type>::type>  _Routable_Inbound_Outbound_Movements_Interface;
//				typedef  Random_Access_Sequence< typename _Routable_Intersection_Interface::get_type_of(inbound_outbound_movements), _Routable_Inbound_Outbound_Movements_Interface*> _Routable_Inbound_Outbound_Movements_Container_Interface;
//
//				typedef  Turn_Movement_Components::Prototypes::Movement<typename remove_pointer< typename _Routable_Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements)::value_type>::type>  _Routable_Movement_Interface;
//				typedef  Random_Access_Sequence< typename _Routable_Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements), _Routable_Movement_Interface*> _Routable_Movements_Container_Interface;
//
//
//				typedef Network_Components::Prototypes::Network<regular_network_type> _Regular_Network_Interface;
//				typedef  Link_Components::Prototypes::Link<typename remove_pointer< typename regular_network_type::links_container_type::value_type>::type>  _Regular_Link_Interface;
//				typedef  Random_Access_Sequence< typename regular_network_type::links_container_type, _Regular_Link_Interface*> _Regular_Links_Container_Interface;
//
//				typedef  Intersection_Components::Prototypes::Intersection<typename remove_pointer< typename regular_network_type::intersections_container_type::value_type>::type>  _Regular_Intersection_Interface;
//				typedef  Random_Access_Sequence< typename regular_network_type::intersections_container_type, _Regular_Intersection_Interface*> _Regular_Intersections_Container_Interface;
//
//				typedef  Intersection_Components::Prototypes::Outbound_Inbound_Movements<typename remove_pointer< typename _Regular_Intersection_Interface::get_type_of(outbound_inbound_movements)::value_type>::type>  _Regular_Outbound_Inbound_Movements_Interface;
//				typedef  Random_Access_Sequence< typename _Regular_Intersection_Interface::get_type_of(outbound_inbound_movements), _Regular_Outbound_Inbound_Movements_Interface*> _Regular_Outbound_Inbound_Movements_Container_Interface;
//
//				typedef  Intersection_Components::Prototypes::Inbound_Outbound_Movements<typename remove_pointer< typename _Regular_Intersection_Interface::get_type_of(inbound_outbound_movements)::value_type>::type>  _Regular_Inbound_Outbound_Movements_Interface;
//				typedef  Random_Access_Sequence< typename _Regular_Intersection_Interface::get_type_of(inbound_outbound_movements), _Regular_Inbound_Outbound_Movements_Interface*> _Regular_Inbound_Outbound_Movements_Container_Interface;
//
//				typedef  Turn_Movement_Components::Prototypes::Movement<typename remove_pointer< typename _Regular_Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements)::value_type>::type>  _Regular_Movement_Interface;
//				typedef  Random_Access_Sequence< typename _Regular_Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements), _Regular_Movement_Interface*> _Regular_Movements_Container_Interface;
//
//				
//
//				_max_free_flow_speed = ((_Regular_Network_Interface*)regular_network)->template max_free_flow_speed<float>();
//
//				
//				_Regular_Links_Container_Interface& regular_links_container = ((_Regular_Network_Interface*)regular_network)->template links_container<_Regular_Links_Container_Interface&>();
//				
//				// copy all links
//				cout << "copying links" << endl;
//				dense_hash_map<int, _Routable_Link_Interface*> linksMap;
//				linksMap.set_empty_key(-1);
//				linksMap.set_deleted_key(-2);
//				typename _Regular_Links_Container_Interface::iterator regular_link_itr;
//				for(regular_link_itr=regular_links_container.begin(); regular_link_itr!=regular_links_container.end(); regular_link_itr++)
//				{
//					_Regular_Link_Interface* regular_link = (_Regular_Link_Interface*)(*regular_link_itr);
//					_Routable_Link_Interface* routable_link =  (_Routable_Link_Interface*)Allocate<typename _Routable_Link_Interface::Component_Type>();
//					routable_link->template construct_routable_from_regular<_Regular_Link_Interface*>(regular_link);
//					links_container<_Routable_Links_Container_Interface&>().push_back(routable_link);
//					linksMap.insert(pair<int, _Routable_Link_Interface*>(regular_link->template internal_id<int>(), routable_link));
//					regular_link->template replicas_container<_Routable_Links_Container_Interface&>().push_back(routable_link);
//				}
//				cout << "copying intersections" << endl;
//				// copy all intersections
//				int counter = -1;
//				_Regular_Intersections_Container_Interface& regular_intersections_container = ((_Regular_Network_Interface*)regular_network)->template intersections_container<_Regular_Intersections_Container_Interface&>();
//				dense_hash_map<int, _Routable_Intersection_Interface*> intersectionsMap;
//				intersectionsMap.set_empty_key(-1);
//				intersectionsMap.set_deleted_key(-2);
//				typename _Regular_Intersections_Container_Interface::iterator regular_intersection_itr;
//				for(regular_intersection_itr=regular_intersections_container.begin(); regular_intersection_itr!=regular_intersections_container.end(); regular_intersection_itr++)
//				{
//					if (++counter % 10000 == 0)
//					{
//						cout << counter << " intersections copied" << endl;
//					}
//					_Regular_Intersection_Interface* regular_intersection = (_Regular_Intersection_Interface*)(*regular_intersection_itr);
//					_Routable_Intersection_Interface* routable_intersection =  (_Routable_Intersection_Interface*)Allocate<typename _Routable_Intersection_Interface::Component_Type>();
//					typedef dense_hash_map<int,_Routable_Link_Interface*>& linksMapType;
//					
//					//routable_intersection->template construct_routable_from_regular<Target_Type<NULLTYPE,void,_Regular_Intersection_Interface*,linksMapType>>(regular_intersection, linksMap);
//					
//					routable_intersection->template construct_routable_from_regular<_Regular_Intersection_Interface*,linksMapType>(regular_intersection, linksMap);
//					
//					//Target_Type<NULLTYPE,void,_Regular_Intersection_Interface*,linksMapType>
//
//					intersections_container<_Routable_Intersections_Container_Interface&>().push_back(routable_intersection);
//					intersectionsMap.insert(pair<int, _Routable_Intersection_Interface*>(regular_intersection->template internal_id<int>(), routable_intersection));
//				}
//				cout << " adding up/down streams to each link" << endl;
//				// add upstream and downstream to each link
//				typename _Routable_Links_Container_Interface::iterator routable_link_itr;
//				for(routable_link_itr=_links_container.begin(); routable_link_itr!=_links_container.end(); routable_link_itr++)
//				{
//
//					_Routable_Link_Interface* routable_link = (_Routable_Link_Interface*)(*routable_link_itr);
//					_Regular_Link_Interface* regular_link = routable_link->template network_link_reference<_Regular_Link_Interface*>();
//		
//					_Regular_Intersection_Interface* regular_upstream_intersection = regular_link->template upstream_intersection<_Regular_Intersection_Interface*>();
//					_Routable_Intersection_Interface* routable_upstream_intersection = intersectionsMap.find(regular_upstream_intersection->template internal_id<int>())->second;
//					routable_link->template upstream_intersection<_Routable_Intersection_Interface*>(routable_upstream_intersection);
//					_Regular_Intersection_Interface* regular_downstream_intersection = regular_link->template downstream_intersection<_Regular_Intersection_Interface*>();
//					_Routable_Intersection_Interface* routable_downstream_intersection = intersectionsMap.find(regular_downstream_intersection->template internal_id<int>())->second;
//					routable_link->template downstream_intersection<_Routable_Intersection_Interface*>(routable_downstream_intersection);
//				}
//				cout << " streams added" << endl;
//			}
//
//			template<typename TargetType> void read_realtime_network_data(typename TargetType::ParamType regular_network, requires(TargetType,check_2(typename TargetType::NetIOType,Network_Components::Types::Regular_Network,is_same)))
//			{
//
//				typedef  Link_Components::Prototypes::Link<typename remove_pointer<typename  type_of(links_container)::value_type>::type>  _Routable_Link_Interface;
//				typedef  Random_Access_Sequence< type_of(links_container), _Routable_Link_Interface*> _Routable_Links_Container_Interface;
//
//				typedef  Intersection_Components::Prototypes::Intersection<typename remove_pointer<typename  type_of(intersections_container)::value_type>::type>  _Routable_Intersection_Interface;
//				typedef  Random_Access_Sequence< type_of(intersections_container), _Routable_Intersection_Interface*> _Routable_Intersections_Container_Interface;
//
//				typedef Outbound_Inbound_Movements<typename remove_pointer< typename _Routable_Intersection_Interface::get_type_of(outbound_inbound_movements)::value_type>::type>  _Routable_Outbound_Inbound_Movements_Interface;
//				typedef  Random_Access_Sequence< typename _Routable_Intersection_Interface::get_type_of(outbound_inbound_movements), _Routable_Outbound_Inbound_Movements_Interface*> _Routable_Outbound_Inbound_Movements_Container_Interface;
//
//				typedef  Intersection_Components::Prototypes::Inbound_Outbound_Movements<typename remove_pointer< typename _Routable_Intersection_Interface::get_type_of(inbound_outbound_movements)::value_type>::type>  _Routable_Inbound_Outbound_Movements_Interface;
//				typedef  Random_Access_Sequence< typename _Routable_Intersection_Interface::get_type_of(inbound_outbound_movements), _Routable_Inbound_Outbound_Movements_Interface*> _Routable_Inbound_Outbound_Movements_Container_Interface;
//
//				typedef  Turn_Movement_Components::Prototypes::Movement<typename remove_pointer< typename _Routable_Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements)::value_type>::type>  _Routable_Movement_Interface;
//				typedef  Random_Access_Sequence< typename _Routable_Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements), _Routable_Movement_Interface*> _Routable_Movements_Container_Interface;
//
//
//				typedef Network_Components::Prototypes::Network<regular_network_type> _Regular_Network_Interface;
//				typedef  Link_Components::Prototypes::Link<typename remove_pointer< typename regular_network_type::links_container_type::value_type>::type>  _Regular_Link_Interface;
//				typedef  Random_Access_Sequence< typename regular_network_type::links_container_type, _Regular_Link_Interface*> _Regular_Links_Container_Interface;
//
//				typedef  Intersection_Components::Prototypes::Intersection<typename remove_pointer< typename regular_network_type::intersections_container_type::value_type>::type>  _Regular_Intersection_Interface;
//				typedef  Random_Access_Sequence< typename regular_network_type::intersections_container_type, _Regular_Intersection_Interface*> _Regular_Intersections_Container_Interface;
//
//				typedef  Intersection_Components::Prototypes::Outbound_Inbound_Movements<typename remove_pointer< typename _Regular_Intersection_Interface::get_type_of(outbound_inbound_movements)::value_type>::type>  _Regular_Outbound_Inbound_Movements_Interface;
//				typedef  Random_Access_Sequence< typename _Regular_Intersection_Interface::get_type_of(outbound_inbound_movements), _Regular_Outbound_Inbound_Movements_Interface*> _Regular_Outbound_Inbound_Movements_Container_Interface;
//
//				typedef  Intersection_Components::Prototypes::Inbound_Outbound_Movements<typename remove_pointer< typename _Regular_Intersection_Interface::get_type_of(inbound_outbound_movements)::value_type>::type>  _Regular_Inbound_Outbound_Movements_Interface;
//				typedef  Random_Access_Sequence< typename _Regular_Intersection_Interface::get_type_of(inbound_outbound_movements), _Regular_Inbound_Outbound_Movements_Interface*> _Regular_Inbound_Outbound_Movements_Container_Interface;
//
//				typedef  Turn_Movement_Components::Prototypes::Movement<typename remove_pointer< typename _Regular_Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements)::value_type>::type>  _Regular_Movement_Interface;
//				typedef  Random_Access_Sequence< typename _Regular_Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements), _Regular_Movement_Interface*> _Regular_Movements_Container_Interface;
//
//				
//
//				_max_free_flow_speed = ((_Regular_Network_Interface*)regular_network)->template max_free_flow_speed<float>();
//
//				
//				_Regular_Links_Container_Interface& regular_links_container = ((_Regular_Network_Interface*)regular_network)->template links_container<_Regular_Links_Container_Interface&>();
//				
//				// copy all links
//				cout << "copying links" << endl;
//				dense_hash_map<int, _Routable_Link_Interface*> linksMap;
//				linksMap.set_empty_key(-1);
//				linksMap.set_deleted_key(-2);
//				typename _Regular_Links_Container_Interface::iterator regular_link_itr;
//				for(regular_link_itr=regular_links_container.begin(); regular_link_itr!=regular_links_container.end(); regular_link_itr++)
//				{
//					_Regular_Link_Interface* regular_link = (_Regular_Link_Interface*)(*regular_link_itr);
//					_Routable_Link_Interface* routable_link =  (_Routable_Link_Interface*)Allocate<typename _Routable_Link_Interface::Component_Type>();
//					routable_link->template construct_realtime_routable_from_regular<_Regular_Link_Interface*>(regular_link);
//					links_container<_Routable_Links_Container_Interface&>().push_back(routable_link);
//					linksMap.insert(pair<int, _Routable_Link_Interface*>(regular_link->template internal_id<int>(), routable_link));
//					regular_link->template realtime_replicas_container<_Routable_Links_Container_Interface&>().push_back(routable_link);
//				}
//				cout << "copying intersections" << endl;
//				// copy all intersections
//				int counter = -1;
//				_Regular_Intersections_Container_Interface& regular_intersections_container = ((_Regular_Network_Interface*)regular_network)->template intersections_container<_Regular_Intersections_Container_Interface&>();
//				dense_hash_map<int, _Routable_Intersection_Interface*> intersectionsMap;
//				intersectionsMap.set_empty_key(-1);
//				intersectionsMap.set_deleted_key(-2);
//				typename _Regular_Intersections_Container_Interface::iterator regular_intersection_itr;
//				for(regular_intersection_itr=regular_intersections_container.begin(); regular_intersection_itr!=regular_intersections_container.end(); regular_intersection_itr++)
//				{
//					if (++counter % 10000 == 0)
//					{
//						cout << counter << " intersections copied" << endl;
//					}
//					_Regular_Intersection_Interface* regular_intersection = (_Regular_Intersection_Interface*)(*regular_intersection_itr);
//					_Routable_Intersection_Interface* routable_intersection =  (_Routable_Intersection_Interface*)Allocate<typename _Routable_Intersection_Interface::Component_Type>();
//					typedef dense_hash_map<int,_Routable_Link_Interface*>& linksMapType;
////TODO
////					routable_intersection->template construct_realtime_routable_from_regular<Target_Type<NULLTYPE,void,_Regular_Intersection_Interface*,linksMapType>>(regular_intersection, linksMap);
//
//					intersections_container<_Routable_Intersections_Container_Interface&>().push_back(routable_intersection);
//					intersectionsMap.insert(pair<int, _Routable_Intersection_Interface*>(regular_intersection->template internal_id<int>(), routable_intersection));
//				}
//				cout << " adding up/down streams to each link" << endl;
//				// add upstream and downstream to each link
//				typename _Routable_Links_Container_Interface::iterator routable_link_itr;
//				for(routable_link_itr=_links_container.begin(); routable_link_itr!=_links_container.end(); routable_link_itr++)
//				{
//
//					_Routable_Link_Interface* routable_link = (_Routable_Link_Interface*)(*routable_link_itr);
//					_Regular_Link_Interface* regular_link = routable_link->template network_link_reference<_Regular_Link_Interface*>();
//		
//					_Regular_Intersection_Interface* regular_upstream_intersection = regular_link->template upstream_intersection<_Regular_Intersection_Interface*>();
//					_Routable_Intersection_Interface* routable_upstream_intersection = intersectionsMap.find(regular_upstream_intersection->template internal_id<int>())->second;
//					routable_link->template upstream_intersection<_Routable_Intersection_Interface*>(routable_upstream_intersection);
//					_Regular_Intersection_Interface* regular_downstream_intersection = regular_link->template downstream_intersection<_Regular_Intersection_Interface*>();
//					_Routable_Intersection_Interface* routable_downstream_intersection = intersectionsMap.find(regular_downstream_intersection->template internal_id<int>())->second;
//					routable_link->template downstream_intersection<_Routable_Intersection_Interface*>(routable_downstream_intersection);
//				}
//				cout << " streams added" << endl;
//			}
//
//			template<typename TargetType> void read_snapshot_data(float maximum_free_flow_speed, typename TargetType::ParamType link_travel_time_map, typename TargetType::ParamType movement_travel_time_map)
//			{
//				typedef  Link_Components::Prototypes::Link<typename remove_pointer<typename  type_of(links_container)::value_type>::type>  _Routable_Link_Interface;
//				typedef  Random_Access_Sequence< type_of(links_container), _Routable_Link_Interface*> _Routable_Links_Container_Interface;
//
//				typedef  Intersection_Components::Prototypes::Intersection<typename remove_pointer<typename  type_of(intersections_container)::value_type>::type>  _Routable_Intersection_Interface;
//				typedef  Random_Access_Sequence< type_of(intersections_container), _Routable_Intersection_Interface*> _Routable_Intersections_Container_Interface;
//
//				typedef Outbound_Inbound_Movements<typename remove_pointer< typename _Routable_Intersection_Interface::get_type_of(outbound_inbound_movements)::value_type>::type>  _Routable_Outbound_Inbound_Movements_Interface;
//				typedef  Random_Access_Sequence< typename _Routable_Intersection_Interface::get_type_of(outbound_inbound_movements), _Routable_Outbound_Inbound_Movements_Interface*> _Routable_Outbound_Inbound_Movements_Container_Interface;
//
//				typedef  Intersection_Components::Prototypes::Inbound_Outbound_Movements<typename remove_pointer< typename _Routable_Intersection_Interface::get_type_of(inbound_outbound_movements)::value_type>::type>  _Routable_Inbound_Outbound_Movements_Interface;
//				typedef  Random_Access_Sequence< typename _Routable_Intersection_Interface::get_type_of(inbound_outbound_movements), _Routable_Inbound_Outbound_Movements_Interface*> _Routable_Inbound_Outbound_Movements_Container_Interface;
//
//				typedef  Turn_Movement_Components::Prototypes::Movement<typename remove_pointer< typename _Routable_Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements)::value_type>::type>  _Routable_Movement_Interface;
//				typedef  Random_Access_Sequence< typename _Routable_Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements), _Routable_Movement_Interface*> _Routable_Movements_Container_Interface;
//
//
//				typedef Network_Components::Prototypes::Network<regular_network_type> _Regular_Network_Interface;
//				typedef  Link_Components::Prototypes::Link<typename remove_pointer< typename regular_network_type::links_container_type::value_type>::type>  _Regular_Link_Interface;
//				typedef  Random_Access_Sequence< typename regular_network_type::links_container_type, _Regular_Link_Interface*> _Regular_Links_Container_Interface;
//
//				typedef  Intersection_Components::Prototypes::Intersection<typename remove_pointer< typename regular_network_type::intersections_container_type::value_type>::type>  _Regular_Intersection_Interface;
//				typedef  Random_Access_Sequence< typename regular_network_type::intersections_container_type, _Regular_Intersection_Interface*> _Regular_Intersections_Container_Interface;
//
//				typedef  Intersection_Components::Prototypes::Outbound_Inbound_Movements<typename remove_pointer< typename _Regular_Intersection_Interface::get_type_of(outbound_inbound_movements)::value_type>::type>  _Regular_Outbound_Inbound_Movements_Interface;
//				typedef  Random_Access_Sequence< typename _Regular_Intersection_Interface::get_type_of(outbound_inbound_movements), _Regular_Outbound_Inbound_Movements_Interface*> _Regular_Outbound_Inbound_Movements_Container_Interface;
//
//				typedef  Intersection_Components::Prototypes::Inbound_Outbound_Movements<typename remove_pointer< typename _Regular_Intersection_Interface::get_type_of(inbound_outbound_movements)::value_type>::type>  _Regular_Inbound_Outbound_Movements_Interface;
//				typedef  Random_Access_Sequence< typename _Regular_Intersection_Interface::get_type_of(inbound_outbound_movements), _Regular_Inbound_Outbound_Movements_Interface*> _Regular_Inbound_Outbound_Movements_Container_Interface;
//
//				typedef  Turn_Movement_Components::Prototypes::Movement<typename remove_pointer< typename _Regular_Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements)::value_type>::type>  _Regular_Movement_Interface;
//				typedef  Random_Access_Sequence< typename _Regular_Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements), _Regular_Movement_Interface*> _Regular_Movements_Container_Interface;
//
//				
//
//				_max_free_flow_speed = maximum_free_flow_speed;
//
//				
//				_Regular_Links_Container_Interface& regular_links_container = ((_Regular_Network_Interface*)_global_network)->template links_container<_Regular_Links_Container_Interface&>();
//				
//				// copy all links
//				cout << "copying links" << endl;
//				dense_hash_map<int, _Routable_Link_Interface*> linksMap;
//				linksMap.set_empty_key(-1);
//				linksMap.set_deleted_key(-2);
//				typename _Regular_Links_Container_Interface::iterator regular_link_itr;
//				for(regular_link_itr=regular_links_container.begin(); regular_link_itr!=regular_links_container.end(); regular_link_itr++)
//				{
//					_Regular_Link_Interface* regular_link = (_Regular_Link_Interface*)(*regular_link_itr);
//					_Routable_Link_Interface* routable_link =  (_Routable_Link_Interface*)Allocate<typename _Routable_Link_Interface::Component_Type>();
//					routable_link->template construct_routable_from_regular<_Regular_Link_Interface*>(regular_link);
//					int link_uuid = routable_link->template uuid<int>();
//					float link_travel_time = link_travel_time_map[link_uuid];
//					routable_link->template travel_time<float>(link_travel_time);
//					links_container<_Routable_Links_Container_Interface&>().push_back(routable_link);
//					linksMap.insert(pair<int, _Routable_Link_Interface*>(regular_link->template internal_id<int>(), routable_link));
//				}
//				cout << "copying intersections" << endl;
//				// copy all intersections
//				int counter = -1;
//				_Regular_Intersections_Container_Interface& regular_intersections_container = ((_Regular_Network_Interface*)_global_network)->template intersections_container<_Regular_Intersections_Container_Interface&>();
//				dense_hash_map<int, _Routable_Intersection_Interface*> intersectionsMap;
//				intersectionsMap.set_empty_key(-1);
//				intersectionsMap.set_deleted_key(-2);
//				typename _Regular_Intersections_Container_Interface::iterator regular_intersection_itr;
//				for(regular_intersection_itr=regular_intersections_container.begin(); regular_intersection_itr!=regular_intersections_container.end(); regular_intersection_itr++)
//				{
//					if (++counter % 10000 == 0)
//					{
//						cout << counter << " intersections copied" << endl;
//					}
//					_Regular_Intersection_Interface* regular_intersection = (_Regular_Intersection_Interface*)(*regular_intersection_itr);
//					_Routable_Intersection_Interface* routable_intersection =  (_Routable_Intersection_Interface*)Allocate<typename _Routable_Intersection_Interface::Component_Type>();
//					typedef dense_hash_map<int,_Routable_Link_Interface*>& linksMapType;
////TODO
////					routable_intersection->template construct_routable_from_regular<Target_Type<NULLTYPE,void,_Regular_Intersection_Interface*,linksMapType>>(regular_intersection, linksMap);
//
//					routable_intersection->template set_forward_link_turn_travel_time<typename MasterType::network_type::id_to_travel_time_map_type&>(movement_travel_time_map);
//					intersections_container<_Routable_Intersections_Container_Interface&>().push_back(routable_intersection);
//					intersectionsMap.insert(pair<int, _Routable_Intersection_Interface*>(regular_intersection->template internal_id<int>(), routable_intersection));
//				}
//				cout << " adding up/down streams to each link" << endl;
//				// add upstream and downstream to each link
//				typename _Routable_Links_Container_Interface::iterator routable_link_itr;
//				for(routable_link_itr=_links_container.begin(); routable_link_itr!=_links_container.end(); routable_link_itr++)
//				{
//
//					_Routable_Link_Interface* routable_link = (_Routable_Link_Interface*)(*routable_link_itr);
//					_Regular_Link_Interface* regular_link = routable_link->template network_link_reference<_Regular_Link_Interface*>();
//		
//					_Regular_Intersection_Interface* regular_upstream_intersection = regular_link->template upstream_intersection<_Regular_Intersection_Interface*>();
//					_Routable_Intersection_Interface* routable_upstream_intersection = intersectionsMap.find(regular_upstream_intersection->template internal_id<int>())->second;
//					routable_link->template upstream_intersection<_Routable_Intersection_Interface*>(routable_upstream_intersection);
//					_Regular_Intersection_Interface* regular_downstream_intersection = regular_link->template downstream_intersection<_Regular_Intersection_Interface*>();
//					_Routable_Intersection_Interface* routable_downstream_intersection = intersectionsMap.find(regular_downstream_intersection->template internal_id<int>())->second;
//					routable_link->template downstream_intersection<_Routable_Intersection_Interface*>(routable_downstream_intersection);
//				}
//			}
//
//			void print_routable_network(int time)
//			{
//				typedef Network<typename MasterType::routable_network_type> _Routable_Network_Interface;
//				typedef Network<typename MasterType::network_type> _Regular_Network_Interface;
//				typedef  Link_Components::Prototypes::Link<typename remove_pointer< typename _Routable_Network_Interface::get_type_of(links_container)::value_type>::type>  _Routable_Link_Interface;
//				typedef  Random_Access_Sequence< typename _Routable_Network_Interface::get_type_of(links_container), _Routable_Link_Interface*> _Routable_Links_Container_Interface;
//
//				typedef  Intersection_Components::Prototypes::Intersection<typename remove_pointer< typename _Routable_Network_Interface::get_type_of(intersections_container)::value_type>::type>  _Routable_Intersection_Interface;
//				typedef  Random_Access_Sequence< typename _Routable_Network_Interface::get_type_of(intersections_container), _Routable_Intersection_Interface*> _Routable_Intersections_Container_Interface;
//
//				typedef  Intersection_Components::Prototypes::Inbound_Outbound_Movements<typename remove_pointer< typename _Routable_Intersection_Interface::get_type_of(inbound_outbound_movements)::value_type>::type>  _Inbound_Outbound_Movements_Interface;
//				typedef  Random_Access_Sequence< typename _Routable_Intersection_Interface::get_type_of(inbound_outbound_movements), _Inbound_Outbound_Movements_Interface*> _Inbound_Outbound_Movements_Container_Interface;
//
//				typedef  Turn_Movement_Components::Prototypes::Movement<typename remove_pointer< typename _Inbound_Outbound_Movements_Interface::get_type_of(outbound_movements)::value_type>::type>  _Movement_Interface;
//				typedef  Random_Access_Sequence< typename _Inbound_Outbound_Movements_Interface::get_type_of(outbound_movements), _Movement_Interface*> _Movements_Container_Interface;
//
//				typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;
//
//
//				cout << "******* " << time << " ********" << endl;
//
//				_Routable_Network_Interface* routable_net = (_Routable_Network_Interface*)(this);
//				_Routable_Intersections_Container_Interface& routable_intersections_container = routable_net->template intersections_container<_Routable_Intersections_Container_Interface&>();
//				typename _Routable_Intersections_Container_Interface::iterator intersection_itr;
//				for (intersection_itr = routable_intersections_container.begin(); intersection_itr != routable_intersections_container.end(); intersection_itr++)
//				{
//
//					_Routable_Intersection_Interface* intersection = (_Routable_Intersection_Interface*)(*intersection_itr);
//					_Inbound_Outbound_Movements_Container_Interface& routable_inbound_outbound_movements_container = intersection->template inbound_outbound_movements<_Inbound_Outbound_Movements_Container_Interface&>();
//					typename _Inbound_Outbound_Movements_Container_Interface::iterator inbound_outbound_movements_itr;
//					for (inbound_outbound_movements_itr = routable_inbound_outbound_movements_container.begin(); inbound_outbound_movements_itr != routable_inbound_outbound_movements_container.end(); inbound_outbound_movements_itr++)
//					{
//
//						_Inbound_Outbound_Movements_Interface* inbound_outbound_movements = (_Inbound_Outbound_Movements_Interface*)(*inbound_outbound_movements_itr);
//						_Routable_Link_Interface* inbound_link = inbound_outbound_movements->template inbound_link_reference<_Routable_Link_Interface*>();
//						_Movements_Container_Interface& outbound_movements = inbound_outbound_movements->template outbound_movements<_Movements_Container_Interface&>();
//						cout
//							<< inbound_link->template uuid<int>() << "\t"
//							<< inbound_link->template travel_time<float>() << "\t"
//							<< outbound_movements.size()
//							<< endl;
//
//						typename _Movements_Container_Interface::iterator movement_itr;
//						for (movement_itr = outbound_movements.begin(); movement_itr != outbound_movements.end(); movement_itr++)
//						{
//							_Movement_Interface* movement = (_Movement_Interface*)(*movement_itr);
//							
//							cout
//								<< movement->template uuid<int>() << ","
//								<< movement->template forward_link_turn_travel_time<float>()
//								<< endl;
//						}
//					}
//				}
//
//				cout << "starting outbound movements" << endl;
//				_Routable_Links_Container_Interface& routable_links_container = routable_net->template links_container<_Routable_Links_Container_Interface&>();
//				typename _Routable_Links_Container_Interface::iterator link_itr;
//				for (link_itr = routable_links_container.begin(); link_itr != routable_links_container.end(); link_itr++)
//				{
//					_Routable_Link_Interface* link = (_Routable_Link_Interface*)(*link_itr);
//					typedef  Turn_Movement_Components::Prototypes::Movement<typename remove_pointer< typename _Routable_Link_Interface::get_type_of(outbound_turn_movements)::value_type>::type>  _Routable_Movement_Interface;
//					typedef  Random_Access_Sequence< typename _Routable_Link_Interface::get_type_of(outbound_turn_movements), _Routable_Movement_Interface*> _Routable_Movements_Container_Interface;
//
//					_Routable_Movements_Container_Interface& routable_outbound_movements = link->template outbound_turn_movements<_Routable_Movements_Container_Interface&>();
//					typename _Routable_Movements_Container_Interface::iterator routable_movement_itr;
//					cout << "print outbound morements of link " << link->template uuid<int>() << endl;
//					for (routable_movement_itr = routable_outbound_movements.begin(); routable_movement_itr != routable_outbound_movements.end(); routable_movement_itr++)
//					{
//						_Routable_Movement_Interface* routable_movement = (_Routable_Movement_Interface*)(*routable_movement_itr);
//						cout << "\t" << routable_movement->template uuid<int>();
//						cout << ": inbound_link = " << routable_movement->template inbound_link<_Routable_Link_Interface*>()->template uuid<int>();
//						cout << ", outbound_link = " << routable_movement->template outbound_link<_Routable_Link_Interface*>()->template uuid<int>();
//						cout << ", forward_link_turn = " << routable_movement->template forward_link_turn_travel_time<float>() << endl;
//					}
//					cout << endl;
//				}
//								
//				cout << "***************" << endl;
//			}
//		};
//
//

//
//#ifndef EXCLUDE_DEMAND
//		implementation struct Skim_Routing_Implementation: public Routing_Implementation<MasterType,INHERIT(Skim_Routing_Implementation)>
//		{
//			typedef typename  Routing_Implementation<MasterType,INHERIT(Skim_Routing_Implementation)>::Component_Type ComponentType;
//			static void Compute_Route_Condition(ComponentType* _this,Event_Response& response)
//			{
//				typedef Routing_Components::Prototypes::Routing<ComponentType> _Routing_Interface;
//				
//				_Routing_Interface* _this_ptr=(_Routing_Interface*)_this;
//				if(sub_iteration() == Network_Skimming_Components::Types::SUB_ITERATIONS::PATH_BUILDING)
//				{
//					if (iteration() >= (int)_this_ptr->start_time<Simulation_Timestep_Increment>() && iteration() < (int)_this_ptr->end_time<Simulation_Timestep_Increment>())
//					{
//						response.result=true;
//						response.next._iteration=Simulation_Time.Future_Time<Simulation_Timestep_Increment,Simulation_Timestep_Increment>(_this_ptr->update_increment<Simulation_Timestep_Increment>());
//						response.next._sub_iteration=Network_Skimming_Components::Types::SUB_ITERATIONS::PATH_BUILDING;
//					}
//					else
//					{
//						response.result=false;
//						response.next._iteration=Simulation_Time.Future_Time<Simulation_Timestep_Increment,Simulation_Timestep_Increment>(_this_ptr->update_increment<Simulation_Timestep_Increment>());
//						response.next._sub_iteration=Network_Skimming_Components::Types::SUB_ITERATIONS::PATH_BUILDING;
//					}
//				}
//				else
//				{
//					assert(false);
//					cout << "Should never reach here in routing conditional!" << endl;
//				}
//
//				CHECK_CONDITIONAL
//			}
//			m_container(boost::container::vector<float>,travel_times_to_link_container, NONE, NONE);
//			
//			member_component_and_feature_accessor(update_increment,Value,Basic_Units::Prototypes::Time,Basic_Units::Implementations::Time_Implementation<MasterType>);
//			member_component_and_feature_accessor(start_time,Value,Basic_Units::Prototypes::Time,Basic_Units::Implementations::Time_Implementation<MasterType>);
//			member_component_and_feature_accessor(end_time,Value,Basic_Units::Prototypes::Time,Basic_Units::Implementations::Time_Implementation<MasterType>);
//
//		};
//#endif
}

}
