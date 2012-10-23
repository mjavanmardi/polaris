#pragma once
#include "Intersection_Prototype.h"

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

		template<typename ComponentType,typename CallerType>
		struct Routable_Network_Prototype
		{
			feature_accessor(scan_list);
			//feature_accessor(num_searches);
			feature_accessor(max_free_flow_speed);
			feature_accessor(intersections);
			feature_accessor(links);
			feature_accessor(reversed_path_container);

			feature void Reset()
			{
				typedef typename ComponentType::scan_list_type ScanListType;
				scan_list<ScanListType&>().clear();
				
				define_container_and_value_interface_local(Polaris_Random_Access_Sequence_Prototype, reversed_path_container, _Reversed_Path_Container_Interface, Link_Prototype, _Regular_Link_Interface, ComponentType);  
				reversed_path_container<_Reversed_Path_Container_Interface&>().clear();

				define_container_and_value_interface_local(Polaris_Random_Access_Sequence_Prototype, links, _Routable_Links_Container_Interface, Link_Prototype, _Routable_Link_Interface, ComponentType);
				_Routable_Links_Container_Interface& links_container=links<_Routable_Links_Container_Interface&>();
				typename _Routable_Links_Container_Interface::iterator link_itr;

				for(link_itr=links_container.begin();link_itr!=links_container.end();link_itr++)
				{
					_Routable_Link_Interface* link_ptr = (_Routable_Link_Interface*)(*link_itr);
					link_ptr->label_cost<float>(FLT_MAX);
					link_ptr->label_pointer<_Routable_Link_Interface*>(nullptr);
					link_ptr->scan_list_status<Scan_List_Status_Keys>(UNSCANNED);
					link_ptr->f_cost<float>(0);
					link_ptr->h_cost<float>(0);
				}
			}

			feature void read_routable_network_data(void* regular_network)
			{
				define_container_and_value_interface_local(Polaris_Random_Access_Sequence_Prototype, links, _Routable_Links_Container_Interface, Link_Prototype, _Routable_Link_Interface, ComponentType);
				define_container_and_value_interface_local(Polaris_Random_Access_Sequence_Prototype, intersections, _Routable_Intersections_Container_Interface, Intersection_Prototype, _Routable_Intersection_Interface, ComponentType);
				define_container_and_value_interface(Polaris_Random_Access_Sequence_Prototype, _Routable_Intersection_Interface_type::outbound_inbound_movements, _Routable_Outbound_Inbound_Movements_Container_Interface,Outbound_Inbound_Movements_Prototype,_Routable_Outbound_Inbound_Movements_Interface,ComponentType);
				define_container_and_value_interface(Polaris_Random_Access_Sequence_Prototype, _Routable_Intersection_Interface_type::inbound_outbound_movements, _Routable_Inbound_Outbound_Movements_Container_Interface,Inbound_Outbound_Movements_Prototype,_Routable_Inbound_Outbound_Movements_Interface,ComponentType);
				define_container_and_value_interface(Polaris_Random_Access_Sequence_Prototype, _Routable_Outbound_Inbound_Movements_Interface_type::inbound_movements, _Routable_Movements_Container_Interface, Movement_Prototype, _Routable_Movement_Interface, ComponentType);

				typedef Network_Prototype<ComponentType::regular_network_type, ComponentType> _Regular_Network_Interface;
				define_container_and_value_interface(Polaris_Random_Access_Sequence_Prototype, ComponentType::regular_network_type::links_container, _Regular_Links_Container_Interface, Link_Prototype, _Regular_Link_Interface, ComponentType);
				define_container_and_value_interface(Polaris_Random_Access_Sequence_Prototype, ComponentType::regular_network_type::intersections_container, _Regular_Intersections_Container_Interface, Intersection_Prototype, _Regular_Intersection_Interface, ComponentType);
				define_container_and_value_interface(Polaris_Random_Access_Sequence_Prototype, _Regular_Intersection_Interface_type::outbound_inbound_movements, _Regular_Outbound_Inbound_Movements_Container_Interface,Outbound_Inbound_Movements_Prototype,_Regular_Outbound_Inbound_Movements_Interface,ComponentType);
				define_container_and_value_interface(Polaris_Random_Access_Sequence_Prototype, _Regular_Intersection_Interface_type::inbound_outbound_movements, _Regular_Inbound_Outbound_Movements_Container_Interface,Inbound_Outbound_Movements_Prototype,_Regular_Inbound_Outbound_Movements_Interface,ComponentType);
				define_container_and_value_interface(Polaris_Random_Access_Sequence_Prototype, _Regular_Outbound_Inbound_Movements_Interface_type::inbound_movements, _Regular_Movements_Container_Interface, Movement_Prototype, _Regular_Movement_Interface, ComponentType);
				

				max_free_flow_speed<float>(((_Regular_Network_Interface*)regular_network)->max_free_flow_speed<float>());

				
				_Regular_Links_Container_Interface& regular_links_container = ((_Regular_Network_Interface*)regular_network)->links_container<_Regular_Links_Container_Interface&>();
				
				// copy all links
				hash_map<_Regular_Link_Interface*, _Routable_Link_Interface*> linksMap;
				_Regular_Links_Container_Interface::iterator regular_link_itr;
				for(regular_link_itr=regular_links_container.begin(); regular_link_itr!=regular_links_container.end(); regular_link_itr++)
				{
					_Regular_Link_Interface* regular_link = (_Regular_Link_Interface*)(*regular_link_itr);
					_Routable_Link_Interface* routable_link =  (_Routable_Link_Interface*)Allocate<_Routable_Link_Interface_type>();
					routable_link->network_link_reference<_Regular_Link_Interface*>(regular_link);
					routable_link->f_cost<float>(0);
					routable_link->h_cost<float>(0);
					routable_link->label_pointer<void*>(nullptr);
					routable_link->label_cost<float>(0);
					routable_link->uuid<int>(regular_link->uuid<int>());
					routable_link->travel_time(regular_link->travel_time<float>());
					links<_Routable_Links_Container_Interface&>().push_back(routable_link);
					linksMap.insert(pair<_Regular_Link_Interface*, _Routable_Link_Interface*>(regular_link, routable_link));
					regular_link->replicas_container<_Routable_Links_Container_Interface&>().push_back(routable_link);
				}

				// copy all intersections
				_Regular_Intersections_Container_Interface& regular_intersections_container = ((_Regular_Network_Interface*)regular_network)->intersections_container<_Regular_Intersections_Container_Interface&>();
				hash_map<_Regular_Intersection_Interface*, _Routable_Intersection_Interface*> intersectionsMap;
				_Regular_Intersections_Container_Interface::iterator regular_intersection_itr;
				for(regular_intersection_itr=regular_intersections_container.begin(); regular_intersection_itr!=regular_intersections_container.end(); regular_intersection_itr++)
				{
					_Regular_Intersection_Interface* regular_intersection = (_Regular_Intersection_Interface*)(*regular_intersection_itr);
					_Routable_Intersection_Interface* routable_intersection =  (_Routable_Intersection_Interface*)Allocate<_Routable_Intersection_Interface_type>();
					routable_intersection->x_position<float>(regular_intersection->x_position<float>());
					routable_intersection->y_position<float>(regular_intersection->y_position<float>());
					routable_intersection->uuid<int>(regular_intersection->uuid<int>());

					// now add movements for the intersection
					//RegularOutboundInboundMovementsContainerType& regular_outbound_inbound_movements_container = regular_intersection->inbound_outbound_movements<RegularOutboundInboundMovementsContainerType&>();
					//RegularOutboundInboundMovementsContainerType::iterator regular_outbound_inbound_movements_itr;
					//for(regular_outbound_inbound_movements_itr=regular_outbound_inbound_movements_container.begin(); regular_outbound_inbound_movements_itr!=regular_outbound_inbound_movements_container.end(); regular_outbound_inbound_movements_itr++)
					//{
					//	RegularOutboundInboundMovementsInterface* regular_outbound_inbound_movements = (RegularOutboundInboundMovementsInterface*)(*regular_outbound_inbound_movements_itr);
					//	RoutableOutboundInboundMovementsInterface* routable_outbound_inbound_movements = (RoutableOutboundInboundMovementsInterface*)Allocate<RoutableOutboundInboundMovementsType>();
					//	//float forward_link_turn_travel_time = regular_outbound_inbound_movements->forward_link_turn_travel_time<float>();
					//	//routable_outbound_inbound_movements->forward_link_turn_travel_time<float>(forward_link_turn_travel_time);
					//	RegularLinkInterface* regular_link = NULL;//regular_outbound_inbound_movements->outbound_movement_reference<RegularLinkInterface*>();
					//	RoutableLinkInterface* routable_link = linksMap.find(regular_link)->second;
					//	routable_outbound_inbound_movements->outbound_movement_reference<RoutableLinkInterface*>(routable_link);
					//	routable_intersection->outbound_inbound_movements<RoutableOutboundInboundMovementsContainerType&>().push_back(routable_outbound_inbound_movements);
					//}
					
					// create inbound_outbound_movements
					_Regular_Inbound_Outbound_Movements_Container_Interface& regular_inbound_outbound_movements_container = regular_intersection->inbound_outbound_movements<_Regular_Inbound_Outbound_Movements_Container_Interface&>();
					_Regular_Inbound_Outbound_Movements_Container_Interface::iterator regular_inbound_outbound_movements_itr;
					
					for(regular_inbound_outbound_movements_itr=regular_inbound_outbound_movements_container.begin(); regular_inbound_outbound_movements_itr!=regular_inbound_outbound_movements_container.end(); regular_inbound_outbound_movements_itr++)
					{
						_Regular_Inbound_Outbound_Movements_Interface* regular_inbound_outbound_movements = (_Regular_Inbound_Outbound_Movements_Interface*)(*regular_inbound_outbound_movements_itr);
						_Routable_Inbound_Outbound_Movements_Interface* routable_inbound_outbound_movements = (_Routable_Inbound_Outbound_Movements_Interface*)Allocate<_Routable_Inbound_Outbound_Movements_Interface_type>();
						//float forward_link_turn_travel_time = regular_outbound_inbound_movements->forward_link_turn_travel_time<float>();
						//routable_outbound_inbound_movements->forward_link_turn_travel_time<float>(forward_link_turn_travel_time);
						_Regular_Link_Interface* regular_link = regular_inbound_outbound_movements->inbound_link_reference<_Regular_Link_Interface*>();
						_Routable_Link_Interface* routable_link = linksMap.find(regular_link)->second;
						routable_inbound_outbound_movements->inbound_link_reference<_Routable_Link_Interface*>(routable_link);
						//another level of loop
						_Regular_Movements_Container_Interface& regular_outbound_movements_container = regular_inbound_outbound_movements->outbound_movements<_Regular_Movements_Container_Interface&>();
						_Regular_Movements_Container_Interface::iterator regular_outbound_movement_itr;
						for(regular_outbound_movement_itr=regular_outbound_movements_container.begin(); regular_outbound_movement_itr!=regular_outbound_movements_container.end(); regular_outbound_movement_itr++)
						{
							_Regular_Movement_Interface* regular_outbound_movement = (_Regular_Movement_Interface*)(*regular_outbound_movement_itr);
							_Routable_Movement_Interface* routable_outbound_movement = (_Routable_Movement_Interface*)Allocate<_Routable_Movement_Interface_type>();
							routable_outbound_movement->forward_link_turn_travel_time<float>(regular_outbound_movement->forward_link_turn_travel_time<float>());
							_Regular_Link_Interface* regular_outbound_link = regular_outbound_movement->outbound_link<_Regular_Link_Interface*>();
							_Routable_Link_Interface* routable_outbound_link = linksMap.find(regular_outbound_link)->second;
							routable_outbound_movement->outbound_link<_Routable_Link_Interface*>(routable_outbound_link);
							regular_outbound_movement->replicas_container<_Routable_Movements_Container_Interface&>().push_back(routable_outbound_movement);
							routable_inbound_outbound_movements->outbound_movements<_Routable_Movements_Container_Interface&>().push_back(routable_outbound_movement);
						}
						routable_intersection->inbound_outbound_movements<_Routable_Inbound_Outbound_Movements_Container_Interface&>().push_back(routable_inbound_outbound_movements);
					}
					
					// create outbound_inbound_movements
					_Regular_Outbound_Inbound_Movements_Container_Interface& regular_outbound_inbound_movements_container = regular_intersection->outbound_inbound_movements<_Regular_Outbound_Inbound_Movements_Container_Interface&>();
					_Regular_Outbound_Inbound_Movements_Container_Interface::iterator regular_outbound_inbound_movements_itr;
					
					for(regular_outbound_inbound_movements_itr=regular_outbound_inbound_movements_container.begin(); regular_outbound_inbound_movements_itr!=regular_outbound_inbound_movements_container.end(); regular_outbound_inbound_movements_itr++)
					{
						_Regular_Outbound_Inbound_Movements_Interface* regular_outbound_inbound_movements = (_Regular_Outbound_Inbound_Movements_Interface*)(*regular_outbound_inbound_movements_itr);
						_Routable_Outbound_Inbound_Movements_Interface* routable_outbound_inbound_movements = (_Routable_Outbound_Inbound_Movements_Interface*)Allocate<_Routable_Outbound_Inbound_Movements_Interface_type>();
						//float forward_link_turn_travel_time = regular_outbound_inbound_movements->forward_link_turn_travel_time<float>();
						//routable_outbound_inbound_movements->forward_link_turn_travel_time<float>(forward_link_turn_travel_time);
						_Regular_Link_Interface* regular_link = regular_outbound_inbound_movements->outbound_link_reference<_Regular_Link_Interface*>();
						_Routable_Link_Interface* routable_link = linksMap.find(regular_link)->second;
						routable_outbound_inbound_movements->outbound_link_reference<_Routable_Link_Interface*>(routable_link);
						//another level of loop
						_Regular_Movements_Container_Interface& regular_inbound_movements_container = regular_outbound_inbound_movements->inbound_movements<_Regular_Movements_Container_Interface&>();
						_Regular_Movements_Container_Interface::iterator regular_inbound_movement_itr;
						for(regular_inbound_movement_itr=regular_inbound_movements_container.begin(); regular_inbound_movement_itr!=regular_inbound_movements_container.end(); regular_inbound_movement_itr++)
						{
							_Regular_Movement_Interface* regular_inbound_movement = (_Regular_Movement_Interface*)(*regular_inbound_movement_itr);
							_Routable_Movement_Interface* routable_inbound_movement = (_Routable_Movement_Interface*)Allocate<_Routable_Movement_Interface_type>();
							routable_inbound_movement->forward_link_turn_travel_time<float>(regular_inbound_movement->forward_link_turn_travel_time<float>());
							_Regular_Link_Interface* regular_inbound_link = regular_inbound_movement->inbound_link<_Regular_Link_Interface*>();
							_Routable_Link_Interface* routable_inbound_link = linksMap.find(regular_inbound_link)->second;
							routable_inbound_movement->inbound_link<_Routable_Link_Interface*>(routable_inbound_link);
							regular_inbound_movement->replicas_container<_Routable_Movements_Container_Interface&>().push_back(routable_inbound_movement);
							routable_outbound_inbound_movements->inbound_movements<_Routable_Movements_Container_Interface&>().push_back(routable_inbound_movement);
						}
						routable_intersection->outbound_inbound_movements<_Routable_Outbound_Inbound_Movements_Container_Interface&>().push_back(routable_outbound_inbound_movements);
					}

					intersections<_Routable_Intersections_Container_Interface&>().push_back(routable_intersection);
					intersectionsMap.insert(pair<_Regular_Intersection_Interface*, _Routable_Intersection_Interface*>(regular_intersection, routable_intersection));
				}

				// add upstream and downstream to each link
				_Routable_Links_Container_Interface::iterator routable_link_itr;
				for(routable_link_itr=links<_Routable_Links_Container_Interface&>().begin(); routable_link_itr!=links<_Routable_Links_Container_Interface&>().end(); routable_link_itr++)
				{

					_Routable_Link_Interface* routable_link = (_Routable_Link_Interface*)(*routable_link_itr);
					_Regular_Link_Interface* regular_link = routable_link->network_link_reference<_Regular_Link_Interface*>();
if (routable_link->uuid<int>() == 3) {
cout << "stop here" << endl;
}					
					_Regular_Intersection_Interface* regular_upstream_intersection = regular_link->upstream_intersection<_Regular_Intersection_Interface*>();
					_Routable_Intersection_Interface* routable_upstream_intersection = intersectionsMap.find(regular_upstream_intersection)->second;
					routable_link->upstream_intersection<_Routable_Intersection_Interface*>(routable_upstream_intersection);
int intersectionId = routable_upstream_intersection->uuid<int>();
					_Regular_Intersection_Interface* regular_downstream_intersection = regular_link->downstream_intersection<_Regular_Intersection_Interface*>();
					_Routable_Intersection_Interface* routable_downstream_intersection = intersectionsMap.find(regular_downstream_intersection)->second;
					routable_link->downstream_intersection<_Routable_Intersection_Interface*>(routable_downstream_intersection);
				}
				
				// for debug, we printout everything of the network
				/*
				cout <<endl<<"*********************************Routable Network**************************************************"<<endl;
				cout<<"all links"<<endl;
				int i;
				for (i = 0; i < (int)links<RoutableLinksContainerType&>().size(); i++)
				{
					RoutableLinkInterface* link = (RoutableLinkInterface*)links<RoutableLinksContainerType&>()[i];
					cout<<"\t link_"<<link->uuid<int>()<<endl;
					cout<<"\t\t upstream: " << link->upstream_intersection<RoutableIntersectionInterface*>()->uuid<int>()<<endl;
					cout<<"\t\t downstream: " << link->downstream_intersection<RoutableIntersectionInterface*>()->uuid<int>()<<endl;
					cout<<"\t\t travel_time: " << link->travel_time<float>()<<endl;
					cout<<"\t\t -----------------------------------"<<endl<<endl;
				}

				cout <<"-------------------------------------------------------------------------------------"<<endl;
				cout << "all intersections" << endl;
				for (i = 0; i < (int)intersections<RoutableIntersectionsContainerType&>().size(); i++)
				{
					RoutableIntersectionInterface* intersection = (RoutableIntersectionInterface*)intersections<RoutableIntersectionsContainerType>()[i];
					cout<<"\t intersection_"<<intersection->uuid<int>()<<endl;
					cout<<"\t\t x_position: "<<intersection->x_position<float>()<<endl;
					cout<<"\t\t y_position: "<<intersection->y_position<float>()<<endl;
					RoutableInboundOutboundMovementsContainerType& inbound_outbound_movements_container = intersection->inbound_outbound_movements<RoutableInboundOutboundMovementsContainerType&>();
					RoutableInboundOutboundMovementsContainerType::iterator inbound_outbound_movements_itr;
					int j;
					for (j=0, inbound_outbound_movements_itr=inbound_outbound_movements_container.begin();inbound_outbound_movements_itr!=inbound_outbound_movements_container.end();inbound_outbound_movements_itr++,j++)
					{
						RoutableInboundOutboundMovementsInterface* inbound_outbound_movements = (RoutableInboundOutboundMovementsInterface*)(*inbound_outbound_movements_itr);
						cout<<"\t\t inbound_outbound_movements_"<<j<<endl;
						cout<<"\t\t\t inbound_link: "<<inbound_outbound_movements->inbound_link_reference<RoutableLinkInterface*>()->uuid<int>()<<endl;
						cout<<"\t\t\t outbound_movements: "<<endl;
						RoutableMovementsContainerType& outbound_movements_container = inbound_outbound_movements->outbound_movements<RoutableMovementsContainerType&>();
						RoutableMovementsContainerType::iterator outbound_movement_itr;
						int k;
						for (k=0,outbound_movement_itr=outbound_movements_container.begin();outbound_movement_itr!=outbound_movements_container.end();outbound_movement_itr++,k++)
						{
							RoutableMovementInterface* outbound_movement = (RoutableMovementInterface*)(*outbound_movement_itr);
							cout<<"\t\t\t\t outbound_movements_"<<k<<endl;
							cout<<"\t\t\t\t\t outbound_link: "<<outbound_movement->outbound_link<RoutableLinkInterface*>()->uuid<int>()<<endl;
							cout<<"\t\t\t\t\t forward_link_turn_travel_time: "<<outbound_movement->forward_link_turn_travel_time<float>()<<endl;
						}
					}

					RoutableOutboundInboundMovementsContainerType& outbound_inbound_movements_container = intersection->outbound_inbound_movements<RoutableOutboundInboundMovementsContainerType&>();
					RoutableOutboundInboundMovementsContainerType::iterator outbound_inbound_movements_itr;

					for (j=0, outbound_inbound_movements_itr=outbound_inbound_movements_container.begin();outbound_inbound_movements_itr!=outbound_inbound_movements_container.end();outbound_inbound_movements_itr++,j++)
					{
						RoutableOutboundInboundMovementsInterface* outbound_inbound_movements = (RoutableOutboundInboundMovementsInterface*)(*outbound_inbound_movements_itr);
						cout<<"\t\t outbound_inbound_movements_"<<j<<endl;
						cout<<"\t\t\t outbound_link: "<<outbound_inbound_movements->outbound_link_reference<RoutableLinkInterface*>()->uuid<int>()<<endl;
						cout<<"\t\t\t inbound_movements: "<<endl;
						RoutableMovementsContainerType& inbound_movements_container = outbound_inbound_movements->inbound_movements<RoutableMovementsContainerType&>();
						RoutableMovementsContainerType::iterator outbound_movement_itr;
						int k;
						for (k=0,outbound_movement_itr=inbound_movements_container.begin();outbound_movement_itr!=inbound_movements_container.end();outbound_movement_itr++,k++)
						{
							RoutableMovementInterface* outbound_movement = (RoutableMovementInterface*)(*outbound_movement_itr);
							cout<<"\t\t\t\t inbound_movements_"<<k<<endl;
							cout<<"\t\t\t\t\t inbound_link: "<<outbound_movement->inbound_link<RoutableLinkInterface*>()->uuid<int>()<<endl;
							cout<<"\t\t\t\t\t forward_link_turn_travel_time: "<<outbound_movement->forward_link_turn_travel_time<float>()<<endl;
						}
					}
					cout<<"\t\t --------------------------------"<<endl<<endl;
				}	
				*/
			}

		};
		
		template<typename ComponentType,typename CallerType>
		struct Routing_Prototype
		{
			feature_accessor(vehicle);
			feature_accessor(network);
			feature_accessor(traveler);
			feature_accessor(routable_network);
			feature_accessor(origin_link);
			feature_accessor(destination_link);

			declare_feature_conditional(Compute_Route_Condition)
			{
				typedef Routing_Prototype<ComponentType, ComponentType> _Routing_Interface;
				define_component_interface_local(_Traveler_Interface, Traveler_Prototype, traveler, ComponentType);
				_Routing_Interface* _this_ptr=(_Routing_Interface*)_this;
				int uuid=_this_ptr->traveler<_Traveler_Interface*>()->uuid<int>();

				//PRINT("\n" << iteration << "." << sub_iteration << ":\t" << "visiting traveler: " << uuid);

				response.next=INT_MAX;
				response.result=true;
			}

			feature void one_to_one_link_based_least_time_path_a_star()
			{

				define_component_interface_local(_Routable_Network_Interface, Routable_Network_Prototype, routable_network, ComponentType);
				define_component_interface_local(_Regular_Network_Interface, Network_Prototype, network, ComponentType);
				define_container_and_value_interface(Polaris_Random_Access_Sequence_Prototype, _Routable_Network_Interface_type::links, _Routable_Links_Container_Interface, Link_Prototype, _Routable_Link_Interface, ComponentType);

				//typedef Link_Interface<ComponentType::routable_link_type, ComponentType> _Routable_Link_Interface;
				define_component_interface(_Routable_Intersection_Interface, Intersection_Prototype, ComponentType::routable_link_type::upstream_intersection, ComponentType);
				define_container_and_value_interface(Polaris_Random_Access_Sequence_Prototype, _Routable_Intersection_Interface_type::inbound_outbound_movements, _Inbound_Outbound_Movements_Container_Interface, Inbound_Outbound_Movements_Prototype, _Inbound_Outbound_Movements_Interface, ComponentType);
				define_container_and_value_interface(Polaris_Random_Access_Sequence_Prototype, _Inbound_Outbound_Movements_Interface_type::outbound_movements, _Movements_Container_Interface, Movement_Prototype, _Movement_Interface, ComponentType);
				define_container_and_value_interface(Polaris_Random_Access_Sequence_Prototype, _Routable_Network_Interface_type::reversed_path_container, _Reversed_Path_Container_Interface, Link_Prototype, _Regular_Link_Interface, ComponentType);
				typedef typename _Routable_Network_Interface_type::scan_list_type ScanListType;

				_Routable_Network_Interface* routable_net=routable_network<_Routable_Network_Interface*>();
				routable_net->Reset<NULLTYPE>();
				
				//RoutableNetworkInterface* routable_network=routable_network<RoutableNetworkInterface*>();
				float max_free_flow_speed=routable_net->max_free_flow_speed<float>();
				ScanListType& scan_list=routable_net->scan_list<ScanListType&>();
				//NumSearchesType& num_searches=routable_network->num_searches<NumSearchesType&>();

				_Routable_Link_Interface* origin_link_ptr=origin_link<_Routable_Link_Interface*>();
				_Routable_Link_Interface* destination_link_ptr=destination_link<_Routable_Link_Interface*>();
				_Regular_Link_Interface* destination_reference=destination_link_ptr->network_link_reference<_Regular_Link_Interface*>();
				
				_Regular_Link_Interface* net_origin_link=origin_link_ptr->network_link_reference<_Regular_Link_Interface*>();
				_Movements_Container_Interface& turn_mvmt_container=net_origin_link->outbound_turn_movements<_Movements_Container_Interface&>();
				int outbound_turn_movement_size = (int)turn_mvmt_container.size();
				if (outbound_turn_movement_size == 0){return;}
			
				//initialization
				//RoutableLinkInterface* nextlink;
				//int icurlink;
				//int iturn_movement;
				float next_cost,new_cost;
				float dx,dy;
				float destination_x,destination_y;

				_Routable_Intersection_Interface* destination_upstream_intersection=destination_link_ptr->upstream_intersection<_Routable_Intersection_Interface*>();

				destination_x = destination_upstream_intersection->x_position<float>();
				destination_y = destination_upstream_intersection->y_position<float>();
				
				next_cost=origin_link_ptr->network_link_reference<_Regular_Link_Interface*>()->travel_time<float>();
				new_cost = next_cost;
				_Routable_Link_Interface* current_link=origin_link_ptr;
				
				current_link->label_cost<float>(new_cost); // g - label_cost

				_Routable_Intersection_Interface* current_intersection;

				if(current_link->network_link_reference<_Regular_Link_Interface*>() != destination_reference)
				{
					current_intersection=current_link->upstream_intersection<_Routable_Intersection_Interface*>();
					dx = destination_x - current_intersection->x_position<float>();
					dy = destination_y - current_intersection->y_position<float>();
					current_link->h_cost<float>(sqrt(dx*dx+dy*dy)/max_free_flow_speed); // h - estimated = sqrt(dx*dx+dy*dy)/max_free_flow_speed 
					current_link->f_cost<float>(current_link->label_cost<float>()+current_link->h_cost<float>()); // f = g + h;
				}
				else
				{
					return;
				}

				current_link->label_pointer<_Regular_Link_Interface*>(nullptr);

				scan_list.insert(make_pair(current_link->f_cost<float>(),current_link));
				current_link->scan_list_status<Scan_List_Status_Keys>(INSELIST);
				int cur_link_id;

				while(!scan_list.empty())
				{
					//selection
					//num_searches++;
					current_link = (_Routable_Link_Interface*)(scan_list.begin()->second);
					cur_link_id=current_link->network_link_reference<_Regular_Link_Interface*>()->uuid<int>();
					if(current_link == destination_link_ptr)
					{
						break;
					}

					scan_list.erase(scan_list.begin());	// no need to find minimum since set is ordered by its first element, i.e. label cost.
					current_link->scan_list_status<Scan_List_Status_Keys>(SCANNED);

					current_intersection=current_link->downstream_intersection<_Routable_Intersection_Interface*>();

					
					//for all outbound turn movements
					_Inbound_Outbound_Movements_Container_Interface& inbound_outbound_movements_container = current_intersection->inbound_outbound_movements<_Inbound_Outbound_Movements_Container_Interface&>();
					_Inbound_Outbound_Movements_Container_Interface::iterator inbound_itr;
					for(inbound_itr=inbound_outbound_movements_container.begin(); inbound_itr!=inbound_outbound_movements_container.end(); inbound_itr++)
					{
						_Inbound_Outbound_Movements_Interface* inbound_outbound_movements = (_Inbound_Outbound_Movements_Interface*)(*inbound_itr);
						_Routable_Link_Interface* inbound_link = inbound_outbound_movements->inbound_link_reference<_Routable_Link_Interface*>();
						if (inbound_link == current_link)
						{
							_Movements_Container_Interface& outbound_movements_container = inbound_outbound_movements->outbound_movements<_Movements_Container_Interface&>();
							_Movements_Container_Interface::iterator outbound_itr;
							for(outbound_itr=outbound_movements_container.begin(); outbound_itr!=outbound_movements_container.end(); outbound_itr++)
							{
								_Movement_Interface* outbound_movement = (_Movement_Interface*)(*outbound_itr);
								_Routable_Link_Interface* next_link=outbound_movement->outbound_link<_Routable_Link_Interface*>();
								next_cost=outbound_movement->forward_link_turn_travel_time<float>();
								new_cost=current_link->label_cost<float>();

								if(next_link->label_cost<float>()>new_cost)
								{
									if(next_link->scan_list_status<Scan_List_Status_Keys>()==INSELIST)
									{
										scan_list.erase(make_pair(next_link->f_cost<float>(),next_link)); // delete the old cost
									}

									next_link->label_cost<float>(new_cost);
									next_link->label_pointer<_Routable_Link_Interface*>(current_link);

									if(next_link->network_link_reference<_Regular_Link_Interface*>()!=destination_reference)
									{
										dx=destination_x - next_link->downstream_intersection<_Routable_Intersection_Interface*>()->x_position<float>();
										dy=destination_y - next_link->downstream_intersection<_Routable_Intersection_Interface*>()->y_position<float>();
										next_link->h_cost<float>(sqrt(dx*dx+dy*dy)/max_free_flow_speed);
									}
									else
									{
										next_link->h_cost<float>(0);
									}

									next_link->f_cost<float>(next_link->label_cost<float>() + next_link->h_cost<float>());

									scan_list.insert(make_pair(next_link->f_cost<float>(),next_link)); // update with the new cost
									next_link->scan_list_status<Scan_List_Status_Keys>(INSELIST);
								}
							}
							break;
						}
					}
				}


				_Reversed_Path_Container_Interface& reversed_path_container=routable_network<_Routable_Network_Interface*>()->reversed_path_container<_Reversed_Path_Container_Interface&>();

				current_link=destination_link<_Routable_Link_Interface*>();
				//_Regular_Link_Interface* a = current_link->network_link_reference<_Regular_Link_Interface*>();
				reversed_path_container.push_back(current_link->network_link_reference<_Regular_Link_Interface*>());
				current_link=current_link->label_pointer<_Routable_Link_Interface*>();
					
				while(current_link != nullptr)
				{
					reversed_path_container.push_back(current_link->network_link_reference<_Regular_Link_Interface*>());
					current_link=current_link->label_pointer<_Routable_Link_Interface*>();
				}
			};

			feature void Schedule_Route_Computation(int departed_time)
			{
				schedule_event_local(ComponentType,Compute_Route_Condition,Compute_Route,departed_time,NULLTYPE);
			}

			//first event
			declare_feature_event(Compute_Route)
			{
				typedef Routing_Prototype<ComponentType, ComponentType> _Routing_Interface;
				typedef Link_Prototype<ComponentType::routable_link_type, ComponentType> _Routable_Link_Interface;

				_Routing_Interface* _this_ptr=(_Routing_Interface*)_this;
				typedef Vehicle_Prototype<ComponentType::vehicle_type, ComponentType> _Vehicle_Interface;
				typedef Routable_Network_Prototype<ComponentType::routable_network_type, ComponentType> _Routable_Network_Interface;
				
				define_container_and_value_interface(Polaris_Random_Access_Sequence_Prototype, ComponentType::routable_network_type::reversed_path_container, _Reversed_Path_Container_Interface, Link_Prototype, _Regular_Link_Interface, ComponentType);
				
				_Vehicle_Interface* veh=_this_ptr->vehicle<_Vehicle_Interface*>();
				

				_Regular_Link_Interface* origin_link=veh->origin_link<_Regular_Link_Interface*>();
				_Regular_Link_Interface* destination_link=veh->destination_link<_Regular_Link_Interface*>();
				
				_this_ptr->origin_link<_Regular_Link_Interface*>(origin_link);
				_this_ptr->destination_link<_Regular_Link_Interface*>(destination_link);

				_this_ptr->one_to_one_link_based_least_time_path_a_star<NULLTYPE>();
				
	
				_Routable_Network_Interface* routable_network_ptr=_this_ptr->routable_network<_Routable_Network_Interface*>();
				
				veh->set_route_links(routable_network_ptr->reversed_path_container<_Reversed_Path_Container_Interface&>());
				origin_link->p_vehicle(veh);
			}
		};
	}
}

using namespace Routing_Components::Prototypes;