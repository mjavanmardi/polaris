#pragma once
#include "Repository_User_Includes.h"
#include "Link_Interface.h"
#include "Vehicle_Interface.h"

namespace Intersection_Components
{
	namespace Types
	{
		enum Turn_Movement_Type_Keys
		{
			LEFT_TURN=0,
			THROUGH_TURN,
			RIGHT_TURN,
			U_TURN,
		};

		enum Turn_Movement_Rule_Keys
		{
			PROHIBITED=0,
			ALLOWED,
		};

		enum Intersection_Type_Keys
		{
			NO_CONTROL=0,
			YIELD_SIGN,
			ALL_WAY_STOP_SIGN,
			TWO_WAY_STOP_SIGN,
			PRE_TIMED_SIGNAL_CONTROL, 
			ACTUATED_SIGNAL_CONTROL, 
			ADAPTIVE_SIGNAL_CONTROL,
		};

		enum Intersection_Simulation_Status
		{
			NONE_COMPLETE,
			COMPUTE_STEP_FLOW_COMPLETE,
			NETWORK_STATE_UPDATE_COMPLETE
		};
	}

	namespace Concepts
	{
	}
	
	namespace Interfaces
	{
		template<typename ThisType,typename CallerType>
		struct Movement_Interface
		{
			facet_accessor(detector);
			facet_accessor(id);
			facet_accessor(inbound_link);
			facet_accessor(outbound_link);

			facet_accessor(movement_capacity);
			facet_accessor(movement_supply);
			facet_accessor(movement_demand);
			facet_accessor(movement_flow);
			facet_accessor(movement_transferred);

			facet_accessor(movement_type);
			facet_accessor(movement_rule);

			facet_accessor(turn_travel_penalty);

			facet_accessor(cached_outbound_link_arrived_time_based_experienced_link_turn_travel_delay_array);
			facet_accessor(cached_inbound_link_departed_time_based_experienced_link_turn_travel_delay_array);
			facet_accessor(cached_outbound_link_departed_time_based_experienced_link_turn_travel_delay_array);
			facet_accessor(inbound_link_departed_time_based_experienced_link_turn_travel_delay);
			facet_accessor(outbound_link_arrived_time_based_experienced_link_turn_travel_delay);
			
			facet_accessor(turn_movement_cumulative_arrived_vehicles);
			facet_accessor(turn_movement_cumulative_vehicles);
			facet_accessor(cached_turn_movement_cumulative_shifted_arrived_vehicles_array);
			facet_accessor(turn_movement_cumulative_shifted_arrived_vehicles);
			
			facet_accessor(forward_link_turn_travel_time);
			
			facet_accessor(replicas_container);

			facet_accessor(vehicles_container);

			facet TargetType pull_vehicle()
			{
				return offer_vehicle<TargetType>();
			}
			
			facet TargetType offer_vehicle()
			{
				typedef ThisType::vehicles_container_type VehiclesContainerType;
				typedef ThisType::vehicles_container_element_type VehiclesContainerElementType;
				typedef Vehicle_Interface<VehiclesContainerElementType,ThisType> Vehicle_Interface;

				Vehicle_Interface* veh=(Vehicle_Interface*)vehicles_container<VehiclesContainerType&>().front();
				vehicles_container<VehiclesContainerType&>().pop_front();

				return (TargetType)veh;
			}

			facet_accessor(minimum_merge_rate);
		};

		template<typename ThisType,typename CallerType>
		struct Outbound_Inbound_Movements_Interface
		{
			facet_accessor(outbound_link_reference);
			facet_accessor(inbound_movements);
		};

		template<typename ThisType,typename CallerType>
		struct Inbound_Outbound_Movements_Interface
		{
			facet_accessor(inbound_link_reference);
			facet_accessor(lane_groups);
			facet_accessor(outbound_movements);
		};

		template<typename ThisType,typename CallerType>
		struct Intersection_Interface
		{
			facet_accessor(uuid);

			facet_accessor(signal);

			facet_accessor(rng_stream);

			facet_accessor(inbound_links);
			facet_accessor(outbound_links);

			facet_accessor(inbound_outbound_movements);
			facet_accessor(outbound_inbound_movements);

			facet_accessor(x_position);
			facet_accessor(y_position);
			facet_accessor(z_position);

			facet_accessor(intersection_type);

			facet_accessor(scenario_reference);

			facet_accessor(network);
			

			facet_accessor(intersection_simulation_status);

			//facet TargetType pull_vehicle()
			//{
			//	//PTHIS(ThisType)->offer_vehicle<Dispatch<ThisType>,TargetType>();
			//}

			facet void push_vehicle(void* vehicle)
			{
				accept_vehicle<TargetType>(vehicle);
			}
			
