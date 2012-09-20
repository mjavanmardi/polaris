#pragma once
#include "Activity_Location_Interface.h"
#include "Routing_Interface.h"
#include "Intersection_Interface.h"
#include "Traveler_Interface.h"
#include <hash_map>

namespace Network_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Interfaces
	{
		template<typename ThisType,typename CallerType>
		struct Network_Interface
		{
			facet_accessor(max_free_flow_speed);
			facet_accessor(intersections_container);
			facet_accessor(links_container);
			facet_accessor(turn_movements_container);
			facet_accessor(activity_locations_container);
			facet_accessor(routable_network);

			facet_accessor(routable_networks_container);

			facet void read_network_data()
			{
				int i = 0;
				float link_length = 5280.0;
				float speed_limit = 60.0;
				float speed_limit_ramp = 30.0;
				float maximum_flow_rate = 2200.0;
				float maximum_flow_rate_ramp = 600.0;
				float maximum_flow_rate_arterial = 900;
				float jam_density = 220.0;
				float backward_wave_speed = 12.0;
				float distance_factor = 1.5;
				
				max_free_flow_speed<int>(-1);

				typedef typename ThisType::intersections_container_type IntersectionsContainerType;
				typedef typename ThisType::intersections_container_element_type IntersectionType;
				typedef typename Intersection_Components::Interfaces::Intersection_Interface<IntersectionType,ThisType> Intersection_Interface;

				intersections_container<IntersectionsContainerType&>().clear();
				
				//node
				//network_information::network_data_information::NodeData node_data;
				

				i = 0;
				//node_data.node_index = i;
				//node_data.uuid = "1";
				Intersection_Interface* intersection_0=(Intersection_Interface*)Allocate<IntersectionType>();
				intersection_0->uuid<int>(0);
				intersection_0->x_position<float>(0.0);
				intersection_0->y_position<float>((float) 2.0*link_length);
				intersection_0->intersection_type<int>(Intersection_Components::Types::NO_CONTROL);
				intersections_container<IntersectionsContainerType&>().push_back(intersection_0);

				i = 1;

				//node_data.node_index = i;
				//node_data.uuid = "2";
				
				Intersection_Interface* intersection_1=(Intersection_Interface*)Allocate<IntersectionType>();
				intersection_1->uuid<int>(1);
				intersection_1->x_position<float>(i * link_length);
				intersection_1->y_position<float>((float) 2.0*link_length);
				intersection_1->intersection_type<int>(Intersection_Components::Types::NO_CONTROL);
				intersections_container<IntersectionsContainerType&>().push_back(intersection_1);

				i = 2;

				//node_data.node_index = i;
				//node_data.uuid = "3";
				
				Intersection_Interface* intersection_2=(Intersection_Interface*)Allocate<IntersectionType>();
				intersection_2->uuid<int>(2);
				intersection_2->x_position<float>(i * link_length);
				intersection_2->y_position<float>((float) 2.0*link_length);
				intersection_2->intersection_type<int>(Intersection_Components::Types::NO_CONTROL);
				intersections_container<IntersectionsContainerType&>().push_back(intersection_2);

				i = 3;

				//node_data.node_index = i;
				//node_data.uuid = "4";
				
				Intersection_Interface* intersection_3=(Intersection_Interface*)Allocate<IntersectionType>();
				intersection_3->uuid<int>(3);
				intersection_3->x_position<float>(i * link_length);
				intersection_3->y_position<float>((float) 2.0*link_length);
				intersection_3->intersection_type<int>(Intersection_Components::Types::NO_CONTROL);
				intersections_container<IntersectionsContainerType&>().push_back(intersection_3);

				i = 4;

				//node_data.node_index = i;
				//node_data.uuid = "5";
				
				Intersection_Interface* intersection_4=(Intersection_Interface*)Allocate<IntersectionType>();
				intersection_4->uuid<int>(4);
				intersection_4->x_position<float>(i * link_length);
				intersection_4->y_position<float>((float) 2.0*link_length);
				intersection_4->intersection_type<int>(Intersection_Components::Types::NO_CONTROL);
				intersections_container<IntersectionsContainerType&>().push_back(intersection_4);

				i = 5;

				//node_data.node_index = i;
				//node_data.uuid = "6";
				
				Intersection_Interface* intersection_5=(Intersection_Interface*)Allocate<IntersectionType>();
				intersection_5->uuid<int>(5);
				intersection_5->x_position<float>((float) 1.0 * link_length);
				intersection_5->y_position<float>((float) 1.0 * link_length);
				intersection_5->intersection_type<int>(Intersection_Components::Types::NO_CONTROL);
				intersections_container<IntersectionsContainerType&>().push_back(intersection_5);

				i = 6;

				//node_data.node_index = i;
				//node_data.uuid = "7";
				
				Intersection_Interface* intersection_6=(Intersection_Interface*)Allocate<IntersectionType>();
				intersection_6->uuid<int>(6);
				intersection_6->x_position<float>(0.0);
				intersection_6->y_position<float>(0.0);
				intersection_6->intersection_type<int>(Intersection_Components::Types::NO_CONTROL);
				intersections_container<IntersectionsContainerType&>().push_back(intersection_6);

				//link
				//network_data.link_data_array.clear();
				typedef typename ThisType::links_container_type LinksContainerType;
				typedef typename ThisType::links_container_element_type LinkType;
				typedef typename Link_Components::Interfaces::Link_Interface<LinkType,ThisType> Link_Interface;

				links_container<LinksContainerType&>().clear();
				
				Link_Interface* link_0 = (Link_Interface*)Allocate<LinkType>();
				Link_Interface* link_1 = (Link_Interface*)Allocate<LinkType>();
				Link_Interface* link_2 = (Link_Interface*)Allocate<LinkType>();
				Link_Interface* link_3 = (Link_Interface*)Allocate<LinkType>();
				Link_Interface* link_4 = (Link_Interface*)Allocate<LinkType>();
				Link_Interface* link_5 = (Link_Interface*)Allocate<LinkType>();

				i = 0;

				link_0->upstream_intersection<Intersection_Interface*>(intersection_0);
				link_0->downstream_intersection<Intersection_Interface*>(intersection_1);
				link_0->uuid<int>(0);
				link_0->num_lanes<int>(2);
				link_0->link_type<Link_Components::Types::Link_Type_Keys>(Link_Components::Types::FREEWAY);
				link_0->length<float>(link_length*distance_factor);
				link_0->speed_limit<float>(speed_limit);
				link_0->maximum_flow_rate<float>(maximum_flow_rate);
				link_0->backward_wave_speed<float>(backward_wave_speed);
				link_0->free_flow_speed<float>(speed_limit + 15.0);
				link_0->jam_density<float>(jam_density);
				link_0->left_turn_bay_length<float>(0.0);
				link_0->right_turn_bay_length<float>(0.0);
				link_0->num_left_turn_bays<int>(0);
				link_0->num_right_turn_bays<int>(0);
				links_container<LinksContainerType&>().push_back(link_0);

				max_free_flow_speed<float>(max(max_free_flow_speed<float>(),link_0->free_flow_speed<float>()));
				
				i = 1;

				link_1->upstream_intersection<Intersection_Interface*>(intersection_1);
				link_1->downstream_intersection<Intersection_Interface*>(intersection_2);
				link_1->uuid<int>(1);
				link_1->num_lanes<int>(2);
				link_1->link_type<Link_Components::Types::Link_Type_Keys>(Link_Components::Types::FREEWAY);
				link_1->length<float>(link_length*distance_factor);
				link_1->speed_limit<float>(speed_limit);
				link_1->maximum_flow_rate<float>(maximum_flow_rate);
				link_1->backward_wave_speed<float>(backward_wave_speed);
				link_1->free_flow_speed<float>(speed_limit + 15.0);
				link_1->jam_density<float>(jam_density);
				link_1->left_turn_bay_length<float>(0.0);
				link_1->right_turn_bay_length<float>(0.0);
				link_1->num_left_turn_bays<int>(0);
				link_1->num_right_turn_bays<int>(0);
				links_container<LinksContainerType&>().push_back(link_1);

				max_free_flow_speed<float>(max(max_free_flow_speed<float>(),link_1->free_flow_speed<float>()));

				i = 2;

				link_2->upstream_intersection<Intersection_Interface*>(intersection_2);
				link_2->downstream_intersection<Intersection_Interface*>(intersection_3);
				link_2->uuid<int>(2);
				link_2->num_lanes<int>(2);
				link_2->link_type<Link_Components::Types::Link_Type_Keys>(Link_Components::Types::FREEWAY);
				link_2->length<float>(link_length*distance_factor);
				link_2->speed_limit<float>(speed_limit);
				link_2->maximum_flow_rate<float>(maximum_flow_rate);
				link_2->backward_wave_speed<float>(backward_wave_speed);
				link_2->free_flow_speed<float>(speed_limit + 15.0);
				link_2->jam_density<float>(jam_density);
				link_2->left_turn_bay_length<float>(0.0);
				link_2->right_turn_bay_length<float>(0.0);
				link_2->num_left_turn_bays<int>(0);
				link_2->num_right_turn_bays<int>(0);
				links_container<LinksContainerType&>().push_back(link_2);

				max_free_flow_speed<float>(max(max_free_flow_speed<float>(),link_2->free_flow_speed<float>()));

				i = 3;

				link_3->upstream_intersection<Intersection_Interface*>(intersection_3);
				link_3->downstream_intersection<Intersection_Interface*>(intersection_4);
				link_3->uuid<int>(3);
				link_3->num_lanes<int>(2);
				link_3->link_type<Link_Components::Types::Link_Type_Keys>(Link_Components::Types::FREEWAY);
				link_3->length<float>(link_length*distance_factor);
				link_3->speed_limit<float>(speed_limit);
				link_3->maximum_flow_rate<float>(maximum_flow_rate);
				link_3->backward_wave_speed<float>(backward_wave_speed);
				link_3->free_flow_speed<float>(speed_limit + 15.0);
				link_3->jam_density<float>(jam_density);
				link_3->left_turn_bay_length<float>(0.0);
				link_3->right_turn_bay_length<float>(0.0);
				link_3->num_left_turn_bays<int>(0);
				link_3->num_right_turn_bays<int>(0);
				links_container<LinksContainerType&>().push_back(link_3);

				max_free_flow_speed<float>(max(max_free_flow_speed<float>(),link_3->free_flow_speed<float>()));


				i = 4;

				link_4->upstream_intersection<Intersection_Interface*>(intersection_5);
				link_4->downstream_intersection<Intersection_Interface*>(intersection_2);
				link_4->uuid<int>(4);
				link_4->num_lanes<int>(1);
				link_4->link_type<Link_Components::Types::Link_Type_Keys>(Link_Components::Types::ON_RAMP);
				link_4->length<float>(link_length*distance_factor);
				link_4->speed_limit<float>(speed_limit_ramp);
				link_4->maximum_flow_rate<float>(maximum_flow_rate_ramp);
				link_4->backward_wave_speed<float>(backward_wave_speed);
				link_4->free_flow_speed<float>(speed_limit + 15.0);
				link_4->jam_density<float>(jam_density);
				link_4->left_turn_bay_length<float>(0.0);
				link_4->right_turn_bay_length<float>(0.0);
				link_4->num_left_turn_bays<int>(0);
				link_4->num_right_turn_bays<int>(0);
				links_container<LinksContainerType&>().push_back(link_4);

				max_free_flow_speed<float>(max(max_free_flow_speed<float>(),link_4->free_flow_speed<float>()));

				i = 5;

				link_5->upstream_intersection<Intersection_Interface*>(intersection_6);
				link_5->downstream_intersection<Intersection_Interface*>(intersection_5);
				link_5->uuid<int>(5);
				link_5->num_lanes<int>(1);
				link_5->link_type<Link_Components::Types::Link_Type_Keys>(Link_Components::Types::ARTERIAL);
				link_5->length<float>(link_length*distance_factor);
				link_5->speed_limit<float>(speed_limit_ramp);
				link_5->maximum_flow_rate<float>(maximum_flow_rate_arterial);
				link_5->backward_wave_speed<float>(backward_wave_speed);
				link_5->free_flow_speed<float>(speed_limit + 10.0);
				link_5->jam_density<float>(jam_density);
				link_5->left_turn_bay_length<float>(0.0);
				link_5->right_turn_bay_length<float>(0.0);
				link_5->num_left_turn_bays<int>(0);
				link_5->num_right_turn_bays<int>(0);
				links_container<LinksContainerType&>().push_back(link_5);

				max_free_flow_speed<float>(max(max_free_flow_speed<float>(),link_5->free_flow_speed<float>()));

				//========================================================================================================
				//Create inbound/outbound links for each intersection
				//--------------------------------------------------------------------------------------------------------
				//
				for (i = 0; i < (int)links_container<LinksContainerType&>().size(); i++)
				{
					Link_Interface* link = (Link_Interface*)links_container<LinksContainerType&>()[i];
					link->upstream_intersection<Intersection_Interface*>()->outbound_links<LinksContainerType&>().push_back(link);
					link->downstream_intersection<Intersection_Interface*>()->inbound_links<LinksContainerType&>().push_back(link);
				}

				//========================================================================================================
				//Create turn movements for each intersection
				//--------------------------------------------------------------------------------------------------------
				//
				
				//turn connectivities
				typedef typename ThisType::turn_movements_container_type TurnMovementsContainerType;
				typedef typename ThisType::turn_movements_element_type TurnMovementType;
				typedef typename Link_Components::Interfaces::Turn_Movement_Interface<TurnMovementType, NULLTYPE> TurnMovementInterface;
				
				TurnMovementInterface* turn_movement_0 = (TurnMovementInterface*)Allocate<TurnMovementType>();
				turn_movement_0->inbound_link<Link_Interface*>((Link_Interface*)links_container<LinksContainerType&>()[0]);
				turn_movement_0->outbound_link<Link_Interface*>((Link_Interface*)links_container<LinksContainerType&>()[1]);

				int c=turn_movement_0->outbound_link<Link_Interface*>()->uuid<int>();

				turn_movement_0->turn_movement_type<Intersection_Components::Types::Turn_Movement_Type_Keys>(Intersection_Components::Types::THROUGH_TURN);
				turn_movement_0->turn_movement_rule<Intersection_Components::Types::Turn_Movement_Rule_Keys>(Intersection_Components::Types::ALLOWED);
				turn_movement_0->id<int>(7);
				turn_movements_container<TurnMovementsContainerType&>().push_back(turn_movement_0);

				TurnMovementInterface* turn_movement_1 = (TurnMovementInterface*)Allocate<TurnMovementType>();
				turn_movement_1->inbound_link<Link_Interface*>((Link_Interface*)links_container<LinksContainerType&>()[1]);
				turn_movement_1->outbound_link<Link_Interface*>((Link_Interface*)links_container<LinksContainerType&>()[2]);
				turn_movement_1->turn_movement_type<Intersection_Components::Types::Turn_Movement_Type_Keys>(Intersection_Components::Types::THROUGH_TURN);
				turn_movement_1->turn_movement_rule<Intersection_Components::Types::Turn_Movement_Rule_Keys>(Intersection_Components::Types::ALLOWED);
				turn_movement_1->id<int>(8);
				turn_movements_container<TurnMovementsContainerType&>().push_back(turn_movement_1);

				TurnMovementInterface* turn_movement_2 = (TurnMovementInterface*)Allocate<TurnMovementType>();
				turn_movement_2->inbound_link<Link_Interface*>((Link_Interface*)links_container<LinksContainerType&>()[2]);
				turn_movement_2->outbound_link<Link_Interface*>((Link_Interface*)links_container<LinksContainerType&>()[3]);
				turn_movement_2->turn_movement_type<Intersection_Components::Types::Turn_Movement_Type_Keys>(Intersection_Components::Types::THROUGH_TURN);
				turn_movement_2->turn_movement_rule<Intersection_Components::Types::Turn_Movement_Rule_Keys>(Intersection_Components::Types::ALLOWED);
				turn_movement_2->id<int>(9);
				turn_movements_container<TurnMovementsContainerType&>().push_back(turn_movement_2);

				TurnMovementInterface* turn_movement_3 = (TurnMovementInterface*)Allocate<TurnMovementType>();
				turn_movement_3->inbound_link<Link_Interface*>((Link_Interface*)links_container<LinksContainerType&>()[4]);
				turn_movement_3->outbound_link<Link_Interface*>((Link_Interface*)links_container<LinksContainerType&>()[2]);
				turn_movement_3->turn_movement_type<Intersection_Components::Types::Turn_Movement_Type_Keys>(Intersection_Components::Types::RIGHT_TURN);
				turn_movement_3->turn_movement_rule<Intersection_Components::Types::Turn_Movement_Rule_Keys>(Intersection_Components::Types::ALLOWED);
				turn_movement_3->id<int>(10);
				turn_movements_container<TurnMovementsContainerType&>().push_back(turn_movement_3);

				
				TurnMovementInterface* turn_movement_4 = (TurnMovementInterface*)Allocate<TurnMovementType>();
				turn_movement_4->inbound_link<Link_Interface*>((Link_Interface*)links_container<LinksContainerType&>()[5]);
				turn_movement_4->outbound_link<Link_Interface*>((Link_Interface*)links_container<LinksContainerType&>()[4]);
				turn_movement_4->turn_movement_type<Intersection_Components::Types::Turn_Movement_Type_Keys>(Intersection_Components::Types::THROUGH_TURN);
				turn_movement_4->turn_movement_rule<Intersection_Components::Types::Turn_Movement_Rule_Keys>(Intersection_Components::Types::ALLOWED);
				turn_movement_4->id<int>(11);
				turn_movements_container<TurnMovementsContainerType&>().push_back(turn_movement_4);


				for (i = 0; i < (int)turn_movements_container<TurnMovementsContainerType&>().size(); i++)
				{
					TurnMovementInterface* turn_mvmt=(TurnMovementInterface*)turn_movements_container<TurnMovementsContainerType&>()[i];

					//outbound turn movement of an inbound link
					Link_Interface* inbound_link = turn_mvmt->inbound_link<Link_Interface*>();
					inbound_link->outbound_turn_movements<TurnMovementsContainerType&>().push_back(turn_mvmt);
					//inbound turn movement of an outbound link
					Link_Interface* outbound_link = turn_mvmt->outbound_link<Link_Interface*>();
					outbound_link->inbound_turn_movements<TurnMovementsContainerType&>().push_back(turn_mvmt);
				}

				typedef typename ThisType::outbound_inbound_movements_container_type OutboundInboundMovementsContainerType;
				typedef typename ThisType::outbound_inbound_movements_container_element_type OutboundInboundMovementsType;
				typedef typename Intersection_Components::Interfaces::Outbound_Inbound_Movements_Interface<OutboundInboundMovementsType,ThisType> OutboundInboundMovementsInterface;

				typedef typename ThisType::inbound_outbound_movements_container_type InboundOutboundMovementsContainerType;
				typedef typename ThisType::inbound_outbound_movements_container_element_type InboundOutboundMovementsType;
				typedef typename Intersection_Components::Interfaces::Inbound_Outbound_Movements_Interface<InboundOutboundMovementsType,ThisType> InboundOutboundMovementsInterface;

				typedef typename ThisType::movements_container_type MovementsContainerType;
				typedef typename ThisType::movements_container_element_type MovementType;
				typedef typename Intersection_Components::Interfaces::Movement_Interface<MovementType,ThisType> MovementInterface;

				int j, k;

				// configure outbound_inbound_movements
				for (i = 0; i < (int)intersections_container<IntersectionsContainerType&>().size(); i++)
				{
					Intersection_Interface* intersection = (Intersection_Interface*)intersections_container<IntersectionsContainerType&>()[i];
					// skip intersections_container that do not have any outbound links
					if (intersection->outbound_links<LinksContainerType&>().size() == 0)
						continue;
					
					for (j = 0; j < (int)intersection->outbound_links<LinksContainerType&>().size(); j++) 
					{
						OutboundInboundMovementsInterface* outboundInboundMovements = (OutboundInboundMovementsInterface*)Allocate<OutboundInboundMovementsType>();
						Link_Interface* outboundLink = (Link_Interface*)intersection->outbound_links<LinksContainerType&>()[j];
						outboundInboundMovements->outbound_link_reference<Link_Interface*>(outboundLink);
						TurnMovementsContainerType& inboundTurnMovements = outboundLink->inbound_turn_movements<TurnMovementsContainerType&>();
						
						for (k = 0; k < (int)inboundTurnMovements.size(); k++)
						{
							TurnMovementInterface* inboundMovement = (TurnMovementInterface*)inboundTurnMovements[k];

							MovementInterface* movement = (MovementInterface*)Allocate<MovementType>();
							movement->movement_reference<Link_Interface*>(inboundMovement->inbound_link<Link_Interface*>());
							movement->movement_type<Intersection_Components::Types::Turn_Movement_Type_Keys>(inboundMovement->turn_movement_type<Intersection_Components::Types::Turn_Movement_Type_Keys>());
							movement->movement_type<Intersection_Components::Types::Turn_Movement_Rule_Keys>(inboundMovement->turn_movement_rule<Intersection_Components::Types::Turn_Movement_Rule_Keys>());
							movement->forward_link_turn_travel_time<float>(inboundMovement->forward_link_turn_travel_time<float>());
							outboundInboundMovements->inbound_movements<MovementsContainerType&>().push_back(movement);
							inboundMovement->replicas_container<MovementsContainerType&>().push_back(movement);
						}
						intersection->outbound_inbound_movements<OutboundInboundMovementsContainerType&>().push_back(outboundInboundMovements);
					}
				}
						
				// configure inbound_outbound_movements
				for (i = 0; i < intersections_container<IntersectionsContainerType>().size(); i++)
				{
					Intersection_Interface* intersection = (Intersection_Interface*)intersections_container<IntersectionsContainerType>()[i];
					// skip intersections_container that do not have any inbound links
					if (intersection->inbound_links<LinksContainerType&>().size() == 0)
						continue;
					
					for (j = 0; j < intersection->inbound_links<LinksContainerType&>().size(); j++) 
					{
						InboundOutboundMovementsInterface* inboundOutboundMovements = (InboundOutboundMovementsInterface*)Allocate<InboundOutboundMovementsType>();
						Link_Interface* inboundLink = (Link_Interface*)intersection->inbound_links<LinksContainerType&>()[j];
						inboundOutboundMovements->inbound_link_reference<Link_Interface*>(inboundLink);
						TurnMovementsContainerType& outboundTurnMovements = inboundLink->outbound_turn_movements<TurnMovementsContainerType&>();
						cout<<"link id = " << inboundLink->uuid<int>()<<endl;
						cout<<"intersection id = " << intersection->uuid<int>()<<endl;
						for (k = 0; k < outboundTurnMovements.size(); k++)
						{
							TurnMovementInterface* outboundMovement = (TurnMovementInterface*)outboundTurnMovements[k];
							MovementInterface* movement = (MovementInterface*)Allocate<MovementType>();
							movement->movement_reference<Link_Interface*>(outboundMovement->outbound_link<Link_Interface*>());
							movement->movement_type<Intersection_Components::Types::Turn_Movement_Type_Keys>(outboundMovement->turn_movement_type<Intersection_Components::Types::Turn_Movement_Type_Keys>());
							movement->movement_type<Intersection_Components::Types::Turn_Movement_Rule_Keys>(outboundMovement->turn_movement_rule<Intersection_Components::Types::Turn_Movement_Rule_Keys>());
							inboundOutboundMovements->outbound_movements<MovementsContainerType&>().push_back(movement);
							outboundMovement->replicas_container<MovementsContainerType&>().push_back(movement);
						}
						intersection->inbound_outbound_movements<InboundOutboundMovementsContainerType&>().push_back(inboundOutboundMovements);
					}
				}

				//activity location

				typedef typename ThisType::activity_locations_container_type ActivityLocationsContainerType;
				typedef typename ThisType::activity_locations_container_element_type ActivityLocationType;
				typedef typename Activity_Location_Components::Interfaces::Activity_Location_Interface<ActivityLocationType,ThisType> ActivityLocationInterface;


				activity_locations_container<ActivityLocationsContainerType&>().clear();
				
				ActivityLocationInterface* activity_location_0 = (ActivityLocationInterface*)Allocate<ActivityLocationType>();
				ActivityLocationInterface* activity_location_1 = (ActivityLocationInterface*)Allocate<ActivityLocationType>();
				ActivityLocationInterface* activity_location_2 = (ActivityLocationInterface*)Allocate<ActivityLocationType>();

				i=0;
				activity_location_0->origin_links<LinksContainerType&>().push_back(link_0);
				activity_location_0->zone<int&>(i);
				activity_locations_container<ActivityLocationsContainerType&>().push_back(activity_location_0);

				i=1;
				activity_location_1->destination_links<LinksContainerType&>().push_back(link_3);
				activity_location_1->zone<int&>(i);
				activity_locations_container<ActivityLocationsContainerType&>().push_back(activity_location_1);

				i=2;
				activity_location_2->origin_links<LinksContainerType&>().push_back(link_5);
				activity_location_2->zone<int&>(i);
				activity_locations_container<ActivityLocationsContainerType&>().push_back(activity_location_2);
				

			}

			facet void printNetwork()
			{
				// for debug, we printout everything of the network
				
				typedef typename ThisType::intersections_container_type IntersectionsContainerType;
				typedef typename ThisType::intersections_container_element_type IntersectionType;
				typedef typename Intersection_Components::Interfaces::Intersection_Interface<IntersectionType,ThisType> Intersection_Interface;

				typedef typename ThisType::links_container_type LinksContainerType;
				typedef typename ThisType::links_container_element_type LinkType;
				typedef typename Link_Components::Interfaces::Link_Interface<LinkType,ThisType> Link_Interface;

				typedef typename ThisType::turn_movements_container_type TurnMovementsContainerType;
				typedef typename ThisType::turn_movements_element_type TurnMovementType;
				typedef typename Link_Components::Interfaces::Turn_Movement_Interface<TurnMovementType, NULLTYPE> TurnMovementInterface;

				typedef typename ThisType::outbound_inbound_movements_container_type OutboundInboundMovementsContainerType;
				typedef typename ThisType::outbound_inbound_movements_container_element_type OutboundInboundMovementsType;
				typedef typename Intersection_Components::Interfaces::Outbound_Inbound_Movements_Interface<OutboundInboundMovementsType,ThisType> OutboundInboundMovementsInterface;

				typedef typename ThisType::inbound_outbound_movements_container_type InboundOutboundMovementsContainerType;
				typedef typename ThisType::inbound_outbound_movements_container_element_type InboundOutboundMovementsType;
				typedef typename Intersection_Components::Interfaces::Inbound_Outbound_Movements_Interface<InboundOutboundMovementsType,ThisType> InboundOutboundMovementsInterface;

				typedef typename ThisType::movements_container_type MovementsContainerType;
				typedef typename ThisType::movements_container_element_type MovementType;
				typedef typename Intersection_Components::Interfaces::Movement_Interface<MovementType,ThisType> MovementInterface;

				typedef typename ThisType::activity_locations_container_type ActivityLocationsContainerType;
				typedef typename ThisType::activity_locations_container_element_type ActivityLocationType;
				typedef typename Activity_Location_Components::Interfaces::Activity_Location_Interface<ActivityLocationType,ThisType> ActivityLocationInterface;
				
				
				cout<<endl<<"***********************************Regular Network************************************************"<<endl;
				cout<<"all links"<<endl;
				int i, j, k;
				for (i = 0; i < (int)links_container<LinksContainerType&>().size(); i++)
				{
					Link_Interface* link = (Link_Interface*)links_container<LinksContainerType&>()[i];
					cout<<"\t link_"<<link->uuid<int>()<<endl;
					cout<<"\t\t upstream: " << link->upstream_intersection<Intersection_Interface*>()->uuid<int>()<<endl;
					cout<<"\t\t downstream: " << link->downstream_intersection<Intersection_Interface*>()->uuid<int>()<<endl;
					cout<<"\t\t num_lanes: " << link->num_lanes<int>()<<endl;
					cout<<"\t\t link_type: " << link->link_type<Link_Components::Types::Link_Type_Keys>()<<endl;
					cout<<"\t\t length: " << link->length<float>()<<endl;
					cout<<"\t\t speed_limit: " << link->speed_limit<float>()<<endl;
					cout<<"\t\t maximum_flow_rate: " << link->maximum_flow_rate<float>()<<endl;
					cout<<"\t\t backward_wave_speed: " << link->backward_wave_speed<float>()<<endl;
					cout<<"\t\t free_flow_speed: " << link->free_flow_speed<float>()<<endl;
					cout<<"\t\t jam_density: " << link->jam_density<float>()<<endl;
					cout<<"\t\t left_turn_bay_length: " << link->left_turn_bay_length<float>()<<endl;
					cout<<"\t\t right_turn_bay_length: " << link->right_turn_bay_length<float>()<<endl;
					cout<<"\t\t num_left_turn_bays: " << link->num_left_turn_bays<int>()<<endl;
					cout<<"\t\t num_right_turn_bays: " << link->num_right_turn_bays<int>()<<endl;
					cout<<"\t\t travel_time: " <<link->travel_time<float>()<<endl;
					cout<<"\t\t -----------------------------------"<<endl<<endl;
				}

				cout <<"-------------------------------------------------------------------------------------"<<endl;
				cout << "all intersections" << endl;
				for (i = 0; i < intersections_container<IntersectionsContainerType>().size(); i++)
				{
					Intersection_Interface* intersection = (Intersection_Interface*)intersections_container<IntersectionsContainerType>()[i];
					cout<<"\t intersection_"<<intersection->uuid<int>()<<endl;
					cout<<"\t\t x_position: "<<intersection->x_position<float>()<<endl;
					cout<<"\t\t y_position: "<<intersection->y_position<float>()<<endl;
					cout<<"\t\t intersection_type: "<<intersection->intersection_type<int>()<<endl;
					cout<<"\t\t all inbound_outbound_movements: "<<endl;
					InboundOutboundMovementsContainerType& inbound_outbound_movements_container = intersection->inbound_outbound_movements<InboundOutboundMovementsContainerType&>();
					InboundOutboundMovementsContainerType::iterator inbound_outbound_movements_itr;
					for (inbound_outbound_movements_itr=inbound_outbound_movements_container.begin(),j=0;inbound_outbound_movements_itr!=inbound_outbound_movements_container.end();inbound_outbound_movements_itr++,j++)
					{
						InboundOutboundMovementsInterface* inbound_outbound_movements = (InboundOutboundMovementsInterface*)(*inbound_outbound_movements_itr);
						cout<<"\t\t inbound_outbound_movements_"<<j<<endl;
						cout<<"\t\t\t inbound_link: "<<inbound_outbound_movements->inbound_link_reference<Link_Interface*>()->uuid<int>()<<endl;
						cout<<"\t\t\t outbound_movements: "<<endl;
						MovementsContainerType& outbound_movements_container = inbound_outbound_movements->outbound_movements<MovementsContainerType&>();
						MovementsContainerType::iterator outbound_movement_itr;
						for (outbound_movement_itr=outbound_movements_container.begin(),k=0;outbound_movement_itr!=outbound_movements_container.end();outbound_movement_itr++,k++)
						{
							MovementInterface* outbound_movement = (MovementInterface*)(*outbound_movement_itr);
							cout<<"\t\t\t\t outbound_movements_"<<k<<endl;
							cout<<"\t\t\t\t\t movement_reference: "<<outbound_movement->movement_reference<Link_Interface*>()->uuid<int>()<<endl;
							cout<<"\t\t\t\t\t movement_type: "<<outbound_movement->movement_type<int>()<<endl;
							cout<<"\t\t\t\t\t forward_link_turn_travel_time: "<<outbound_movement->forward_link_turn_travel_time<float>()<<endl;
						}
					}

					cout<<"\t\t all outbound_inbound_movements: " <<endl;
					OutboundInboundMovementsContainerType& outbound_inbound_movements_container = intersection->outbound_inbound_movements<OutboundInboundMovementsContainerType&>();
					OutboundInboundMovementsContainerType::iterator outbound_inbound_movements_itr;
					for (outbound_inbound_movements_itr=outbound_inbound_movements_container.begin(),j=0;outbound_inbound_movements_itr!=outbound_inbound_movements_container.end();outbound_inbound_movements_itr++,j++)
					{
						OutboundInboundMovementsInterface* outbound_inbound_movements = (OutboundInboundMovementsInterface*)(*outbound_inbound_movements_itr);
						cout<<"\t\t outbound_inbound_movements_"<<j<<endl;
						cout<<"\t\t\t outbound_link: "<<outbound_inbound_movements->outbound_link_reference<Link_Interface*>()->uuid<int>()<<endl;
						cout<<"\t\t\t inbound_movements: "<<endl;
						MovementsContainerType& inbound_movements_container = outbound_inbound_movements->inbound_movements<MovementsContainerType&>();
						MovementsContainerType::iterator inbound_movement_itr;
						for (inbound_movement_itr=inbound_movements_container.begin(),k=0;inbound_movement_itr!=inbound_movements_container.end();inbound_movement_itr++,k++)
						{
							MovementInterface* inbound_movement = (MovementInterface*)(*inbound_movement_itr);
							cout<<"\t\t\t\t inbound_movements_"<<k<<endl;
							cout<<"\t\t\t\t\t movement_reference: "<<inbound_movement->movement_reference<Link_Interface*>()->uuid<int>()<<endl;
							cout<<"\t\t\t\t\t movement_type: "<<inbound_movement->movement_type<int>()<<endl;
							cout<<"\t\t\t\t\t forward_link_turn_travel_time: "<<inbound_movement->forward_link_turn_travel_time<float>()<<endl;
						}
					}
					cout<<"\t\t --------------------------------"<<endl<<endl;
				}

				cout <<"***********************************************************************************"<<endl;
				cout << "all activity locations" << endl;

				ActivityLocationsContainerType::iterator activity_location_itr;
				for (activity_location_itr=activity_locations_container<ActivityLocationsContainerType&>().begin(),i=0; activity_location_itr!=activity_locations_container<ActivityLocationsContainerType&>().end(); activity_location_itr++,i++)
				{
					ActivityLocationInterface* activity_location = (ActivityLocationInterface*)(*activity_location_itr);
					cout<<"\t activity_location_"<<i<<endl;
					cout<<"\t\t zone: "<<activity_location->zone<int>()<<endl;
					cout<<"\t\t origin_links:"<<endl;
					LinksContainerType::iterator origin_link_itr;
					LinksContainerType& origin_links_container = activity_location->origin_links<LinksContainerType&>();
					for (origin_link_itr=origin_links_container.begin(); origin_link_itr!=origin_links_container.end(); origin_link_itr++)
					{
						cout<<"\t\t link_"<<((Link_Interface*)(*origin_link_itr))->uuid<int>()<<endl;
					}
					cout<<"\t\t destination_links:"<<endl;
					LinksContainerType::iterator destination_link_itr;
					LinksContainerType& destination_links_container = activity_location->destination_links<LinksContainerType&>();
					for (destination_link_itr=destination_links_container.begin(); destination_link_itr!=destination_links_container.end(); destination_link_itr++)
					{
						cout<<"\t\t link_"<<((Link_Interface*)(*destination_link_itr))->uuid<int>()<<endl;
					}
				}

			}

			facet void construct_network_cost()
			{
				typedef typename ThisType::links_container_type LinksContainerType;
				typedef typename ThisType::links_container_element_type LinkType;
				typedef typename Link_Components::Interfaces::Link_Interface<LinkType,ThisType> Link_Interface;

				typedef typename ThisType::movements_container_type MovementsContainerType;
				typedef typename ThisType::movements_container_element_type MovementType;
				typedef typename Intersection_Components::Interfaces::Movement_Interface<MovementType,ThisType> MovementInterface;
			
				for (int i=0; i < (int)links_container<LinksContainerType&>().size(); i++)
				{
					Link_Interface* link = (Link_Interface*)links_container<LinksContainerType&>()[i];
					float free_flow_speed = (float) ((5280.0) * link->free_flow_speed<float>()/3600.0); // feet per second
					float link_travel_time = float (link->length<float>() / free_flow_speed);
					
					max_free_flow_speed<float>(max(max_free_flow_speed<float>(),free_flow_speed));
					link_travel_time = max((float)1.0,link_travel_time);
					link->travel_time<float>(link_travel_time);
				}
				
				//turn penalty
				typedef typename ThisType::turn_movements_container_type TurnMovementsContainerType;
				typedef typename ThisType::turn_movement_type TurnMovementType;
				typedef typename Link_Components::Interfaces::Turn_Movement_Interface<TurnMovementType, NULLTYPE> TurnMovementInterface;
				//network_data.turn_travel_penalty_array.clear();
				//network_data.turn_travel_penalty_array.resize(network_data.network_turn_movement_size);
				//network_data.forward_link_turn_travel_time_array.clear();
				//network_data.forward_link_turn_travel_time_array.resize(network_data.network_turn_movement_size);
				for (int i=0; i < (int)turn_movements_container<TurnMovementsContainerType&>().size(); i++)
				{
					TurnMovementInterface* mvmt_itf=(TurnMovementInterface*)turn_movements_container<TurnMovementsContainerType&>()[i];
					Link_Interface* inboundLink = mvmt_itf->inbound_link<Link_Interface*>();
					Link_Interface* outboundLink = mvmt_itf->outbound_link<Link_Interface*>();

					float turn_travel_penalty = (float) (inboundLink->travel_time<float>() + outboundLink->travel_time<float>()*0.10);
					float forward_link_turn_travel_time = inboundLink->travel_time<float>() + turn_travel_penalty;

					mvmt_itf->turn_travel_penalty<float>(turn_travel_penalty);
					mvmt_itf->forward_link_turn_travel_time<float>(forward_link_turn_travel_time);
					for (int j = 0; j < (int)mvmt_itf->replicas_container<MovementsContainerType&>().size(); j++)
					{
						MovementInterface* replica = (MovementInterface*)mvmt_itf->replicas_container<MovementsContainerType&>()[j];
						replica->forward_link_turn_travel_time<float>(forward_link_turn_travel_time);
					}
				}
			}

			facet void construct_routable_network()
			{
				typedef typename ThisType::routable_networks_container_type RoutableNetworksContainerType;
				RoutableNetworksContainerType& routable_networks=routable_networks_container<RoutableNetworksContainerType&>();
				typedef typename ThisType::routable_network_type routable_network_type;
				typedef Routing_Components::Interfaces::Routable_Network_Interface<routable_network_type, NULLTYPE> Routable_Network_Interface;

				for(int i=0;i<num_threads;i++)
				{
					Routable_Network_Interface* routable_network = (Routable_Network_Interface*)Allocate<routable_network_type>();
					routable_network->read_routable_network_data<NULLTYPE>(PTHIS(ThisType));
					routable_networks.push_back(routable_network);
				}
			}
		};
	}
}