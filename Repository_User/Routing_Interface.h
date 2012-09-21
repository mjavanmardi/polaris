#pragma once
#include "Intersection_Interface.h"

namespace Routing_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Interfaces
	{
		enum Scan_List_Status_Keys
		{
				UNSCANNED=0,
				SCANNED,
				INSELIST,
		};


		template<typename ThisType,typename CallerType>
		struct Routable_Network_Interface
		{
			facet_accessor(scan_list);
			//facet_accessor(num_searches);
			facet_accessor(max_free_flow_speed);
			facet_accessor(intersections);
			facet_accessor(links);
			facet_accessor(reversed_path_container);

			facet void Reset()
			{
				typedef typename ThisType::scan_list_type ScanListType;
				scan_list<ScanListType&>().clear();
				
				typedef typename ThisType::reversed_path_container_type ReversedPathContainerType;
				reversed_path_container<ReversedPathContainerType&>().clear();

				typedef typename ThisType::links_container_type RoutableLinksType;
				typedef typename ThisType::link_type RoutableLinkType;
				typedef Link_Components::Interfaces::Link_Interface<RoutableLinkType,ThisType> RoutableLinkInterface;
				
				RoutableLinksType& links_container=links<RoutableLinksType&>();
				typename RoutableLinksType::iterator link_itr;

				for(link_itr=links_container.begin();link_itr!=links_container.end();link_itr++)
				{
					RoutableLinkInterface* link_ptr = (RoutableLinkInterface*)(*link_itr);
					link_ptr->label_cost<float>(FLT_MAX);
					link_ptr->label_pointer<RoutableLinkInterface*>(nullptr);
					link_ptr->scan_list_status<Scan_List_Status_Keys>(UNSCANNED);
					link_ptr->f_cost<float>(0);
					link_ptr->h_cost<float>(0);
				}
			}

			facet void read_routable_network_data(void* regular_network)
			{
				typedef typename ThisType::regular_network_type RegularNetworkType;
				typedef typename Network_Components::Interfaces::Network_Interface<RegularNetworkType, ThisType> RegularNetworkInterface;
				
				typedef typename ThisType::regular_links_container_type RegularLinksContainerType;
				typedef typename ThisType::regular_links_container_element_type RegularLinkType;
				typedef typename Link_Components::Interfaces::Link_Interface<RegularLinkType, ThisType> RegularLinkInterface;

				typedef typename ThisType::regular_intersections_container_type RegularIntersectionsContainerType;
				typedef typename ThisType::regular_intersections_container_element_type RegularIntersectionType;
				typedef typename Intersection_Components::Interfaces::Intersection_Interface<RegularIntersectionType, ThisType> RegularIntersectionInterface;

				typedef typename ThisType::routable_links_container_element_type RoutableLinkType;
				typedef typename ThisType::routable_links_container_type RoutableLinksContainerType;
				typedef typename Link_Components::Interfaces::Link_Interface<RoutableLinkType, ThisType> RoutableLinkInterface;

				typedef typename ThisType::routable_intersections_container_element_type RoutableIntersectionType;
				typedef typename ThisType::routable_intersections_container_type RoutableIntersectionsContainerType;
				typedef typename Intersection_Components::Interfaces::Intersection_Interface<RoutableIntersectionType, ThisType> RoutableIntersectionInterface;

				typedef typename ThisType::regular_outbound_inbound_movements_container_type RegularOutboundInboundMovementsContainerType;
				typedef typename ThisType::routable_outbound_inbound_movements_container_type RoutableOutboundInboundMovementsContainerType;
				typedef typename ThisType::regular_outbound_inbound_movements_container_element_type RegularOutboundInboundMovementsType;
				typedef typename ThisType::routable_outbound_inbound_movements_container_element_type RoutableOutboundInboundMovementsType;
				typedef typename Intersection_Components::Interfaces::Outbound_Inbound_Movements_Interface<RegularOutboundInboundMovementsType, ThisType> RegularOutboundInboundMovementsInterface;
				typedef typename Intersection_Components::Interfaces::Outbound_Inbound_Movements_Interface<RoutableOutboundInboundMovementsType, ThisType> RoutableOutboundInboundMovementsInterface;

				typedef typename ThisType::regular_inbound_outbound_movements_container_type RegularInboundOutboundMovementsContainerType;
				typedef typename ThisType::routable_inbound_outbound_movements_container_type RoutableInboundOutboundMovementsContainerType;
				typedef typename ThisType::regular_inbound_outbound_movements_container_element_type RegularInboundOutboundMovementsType;
				typedef typename ThisType::routable_inbound_outbound_movements_container_element_type RoutableInboundOutboundMovementsType;
				typedef typename Intersection_Components::Interfaces::Inbound_Outbound_Movements_Interface<RegularInboundOutboundMovementsType, ThisType> RegularInboundOutboundMovementsInterface;
				typedef typename Intersection_Components::Interfaces::Inbound_Outbound_Movements_Interface<RoutableInboundOutboundMovementsType, ThisType> RoutableInboundOutboundMovementsInterface;

				typedef typename ThisType::regular_movements_container_type RegularMovementsContainerType;
				typedef typename ThisType::regular_movements_container_element_type RegularMovementType;
				typedef typename Intersection_Components::Interfaces::Movement_Interface<RegularMovementType, ThisType> RegularMovementInterface;

				typedef typename ThisType::routable_movements_container_type RoutableMovementsContainerType;
				typedef typename ThisType::routable_movements_container_element_type RoutableMovementType;
				typedef typename Intersection_Components::Interfaces::Movement_Interface<RoutableMovementType, ThisType> RoutableMovementInterface;


				
				max_free_flow_speed<float>(((RegularNetworkInterface*)regular_network)->max_free_flow_speed<float>());

				RegularLinksContainerType& regular_links_container = ((RegularNetworkInterface*)regular_network)->links_container<RegularLinksContainerType&>();
				
				// copy all links
				hash_map<RegularLinkInterface*, RoutableLinkInterface*> linksMap;
				RegularLinksContainerType::iterator regular_link_itr;
				for(regular_link_itr=regular_links_container.begin(); regular_link_itr!=regular_links_container.end(); regular_link_itr++)
				{
					RegularLinkInterface* regular_link = (RegularLinkInterface*)(*regular_link_itr);
					RoutableLinkInterface* routable_link =  (RoutableLinkInterface*)Allocate<RoutableLinkType>();
					routable_link->network_link_reference<RegularLinkInterface*>(regular_link);
					routable_link->f_cost<float>(0);
					routable_link->h_cost<float>(0);
					routable_link->label_pointer<void*>(nullptr);
					routable_link->label_cost<float>(0);
					routable_link->uuid<int>(regular_link->uuid<int>());
					routable_link->travel_time(regular_link->travel_time<float>());
					links<RoutableIntersectionsContainerType&>().push_back(routable_link);
					linksMap.insert(pair<RegularLinkInterface*, RoutableLinkInterface*>(regular_link, routable_link));
					regular_link->replicas_container<RoutableLinksContainerType&>().push_back(routable_link);
				}

				// copy all intersections
				RegularIntersectionsContainerType& regular_intersections_container = ((RegularNetworkInterface*)regular_network)->intersections_container<RegularIntersectionsContainerType&>();
				hash_map<RegularIntersectionInterface*, RoutableIntersectionInterface*> intersectionsMap;
				RegularIntersectionsContainerType::iterator regular_intersection_itr;
				for(regular_intersection_itr=regular_intersections_container.begin(); regular_intersection_itr!=regular_intersections_container.end(); regular_intersection_itr++)
				{
					RegularIntersectionInterface* regular_intersection = (RegularIntersectionInterface*)(*regular_intersection_itr);
					RoutableIntersectionInterface* routable_intersection =  (RoutableIntersectionInterface*)Allocate<RoutableIntersectionType>();
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
					RegularInboundOutboundMovementsContainerType& regular_inbound_outbound_movements_container = regular_intersection->inbound_outbound_movements<RegularInboundOutboundMovementsContainerType&>();
					RegularInboundOutboundMovementsContainerType::iterator regular_inbound_outbound_movements_itr;
					
					for(regular_inbound_outbound_movements_itr=regular_inbound_outbound_movements_container.begin(); regular_inbound_outbound_movements_itr!=regular_inbound_outbound_movements_container.end(); regular_inbound_outbound_movements_itr++)
					{
						RegularInboundOutboundMovementsInterface* regular_inbound_outbound_movements = (RegularInboundOutboundMovementsInterface*)(*regular_inbound_outbound_movements_itr);
						RoutableInboundOutboundMovementsInterface* routable_inbound_outbound_movements = (RoutableInboundOutboundMovementsInterface*)Allocate<RoutableInboundOutboundMovementsType>();
						//float forward_link_turn_travel_time = regular_outbound_inbound_movements->forward_link_turn_travel_time<float>();
						//routable_outbound_inbound_movements->forward_link_turn_travel_time<float>(forward_link_turn_travel_time);
						RegularLinkInterface* regular_link = regular_inbound_outbound_movements->inbound_link_reference<RegularLinkInterface*>();
						RoutableLinkInterface* routable_link = linksMap.find(regular_link)->second;
						routable_inbound_outbound_movements->inbound_link_reference<RoutableLinkInterface*>(routable_link);
						//another level of loop
						RegularMovementsContainerType& regular_outbound_movements_container = regular_inbound_outbound_movements->outbound_movements<RegularMovementsContainerType&>();
						RegularMovementsContainerType::iterator regular_outbound_movement_itr;
						for(regular_outbound_movement_itr=regular_outbound_movements_container.begin(); regular_outbound_movement_itr!=regular_outbound_movements_container.end(); regular_outbound_movement_itr++)
						{
							RegularMovementInterface* regular_outbound_movement = (RegularMovementInterface*)(*regular_outbound_movement_itr);
							RoutableMovementInterface* routable_outbound_movement = (RoutableMovementInterface*)Allocate<RoutableMovementType>();
							routable_outbound_movement->forward_link_turn_travel_time<float>(regular_outbound_movement->forward_link_turn_travel_time<float>());
							RegularLinkInterface* regular_outbound_link = regular_outbound_movement->movement_reference<RegularLinkInterface*>();
							RoutableLinkInterface* routable_outbound_link = linksMap.find(regular_outbound_link)->second;
							routable_outbound_movement->movement_reference<RoutableLinkInterface*>(routable_outbound_link);
							regular_outbound_movement->replicas_container<RoutableMovementsContainerType&>().push_back(routable_outbound_movement);
							routable_inbound_outbound_movements->outbound_movements<RoutableMovementsContainerType&>().push_back(routable_outbound_movement);
						}
						routable_intersection->inbound_outbound_movements<RoutableInboundOutboundMovementsContainerType&>().push_back(routable_inbound_outbound_movements);
					}
					
					// create outbound_inbound_movements
					RegularOutboundInboundMovementsContainerType& regular_outbound_inbound_movements_container = regular_intersection->outbound_inbound_movements<RegularOutboundInboundMovementsContainerType&>();
					RegularOutboundInboundMovementsContainerType::iterator regular_outbound_inbound_movements_itr;
					
					for(regular_outbound_inbound_movements_itr=regular_outbound_inbound_movements_container.begin(); regular_outbound_inbound_movements_itr!=regular_outbound_inbound_movements_container.end(); regular_outbound_inbound_movements_itr++)
					{
						RegularOutboundInboundMovementsInterface* regular_outbound_inbound_movements = (RegularOutboundInboundMovementsInterface*)(*regular_outbound_inbound_movements_itr);
						RoutableOutboundInboundMovementsInterface* routable_outbound_inbound_movements = (RoutableOutboundInboundMovementsInterface*)Allocate<RoutableOutboundInboundMovementsType>();
						//float forward_link_turn_travel_time = regular_outbound_inbound_movements->forward_link_turn_travel_time<float>();
						//routable_outbound_inbound_movements->forward_link_turn_travel_time<float>(forward_link_turn_travel_time);
						RegularLinkInterface* regular_link = regular_outbound_inbound_movements->outbound_link_reference<RegularLinkInterface*>();
						RoutableLinkInterface* routable_link = linksMap.find(regular_link)->second;
						routable_outbound_inbound_movements->outbound_link_reference<RoutableLinkInterface*>(routable_link);
						//another level of loop
						RegularMovementsContainerType& regular_inbound_movements_container = regular_outbound_inbound_movements->inbound_movements<RegularMovementsContainerType&>();
						RegularMovementsContainerType::iterator regular_inbound_movement_itr;
						for(regular_inbound_movement_itr=regular_inbound_movements_container.begin(); regular_inbound_movement_itr!=regular_inbound_movements_container.end(); regular_inbound_movement_itr++)
						{
							RegularMovementInterface* regular_inbound_movement = (RegularMovementInterface*)(*regular_inbound_movement_itr);
							RoutableMovementInterface* routable_inbound_movement = (RoutableMovementInterface*)Allocate<RoutableMovementType>();
							routable_inbound_movement->forward_link_turn_travel_time<float>(regular_inbound_movement->forward_link_turn_travel_time<float>());
							RegularLinkInterface* regular_outbound_link = regular_inbound_movement->movement_reference<RegularLinkInterface*>();
							RoutableLinkInterface* routable_outbound_link = linksMap.find(regular_outbound_link)->second;
							routable_inbound_movement->movement_reference<RoutableLinkInterface*>(routable_outbound_link);
							regular_inbound_movement->replicas_container<RoutableMovementsContainerType&>().push_back(routable_inbound_movement);
							routable_outbound_inbound_movements->inbound_movements<RoutableMovementsContainerType&>().push_back(routable_inbound_movement);
						}
						routable_intersection->outbound_inbound_movements<RoutableInboundOutboundMovementsContainerType&>().push_back(routable_outbound_inbound_movements);
					}

					intersections<RoutableIntersectionsContainerType&>().push_back(routable_intersection);
					intersectionsMap.insert(pair<RegularIntersectionInterface*, RoutableIntersectionInterface*>(regular_intersection, routable_intersection));
				}

				// add upstream and downstream to each link
				RoutableLinksContainerType::iterator routable_link_itr;
				for(routable_link_itr=links<RoutableLinksContainerType&>().begin(); routable_link_itr!=links<RoutableLinksContainerType&>().end(); routable_link_itr++)
				{
					RoutableLinkInterface* routable_link = (RoutableLinkInterface*)(*routable_link_itr);
					RegularLinkInterface* regular_link = routable_link->network_link_reference<RegularLinkInterface*>();
					
					RegularIntersectionInterface* regular_upstream_intersection = regular_link->upstream_intersection<RegularIntersectionInterface*>();
					RoutableIntersectionInterface* routable_upstream_intersection = intersectionsMap.find(regular_upstream_intersection)->second;
					routable_link->upstream_intersection<RoutableIntersectionInterface*>(routable_upstream_intersection);

					RegularIntersectionInterface* regular_downstream_intersection = regular_link->downstream_intersection<RegularIntersectionInterface*>();
					RoutableIntersectionInterface* routable_downstream_intersection = intersectionsMap.find(regular_downstream_intersection)->second;
					routable_link->downstream_intersection<RoutableIntersectionInterface*>(routable_downstream_intersection);
				}
				
				// for debug, we printout everything of the network
				
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
							cout<<"\t\t\t\t\t movement_reference: "<<outbound_movement->movement_reference<RoutableLinkInterface*>()->uuid<int>()<<endl;
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
							cout<<"\t\t\t\t\t movement_reference: "<<outbound_movement->movement_reference<RoutableLinkInterface*>()->uuid<int>()<<endl;
							cout<<"\t\t\t\t\t forward_link_turn_travel_time: "<<outbound_movement->forward_link_turn_travel_time<float>()<<endl;
						}
					}
					cout<<"\t\t --------------------------------"<<endl<<endl;
				}	
			}

		};
		
		template<typename ThisType,typename CallerType>
		struct Routing_Interface
		{
			facet_accessor(vehicle);
			facet_accessor(network);
			facet_accessor(routable_network);
			facet_accessor(origin_link);
			facet_accessor(destination_link);

			declare_facet_conditional(Compute_Route_Condition)
			{
				response.next=INT_MAX;
				response.result=true;
			}

			facet void one_to_one_link_based_least_time_path_a_star()
			{
				
				typedef typename ThisType::routable_network_type RoutableNetworkType;
				typedef Routable_Network_Interface<RoutableNetworkType,ThisType> RoutableNetworkInterface;

				typedef typename RoutableNetworkType::scan_list_type ScanListType;
				//typedef typename RoutableNetworkType::num_searches_type NumSearchesType;
				typedef Routable_Network_Interface<RoutableNetworkType,ThisType> RoutableNetworkInterface;
				
				typedef typename RoutableNetworkType::links_container_type RoutableLinksContainerType;
				typedef typename RoutableNetworkType::links_container_element_type RoutableLinkType;
				
				typedef typename RoutableLinkType::network_link_type NetworkLinkType;
				
				typedef Link_Components::Interfaces::Link_Interface<RoutableLinkType,ThisType> RoutableLinkInterface;
				typedef Link_Components::Interfaces::Link_Interface<NetworkLinkType,ThisType> NetworkLinkInterface;

				typedef typename RoutableNetworkType::intersections_container_type IntersectionsContainerType;
				typedef typename RoutableNetworkType::intersections_container_element_type IntersectionType;
				typedef Intersection_Components::Interfaces::Intersection_Interface<IntersectionType,ThisType> IntersectionInterface;
				
				typedef typename IntersectionType::inbound_outbound_movements_container_type InboundOutboundMovementsContainerType;
				typedef typename IntersectionType::inbound_outbound_movements_container_element_type InboundOutboundMovementsType;
				typedef Intersection_Components::Interfaces::Inbound_Outbound_Movements_Interface<InboundOutboundMovementsType,ThisType> InboundOutboundMovementsInterface;

				typedef typename IntersectionType::movements_container_type MovementsContainerType;
				typedef typename IntersectionType::movements_container_element_type MovementType;
				typedef Intersection_Components::Interfaces::Movement_Interface<MovementType,ThisType> MovementInterface;
				

				RoutableNetworkInterface* routable_net=routable_network<RoutableNetworkInterface*>();
				routable_net->Reset<NULLTYPE>();

				//RoutableNetworkInterface* routable_network=routable_network<RoutableNetworkInterface*>();
				float max_free_flow_speed=routable_net->max_free_flow_speed<float>();
				ScanListType& scan_list=routable_net->scan_list<ScanListType&>();
				//NumSearchesType& num_searches=routable_network->num_searches<NumSearchesType&>();

				RoutableLinkInterface* origin_link_ptr=origin_link<RoutableLinkInterface*>();
				RoutableLinkInterface* destination_link_ptr=destination_link<RoutableLinkInterface*>();
				NetworkLinkInterface* destination_reference=destination_link_ptr->network_link_reference<NetworkLinkInterface*>();
				typedef ThisType::turn_movements_container_type TurnMovementsContainerType;
				NetworkLinkInterface* net_origin_link=origin_link_ptr->network_link_reference<NetworkLinkInterface*>();
				TurnMovementsContainerType& turn_mvmt_container=net_origin_link->outbound_turn_movements<TurnMovementsContainerType&>();
				int outbound_turn_movement_size = turn_mvmt_container.size();
				if (outbound_turn_movement_size == 0){return;}

				//initialization
				RoutableLinkInterface* nextlink;
				//int icurlink;
				//int iturn_movement;
				float next_cost,new_cost;
				float dx,dy;
				float destination_x,destination_y;

				IntersectionInterface* destination_upstream_intersection=destination_link_ptr->upstream_intersection<IntersectionInterface*>();
				destination_x = destination_upstream_intersection->x_position<float>();
				destination_y = destination_upstream_intersection->y_position<float>();
				
				next_cost=origin_link_ptr->network_link_reference<NetworkLinkInterface*>()->travel_time<float>();
				new_cost = next_cost;
				RoutableLinkInterface* current_link=origin_link_ptr;

				current_link->label_cost<float>(new_cost); // g - label_cost

				IntersectionInterface* current_intersection;

				if(current_link->network_link_reference<NetworkLinkInterface*>() != destination_reference)
				{
					current_intersection=current_link->upstream_intersection<IntersectionInterface*>();
					dx = destination_x - current_intersection->x_position<float>();
					dy = destination_y - current_intersection->y_position<float>();
					current_link->h_cost<float>(sqrt(dx*dx+dy*dy)/max_free_flow_speed); // h - estimated = sqrt(dx*dx+dy*dy)/max_free_flow_speed 
					current_link->f_cost<float>(current_link->label_cost<float>()+current_link->h_cost<float>()); // f = g + h;
				}
				else
				{
					return;
				}

				current_link->label_pointer<RoutableLinkInterface*>(nullptr);

				scan_list.insert(make_pair(current_link->f_cost<float>(),current_link));
				current_link->scan_list_status<Scan_List_Status_Keys>(INSELIST);

				while(!scan_list.empty())
				{
					//selection
					//num_searches++;
					current_link = (RoutableLinkInterface*)(scan_list.begin()->second);
					
					if(current_link == destination_link_ptr)
					{
						break;
					}

					scan_list.erase(scan_list.begin());	// no need to find minimum since set is ordered by its first element, i.e. label cost.
					current_link->scan_list_status<Scan_List_Status_Keys>(SCANNED);

					current_intersection=current_link->downstream_intersection<IntersectionInterface*>();

					
					//for all outbound turn movements
					InboundOutboundMovementsContainerType& inbound_outbound_movements_container = current_intersection->inbound_outbound_movements<InboundOutboundMovementsContainerType&>();
					InboundOutboundMovementsContainerType::iterator inbound_itr;
					for(inbound_itr=inbound_outbound_movements_container.begin(); inbound_itr!=inbound_outbound_movements_container.end(); inbound_itr++)
					{
						InboundOutboundMovementsInterface* inbound_outbound_movements = (InboundOutboundMovementsInterface*)(*inbound_itr);
						RoutableLinkInterface* inbound_link = inbound_outbound_movements->inbound_link_reference<RoutableLinkInterface*>();
						if (inbound_link == current_link)
						{
							MovementsContainerType& outbound_movements_container = inbound_outbound_movements->outbound_movements<MovementsContainerType&>();
							MovementsContainerType::iterator outbound_itr;
							for(outbound_itr=outbound_movements_container.begin(); outbound_itr!=outbound_movements_container.end(); outbound_itr++)
							{
								MovementInterface* outbound_movement = (MovementInterface*)(*outbound_itr);
								RoutableLinkInterface* next_link=outbound_movement->movement_reference<RoutableLinkInterface*>();
								next_cost=outbound_movement->forward_link_turn_travel_time<float>();
								new_cost=current_link->label_cost<float>();

								if(next_link->label_cost<float>()>new_cost)
								{
									if(next_link->scan_list_status<Scan_List_Status_Keys>()==INSELIST)
									{
										scan_list.erase(make_pair(next_link->f_cost<float>(),nextlink)); // delete the old cost
									}

									next_link->label_cost<float>(new_cost);
									next_link->label_pointer<RoutableLinkInterface*>(current_link);

									if(next_link->network_link_reference<NetworkLinkInterface*>()!=destination_reference)
									{
										dx=destination_x - next_link->downstream_intersection<IntersectionInterface*>()->x_position<float>();
										dy=destination_y - next_link->downstream_intersection<IntersectionInterface*>()->y_position<float>();
										next_link->h_cost<float>(sqrt(dx*dx+dy*dy)/max_free_flow_speed);
									}
									else
									{
										next_link->h_cost<float>(0);
									}

									next_link->f_cost<float>(next_link->label_cost<float>() + next_link->h_cost<float>());

									scan_list.insert(make_pair(next_link->f_cost<float>(),nextlink)); // update with the new cost
									next_link->scan_list_status<Scan_List_Status_Keys>(INSELIST);
								}
							}
							break;
						}
					}
				}


				typedef typename RoutableNetworkType::reversed_path_container_type ReversedPathContainerType;
				ReversedPathContainerType& reversed_path_container=routable_network<RoutableNetworkInterface*>()->reversed_path_container<ReversedPathContainerType&>();

				current_link=destination_link<RoutableLinkInterface*>();
				reversed_path_container.push_back(current_link->network_link_reference<NetworkLinkInterface*>());
				current_link=current_link->label_pointer<RoutableLinkInterface*>();
					
				while(current_link != nullptr)
				{
					reversed_path_container.push_back(current_link->network_link_reference<NetworkLinkInterface*>());
					current_link=current_link->label_pointer<RoutableLinkInterface*>();
				}
			};


			facet void Schedule_Route_Computation(int departure_time)
			{
				schedule_event_local(ThisType,Compute_Route_Condition,Compute_Route,departure_time,NULLTYPE);
			}

			declare_facet_event(Compute_Route)
			{
				Routing_Interface* _this=(Routing_Interface*)pthis;
				typedef typename ThisType::vehicle_type VehicleType;
				typedef Vehicle_Components::Interfaces::Vehicle_Interface<VehicleType,ThisType> VehicleInterface;	
				VehicleInterface* veh=_this->vehicle<VehicleInterface*>();
				
				typedef typename ThisType::routable_network_type RoutableNetworkType;
				typedef typename RoutableNetworkType::links_container_element_type RoutableLinkType;
				typedef typename RoutableLinkType::network_link_type NetworkLinkType;

				typedef Link_Components::Interfaces::Link_Interface<NetworkLinkType,ThisType> NetworkLinkInterface;	
				typedef Link_Components::Interfaces::Link_Interface<RoutableLinkType,ThisType> RoutableLinkInterface;
				
				NetworkLinkInterface* origin_link=veh->origin_link<NetworkLinkInterface*>();
				NetworkLinkInterface* destination_link=veh->destination_link<NetworkLinkInterface*>();
				
				_this->origin_link<NetworkLinkInterface*>(origin_link);
				_this->destination_link<NetworkLinkInterface*>(destination_link);

				_this->one_to_one_link_based_least_time_path_a_star<NULLTYPE>();
				
				typedef Routable_Network_Interface<RoutableNetworkType,ThisType> RoutableNetworkInterface;
				RoutableNetworkInterface* routable_network_ptr=_this->routable_network<RoutableNetworkInterface*>();
				
				typedef typename RoutableNetworkType::reversed_path_container_type ReversedPathContainerType;

				veh->set_route_links(routable_network_ptr->reversed_path_container<ReversedPathContainerType&>());

				origin_link->push_vehicle(veh);
			}
		};
	}
}