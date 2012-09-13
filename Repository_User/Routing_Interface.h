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

				typedef typename ThisType::links_type RoutableLinksType;
				typedef typename ThisType::links_element_type RoutableLinkType;
				typedef Link_Components::Interfaces::Link_Interface<RoutableLinkType,ThisType> RoutableLinkInterface;
				
				RoutableLinksType& links_container=links<RoutableLinksType&>();
				typename RoutableLinksType::iterator link_itr;

				for(link_itr=links_container.begin();link_itr!=links_container.end();links_itr++)
				{
					link_itr->label_cost<float>(FLT_MAX);
					link_itr->label_pointer<RoutableLinkInterface*>(nullptr);
					link_itr->scan_list_status<Scan_List_Status_Keys>(UNSCANNED);
					link_itr->f_cost<float>(0);
					link_itr->h_cost<float>(0);
				}
			}
		};
		
		template<typename ThisType,typename CallerType>
		struct Routing_Interface
		{
			facet_accessor(vehicle);
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
				
				typedef typename RoutableNetworkType::links_type RoutableLinksType;
				typedef typename RoutableNetworkType::links_element_type RoutableLinkType;
				
				typedef typename LinkType::network_link_reference_type NetworkLinkType;
				
				typedef Link_Components::Interfaces::Link_Interface<RoutableLinkType,ThisType> RoutableLinkInterface;
				typedef Link_Components::Interfaces::Link_Interface<NetworkLinkType,ThisType> NetworkLinkInterface;

				typedef typename RoutableNetworkType::intersections_type IntersectionsType;
				typedef typename RoutableNetworkType::intersections_element_type IntersectionType;
				typedef Intersection_Components::Interfaces::Intersection_Interface<IntersectionType,ThisType> IntersectionInterface;
				
				typedef typename IntersectionType::outbound_inbound_movements_type OutboundInboundType;
				typedef typename IntersectionType::outbound_inbound_movements_element_type OutboundInboundMovementsType;
				typedef Intersection_Components::Interfaces::Outbound_Inbound_Movements_Interface<OutboundInboundElementType,ThisType> OutboundInboundMovementsInterface;			
				

				RoutableNetworkInterface* routable_network=routable_network<RoutableNetworkInterface*>();
				routable_network->Reset<NULLTYPE>();

				RoutableNetworkInterface* routable_network=routable_network<RoutableNetworkInterface*>();
				float max_free_flow_speed=routable_network->max_free_flow_speed<float>();
				ScanListType& scan_list=routable_network->scan_list<ScanListType&>();
				//NumSearchesType& num_searches=routable_network->num_searches<NumSearchesType&>();

				RoutableLinkInterface* origin_link=origin_link<RoutableLinkInterface*>();
				RoutableLinkInterface* destination_link=destination_link<RoutableLinkInterface*>();
				NetworkLinkInterface* destination_reference=destination_link->network_link_reference<NetworkLinkInterface*>();

				int outbound_turn_movement_size = origin_link->outbound_turn_movement_size<int>();
				if (outbound_turn_movement_size == 0){return;}

				//initialization
				int icurlink,nextlink,iturn_movement;
				float next_cost,new_cost;
				float dx,dy;
				float destination_x,destination_y;

				IntersectionInterface* destination_upstream_intersection=destination_link->upstream_intersection<IntersectionInterface*>();
				destination_upstream_intersection->x_position<float>();
				destination_upstream_intersection->y_position<float>();
				
				next_cost=origin_link->link_travel_time<float>();
				new_cost = next_cost;
				RoutableLinkInterface* current_link=origin_link;

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
					current_link = scan_list.begin()->second;
					
					if(current_link == destination_link)
					{
						break;
					}

					scan_list.erase(scan_list.begin());	// no need to find minimum since set is ordered by its first element, i.e. label cost.
					current_link->scan_list_status<Scan_List_Status_Keys>(SCANNED);

					current_intersection=current_link->downstream_intersection<IntersectionInterface*>();

					//for all outbound turn movements
					
					OutboundInboundType& outbound_links_container=current_intersection->outbound_inbound_movements<OutboundInboundType&>();
					typename OutboundInboundType::iterator outbound_itr;

					for(outbound_itr=outbound_links_container.begin(); outbound_itr!=outbound_links_container.end(); outbound_itr++)
					{
						OutboundInboundMovementsInterface* outbound=(*outbound_itr);
						RoutableLinkInterface* next_link=outbound->outbound_movement_reference<RoutableLinkInterface*>();
						next_cost=outbound->forward_link_turn_travel_time<float>();
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
								dx=destination_x - next_link->downstream_intersection<IntersectionInterface*>()->x_position();
								dy=destination_y - next_link->downstream_intersection<IntersectionInterface*>()->y_position();
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

					typedef typename RoutableNetworkType::reversed_path_container_type ReversedPathContainerType;
					ReversedPathContainerType& reversed_path_container=routable_network->reversed_path_container<ReversedPathContainerType&>()

					current_link=destination_link<RoutableLinkInterface*>();
					reversed_path_container.push_back(current_link->network_link_reference<NetworkLinkInterface*>());
					current_link=current_link->label_pointer<RoutableLinkInterface*>();
					
					while(current_link != nullptr)
					{
						reversed_path_container.push_back(current_link->network_link_reference<NetworkLinkInterface*>());
						current_link=current_link->label_pointer<RoutableLinkInterface*>();
					}
				}
			};

			declare_facet_event(Compute_Route)
			{
				typedef typename ThisType::vehicle_type VehicleType;
				typedef Vehicle_Components::Interfaces::Vehicle_Interface<VehicleType,ThisType> VehicleInterface;	
				VehicleInterface* veh=vehicle<VehicleInterface*>();
				
				typedef typename ThisType::routable_network_type RoutableNetworkType;
				typedef typename RoutableNetworkType::links_element_type RoutableLinkType;
				typedef typename RoutableLinkType::network_link_reference_type NetworkLinkType;

				typedef Link_Components::Interfaces::Link_Interface<NetworkLinkType,ThisType> NetworkLinkInterface;	
				typedef Link_Components::Interfaces::Link_Interface<RoutableLinkType,ThisType> RoutableLinkInterface;
				
				NetworkLinkInterface* origin_link=veh->origin_link<NetworkLinkInterface*>();
				NetworkLinkInterface* destination_link=veh->destination_link<NetworkLinkInterface*>();
				
				origin_link<NetworkLinkInterface*>();
				destination_link<NetworkLinkInterface*>();

				one_to_one_link_based_least_time_path_a_star<NULLTYPE>();
				
				typedef Routable_Network_Interface<RoutableNetworkType,ThisType> RoutableNetworkInterface;
				RoutableNetworkInterface* routable_network=routable_network<RoutableNetworkInterface*>();
				
				typedef typename RoutableNetworkType::reversed_path_container_type ReversedPathContainerType;

				veh->set_route_links(routable_network->reversed_path_container<ReversedPathContainerType&>());

				origin_link->push_vehicle(veh);
			}
		};
	}
}