			facet void accept_vehicle(void* vehicle)
			{
				typedef typename ThisType::scenario_type ScenarioType;
				typedef typename Scenario_Interface<ScenarioType,ThisType> Scenario_Interface;

				Scenario_Interface* scenario=scenario_reference<Scenario_Interface*>();
				int current_simulation_interval_index = scenario->current_simulation_interval_index<int>();
				int simulation_interval_length = scenario->simulation_interval_length<int>();
				int current_time = scenario->current_time<int>();

				typedef typename ThisType::outbound_inbound_movements_container_type OutboundInboundType;
				typedef typename ThisType::outbound_inbound_movements_container_element_type OutboundInboundElementType;
				typedef typename Outbound_Inbound_Movements_Interface<OutboundInboundElementType,ThisType> Outbound_Inbound_Movement_Interface;


				typedef typename OutboundInboundElementType::outbound_link_reference_type LinkType;
				typedef typename Link_Interface<LinkType,ThisType> Link_Interface;


				typedef typename ThisType::inbound_movements_container_type InboundMovementsType;
				typedef typename ThisType::inbound_movement_type InboundMovementType;
				typedef typename Movement_Interface<InboundMovementType,ThisType> Movement_Interface;
				

				
				
				typedef typename ThisType::vehicle_type VehicleType;
				typedef typename Vehicle_Interface<VehicleType,ThisType> Vehicle_Interface;
				typedef typename InboundMovementType::vehicles_container_type VehiclesContainerType;

				Link_Interface* outbound_link;

				//PRINT(uuid<int>());

				OutboundInboundType& outbound_links_container=outbound_inbound_movements<OutboundInboundType&>();
				typename OutboundInboundType::iterator outbound_itr;

				for (outbound_itr=outbound_links_container.begin(); outbound_itr!=outbound_links_container.end(); outbound_itr++)
				{
					outbound_link=((Outbound_Inbound_Movement_Interface*)(*outbound_itr))->outbound_link_reference<Link_Interface*>();

					Movement_Interface* inbound_movement;
					Link_Interface* inbound_link;

					InboundMovementsType& inbound_movements = ((Outbound_Inbound_Movement_Interface*)(*outbound_itr))->inbound_movements<InboundMovementsType&>();
					InboundMovementsType::iterator inbound_itr;

					for(inbound_itr=inbound_movements.begin();inbound_itr!=inbound_movements.end();inbound_itr++)
					{
						inbound_movement=(Movement_Interface*)(*inbound_itr);

						inbound_link=inbound_movement->inbound_link<Link_Interface*>();

						//PRINT(((Vehicle_Interface*)vehicle)->next_link<Link_Interface*>()->uuid<int>() << "," << outbound_link->uuid<int>());
						//PRINT(((Vehicle_Interface*)vehicle)->current_link<Link_Interface*>()->uuid<int>() << "," << inbound_link->uuid<int>());

						if(((Vehicle_Interface*)vehicle)->next_link<Link_Interface*>()==outbound_link && ((Vehicle_Interface*)vehicle)->current_link<Link_Interface*>()==inbound_link)
						{
							
							Signal_Components::Interfaces::Detector_Interface<typename ThisType::Master_Type::DETECTOR_TYPE,NULLTYPE>* detector;
							detector = inbound_movement->detector<Signal_Components::Interfaces::Detector_Interface<typename ThisType::Master_Type::DETECTOR_TYPE,NULLTYPE>*>();
							if (detector != NULL) detector->detect_vehicle<int>();
							inbound_movement->vehicles_container<VehiclesContainerType&>().push_back(vehicle);
							inbound_movement->turn_movement_cumulative_arrived_vehicles<int&>()++;

							if (outbound_link->uuid<int>() == 2 && inbound_link->uuid<int>() == 1)
							{
								int count;
								count = detector->Get_Count<int>();
								bool test = false;
							}

							char s[100];
							sprintf_s(s, "stop:%i:%i:%i\n", inbound_link->uuid<int>(), this->uuid<int>(), scenario_reference<Scenario_Interface*>()->current_time<int>());
							std::string s1 = s;
							scenario_reference<Scenario_Interface*>()->output<NULLTYPE>(s1);
							sprintf_s(s, "advanced:%i:%i:%i\n", outbound_link->uuid<int>(), outbound_link->downstream_intersection<Intersection_Interface*>()->uuid<int>(), scenario_reference<Scenario_Interface*>()->current_time<int>());
							std::string s2 = s;
							scenario_reference<Scenario_Interface*>()->output<NULLTYPE>(s2);
						}
					}
				}
			}

