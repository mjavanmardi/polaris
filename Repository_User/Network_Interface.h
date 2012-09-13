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
			facet_accessor(activity_locations);

			facet void construct_network_cost()
			{
			}
			
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

				typedef typename ThisType::intersections_type IntersectionsType;
				typedef typename ThisType::intersections_element_type IntersectionType;
				typedef typename Intersection_Components::Interfaces::Intersection_Interface<IntersectionType,ThisType> Intersection_Interface;

				intersections<IntersectionsType>().clear();
				
				//node
				//network_information::network_data_information::NodeData node_data;

				Intersection_Interface* intersection=Allocate<IntersectionType>();

				i = 0;
				//node_data.node_index = i;
				//node_data.uuid = "1";

				intersection->x_position<float>(0.0);
				intersection->y_position<float>((float) 2.0*link_length);















				node_data.y = (float) 2.0*link_length;
				node_data.node_type = NO_CONTROL;
				network_data.node_data_array.push_back(node_data);

				i = 1;
				node_data.node_index = i;
				
				node_data.uuid = "2";
				node_data.x = (float) i*link_length;
				node_data.y = (float) 2.0*link_length;
				node_data.node_type = NO_CONTROL;
				network_data.node_data_array.push_back(node_data);

				i = 2;
				node_data.node_index = i;
				
				node_data.uuid = "3";
				node_data.x = (float) i*link_length;
				node_data.y = (float) 2.0*link_length;
				node_data.node_type = NO_CONTROL;
				network_data.node_data_array.push_back(node_data);

				//link
				network_data.link_data_array.clear();
				network_information::network_data_information::LinkData link_data;

				i = 0;
				link_data.link_index = i;
				link_data.unode_index = 0;
				link_data.dnode_index = 1;

				link_data.uuid = "1";
				link_data.oppersite_link_index = -1;
				link_data.num_lanes = 2;
				link_data.link_type = FREEWAY;
				link_data.length = link_length*distance_factor;
				link_data.speed_limit = speed_limit;
				link_data.maximum_flow_rate = maximum_flow_rate;
				link_data.backward_wave_speed = backward_wave_speed;
				link_data.free_flow_speed = float (speed_limit + 15.0);
				link_data.jam_density = jam_density;
				link_data.left_turn_bay_length = 0.0;
				link_data.right_turn_bay_length = 0.0;
				link_data.num_left_turn_bays = 0;
				link_data.num_right_turn_bays = 0;
				network_data.link_data_array.push_back(link_data);
				network_data.max_free_flow_speed = max(network_data.max_free_flow_speed,link_data.free_flow_speed);

				i = 1;
				link_data.link_index = i;
				link_data.unode_index = 1;
				link_data.dnode_index = 2;

				link_data.uuid = "2";
				link_data.oppersite_link_index = -1;
				link_data.num_lanes = 2;
				link_data.link_type = FREEWAY;
				link_data.length = link_length*distance_factor;
				link_data.speed_limit = speed_limit;
				link_data.maximum_flow_rate = maximum_flow_rate;
				link_data.backward_wave_speed = backward_wave_speed;
				link_data.free_flow_speed =  float (speed_limit + 15.0);
				link_data.jam_density = jam_density;
				link_data.left_turn_bay_length = 0.0;
				link_data.right_turn_bay_length = 0.0;
				link_data.num_left_turn_bays = 0;
				link_data.num_right_turn_bays = 0;
				network_data.link_data_array.push_back(link_data);
				network_data.max_free_flow_speed = max(network_data.max_free_flow_speed,link_data.free_flow_speed);

				//turn
				network_data.turn_movement_data_array.clear();
				network_information::network_data_information::TurnMovementData turn_movement_data;

				i=0;
				turn_movement_data.turn_movement_index = i;
				turn_movement_data.inbound_link_index = 0;
				turn_movement_data.outbound_link_index = 1;
	
				turn_movement_data.uuid = "1";
				turn_movement_data.turn_movement_type = THROUGH_TURN;
				turn_movement_data.turn_movement_rule = ALLOWED;
				network_data.turn_movement_data_array.push_back(turn_movement_data);

				//activity location
				network_data.activity_location_data_array.clear();
				network_information::network_data_information::ActivityLocationData activity_location_data;

				i=0;
				activity_location_data.activity_location_index = i;

				activity_location_data.uuid = "1";
				activity_location_data.origin_links.clear();
				activity_location_data.destination_links.clear();
				activity_location_data.origin_links.push_back(0);
				activity_location_data.num_origin_links = (int) activity_location_data.origin_links.size();
				activity_location_data.num_destination_links = (int) activity_location_data.destination_links.size();
				activity_location_data.zone_index = i;
				network_data.activity_location_data_array.push_back(activity_location_data);

				i=1;
				activity_location_data.activity_location_index = i;
				
				activity_location_data.uuid = "2";
				activity_location_data.origin_links.clear();
				activity_location_data.destination_links.clear();
				activity_location_data.destination_links.push_back(1);
				activity_location_data.num_origin_links = (int) activity_location_data.origin_links.size();
				activity_location_data.num_destination_links = (int) activity_location_data.destination_links.size();
				activity_location_data.zone_index = i;
				network_data.activity_location_data_array.push_back(activity_location_data);

				network_data.network_activity_location_size = (int) network_data.activity_location_data_array.size();
				network_data.network_node_size = (int) network_data.node_data_array.size();
				network_data.network_link_size = (int) network_data.link_data_array.size();
				network_data.network_turn_movement_size = (int) network_data.turn_movement_data_array.size();

				//turn movement
				for (int i=0;i<network_data.network_turn_movement_size;i++)
				{
					//outbound turn movement of an inbound link
					int inbound_link_index = network_data.turn_movement_data_array[i].inbound_link_index;
					network_data.link_data_array[inbound_link_index].outbound_turn_movement_index_array.push_back(i);
		
					//inbound turn movement of an outbound link
					int outbound_link_index = network_data.turn_movement_data_array[i].outbound_link_index;
					network_data.link_data_array[outbound_link_index].inbound_turn_movement_index_array.push_back(i);

					//printf("tm_index= %d il=%d ol=%d\n",i,network_data.turn_movement_data_array[i].inbound_link_index,network_data.turn_movement_data_array[i].outbound_link_index);
				}

				//link
				for(int i=0;i<network_data.network_link_size;i++)
				{
					//outbound turn movement size
					network_data.link_data_array[i].outbound_turn_movement_size = (int) network_data.link_data_array[i].outbound_turn_movement_index_array.size();
					//outbound link
					int unode_index = network_data.link_data_array[i].unode_index;
					network_data.node_data_array[unode_index].outbound_link_index_array.push_back(i);
		
					//inbound turn movement size
					network_data.link_data_array[i].inbound_turn_movement_size = (int) network_data.link_data_array[i].inbound_turn_movement_index_array.size();
					//inbound link
					int dnode_index = network_data.link_data_array[i].dnode_index;
					network_data.node_data_array[dnode_index].inbound_link_index_array.push_back(i);
					//printf("link_index= %d unode=%d dnode=%d outboud_tm_size= %d\n",i,network_data.link_data_array[i].unode_index,network_data.link_data_array[i].dnode_index,network_data.link_data_array[i].outbound_turn_movement_size);
				}
				//node
				for (int i=0;i<network_data.network_node_size;i++)
				{
					//network_data.node_data_array[i].g.SetSeed(i+1234567);
					//outbound link size
					network_data.node_data_array[i].outbound_link_size = (int) network_data.node_data_array[i].outbound_link_index_array.size();

					//inbound link size
					network_data.node_data_array[i].inbound_link_size = (int) network_data.node_data_array[i].inbound_link_index_array.size();

					//printf("node_index= %d outboud_link_size= %d\n",i,network_data.node_data_array[i].outbound_link_size);
				}

				//link - turn movement map
				network_data.link_turn_movement_map.clear();
				for (int i=0;i<network_data.network_turn_movement_size;i++)
				{
					int inbound_link_index = network_data.turn_movement_data_array[i].inbound_link_index;
					int outbond_link_index = network_data.turn_movement_data_array[i].outbound_link_index;
					network_data.link_turn_movement_map.insert(make_pair(make_pair(inbound_link_index,outbond_link_index),i));
				}
			}

		};

	}
}