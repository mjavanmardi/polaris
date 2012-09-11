#pragma once
#include "Repository_User_Includes.h"
#include "Link_Interface.h"
#include "Vehicle_Interface.h"

namespace Intersection_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Interfaces
	{
		//struct Inbound_Outbound_Movements_Interface
		//{
		//	facet_accessor(inbound_movement_reference);
		//	facet_accessor(outbound_movements);
		//	facet_accessor(num_movements);
		//};

		template<typename ThisType,typename CallerType>
		struct Outbound_Inbound_Movements_Interface
		{
			facet_accessor(outbound_movement_reference);
			facet_accessor(inbound_movements);
		};
		
		template<typename ThisType,typename CallerType>
		struct Movement_Interface
		{
			facet_accessor(movement_reference);

			facet_accessor(movement_capacity);
			facet_accessor(movement_supply);
			facet_accessor(movement_demand);
			facet_accessor(movement_flow);
			facet_accessor(movement_transferred);

			facet_accessor(movement_type);
			facet_accessor(movement_rule);

			facet_accessor(cached_outbound_link_arrival_time_based_experienced_link_turn_travel_delay_array);
			facet_accessor(cached_inbound_link_departure_time_based_experienced_link_turn_travel_delay_array);
			facet_accessor(cached_outbound_link_departure_time_based_experienced_link_turn_travel_delay_array);
			facet_accessor(inbound_link_departure_time_based_experienced_link_turn_travel_delay);
			facet_accessor(outbound_link_arrival_time_based_experienced_link_turn_travel_delay);
			
			facet_accessor(turn_movement_cumulative_arrival_vehicles);
			facet_accessor(turn_movement_cumulative_vehicles);
			facet_accessor(cached_turn_movement_cumulative_shifted_arrival_vehicles_array);
			facet_accessor(turn_movement_cumulative_shifted_arrival_vehicles);
			
			facet_accessor(vehicles_container);

			facet_accessor(minimum_merge_rate);
		};

		template<typename ThisType,typename CallerType>
		struct Intersection_Interface
		{
			facet_accessor(uuid);

			facet_accessor(inbound_outbound_movements);
			facet_accessor(outbound_inbound_movements);

			facet_accessor(x_position);
			facet_accessor(y_position);
			facet_accessor(z_position);

			facet_accessor(intersection_type);

			facet_accessor(scenario_reference);

			//facet TargetType pull_vehicle(call_requirements(requires_2(CallerType,Is_Same_Entity)))
			//{
			//	PTHIS(ThisType)->offer_vehicle<Dispatch<ThisType>,TargetType>();
			//}

			facet void push_vehicle(Vehicle_Components::Interfaces::Vehicle_Interface<ThisType,CallerType>* vehicle)
			{
				accept_vehicle<TargetType>(vehicle);
			}
			
			facet void accept_vehicle(Vehicle_Components::Interfaces::Vehicle_Interface* vehicle)
			{
				typedef typename ThisType::scenario_reference_type ScenarioType;
				typedef typename Scenario_Components::Interfaces::Scenario_Interface<ScenarioType,ThisType> Scenario_Interface;

				Scenario_Interface* scenario=scenario_reference<Scenario_Interface*>();
				int current_simulation_interval_index = scenario->current_simulation_interval_index<ScenarioType,int>();
				int simulation_interval_length = scenario->simulation_interval_length<ScenarioType,int>();
				int current_time = scenario->current_time<ScenarioType,int>();

				
				typedef typename ThisType::outbound_inbound_movements_type OutboundInboundType;
				
				typedef typename OutboundInboundType::element_type::inbound_movements_type InboundMovementsType;
				typedef typename InboundMovementsType::element_type InboundMovementType;
				typedef typename Movement_Interface<InboundMovementType,ThisType> Movement_Interface;

				typedef typename OutboundInboundType::outbound_movement_reference_type LinkType;
				typedef typename Link_Components::Interfaces::Link_Interface<LinkType,ThisType> Link_Interface;
				
				typedef typename InboundMovementType::vehicles_container::Element_Type VehicleType;
				typedef typename Vehicle_Components::Interfaces::Vehicle_Interface<VehicleType,ThisType> Vehicle_Interface;

				typedef typename InboundMovementType::vehicles_container_type VehiclesContainerType;



				Link_Interface* outbound_link;

				OutboundInboundType& outbound_links_container=outbound_inbound_movements<OutboundInboundType&>();
				typename OutboundInboundType::iterator outbound_itr;

				for (outbound_itr=outbound_links_container.begin(); outbound_itr!=outbound_links_container.end(); outbound_itr++)
				{
					outbound_link=(*outbound_itr)->outbound_movement_reference<Link_Interface*>();

					Movement_Interface* inbound_movement;
					Link_Interface* inbound_link;

					InboundMovementsType& inbound_movements = (*outbound_itr)->inbound_movements<InboundMovementsType&>();
					InboundMovementsType::iterator inbound_itr;

					for(inbound_itr=inbound_movements.begin();inbound_itr!=inbound_movements.end();inbound_itr++)
					{
						inbound_movement=(*inbound_itr);

						inbound_link=inbound_movement->movement_reference<Link_Interface*>();

						if(vehicle->next_link<Link_Interface*>()==inbound_link)
						{
							inbound_movement->vehicles_container<VehicleQueueType&>().push_back(vehicle);
							inbound_movement->turn_movement_cumulative_arrival_vehicles<int&>()++;
						}
					}
				}
			}

			facet void turn_movement_capacity_update()
			{
				typedef typename ThisType::scenario_reference_type ScenarioType;
				typedef typename Scenario_Components::Interfaces::Scenario_Interface<ScenarioType,ThisType> Scenario_Interface;

				Scenario_Interface* scenario=scenario_reference<Scenario_Interface*>();

				typedef typename ThisType::outbound_inbound_movements_type OutboundInboundType;
				typedef typename OutboundInboundType::outbound_movement_reference_type LinkType;
				typedef typename Link_Components::Interfaces::Link_Interface<LinkType,ThisType> Link_Interface;
				
				typedef typename OutboundInboundType::element_type::inbound_movements_type InboundMovementsType;
				typedef typename InboundMovementsType::element_type InboundMovementType;
				typedef typename Movement_Interface<InboundMovementType,ThisType> Movement_Interface;

				// y_capacity(a,t) = q_max(a,t) * delta_t * nlanes(a);
				// y_capacity(a,a',t)
				//	= min{y_capacity(a,t),y_capacity(a',t)} no control
				//	= delta_t * [green(a,a')/C] * min{y_capacity(a,t),y_capacity(a',t)} signal control method 1: no control simulation
				//	= [green(a,a',t)/delta_t] * min{y_capacity(a,t),y_capacity(a',t)} signal control method 2: control simulation

				Link_Interface* outbound_link;
				OutboundInboundType& outbound_links_container=outbound_inbound_movements<OutboundInboundType&>();
				typename OutboundInboundType::iterator outbound_itr;
				
				for(outbound_itr=outbound_links_container.begin(); outbound_itr!=outbound_links_container.end(); outbound_itr++)
				{
					outbound_link=(*outbound_itr)->outbound_movement_reference<Link_Interface*>();

					float outbound_link_capacity = (float)(scenario->simulation_interval_length<int> * outbound_link->num_lanes<int>() * (outbound_link->maximum_flow_rate<float>()/3600.0));

					Link_Interface* inbound_link;
					InboundMovementsType& inbound_links_container=(*outbound_itr)->inbound_movements<InboundMovementsType&>();
					typename InboundMovementsType::iterator inbound_itr;
					Movement_Interface* inbound_movement;

					for(inbound_itr=inbound_links_container.begin();inbound_itr!=inbound_links_container.end();inbound_itr++)
					{
						inbound_movement=(*inbound_itr);

						inbound_link=(*inbound_itr)->movement_reference<Link_Interface*>();
						float inbound_link_capacity = inbound_link->link_capacity<float>(); 
						
						if(intersection_type<Intersection_Type_Keys>() == NO_CONTROL)
						{
							inbound_movement->movement_capacity<float>(min(inbound_link_capacity,outbound_link_capacity));
						}
						else
						{
							inbound_movement->movement_capacity<float>(min(inbound_link_capacity,outbound_link_capacity));
						}
					}
				}
			}

			facet void turn_movement_supply_allocation()
			{
				typedef typename ThisType::outbound_inbound_movements_type OutboundInboundType;
				typedef typename OutboundInboundType::outbound_movement_reference_type LinkType;
				typedef typename Link_Components::Interfaces::Link_Interface<LinkType,ThisType> Link_Interface;
				
				typedef typename OutboundInboundType::element_type::inbound_movements_type InboundMovementsType;
				typedef typename InboundMovementsType::element_type InboundMovementType;
				typedef typename Movement_Interface<InboundMovementType,ThisType> Movement_Interface;

				// y_capacity(a,t) = q_max(a,t) * delta_t * nlanes(a);
				// y_capacity(a,a',t)
				//	= min{y_capacity(a,t),y_capacity(a',t)} no control
				//	= delta_t * [green(a,a')/C] * min{y_capacity(a,t),y_capacity(a',t)} signal control method 1: no control simulation
				//	= [green(a,a',t)/delta_t] * min{y_capacity(a,t),y_capacity(a',t)} signal control method 2: control simulation
				
				
				vector<float> merge_ratios;
				const int max_inbound_turn_movement_size=20;
				merge_ratios.resize(max_inbound_turn_movement_size);

				Link_Interface* outbound_link;
				OutboundInboundType& outbound_links_container=outbound_inbound_movements<OutboundInboundType&>();
				typename OutboundInboundType::iterator outbound_itr;

				for (outbound_itr=outbound_links_container.begin(); outbound_itr!=outbound_links_container.end(); outbound_itr++)
				{
					outbound_link=(*outbound_itr)->outbound_movement_reference<Link_Interface*>();
					
					Movement_Interface* inbound_movement;
					Link_Interface* inbound_link;
					InboundMovementsType& inbound_links_container=(*outbound_itr)->inbound_movements<InboundMovementsType&>();
					typename InboundMovementsType::iterator inbound_itr;

					int inbound_turn_movement_size=inbound_links_container.size();
					
					if(inbound_turn_movement_size==1)
					{
						inbound_links_container[0]->movement_supply<float>(outbound_link->link_supply<float>());
					}
					else
					{
						///simple rule as Daganzo --- need to work on this later
						int num_lanes_outbound_link = outbound_link->num_lanes<int>();
						int link_type_outbound_link = outbound_link->link_type<int>();
						int num_merge_lanes = 0;

						int num_through_turns = 0;

						for(inbound_itr=inbound_links_container.begin();inbound_itr!=inbound_links_container.end();inbound_itr++)
						{
							inbound_movement=(*inbound_itr);

							inbound_link=inbound_movement->movement_reference<Link_Interface*>();

							int num_lanes_inbound_link = inbound_link->num_lanes<int>();
							int link_type_inbound_link = inbound_link->link_type<int>();
							
							if (link_type_inbound_link != link_type_outbound_link)
							{
								if (num_merge_lanes<num_lanes_inbound_link)
								{
									num_merge_lanes = num_lanes_inbound_link;
								}
							}
							if (inbound_movement->turn_movement_type<Turn_Movement_Type_Keys>() == THROUGH_TURN)
							{
								num_through_turns++;
							}
						}
						float major_approach_merge_ratio = 0.6;
						float minor_approach_merge_ratio = 0.4;


						if (num_through_turns>0)
						{
							float through_turn_ratio = 1.0/(float(num_through_turns));
							if (num_merge_lanes<num_lanes_outbound_link)
							{
								float dedicated_lanes = float(num_lanes_outbound_link - num_merge_lanes);
								major_approach_merge_ratio = through_turn_ratio* dedicated_lanes / (float(num_lanes_outbound_link)) + 0.6 * ((float (num_merge_lanes))/(float(num_lanes_outbound_link)));
								minor_approach_merge_ratio = 0.4 * ((float (num_merge_lanes))/(float(num_lanes_outbound_link))) / (float(inbound_turn_movement_size-1));
							}
							else
							{
								major_approach_merge_ratio =through_turn_ratio* 0.6;
								minor_approach_merge_ratio = 0.4 / (float(inbound_turn_movement_size-1));
							}
						}
						else
						{
							if (num_merge_lanes<num_lanes_outbound_link)
							{
								float dedicated_lanes = float(num_lanes_outbound_link - num_merge_lanes);
								major_approach_merge_ratio =  dedicated_lanes / (float(num_lanes_outbound_link)) + 0.5 * ((float (num_merge_lanes))/(float(num_lanes_outbound_link)));
								minor_approach_merge_ratio = 0.5 * ((float (num_merge_lanes))/(float(num_lanes_outbound_link))) / (float(inbound_turn_movement_size-1));
							}
							else
							{
								major_approach_merge_ratio = 0.5;
								minor_approach_merge_ratio = 0.5 / (float(inbound_turn_movement_size-1));
							}
						}

						for(inbound_itr=inbound_links_container.begin();inbound_itr!=inbound_links_container.end();inbound_itr++)
						{
							inbound_movement=(*inbound_itr);

							inbound_link=inbound_movement->movement_reference<Link_Interface*>();

							int link_type_inbound_link = inbound_link->link_type<LinkType,int>();
							float merge_ratio = 0;

							if(link_type_inbound_link == link_type_outbound_link && inbound_movement->movement_type<Turn_Movement_Type_Keys>() == THROUGH_TURN)
							{
								merge_ratio = major_approach_merge_ratio;
							}
							else
							{
								merge_ratio = minor_approach_merge_ratio;
							}

							inbound_movement->movement_supply<float>(merge_ratio*outbound_link->link_supply<float>());
						}
						///simple rule as Daganzo
					}
				}
			}

			facet void turn_movement_flow_calculation()
			{
				typedef typename ThisType::scenario_reference_type ScenarioType;
				typedef typename Scenario_Components::Interfaces::Scenario_Interface<ScenarioType,ThisType> Scenario_Interface;

				Scenario_Interface* scenario=scenario_reference<Scenario_Interface*>();
				
				int current_simulation_interval_index = scenario->current_simulation_interval_index<int>();
				int simulation_interval_length = scenario->simulation_interval_length<int>();
				int current_time = scenario->current_time<int>();

				typedef typename ThisType::outbound_inbound_movements_type OutboundInboundType;
				typedef typename OutboundInboundType::outbound_movement_reference_type LinkType;
				typedef typename Link_Components::Interfaces::Link_Interface<LinkType,ThisType> Link_Interface;

				typedef typename OutboundInboundType::element_type::inbound_movements_type InboundMovementsType;
				typedef typename InboundMovementsType::element_type InboundMovementType;
				typedef typename Movement_Interface<InboundMovementType,ThisType> Movement_Interface;

				LinkType* outbound_link;
				OutboundInboundType& outbound_links_container=outbound_inbound_movements<OutboundInboundType&>();
				typename OutboundInboundType::iterator outbound_itr;

				for (outbound_itr=outbound_links_container.begin(); outbound_itr!=outbound_links_container.end(); outbound_itr++)
				{
					outbound_link=(*outbound_itr)->outbound_movement_reference<Link_Interface*>();

					Movement_Interface* inbound_movement;
					Link_Interface* inbound_link;

					InboundMovementsType& inbound_links_container = (*outbound_itr)->inbound_movements<InboundMovementsType&>();
					InboundMovementsType::iterator inbound_itr;

					for(inbound_itr=inbound_links_container.begin();inbound_itr!=inbound_links_container.end();inbound_itr++)
					{
						inbound_movement=(*inbound_itr);

						inbound_link=inbound_movement->movement_reference<Link_Interface*>();

						int t_minus_fftt = -1;

						int link_fftt_cached_simulation_interval = inbound_link->link_fftt_cached_simulation_interval_size<int>();

						if (current_simulation_interval_index >= link_fftt_cached_simulation_interval)
						{
							t_minus_fftt = (current_simulation_interval_index - link_fftt_cached_simulation_interval) % link_fftt_cached_simulation_interval;			
						}

						if (t_minus_fftt > -1)
						{
							int cached_turn_movement_cumulative_shifted_arrival_vehicles=((*inbound_itr)->cached_turn_movement_cumulative_shifted_arrival_vehicles<int*>())[t_minus_fftt];
							inbound_movement->turn_movement_cumulative_shifted_arrival_vehicles<int>(cached_turn_movement_cumulative_shifted_arrival_vehicles);
						}
						else
						{
							inbound_movement->turn_movement_cumulative_shifted_arrival_vehicles<int>(0);
						}
						
						int movement_demand = inbound_movement->turn_movement_cumulative_shifted_arrival_vehicles<int>() - inbound_movement->turn_movement_cumulative_vehicles<int>();
						
						inbound_movement->movement_demand<float>(movement_demand);

						if(inbound_movement->movement_demand<float>() < 0.0) 
						{
							inbound_movement->movement_demand<float>(0);
						}

						float turn_movement_demand = inbound_movement->movement_demand<float>();
						float turn_movement_capacity = inbound_movement->movement_capacity<float>();
						float turn_movement_supply =  inbound_movement->movement_supply<float>();
						float turn_movement_flow = (float) min(min((double)turn_movement_demand,(double)turn_movement_capacity),(double)turn_movement_supply);
						inbound_movement->movement_flow<float>(turn_movement_flow);
					}
				}
			}

			facet void node_transfer()
			{
				typedef typename ThisType::scenario_reference_type ScenarioType;
				typedef typename Scenario_Components::Interfaces::Scenario_Interface<ScenarioType,ThisType> Scenario_Interface;

				Scenario_Interface* scenario=scenario_reference<Scenario_Interface*>();
				
				int current_simulation_interval_index = scenario->current_simulation_interval_index<int>();
				int simulation_interval_length = scenario->simulation_interval_length<int>();
				int current_time = scenario->current_time<int>();

				typedef typename ThisType::outbound_inbound_movements_type OutboundInboundType;

				typedef typename OutboundInboundType::outbound_movement_reference_type LinkType;
				typedef typename Link_Components::Interfaces::Link_Interface<LinkType,ThisType> Link_Interface;

				typedef typename OutboundInboundType::element_type::inbound_movements_type InboundMovementsType;
				typedef typename InboundMovementsType::element_type InboundMovementType;
				typedef typename Movement_Interface<InboundMovementType,ThisType> Movement_Interface;

				// apply transfers of y(a,a''t) for each turn;
				//int arrived_vehicles = this->network_cumulative_arrived_vehicles;
				//int in_network_vehicles = this->network_in_network_vehicles;
				
				Link_Interface* outbound_link;
				OutboundInboundType& outbound_links_container=outbound_inbound_movements<OutboundInboundType&>();
				typename OutboundInboundType::iterator outbound_itr;

				for(outbound_itr=outbound_links_container.begin(); outbound_itr!=outbound_links_container.end(); outbound_itr++)
				{
					outbound_link=(*outbound_itr)->outbound_movement_reference<Link_Interface*>();

					Movement_Interface* inbound_movement;
					Link_Interface* inbound_link;

					InboundMovementsType& inbound_links_container = (*outbound_itr)->inbound_movements<InboundMovementsType>();
					typename InboundMovementsType::iterator inbound_itr;
					
					//initialization -> move to network state update
					outbound_link->link_upstream_arrived_vehicles<int>(0);
					outbound_link->link_destination_arrived_vehicles<int>(0);

					for(inbound_itr=inbound_links_container.begin();inbound_itr!=inbound_links_container.end();inbound_itr++)
					{
						inbound_movement=(*inbound_itr);
						inbound_link=inbound_movement->movement_reference<Link_Interface*>();

						inbound_movement->outbound_link_arrival_time_based_experienced_link_turn_travel_delay<float>(0);
						inbound_movement->inbound_link_departure_time_based_experienced_link_turn_travel_delay<float>(0);

						if(outbound_itr==outbound_links_container.begin())
						{
							//initialization
							inbound_link->link_downstream_departed_vehicles<int>(0);
						}

						//determine number of transfered vehicles
						float transfer_flow_turn_movement = inbound_movement->movement_flow<float>();
						
						int num_transfer_vehicles_of_turn_movement = (int)transfer_flow_turn_movement;
						transfer_flow_turn_movement = transfer_flow_turn_movement - num_transfer_vehicles_of_turn_movement;
						
						if(transfer_flow_turn_movement > 0.0)
						{
							if(rand()%2==0)
							{//partial vehicle, incomplete implementation
								++num_transfer_vehicles_of_turn_movement;
							}
						}

						inbound_movement->movement_transferred<int>(num_transfer_vehicles_of_turn_movement);
						
						if (num_transfer_vehicles_of_turn_movement>0)
						{
							typedef typename InboundMovementType::turn_movement_vehicles_element_type VehicleType;
							typedef typename Vehicle_Components::Interfaces::Vehicle_Interface<VehicleType,ThisType> Vehicle_Interface;
							
							//move vehicles for this turn movement
							for (int iv=0;iv<num_transfer_vehicles_of_turn_movement;iv++)
							{
								Vehicle_Interface* vehicle=inbound_movement->pull_vehicle<Vehicle_Interface*>();

								//update vehicle state: transfer to next link
								int enter_time=vehicle->current_link_enter_time<int>();
								int delayed_time = (scenario->current_time<int>() - enter_time) - inbound_link->link_fftt<float>();
								int enter_interval_index = vehicle->current_link_enter_interval_index<int>();
								int delayed_interval = current_simulation_interval_index - enter_interval_index;

								//update inbound link state: N(a',L,t)
								inbound_link->link_downstream_cumulative_vehicles<int&>()++;
								inbound_link->link_downstream_departed_vehicles<int&>()++;

								//update turn movement state: N(a',a,t)
								inbound_movement->turn_movement_cumulative_vehicles<int&>()++;

								//update link_turn_travel_delay
								inbound_movement->outbound_link_arrival_time_based_experienced_link_turn_travel_delay<float&>()+=delayed_time;

								//find the departure time
								int departure_time_position = -1;
								
								int num_intervals=scenario->num_simulation_intervals_per_assignment_interval<int>();

								if (current_simulation_interval_index>=num_intervals)
								{
									departure_time_position = (current_simulation_interval_index+num_intervals-delayed_interval)%num_intervals;
								}
								else
								{
									departure_time_position = enter_interval_index;
								}

								inbound_movement->cached_inbound_link_departure_time_based_experienced_link_turn_travel_delay_array<float*>()[departure_time_position]+=delayed_interval*simulation_interval_length;
								
								outbound_link->push_vehicle<NULLTYPE>(vehicle);
							}
						}

						if (num_transfer_vehicles_of_turn_movement>0)
						{
							float delay=inbound_movement->outbound_link_arrival_time_based_experienced_link_turn_travel_delay<float>()/((float)num_transfer_vehicles_of_turn_movement);
							inbound_movement->outbound_link_arrival_time_based_experienced_link_turn_travel_delay<float>(delay);
						}
						else
						{
							if(inbound_movement->turn_movement_rule<Turn_Movement_Rule_Keys>() == PROHIBITED)
							{
								inbound_movement->outbound_link_arrival_time_based_experienced_link_turn_travel_delay<float>(FLT_MAX);
							}
							else
							{
								if(inbound_movement->movement_demand<float>() == 0)
								{//no demand
									inbound_movement->outbound_link_arrival_time_based_experienced_link_turn_travel_delay<float>(0);
								}
								else
								{//no supply for this turn -- need a prediction -- need to work it later
									int t_minus_one = -1;
									if (current_simulation_interval_index>0)
									{
										t_minus_one = (current_simulation_interval_index-1)%scenario->num_simulation_intervals_per_assignment_interval<int>();

										float delay = simulation_interval_length +
											inbound_movement->cached_outbound_link_arrival_time_based_experienced_link_turn_travel_delay_array<float*>()[t_minus_one];

										inbound_movement->outbound_link_arrival_time_based_experienced_link_turn_travel_delay_array<float>(delay);
									}
									else
									{
										inbound_movement->outbound_link_arrival_time_based_experienced_link_turn_travel_delay_array<float>(0);
									}
								}
							}
						}
					}
				}
				
				//network_cumulative_arrived_vehicles=arrived_vehicles;
				//network_in_network_vehicles=in_network_vehicles;
			}

			facet void network_state_update()
			{
				typedef typename ThisType::scenario_reference_type ScenarioType;
				typedef typename Scenario_Components::Interfaces::Scenario_Interface<ScenarioType,ThisType> Scenario_Interface;

				Scenario_Interface* scenario=scenario_reference<Scenario_Interface*>();
				
				int current_simulation_interval_index = scenario->current_simulation_interval_index<int>();
				int simulation_interval_length = scenario->simulation_interval_length<int>();
				int current_time = scenario->current_time<int>();

				typedef typename ThisType::outbound_inbound_movements_type OutboundInboundType;

				typedef typename OutboundInboundType::outbound_movement_reference_type LinkType;
				typedef typename Link_Components::Interfaces::Link_Interface<LinkType,ThisType> Link_Interface;

				typedef typename OutboundInboundType::element_type::inbound_movements_type InboundMovementsType;
				typedef typename InboundMovementsType::element_type InboundMovementType;
				typedef typename Movement_Interface<InboundMovementType,ThisType> Movement_Interface;


				int t_fftt = -1;
				int t_bwtt = -1;
				int t_cached_delay = -1;

				if (current_simulation_interval_index>0)
				{
					t_fftt = (current_simulation_interval_index)%fftt_cached_simulation_interval_size;
					t_bwtt = (current_simulation_interval_index)%bwtt_cached_simulation_interval_size;
					t_cached_delay = (current_simulation_interval_index)%scenario->num_simulation_intervals_per_assignment_interval<int>();
				}
				else
				{
					t_fftt = 0;
					t_bwtt = 0;
					t_cached_delay = 0;
				}

				Link_Interface* outbound_link;
				OutboundInboundType& outbound_links_container=outbound_inbound_movements<OutboundInboundType&>();
				typename OutboundInboundType::iterator outbound_itr;

				for (outbound_itr=outbound_links_container.begin(); outbound_itr!=outbound_links_container.end(); outbound_itr++)
				{
					outbound_link=(*outbound_itr)->outbound_movement_reference<Link_Interface*>();

					Movement_Interface* inbound_movement;
					Link_Interface* inbound_link;

					InboundMovementsType& inbound_links_container = (*outbound_itr)->inbound_movements<InboundMovementsType&>();
					typename InboundMovementsType::iterator inbound_itr;

					for(inbound_itr=inbound_links_container.begin();inbound_itr!=inbound_links_container.end();inbound_itr++)
					{
						inbound_movement=(*inbound_itr);

						inbound_link=inbound_movement->movement_reference<Link_Interface*>();

						int t_plus_fftt = -1;

						if (current_simulation_interval_index>0)
						{
							t_plus_fftt = (current_simulation_interval_index) % (inbound_link->link_fftt_cached_simulation_interval_size<int>());
						}
						else
						{
							t_plus_fftt = 0;
						}

						//cached cumulative shifted arrival vehicles
						inbound_movement->cached_turn_movement_cumulative_shifted_arrival_vehicles_array<int*>()[t_plus_fftt] = inbound_movement->turn_movement_cumulative_arrival_vehicles<int>();
						
						//turn movement delayed time update
						inbound_movement->cached_outbound_link_arrival_time_based_experienced_link_turn_travel_delay_array<int*>()[t_cached_delay] = inbound_movement->outbound_link_arrival_time_based_experienced_link_turn_travel_delay<int>();
						
						//network_data.turn_travel_penalty_array[inbound_turn_movement_index] = this->outbound_link_arrival_time_based_experienced_link_turn_travel_delay_array[inbound_turn_movement_index];
					}
				}
			}

			facet_local bool adjacent_link_supply_finished()
			{

			}

			facet_local bool adjacent_link_moving_finished()
			{

			}

			declare_facet_conditional(Newells_Conditional)
			{
				Intersection_Interface* _this=pthis;
				
				if(_this->adjacent_link_supply_finished<NULLTYPE>())
				{
					if(! _this->adjacent_link_moving_finished<NULLTYPE>())
					{

						((Execution_Object*)pthis)->Swap_Event(&Intersection_Interface::Compute_Step_Flow);
						response.result=true;
						response.next=iteration;
					}
					else
					{
						((Execution_Object*)pthis)->Swap_Event(&Intersection_Interface::Network_State_Update);
						response.result=true;
						response.next=iteration+1;
					}
				}
				else
				{
					response.result=false;
					response.next=iteration;
				}
			}

			declare_facet_event(Compute_Step_Flow)
			{
				Intersection_Interface* _this=pthis;

				//step 2: turn vehicles updating based on node control and link management, inbound link demand, and outbound link supply
				_this->turn_movement_capacity_update(); 

				//step 3: allocate link supply to inbound turn movements according to a given merging policy
				_this->turn_movement_supply_allocation();

				//step 4: determine turn movement flow rate based on demand, capacity, and supply
				_this->turn_movement_flow_calculation();

				//step 6: node transfer
				_this->node_transfer();
			}

			declare_facet_event(Network_State_Update)
			{
				Intersection_Interface* _this=pthis;

				//step 9: intersection network state update
				_this->network_state_update();
			}

		};
	}
}