			facet void turn_movement_capacity_update()
			{
				typedef typename ThisType::scenario_type ScenarioType;
				typedef typename Scenario_Interface<ScenarioType,ThisType> Scenario_Interface;

				Scenario_Interface* scenario=scenario_reference<Scenario_Interface*>();

				typedef typename ThisType::outbound_inbound_movements_container_type OutboundInboundType;
				typedef typename ThisType::outbound_inbound_movements_container_element_type OutboundInboundElementType;
				typedef typename Outbound_Inbound_Movements_Interface<OutboundInboundElementType,ThisType> Outbound_Inbound_Movement_Interface;

				typedef typename OutboundInboundElementType::outbound_link_reference_type LinkType;
				typedef typename Link_Interface<LinkType,ThisType> Link_Interface;
				
				typedef typename ThisType::inbound_movements_container_type InboundMovementsType;
				typedef typename ThisType::inbound_movement_type InboundMovementType;
				typedef typename Movement_Interface<InboundMovementType,ThisType> Movement_Interface;

				// y_capacity(a,t) = q_max(a,t) * delta_t * nlanes(a);
				// y_capacity(a,a',t)
				//	= min{y_capacity(a,t),y_capacity(a',t)} no control
				//	= delta_t * [green(a,a')/C] * min{y_capacity(a,t),y_capacity(a',t)} signal control method 1: no control simulation
				//	= [green(a,a',t)/delta_t] * min{y_capacity(a,t),y_capacity(a',t)} signal control method 2: control simulation

				Link_Interface* outbound_link;
				OutboundInboundType& outbound_links_container=outbound_inbound_movements<OutboundInboundType&>();
				typename OutboundInboundType::iterator outbound_itr;
				
				//PRINT("\t" << "turn movement capacities of node " << uuid<int>());

				for(outbound_itr=outbound_links_container.begin(); outbound_itr!=outbound_links_container.end(); outbound_itr++)
				{
					outbound_link=((Outbound_Inbound_Movement_Interface*)(*outbound_itr))->outbound_link_reference<Link_Interface*>();

					float outbound_link_capacity = (float)(scenario->simulation_interval_length<int>() * outbound_link->num_lanes<int>() * (outbound_link->maximum_flow_rate<float>()/3600.0));

					Link_Interface* inbound_link;
					InboundMovementsType& inbound_links_container=((Outbound_Inbound_Movement_Interface*)(*outbound_itr))->inbound_movements<InboundMovementsType&>();
					typename InboundMovementsType::iterator inbound_itr;
					Movement_Interface* inbound_movement;
					
					//PRINT("\t\t" << "for outbound link " << outbound_link->uuid<int>());

					for(inbound_itr=inbound_links_container.begin();inbound_itr!=inbound_links_container.end();inbound_itr++)
					{
						inbound_movement=(Movement_Interface*)(*inbound_itr);

						inbound_link=inbound_movement->inbound_link<Link_Interface*>();
						float inbound_link_capacity = inbound_link->link_capacity<float>(); 
						
						if(intersection_type<Types::Intersection_Type_Keys>() == Types::Intersection_Type_Keys::NO_CONTROL)
						{
							inbound_movement->movement_capacity<float>(min(inbound_link_capacity,outbound_link_capacity));
						}
						else
						{
							inbound_movement->movement_capacity<float>(min(inbound_link_capacity,outbound_link_capacity));
						}

						//PRINT("\t\t\t" << "for inbound link " << inbound_link->uuid<int>() << " is: " << inbound_movement->movement_capacity<float>());
					}
				}
			}

