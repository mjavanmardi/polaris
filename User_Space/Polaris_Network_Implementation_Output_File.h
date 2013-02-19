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
		feature_implementation_definition void Polaris_Network_Implementation<MasterType,ParentType,InheritanceList>::write_network_data(network_models::network_information::network_data_information::NetworkData& network_data)
		{
			network_models::network_information::network_data_information::network_data_initialization(network_data);
			write_intersection_data<ComponentType,CallerType,TargetType>(network_data);
			write_link_data<ComponentType,CallerType,TargetType>(network_data);
			write_turn_movement_data<ComponentType,CallerType,TargetType>(network_data);
			write_activity_location_data<ComponentType,CallerType,TargetType>(network_data);
			write_zone_data<ComponentType,CallerType,TargetType>(network_data);
		}

		feature_implementation_definition void Polaris_Network_Implementation<MasterType,ParentType,InheritanceList>::write_intersection_data(network_models::network_information::network_data_information::NetworkData& network_data)
		{
			define_container_and_value_interface_unqualified_container(_Intersections_Container_Interface, _Intersection_Interface, type_of(intersections_container), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Intersection_Prototype, ComponentType);
			typename _Intersections_Container_Interface::iterator intersection_itr;
			cout << "writing intersections" << endl;
			for (intersection_itr = _intersections_container.begin(); intersection_itr != _intersections_container.end(); intersection_itr++)
			{
				_Intersection_Interface* intersection = (_Intersection_Interface*)(*intersection_itr);
				network_models::network_information::network_data_information::NodeData node_data;
				node_data.uuid = intersection->template uuid<int>();
				node_data.node_index = intersection->template internal_id<int>();
				node_data.x = intersection->template x_position<float>();
				node_data.y = intersection->template y_position<float>();
				node_data.node_type = intersection->template intersection_type<network_models::network_information::network_data_information::Node_Type_Keys>();
				network_data.node_data_array.push_back(node_data);
			}
			network_data.network_node_size = (int)network_data.node_data_array.size();
		}

		feature_implementation_definition void Polaris_Network_Implementation<MasterType,ParentType,InheritanceList>::write_link_data(network_models::network_information::network_data_information::NetworkData& network_data)
		{
			define_container_and_value_interface_unqualified_container(_Intersections_Container_Interface, _Intersection_Interface, type_of(intersections_container), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Intersection_Prototype, ComponentType);
			define_container_and_value_interface_unqualified_container(_Links_Container_Interface, _Link_Interface, type_of(links_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
			cout << "writing links" << endl;
			network_data.network_link_size = (int)_links_container.size();
			typename _Links_Container_Interface::iterator link_itr;
			for (link_itr = _links_container.begin(); link_itr != _links_container.end(); link_itr++)
			{
				//flow
				_Link_Interface* link = (_Link_Interface*)(*link_itr);
				_Intersection_Interface* upstream_intersection = link->template upstream_intersection<_Intersection_Interface*>();
				network_models::network_information::network_data_information::LinkData link_data;
					
				link_data.unode_index = upstream_intersection->template internal_id<int>();

				_Intersection_Interface* downstream_intersection = link->template downstream_intersection<_Intersection_Interface*>();
				link_data.dnode_index = downstream_intersection->template internal_id<int>();

				link_data.uuid = link->template uuid<int>();
				link_data.link_index = link->template internal_id<int>();
				link_data.num_lanes = link->template num_lanes<int>();
				link_data.link_type = link->template link_type<network_models::network_information::network_data_information::Link_Type_Keys>();
				link_data.length = link->template length<float>();
				link_data.speed_limit = link->template speed_limit<float>();
				link_data.maximum_flow_rate = link->template maximum_flow_rate<float>();
				link_data.backward_wave_speed = link->template backward_wave_speed<float>();
				link_data.free_flow_speed = link->template free_flow_speed<float>();
				link_data.jam_density = link->template jam_density<float>();
				link_data.left_turn_bay_length = link->template left_turn_bay_length<float>();
				link_data.right_turn_bay_length = link->template right_turn_bay_length<float>();
				link_data.num_left_turn_bays = link->template num_left_turn_bays<int>();
				link_data.num_right_turn_bays = link->template num_right_turn_bays<int>();

				network_data.link_data_array.push_back(link_data);
			}
			network_data.network_link_size = (int)network_data.link_data_array.size();

			//========================================================================================================
			//Create inbound/outbound links for each intersection
			//--------------------------------------------------------------------------------------------------------
			//

			for (link_itr = _links_container.begin(); link_itr != _links_container.end(); link_itr++)
			{
				_Link_Interface* link = (_Link_Interface*)(*link_itr);
				int unode_index = network_data.link_data_array[link->template internal_id<int>()].unode_index;
				network_data.node_data_array[unode_index].outbound_link_index_array.push_back(network_data.link_data_array[link->template internal_id<int>()].link_index);
					
				int dnode_index = network_data.link_data_array[link->template internal_id<int>()].dnode_index;
				network_data.node_data_array[dnode_index].inbound_link_index_array.push_back(network_data.link_data_array[link->template internal_id<int>()].link_index);
			}

			for (int i = 0; i < network_data.network_node_size; i++) {
				network_data.node_data_array[i].outbound_link_size = (int)network_data.node_data_array[i].outbound_link_index_array.size();
				network_data.node_data_array[i].inbound_link_size = (int)network_data.node_data_array[i].inbound_link_index_array.size();
			}
		}

		feature_implementation_definition void Polaris_Network_Implementation<MasterType,ParentType,InheritanceList>::write_turn_movement_data(network_models::network_information::network_data_information::NetworkData& network_data)
		{
			define_container_and_value_interface_unqualified_container(_Intersections_Container_Interface, _Intersection_Interface, type_of(intersections_container), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Intersection_Prototype, ComponentType);
			define_container_and_value_interface_unqualified_container(_Links_Container_Interface, _Link_Interface, type_of(links_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
			define_container_and_value_interface_unqualified_container(_Turn_Movements_Container_Interface, _Turn_Movement_Interface, type_of(turn_movements_container), Random_Access_Sequence_Prototype, Turn_Movement_Components::Prototypes::Movement_Prototype, ComponentType);
			define_container_and_value_interface(_Outbound_Inbound_Movements_Container_Interface, _Outbound_Inbound_Movements_Interface, typename _Intersection_Interface::get_type_of(outbound_inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Outbound_Inbound_Movements_Prototype, ComponentType);			
			define_container_and_value_interface(_Inbound_Outbound_Movements_Container_Interface, _Inbound_Outbound_Movements_Interface, typename _Intersection_Interface::get_type_of(inbound_outbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Inbound_Outbound_Movements_Prototype, ComponentType);
			cout << "writing turn movements" << endl;
			typename _Turn_Movements_Container_Interface::iterator movement_itr;
			for (movement_itr = _turn_movements_container.begin(); movement_itr != _turn_movements_container.end(); movement_itr++)
			{

				_Turn_Movement_Interface* turn_movement = (_Turn_Movement_Interface*)(*movement_itr);
					
				network_models::network_information::network_data_information::TurnMovementData turn_movement_data;

				turn_movement_data.inbound_link_index = turn_movement->template inbound_link<_Link_Interface*>()->template internal_id<int>();
				turn_movement_data.outbound_link_index = turn_movement->template outbound_link<_Link_Interface*>()->template internal_id<int>();
				turn_movement_data.turn_movement_type = turn_movement->template movement_type<network_models::network_information::network_data_information::Turn_Movement_Type_Keys>();
				turn_movement_data.turn_movement_rule = turn_movement->template movement_rule<network_models::network_information::network_data_information::Turn_Movement_Rule_Keys>();
				turn_movement_data.uuid = turn_movement->template uuid<int>();
				turn_movement_data.turn_movement_index = turn_movement->template internal_id<int>();
				//// assign the detector
				//turn_movement_1->template detector<Detector_Interface*>(lane->template Detector_Thru<Detector_Interface*>());
				network_data.turn_movement_data_array.push_back(turn_movement_data);
			}
			network_data.network_turn_movement_size = (int)network_data.turn_movement_data_array.size();

			for (movement_itr = _turn_movements_container.begin(); movement_itr != _turn_movements_container.end(); movement_itr++)
			{
				_Turn_Movement_Interface* turn_mvmt=(_Turn_Movement_Interface*)(*movement_itr);
				network_models::network_information::network_data_information::TurnMovementData& turn_movement_data = network_data.turn_movement_data_array[turn_mvmt->template internal_id<int>()];

				//outbound turn movement of an inbound link
				_Link_Interface* inbound_link = turn_mvmt->template inbound_link<_Link_Interface*>();
					
				network_data.link_data_array[inbound_link->template internal_id<int>()].outbound_turn_movement_index_array.push_back(turn_movement_data.turn_movement_index);
					
				//inbound turn movement of an outbound link
				_Link_Interface* outbound_link = turn_mvmt->template outbound_link<_Link_Interface*>();
					
				network_data.link_data_array[outbound_link->template internal_id<int>()].inbound_turn_movement_index_array.push_back(turn_movement_data.turn_movement_index);
			}

			for (int i = 0; i < network_data.network_link_size; i++)
			{
				network_data.link_data_array[i].outbound_turn_movement_size = (int)network_data.link_data_array[i].outbound_turn_movement_index_array.size();
				network_data.link_data_array[i].inbound_turn_movement_size = (int)network_data.link_data_array[i].inbound_turn_movement_index_array.size();
			}
		}

		feature_implementation_definition void Polaris_Network_Implementation<MasterType,ParentType,InheritanceList>::write_activity_location_data(network_models::network_information::network_data_information::NetworkData& network_data)
		{
			define_container_and_value_interface_unqualified_container(_Activity_Locations_Container_Interface, _Activity_Location_Interface, type_of(activity_locations_container), Random_Access_Sequence_Prototype, Activity_Location_Components::Prototypes::Activity_Location_Prototype, ComponentType);
			define_container_and_value_interface_unqualified_container(_Links_Container_Interface, _Link_Interface, type_of(links_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
			define_container_and_value_interface_unqualified_container(_Zones_Container_Interface, _Zone_Interface, type_of(zones_container), Random_Access_Sequence_Prototype, Zone_Components::Prototypes::Zone_Prototype, ComponentType);

			cout << "writing activity locations" << endl;
			typename _Activity_Locations_Container_Interface::iterator activity_location_itr;
			for (activity_location_itr = _activity_locations_container.begin(); activity_location_itr != _activity_locations_container.end(); activity_location_itr++) 
			{
				_Activity_Location_Interface* activity_location = (_Activity_Location_Interface*)(*activity_location_itr);
					
				network_models::network_information::network_data_information::ActivityLocationData activity_location_data;

				int j;
				int link_index;
				// initialize origins
				for (j = 0; j < (int)activity_location->template origin_links<_Links_Container_Interface&>().size(); j++) 
				{
					link_index = activity_location->template origin_links<_Links_Container_Interface&>()[j]->template internal_id<int>();
					activity_location_data.origin_link_index_array.push_back(link_index);
					activity_location_data.origin_link_choice_cdf_array.push_back(link_index);
				}
				activity_location_data.num_origin_links = (int)activity_location->template origin_links<_Links_Container_Interface&>().size();

				// initialize destinations
				for (j = 0; j < (int)activity_location->template destination_links<_Links_Container_Interface&>().size(); j++) 
				{
					link_index = activity_location->template destination_links<_Links_Container_Interface&>()[j]->template internal_id<int>();
					activity_location_data.destination_link_index_array.push_back(link_index);
					activity_location_data.destination_link_choice_cdf_array.push_back(link_index);
				}
				activity_location_data.num_destination_links = (int)activity_location->template destination_links<_Links_Container_Interface&>().size();

				activity_location_data.uuid = activity_location->template uuid<int>();
				activity_location_data.activity_location_index = activity_location->template internal_id<int>();
				activity_location_data.zone_index = activity_location->template zone<_Zone_Interface*>()-> template internal_id<int>();
				network_data.activity_location_data_array.push_back(activity_location_data);
			}
			network_data.network_activity_location_size = (int)network_data.activity_location_data_array.size();
		}

		feature_implementation_definition void Polaris_Network_Implementation<MasterType,ParentType,InheritanceList>::write_zone_data(network_models::network_information::network_data_information::NetworkData& network_data)
		{
			define_container_and_value_interface_unqualified_container(_Zones_Container_Interface, _Zone_Interface, type_of(zones_container), Random_Access_Sequence_Prototype, Zone_Components::Prototypes::Zone_Prototype, ComponentType);
			define_container_and_value_interface_unqualified_container(_Activity_Locations_Container_Interface, _Activity_Location_Interface, type_of(activity_locations_container), Random_Access_Sequence_Prototype, Activity_Location_Components::Prototypes::Activity_Location_Prototype, ComponentType);

			cout << "writing zones" << endl;
			typename _Zones_Container_Interface::iterator zone_itr;
			for (zone_itr = _zones_container.begin(); zone_itr != _zones_container.end(); zone_itr++) 
			{
				_Zone_Interface* zone = (_Zone_Interface*)(*zone_itr);
					
				network_models::network_information::network_data_information::ZoneData zone_data;

				int j;
				int activity_location_index;

				// initialize origins
				for (j = 0; j < (int)zone->template origin_activity_locations<_Activity_Locations_Container_Interface&>().size(); j++) 
				{
					activity_location_index = zone->template origin_activity_locations<_Activity_Locations_Container_Interface&>()[j]->template internal_id<int>();
					zone_data.origin_activity_location_index_array.push_back(activity_location_index);
					zone_data.origin_activity_location_choice_cdf_array.push_back(activity_location_index);
				}
				zone_data.num_origin_activity_locations = (int)zone_data.origin_activity_location_index_array.size();

				// initialize destinations
				for (j = 0; j < (int)zone->template destination_activity_locations<_Activity_Locations_Container_Interface&>().size(); j++) 
				{
					activity_location_index = zone->template destination_activity_locations<_Activity_Locations_Container_Interface&>()[j]->template internal_id<int>();
					zone_data.destination_activity_location_index_array.push_back(activity_location_index);
					zone_data.destination_activity_location_choice_cdf_array.push_back(activity_location_index);
				}
				zone_data.num_destination_activity_locations = (int)zone_data.destination_activity_location_index_array.size();

				zone_data.uuid = zone->template uuid<int>();
				zone_data.zone_index = zone->template internal_id<int>();
				network_data.zone_data_array.push_back(zone_data);
			}
			network_data.network_zone_size = (int)network_data.zone_data_array.size();
		}
	

	}
}