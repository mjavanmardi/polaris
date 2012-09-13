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
		
		template<typename ThisType,typename CallerType>
		struct Demand_Interface
		{
			facet_accessor(scenario_reference);
			facet_accessor(network_reference);

			facet void read_demand_data()
			{
				float freeway_demand_rate = 1800.0;
				float ramp_demand_rate = 900.0;
				
				//RngStream g1("Demand");
				//g1.SetSeed(1);
				
				//int vehilce_index = -1;



				//sort demand by departure time
				//demand_data.time_dependent_vehicle_index_array.resize(scenario_data.num_simulation_intervals*scenario_data.simulation_interval_length);

				typedef typename ThisType::network_reference_type NetworkType;
				typedef typename Network_Components::Interfaces::Network_Interface<NetworkType,ThisType> Network_Interface;

				NetworkInterface* network=network_reference<Network_Interface*>();
				
				typedef typename NetworkType::links_type LinksType;
				typedef typename LinksType::link_type LinkType;
				typedef Link_Components::Interfaces::Link_Interface<LinkType,ThisType> Link_Interface;

				LinksType& network_links=network->links<LinksType&>();

				typedef typename NetworkType::activity_locations_type ActivityLocationsType;
				typedef typename NetworkType::activity_locations_element_type ActivityLocationType;
				typedef Activity_Location_Components::Interfaces::Activity_Location_Interface<ActivityLocationType,ThisType> Activity_Location_Interface;

				typedef typename ActivityLocationType::origin_links_type OriginLinksType;
				typedef typename ActivityLocationType::destination_links_type DestinationLinksType;

				typedef typename ThisType::scenario_reference_type ScenarioType;
				typedef Scenario_Components::Interfaces::Scenario_Interface<ScenarioType,ThisType> Scenario_Interface;

				Scenario_Interface* scenario=scenario_reference<Scenario_Interface*>();

				typedef typename ThisType::traveler_type TravelerType;
				typedef Traveler_Components::Interfaces::Traveler_Interface<TravelerType,ThisType> Traveler_Interface;
				
				typedef typename TravelerType::vehicle_type VehicleType;
				typedef Vehicle_Components::Interfaces::Vehicle_Interface<VehicleType,ThisType> Vehicle_Interface;
				
				typedef typename TravelerType::routing_type RoutingType;
				typedef Routing_Components::Interfaces::Routing_Interface<RoutingType,ThisType> Routing_Interface;




				for(int simulation_interval_index=0;simulation_interval_index<scenario->num_simulation_intervals<int>();simulation_interval_index++)
				{// generate vehicles for each simulation interval

					if(simulation_interval_index<=300)
					{
						freeway_demand_rate += 5.0;
						ramp_demand_rate +=5.0;
					}
					else
					{
						freeway_demand_rate -= 5.0;
						ramp_demand_rate -=5.0;
					}

					int freeway_origin_activity_location_index = 0;
					Link_Interface* freeway_origin_link = (network_links->activity_locations<ActivityLocationsType&>()[freeway_origin_activity_location_index]).origin_links<OriginLinksType&>()[network_links[0]];
					int num_lanes_freeway = freeway_origin_link->num_lanes<int>();

					int ramp_origin_activity_location_index = 2;
					Link_Interface* ramp_origin_link = (network_links->activity_locations<ActivityLocationsType&>()[ramp_origin_activity_location_index]).origin_links<OriginLinksType&>()[network_links[0]];
					int num_lanes_ramp = network_data.link_data_array[ramp_origin_link_index].num_lanes;
					
					float num_generated_vehicles_freeway = (float) (1.0+(num_lanes_freeway*scenario->simulation_interval_length<int>() * freeway_demand_rate)/3600.0);
					float num_generated_vehicles_ramp = (float) ((1.0+num_lanes_ramp*scenario->simulation_interval_length<int>() * ramp_demand_rate)/3600.0);

					int destination_activity_location_index = 1;
					Link_Interface* destination_link = (network_links->activity_locations<ActivityLocationsType&>()[destination_activity_location_index]).destination_links<DestinationLinksType&>()[network_links[0]];
					
					Traveler_Interface* traveler;
					Vehicle_Interface* vehicle;
					Routing_Interface* router;

					float vehicle_rate_per_simulation_interval;
					int departure_time = -1;
					Link_Interface* origin_link;
					Activity_Location_Interface* origin_activity_location;
					int assignment_interval_index;
					assignment_interval_index = (int)((simulation_interval_index+1) * scenario->simulation_interval_length<int>() / scenario->assignment_interval_length<int>());					
					
					vehicle_rate_per_simulation_interval = (float)(num_generated_vehicles_freeway(scenario->simulation_interval_length<float>());
					origin_link = freeway_origin_link;
					origin_activity_location = network_links->activity_locations<ActivityLocationsType&>()[freeway_origin_activity_location_index];


					for (int i=0;i<scenario_data.simulation_interval_length;i++)
					{
						float cur_vehicle_rate_per_simulation_interval = vehicle_rate_per_simulation_interval;
						while(cur_vehicle_rate_per_simulation_interval>0)
						{
							if (cur_vehicle_rate_per_simulation_interval>=1.0)
							{
								vehicle_index = vehicle_index + 1;

								traveler=Allocate<TravelerType>();
								vehicle=Allocate<VehicleType>();
								router=Allocate<RouterType>();

								traveler->router<Router_Interface*>(router);
								traveler->vehicle<Router_Interface*>(vehicle);

								vehicle->origin_link<Link_Interface*>(origin_link);
								vehicle->destination_link<Link_Interface*>(destination_link);
								vehicle->origin_activity_location<Activity_Location_Interface*>(origin_activity_location);
								vehicle->destination_activity_location<Activity_Location_Interface*>(destination_activity_location);
								//vehicle_data.set_seed(scenario_data.iseed);
								vehicle->departure_simulation_interval_index<int>(assignment_interval_index);
								vehicle->departure_assignment_interval_index<int>(simulation_interval_index);

								departure_time = simulation_interval_index * scenario->simulation_interval_length<int>()  + i;
								traveler->Schedule_New_Departure<NULLTYPE>(departure_time);

								cur_vehicle_rate_per_simulation_interval = (float) (cur_vehicle_rate_per_simulation_interval - 1.0);
							}
							else
							{//monte carlo
								double r1;
								//r1 = (double) g1.RandU01();
								
								if (r1<=cur_vehicle_rate_per_simulation_interval)
								{
									vehicle_index = vehicle_index + 1;

									traveler=Allocate<TravelerType>();
									vehicle=Allocate<VehicleType>();
									router=Allocate<RouterType>();

									traveler->router<Router_Interface*>(router);
									traveler->vehicle<Router_Interface*>(vehicle);

									vehicle->origin_link<Link_Interface*>(origin_link);
									vehicle->destination_link<Link_Interface*>(destination_link);
									vehicle->origin_activity_location<Activity_Location_Interface*>(origin_activity_location);
									vehicle->destination_activity_location<Activity_Location_Interface*>(destination_activity_location);
									//vehicle_data.set_seed(scenario_data.iseed);
									vehicle->departure_simulation_interval_index<int>(assignment_interval_index);
									vehicle->departure_assignment_interval_index<int>(simulation_interval_index);

									departure_time = simulation_interval_index * scenario->simulation_interval_length<int>()  + i;
									traveler->Schedule_New_Departure<NULLTYPE>(departure_time);
								}

								cur_vehicle_rate_per_simulation_interval = (float)(cur_vehicle_rate_per_simulation_interval - 1.0);
							}
						}
					}
















					vehicle_rate_per_simulation_interval = (float)(num_generated_vehicles_ramp(scenario->simulation_interval_length<float>());
					origin_link = ramp_origin_link;
					origin_activity_location = network_links->activity_locations<ActivityLocationsType&>()[ramp_origin_activity_location_index];


					for (int i=0;i<scenario_data.simulation_interval_length;i++)
					{
						float cur_vehicle_rate_per_simulation_interval = vehicle_rate_per_simulation_interval;
						while(cur_vehicle_rate_per_simulation_interval>0)
						{
							if (cur_vehicle_rate_per_simulation_interval>=1.0)
							{
								vehicle_index = vehicle_index + 1;

								traveler=Allocate<TravelerType>();
								vehicle=Allocate<VehicleType>();
								router=Allocate<RouterType>();

								traveler->router<Router_Interface*>(router);
								traveler->vehicle<Router_Interface*>(vehicle);

								vehicle->origin_link<Link_Interface*>(origin_link);
								vehicle->destination_link<Link_Interface*>(destination_link);
								vehicle->origin_activity_location<Activity_Location_Interface*>(origin_activity_location);
								vehicle->destination_activity_location<Activity_Location_Interface*>(destination_activity_location);
								//vehicle_data.set_seed(scenario_data.iseed);
								vehicle->departure_simulation_interval_index<int>(assignment_interval_index);
								vehicle->departure_assignment_interval_index<int>(simulation_interval_index);

								departure_time = simulation_interval_index * scenario->simulation_interval_length<int>()  + i;
								traveler->Schedule_New_Departure<NULLTYPE>(departure_time);

								cur_vehicle_rate_per_simulation_interval = (float) (cur_vehicle_rate_per_simulation_interval - 1.0);
							}
							else
							{//monte carlo
								double r1;
								//r1 = (double) g1.RandU01();
								
								if (r1<=cur_vehicle_rate_per_simulation_interval)
								{
									vehicle_index = vehicle_index + 1;

									traveler=Allocate<TravelerType>();
									vehicle=Allocate<VehicleType>();
									router=Allocate<RouterType>();

									traveler->router<Router_Interface*>(router);
									traveler->vehicle<Router_Interface*>(vehicle);

									vehicle->origin_link<Link_Interface*>(origin_link);
									vehicle->destination_link<Link_Interface*>(destination_link);
									vehicle->origin_activity_location<Activity_Location_Interface*>(origin_activity_location);
									vehicle->destination_activity_location<Activity_Location_Interface*>(destination_activity_location);
									//vehicle_data.set_seed(scenario_data.iseed);
									vehicle->departure_simulation_interval_index<int>(assignment_interval_index);
									vehicle->departure_assignment_interval_index<int>(simulation_interval_index);

									departure_time = simulation_interval_index * scenario->simulation_interval_length<int>()  + i;
									traveler->Schedule_New_Departure<NULLTYPE>(departure_time);
								}

								cur_vehicle_rate_per_simulation_interval = (float)(cur_vehicle_rate_per_simulation_interval - 1.0);
							}
						}
					}
				}
				
				//done with function
			}
		

		};
	}
}