			facet void turn_movement_supply_allocation()
			{
				typedef typename ThisType::scenario_type ScenarioType;
				typedef typename Scenario_Interface<ScenarioType,ThisType> Scenario_Interface;

				Scenario_Interface* scenario=scenario_reference<Scenario_Interface*>();

				typedef typename ThisType::outbound_inbound_movements_container_type OutboundInboundType;
				typedef typename ThisType::outbound_inbound_movements_container_element_type OutboundInboundElementType;
				typedef typename Outbound_Inbound_Movements_Interface<OutboundInboundElementType,ThisType> Outbound_Inbound_Movement_Interface;

				typedef typename OutboundInboundElementType::outbound_link_reference_type LinkType;
				typedef typename Link_Interface<LinkType,ThisType> Link_Interface;
				
				typedef typename ThisType::inbound_movements_container_type InboundMovementsType;
				typedef typename ThisType::inbound_movement_type InboundMovementType;
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

				//PRINT("\t" << "turn movement supplies of node " << uuid<int>());

				for (outbound_itr=outbound_links_container.begin(); outbound_itr!=outbound_links_container.end(); outbound_itr++)
				{
					outbound_link=((Outbound_Inbound_Movement_Interface*)(*outbound_itr))->outbound_link_reference<Link_Interface*>();
					
					//PRINT("\t\t" << "for outbound link " << outbound_link->uuid<int>());

					Movement_Interface* inbound_movement;
					Link_Interface* inbound_link;
					InboundMovementsType& inbound_links_container=((Outbound_Inbound_Movement_Interface*)(*outbound_itr))->inbound_movements<InboundMovementsType&>();
					typename InboundMovementsType::iterator inbound_itr;

					int inbound_turn_movement_size=(int)inbound_links_container.size();
					
					if(inbound_turn_movement_size==1)
					{
						((Movement_Interface*)inbound_links_container[0])->movement_supply<float>(outbound_link->link_supply<float>());
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
							inbound_movement=(Movement_Interface*)(*inbound_itr);

							inbound_link=inbound_movement->inbound_link<Link_Interface*>();

							int num_lanes_inbound_link = inbound_link->num_lanes<int>();
							int link_type_inbound_link = inbound_link->link_type<int>();
							
							if (link_type_inbound_link != link_type_outbound_link)
							{
								if (num_merge_lanes<num_lanes_inbound_link)
								{
									num_merge_lanes = num_lanes_inbound_link;
								}
							}
							if (inbound_movement->movement_type<Types::Turn_Movement_Type_Keys>() == Types::Turn_Movement_Type_Keys::THROUGH_TURN)
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
							inbound_movement=(Movement_Interface*)(*inbound_itr);

							inbound_link=inbound_movement->inbound_link<Link_Interface*>();

							int link_type_inbound_link = inbound_link->link_type<int>();
							float merge_ratio = 0;

							if(link_type_inbound_link == link_type_outbound_link && inbound_movement->movement_type<Types::Turn_Movement_Type_Keys>() == Types::Turn_Movement_Type_Keys::THROUGH_TURN)
							{
								merge_ratio = major_approach_merge_ratio;
							}
							else
							{
								merge_ratio = minor_approach_merge_ratio;
							}

							inbound_movement->movement_supply<float>(merge_ratio*outbound_link->link_supply<float>());

							//PRINT("\t\t\t" << "for inbound link " << inbound_link->uuid<int>() << " is: " << inbound_movement->movement_supply<float>());
						}
						///simple rule as Daganzo
					}
				}
			}

			facet void turn_movement_flow_calculation()
			{
				typedef typename ThisType::scenario_type ScenarioType;
				typedef typename Scenario_Interface<ScenarioType,ThisType> Scenario_Interface;

				Scenario_Interface* scenario=scenario_reference<Scenario_Interface*>();
				
				int current_simulation_interval_index = scenario->current_simulation_interval_index<int>();
				int simulation_interval_length = scenario->simulation_interval_length<int>();
				int current_time = scenario->current_time<int>();

				typedef typename ThisType::outbound_inbound_movements_container_type OutboundInboundType;
				typedef typename ThisType::outbound_inbound_movements_container_element_type OutboundInboundElementType;
				typedef typename Outbound_Inbound_Movements_Interface<OutboundInboundElementType,ThisType> Outbound_Inbound_Movement_Interface;

				typedef typename OutboundInboundElementType::outbound_link_reference_type LinkType;
				typedef typename Link_Interface<LinkType,ThisType> Link_Interface;
				
				typedef typename ThisType::inbound_movements_container_type InboundMovementsType;
				typedef typename ThisType::inbound_movement_type InboundMovementType;
				typedef typename Movement_Interface<InboundMovementType,ThisType> Movement_Interface;

				Link_Interface* outbound_link;
				OutboundInboundType& outbound_links_container=outbound_inbound_movements<OutboundInboundType&>();
				typename OutboundInboundType::iterator outbound_itr;

				for (outbound_itr=outbound_links_container.begin(); outbound_itr!=outbound_links_container.end(); outbound_itr++)
				{
					outbound_link=((Outbound_Inbound_Movement_Interface*)(*outbound_itr))->outbound_link_reference<Link_Interface*>();

					Movement_Interface* inbound_movement;
					Link_Interface* inbound_link;

					InboundMovementsType& inbound_links_container = ((Outbound_Inbound_Movement_Interface*)(*outbound_itr))->inbound_movements<InboundMovementsType&>();
					InboundMovementsType::iterator inbound_itr;

					for(inbound_itr=inbound_links_container.begin();inbound_itr!=inbound_links_container.end();inbound_itr++)
					{
						inbound_movement=(Movement_Interface*)(*inbound_itr);

						inbound_link=inbound_movement->inbound_link<Link_Interface*>();

						int t_minus_fftt = -1;

						int link_fftt_cached_simulation_interval = inbound_link->link_fftt_cached_simulation_interval_size<int>();

						if (current_simulation_interval_index >= link_fftt_cached_simulation_interval)
						{
							t_minus_fftt = (current_simulation_interval_index - link_fftt_cached_simulation_interval) % link_fftt_cached_simulation_interval;			
						}

						if (t_minus_fftt > -1)
						{
							int cached_turn_movement_cumulative_shifted_arrived_vehicles=(inbound_movement->cached_turn_movement_cumulative_shifted_arrived_vehicles_array<int*>())[t_minus_fftt];
							inbound_movement->turn_movement_cumulative_shifted_arrived_vehicles<int>(cached_turn_movement_cumulative_shifted_arrived_vehicles);
						}
						else
						{
							inbound_movement->turn_movement_cumulative_shifted_arrived_vehicles<int>(0);
						}
						
						int movement_demand = inbound_movement->turn_movement_cumulative_shifted_arrived_vehicles<int>() - inbound_movement->turn_movement_cumulative_vehicles<int>();
						
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

						//PRINT("\t" << iteration << ": turn movement flow: " << turn_movement_flow);
					}
				}
			}

