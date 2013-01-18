#pragma once
#include "Intersection_Prototype.h"
#include "../File_IO/utilities.h"

#ifndef WINDOWS
// for hash_map
using namespace __gnu_cxx;
#endif
/*
 * Needed for Linux compatability. Do not hurt Windows compilation.
 */

namespace Link_Components {
	namespace Prototypes
	{
		forward_declare_prototype struct Link_Prototype;
	}
};
namespace Vehicle_Components {
	namespace Prototypes
	{
		forward_declare_prototype struct Vehicle_Prototype;
	}
};
namespace Routable_Network_Components {
	namespace Prototypes
	{
		forward_declare_prototype struct Routable_Network_Prototype;
	}
};
namespace Intersection_Components {
	namespace Prototypes
	{
		forward_declare_prototype struct Intersection_Prototype;
		forward_declare_prototype struct Movement_Prototype;
		forward_declare_prototype struct Outbound_Inbound_Movements_Prototype;
		forward_declare_prototype struct Inbound_Outbound_Movements_Prototype;
	}
};
namespace Routing_Components {
	namespace Prototypes
	{
		forward_declare_prototype struct Routing_Prototype;
	}
};
namespace Traveler_Components {
	namespace Prototypes
	{
		forward_declare_prototype struct Traveler_Prototype;
	}
};
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
		enum Scan_List_Status_Keys
		{
				UNSCANNED=0,
				SCANNED,
				INSELIST,
		};

		prototype struct Routable_Network_Prototype
		{
			tag_as_prototype;

			feature_accessor(scan_list, none, none);
			//feature_accessor(num_searches);
			feature_accessor(max_free_flow_speed, none, none);
			feature_accessor(intersections, none, none);
			feature_accessor(links, none, none);
			feature_accessor(reversed_path_container, none, none);

			feature_prototype void Reset()
			{
				typedef typename ComponentType::scan_list_type ScanListType;
				scan_list<ScanListType&>().clear();
				
				define_container_and_value_interface(_Reversed_Path_Container_Interface, _Regular_Link_Interface, get_type_of(reversed_path_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);  
				reversed_path_container<_Reversed_Path_Container_Interface&>().clear();

				define_container_and_value_interface(_Routable_Links_Container_Interface, _Routable_Link_Interface, get_type_of(links), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
				_Routable_Links_Container_Interface& links_container=links<_Routable_Links_Container_Interface&>();
				typename _Routable_Links_Container_Interface::iterator link_itr;

				for(link_itr=links_container.begin();link_itr!=links_container.end();link_itr++)
				{
					_Routable_Link_Interface* link_ptr = (_Routable_Link_Interface*)(*link_itr);
					link_ptr->template label_cost<float>(INFINITY_FLOAT);
					link_ptr->template label_pointer<_Routable_Link_Interface*>(link_ptr);
					link_ptr->template scan_list_status<Scan_List_Status_Keys>(UNSCANNED);
					link_ptr->template f_cost<float>(0.0);
					link_ptr->template h_cost<float>(0.0);
				}
			}

			feature_prototype void read_routable_network_data(void* regular_network)
			{
				define_container_and_value_interface(_Routable_Links_Container_Interface, _Routable_Link_Interface, get_type_of(links), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_container_and_value_interface(_Routable_Intersections_Container_Interface, _Routable_Intersection_Interface, get_type_of(intersections), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Intersection_Prototype, ComponentType);
				define_container_and_value_interface(_Routable_Outbound_Inbound_Movements_Container_Interface, _Routable_Outbound_Inbound_Movements_Interface, _Routable_Intersection_Interface::get_type_of(outbound_inbound_movements), Random_Access_Sequence_Prototype,Outbound_Inbound_Movements_Prototype, ComponentType);
				define_container_and_value_interface(_Routable_Inbound_Outbound_Movements_Container_Interface, _Routable_Inbound_Outbound_Movements_Interface, _Routable_Intersection_Interface::get_type_of(inbound_outbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Inbound_Outbound_Movements_Prototype, ComponentType);
				define_container_and_value_interface(_Routable_Movements_Container_Interface, _Routable_Movement_Interface, _Routable_Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Movement_Prototype, ComponentType);

				typedef Network_Components::Prototypes::Network_Prototype<typename ComponentType::regular_network_type, ComponentType> _Regular_Network_Interface;
				define_container_and_value_interface(_Regular_Links_Container_Interface, _Regular_Link_Interface, ComponentType::regular_network_type::links_container_type, Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_container_and_value_interface(_Regular_Intersections_Container_Interface, _Regular_Intersection_Interface, ComponentType::regular_network_type::intersections_container_type, Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Intersection_Prototype, ComponentType);
				define_container_and_value_interface(_Regular_Outbound_Inbound_Movements_Container_Interface, _Regular_Outbound_Inbound_Movements_Interface, _Regular_Intersection_Interface::get_type_of(outbound_inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Outbound_Inbound_Movements_Prototype, ComponentType);
				define_container_and_value_interface(_Regular_Inbound_Outbound_Movements_Container_Interface, _Regular_Inbound_Outbound_Movements_Interface, _Regular_Intersection_Interface::get_type_of(inbound_outbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Inbound_Outbound_Movements_Prototype, ComponentType);
				define_container_and_value_interface(_Regular_Movements_Container_Interface, _Regular_Movement_Interface, _Regular_Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Movement_Prototype, ComponentType);
				

				max_free_flow_speed<float>(((_Regular_Network_Interface*)regular_network)->template max_free_flow_speed<float>());

				
				_Regular_Links_Container_Interface& regular_links_container = ((_Regular_Network_Interface*)regular_network)->template links_container<_Regular_Links_Container_Interface&>();
				
				// copy all links
				cout << "copying links" << endl;
				dense_hash_map<int, _Routable_Link_Interface*> linksMap;
				linksMap.set_empty_key(-1);
				linksMap.set_deleted_key(-2);
				typename _Regular_Links_Container_Interface::iterator regular_link_itr;
				for(regular_link_itr=regular_links_container.begin(); regular_link_itr!=regular_links_container.end(); regular_link_itr++)
				{
					_Regular_Link_Interface* regular_link = (_Regular_Link_Interface*)(*regular_link_itr);
					_Routable_Link_Interface* routable_link =  (_Routable_Link_Interface*)Allocate<typename _Routable_Link_Interface::Component_Type>();
					routable_link->template network_link_reference<_Regular_Link_Interface*>(regular_link);
					routable_link->template f_cost<float>(0.0);
					routable_link->template h_cost<float>(0.0);
					routable_link->template label_pointer<void*>(routable_link);
					routable_link->template label_cost<float>(0.0);
					routable_link->template uuid<int>(regular_link->template uuid<int>());
					routable_link->template internal_id<int>(regular_link->template internal_id<int>());
					routable_link->template travel_time<float>(regular_link->template travel_time<float>());
					links<_Routable_Links_Container_Interface&>().push_back(routable_link);
					linksMap.insert(pair<int, _Routable_Link_Interface*>(regular_link->template internal_id<int>(), routable_link));
					regular_link->template replicas_container<_Routable_Links_Container_Interface&>().push_back(routable_link);
				}
				cout << "copying intersections" << endl;
				// copy all intersections
				int counter = -1;
				_Regular_Intersections_Container_Interface& regular_intersections_container = ((_Regular_Network_Interface*)regular_network)->template intersections_container<_Regular_Intersections_Container_Interface&>();
				dense_hash_map<int, _Routable_Intersection_Interface*> intersectionsMap;
				intersectionsMap.set_empty_key(-1);
				intersectionsMap.set_deleted_key(-2);
				typename _Regular_Intersections_Container_Interface::iterator regular_intersection_itr;
				for(regular_intersection_itr=regular_intersections_container.begin(); regular_intersection_itr!=regular_intersections_container.end(); regular_intersection_itr++)
				{
					if (++counter % 10000 == 0)
					{
						cout << counter << " intersections copied" << endl;
					}
					_Regular_Intersection_Interface* regular_intersection = (_Regular_Intersection_Interface*)(*regular_intersection_itr);
					_Routable_Intersection_Interface* routable_intersection =  (_Routable_Intersection_Interface*)Allocate<typename _Routable_Intersection_Interface::Component_Type>();
					routable_intersection->template x_position<float>(regular_intersection->template x_position<float>());
					routable_intersection->template y_position<float>(regular_intersection->template y_position<float>());
					routable_intersection->template uuid<int>(regular_intersection->template uuid<int>());
					routable_intersection->template internal_id<int>(regular_intersection->template internal_id<int>());
					
					// create inbound_outbound_movements
					_Regular_Inbound_Outbound_Movements_Container_Interface& regular_inbound_outbound_movements_container = regular_intersection->template inbound_outbound_movements<_Regular_Inbound_Outbound_Movements_Container_Interface&>();
					typename _Regular_Inbound_Outbound_Movements_Container_Interface::iterator regular_inbound_outbound_movements_itr;
					
					for(regular_inbound_outbound_movements_itr=regular_inbound_outbound_movements_container.begin(); regular_inbound_outbound_movements_itr!=regular_inbound_outbound_movements_container.end(); regular_inbound_outbound_movements_itr++)
					{
						_Regular_Inbound_Outbound_Movements_Interface* regular_inbound_outbound_movements = (_Regular_Inbound_Outbound_Movements_Interface*)(*regular_inbound_outbound_movements_itr);
						_Routable_Inbound_Outbound_Movements_Interface* routable_inbound_outbound_movements = (_Routable_Inbound_Outbound_Movements_Interface*)Allocate<typename _Routable_Inbound_Outbound_Movements_Interface::Component_Type>();
						_Regular_Link_Interface* regular_link = regular_inbound_outbound_movements->template inbound_link_reference<_Regular_Link_Interface*>();
						_Routable_Link_Interface* routable_link = linksMap.find(regular_link->template internal_id<int>())->second;
						routable_inbound_outbound_movements->template inbound_link_reference<_Routable_Link_Interface*>(routable_link);
						//another level of loop
						_Regular_Movements_Container_Interface& regular_outbound_movements_container = regular_inbound_outbound_movements->template outbound_movements<_Regular_Movements_Container_Interface&>();
						typename _Regular_Movements_Container_Interface::iterator regular_outbound_movement_itr;
						for(regular_outbound_movement_itr=regular_outbound_movements_container.begin(); regular_outbound_movement_itr!=regular_outbound_movements_container.end(); regular_outbound_movement_itr++)
						{
							_Regular_Movement_Interface* regular_outbound_movement = (_Regular_Movement_Interface*)(*regular_outbound_movement_itr);
							_Routable_Movement_Interface* routable_outbound_movement = (_Routable_Movement_Interface*)Allocate<typename _Routable_Movement_Interface::Component_Type>();
							routable_outbound_movement->template forward_link_turn_travel_time<float>(regular_outbound_movement->template forward_link_turn_travel_time<float>());
							_Regular_Link_Interface* regular_outbound_link = regular_outbound_movement->template outbound_link<_Regular_Link_Interface*>();
							_Routable_Link_Interface* routable_outbound_link = linksMap.find(regular_outbound_link->template internal_id<int>())->second;
							routable_outbound_movement->template outbound_link<_Routable_Link_Interface*>(routable_outbound_link);
							regular_outbound_movement->template replicas_container<_Routable_Movements_Container_Interface&>().push_back(routable_outbound_movement);
							routable_inbound_outbound_movements->template outbound_movements<_Routable_Movements_Container_Interface&>().push_back(routable_outbound_movement);
						}
						routable_intersection->template inbound_outbound_movements<_Routable_Inbound_Outbound_Movements_Container_Interface&>().push_back(routable_inbound_outbound_movements);
					}
					
					// create outbound_inbound_movements
					_Regular_Outbound_Inbound_Movements_Container_Interface& regular_outbound_inbound_movements_container = regular_intersection->template outbound_inbound_movements<_Regular_Outbound_Inbound_Movements_Container_Interface&>();
					typename _Regular_Outbound_Inbound_Movements_Container_Interface::iterator regular_outbound_inbound_movements_itr;
					
					for(regular_outbound_inbound_movements_itr=regular_outbound_inbound_movements_container.begin(); regular_outbound_inbound_movements_itr!=regular_outbound_inbound_movements_container.end(); regular_outbound_inbound_movements_itr++)
					{
						_Regular_Outbound_Inbound_Movements_Interface* regular_outbound_inbound_movements = (_Regular_Outbound_Inbound_Movements_Interface*)(*regular_outbound_inbound_movements_itr);
						_Routable_Outbound_Inbound_Movements_Interface* routable_outbound_inbound_movements = (_Routable_Outbound_Inbound_Movements_Interface*)Allocate<typename _Routable_Outbound_Inbound_Movements_Interface::Component_Type>();
						_Regular_Link_Interface* regular_link = regular_outbound_inbound_movements->template outbound_link_reference<_Regular_Link_Interface*>();
						_Routable_Link_Interface* routable_link = linksMap.find(regular_link->template internal_id<int>())->second;
						routable_outbound_inbound_movements->template outbound_link_reference<_Routable_Link_Interface*>(routable_link);
						//another level of loop
						_Regular_Movements_Container_Interface& regular_inbound_movements_container = regular_outbound_inbound_movements->template inbound_movements<_Regular_Movements_Container_Interface&>();
						typename _Regular_Movements_Container_Interface::iterator regular_inbound_movement_itr;
						for(regular_inbound_movement_itr=regular_inbound_movements_container.begin(); regular_inbound_movement_itr!=regular_inbound_movements_container.end(); regular_inbound_movement_itr++)
						{
							_Regular_Movement_Interface* regular_inbound_movement = (_Regular_Movement_Interface*)(*regular_inbound_movement_itr);
							_Routable_Movement_Interface* routable_inbound_movement = (_Routable_Movement_Interface*)Allocate<typename _Routable_Movement_Interface::Component_Type>();
							routable_inbound_movement->template forward_link_turn_travel_time<float>(regular_inbound_movement->template forward_link_turn_travel_time<float>());
							_Regular_Link_Interface* regular_inbound_link = regular_inbound_movement->template inbound_link<_Regular_Link_Interface*>();
							_Routable_Link_Interface* routable_inbound_link = linksMap.find(regular_inbound_link->template internal_id<int>())->second;
							routable_inbound_movement->template inbound_link<_Routable_Link_Interface*>(routable_inbound_link);
							regular_inbound_movement->template replicas_container<_Routable_Movements_Container_Interface&>().push_back(routable_inbound_movement);
							routable_outbound_inbound_movements->template inbound_movements<_Routable_Movements_Container_Interface&>().push_back(routable_inbound_movement);
						}
						routable_intersection->template outbound_inbound_movements<_Routable_Outbound_Inbound_Movements_Container_Interface&>().push_back(routable_outbound_inbound_movements);
					}

					intersections<_Routable_Intersections_Container_Interface&>().push_back(routable_intersection);
					intersectionsMap.insert(pair<int, _Routable_Intersection_Interface*>(regular_intersection->template internal_id<int>(), routable_intersection));
				}
				cout << " adding up/down streams to each link" << endl;
				// add upstream and downstream to each link
				typename _Routable_Links_Container_Interface::iterator routable_link_itr;
				for(routable_link_itr=links<_Routable_Links_Container_Interface&>().begin(); routable_link_itr!=links<_Routable_Links_Container_Interface&>().end(); routable_link_itr++)
				{

					_Routable_Link_Interface* routable_link = (_Routable_Link_Interface*)(*routable_link_itr);
					_Regular_Link_Interface* regular_link = routable_link->template network_link_reference<_Regular_Link_Interface*>();
		
					_Regular_Intersection_Interface* regular_upstream_intersection = regular_link->template upstream_intersection<_Regular_Intersection_Interface*>();
					_Routable_Intersection_Interface* routable_upstream_intersection = intersectionsMap.find(regular_upstream_intersection->template internal_id<int>())->second;
					routable_link->template upstream_intersection<_Routable_Intersection_Interface*>(routable_upstream_intersection);
					_Regular_Intersection_Interface* regular_downstream_intersection = regular_link->template downstream_intersection<_Regular_Intersection_Interface*>();
					_Routable_Intersection_Interface* routable_downstream_intersection = intersectionsMap.find(regular_downstream_intersection->template internal_id<int>())->second;
					routable_link->template downstream_intersection<_Routable_Intersection_Interface*>(routable_downstream_intersection);
				}
				cout << " streams added" << endl;
				
				// for debug, we printout everything of the network
				/*
				cout <<endl<<"*********************************Routable Network**************************************************"<<endl;
				cout<<"all links"<<endl;
				int i;
				for (i = 0; i < (int)links<RoutableLinksContainerType&>().size(); i++)
				{
					RoutableLinkInterface* link = (RoutableLinkInterface*)links<RoutableLinksContainerType&>()[i];
					cout<<"\t link_"<<link->template uuid<int>()<<endl;
					cout<<"\t\t upstream: " << link->template upstream_intersection<RoutableIntersectionInterface*>()->template uuid<int>()<<endl;
					cout<<"\t\t downstream: " << link->template downstream_intersection<RoutableIntersectionInterface*>()->template uuid<int>()<<endl;
					cout<<"\t\t travel_time: " << link->template travel_time<float>()<<endl;
					cout<<"\t\t -----------------------------------"<<endl<<endl;
				}

				cout <<"-------------------------------------------------------------------------------------"<<endl;
				cout << "all intersections" << endl;
				for (i = 0; i < (int)intersections<RoutableIntersectionsContainerType&>().size(); i++)
				{
					RoutableIntersectionInterface* intersection = (RoutableIntersectionInterface*)intersections<RoutableIntersectionsContainerType>()[i];
					cout<<"\t intersection_"<<intersection->template uuid<int>()<<endl;
					cout<<"\t\t x_position: "<<intersection->template x_position<float>()<<endl;
					cout<<"\t\t y_position: "<<intersection->template y_position<float>()<<endl;
					RoutableInboundOutboundMovementsContainerType& inbound_outbound_movements_container = intersection->template inbound_outbound_movements<RoutableInboundOutboundMovementsContainerType&>();
					typename RoutableInboundOutboundMovementsContainerType::iterator inbound_outbound_movements_itr;
					int j;
					for (j=0, inbound_outbound_movements_itr=inbound_outbound_movements_container.begin();inbound_outbound_movements_itr!=inbound_outbound_movements_container.end();inbound_outbound_movements_itr++,j++)
					{
						RoutableInboundOutboundMovementsInterface* inbound_outbound_movements = (RoutableInboundOutboundMovementsInterface*)(*inbound_outbound_movements_itr);
						cout<<"\t\t inbound_outbound_movements_"<<j<<endl;
						cout<<"\t\t\t inbound_link: "<<inbound_outbound_movements->template inbound_link_reference<RoutableLinkInterface*>()->template uuid<int>()<<endl;
						cout<<"\t\t\t outbound_movements: "<<endl;
						RoutableMovementsContainerType& outbound_movements_container = inbound_outbound_movements->template outbound_movements<RoutableMovementsContainerType&>();
						typename RoutableMovementsContainerType::iterator outbound_movement_itr;
						int k;
						for (k=0,outbound_movement_itr=outbound_movements_container.begin();outbound_movement_itr!=outbound_movements_container.end();outbound_movement_itr++,k++)
						{
							RoutableMovementInterface* outbound_movement = (RoutableMovementInterface*)(*outbound_movement_itr);
							cout<<"\t\t\t\t outbound_movements_"<<k<<endl;
							cout<<"\t\t\t\t\t outbound_link: "<<outbound_movement->template outbound_link<RoutableLinkInterface*>()->template uuid<int>()<<endl;
							cout<<"\t\t\t\t\t forward_link_turn_travel_time: "<<outbound_movement->template forward_link_turn_travel_time<float>()<<endl;
						}
					}

					RoutableOutboundInboundMovementsContainerType& outbound_inbound_movements_container = intersection->template outbound_inbound_movements<RoutableOutboundInboundMovementsContainerType&>();
					typename RoutableOutboundInboundMovementsContainerType::iterator outbound_inbound_movements_itr;

					for (j=0, outbound_inbound_movements_itr=outbound_inbound_movements_container.begin();outbound_inbound_movements_itr!=outbound_inbound_movements_container.end();outbound_inbound_movements_itr++,j++)
					{
						RoutableOutboundInboundMovementsInterface* outbound_inbound_movements = (RoutableOutboundInboundMovementsInterface*)(*outbound_inbound_movements_itr);
						cout<<"\t\t outbound_inbound_movements_"<<j<<endl;
						cout<<"\t\t\t outbound_link: "<<outbound_inbound_movements->template outbound_link_reference<RoutableLinkInterface*>()->template uuid<int>()<<endl;
						cout<<"\t\t\t inbound_movements: "<<endl;
						RoutableMovementsContainerType& inbound_movements_container = outbound_inbound_movements->template inbound_movements<RoutableMovementsContainerType&>();
						typename RoutableMovementsContainerType::iterator outbound_movement_itr;
						int k;
						for (k=0,outbound_movement_itr=inbound_movements_container.begin();outbound_movement_itr!=inbound_movements_container.end();outbound_movement_itr++,k++)
						{
							RoutableMovementInterface* outbound_movement = (RoutableMovementInterface*)(*outbound_movement_itr);
							cout<<"\t\t\t\t inbound_movements_"<<k<<endl;
							cout<<"\t\t\t\t\t inbound_link: "<<outbound_movement->template inbound_link<RoutableLinkInterface*>()->template uuid<int>()<<endl;
							cout<<"\t\t\t\t\t forward_link_turn_travel_time: "<<outbound_movement->template forward_link_turn_travel_time<float>()<<endl;
						}
					}
					cout<<"\t\t --------------------------------"<<endl<<endl;
				}	
				*/
			}

		};
		
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
				if(_sub_iteration == Scenario_Components::Types::Type_Iteration_keys::DEMAND_ITERATION)
				{
					response.result=true;
					response.next._iteration=END;
					response.next._sub_iteration=Scenario_Components::Types::Type_Iteration_keys::DEMAND_ITERATION;
				}
				else
				{
					assert(false);
					cout << "Should never reach here in routing conditional!" << endl;
				}
			}

			feature_prototype bool one_to_one_link_based_least_time_path_a_star()
			{

				define_component_interface(_Routable_Network_Interface, get_type_of(routable_network), Routing_Components::Prototypes::Routable_Network_Prototype, ComponentType);
				define_component_interface(_Regular_Network_Interface, get_type_of(network), Network_Components::Prototypes::Network_Prototype, ComponentType);
				define_container_and_value_interface(_Routable_Links_Container_Interface, _Routable_Link_Interface, _Routable_Network_Interface::get_type_of(links), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
				//typedef Link_Interface<typename ComponentType::routable_link_type, ComponentType> _Routable_Link_Interface;
				define_component_interface(_Routable_Intersection_Interface, ComponentType::routable_link_type::upstream_intersection_type, Intersection_Components::Prototypes::Intersection_Prototype, ComponentType);
				define_container_and_value_interface(_Inbound_Outbound_Movements_Container_Interface, _Inbound_Outbound_Movements_Interface, _Routable_Intersection_Interface::get_type_of(inbound_outbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Inbound_Outbound_Movements_Prototype, ComponentType);
				define_container_and_value_interface(_Movements_Container_Interface, _Movement_Interface, _Inbound_Outbound_Movements_Interface::get_type_of(outbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Movement_Prototype, ComponentType);
				define_container_and_value_interface(_Reversed_Path_Container_Interface, _Regular_Link_Interface, _Routable_Network_Interface::get_type_of(reversed_path_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
				typedef typename _Routable_Network_Interface::get_type_of(scan_list) ScanListType;

				_Routable_Network_Interface* routable_net=routable_network<_Routable_Network_Interface*>();
				routable_net->template Reset<NULLTYPE>();
				
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
				current_link->template scan_list_status<Scan_List_Status_Keys>(INSELIST);
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
					current_link->template scan_list_status<Scan_List_Status_Keys>(SCANNED);

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
									if(next_link->template scan_list_status<Scan_List_Status_Keys>()==INSELIST)
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

									next_link->template scan_list_status<Scan_List_Status_Keys>(INSELIST);
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

			feature_prototype void Schedule_Route_Computation(int departed_time)
			{
				define_component_interface(_Regular_Network_Interface, get_type_of(network), Network_Components::Prototypes::Network_Prototype, ComponentType);
				define_component_interface(_Scenario_Interface, _Regular_Network_Interface::get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
				load_event(ComponentType,Compute_Route_Condition,Compute_Route,departed_time,Scenario_Components::Types::Type_Iteration_keys::DEMAND_ITERATION,NULLTYPE);
			}

			//first event
			declare_feature_event(Compute_Route)
			{
				typedef Routing_Components::Prototypes::Routing_Prototype<ComponentType, ComponentType> _Routing_Interface;
				typedef Link_Components::Prototypes::Link_Prototype<typename ComponentType::routable_link_type, ComponentType> _Routable_Link_Interface;
				_Routing_Interface* _this_ptr=(_Routing_Interface*)_this;
				typedef Routing_Components::Prototypes::Routable_Network_Prototype<typename ComponentType::routable_network_type, ComponentType> _Routable_Network_Interface;
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
					origin_link->p_vehicle(veh);
				}
			}
		};
	}
}

using namespace Routing_Components::Prototypes;