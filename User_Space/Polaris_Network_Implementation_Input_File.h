#pragma once
#include "Polaris_Network_Implementation.h"
namespace Network_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{
		feature_implementation_definition void Polaris_Network_Implementation<MasterType,ParentType,InheritanceList>::read_network_data(network_models::network_information::network_data_information::NetworkData& network_data)
		{
			read_intersection_data<ComponentType,CallerType,TargetType>(network_data);
			read_link_data<ComponentType,CallerType,TargetType>(network_data);
			read_turn_movement_data<ComponentType,CallerType,TargetType>(network_data);
			read_activity_location_data<ComponentType,CallerType,TargetType>(network_data);
			read_zone_data<ComponentType,CallerType,TargetType>(network_data);
		}

		feature_implementation_definition void Polaris_Network_Implementation<MasterType,ParentType,InheritanceList>::read_intersection_data(network_models::network_information::network_data_information::NetworkData& network_data)
		{
			define_container_and_value_interface_unqualified_container(_Intersections_Container_Interface, _Intersection_Interface, type_of(intersections_container), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Intersection_Prototype, ComponentType);
			_Intersection_Interface* intersection;
			for (int i = 0; i < network_data.network_node_size; i++) {
				intersection = (_Intersection_Interface*)Allocate<typename _Intersection_Interface::Component_Type>();
				intersection->template uuid<int>(network_data.node_data_array[i].uuid);
				intersection->template internal_id<int>(i);
				intersection->template x_position<float>(network_data.node_data_array[i].x);
				intersection->template y_position<float>(network_data.node_data_array[i].y);
				intersection->template intersection_type<int>(network_data.node_data_array[i].node_type);
				intersections_container<ComponentType,CallerType,_Intersections_Container_Interface&>().push_back(intersection);
			}
		}