			facet void node_transfer()
			{
				typedef typename ThisType::scenario_type ScenarioType;
				typedef typename Scenario_Interface<ScenarioType,ThisType> Scenario_Interface;

				Scenario_Interface* scenario=scenario_reference<Scenario_Interface*>();
				
				int current_simulation_interval_index = scenario->current_simulation_interval_index<int>();
				int simulation_interval_length = scenario->simulation_interval_length<int>();
				int current_time = scenario->current_time<int>();

				typedef typename ThisType::outbound_inbound_movements_container_type OutboundInboundType;
				typedef typename ThisType::outbound_inbound_movements_container_element_type OutboundInboundElementType;
				typedef typename Outbound_Inbound_Movements_Interface<OutboundInboundElementType,ThisType> Outbound_Inbound_Movement_Interface;

				typedef typename OutboundInboundElementType::outbound_link_reference_type LinkType;
				typedef typename Link_Interface<LinkType,ThisType> Link_Interface;
				
				typedef typename ThisType::inbound_movements_container_type InboundMovementsType;
				typedef typename ThisType::inbound_movement_type InboundMovementType;
				typedef typename Movement_Interface<InboundMovementType,ThisType> Movement_Interface;

				typedef typename ThisType::rng_stream_type RngStreamType;

				// apply transfers of y(a,a''t) for each turn;
				//int arrived_vehicles = this->network_cumulative_arrived_vehicles;
				//int in_network_vehicles = this->network_in_network_vehicles;
				
				Link_Interface* outbound_link;
				OutboundInboundType& outbound_links_container=outbound_inbound_movements<OutboundInboundType&>();
				typename OutboundInboundType::iterator outbound_itr;

				for(outbound_itr=outbound_links_container.begin(); outbound_itr!=outbound_links_container.end(); outbound_itr++)
				{
					outbound_link=((Outbound_Inbound_Movement_Interface*)(*outbound_itr))->outbound_link_reference<Link_Interface*>();

					Movement_Interface* inbound_movement;
					Link_Interface* inbound_link;

					InboundMovementsType& inbound_links_container = ((Outbound_Inbound_Movement_Interface*)(*outbound_itr))->inbound_movements<InboundMovementsType>();
					typename InboundMovementsType::iterator inbound_itr;
					
					//initialization -> move to network state update
					outbound_link->link_upstream_arrived_vehicles<int>(0);
					outbound_link->link_destination_arrived_vehicles<int>(0);

					for(inbound_itr=inbound_links_container.begin();inbound_itr!=inbound_links_container.end();inbound_itr++)
					{
						inbound_movement=(Movement_Interface*)(*inbound_itr);

						inbound_link=inbound_movement->inbound_link<Link_Interface*>();

						inbound_movement->outbound_link_arrived_time_based_experienced_link_turn_travel_delay<float>(0);
						inbound_movement->inbound_link_departed_time_based_experienced_link_turn_travel_delay<float>(0);

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
							double rng = (this->rng_stream<RngStreamType&>()).RandU01();
							if(rng<=transfer_flow_turn_movement)
							{//partial vehicle, incomplete implementation
								++num_transfer_vehicles_of_turn_movement;
							}
						}

						inbound_movement->movement_transferred<int>(num_transfer_vehicles_of_turn_movement);
						
						if (num_transfer_vehicles_of_turn_movement>0)
						{
							typedef typename InboundMovementType::vehicle_type VehicleType;
							typedef typename Vehicle_Interface<VehicleType,ThisType> Vehicle_Interface;
							
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

								outbound_link->link_upstream_arrived_vehicles<int&>()++;
								outbound_link->link_upstream_cumulative_arrived_vehicles<int&>()++;
								//update turn movement state: N(a',a,t)
								inbound_movement->turn_movement_cumulative_vehicles<int&>()++;

								//update link_turn_travel_delay
								inbound_movement->outbound_link_arrived_time_based_experienced_link_turn_travel_delay<float&>()+=delayed_time;

								//find the departed time
								int departed_time_position = -1;
								
								int num_intervals=scenario->num_simulation_intervals_per_assignment_interval<int>();

								if (current_simulation_interval_index>=num_intervals)
								{
									departed_time_position = (current_simulation_interval_index+num_intervals-delayed_interval)%num_intervals;
								}
								else
								{
									departed_time_position = enter_interval_index;
								}

								inbound_movement->cached_inbound_link_departed_time_based_experienced_link_turn_travel_delay_array<float*>()[departed_time_position]+=delayed_interval*simulation_interval_length;
								
								outbound_link->push_vehicle<Vehicle_Interface*>(vehicle);
							}
						}

						if (num_transfer_vehicles_of_turn_movement>0)
						{
							float delay=inbound_movement->outbound_link_arrived_time_based_experienced_link_turn_travel_delay<float>()/((float)num_transfer_vehicles_of_turn_movement);
							inbound_movement->outbound_link_arrived_time_based_experienced_link_turn_travel_delay<float>(delay);
						}
						else
						{
							if(inbound_movement->movement_rule<Types::Turn_Movement_Rule_Keys>() == Types::Turn_Movement_Rule_Keys::PROHIBITED)
							{
								inbound_movement->outbound_link_arrived_time_based_experienced_link_turn_travel_delay<float>(FLT_MAX);
							}
							else
							{
								if(inbound_movement->movement_demand<float>() == 0)
								{//no demand
									inbound_movement->outbound_link_arrived_time_based_experienced_link_turn_travel_delay<float>(0);
								}
								else
								{//no supply for this turn -- need a prediction -- need to work it later
									int t_minus_one = -1;
									if (current_simulation_interval_index>0)
									{
										t_minus_one = (current_simulation_interval_index-1)%scenario->num_simulation_intervals_per_assignment_interval<int>();

										float delay = simulation_interval_length +
											inbound_movement->cached_outbound_link_arrived_time_based_experienced_link_turn_travel_delay_array<float*>()[t_minus_one];

										inbound_movement->outbound_link_arrived_time_based_experienced_link_turn_travel_delay<float>(delay);
									}
									else
									{
										inbound_movement->outbound_link_arrived_time_based_experienced_link_turn_travel_delay<float>(0);
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
				typedef typename ThisType::scenario_type ScenarioType;
				typedef typename Scenario_Interface<ScenarioType,ThisType> Scenario_Interface;

				Scenario_Interface* scenario=scenario_reference<Scenario_Interface*>();
				
				int current_simulation_interval_index = scenario->current_simulation_interval_index<int>();
				int simulation_interval_length = scenario->simulation_interval_length<int>();
				int current_time = scenario->current_time<int>();

				typedef typename ThisType::outbound_inbound_movements_container_type OutboundInboundType;
				typedef typename ThisType::outbound_inbound_movements_container_element_type OutboundInboundElementType;
				typedef typename Outbound_Inbound_Movements_Interface<OutboundInboundElementType,ThisType> Outbound_Inbound_Movement_Interface;

				typedef typename OutboundInboundElementType::outbound_link_reference_type LinkType;
				typedef typename Link_Interface<LinkType,ThisType> Link_Interface;
				
				typedef typename ThisType::inbound_movements_container_type InboundMovementsType;
				typedef typename ThisType::inbound_movement_type InboundMovementType;
				typedef typename Movement_Interface<InboundMovementType,ThisType> Movement_Interface;


				//network_data.network_cumulative_arrived_vehicles = this->network_cumulative_arrived_vehicles;
				//network_data.network_cumulative_departed_vehicles = this->network_cumulative_departed_vehicles;
				//network_data.network_cumulative_loaded_vehicles = this->network_cumulative_loaded_vehicles;
				//network_data.network_in_network_vehicles = this->network_in_network_vehicles;


				Link_Interface* outbound_link;
				OutboundInboundType& outbound_links_container=outbound_inbound_movements<OutboundInboundType&>();
				typename OutboundInboundType::iterator outbound_itr;

				for (outbound_itr=outbound_links_container.begin(); outbound_itr!=outbound_links_container.end(); outbound_itr++)
				{
					outbound_link=((Outbound_Inbound_Movement_Interface*)(*outbound_itr))->outbound_link_reference<Link_Interface*>();
					
					outbound_link->travel_time<float>(outbound_link->link_fftt<float>());

					int t_fftt = -1;
					int t_bwtt = -1;
					int t_cached_delay = -1;

					if (current_simulation_interval_index>0)
					{
						t_fftt = (current_simulation_interval_index)%outbound_link->link_fftt_cached_simulation_interval_size<int>();
						t_bwtt = (current_simulation_interval_index)%outbound_link->link_bwtt_cached_simulation_interval_size<int>();
						t_cached_delay = (current_simulation_interval_index)%scenario->num_simulation_intervals_per_assignment_interval<int>();
					}
					else
					{
						t_fftt = 0;
						t_bwtt = 0;
						t_cached_delay = 0;
					}

					Movement_Interface* inbound_movement;
					Link_Interface* inbound_link;

					InboundMovementsType& inbound_links_container = ((Outbound_Inbound_Movement_Interface*)(*outbound_itr))->inbound_movements<InboundMovementsType&>();
					typename InboundMovementsType::iterator inbound_itr;

					for(inbound_itr=inbound_links_container.begin();inbound_itr!=inbound_links_container.end();inbound_itr++)
					{
						inbound_movement=(Movement_Interface*)(*inbound_itr);

						inbound_link=inbound_movement->inbound_link<Link_Interface*>();

						int t_plus_fftt = -1;

						if (current_simulation_interval_index>0)
						{
							t_plus_fftt = (current_simulation_interval_index) % (inbound_link->link_fftt_cached_simulation_interval_size<int>());
						}
						else
						{
							t_plus_fftt = 0;
						}

						//cached cumulative shifted arrived vehicles
						inbound_movement->cached_turn_movement_cumulative_shifted_arrived_vehicles_array<int*>()[t_plus_fftt] = inbound_movement->turn_movement_cumulative_arrived_vehicles<int>();
						
						//turn movement delayed time update
						inbound_movement->cached_outbound_link_arrived_time_based_experienced_link_turn_travel_delay_array<int*>()[t_cached_delay] = inbound_movement->outbound_link_arrived_time_based_experienced_link_turn_travel_delay<int>();
						
						inbound_movement->turn_travel_penalty<float>(inbound_movement->outbound_link_arrived_time_based_experienced_link_turn_travel_delay<float>());
						
						float turn_travel_penalty = 0.0;

						for (int t_cached_time=0;t_cached_time<scenario->num_simulation_intervals_per_assignment_interval<int>();t_cached_time++)
						{
							turn_travel_penalty += inbound_movement->cached_outbound_link_arrived_time_based_experienced_link_turn_travel_delay_array<int*>()[t_cached_time];
						}

						turn_travel_penalty = (float) ( turn_travel_penalty/((float)scenario->num_simulation_intervals_per_assignment_interval<int>()) );
						
						inbound_movement->forward_link_turn_travel_time<float>(outbound_link->travel_time<float>()+inbound_movement->turn_travel_penalty<float>());
					}
				}
			}
			
			facet void Initialize()
			{
				schedule_event_local(ThisType,Newells_Conditional,Compute_Step_Flow,0,NULLTYPE);
			}

			declare_facet_conditional(Newells_Conditional)
			{
				Intersection_Interface* _this=(Intersection_Interface*)pthis;
				
				//PRINT("\n" << iteration << "." << sub_iteration << ":\t" << "visiting intersection: " << _this->uuid<int>());
				
				Revision intersection_current_revision=pthis->object_current_revision();
				
				typedef typename ThisType::link_type link_type;
				
				typedef Link_Components::Types::Link_Simulation_Status link_simulation_status_type;

				typedef typename ThisType::outbound_inbound_movements_container_type OutboundInboundType;
				typedef typename ThisType::outbound_inbound_movements_container_element_type OutboundInboundElementType;
				typedef typename Outbound_Inbound_Movements_Interface<OutboundInboundElementType,ThisType> Outbound_Inbound_Movement_Interface;

				typedef typename OutboundInboundElementType::outbound_link_reference_type LinkType;
				typedef typename Link_Interface<LinkType,ThisType> Link_Interface;
				
				typedef typename ThisType::inbound_movements_container_type InboundMovementsType;
				typedef typename ThisType::inbound_movement_type InboundMovementType;
				typedef typename Movement_Interface<InboundMovementType,ThisType> Movement_Interface;

				typedef typename ThisType::scenario_type Scenario_Type;
				typedef Scenario_Interface<Scenario_Type,ThisType> Scenario_Interface;

				if(intersection_current_revision.iteration!=iteration)
				{
					//first visit this iteration, update status
					_this->intersection_simulation_status<Types::Intersection_Simulation_Status>(Types::Intersection_Simulation_Status::NONE_COMPLETE);
				}

				if(_this->intersection_simulation_status<Types::Intersection_Simulation_Status>()==Types::Intersection_Simulation_Status::NONE_COMPLETE)
				{
					Revision links_current_revision=Execution_Object::allocator_template<link_type>::allocator_reference.type_current_revision();

					if(links_current_revision.iteration==iteration)
					{
						//links visited at least once, link_simulation_status is accurate

						//necessary condition is that all intersection's downstream/upstream links are done

						Link_Interface* outbound_link;
						OutboundInboundType& outbound_links_container=_this->outbound_inbound_movements<OutboundInboundType&>();
						typename OutboundInboundType::iterator outbound_itr;

						bool done=true;

						for(outbound_itr=outbound_links_container.begin(); outbound_itr!=outbound_links_container.end(); outbound_itr++)
						{
							outbound_link=((Outbound_Inbound_Movement_Interface*)(*outbound_itr))->outbound_link_reference<Link_Interface*>();

							Link_Interface* inbound_link;
							Movement_Interface* inbound_movement;
							InboundMovementsType& inbound_links_container = ((Outbound_Inbound_Movement_Interface*)(*outbound_itr))->inbound_movements<InboundMovementsType&>();
							
							typename InboundMovementsType::iterator inbound_itr;

							for(inbound_itr=inbound_links_container.begin();inbound_itr!=inbound_links_container.end();inbound_itr++)
							{
								inbound_movement=(Movement_Interface*)(*inbound_itr);
								inbound_link=inbound_movement->inbound_link<Link_Interface*>();

								done=done &&
									(inbound_link->link_simulation_status<link_simulation_status_type>()
									 == link_simulation_status_type::COMPUTE_STEP_FLOW_SUPPLY_UPDATE_COMPLETE);

								if(!done) break;
							}
							
							if(!done) break;
						}

						if(done)
						{
							//PRINT("\t" << "Run Compute_Step_Flow, Return This Iteration");
							
							pthis->Swap_Event((Event)&Intersection_Interface::Compute_Step_Flow<NULLTYPE>);
							response.result=true;
							response.next=iteration;
						}
						else
						{
							//PRINT("\t" << "Compute_Step_Flow_Supply_Update Not Finished, Return This Iteration");
							response.result=false;
							response.next=iteration;
						}
					}
					else
					{
						//link not visited yet

						//PRINT("\t" << "Link Not Visited, Return This Iteration");
						response.result=false;
						response.next=iteration;
					}
				}
				else if(_this->intersection_simulation_status<Types::Intersection_Simulation_Status>()==Types::Intersection_Simulation_Status::COMPUTE_STEP_FLOW_COMPLETE)
				{
					//although not ideal, simply check whether links are completely done this iteration

					Revision link_next_revision=Execution_Object::allocator_template<link_type>::allocator_reference.type_next_check();

					if(link_next_revision.iteration>iteration)
					{
						//PRINT("\t" << "Run Network_State_Update, Return Next Iteration");
						pthis->Swap_Event((Event)&Intersection_Interface::Network_State_Update<NULLTYPE>);
						response.result=true;
						response.next=iteration + _this->scenario_reference<Scenario_Interface*>()->simulation_interval_length<int>();
					}
					else
					{
						//PRINT("\t" << "Compute_Step_Flow_Link_Moving Not Finished, Return This Iteration");
						response.result=false;
						response.next=iteration;
					}
				}
			}

			declare_facet_event(Compute_Step_Flow)
			{
				Intersection_Interface* _this=(Intersection_Interface*)pthis;

				//step 2: turn vehicles updating based on node control and link management, inbound link demand, and outbound link supply
				_this->turn_movement_capacity_update<NULLTYPE>(); 

				//step 3: allocate link supply to inbound turn movements according to a given merging policy
				_this->turn_movement_supply_allocation<NULLTYPE>();

				//step 4: determine turn movement flow rate based on demand, capacity, and supply
				_this->turn_movement_flow_calculation<NULLTYPE>();

				//step 6: node transfer
				_this->node_transfer<NULLTYPE>();

				_this->intersection_simulation_status<Types::Intersection_Simulation_Status>(Types::Intersection_Simulation_Status::COMPUTE_STEP_FLOW_COMPLETE);

				//PRINT("\t\t" << "COMPUTE_STEP_FLOW_COMPLETE");
			}

			declare_facet_event(Network_State_Update)
			{
				Intersection_Interface* _this=(Intersection_Interface*)pthis;

				//step 9: intersection network state update
				_this->network_state_update<NULLTYPE>();

				_this->intersection_simulation_status<Types::Intersection_Simulation_Status>(Types::Intersection_Simulation_Status::NETWORK_STATE_UPDATE_COMPLETE);

				//PRINT("\t\t" << "NETWORK_STATE_UPDATE_COMPLETE");
			}
		};
	}
}

using namespace Intersection_Components::Interfaces;