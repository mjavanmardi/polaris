#pragma once
#include "Intersection_Prototype.h"
#include "Turn_Movement_Prototype.h"
//#include "Scenario_Prototype.h"
#include "Link_Implementation.h"

namespace Intersection_Components
{
	namespace Types
	{
	
	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{
		implementation struct Outbound_Inbound_Movements_Implementation:public Polaris_Component<MasterType,INHERIT(Outbound_Inbound_Movements_Implementation),Data_Object>
		{
			typedef typename Polaris_Component<MasterType,INHERIT(Outbound_Inbound_Movements_Implementation),Data_Object>::ComponentType ComponentType;

			// pointer to the outbound link
			m_prototype(Null_Prototype,typename MasterType::link_type, outbound_link_reference, NONE, NONE);
			
			// container of inbound movements
			m_container(std::vector<typename MasterType::movement_type*>, inbound_movements, NONE, NONE);

			typedef  Link_Components::Prototypes::Link<type_of(outbound_link_reference)> _Link_Interface;
			typedef  Turn_Movement_Components::Prototypes::Movement<typename remove_pointer<typename  type_of(inbound_movements)::value_type>::type>  _Movement_Interface;
			typedef  Random_Access_Sequence<type_of(inbound_movements), _Movement_Interface*> _Movements_Container_Interface;

			typedef Network_Components::Prototypes::Network<typename MasterType::network_type> _Network_Interface;
			typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;

			// update state of outbound link and inbound movements
			template<typename TargetType> void update_state()
			{
				//int current_simulation_interval_index = ((_Network_Interface*)_global_network)->template current_simulation_interval_index<int>();
				((_Link_Interface*)_outbound_link_reference)->template travel_time<float>(((_Link_Interface*)_outbound_link_reference)->template link_fftt<float>());

				_Movement_Interface* inbound_movement;
				typename _Movements_Container_Interface::iterator inbound_itr;
				for(inbound_itr=_inbound_movements.begin();inbound_itr!=_inbound_movements.end();inbound_itr++)
				{
					inbound_movement=(_Movement_Interface*)(*inbound_itr);
					inbound_movement->template update_state<int>();
				}
			}

			// allocate supply based on driving rule
			template<typename TargetType> void supply_allocation_based_on_driving_rule()
			{

				_Movement_Interface* inbound_movement;
				_Link_Interface* inbound_link;
				typename _Movements_Container_Interface::iterator inbound_itr;

				float outbound_link_capacity = ((_Link_Interface*)_outbound_link_reference)->template link_capacity<float>();
				int inbound_turn_movement_size=(int)_inbound_movements.size();
				if(inbound_turn_movement_size >= 1)
				{
					if (inbound_turn_movement_size == 1)
					{
						((_Movement_Interface*)_inbound_movements[0])->template movement_supply<float>(((_Link_Interface*)_outbound_link_reference)->template link_supply<float>());
						//((_Movement_Interface*)_inbound_movements[0])->template movement_capacity<float>(outbound_link_capacity * 1.0f);
					}
					else
					{//divide space based on merging policy
						///driving rule with priority : protected movement v.s. permitted movements
						///only one movement can be specified as protected movement for merging to a link, other movements should be yielded the protected movement
						///there will be capacity lose for such driving rule for permitted movements
						///if there is no protected movement, then all the movements are all permitted and thus share the capacity based on demand
						int num_allowed_movements = 0;
						float total_transfer_demand = 0.0f;
						_Movement_Interface* protected_turn_movement = nullptr;

						for(inbound_itr=_inbound_movements.begin();inbound_itr!=_inbound_movements.end();inbound_itr++)
						{
							inbound_movement=(_Movement_Interface*)(*inbound_itr);
							inbound_link=inbound_movement->template inbound_link<_Link_Interface*>();
							total_transfer_demand += inbound_movement->template movement_demand<float>();
							if (inbound_movement->template merge_priority<int>() == 1)
							{
								protected_turn_movement = inbound_movement;
							}
							if (inbound_movement->template movement_rule<int>() ==  Turn_Movement_Components::Types::Turn_Movement_Rule_Keys::ALLOWED)
							{
								num_allowed_movements++;
							}
						}
							
						if (total_transfer_demand>0)
						{
							if (total_transfer_demand<=((_Link_Interface*)_outbound_link_reference)->template link_supply<float>())
							{//case 1: total_transfer_demand<= link_supply_array
								for(inbound_itr=_inbound_movements.begin();inbound_itr!=_inbound_movements.end();inbound_itr++)
								{
									inbound_movement=(_Movement_Interface*)(*inbound_itr);
									//inbound_movement->template movement_supply<float>(inbound_movement->template movement_demand<float>());
									inbound_movement->template movement_supply<float>(((_Link_Interface*)_outbound_link_reference)->template link_supply<float>());
									//if (((_Scenario_Interface*)_global_scenario)->template maximum_flow_rate_constraints_enforced<bool>())
									//	inbound_movement->template movement_capacity<float>(outbound_link_capacity / (float)inbound_turn_movement_size);
									//else
									//	inbound_movement->template movement_capacity<float>(outbound_link_capacity);
								}
							}
							else
							{//case 2: total_transfer_demand<= link_supply_array
								if (protected_turn_movement != nullptr)
								{
									if (protected_turn_movement->template movement_demand<float>() >= ((_Link_Interface*)_outbound_link_reference)->template link_supply<float>())
									{
										protected_turn_movement->template movement_supply<float>(((_Link_Interface*)_outbound_link_reference)->template link_supply<float>());
										for(inbound_itr=_inbound_movements.begin();inbound_itr!=_inbound_movements.end();inbound_itr++)
										{
											inbound_movement=(_Movement_Interface*)(*inbound_itr);
											if (inbound_movement->template internal_id<int>() != protected_turn_movement->template internal_id<int>())
											{
												inbound_movement->template movement_supply<float>(0.0);
												//if (((_Scenario_Interface*)_global_scenario)->template maximum_flow_rate_constraints_enforced<bool>())
												//	inbound_movement->template movement_capacity<float>(0.0f);
												//else
												//	inbound_movement->template movement_capacity<float>(outbound_link_capacity);
												
											}
										}
									}
									else
									{
										protected_turn_movement->template movement_supply<float>(protected_turn_movement->template movement_demand<float>());
								
										total_transfer_demand -= protected_turn_movement->template movement_demand<float>();
										float link_supply_leftover = ((_Link_Interface*)_outbound_link_reference)->template link_supply<float>() - protected_turn_movement->template movement_supply<float>();
								
										for(inbound_itr=_inbound_movements.begin();inbound_itr!=_inbound_movements.end();inbound_itr++)
										{
											inbound_movement=(_Movement_Interface*)(*inbound_itr);
											if (inbound_movement->template internal_id<int>() != protected_turn_movement->template internal_id<int>())
											{
												inbound_movement->template movement_supply<float>(link_supply_leftover * inbound_movement->template movement_demand<float>() *1.0f / (total_transfer_demand*1.0f));
/*												if (((_Scenario_Interface*)_global_scenario)->template maximum_flow_rate_constraints_enforced<bool>())
												{
													if (((_Link_Interface*)_outbound_link_reference)->template link_supply<float>() == 0.0f)
														inbound_movement->template movement_capacity<float>(0.0f);
													else
														inbound_movement->template movement_capacity<float>(outbound_link_capacity * inbound_movement->template movement_supply<float>() / ((_Link_Interface*)_outbound_link_reference)->template link_supply<float>());
												}
												else
													inbound_movement->template movement_capacity<float>(outbound_link_capacity);	*/											
											}
										}
									}
								}
								else
								{
									for(inbound_itr=_inbound_movements.begin();inbound_itr!=_inbound_movements.end();inbound_itr++)
									{
										inbound_movement=(_Movement_Interface*)(*inbound_itr);
										inbound_movement->template movement_supply<float>(((_Link_Interface*)_outbound_link_reference)->template link_supply<float>() * inbound_movement->template movement_demand<float>() *1.0f / (total_transfer_demand*1.0f));
										//if (((_Scenario_Interface*)_global_scenario)->template maximum_flow_rate_constraints_enforced<bool>())
										//	inbound_movement->template movement_capacity<float>(outbound_link_capacity * inbound_movement->template movement_demand<float>() *1.0f / (total_transfer_demand*1.0f));
										//else
										//	inbound_movement->template movement_capacity<float>(outbound_link_capacity);
										
									}
										
								}
							}
						}
						else
						{
							for(inbound_itr=_inbound_movements.begin();inbound_itr!=_inbound_movements.end();inbound_itr++)
							{
								inbound_movement=(_Movement_Interface*)(*inbound_itr);
						
								if (inbound_movement->template movement_rule<int>() == Turn_Movement_Components::Types::Turn_Movement_Rule_Keys::ALLOWED)
								{
									inbound_movement->template movement_supply<float>(((_Link_Interface*)_outbound_link_reference)->template link_supply<float>() / (num_allowed_movements*1.0f));
									//if (((_Scenario_Interface*)_global_scenario)->template maximum_flow_rate_constraints_enforced<bool>())
									//	inbound_movement->template movement_capacity<float>(outbound_link_capacity / float(num_allowed_movements));
									//else
									//	inbound_movement->template movement_capacity<float>(outbound_link_capacity);
								}
								else
								{
									inbound_movement->template movement_supply<float>(0.0f);
									//if (((_Scenario_Interface*)_global_scenario)->template maximum_flow_rate_constraints_enforced<bool>())
									//	inbound_movement->template movement_capacity<float>(0.0f);
									//else
									//	inbound_movement->template movement_capacity<float>(outbound_link_capacity);
									
								}

							}
						}

					}
				}
			}

			// compute total transfer demand
			template<typename TargetType> float compute_total_transfer_demand()
			{
				float total_transfer_demand = 0.0f;
				typename _Movements_Container_Interface::iterator inbound_itr;
				for(inbound_itr=_inbound_movements.begin();inbound_itr!=_inbound_movements.end();inbound_itr++)
				{
					total_transfer_demand += ((_Movement_Interface*)(*inbound_itr))->template movement_demand<float>();
				}
				return total_transfer_demand;
			}

			// allocate supply proprotional to demand
			template<typename TargetType> void supply_allocation_proportion_to_demand()
			{
				_Movement_Interface* inbound_movement;

				typename _Movements_Container_Interface::iterator inbound_itr;

				int inbound_turn_movement_size=(int)_inbound_movements.size();
				float outbound_link_capacity = ((_Link_Interface*)_outbound_link_reference)->template link_capacity<float>();
				if (inbound_turn_movement_size == 1)
				{//only one inbound turn movement - allocate full space of outbound link to inbound turn movement
					inbound_movement = (_Movement_Interface*)(*_inbound_movements.begin());
					inbound_movement->template movement_supply<float>(((_Link_Interface*)_outbound_link_reference)->template link_supply<float>());
					//inbound_movement->template movement_capacity<float>(outbound_link_capacity);
				}
				else
				{//divide space based on merging policy
					///propotional to demand
					float total_transfer_demand = compute_total_transfer_demand<float>();
					for(inbound_itr=_inbound_movements.begin();inbound_itr!=_inbound_movements.end();inbound_itr++)
					{
						inbound_movement=(_Movement_Interface*)(*inbound_itr);

						if (total_transfer_demand>0)
						{
							if (total_transfer_demand<=((_Link_Interface*)_outbound_link_reference)->template link_supply<float>())
							{//case 1: total_transfer_demand<= link_supply_array
								inbound_movement->template movement_supply<float>(inbound_movement->template movement_demand<float>());
							}
							else
							{//case 2: total_transfer_demand<= link_supply_array
								inbound_movement->template movement_supply<float>(((_Link_Interface*)_outbound_link_reference)->template link_supply<float>() * inbound_movement->template movement_demand<float>() *1.0f / (total_transfer_demand*1.0f));
							}
							//if (((_Scenario_Interface*)_global_scenario)->template maximum_flow_rate_constraints_enforced<bool>())
							//	inbound_movement->template movement_capacity<float>(outbound_link_capacity * inbound_movement->template movement_demand<float>() *1.0f / (total_transfer_demand*1.0f));
							//else
							//	inbound_movement->template movement_capacity<float>(outbound_link_capacity);
							
						}
						else
						{
							inbound_movement->template movement_supply<float>(((_Link_Interface*)_outbound_link_reference)->template link_supply<float>() / (inbound_turn_movement_size*1.0f));
/*							if (((_Scenario_Interface*)_global_scenario)->template maximum_flow_rate_constraints_enforced<bool>())
								inbound_movement->template movement_capacity<float>(outbound_link_capacity / float(inbound_turn_movement_size));
							else
								inbound_movement->template movement_capacity<float>(outbound_link_capacity);*/							
							
						}	
					}
				}
			}

			template<typename TargetType> void supply_allocation_proportion_to_link()
			{
				int inbound_turn_movement_size=(int)_inbound_movements.size();
				if (inbound_turn_movement_size >= 1)
				{
					_Movement_Interface* inbound_movement;
					typename _Movements_Container_Interface::iterator inbound_itr;
					float outbound_link_supply = ((_Link_Interface*)_outbound_link_reference)->template link_supply<float>();
					float supply_per_movement = outbound_link_supply / (inbound_turn_movement_size * 1.0f);
					float outbound_link_capacity = ((_Link_Interface*)_outbound_link_reference)->template link_capacity<float>();
					for(inbound_itr=_inbound_movements.begin();inbound_itr!=_inbound_movements.end();inbound_itr++)
					{
						inbound_movement=(_Movement_Interface*)(*inbound_itr);
						inbound_movement->template movement_supply<float>(supply_per_movement);
/*						if (((_Scenario_Interface*)_global_scenario)->template maximum_flow_rate_constraints_enforced<bool>())
							inbound_movement->template movement_capacity<float>(outbound_link_capacity / (float)inbound_turn_movement_size);
						else
							inbound_movement->template movement_capacity<float>(outbound_link_capacity);	*/		
						
					}
				}
			};

			template<typename TargetType> void supply_allocation_proportion_to_lane()
			{
				int inbound_turn_movement_size=(int)_inbound_movements.size();
				if (inbound_turn_movement_size >= 1)
				{
					_Movement_Interface* inbound_movement;
					if (((_Link_Interface*)_outbound_link_reference)->template num_inbound_turn_lanes<int>() > 0)
					{
						typename _Movements_Container_Interface::iterator inbound_itr;
						float outbound_link_supply = ((_Link_Interface*)_outbound_link_reference)->template link_supply<float>();
						float outbound_link_capacity = ((_Link_Interface*)_outbound_link_reference)->template link_capacity<float>();
						int num_inbound_turn_lanes = ((_Link_Interface*)_outbound_link_reference)->template num_inbound_turn_lanes<int>();
						float supply_per_lane = outbound_link_supply / (num_inbound_turn_lanes * 1.0f);
						for(inbound_itr=_inbound_movements.begin();inbound_itr!=_inbound_movements.end();inbound_itr++)
						{
							inbound_movement=(_Movement_Interface*)(*inbound_itr);
							inbound_movement->template movement_supply<float>(supply_per_lane * inbound_movement->template num_turn_lanes<int>() * 1.0f);
/*							if (((_Scenario_Interface*)_global_scenario)->template maximum_flow_rate_constraints_enforced<bool>())
								inbound_movement->template movement_capacity<float>(outbound_link_capacity * inbound_movement->template num_turn_lanes<float>() / (float)num_inbound_turn_lanes);
							else
								inbound_movement->template movement_capacity<float>(outbound_link_capacity);*/								
							
						}
					}
					else
					{
						cout << "link " << ((_Link_Interface*)_outbound_link_reference)->template uuid<int>()
							<< " does not have inbound link turn lanes. "<< endl;
						assert(false);
					}
				}
			};
		};
	
		implementation struct Inbound_Outbound_Movements_Implementation:public Polaris_Component<MasterType,INHERIT(Inbound_Outbound_Movements_Implementation),Data_Object>
		{
			typedef typename Polaris_Component<MasterType,INHERIT(Inbound_Outbound_Movements_Implementation),Data_Object>::ComponentType ComponentType;

			m_prototype(Null_Prototype,typename MasterType::link_type, inbound_link_reference, NONE, NONE);
			m_container(std::vector<typename MasterType::movement_type*>, outbound_movements, NONE, NONE);

			typedef  Link_Components::Prototypes::Link<type_of(inbound_link_reference)> _Link_Interface;
			typedef  Turn_Movement_Components::Prototypes::Movement<typename remove_pointer<typename  type_of(outbound_movements)::value_type>::type>  _Movement_Interface;
			typedef  Random_Access_Sequence<type_of(outbound_movements), _Movement_Interface*> _Movements_Container_Interface;

			typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;

			//=================================================================================
			/// ERROR HERE..........
			//TODO: check this - causing an error......................
			template<typename TargetType> void link_capacity_allocation()
			{
				// Computation is a disaggregated interpretation of HCM Equation 17-1

				_Link_Interface* link = (_Link_Interface*)_inbound_link_reference;
				
				int num_lanes = link->template num_lanes<int>();
				
				// skip non-arterial/local links

				Link_Components::Types::Link_Type_Keys link_type = link->template link_type<Link_Components::Types::Link_Type_Keys>();

				// skip non-arterial/local links
				if(link_type == Link_Components::Types::Link_Type_Keys::FREEWAY || 
					link_type == Link_Components::Types::Link_Type_Keys::EXPRESSWAY || 
					link_type == Link_Components::Types::Link_Type_Keys::ON_RAMP || 
					link_type == Link_Components::Types::Link_Type_Keys::OFF_RAMP)
				{
					return;
				}
				
				Link_Components::Implementations::Pocket_Data* pocket_data = link->template pocket_data<Link_Components::Implementations::Pocket_Data*>();

				if(!pocket_data->num_pockets_left || !pocket_data->num_pockets_right)
				{
					const float interval = (float)((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>();

					const float interval_capacity = (1800.0f/(60.0f*60.0f))*interval;

					//std::deque<_Movement_Interface*> through_movements;
					//std::deque<_Movement_Interface*> right_movements;
					//std::deque<_Movement_Interface*> left_movements;
					
					_Movement_Interface* outbound_movement;
					typename _Movements_Container_Interface::iterator outbound_itr;
					
					float total_demand = 0.0f;
					float left_demand = 0.0f;
					float right_demand = 0.0f;
					float through_demand = 0.0f;

					float total_capacity = interval_capacity*((float)num_lanes)+interval_capacity*((float)pocket_data->num_pockets_left)+interval_capacity*((float)pocket_data->num_pockets_right);

					for(outbound_itr=_outbound_movements.begin();outbound_itr!=_outbound_movements.end();outbound_itr++)
					{
						outbound_movement=(_Movement_Interface*)(*outbound_itr);
						
						total_demand += outbound_movement->template movement_demand<float>();

						Turn_Movement_Components::Types::Turn_Movement_Type_Keys type = outbound_movement->template movement_type<Turn_Movement_Components::Types::Turn_Movement_Type_Keys>();

						if(type == Turn_Movement_Components::Types::U_TURN || type == Turn_Movement_Components::Types::LEFT_TURN)
						{
							left_demand += outbound_movement->template movement_demand<float>();
						}
						else if(type == Turn_Movement_Components::Types::RIGHT_TURN)
						{
							right_demand += outbound_movement->template movement_demand<float>();
						}
						else if(type == Turn_Movement_Components::Types::THROUGH_TURN)
						{
							through_demand += outbound_movement->template movement_demand<float>();
						}
					}

					if(total_demand <= 0.5f) return;

					// ascertain whether the link is constrained by left, right, and center
					
					float movement_supply;

					/// if there are 2 or more lanes, only the right (or left) lane would be backed up, the left (or right) lanes would have full capacity
					//	in addition, turners will only utilize the rightmost or leftmost lane
					/// this means the turn movement can receive no more than one lane's worth of capacity
					//	this one lane's capacity should also be subject to the percentage of demand for that lane
					/// however, we don't have a good way of estimating the % of through movers in one of those lanes; it should be less than an even split because they will avoid queues
					//	until we can get an estimate for that just compute proportion to demand and cap the capacity

					// Additionally, there should be some mechanism to let unused supply flow back to other incident links

					if(!pocket_data->num_pockets_left && !pocket_data->num_pockets_right)
					{
						float movement_remain = 0.0;
						float total_remain = 0.0;

						for(outbound_itr=_outbound_movements.begin();outbound_itr!=_outbound_movements.end();outbound_itr++)
						{
							outbound_movement=(_Movement_Interface*)(*outbound_itr);

							// movement gets fraction of capacity proportional to fraction of demand
							movement_supply = outbound_movement->template movement_supply<float>();

							// if movement demand is 0, then the supply will be 0, which makes the demand computation meaningless, but ultimately gives the same result

							Turn_Movement_Components::Types::Turn_Movement_Type_Keys type = outbound_movement->template movement_type<Turn_Movement_Components::Types::Turn_Movement_Type_Keys>();

							//TODO: evaluate this code 
							/// THIS HAS BEEN UPDATED TO REPLACE THE FLOOR WHICH WAS ROUNDING DOWN AND CREATING JAMS FOR LOW DEMAND MOVEMENTS
							//	i.e the interval capacity when simulation interval is 6s = 3 veh. if less then 33% of demand is allocated to a specific movement, the flow for that movement will always be 0 until the demand percentage increases to greter than 33%.
							/// this causes a lot of problems for turn movements - JAA 1/24/17

							float movement_cap = total_capacity * (outbound_movement->template movement_demand<float>() / total_demand);
							movement_remain = movement_cap - floor(movement_cap);
							if (GLOBALS::Uniform_RNG.Next_Rand<float>() < movement_remain + total_remain)
							{
								total_remain -= movement_cap;
								movement_cap += 1.0;
							}
							else total_remain += movement_remain;

							if(type == Turn_Movement_Components::Types::THROUGH_TURN)
							{
								movement_supply = floor(min( movement_supply, movement_cap));
							}
							else
							{
								movement_supply = floor(min(interval_capacity,min( movement_supply, movement_cap)));
							}

							//((_Movement_Interface::Component_Type*)outbound_movement)->pocket_movement_supply = ceil( total_capacity * ( outbound_movement->template movement_demand<float>() / total_demand));
							//if(movement_supply < outbound_movement->template movement_supply<float>() && movement_supply!=0.0f && outbound_movement->template movement_demand<float>()!=total_demand)
							//{
							//	cout << "Left and Right Missing: " << outbound_movement->template movement_supply<float>() << "," << total_capacity << "," << outbound_movement->template movement_demand<float>() << "," << total_demand << "," << movement_supply << endl;
							//}
							outbound_movement->template movement_supply<float>(movement_supply);
						}
					}
					else if(!pocket_data->num_pockets_right)
					{
						// there are left pockets, but not right pockets, you can deduct the left turn demand from the total demand
						// also deduct the left turn capacity from the total capacity

						float shared_demand = total_demand - left_demand;
						float shared_capacity = total_capacity - interval_capacity*((float)pocket_data->num_pockets_left);
						float total_remain = 0.0;

						for(outbound_itr=_outbound_movements.begin();outbound_itr!=_outbound_movements.end();outbound_itr++)
						{
							outbound_movement=(_Movement_Interface*)(*outbound_itr);

							Turn_Movement_Components::Types::Turn_Movement_Type_Keys type = outbound_movement->template movement_type<Turn_Movement_Components::Types::Turn_Movement_Type_Keys>();

							if(type == Turn_Movement_Components::Types::U_TURN || type == Turn_Movement_Components::Types::LEFT_TURN)
							{
								// skip these, they get full supply
								continue;
							}

							// movement gets fraction of capacity proportional to fraction of demand
							movement_supply = outbound_movement->template movement_supply<float>();
								
							// if movement demand is 0, then the supply will be 0, which makes the demand computation meaningless, but ultimately gives the same result

							// allocate remaining capacity randomly
							float movement_cap = shared_capacity * (outbound_movement->template movement_demand<float>() / total_demand);
							float movement_remain = movement_cap - floor(movement_cap);
							if (GLOBALS::Uniform_RNG.Next_Rand<float>() < movement_remain + total_remain)
							{
								total_remain -= movement_cap;
								movement_cap += 1.0;
							}
							else total_remain += movement_remain;


							if(type == Turn_Movement_Components::Types::THROUGH_TURN)
							{
								movement_supply = floor(min( movement_supply, movement_cap ));
							}
							else
							{
								movement_supply = floor(min(interval_capacity,min( movement_supply, movement_cap )));
							}

							//((_Movement_Interface::Component_Type*)outbound_movement)->pocket_movement_supply = ceil( shared_capacity * ( outbound_movement->template movement_demand<float>() / shared_demand));

							//if(movement_supply < outbound_movement->template movement_supply<float>() && movement_supply!=0.0f && outbound_movement->template movement_demand<float>()!=shared_demand)
							//{
							//	cout << "Right Missing: " << outbound_movement->template movement_supply<float>() << "," << shared_capacity << "," << outbound_movement->template movement_demand<float>() << "," << shared_demand << "," << movement_supply << endl;
							//}
							outbound_movement->template movement_supply<float>(movement_supply);
						}
					}
					else if(!pocket_data->num_pockets_left)
					{
						float shared_demand = total_demand - right_demand;
						float shared_capacity = total_capacity - interval_capacity*((float)pocket_data->num_pockets_right);
						float total_remain = 0.0;

						for(outbound_itr=_outbound_movements.begin();outbound_itr!=_outbound_movements.end();outbound_itr++)
						{
							outbound_movement=(_Movement_Interface*)(*outbound_itr);

							Turn_Movement_Components::Types::Turn_Movement_Type_Keys type = outbound_movement->template movement_type<Turn_Movement_Components::Types::Turn_Movement_Type_Keys>();

							if(type == Turn_Movement_Components::Types::RIGHT_TURN)
							{
								// skip these, they get full supply
								continue;
							}

							// movement gets fraction of capacity proportional to fraction of demand
							movement_supply = outbound_movement->template movement_supply<float>();

							// allocate remaining capacity randomly
							float movement_cap = shared_capacity * (outbound_movement->template movement_demand<float>() / total_demand);
							float movement_remain = movement_cap - floor(movement_cap);
							if (GLOBALS::Uniform_RNG.Next_Rand<float>() < movement_remain + total_remain)
							{
								total_remain -= movement_cap;
								movement_cap += 1.0;
							}
							else total_remain += movement_remain;

							
							// if movement demand is 0, then the supply will be 0, which makes the demand computation meaningless, but ultimately gives the same result
							if(type == Turn_Movement_Components::Types::THROUGH_TURN)
							{
								movement_supply = floor(min( movement_supply, movement_cap ));
							}
							else
							{
								movement_supply = floor(min(interval_capacity,min( movement_supply, movement_cap )));
							}
							
							//((_Movement_Interface::Component_Type*)outbound_movement)->pocket_movement_supply = ceil( shared_capacity * ( outbound_movement->template movement_demand<float>() / shared_demand));

							//if(movement_supply < outbound_movement->template movement_supply<float>() && movement_supply!=0.0f && outbound_movement->template movement_demand<float>()!=shared_demand)
							//{
							//	cout << "Left Missing: " << outbound_movement->template movement_supply<float>() << "," << shared_capacity << "," << outbound_movement->template movement_demand<float>() << "," << shared_demand << "," << movement_supply << endl;
							//}

							outbound_movement->template movement_supply<float>(movement_supply);
						}
					}
				}
			}
		};
		
		implementation struct Intersection_Implementation:public Polaris_Component<MasterType,INHERIT(Intersection_Implementation),Execution_Object>
		{
			typedef typename Polaris_Component<MasterType,INHERIT(Intersection_Implementation),Execution_Object>::ComponentType ComponentType;

			m_data(int, area_type, NONE, NONE);


			m_data(int, uuid, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(std::string, dbid, NONE, NONE);
			m_data(int, internal_id, NONE, NONE);
			m_data(float, x_position, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(float, y_position, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(float, z_position, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(int, zone, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(std::string, agency, NONE, NONE);
			m_data(std::string, street, NONE, NONE);
			m_data(std::string, name, NONE, NONE);
			m_data(std::string, description, NONE, NONE);
			m_data(Intersection_Components::Types::Intersection_Type_Keys, intersection_type, NONE, NONE);
			m_container(std::vector<typename MasterType::link_type*>, inbound_links, NONE, NONE);
			m_container(std::vector<typename MasterType::link_type*>, outbound_links, NONE, NONE);
			/*m_container(std::vector<typename MasterType::link_type*>, inbound_walk_links, NONE, NONE);
			m_container(std::vector<typename MasterType::link_type*>, outbound_walk_links, NONE, NONE);
			m_container(std::vector<typename MasterType::link_type*>, inbound_transit_links, NONE, NONE);
			m_container(std::vector<typename MasterType::link_type*>, outbound_transit_links, NONE, NONE);*/
			m_container(std::vector<typename MasterType::outbound_inbound_movements_type*>, outbound_inbound_movements, NONE, NONE);
			m_container(std::vector<typename MasterType::inbound_outbound_movements_type*>, inbound_outbound_movements, NONE, NONE);
			//m_data(RNG_Components::RngStream, rng_stream, NONE, NONE);
			m_prototype(Null_Prototype,typename MasterType::network_type, network_reference, NONE, NONE);
			m_prototype(Null_Prototype,typename MasterType::intersection_control_type, intersection_control, NONE, NONE);

			typedef typename MasterType::vehicle_type vehicle_type;
//			member_component(typename MasterType::SIGNAL_TYPE,signal, none, none);
			
			
			typedef Intersection_Components::Prototypes::Outbound_Inbound_Movements<typename remove_pointer<typename  type_of(outbound_inbound_movements)::value_type>::type>  _Outbound_Inbound_Movements_Interface;
			typedef Random_Access_Sequence<type_of(outbound_inbound_movements), _Outbound_Inbound_Movements_Interface*> _Outbound_Inbound_Movements_Container_Interface;

			typedef Turn_Movement_Components::Prototypes::Movement<typename remove_pointer< typename _Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements)::value_type>::type>  _Inbound_Movement_Interface;
			typedef Random_Access_Sequence< typename _Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements), _Inbound_Movement_Interface*> _Inbound_Movements_Container_Interface;


			typedef Intersection_Components::Prototypes::Inbound_Outbound_Movements<typename remove_pointer<typename  type_of(inbound_outbound_movements)::value_type>::type>  _Inbound_Outbound_Movements_Interface;
			typedef Random_Access_Sequence<type_of(inbound_outbound_movements), _Inbound_Outbound_Movements_Interface*> _Inbound_Outbound_Movements_Container_Interface;

			typedef Turn_Movement_Components::Prototypes::Movement<typename remove_pointer< typename _Inbound_Outbound_Movements_Interface::get_type_of(outbound_movements)::value_type>::type>  _Outbound_Movement_Interface;
			typedef Random_Access_Sequence< typename _Inbound_Outbound_Movements_Interface::get_type_of(outbound_movements), _Outbound_Movement_Interface*> _Outbound_Movements_Container_Interface;



			typedef Vehicle_Components::Prototypes::Vehicle<typename remove_pointer< typename _Outbound_Movement_Interface::get_type_of(vehicles_container)::value_type>::type>  _Vehicle_Interface;
			typedef Back_Insertion_Sequence< typename _Outbound_Movement_Interface::get_type_of(vehicles_container), _Vehicle_Interface*> _Vehicles_Container_Interface;

			typedef Link_Components::Prototypes::Link< typename _Outbound_Inbound_Movements_Interface::get_type_of(outbound_link_reference)> _Link_Interface;
			typedef Movement_Plan_Components::Prototypes::Movement_Plan< typename _Vehicle_Interface::get_type_of(movement_plan)> _Movement_Plan_Interface;
			typedef Intersection_Control_Components::Prototypes::Intersection_Control<type_of(intersection_control)> _Intersection_Control_Interface;
			typedef Intersection_Control_Components::Prototypes::Control_Plan< typename _Intersection_Control_Interface::get_type_of(current_control_plan)> _Control_Plan_Interface;
			typedef Network_Components::Prototypes::Network<typename MasterType::network_type> _Network_Interface;
			typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;
			typedef Intersection<typename MasterType::intersection_type> _Intersection_Interface;

			typedef typename MasterType::outbound_inbound_movements_type _outbound_inbound_movements_component_type;
			typedef typename MasterType::inbound_outbound_movements_type _inbound_outbound_movements_component_type;
			typedef typename MasterType::link_type _link_component_type;
			typedef typename MasterType::turn_movement_type _movement_component_type;
			// transfer vehicles 
			template<typename TargetType> void node_transfer()
			{
				typename _Outbound_Inbound_Movements_Container_Interface::iterator outbound_itr;
				_Link_Interface* outbound_link;
				for(outbound_itr=_outbound_inbound_movements.begin(); outbound_itr!=_outbound_inbound_movements.end(); outbound_itr++)
				{
					outbound_link=((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->template outbound_link_reference<_Link_Interface*>();
					_Inbound_Movement_Interface* inbound_movement;
					_Link_Interface* inbound_link;
					_Inbound_Movements_Container_Interface& inbound_links_container = ((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->template inbound_movements<_Inbound_Movements_Container_Interface&>();
					typename _Inbound_Movements_Container_Interface::iterator inbound_itr;
					outbound_link->template link_upstream_arrived_vehicles<int>(0.0);
					outbound_link->template link_destination_arrived_vehicles<int>(0.0);
					for(inbound_itr=inbound_links_container.begin();inbound_itr!=inbound_links_container.end();inbound_itr++)
					{
						inbound_movement=(_Inbound_Movement_Interface*)(*inbound_itr);
						inbound_link=inbound_movement->template inbound_link<_Link_Interface*>();
						if(outbound_itr==_outbound_inbound_movements.begin())
						{
							//initialization
							inbound_link->template link_downstream_departed_vehicles<int>(0.0);
						}
						inbound_movement->template transfer_vehicles<RNG_Components::RngStream&>();
					}
				}
			}

			// accept a vehicle
			template<typename TargetType> void accept_vehicle(void* vehicle)
			{

				typename _Outbound_Inbound_Movements_Container_Interface::iterator outbound_itr;
				_Link_Interface* outbound_link;
				for (outbound_itr=_outbound_inbound_movements.begin(); outbound_itr!=_outbound_inbound_movements.end(); outbound_itr++)
				{
					outbound_link=((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->template outbound_link_reference<_Link_Interface*>();
					_Inbound_Movement_Interface* inbound_movement;
					_Link_Interface* inbound_link;
					_Inbound_Movements_Container_Interface& inbound_movements = ((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->template inbound_movements<_Inbound_Movements_Container_Interface&>();
					typename _Inbound_Movements_Container_Interface::iterator inbound_itr;
					for(inbound_itr=inbound_movements.begin();inbound_itr!=inbound_movements.end();inbound_itr++)
					{
						inbound_movement=(_Inbound_Movement_Interface*)(*inbound_itr);
						inbound_link=inbound_movement->template inbound_link<_Link_Interface*>();
						_Movement_Plan_Interface* mp = ((_Vehicle_Interface*)vehicle)->template movement_plan<_Movement_Plan_Interface*>();
						if(mp->template next_link<_Link_Interface*>()==outbound_link && mp->template current_link<_Link_Interface*>()==inbound_link)
						{
							//_Detector_Interface* detector;
							//detector = inbound_movement->template detector<_Detector_Interface*>();
							//if (detector != NULL) detector->template detect_vehicle<int>();
							inbound_movement->template accept_vehicle<void*>(vehicle);
						}
					}
				}
			}

			// allocate supply among turn movements
			template<typename TargetType> void turn_movement_supply_allocation()
			{
				Types::Intersection_Type_Keys control_type = ((_Intersection_Control_Interface*)_intersection_control)->template current_control_plan<_Control_Plan_Interface*>()->template control_type<Types::Intersection_Type_Keys>();
				switch(control_type)
				{
					case Types::NO_CONTROL:
						//supply_allocation_proportion_to_link<TargetType>();
						supply_allocation_based_on_merging_model<TargetType>();
						break;
					case Types::YIELD_SIGN:
						//supply_allocation_based_on_driving_rule<TargetType>();
						supply_allocation_based_on_merging_model<TargetType>();
						break;
					case Types::ALL_WAY_STOP_SIGN:
						//supply_allocation_proportion_to_demand<TargetType>();
						supply_allocation_based_on_merging_model<TargetType>();
						break;
					case Types::TWO_WAY_STOP_SIGN:
						//supply_allocation_based_on_driving_rule<TargetType>();
						supply_allocation_based_on_merging_model<TargetType>();
						break;
					case Types::PRE_TIMED_SIGNAL_CONTROL:
						//supply_allocation_based_on_driving_rule<TargetType>();
						supply_allocation_based_on_merging_model<TargetType>();
						break;
					case Types::ACTUATED_SIGNAL_CONTROL:
						//supply_allocation_based_on_driving_rule<TargetType>();
						supply_allocation_based_on_merging_model<TargetType>();
						break;
					case Types::ADAPTIVE_SIGNAL_CONTROL:
						//supply_allocation_based_on_driving_rule<TargetType>();
						supply_allocation_based_on_merging_model<TargetType>();
						break;
				}
			}

			// allocate supply based on driving rule
			template<typename TargetType> void supply_allocation_based_on_driving_rule()
			{
				typename _Outbound_Inbound_Movements_Container_Interface::iterator outbound_itr;

				for (outbound_itr=_outbound_inbound_movements.begin(); outbound_itr!=_outbound_inbound_movements.end(); outbound_itr++)
				{
					((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->template supply_allocation_based_on_driving_rule<NULLTYPE>();
				}
			}

			// allocate supply proportional to demand
			template<typename TargetType> void supply_allocation_proportion_to_demand()
			{
				//typedef  Intersection_Components::Prototypes::Outbound_Inbound_Movements<typename remove_pointer<typename  type_of(outbound_inbound_movements)::value_type>::type>  _Outbound_Inbound_Movements_Interface;
				//typedef  Random_Access_Sequence< type_of(outbound_inbound_movements), _Outbound_Inbound_Movements_Interface*> _Outbound_Inbound_Movements_Container_Interface;

				typename _Outbound_Inbound_Movements_Container_Interface::iterator outbound_itr;
				for (outbound_itr=_outbound_inbound_movements.begin(); outbound_itr!=_outbound_inbound_movements.end(); outbound_itr++)
				{
					((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->template supply_allocation_proportion_to_demand<NULLTYPE>();
				}
			}

			// allocate supply proportional to number of links
			template<typename TargetType> void supply_allocation_proportion_to_link()
			{
				typename _Outbound_Inbound_Movements_Container_Interface::iterator outbound_itr;
				for (outbound_itr=_outbound_inbound_movements.begin(); outbound_itr!=_outbound_inbound_movements.end(); outbound_itr++)
				{
					((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->template supply_allocation_proportion_to_link<NULLTYPE>();
				}
			}

			// allocate supply proportional to number of lanes
			template<typename TargetType> void supply_allocation_proportion_to_lane()
			{
				//typedef  Intersection_Components::Prototypes::Outbound_Inbound_Movements<typename remove_pointer<typename  type_of(outbound_inbound_movements)::value_type>::type>  _Outbound_Inbound_Movements_Interface;
				//typedef  Random_Access_Sequence< type_of(outbound_inbound_movements), _Outbound_Inbound_Movements_Interface*> _Outbound_Inbound_Movements_Container_Interface;

				typename _Outbound_Inbound_Movements_Container_Interface::iterator outbound_itr;
				for (outbound_itr=_outbound_inbound_movements.begin(); outbound_itr!=_outbound_inbound_movements.end(); outbound_itr++)
				{
					((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->template supply_allocation_proportion_to_lane<NULLTYPE>();
				}
			}

			template<typename TargetType> void supply_allocation_based_on_merging_model()
			{
				switch (((_Scenario_Interface*)_global_scenario)->template merging_mode<int>())
				{
				case Scenario_Components::Types::Merging_Mode_Keys::DRIVING_RULE:
					supply_allocation_based_on_driving_rule<TargetType>();
					break;
				case Scenario_Components::Types::Merging_Mode_Keys::PROPORTION_TO_DEMAND:
					supply_allocation_proportion_to_demand<TargetType>();
					break;
				case Scenario_Components::Types::Merging_Mode_Keys::PROPORTION_TO_LINK:
					supply_allocation_proportion_to_link<TargetType>();
					break;
				case Scenario_Components::Types::Merging_Mode_Keys::PROPORTION_TO_LANE:
					supply_allocation_proportion_to_lane<TargetType>();
					break;
				default:
					supply_allocation_based_on_driving_rule<TargetType>();
					break;
				}
			};

			template<typename TargetType> void link_capacity_allocation()
			{
				//typedef  Intersection_Components::Prototypes::Outbound_Inbound_Movements<typename remove_pointer<typename  type_of(outbound_inbound_movements)::value_type>::type>  _Outbound_Inbound_Movements_Interface;
				//typedef  Random_Access_Sequence< type_of(outbound_inbound_movements), _Outbound_Inbound_Movements_Interface*> _Outbound_Inbound_Movements_Container_Interface;

				typename _Inbound_Outbound_Movements_Container_Interface::iterator inbound_itr;
				for (inbound_itr=_inbound_outbound_movements.begin(); inbound_itr!=_inbound_outbound_movements.end(); inbound_itr++)
				{
					((_Inbound_Outbound_Movements_Interface*)(*inbound_itr))->template link_capacity_allocation<NULLTYPE>();
				}
			}

			// load vehicles to their origin link
			template<typename TargetType> void origin_link_loading()
			{
				_Link_Interface* outbound_link;
				typename _Outbound_Inbound_Movements_Container_Interface::iterator outbound_itr;

				for (outbound_itr=_outbound_inbound_movements.begin(); outbound_itr!=_outbound_inbound_movements.end(); outbound_itr++)
				{
					outbound_link=((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->template outbound_link_reference<_Link_Interface*>();
					outbound_link->template origin_link_loading<RNG_Components::RngStream&>();
				}
			}

			template<typename TargetType> void initialize_features(void* network)
			{
				_network_reference = (network_reference_type)network;
				//unsigned long seed = ((_Scenario_Interface*)_global_scenario)->template iseed<unsigned int>()+_internal_id+1;
				unsigned long seed = abs(std::sin(((_Scenario_Interface*)_global_scenario)->template iseed<unsigned int>() + (float)_internal_id + 1)*(float)INT_MAX);
				//unsigned long seed = 1;
				//_rng_stream.SetSeed(seed);

				typename _Outbound_Inbound_Movements_Container_Interface::iterator outbound_inbound_movements_itr;
				for (outbound_inbound_movements_itr=_outbound_inbound_movements.begin();outbound_inbound_movements_itr!=_outbound_inbound_movements.end();outbound_inbound_movements_itr++)
				{
					_Outbound_Inbound_Movements_Interface* outbound_inbound_movements = (_Outbound_Inbound_Movements_Interface*)(*outbound_inbound_movements_itr);

					_Inbound_Movements_Container_Interface& inbound_movements_container = outbound_inbound_movements->template inbound_movements<_Inbound_Movements_Container_Interface&>();
					typename _Inbound_Movements_Container_Interface::iterator inbound_movement_itr;
					for (inbound_movement_itr=inbound_movements_container.begin();inbound_movement_itr!=inbound_movements_container.end();inbound_movement_itr++)
					{
						((_Inbound_Movement_Interface*)(*inbound_movement_itr))->template initialize_features<NULLTYPE>();
					}
				}
			}

			template<typename TargetType> void Initialize()
			{

				if (_outbound_inbound_movements.size() > 0 && _inbound_outbound_movements.size() > 0)
				{
					((_Intersection_Control_Interface*)_intersection_control)->template Initialize<NULLTYPE>();
					int start_iteration = ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>()-1;
					this->template Load_Event<ComponentType>(&ComponentType::Newells_Conditional,start_iteration,Scenario_Components::Types::Type_Sub_Iteration_keys::INTERSECTION_COMPUTE_STEP_FLOW_SUB_ITERATION);
					////TODO
					//load_event(ComponentType,,ComponentType::template Compute_Step_Flow,start_iteration,Scenario_Components::Types::Type_Sub_Iteration_keys::INTERSECTION_COMPUTE_STEP_FLOW_SUB_ITERATION,NULLTYPE);
				}
			}
			
			template<typename TargetType> void calculate_moe_for_simulation_interval()
			{

				typename _Outbound_Inbound_Movements_Container_Interface::iterator outbound_inbound_movements_itr;
				for (outbound_inbound_movements_itr=_outbound_inbound_movements.begin();outbound_inbound_movements_itr!=_outbound_inbound_movements.end();outbound_inbound_movements_itr++)
				{
					_Outbound_Inbound_Movements_Interface* outbound_inbound_movements = (_Outbound_Inbound_Movements_Interface*)(*outbound_inbound_movements_itr);
					
					_Link_Interface* outbound_link=outbound_inbound_movements->template outbound_link_reference<_Link_Interface*>();
					_link_component_type* outbound_link_component = outbound_inbound_movements->template outbound_link_reference<_link_component_type*>();
					outbound_link_component->link_moe_data.link_queue_length += outbound_link_component->link_num_vehicles_in_queue<int>();
					outbound_link_component->realtime_link_moe_data.link_in_volume = 0.0f;
					_Inbound_Movements_Container_Interface& inbound_movements_container = outbound_inbound_movements->template inbound_movements<_Inbound_Movements_Container_Interface&>();
					typename _Inbound_Movements_Container_Interface::iterator inbound_movement_itr;
					for (inbound_movement_itr=inbound_movements_container.begin();inbound_movement_itr!=inbound_movements_container.end();inbound_movement_itr++)
					{
						((_Inbound_Movement_Interface*)(*inbound_movement_itr))->template calculate_moe_for_simulation_interval_from_outbound_link<NULLTYPE>();
					}

				}

				typename _Inbound_Outbound_Movements_Container_Interface::iterator inbound_outbound_movements_itr;
				for (inbound_outbound_movements_itr=_inbound_outbound_movements.begin();inbound_outbound_movements_itr!=_inbound_outbound_movements.end();inbound_outbound_movements_itr++)
				{
					_Inbound_Outbound_Movements_Interface* inbound_outbound_movements = (_Inbound_Outbound_Movements_Interface*)(*inbound_outbound_movements_itr);
					_Link_Interface* inbound_link=inbound_outbound_movements->template inbound_link_reference<_Link_Interface*>();
					_link_component_type* inbound_link_component = inbound_outbound_movements->template inbound_link_reference<_link_component_type*>();

					float avg_turn_penalty = 0.0f;
					int allowed_movement_size = 0;
					int num_vehicles_in_link = 0;

					inbound_link_component->realtime_link_moe_data.link_out_volume = 0.0f;

					_Outbound_Movements_Container_Interface& outbound_movements_container = inbound_outbound_movements->template outbound_movements<_Outbound_Movements_Container_Interface&>();
					typename _Outbound_Movements_Container_Interface::iterator outbound_movement_itr;
					for (outbound_movement_itr=outbound_movements_container.begin();outbound_movement_itr!=outbound_movements_container.end();outbound_movement_itr++)
					{
						((_Outbound_Movement_Interface*)(*outbound_movement_itr))->template calculate_moe_for_simulation_interval_from_inbound_link<NULLTYPE>();
						_movement_component_type* outbound_movement_component = (_movement_component_type*)(*outbound_movement_itr);
						if (outbound_movement_component->turn_travel_penalty<float>() < 0.01f*INFINITY_FLOAT)
						{
							avg_turn_penalty += ((_Outbound_Movement_Interface*)(*outbound_movement_itr))->template outbound_link_arrived_time_based_experienced_link_turn_travel_delay<float>();
							allowed_movement_size++;
						}
						num_vehicles_in_link += int(((_Outbound_Movement_Interface*)(*outbound_movement_itr))->template vehicles_container<_Vehicles_Container_Interface&>().size());
					}

					inbound_link_component->link_moe_data.link_density += ((float)num_vehicles_in_link / (inbound_link_component->length<float>() / 5280.0f)) / inbound_link_component->num_lanes<float>();
					inbound_link_component->realtime_link_moe_data.num_vehicles_in_link = num_vehicles_in_link;
					if (allowed_movement_size>0)
					{
						avg_turn_penalty /= float(allowed_movement_size);
					}
					else
					{
						avg_turn_penalty = 0.0f;
					}
					
					//realtime moe
					//link travel delay in minutes
					inbound_link_component->realtime_link_moe_data.link_travel_delay = avg_turn_penalty / 60.0f;
					//link travel time
					inbound_link_component->realtime_link_moe_data.link_travel_time = (inbound_link_component->link_fftt<float>() + avg_turn_penalty) /60.0f;
					inbound_link->template calculate_moe_for_simulation_interval<NULLTYPE>();
				}
			}

			template<typename TargetType> void calculate_moe_for_assignment_interval()
			{

				typename _Outbound_Inbound_Movements_Container_Interface::iterator outbound_inbound_movements_itr;
				for (outbound_inbound_movements_itr=_outbound_inbound_movements.begin();outbound_inbound_movements_itr!=_outbound_inbound_movements.end();outbound_inbound_movements_itr++)
				{
					_Outbound_Inbound_Movements_Interface* outbound_inbound_movements = (_Outbound_Inbound_Movements_Interface*)(*outbound_inbound_movements_itr);
					_Inbound_Movements_Container_Interface& inbound_movements_container = outbound_inbound_movements->template inbound_movements<_Inbound_Movements_Container_Interface&>();
					typename _Inbound_Movements_Container_Interface::iterator inbound_movement_itr;
					for (inbound_movement_itr=inbound_movements_container.begin();inbound_movement_itr!=inbound_movements_container.end();inbound_movement_itr++)
					{
						((_Inbound_Movement_Interface*)(*inbound_movement_itr))->template calculate_moe_for_assignment_interval_from_outbound_link<NULLTYPE>();
					}
				}

				typename _Inbound_Outbound_Movements_Container_Interface::iterator inbound_outbound_movements_itr;
				for (inbound_outbound_movements_itr=_inbound_outbound_movements.begin();inbound_outbound_movements_itr!=_inbound_outbound_movements.end();inbound_outbound_movements_itr++)
				{
					_Inbound_Outbound_Movements_Interface* inbound_outbound_movements = (_Inbound_Outbound_Movements_Interface*)(*inbound_outbound_movements_itr);
					_Link_Interface* inbound_link=inbound_outbound_movements->template inbound_link_reference<_Link_Interface*>();
					_link_component_type* inbound_link_component =((_inbound_outbound_movements_component_type*)(*inbound_outbound_movements_itr))->inbound_link_reference<_link_component_type*>();
				
					float avg_turn_penalty = 0.0f;
					float avg_turn_penalty_standard_deviation = 0.0f;
					int allowed_movement_size = 0;
					int num_vehicles_in_link = 0;

					_Outbound_Movements_Container_Interface& outbound_movements_container = inbound_outbound_movements->template outbound_movements<_Outbound_Movements_Container_Interface&>();
					typename _Outbound_Movements_Container_Interface::iterator outbound_movement_itr;
					for (outbound_movement_itr=outbound_movements_container.begin();outbound_movement_itr!=outbound_movements_container.end();outbound_movement_itr++)
					{
						_movement_component_type* outbound_movement_component = (_movement_component_type*)(*outbound_movement_itr);
						((_Outbound_Movement_Interface*)(*outbound_movement_itr))->template calculate_moe_for_assignment_interval_from_inbound_link<NULLTYPE>();
						if (outbound_movement_component->turn_travel_penalty<float>() < 0.01f*INFINITY_FLOAT)
						{
							allowed_movement_size++;
							avg_turn_penalty += outbound_movement_component->movement_moe_data.turn_penalty;
							avg_turn_penalty_standard_deviation += outbound_movement_component->movement_moe_data.turn_penalty_standard_deviation;
						}
					}

					if (allowed_movement_size > 0)
					{
						avg_turn_penalty /= allowed_movement_size;
						avg_turn_penalty_standard_deviation /= allowed_movement_size;
					}
					else
					{
						avg_turn_penalty = 0;
						avg_turn_penalty_standard_deviation = 0;
					}
					inbound_link_component->link_moe_data.link_travel_delay = avg_turn_penalty;
					inbound_link_component->link_moe_data.link_travel_delay_standard_deviation = avg_turn_penalty_standard_deviation;
					inbound_link->template calculate_moe_for_assignment_interval<NULLTYPE>();
				}
			}
			
			template<typename TargetType> void update_vehicle_locations()
			{
				typename _Inbound_Outbound_Movements_Container_Interface::iterator inbound_outbound_movements_itr;
				for (inbound_outbound_movements_itr=_inbound_outbound_movements.begin();inbound_outbound_movements_itr!=_inbound_outbound_movements.end();inbound_outbound_movements_itr++)
				{
					_Inbound_Outbound_Movements_Interface* inbound_outbound_movements = (_Inbound_Outbound_Movements_Interface*)(*inbound_outbound_movements_itr);
					_link_component_type* inbound_link_component = (_link_component_type*)((_inbound_outbound_movements_component_type*)(*inbound_outbound_movements_itr))->_inbound_link_reference;
					float link_speed = inbound_link_component->realtime_link_moe_data.link_speed;

					float travel_distance = (link_speed * 5280.0f / 3600.0f) * ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<float>();
					_Outbound_Movements_Container_Interface& outbound_movements_container = inbound_outbound_movements->template outbound_movements<_Outbound_Movements_Container_Interface&>();
					typename _Outbound_Movements_Container_Interface::iterator outbound_movement_itr;
					for (outbound_movement_itr=outbound_movements_container.begin();outbound_movement_itr!=outbound_movements_container.end();outbound_movement_itr++)
					{
						//num_vehicles_in_link += int(((_Outbound_Movement_Interface*)(*outbound_movement_itr))->template vehicles_container<_Vehicles_Container_Interface&>().size());
						typename _Vehicles_Container_Interface::iterator vehicle_itr;
						_Vehicles_Container_Interface& vehicles = ((_Outbound_Movement_Interface*)(*outbound_movement_itr))->template vehicles_container<_Vehicles_Container_Interface&>();

						for (vehicle_itr=vehicles.begin();vehicle_itr!=vehicles.end();vehicle_itr++)
						{
							float current_distance = ((_Vehicle_Interface*)(*vehicle_itr))->template distance_to_stop_bar<float>();
							float new_distance = max(0.0f,(current_distance - travel_distance));
							((_Vehicle_Interface*)(*vehicle_itr))->template distance_to_stop_bar<float>(new_distance); 
							((_Vehicle_Interface*)(*vehicle_itr))->template local_speed<float>(link_speed);
						}
					}
				}
			}

			template<typename TargetType> void update_in_network_vehicle_vht()
			{
				typename _Inbound_Outbound_Movements_Container_Interface::iterator inbound_outbound_movements_itr;
				for (inbound_outbound_movements_itr=_inbound_outbound_movements.begin();inbound_outbound_movements_itr!=_inbound_outbound_movements.end();inbound_outbound_movements_itr++)
				{
					_Inbound_Outbound_Movements_Interface* inbound_outbound_movements = (_Inbound_Outbound_Movements_Interface*)(*inbound_outbound_movements_itr);
					_link_component_type* inbound_link_component = ((_inbound_outbound_movements_component_type*)(*inbound_outbound_movements_itr))->_inbound_link_reference;
					float link_speed = inbound_link_component->realtime_link_moe_data.link_speed;

					float travel_distance = (link_speed * 5280.0f / 3600.0f) * ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<float>();
					_Outbound_Movements_Container_Interface& outbound_movements_container = inbound_outbound_movements->template outbound_movements<_Outbound_Movements_Container_Interface&>();
					typename _Outbound_Movements_Container_Interface::iterator outbound_movement_itr;
					for (outbound_movement_itr=outbound_movements_container.begin();outbound_movement_itr!=outbound_movements_container.end();outbound_movement_itr++)
					{
						//num_vehicles_in_link += int(((_Outbound_Movement_Interface*)(*outbound_movement_itr))->template vehicles_container<_Vehicles_Container_Interface&>().size());
						typename _Vehicles_Container_Interface::iterator vehicle_itr;
						_Vehicles_Container_Interface& vehicles = ((_Outbound_Movement_Interface*)(*outbound_movement_itr))->template vehicles_container<_Vehicles_Container_Interface&>();

						for (vehicle_itr=vehicles.begin();vehicle_itr!=vehicles.end();vehicle_itr++)
						{
							((_Vehicle_Interface*)(*vehicle_itr))->template update_vht<NT>();
						}
					}
				}
			}

			static void Newells_Conditional(ComponentType* _this,Event_Response& response)
			{
				typename MasterType::intersection_type* _pthis = (typename MasterType::intersection_type*)_this;
				_Intersection_Interface* _this_ptr=(_Intersection_Interface*)_this;
				
				if(sub_iteration() == Scenario_Components::Types::Type_Sub_Iteration_keys::INTERSECTION_COMPUTE_STEP_FLOW_SUB_ITERATION)
				{
					//((typename MasterType::intersection_type*)_this)->Swap_Event((Event)&Compute_Step_Flow<NULLTYPE>);
					_pthis->Compute_Step_Flow();
					//response.result=true;
					response.next._iteration=iteration();
					response.next._sub_iteration=Scenario_Components::Types::Type_Sub_Iteration_keys::INTERSECTION_ORIGIN_LINK_LOADING_SUB_ITERATION;
				}
				else if(sub_iteration() == Scenario_Components::Types::Type_Sub_Iteration_keys::INTERSECTION_ORIGIN_LINK_LOADING_SUB_ITERATION)
				{
					//((typename MasterType::intersection_type*)_this)->Swap_Event((Event)&Origin_Loading_Step<NULLTYPE>);
					_pthis->Origin_Loading_Step();
					//response.result=true;
					response.next._iteration=iteration();
					response.next._sub_iteration=Scenario_Components::Types::Type_Sub_Iteration_keys::INTERSECTION_REALTIME_MOE_COMPUTATION_SUB_ITERATION;
				}
				//else if(sub_iteration() == Scenario_Components::Types::Type_Sub_Iteration_keys::INTERSECTION_NETWORK_STATE_UPDATE_SUB_ITERATION)
				//{
				//	//TODO:BIG_CHANGE!
				//	//((typename MasterType::intersection_type*)_this)->Swap_Event((Event)&Network_State_Update<NULLTYPE>);
				//	_pthis->Network_State_Update();
				//	//response.result=true;
				//	response.next._iteration=iteration();
				//	response.next._sub_iteration=Scenario_Components::Types::Type_Sub_Iteration_keys::INTERSECTION_REALTIME_MOE_COMPUTATION_SUB_ITERATION;
				//} 
				else if(sub_iteration() == Scenario_Components::Types::Type_Sub_Iteration_keys::INTERSECTION_REALTIME_MOE_COMPUTATION_SUB_ITERATION)
				{
					//((typename MasterType::intersection_type*)_this)->Swap_Event((Event)&ComponentType::template Intersection_REALTIME_MOE_Update<NULLTYPE>);
					_pthis->Intersection_REALTIME_MOE_Update();
					//response.result=true;
					response.next._iteration=iteration();
					response.next._sub_iteration=Scenario_Components::Types::Type_Sub_Iteration_keys::INTERSECTION_MOE_COMPUTATION_SUB_ITERATION;
				}
				else if(sub_iteration() == Scenario_Components::Types::Type_Sub_Iteration_keys::INTERSECTION_MOE_COMPUTATION_SUB_ITERATION)
				{

					//((typename MasterType::intersection_type*)_this)->Swap_Event((Event)&Intersection_MOE_Update<NULLTYPE>);
					_pthis->Intersection_MOE_Update();
					//response.result=true;
					response.next._iteration=iteration() + ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>();
					response.next._sub_iteration=Scenario_Components::Types::Type_Sub_Iteration_keys::INTERSECTION_COMPUTE_STEP_FLOW_SUB_ITERATION;
				}
				else
				{
					assert(false);
					cout << "Should never reach here in intersection conditional!" << endl;
				}
			}

			//declare_event(Compute_Step_Flow)
			void Compute_Step_Flow()
			{
				_Intersection_Interface* _this_ptr=(_Intersection_Interface*)this;
				//step 2: turn vehicles updating based on node control and link management, inbound link demand, and outbound link supply
				_this_ptr->template turn_movement_capacity_update<NULLTYPE>(); 
				//step 3: allocate link supply to inbound turn movements according to a given merging policy
				
				_this_ptr->template turn_movement_demand_calculation<NULLTYPE>();
			}

			//declare_event(Origin_Loading_Step)
			void Origin_Loading_Step()
			{
				_Intersection_Interface* _this_ptr=(_Intersection_Interface*)this;

				_this_ptr->template turn_movement_supply_allocation<NULLTYPE>();
				
				_this_ptr->template link_capacity_allocation<NULLTYPE>();

				//step 4: determine turn movement flow rate based on demand, capacity, and supply
				_this_ptr->template turn_movement_flow_calculation<NULLTYPE>();
				//step 5: node transfer
				_this_ptr->template node_transfer<NULLTYPE>();

				//step 6: origin link loading
				//_this_ptr->template origin_link_loading<NULLTYPE>();
			}

			//declare_event(Network_State_Update)
			void Network_State_Update()
			{
				//typedef Intersection<typename MasterType::intersection_type> _Intersection_Interface;
				_Intersection_Interface* _this_ptr=(_Intersection_Interface*)this;

				//step 9: intersection network state update
				_this_ptr->template network_state_update<NULLTYPE>();
			}

			//declare_event(Intersection_REALTIME_MOE_Update)
			void Intersection_REALTIME_MOE_Update()
			{
				_Intersection_Interface* _this_ptr=(_Intersection_Interface*)this;
				//step 10: intersection realtime MOE update

				if (((_Scenario_Interface*)_global_scenario)->template calculate_realtime_moe<bool>())
				{
					_this_ptr->template calculate_moe_for_simulation_interval<NULLTYPE>();
				}
			}

			//declare_event(Intersection_MOE_Update)
			void Intersection_MOE_Update()
			{
				_Intersection_Interface* _this_ptr=(_Intersection_Interface*)this;
				//step 11: intersection MOE update
				if(((((_Network_Interface*)_global_network)->template current_simulation_interval_index<int>()+1)*((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>())%((_Scenario_Interface*)_global_scenario)->template assignment_interval_length<int>() == 0)
				{
					_this_ptr->template calculate_moe_for_assignment_interval<NULLTYPE>();
				}
			}
		};
	}
}

