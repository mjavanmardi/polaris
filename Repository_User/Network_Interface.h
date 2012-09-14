#pragma once
#include "Activity_Location_Interface.h"
#include "Routing_Interface.h"
#include "Intersection_Interface.h"
#include "Traveler_Interface.h"

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
			facet_accessor(intersections);
			facet_accessor(links);
			facet_accessor(turn_movements);
			facet_accessor(activity_locations);

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
				typedef typename ThisType::intersections_element_type IntersectionType;
				typedef typename Intersection_Components::Interfaces::Intersection_Interface<IntersectionType,ThisType> Intersection_Interface;

				intersections<IntersectionContainerType&>().clear();
				
				//node
				//network_information::network_data_information::NodeData node_data;
				
				Intersection_Interface* intersection_0 = Allocate<IntersectionType>();
				Intersection_Interface* intersection_1 = Allocate<IntersectionType>();
				Intersection_Interface* intersection_2 = Allocate<IntersectionType>();
				Intersection_Interface* intersection_3 = Allocate<IntersectionType>();
				Intersection_Interface* intersection_4 = Allocate<IntersectionType>();
				Intersection_Interface* intersection_5 = Allocate<IntersectionType>();
				Intersection_Interface* intersection_6 = Allocate<IntersectionType>();

				i = 0;
				//node_data.node_index = i;
				//node_data.uuid = "1";
				
				intersection_0->x_position<float>(0.0);
				intersection_0->y_position<float>((float) 2.0*link_length);
				intersection_0->intersection_type<int>(Intersection_Components::Types::NO_CONTROL);
				intersections<IntersectionsContainerType&>().push_back(intersection_0);

				i = 1;

				//node_data.node_index = i;
				//node_data.uuid = "2";

				intersection_1=Allocate<IntersectionType>();
				intersection_1->x_position<float>(i * link_length);
				intersection_1->y_position<float>((float) 2.0*link_length);
				intersection_1->intersection_type<int>(Intersection_Components::Types::NO_CONTROL);
				intersections<IntersectionsContainerType&>().push_back(intersection_1);

				i = 2;

				//node_data.node_index = i;
				//node_data.uuid = "3";

				intersection_2=Allocate<IntersectionType>();
				intersection_2->x_position<float>(i * link_length);
				intersection_2->y_position<float>((float) 2.0*link_length);
				intersection_2->intersection_type<int>(Intersection_Components::Types::NO_CONTROL);
				intersections<IntersectionsContainerType&>().push_back(intersection_2);

				i = 3;

				//node_data.node_index = i;
				//node_data.uuid = "4";

				intersection_3=Allocate<IntersectionType>();
				intersection_3->x_position<float>(i * link_length);
				intersection_3->y_position<float>((float) 2.0*link_length);
				intersection_3->intersection_type<int>(Intersection_Components::Types::NO_CONTROL);
				intersection_3<IntersectionsContainerType&>().push_back(intersection_3);

				i = 4;

				//node_data.node_index = i;
				//node_data.uuid = "5";

				intersection_4=Allocate<IntersectionType>();
				intersection_4->x_position<float>(i * link_length);
				intersection_4->y_position<float>((float) 2.0*link_length);
				intersection_4->intersection_type<int>(Intersection_Components::Types::NO_CONTROL);
				intersections<IntersectionsContainerType&>().push_back(intersection_4);

				i = 5;

				//node_data.node_index = i;
				//node_data.uuid = "6";

				intersection_5=Allocate<IntersectionType>();
				intersection_5->x_position<float>((float) 1.0 * link_length);
				intersection_5->y_position<float>((float) 1.0 * link_length);
				intersection_5->intersection_type<int>(Intersection_Components::Types::NO_CONTROL);
				intersections<IntersectionsContainerType&>().push_back(intersection_5);

				i = 6;

				//node_data.node_index = i;
				//node_data.uuid = "7";

				intersection_6=Allocate<IntersectionType>();
				intersection_6->x_position<float>(0.0);
				intersection_6->y_position<float>(0.0);
				intersection_6->intersection_type<int>(Intersection_Components::Types::NO_CONTROL);
				intersections<IntersectionsContainerType&>().push_back(intersection_6);

				//link
				//network_data.link_data_array.clear();
				typedef typename ThisType::links_container_type LinksContainerType;
				typedef typename ThisType::links_element_type LinkType;
				typedef typename Link_Components::Interfaces::Link_Interface<LinkType,ThisType> Link_Interface;

				links<LinkContainerType&>().clear();
				
				Link_Interface* link_0 = Allocate<LinkType>();
				Link_Interface* link_1 = Allocate<LinkType>();
				Link_Interface* link_2 = Allocate<LinkType>();
				Link_Interface* link_3 = Allocate<LinkType>();
				Link_Interface* link_4 = Allocate<LinkType>();
				Link_Interface* link_5 = Allocate<LinkType>();

				i = 0;

				link_0->upstream_intersection<Intersection_Interface*>(intersection_0);
				link_0->downstream_intersection<Intersection_Interface*>(intersection_1);
				link_0->uuid<int>(1);
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
				links<LinksContainerType&>().push_back(link_0);

				max_free_flow_speed<float>(max(max_free_flow_speed<float>(),link_0->free_flow_speed<float>());
				
				i = 1;

				link_1->upstream_intersection<Intersection_Interface*>(intersection_1);
				link_1->downstream_intersection<Intersection_Interface*>(intersection_2);
				link_1->uuid<int>(2);
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
				links<LinksContainerType&>().push_back(link_0);

				max_free_flow_speed<float>(max(max_free_flow_speed<float>(),link_1->free_flow_speed<float>());

				i = 2;

				link_2->upstream_intersection<Intersection_Interface*>(intersection_2);
				link_2->downstream_intersection<Intersection_Interface*>(intersection_3);
				link_2->uuid<int>(3);
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
				links<LinksContainerType&>().push_back(link_2);

				max_free_flow_speed<float>(max(max_free_flow_speed<float>(),link_2->free_flow_speed<float>());

				i = 3;

				link_3->upstream_intersection<Intersection_Interface*>(intersection_3);
				link_3->downstream_intersection<Intersection_Interface*>(intersection_4);
				link_3->uuid<int>(4);
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
				links<LinksContainerType&>().push_back(link_3);

				max_free_flow_speed<float>(max(max_free_flow_speed<float>(),link_3->free_flow_speed<float>());


				i = 4;

				link_4->upstream_intersection<Intersection_Interface*>(intersection_5);
				link_4->downstream_intersection<Intersection_Interface*>(intersection_2);
				link_4->uuid<int>(5);
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
				links<LinksContainerType&>().push_back(link_4);

				max_free_flow_speed<float>(max(max_free_flow_speed<float>(),link_4->free_flow_speed<float>());

				i = 5;

				link_5->upstream_intersection<Intersection_Interface*>(intersection_6);
				link_5->downstream_intersection<Intersection_Interface*>(intersection_5);
				link_5->uuid<int>(6);
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
				links<LinksContainerType&>().push_back(link_5);

				max_free_flow_speed<float>(max(max_free_flow_speed<float>(),link_5->free_flow_speed<float>());

				//========================================================================================================
				//Create inbound/outbound links for each intersection
				//--------------------------------------------------------------------------------------------------------
				//
				for (i = 0; i < links<LinksContainerType>().size(); i++)
				{
					Link_Interface* link = links<LinksContainerType&>()[i];
					link->upstream_intersection<Intersection_Interface*>()->outboundLinks<LinksContainerType&>().pushback(link);
					link->downstream_intersection<Intersection_Interface*>()->inboundLinks<LinksContainerType&>().pushback(link);
				}

				//========================================================================================================
				//Create turn movements for each intersection
				//--------------------------------------------------------------------------------------------------------
				//
				
				//turn connectivities
				typedef typename ThisType::turn_movements_container_type TurnMovementsContainerType;
				typedef typename ThisType::turn_movements_element_type TurnMovementType;
				typedef typename Link_Components::Interfaces::Turn_Movement_Interface<TurnMovementType, NULLTYPE> TurnMovementInterface;
				
				TurnMovementInterface* turn_movement_0 = Allocate<TurnMovementType>();
				turn_movement_0->inbound_link_index<Link_Interface*>(links<LinksContainerType&>[0]);
				turn_movement_0->outbound_link_index<Link_Interface*>(links<LinksContainerType&>[1]);
				turn_movement_0->turn_movement_type<Intersection_Components::Types::Turn_Movement_Type_Keys> = Intersection_Components::Types::THROUGH_TURN;
				turn_movement_0->turn_movement_rule<Intersection_Components::Types::Turn_Movement_Rule_Keys> = Intersection_Components::Types::ALLOWED;
				turn_movements<TurnMovementsContainerType&>().push_back(turn_movement_0);

				TurnMovementInterface* turn_movement_1 = Allocate<TurnMovementType>();
				turn_movement_1->inbound_link_index<Link_Interface*>(links<LinksContainerType&>[1]);
				turn_movement_1->outbound_link_index<Link_Interface*>(links<LinksContainerType&>[2]);
				turn_movement_1->turn_movement_type<Intersection_Components::Types::Turn_Movement_Type_Keys> = Intersection_Components::Types::THROUGH_TURN;
				turn_movement_1->turn_movement_rule<Intersection_Components::Types::Turn_Movement_Rule_Keys> = Intersection_Components::Types::ALLOWED;
				turn_movements<TurnMovementsContainerType&>().push_back(turn_movement_1);

				TurnMovementInterface* turn_movement_2 = Allocate<TurnMovementType>();
				turn_movement_2->inbound_link_index<Link_Interface*>(links<LinksContainerType&>[2]);
				turn_movement_2->outbound_link_index<Link_Interface*>(links<LinksContainerType&>[3]);
				turn_movement_2->turn_movement_type<Intersection_Components::Types::Turn_Movement_Type_Keys> = Intersection_Components::Types::THROUGH_TURN;
				turn_movement_2->turn_movement_rule<Intersection_Components::Types::Turn_Movement_Rule_Keys> = Intersection_Components::Types::ALLOWED;
				turn_movements<TurnMovementsContainerType&>().push_back(turn_movement_2);

				TurnMovementInterface* turn_movement_3 = Allocate<TurnMovementType>();
				turn_movement_3->inbound_link_index<Link_Interface*>(links<LinksContainerType&>[4]);
				turn_movement_3->outbound_link_index<Link_Interface*>(links<LinksContainerType&>[2]);
				turn_movement_3->turn_movement_type<Intersection_Components::Types::Turn_Movement_Type_Keys> = Intersection_Components::Types::RIGHT_TURN;
				turn_movement_3->turn_movement_rule<Intersection_Components::Types::Turn_Movement_Rule_Keys> = Intersection_Components::Types::ALLOWED;
				turn_movements<TurnMovementsContainerType&>().push_back(turn_movement_3);

				
				TurnMovementInterface* turn_movement_4 = Allocate<TurnMovementType>();
				turn_movement_4->inbound_link_index<Link_Interface*>(links<LinksContainerType&>[5]);
				turn_movement_4->outbound_link_index<Link_Interface*>(links<LinksContainerType&>[4]);
				turn_movement_4->turn_movement_type<Intersection_Components::Types::Turn_Movement_Type_Keys> = Intersection_Components::Types::THROUGH_TURN;
				turn_movement_4->turn_movement_rule<Intersection_Components::Types::Turn_Movement_Rule_Keys> = Intersection_Components::Types::ALLOWED;
				turn_movements<TurnMovementsContainerType&>().push_back(turn_movement_4);


				for (i = 0; i < turn_movements<TurnMovementsContainerType&>().size(); i++)
				{
					//outbound turn movement of an inbound link
					LinkInterface* inbound_link = turn_movements<TurnMovementsContainerType&>()[i].inbound_link<LinkInterface*>();
					inbound_link->outbound_turn_movements<TurnMovementsContainerType&>().push_back(turn_movements<TurnMovementsContainerType&>()[i].outbound_link<LinkInterface*>());
		
					//inbound turn movement of an outbound link
					LinkInterface* outbound_link = turn_movements<TurnMovementsContainerType&>()[i].outbound_link<LinkInterface*>();
					outbound_link->inbound_turn_movements<TurnMovementsContainerType&>().push_back(turn_movements<TurnMovementsContainerType&>()[i].inbound_link<LinkInterface*>());
				}

				
				typedef typename ThisType::outbound_inbound_movements_container_type OutboundInboundMovementsContainerType;
				typedef typename ThisType::outbound_inbound_movements_element_type OutboundInboundMovementsType;
				typedef typename Intersection_Components::Interfaces::Outbound_Inbound_Movements_Interface<OutboundInboundMovementsType,ThisType> OutboundInboundMovementsInterface;

				typedef typename ThisType::outbound_inbound_movements_container_type InboundOutboundMovementsContainerType
				typedef typename ThisType::inbound_outbound_movements_type InboundOutboundMovementsType;
				typedef typename Intersection_Components::Interfaces::Inbound_Outbound_Movements_Interface<InboundOutboundMovementsType,ThisType> InboundOutboundMovementsInterface;

				typedef typename ThisType::movements_container_type MovementsContainerType;
				typedef typename ThisType::movements_element_type MovementType;
				typedef typename Intersection_Components::Interfaces::Movement_Interface<MovementType,ThisType> MovementInterface;

				int j, k;

				// configure outbound_inbound_movements
				for (i = 0; i < intersections<IntersectionsContainerType>().size(); i++)
				{
					Intersection_Interface* intersection = intersections<IntersectionsContainerType>()[i];
					// skip intersections that do not have any outbound links
					if (intersection->outboundLinks<LinksContainerType&>().size() == 0)
						continue;
					
					for (j = 0; j < intersection->outboundLinks<LinksContainerType&>().size(); j++) 
					{
						OutboundInboundMovementsInterface* outboundInboundMovements = Allocate<OutboundInboundMovementsType>();
						Link_Interface* outboundLink = intersection->outboundLinks<LinksContainerType&>()[i];
						outboundInboundMovements->outbound_movement_reference<Link_Interface*>(outboundLink);
						TurnMovementsContainerType& inboundTurnMovements = outboundLink->inbound_turn_movements<TurnMovementsContainerType&>();
						for (k = 0; k < inboundTurnMovements.size(); k++)
						{
							Turn_Movement_Interface* inboundMovement = inboundTurnMovements[k];

							MovementInterface* movement = Allocate<MovementType>();
							movement->movement_reference<Link_Interface*>(inboundMovement->inbound_link<Link_Interface*>());
							movement->movement_type<Intersection_Components::Types::Turn_Movement_Type_Keys>(inboundMovement->turn_movement_type<Intersection_Components::Types::Turn_Movement_Type_Keys>());
							movement->movement_type<Intersection_Components::Types::Turn_Movement_Rule_Keys>(inboundMovement->turn_movement_rule<Intersection_Components::Types::Turn_Movement_Rule_Keys>());
							outboundInboundMovements->inbound_movements<MovementsContainerType&>().push_back(movement);
						}
						intersection->outbound_inbound_movements<OutboundInboundMovementsContainerType>.push_back(outboundInboundMovements);
					}
				}
						
				// configure inbound_outbound_movements
				for (i = 0; i < intersections<IntersectionsContainerType>().size(); i++)
				{
					Intersection_Interface* intersection = intersections<IntersectionsContainerType>()[i];
					// skip intersections that do not have any inbound links
					if (intersection->inboundLinks<LinksContainerType&>().size() == 0)
						continue;
					
					for (j = 0; j < intersection->inboundLinks<LinksContainerType&>().size(); j++) 
					{
						InboundOutboundMovementsInterface* inboundOutboundMovements = Allocate<InboundOutboundMovementsType>();
						Link_Interface* inboundLink = intersection->inboundLinks<LinksContainerType&>()[i];
						inboundOutboundMovements->inbound_movement_reference<Link_Interface*>(inboundLink);
						TurnMovementsContainerType& outboundTurnMovements = inboundLink->outbound_turn_movements<TurnMovementsContainerType&>();
						for (k = 0; k < outboundTurnMovements.size(); k++)
						{
							Turn_Movement_Interface* outboundMovement = outboundTurnMovements[k];

							MovementInterface* movement = Allocate<MovementType>();
							movement->movement_reference<Link_Interface*>(outboundMovement->outbound_link<Link_Interface*>());
							movement->movement_type<Intersection_Components::Types::Turn_Movement_Type_Keys>(outboundMovement->turn_movement_type<Intersection_Components::Types::Turn_Movement_Type_Keys>());
							movement->movement_type<Intersection_Components::Types::Turn_Movement_Rule_Keys>(outboundMovement->turn_movement_rule<Intersection_Components::Types::Turn_Movement_Rule_Keys>());
							inboundOutboundMovements->outbound_movements<MovementsContainerType&>().push_back(movement);
						}
						intersection->inbound_outbound_movements<OutboundInboundMovementsContainerType>.push_back(inboundInboundMovements);
					}
				}
						
				////intersection_1; outboundInboundMovements={{outbound=link_1, inbound={link_0}}}
				//
				//outboundInboundMovementsForIntersection1->outbound_movement_reference<Link_Interface*>(link_1);
				//MovementInterface* inbound_link0_for_outbound_link1 = Allocate<MovementType>();
				//inbound_link0_for_outbound_link1->movement_type<Intersection_Components::Types::Turn_Movement_Type_Keys>(Intersection_Components::Types::THROUGH_TURN);
				//inbound_link0_for_outbound_link1->movement_type<Intersection_Components::Types::Turn_Movement_Rule_Keys>(Intersection_Components::Types::ALLOWED);
				//outboundInboundMovementsForIntersection0->inbound_movements<InboundMovementsType&>().push_back(inbound_link0_for_outbound_link1);
				//intersection_1->inbound_outbound_movements<OutboundInboundMovementsType&>().push_back(outboundInboundMovementsForIntersection1);

				//
				////intersection_2; outboundInboundMovements={{outbound=link_2, inbound={link_1, link_4}}}
				//OutboundInboundMovementsInterface* outboundInboundMovementsForIntersection2 = Allocate<OutboundInboundMovementsType>();
				//outboundInboundMovementsForIntersection2->outbound_movement_reference<Link_Interface*>(link_2);
				//
				//MovementInterface* inbound_link1_for_outbound_link2 = Allocate<MovementType>();
				//inbound_link1_for_outbound_link2->movement_type<Intersection_Components::Types::Turn_Movement_Type_Keys>(Intersection_Components::Types::THROUGH_TURN);
				//inbound_link1_for_outbound_link2->movement_type<Intersection_Components::Types::Turn_Movement_Rule_Keys>(Intersection_Components::Types::ALLOWED);
				//outboundInboundMovementsForIntersection1->inbound_movements<InboundMovementsType&>().push_back(inbound_link_1_for_outbound_link2);

				//MovementInterface* inbound_link4_for_outbound_link2 = Allocate<MovementType>();
				//inbound_link4_for_outbound_link2->movement_type<Intersection_Components::Types::Turn_Movement_Type_Keys>(Intersection_Components::Types::RIGHT_TURN);
				//inbound_link4_for_outbound_link2->movement_type<Intersection_Components::Types::Turn_Movement_Rule_Keys>(Intersection_Components::Types::ALLOWED);
				//outboundInboundMovementsForIntersection1->inbound_movements<InboundMovementsType&>().push_back(inbound_link4_for_outbound_link2);
				//
				//intersection_2->inbound_outbound_movements<OutboundInboundMovementsType&>().push_back(outboundInboundMovementsForIntersection2);

				////intersection_3; outboundInboundMovements={{outbound=link_3, inbound={link_2}}}
				//OutboundInboundMovementsInterface* outboundInboundMovementsForIntersection3 = Allocate<OutboundInboundMovementsType>();
				//outboundInboundMovementsForIntersection3->outbound_movement_reference<Link_Interface*>(link_3);
				//MovementInterface* inbound_link2_for_outbound_link3 = Allocate<MovementType>();
				//inbound_link2_for_outbound_link3->movement_type<Intersection_Components::Types::Turn_Movement_Type_Keys>(Intersection_Components::Types::THROUGH_TURN);
				//inbound_link2_for_outbound_link3->movement_type<Intersection_Components::Types::Turn_Movement_Rule_Keys>(Intersection_Components::Types::ALLOWED);
				//outboundInboundMovementsForIntersection0->inbound_movements<InboundMovementsType&>().push_back(inbound_link2_for_outbound_link3);
				//intersection_3->inbound_outbound_movements<OutboundInboundMovementsType&>().push_back(outboundInboundMovementsForIntersection3);
				//
				////intersection_5; outboundInboundMovements={{outbound=link_4, inbound={link_5}}}
				//OutboundInboundMovementsInterface* outboundInboundMovementsForIntersection5 = Allocate<OutboundInboundMovementsType>();
				//outboundInboundMovementsForIntersection5->outbound_movement_reference<Link_Interface*>(link_5);
				//MovementInterface* inbound_link4_for_outbound_link5 = Allocate<MovementType>();
				//inbound_link4_for_outbound_link5->movement_type<Intersection_Components::Types::Turn_Movement_Type_Keys>(Intersection_Components::Types::THROUGH_TURN);
				//inbound_link4_for_outbound_link5->movement_type<Intersection_Components::Types::Turn_Movement_Rule_Keys>(Intersection_Components::Types::ALLOWED);
				//outboundInboundMovementsForIntersection5->inbound_movements<InboundMovementsType&>().push_back(inbound_link5_for_outbound_link4);
				//intersection_5->inbound_outbound_movements<OutboundInboundMovementsType&>().push_back(outboundInboundMovementsForIntersection5);

			}

			facet void construct_network_cost()
			{
				typedef typename ThisType::links_container_type LinksContainerType;
				typedef typename ThisType::links_element_type LinkType;
				typedef typename Link_Components::Interfaces::Link_Interface<LinkType,ThisType> Link_Interface;
			
				for (int i=0; i < links<LinksContainerType&>().size(); i++)
				{
					LinkInterface* link = links<LinksContainerType&>()[i];
					float free_flow_speed = (float) ((5280.0) * link->free_flow_speed<float>()/3600.0); // feet per second
					float link_travel_time = float (link->length<float>() / free_flow_speed);
		
					max_free_flow_speed = max(max_free_flow_speed,free_flow_speed);
					link_travel_time = max((float)1.0,link_travel_time);
					link->travel_time<float>(link_travel_time);
				}
	
				//turn penalty
				typedef typename ThisType::turn_movements_container_type TurnMovementsContainerType;
				typedef typename ThisType::turn_movement_type TurnMovementType;
				typedef typename Link_Components::Interfaces::Turn_Movement_Interface<TurnMovementType, NULLTYPE> TurnMovementInterface;
				network_data.turn_travel_penalty_array.clear();
				network_data.turn_travel_penalty_array.resize(network_data.network_turn_movement_size);
				network_data.forward_link_turn_travel_time_array.clear();
				network_data.forward_link_turn_travel_time_array.resize(network_data.network_turn_movement_size);
				for (int i=0; i < turn_movements<TurnMovementsContainerType&>().size(); i++)
				{
					LinkInterface* inboundLink = turn_movements<TurnMovementsContainerType&>()[i].inbound_link<LinkInterface*>();
					LinkInterface* outboundLink = turn_movements<TurnMovementsContainerType&>()[i].outbound_link<LinkInterface*>();

					float turn_travel_penalty = (float) ((inboundLink->travel_time<float>() + outboundLink->travel_time<float>()*0.10);
					float forward_link_turn_travel_time = inboundLink->travel_time<float>() + turn_travel_penalty;

					turn_movements<TurnMovementsContainerType&>()[i].turn_travel_penalty<float>(turn_travel_penalty);
					turn_movements<TurnMovementsContainerType&>()[i].forward_link_turn_travel_time<float>(forward_link_turn_travel_time);
				}
			}
		};
	}
}