		feature_implementation_definition void Polaris_Network_Implementation<MasterType,ParentType,InheritanceList>::read_link_data(network_models::network_information::network_data_information::NetworkData& network_data)
		{
			define_container_and_value_interface_unqualified_container(_Intersections_Container_Interface, _Intersection_Interface, type_of(intersections_container), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Intersection_Prototype, ComponentType);
			define_container_and_value_interface_unqualified_container(_Links_Container_Interface, _Link_Interface, type_of(links_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
			_Link_Interface* link;
			for (int i = 0; i < network_data.network_link_size; i++) {
				link = (_Link_Interface*)Allocate<typename _Link_Interface::Component_Type>();
				int upstream_intersection_id = network_data.link_data_array[i].unode_index;
				_Intersection_Interface* upstream_intersection = (_Intersection_Interface*)_intersections_container.at(upstream_intersection_id);
				link->template upstream_intersection<_Intersection_Interface*>(upstream_intersection);

				int downstream_intersection_id = network_data.link_data_array[i].dnode_index;
				_Intersection_Interface* downstream_intersection = (_Intersection_Interface*)_intersections_container.at(downstream_intersection_id);
				link->template downstream_intersection<_Intersection_Interface*>(downstream_intersection);

				network_models::network_information::network_data_information::LinkData& raw_link = network_data.link_data_array[i];
				link->template uuid<int>(raw_link.uuid);
				link->template internal_id<int>(i);
				link->template num_lanes<int>(raw_link.num_lanes);
				link->template link_type<int>(raw_link.link_type);
				link->template length<float>(raw_link.length);
				link->template speed_limit<float>(raw_link.speed_limit);
				link->template maximum_flow_rate<float>(raw_link.maximum_flow_rate);
				link->template backward_wave_speed<float>(raw_link.backward_wave_speed);
				link->template free_flow_speed<float>(raw_link.free_flow_speed);
				link->template jam_density<float>(raw_link.jam_density);
				link->template left_turn_bay_length<float>(raw_link.left_turn_bay_length);
				link->template right_turn_bay_length<float>(raw_link.right_turn_bay_length);
				link->template num_left_turn_bays<int>(raw_link.num_left_turn_bays);
				link->template num_right_turn_bays<int>(raw_link.num_right_turn_bays);

				link->template upstream_intersection<_Intersection_Interface*>()->template outbound_links<_Links_Container_Interface&>().push_back(link);
				link->template downstream_intersection<_Intersection_Interface*>()->template inbound_links<_Links_Container_Interface&>().push_back(link);

				links_container<ComponentType,CallerType,_Links_Container_Interface&>().push_back(link);
			}
		}

		feature_implementation_definition void Polaris_Network_Implementation<MasterType,ParentType,InheritanceList>::read_turn_movement_data(network_models::network_information::network_data_information::NetworkData& network_data)
		{
			define_container_and_value_interface_unqualified_container(_Intersections_Container_Interface, _Intersection_Interface, type_of(intersections_container), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Intersection_Prototype, ComponentType);
			define_container_and_value_interface_unqualified_container(_Links_Container_Interface, _Link_Interface, type_of(links_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
			define_container_and_value_interface_unqualified_container(_Turn_Movements_Container_Interface, _Turn_Movement_Interface, type_of(turn_movements_container), Random_Access_Sequence_Prototype, Turn_Movement_Components::Prototypes::Movement_Prototype, ComponentType);
			define_container_and_value_interface(_Outbound_Inbound_Movements_Container_Interface, _Outbound_Inbound_Movements_Interface, typename _Intersection_Interface::get_type_of(outbound_inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Outbound_Inbound_Movements_Prototype, ComponentType);			
			define_container_and_value_interface(_Inbound_Outbound_Movements_Container_Interface, _Inbound_Outbound_Movements_Interface, typename _Intersection_Interface::get_type_of(inbound_outbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Inbound_Outbound_Movements_Prototype, ComponentType);
			_Turn_Movement_Interface* turn_movement;
			int i;
			for (i = 0; i < network_data.network_turn_movement_size; i++)
			{
				turn_movement = (_Turn_Movement_Interface*)Allocate<typename _Turn_Movement_Interface::Component_Type>();
				network_models::network_information::network_data_information::TurnMovementData& raw_turn_movement = network_data.turn_movement_data_array[i];
				turn_movement->template inbound_link<_Link_Interface*>((_Link_Interface*)_links_container.at(raw_turn_movement.inbound_link_index));
				turn_movement->template outbound_link<_Link_Interface*>((_Link_Interface*)_links_container.at(raw_turn_movement.outbound_link_index));
				turn_movement->template movement_type<int>(raw_turn_movement.turn_movement_type);
				turn_movement->template movement_rule<int>(raw_turn_movement.turn_movement_rule);
				turn_movement->template uuid<int>(raw_turn_movement.uuid);
				turn_movement->template internal_id<int>(i);
				//// assign the detector
				//turn_movement_1->template detector<Detector_Interface*>(lane->template Detector_Thru<Detector_Interface*>());
				turn_movements_container<ComponentType,CallerType,_Turn_Movements_Container_Interface&>().push_back(turn_movement);
			}

			typename _Turn_Movements_Container_Interface::iterator turn_movements_itr;
			for (turn_movements_itr = _turn_movements_container.begin(); turn_movements_itr != _turn_movements_container.end(); turn_movements_itr++)
			{
				_Turn_Movement_Interface* turn_mvmt=(_Turn_Movement_Interface*)(*turn_movements_itr);
				//outbound turn movement of an inbound link
				_Link_Interface* inbound_link = turn_mvmt->template inbound_link<_Link_Interface*>();
				inbound_link->template outbound_turn_movements<_Turn_Movements_Container_Interface&>().push_back(turn_mvmt);
				//inbound turn movement of an outbound link
				_Link_Interface* outbound_link = turn_mvmt->template outbound_link<_Link_Interface*>();
				outbound_link->template inbound_turn_movements<_Turn_Movements_Container_Interface&>().push_back(turn_mvmt);
			}

			// configure outbound_inbound_movements
			typename _Intersections_Container_Interface::iterator intersections_itr;
			for (intersections_itr = _intersections_container.begin(); intersections_itr != _intersections_container.end(); intersections_itr++)
			{
				_Intersection_Interface* intersection = (_Intersection_Interface*)(*intersections_itr);
				_Links_Container_Interface& outbound_links = intersection->template outbound_links<_Links_Container_Interface&>();
				// skip intersections_container that do not have any outbound links
				if (outbound_links.size() == 0)
					continue;
				typename _Links_Container_Interface::iterator links_itr;
				for (links_itr = outbound_links.begin(); links_itr != outbound_links.end(); links_itr++)
				{
					_Link_Interface* outboundLink = (_Link_Interface*)(*links_itr);
					_Outbound_Inbound_Movements_Interface* outboundInboundMovements = (_Outbound_Inbound_Movements_Interface*)Allocate<typename _Outbound_Inbound_Movements_Interface::Component_Type>();
					outboundInboundMovements->template outbound_link_reference<_Link_Interface*>(outboundLink);
					_Turn_Movements_Container_Interface& inboundTurnMovements = outboundLink->template inbound_turn_movements<_Turn_Movements_Container_Interface&>();
					for (int k = 0; k < (int)inboundTurnMovements.size(); k++)
					{
						outboundInboundMovements->template inbound_movements<_Turn_Movements_Container_Interface&>().push_back(inboundTurnMovements[k]);
					}
					intersection->template outbound_inbound_movements<_Outbound_Inbound_Movements_Container_Interface&>().push_back(outboundInboundMovements);
				}
			}

			// configure inbound_outbound_movements
			for (i = 0; i < (int)_intersections_container.size(); i++)
			{
				_Intersection_Interface* intersection = (_Intersection_Interface*)_intersections_container[i];
				// skip intersections_container that do not have any inbound links
				if (intersection->template inbound_links<_Links_Container_Interface&>().size() == 0)
					continue;
				for (int j = 0; j < (int)intersection->template inbound_links<_Links_Container_Interface&>().size(); j++) 
				{
					_Inbound_Outbound_Movements_Interface* inboundOutboundMovements = (_Inbound_Outbound_Movements_Interface*)Allocate<typename _Inbound_Outbound_Movements_Interface::Component_Type>();
					_Link_Interface* inboundLink = (_Link_Interface*)intersection->template inbound_links<_Links_Container_Interface&>()[j];
					inboundOutboundMovements->template inbound_link_reference<_Link_Interface*>(inboundLink);
					_Turn_Movements_Container_Interface& outboundTurnMovements = inboundLink->template outbound_turn_movements<_Turn_Movements_Container_Interface&>();
					for (int k = 0; k < (int)outboundTurnMovements.size(); k++)
					{
						inboundOutboundMovements->template outbound_movements<_Turn_Movements_Container_Interface&>().push_back(outboundTurnMovements[k]);
					}
					intersection->template inbound_outbound_movements<_Inbound_Outbound_Movements_Container_Interface&>().push_back(inboundOutboundMovements);
				}
			}
		}

		feature_implementation_definition void Polaris_Network_Implementation<MasterType,ParentType,InheritanceList>::read_activity_location_data(network_models::network_information::network_data_information::NetworkData& network_data)
		{
			define_container_and_value_interface_unqualified_container(_Links_Container_Interface, _Link_Interface, type_of(links_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
			define_container_and_value_interface_unqualified_container(_Activity_Locations_Container_Interface, _Activity_Location_Interface, type_of(activity_locations_container), Random_Access_Sequence_Prototype, Activity_Location_Components::Prototypes::Activity_Location_Prototype, ComponentType);
			_Activity_Location_Interface* activity_location;
			for (int i = 0; i < network_data.network_activity_location_size; i++) 
			{
				activity_location = (_Activity_Location_Interface*)Allocate<typename _Activity_Location_Interface::Component_Type>();
				network_models::network_information::network_data_information::ActivityLocationData& raw_activity_location = network_data.activity_location_data_array[i];

				int j;
				int link_index;
				// initialize origins
				for (j = 0; j < raw_activity_location.num_origin_links; j++) 
				{
					link_index = raw_activity_location.origin_link_index_array[j];
					activity_location->template origin_links<_Links_Container_Interface&>().push_back((_Link_Interface*)_links_container.at(link_index));
					activity_location->template origin_link_choice_cdfs<vector<float>&>().push_back(raw_activity_location.origin_link_choice_cdf_array[j]);
				}
				// initialize destinations
				for (j = 0; j < raw_activity_location.num_destination_links; j++) 
				{
					link_index = raw_activity_location.destination_link_index_array[j];
					activity_location->template destination_links<_Links_Container_Interface&>().push_back((_Link_Interface*)_links_container.at(link_index));
					activity_location->template destination_link_choice_cdfs<vector<float>&>().push_back(raw_activity_location.destination_link_choice_cdf_array[j]);
				}
				activity_location->template uuid<int>(raw_activity_location.uuid);
				activity_location->template internal_id<int>(i);
				activity_locations_container<ComponentType,CallerType,_Activity_Locations_Container_Interface&>().push_back(activity_location);
			}
		}

		feature_implementation_definition void Polaris_Network_Implementation<MasterType,ParentType,InheritanceList>::read_zone_data(network_models::network_information::network_data_information::NetworkData& network_data)
		{
			define_container_and_value_interface_unqualified_container(_Zones_Container_Interface, _Zone_Interface, type_of(zones_container), Associative_Container_Prototype, Zone_Components::Prototypes::Zone_Prototype, ComponentType);
			define_container_and_value_interface_unqualified_container(_Activity_Locations_Container_Interface, _Activity_Location_Interface, type_of(activity_locations_container), Random_Access_Sequence_Prototype, Activity_Location_Components::Prototypes::Activity_Location_Prototype, ComponentType);
			typename _Zones_Container_Interface::iterator zone_itr;

			// initialzie zone hash_map
			//_zones_container.clear();
			_zones_container.set_empty_key(-1);
			_zones_container.set_deleted_key(-2);

			_Zone_Interface* zone;

			for (int i = 0; i < network_data.network_zone_size; i++)
			{
				zone = (_Zone_Interface*)Allocate<typename _Zone_Interface::Component_Type>();
				network_models::network_information::network_data_information::ZoneData& raw_zone = network_data.zone_data_array[i];
				int j;
				int activity_location_index;
				// initialize origins
				for (j = 0; j < raw_zone.num_origin_activity_locations; j++) 
				{
					activity_location_index = raw_zone.origin_activity_location_index_array[j];
					zone->template origin_activity_locations<_Activity_Locations_Container_Interface&>().push_back((_Activity_Location_Interface*)_activity_locations_container.at(activity_location_index));
					zone->template origin_activity_location_choice_cdfs<vector<float>&>().push_back(raw_zone.origin_activity_location_choice_cdf_array[j]);
				}
				// initialize destinations
				for (j = 0; j < raw_zone.num_destination_activity_locations; j++) 
				{
					activity_location_index = raw_zone.destination_activity_location_index_array[j];
					zone->template destination_activity_locations<_Activity_Locations_Container_Interface&>().push_back((_Activity_Location_Interface*)_activity_locations_container.at(activity_location_index));
					zone->template destination_activity_location_choice_cdfs<vector<float>&>().push_back(raw_zone.destination_activity_location_choice_cdf_array[j]);
				}
					
				zone->template uuid<int>(raw_zone.uuid);
				zone->template internal_id<int>(i);
				zone->template X<double>(raw_zone.x);
				zone->template Y<double>(raw_zone.y);
				zone->template population<int>(raw_zone.population);

				zones_container<ComponentType,CallerType,_Zones_Container_Interface&>().insert(pair<int,_Zone_Interface*>(zone->template uuid<int>(), zone));
				//zones_container<ComponentType,CallerType,_Zones_Container_Interface&>().push_back(zone);
			}
			for (int i = 0; i < network_data.network_activity_location_size; i++)
			{
				network_models::network_information::network_data_information::ActivityLocationData& raw_activity_location = network_data.activity_location_data_array[i];
				_Activity_Location_Interface* activity_location = (_Activity_Location_Interface*)_activity_locations_container[i];
				if ((zone_itr = _zones_container.find(raw_activity_location.zone_index)) != _zones_container.end())
				{
					activity_location->template zone<_Zone_Interface*>(zone_itr->second);
					zone = (_Zone_Interface*)zone_itr->second;
					zone->origin_activity_locations<_Activity_Locations_Container_Interface&>().push_back(activity_location);
					zone->destination_activity_locations<_Activity_Locations_Container_Interface&>().push_back(activity_location);
				}
				else
				{
					THROW_EXCEPTION("ERROR: zone id: " << raw_activity_location.zone_index << ", from activity location: " << activity_location->uuid<int>() << ", was not found.");
				}
			}
		}

	}
}