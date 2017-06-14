#pragma once

//#include "../io/Io.h"

#include "Traffic_Simulator_Includes.h"
#include "Network_Prototype.h"
//#include "../File_IO/network_operation_data.h"
#include "Intersection_Control_Prototype.h"
#include "Traffic_Simulator_Forward_Declaration.h"
#include "Intersection_Prototype.h"
extern void* _global_scenario;
extern void* _global_network;


//IO part of reading the intersection controls is implemented in this namespace
namespace Operation_Components
{
	using std::shared_ptr;
	namespace Types
	{
		struct Hard_Coded_Operation{};
		struct ODB_Operation{};
		struct File_Operation{};
	}

	namespace Concepts
	{
	}
	
	namespace Prototypes
	{
		prototype struct Operation ADD_DEBUG_INFO
		{
			tag_as_prototype;

			accessor(network_reference, NONE, NONE);

			template<typename TargetType> void read_operation_data(typename TargetType::ParamType& network_mapping,requires(TargetType,check_2(typename TargetType::NetIOType,Network_Components::Types::ODB_Network,is_same)))
			{
				read_intersection_control_data(network_mapping);
				read_ramp_metering_data(network_mapping);
			}

			template<typename TargetType> void read_ramp_metering_data(typename TargetType::ParamType& network_mapping,requires(TargetType,check_2(typename TargetType::NetIOType,Network_Components::Types::ODB_Network,is_same)))
			{
				typedef  Network_Components::Prototypes::Network< typename get_type_of(network_reference)> _Network_Interface;
				typedef  Ramp_Metering_Components::Prototypes::Ramp_Metering<typename remove_pointer< typename _Network_Interface::get_type_of(ramp_metering_container)::value_type>::type>  _Ramp_Metering_Interface;
				typedef  Random_Access_Sequence< typename _Network_Interface::get_type_of(ramp_metering_container), _Ramp_Metering_Interface*> _Ramp_Metering_Container_Interface;

				typedef  Link_Components::Prototypes::Link<typename remove_pointer< typename _Network_Interface::get_type_of(links_container)::value_type>::type>  _Link_Interface;
				typedef  Random_Access_Sequence< typename _Network_Interface::get_type_of(links_container), _Link_Interface*> _Links_Container_Interface;

				typedef  Turn_Movement_Components::Prototypes::Movement<typename remove_pointer< typename _Link_Interface::get_type_of(outbound_turn_movements)::value_type>::type>  _Turn_Movement_Interface;
				typedef  Random_Access_Sequence< typename _Link_Interface::get_type_of(outbound_turn_movements), _Turn_Movement_Interface*> _Turn_Movements_Container_Interface;


				_Links_Container_Interface& links = ((_Network_Interface*)_global_network)->template links_container<_Links_Container_Interface&>();
				_Link_Interface* link;
				int ramp_metering_counter = 0;
				typename _Links_Container_Interface::iterator link_itr;
				for (link_itr = links.begin(); link_itr != links.end(); link_itr++)
				{
					link = (_Link_Interface*)(*link_itr);
					if (link->template link_type<int>() == Link_Components::Types::ON_RAMP)
					{
						_Turn_Movements_Container_Interface& outbound_movements = link->template outbound_turn_movements<_Turn_Movements_Container_Interface&>();
						if (outbound_movements.size() == 1)
						{
							_Link_Interface* outbound_link = ((_Turn_Movement_Interface*)outbound_movements[0])->template outbound_link<_Link_Interface*>();
							if (outbound_link->template link_type<int>() == Link_Components::Types::FREEWAY || outbound_link->template link_type<int>() == Link_Components::Types::EXPRESSWAY)
							{
								_Turn_Movements_Container_Interface& inbound_movements = link->template inbound_turn_movements<_Turn_Movements_Container_Interface&>();
								if (inbound_movements.size() >= 1)
								{
									_Link_Interface* inbound_link = inbound_movements[0]->template inbound_link<_Link_Interface*>();
									if (inbound_link->template link_type<int>() == Link_Components::Types::ARTERIAL || inbound_link->template link_type<int>() == Link_Components::Types::LOCAL)
									{
										_Ramp_Metering_Interface* ramp_metering = (_Ramp_Metering_Interface*)Allocate<typename _Ramp_Metering_Interface::Component_Type>();

										///

										ramp_metering->template internal_id<int>(ramp_metering_counter++);
				
										ramp_metering->template on_ramp_link<_Link_Interface*>(link);
										ramp_metering->template downstream_freeway_link<_Link_Interface*>(outbound_link);

										float link_length = outbound_link->template length<float>();
										ramp_metering->template downstream_freeway_detector_length<float>(link_length);
										ramp_metering->template position_first_detector_on_freeway<float>(0.0f);
										ramp_metering->template position_second_detector_on_freeway<float>(link_length);

										ramp_metering->template starting_time<int>(0.0);
										ramp_metering->template ending_time<int>(24*60*60);
										ramp_metering->template metering_updating_interval_length<int>(6*10);

										float alpha = 70.0f / float(link->template num_lanes<int>());
										ramp_metering->template alpha<float>(alpha);
										ramp_metering->template beta<float>(0.2f);
										ramp_metering->template downstream_freeway_link_occupancy<float>(0.0f);
										((_Network_Interface*)_global_network)->template ramp_metering_container<_Ramp_Metering_Container_Interface&>().push_back(ramp_metering);
										link->template ramp_meter<_Ramp_Metering_Interface*>(ramp_metering);
										if (ramp_metering_counter % 10 == 0)
											cout << "number of ramp meters = " << ramp_metering_counter << endl;
									}
								}
							}
						}
					}
				}
			}

			template<typename TargetType> void read_intersection_control_data(typename TargetType::ParamType& network_mapping,requires(TargetType,check_2(typename TargetType::NetIOType,Network_Components::Types::ODB_Network,is_same)))
			{
				Network_Components::Types::Network_IO_Maps& net_io_maps=(Network_Components::Types::Network_IO_Maps&)network_mapping;

				typedef  Network_Components::Prototypes::Network< typename get_type_of(network_reference)> _Network_Interface;
				typedef  Scenario_Components::Prototypes::Scenario< typename _Network_Interface::get_type_of(scenario_reference)> _Scenario_Interface;

				typedef  Intersection_Components::Prototypes::Intersection<typename remove_pointer< typename _Network_Interface::get_type_of(intersections_container)::value_type>::type>  _Intersection_Interface;
				typedef  Random_Access_Sequence< typename _Network_Interface::get_type_of(intersections_container), _Intersection_Interface*> _Intersections_Container_Interface;

				typedef  Link_Components::Prototypes::Link<typename remove_pointer< typename _Network_Interface::get_type_of(links_container)::value_type>::type>  _Link_Interface;
				typedef  Random_Access_Sequence< typename _Network_Interface::get_type_of(links_container), _Link_Interface*> _Links_Container_Interface;

				typedef Intersection_Components::Prototypes::Inbound_Outbound_Movements<typename remove_pointer<typename _Intersection_Interface::get_type_of(inbound_outbound_movements)::value_type>::type> _Inbound_Outbound_Interface;
				typedef Random_Access_Sequence<typename _Intersection_Interface::get_type_of(inbound_outbound_movements),_Inbound_Outbound_Interface*> _Inbound_Outbound_Container_Interface;


				typedef  Intersection_Control_Components::Prototypes::Intersection_Control< typename _Intersection_Interface::get_type_of(intersection_control)> _Intersection_Control_Interface;
				typedef  Intersection_Control_Components::Prototypes::Control_Plan<typename remove_pointer< typename _Intersection_Control_Interface::get_type_of(control_plan_data_array)::value_type>::type>  _Control_Plan_Interface;
				typedef  Random_Access_Sequence< typename _Intersection_Control_Interface::get_type_of(control_plan_data_array), _Control_Plan_Interface*> _Control_Plans_Container_Interface;

				typedef  Intersection_Control_Components::Prototypes::Phase<typename remove_pointer< typename _Control_Plan_Interface::get_type_of(phase_data_array)::value_type>::type>  _Phase_Interface;
				typedef  Random_Access_Sequence< typename _Control_Plan_Interface::get_type_of(phase_data_array), _Phase_Interface*> _Phases_Container_Interface;

				typedef  Intersection_Control_Components::Prototypes::Phase_Movement<typename remove_pointer< typename _Phase_Interface::get_type_of(turn_movements_in_the_phase_array)::value_type>::type>  _Phase_Movement_Interface;
				typedef  Random_Access_Sequence< typename _Phase_Interface::get_type_of(turn_movements_in_the_phase_array), _Phase_Movement_Interface*> _Phase_Movements_Container_Interface;

				typedef  Turn_Movement_Components::Prototypes::Movement<typename remove_pointer< typename _Network_Interface::get_type_of(turn_movements_container)::value_type>::type>  _Movement_Interface;
				typedef  Random_Access_Sequence< typename _Network_Interface::get_type_of(turn_movements_container), _Movement_Interface*> _Movements_Container_Interface;

				typedef  Intersection_Control_Components::Prototypes::Approach<typename remove_pointer< typename _Control_Plan_Interface::get_type_of(approach_data_array)::value_type>::type>  _Approach_Interface;
				typedef  Random_Access_Sequence< typename _Control_Plan_Interface::get_type_of(approach_data_array), _Approach_Interface*> _Approaches_Container_Interface;


				
				using namespace odb;
				using namespace polaris::io;

				string name(((_Scenario_Interface*)_global_scenario)->template database_name<string&>());
				unique_ptr<database> db (open_sqlite_database (name));
				transaction t(db->begin());

				_Network_Interface* network = network_reference<_Network_Interface*>();

				_Intersections_Container_Interface& intersections_container = network->template intersections_container<_Intersections_Container_Interface&>();

				_Links_Container_Interface& links_container = network->template links_container<_Links_Container_Interface&>();

				union Link_ID_Dir
				{
					struct
					{
						int id;
						int dir;
					};

					long long id_dir;
				} link_id_dir;
				


				int counter=0;

				cout << "Reading Signals" << endl;

				result<Signal> signal_result=db->template query<Signal>(query<Signal>::true_expr);

				for(result<Signal>::iterator db_itr=signal_result.begin();db_itr!=signal_result.end();++db_itr)
				{
					//if(rand()%2!=0) continue;

					_Intersection_Interface* intersection = (_Intersection_Interface*)net_io_maps.intersection_id_to_ptr[db_itr->getNodes()->getNode()];

					if(++counter%1000==0) cout << "\t" << counter << endl;

					assert(net_io_maps.intersection_id_to_ptr.count(db_itr->getNodes()->getNode()));

					
					_Intersection_Control_Interface* intersection_control = (_Intersection_Control_Interface*)Allocate<typename _Intersection_Control_Interface::Component_Type>();

					intersection_control->template intersection<_Intersection_Interface*>(intersection);
					intersection->template intersection_control<_Intersection_Control_Interface*>(intersection_control);

					intersection->template intersection_type<Intersection_Control_Components::Types::Intersection_Type_Keys>(PRE_TIMED_SIGNAL_CONTROL);

					std::vector<signal_time>::iterator signal_itr;
					
					for(signal_itr=db_itr->nested_records.begin();signal_itr!=db_itr->nested_records.end();signal_itr++)
					{
						_Control_Plan_Interface* control_plan = (_Control_Plan_Interface*)Allocate<typename _Control_Plan_Interface::Component_Type>();
						
						control_plan->template starting_time<int>(signal_itr->start);
						//control_plan->template ending_time<int>(signal_itr->end);

						control_plan->template ending_time<int>(24*60*60);

						

						control_plan->template control_type<int>(Intersection_Control_Components::Types::Intersection_Type_Keys::PRE_TIMED_SIGNAL_CONTROL);
						control_plan->template cycle_starting_time<int>(0.0);
						control_plan->template cycle_ending_time<int>(0.0);
						control_plan->template cycle_leftover_time<int>(0.0);
						
						//marker for timing to interpret referentially
						control_plan->template cycle_index<int>(signal_itr->timing);

						//marker for phasing to interpret referentially
						control_plan->template control_plan_index<int>(signal_itr->phasing);

						intersection_control->template control_plan_data_array<_Control_Plans_Container_Interface&>().push_back(control_plan);

						
						_Inbound_Outbound_Container_Interface& inbound_outbound_container=intersection->template inbound_outbound_movements<_Inbound_Outbound_Container_Interface&>();

						typename _Inbound_Outbound_Container_Interface::iterator inbound_itr;

						for(inbound_itr=inbound_outbound_container.begin();inbound_itr!=inbound_outbound_container.end();inbound_itr++)
						{
							_Approach_Interface* approach = (_Approach_Interface*)Allocate<typename _Approach_Interface::Component_Type>();
							_Inbound_Outbound_Interface* inbound_outbound = (_Inbound_Outbound_Interface*)(*inbound_itr);
							_Link_Interface* link = inbound_outbound->template inbound_link_reference<_Link_Interface*>();
							approach->template inbound_link<_Link_Interface*>(link);
							approach->template green_cycle_ratio<int>(0.0);

							link->template approach<_Approach_Interface*>(approach);

							control_plan->template approach_data_array<_Approaches_Container_Interface&>().push_back(approach);

						}
					}
				}

				int skipped = 0;

				counter=0;

				cout << "Reading Timing" << endl;

				result<Timing> timing_result=db->template query<Timing>(query<Timing>::true_expr);
				
				// pass over each timing once
				// every iteration one set of timing phases will be committed to one or more control plans

				for(result<Timing>::iterator db_itr = timing_result.begin (); db_itr != timing_result.end (); ++db_itr)
				{
					shared_ptr<Signal> signal=timing_result.begin ()->getSignal();

					assert(net_io_maps.intersection_id_to_ptr.count(signal->getNodes()->getNode()));

					_Intersection_Interface* intersection = (_Intersection_Interface*)net_io_maps.intersection_id_to_ptr[signal->getNodes()->getNode()];


					if(++counter%10000==0) cout << "\t" << counter << endl;

					_Intersection_Control_Interface* intersection_control = intersection->template intersection_control<_Intersection_Control_Interface*>();
					
					if(intersection_control == nullptr){ continue;}

					_Control_Plans_Container_Interface& control_plans=intersection_control->template control_plan_data_array<_Control_Plans_Container_Interface&>();

					// find out which control plans request this particular timing

					for(typename _Control_Plans_Container_Interface::iterator control_plans_itr=control_plans.begin();control_plans_itr!=control_plans.end();control_plans_itr++)
					{
						_Control_Plan_Interface* control_plan=(_Control_Plan_Interface*)(*control_plans_itr);

						// match timing reference

						if(control_plan->template cycle_index<int>()==db_itr->getTiming())
						{
							if(control_plan->template phase_data_array<_Phases_Container_Interface&>().size()>0) break;

							// fill out phases for this timing

							for(std::vector<timing_phase>::iterator timing_itr=db_itr->nested_records.begin();timing_itr!=db_itr->nested_records.end();timing_itr++)
							{
								_Phase_Interface* phase = (_Phase_Interface*)Allocate<typename _Phase_Interface::Component_Type>();
								
								typename _Phase_Interface::Component_Type* phase_monitor=(typename _Phase_Interface::Component_Type*)phase;

								control_plan->template cycle_length<int>(db_itr->getCycle());
								control_plan->template offset<int>(db_itr->getOffset());
								
								// reset marker
								// control_plan->template cycle_index<int>(0.0);
								
								phase->template yellow_time<int>(timing_itr->yellow);
								phase->template all_red_time<int>(timing_itr->red);
								phase->template maximum_green_time<int>(timing_itr->maximum);
								phase->template minimum_green_time<int>(timing_itr->minimum);
								phase->template green_time<int>(timing_itr->minimum);

								//phase->template phase_index<int>(phase_data.phase_index);
								

								phase->template green_starting_time<int>(0.0);
								phase->template yellow_starting_time<int>(0.0);
								phase->template red_start_time<int>(0.0);
								phase->template adjusted_green_time<int>(0.0);
								phase->template adjusted_yellow_time<int>(0.0);
								phase->template adjusted_maximum_green_time<int>(0.0);
								phase->template adjusted_minimum_green_time<int>(0.0);
								//phase->template offset<int>(phase_data.offset);

								control_plan->template phase_data_array<_Phases_Container_Interface&>().push_back(phase);
							}
						}
					}
				}

				
				
				result<Phasing> phasing_result=db->template query<Phasing>(query<Phasing>::true_expr);
				
				counter=0;

				cout << "Reading Signal Phases" << endl;

				// pass over each set of phasing movements once
				// every iteration one set of phasing movements will be committed one or more times to control plans

				try
				{
				for(result<Phasing>::iterator db_itr = phasing_result.begin(); db_itr != phasing_result.end();++db_itr)
				{

					shared_ptr<Signal> signal=db_itr->getSignal();

					if(!net_io_maps.intersection_id_to_ptr.count(signal->getNodes()->getNode())){ ++skipped; continue;}

					_Intersection_Interface* intersection = (_Intersection_Interface*)net_io_maps.intersection_id_to_ptr[signal->getNodes()->getNode()];


					if(++counter%10000==0) cout << "\t" << counter << endl;
					_Intersection_Control_Interface* intersection_control = intersection->template intersection_control<_Intersection_Control_Interface*>();

					if(intersection_control == nullptr){ ++skipped; continue;}

					_Inbound_Outbound_Container_Interface& inbound_outbound_container=intersection->template inbound_outbound_movements<_Inbound_Outbound_Container_Interface&>();

					typename _Control_Plans_Container_Interface::iterator control_plan_itr;



					// find control plans which call for this particular phasing label

					for(control_plan_itr=intersection_control->template control_plan_data_array<_Control_Plans_Container_Interface&>().begin();
						control_plan_itr!=intersection_control->template control_plan_data_array<_Control_Plans_Container_Interface&>().end();
						control_plan_itr++)
					{
						_Control_Plan_Interface* control_plan = (_Control_Plan_Interface*)(*control_plan_itr);

						typename _Inbound_Outbound_Container_Interface::iterator inbound_itr;

						if(control_plan->template control_plan_index<int>()==db_itr->getPhasing())
						{
							// something is inaccurate, should fit into the phase data array
							if(db_itr->getPhase() > control_plan->template phase_data_array<_Phases_Container_Interface&>().size()) break;

							// will only be filling one phase (the one active in db_itr) for this control plan
							_Phase_Interface* phase=control_plan->template phase_data_array<_Phases_Container_Interface&>()[db_itr->getPhase()-1];
							
							// skip phasing plans which are already filled, first come first serve

							if(phase->template turn_movements_in_the_phase_array<_Phase_Movements_Container_Interface&>().size()!=0) break;

							phase->template phase_sequence_number<int>(db_itr->getPhase());
							

							// fill in the movements which correspond to this phase, outbound then inbound

							for(std::vector<phase_movement>::iterator phase_mvmt_itr=db_itr->nested_records.begin();phase_mvmt_itr!=db_itr->nested_records.end();phase_mvmt_itr++)
							{

								// must determine the in_link, out_link pair; first find the in_link to match

								link_id_dir.dir=phase_mvmt_itr->dir;
								link_id_dir.id=phase_mvmt_itr->link->getLink();
								
								if(!net_io_maps.link_id_dir_to_ptr.count(link_id_dir.id_dir)){ ++skipped; continue;}
								
								_Phase_Movement_Interface* phase_movement = (_Phase_Movement_Interface*)Allocate<typename _Phase_Movement_Interface::Component_Type>();
								
								_Link_Interface* in_link = (_Link_Interface*)net_io_maps.link_id_dir_to_ptr[link_id_dir.id_dir];								

								// check dir==0 for the out_link

								link_id_dir.id=phase_mvmt_itr->to_link->getLink();
								link_id_dir.dir=0;
								
								_Link_Interface* out_link=nullptr;

								if(net_io_maps.link_id_dir_to_ptr.count(link_id_dir.id_dir))
								{
									//cout << (int*)net_io_maps.link_id_dir_to_ptr[link_id_dir.id_dir] << endl;
									// an upstream intersection match confirms this is the correct out_link

									if(((_Link_Interface*)net_io_maps.link_id_dir_to_ptr[link_id_dir.id_dir])->template upstream_intersection<_Intersection_Interface*>()==intersection)
									{
										out_link=(_Link_Interface*)net_io_maps.link_id_dir_to_ptr[link_id_dir.id_dir];
									}
								}
								
								// check dir==1 for the out_link

								link_id_dir.dir=1;

								if(net_io_maps.link_id_dir_to_ptr.count(link_id_dir.id_dir))
								{
									// an upstream intersection match confirms this is the correct out_link

									if(((_Link_Interface*)net_io_maps.link_id_dir_to_ptr[link_id_dir.id_dir])->template upstream_intersection<_Intersection_Interface*>()==intersection)
									{
										out_link=(_Link_Interface*)net_io_maps.link_id_dir_to_ptr[link_id_dir.id_dir];
									}
								}

								if(out_link==nullptr)
								{
									THROW_EXCEPTION("ERROR: outbound link '"<<link_id_dir.id<<" for movement from phasing table was not found in network. Check link type, signal table, etc. for errors.");
								}					


								// with in_link and out_link in place, must now match movements, rummage through the inbound_outbound structure for this intersection

								for(inbound_itr=inbound_outbound_container.begin();inbound_itr!=inbound_outbound_container.end();inbound_itr++)
								{
									_Inbound_Outbound_Interface* inbound_outbound = (_Inbound_Outbound_Interface*)(*inbound_itr);

									// match the in_link of the inbound_outbound structure with the in_link we want

									_Link_Interface* in_link_check=inbound_outbound->template inbound_link_reference<_Link_Interface*>();

									if(in_link_check==in_link)
									{
										typename _Movements_Container_Interface::iterator outbound_itr;

										_Movements_Container_Interface& outbound_mvmts_container=inbound_outbound->template outbound_movements<_Movements_Container_Interface&>();



										// rummage through the outbound movements with this inbound movement for this intersection

										for(outbound_itr=outbound_mvmts_container.begin();outbound_itr!=outbound_mvmts_container.end();outbound_itr++)
										{
											_Movement_Interface* mvmt=(_Movement_Interface*)(*outbound_itr);

											// match the out_link of the outbound_movements structure with the out_link we want

											_Link_Interface* out_link_check=mvmt->template outbound_link<_Link_Interface*>();

											if(out_link_check==out_link)
											{
												// finally can add the movement reference and movement priority type

												mvmt->template merge_priority<int>(0.0);
												mvmt->template green_time<int>(0.0);
												mvmt->template inbound_link_green_cycle_ratio<float>(0.0);

												phase_movement->template movement<_Movement_Interface*>((_Movement_Interface*)(*outbound_itr));
												
												if(phase_mvmt_itr->protect=="PROTECTED")
												{
													phase_movement->template movement_priority_type<int>(Intersection_Control_Components::Types::Movement_Priority_Type_Keys::PROTECTED);
												}
												else if(phase_mvmt_itr->protect=="STOP_PERMIT")
												{
													phase_movement->template movement_priority_type<int>(Intersection_Control_Components::Types::Movement_Priority_Type_Keys::PERMITTED);
												}
												else if(phase_mvmt_itr->protect=="PERMITTED")
												{
													phase_movement->template movement_priority_type<int>(Intersection_Control_Components::Types::Movement_Priority_Type_Keys::PERMITTED);
												}
												else
												{
													assert(false);
												}

												phase->template turn_movements_in_the_phase_array<_Phase_Movements_Container_Interface&>().push_back(phase_movement);
											}
										}



									}
								}



							}



						}
					}



				}
				}
				catch (odb::sqlite::database_exception ex)
				{
					cout << ex.what();
				}


				counter=0;
				cout << skipped << endl;

				//cout << "Reading Signs" << endl;

				//result<Sign> sign_result=db->template query<Sign>(query<Sign>::true_expr);

				//for(result<Sign>::iterator db_itr=sign_result.begin();db_itr!=sign_result.end();++db_itr)
				//{
				//	if(++counter%1000==0) cout << "\t" << counter << endl;

				//	link_id_dir.id=db_itr->getLink()->getLink();
				//	link_id_dir.dir=db_itr->getDir();

				//	if(!net_io_maps.link_id_dir_to_ptr.count(link_id_dir.id_dir)){ continue;}


				//	_Intersection_Interface* intersection=((_Link_Interface*)net_io_maps.link_id_dir_to_ptr[link_id_dir.id_dir])->template downstream_intersection<_Intersection_Interface*>();

				//	_Intersection_Control_Interface* intersection_control;
				//	
				//	if(intersection->template intersection_control<_Intersection_Control_Interface*>()==nullptr)
				//	{
				//		intersection_control=(_Intersection_Control_Interface*)Allocate<typename _Intersection_Control_Interface::Component_Type>();

				//		intersection->template intersection_control<_Intersection_Control_Interface*>(intersection_control);

				//		_Control_Plan_Interface* control_plan = (_Control_Plan_Interface*)Allocate<typename _Control_Plan_Interface::Component_Type>();
				//		
				//		control_plan->template starting_time<int>(0.0);
				//		control_plan->template ending_time<int>(USHRT_MAX);
				//		
				//		control_plan->template control_type<int>(Intersection_Control_Components::Types::Intersection_Type_Keys::ALL_WAY_STOP_SIGN);
				//		control_plan->template cycle_starting_time<int>(0.0);
				//		control_plan->template cycle_ending_time<int>(0.0);
				//		control_plan->template cycle_leftover_time<int>(0.0);
				//		control_plan->template cycle_index<int>(0.0);
				//		control_plan->template control_plan_index<int>(0.0);

				//		intersection_control->template control_plan_data_array<_Control_Plans_Container_Interface&>().push_back(control_plan);

				//		
				//		_Inbound_Outbound_Container_Interface& inbound_outbound_container=intersection->template inbound_outbound_movements<_Inbound_Outbound_Container_Interface&>();

				//		typename _Inbound_Outbound_Container_Interface::iterator inbound_itr;

				//		for(inbound_itr=inbound_outbound_container.begin();inbound_itr!=inbound_outbound_container.end();inbound_itr++)
				//		{
				//			_Approach_Interface* approach = (_Approach_Interface*)Allocate<typename _Approach_Interface::Component_Type>();
				//			_Inbound_Outbound_Interface* inbound_outbound = (_Inbound_Outbound_Interface*)(*inbound_itr);

				//			_Link_Interface* link=inbound_outbound->template inbound_link_reference<_Link_Interface*>();

				//			approach->template inbound_link<_Link_Interface*>(link);
				//			approach->template green_cycle_ratio<int>(0.0);

				//			link->template approach<_Approach_Interface*>(approach);

				//			control_plan->template approach_data_array<_Approaches_Container_Interface&>().push_back(approach);
				//		}
				//	}

				//}
				
				counter=0;

				cout << "Filling All Unreferenced Intersections With 4 Way Stops" << endl;

				_Intersections_Container_Interface& intersections=network->template intersections_container<_Intersections_Container_Interface&>();

				for(typename _Intersections_Container_Interface::iterator itr=intersections.begin();itr!=intersections.end();itr++)
				{
					_Intersection_Interface* intersection=(_Intersection_Interface*)(*itr);
					
					if(intersection->template intersection_control<_Intersection_Control_Interface*>()==nullptr)
					{
						intersection->template intersection_type<int>(Intersection_Components::Types::NO_CONTROL);

						_Intersection_Control_Interface* intersection_control = (_Intersection_Control_Interface*)Allocate<typename _Intersection_Control_Interface::Component_Type>();
						intersection_control->template intersection<_Intersection_Interface*>(intersection);
					
						_Control_Plan_Interface* control_plan = (_Control_Plan_Interface*)Allocate<typename _Control_Plan_Interface::Component_Type>();
						control_plan->template control_plan_index<int>(0.0);
						control_plan->template control_type<int>(Intersection_Components::Types::NO_CONTROL);
						control_plan->template starting_time<int>(0.0);
						control_plan->template ending_time<int>(24*60*60);
						intersection_control->template control_plan_data_array<_Control_Plans_Container_Interface&>().push_back(control_plan);
						intersection->template intersection_control<_Intersection_Control_Interface*>(intersection_control);

						







						if(++counter%1000==0) cout << "\t" << counter << endl;

						//_Intersection_Control_Interface* intersection_control=(_Intersection_Control_Interface*)Allocate<typename _Intersection_Control_Interface::Component_Type>();

						//intersection->template intersection_control<_Intersection_Control_Interface*>(intersection_control);

						//_Control_Plan_Interface* control_plan = (_Control_Plan_Interface*)Allocate<typename _Control_Plan_Interface::Component_Type>();
						//
						//control_plan->template starting_time<int>(0.0);
						//control_plan->template ending_time<int>(USHRT_MAX);
						//
						//control_plan->template control_type<int>(Intersection_Control_Components::Types::Intersection_Type_Keys::ALL_WAY_STOP_SIGN);
						//control_plan->template cycle_starting_time<int>(0.0);
						//control_plan->template cycle_ending_time<int>(0.0);
						//control_plan->template cycle_leftover_time<int>(0.0);
						//control_plan->template cycle_index<int>(0.0);
						//control_plan->template control_plan_index<int>(0.0);

						//intersection_control->template control_plan_data_array<_Control_Plans_Container_Interface&>().push_back(control_plan);

						
						//_Inbound_Outbound_Container_Interface& inbound_outbound_container=intersection->template inbound_outbound_movements<_Inbound_Outbound_Container_Interface&>();

						//typename _Inbound_Outbound_Container_Interface::iterator inbound_itr;

						//for(inbound_itr=inbound_outbound_container.begin();inbound_itr!=inbound_outbound_container.end();inbound_itr++)
						//{
						//	_Approach_Interface* approach = (_Approach_Interface*)Allocate<typename _Approach_Interface::Component_Type>();
						//	_Inbound_Outbound_Interface* inbound_outbound = (_Inbound_Outbound_Interface*)(*inbound_itr);

						//	_Link_Interface* link=inbound_outbound->template inbound_link_reference<_Link_Interface*>();

						//	approach->template inbound_link<_Link_Interface*>(link);
						//	approach->template green_cycle_ratio<int>(0.0);

						//	link->template approach<_Approach_Interface*>(approach);

						//	control_plan->template approach_data_array<_Approaches_Container_Interface&>().push_back(approach);
						//}
					}
				}
			}


			// template<typename TargetType> void write_operation_data(network_models::network_information::network_data_information::NetworkData& network_data, network_models::network_information::operation_data_information::OperationData& operation_data)
			// {
				// typedef  Network_Components::Prototypes::Network< typename get_type_of(network_reference)> _Network_Interface;
				// typedef  Intersection_Components::Prototypes::Intersection<typename remove_pointer< typename _Network_Interface::get_type_of(intersections_container)::value_type>::type>  _Intersection_Interface;
				// typedef  Random_Access_Sequence< typename _Network_Interface::get_type_of(intersections_container), _Intersection_Interface*> _Intersections_Container_Interface;

				// typedef  Intersection_Control_Components::Prototypes::Intersection_Control< typename _Intersection_Interface::get_type_of(intersection_control)> _Intersection_Control_Interface;
				// typedef  Intersection_Control_Components::Prototypes::Control_Plan<typename remove_pointer< typename _Intersection_Control_Interface::get_type_of(control_plan_data_array)::value_type>::type>  _Control_Plan_Interface;
				// typedef  Random_Access_Sequence< typename _Intersection_Control_Interface::get_type_of(control_plan_data_array), _Control_Plan_Interface*> _Control_Plans_Container_Interface;

				// typedef  Intersection_Control_Components::Prototypes::Phase<typename remove_pointer< typename _Control_Plan_Interface::get_type_of(phase_data_array)::value_type>::type>  _Phase_Interface;
				// typedef  Random_Access_Sequence< typename _Control_Plan_Interface::get_type_of(phase_data_array), _Phase_Interface*> _Phases_Container_Interface;

				// typedef  Intersection_Control_Components::Prototypes::Phase_Movement<typename remove_pointer< typename _Phase_Interface::get_type_of(turn_movements_in_the_phase_array)::value_type>::type>  _Phase_Movement_Interface;
				// typedef  Random_Access_Sequence< typename _Phase_Interface::get_type_of(turn_movements_in_the_phase_array), _Phase_Movement_Interface*> _Phase_Movements_Container_Interface;

				// typedef  Turn_Movement_Components::Prototypes::Movement<typename remove_pointer< typename _Network_Interface::get_type_of(turn_movements_container)::value_type>::type>  _Movement_Interface;
				// typedef  Random_Access_Sequence< typename _Network_Interface::get_type_of(turn_movements_container), _Movement_Interface*> _Movements_Container_Interface;

				// typedef  Intersection_Control_Components::Prototypes::Approach<typename remove_pointer< typename _Control_Plan_Interface::get_type_of(approach_data_array)::value_type>::type>  _Approach_Interface;
				// typedef  Random_Access_Sequence< typename _Control_Plan_Interface::get_type_of(approach_data_array), _Approach_Interface*> _Approaches_Container_Interface;

				// typedef  Link_Components::Prototypes::Link<typename remove_pointer< typename _Network_Interface::get_type_of(links_container)::value_type>::type>  _Link_Interface;
				// typedef  Random_Access_Sequence< typename _Network_Interface::get_type_of(links_container), _Link_Interface*> _Links_Container_Interface;



				// _Intersections_Container_Interface& intersections_container = network_reference<_Network_Interface*>()->template intersections_container<_Intersections_Container_Interface&>();

				// for (int i = 0; i < (int)intersections_container.size(); i++)
				// {
					// _Intersection_Interface* intersection = intersections_container[i];

					// network_models::network_information::operation_data_information::NodeControlData node_control_data;
					// _Intersection_Control_Interface* intersection_control = intersection->template intersection_control<_Intersection_Control_Interface*>();
					
					// node_control_data.node_index = intersection->template internal_id<int>();

					// for (int j = 0; j < (int)intersection_control->template control_plan_data_array<_Control_Plans_Container_Interface&>().size(); j++) 
					// {
						// _Control_Plan_Interface* control_plan = intersection_control->template control_plan_data_array<_Control_Plans_Container_Interface&>()[j];
						// network_models::network_information::operation_data_information::ControlPlanData control_plan_data;
						
						// control_plan_data.control_plan_index = control_plan->template control_plan_index<int>();
						// control_plan_data.starting_time = control_plan->template starting_time<int>();
						// control_plan_data.ending_time = control_plan->template ending_time<int>();
						// control_plan_data.control_type = control_plan->template control_type<network_models::network_information::network_data_information::Node_Type_Keys>();
						// control_plan_data.offset = control_plan->template offset<int>();
						// control_plan_data.cycle_length = control_plan->template cycle_length<int>();
						// control_plan_data.cycle_index = control_plan->template cycle_index<int>();
						// control_plan_data.cycle_starting_time = control_plan->template cycle_starting_time<int>();
						// control_plan_data.cycle_ending_time = control_plan->template cycle_ending_time<int>();
						// control_plan_data.cycle_leftover_time = control_plan->template cycle_leftover_time<int>();

						// for (int k = 0; k < (int)control_plan->template phase_data_array<_Phases_Container_Interface&>().size(); k++)
						// {
							// _Phase_Interface* phase = control_plan->template phase_data_array<_Phases_Container_Interface&>()[k];
							// network_models::network_information::operation_data_information::PhaseData phase_data;
							
							// phase_data.phase_index = phase->template phase_index<int>();
							// phase_data.phase_sequence_number = phase->template phase_sequence_number<int>();
							// phase_data.green_starting_time = phase->template green_starting_time<int>();
							// phase_data.yellow_starting_time = phase->template yellow_starting_time<int>();
							// phase_data.red_start_time = phase->template red_start_time<int>();
							// phase_data.maximum_green_time = phase->template maximum_green_time<int>();
							// phase_data.minimum_green_time = phase->template minimum_green_time<int>();
							// phase_data.offset = phase->template offset<int>();
							// phase_data.green_time = phase->template green_time<int>();
							// phase_data.yellow_time = phase->template yellow_time<int>();
							// phase_data.all_red_time = phase->template all_red_time<int>();
							// phase_data.adjusted_green_time = phase->template adjusted_green_time<int>();
							// phase_data.adjusted_yellow_time = phase->template adjusted_yellow_time<int>();
							// phase_data.adjusted_maximum_green_time = phase->template adjusted_maximum_green_time<int>();
							// phase_data.adjusted_minimum_green_time = phase->template adjusted_minimum_green_time<int>();

							// for (int l = 0; l < (int)phase->template turn_movements_in_the_phase_array<_Phase_Movements_Container_Interface&>().size(); l++)
							// {
								// _Phase_Movement_Interface* phase_movement = phase->template turn_movements_in_the_phase_array<_Phase_Movements_Container_Interface&>()[l];
								// network_models::network_information::operation_data_information::PhaseMovementData phase_movement_data;
								
								// _Movement_Interface* movement = phase_movement->template movement<_Movement_Interface*>();
								// phase_movement_data.turn_movement_index = phase_movement->template movement<_Movement_Interface*>()->template internal_id<int>();
								// phase_movement_data.movement_priority_type = phase_movement->template movement_priority_type<network_models::network_information::operation_data_information::Movement_Priority_Type_Keys>();
								// phase_movement_data.phase_movement_index = l;
								// phase_data.turn_movements_in_the_phase_array.push_back(phase_movement_data);

							// }
							// phase_data.num_turn_movements_in_the_phase = (int)phase_data.turn_movements_in_the_phase_array.size();

							// control_plan_data.phase_data_array.push_back(phase_data);

						// }
						// control_plan_data.num_phases = (int)control_plan_data.phase_data_array.size();

						// for (int m = 0; m < (int)control_plan->template approach_data_array<_Approaches_Container_Interface&>().size(); m++)
						// {
							// _Approach_Interface* approach = control_plan->template approach_data_array<_Approaches_Container_Interface&>()[m];
							// network_models::network_information::operation_data_information::ApproachData approach_data;
							
							// approach_data.approach_index = approach->template approach_index<int>();
							// approach_data.approach_type = approach->template approach_type<network_models::network_information::operation_data_information::Approach_Type_Keys>();
							// approach_data.inbound_link_index = approach->template inbound_link<_Link_Interface*>()->template internal_id<int>();
							// approach_data.green_cycle_ratio = approach->template green_cycle_ratio<int>();
							// control_plan_data.approach_data_array.push_back(approach_data);
						// }
						// control_plan_data.num_approaches = (int)control_plan_data.approach_data_array.size();

						// for (int n = 0; n < (int)control_plan->template major_approach_data_array<_Phases_Container_Interface&>().size(); n++)
						// {
							// _Approach_Interface* approach = (_Approach_Interface*)control_plan->template major_approach_data_array<_Approaches_Container_Interface&>()[n];
							// control_plan_data.major_approach_data_array.push_back(approach->template approach_index<int>());
						// }
						// control_plan_data.num_major_approaches = (int)control_plan_data.major_approach_data_array.size();

						// for (int o = 0; o < (int)control_plan->template minor_approach_data_array<_Phases_Container_Interface&>().size(); o++)
						// {
							// _Approach_Interface* approach = (_Approach_Interface*)control_plan->template minor_approach_data_array<_Approaches_Container_Interface&>()[o];
							// control_plan_data.minor_approach_data_array.push_back(approach->template approach_index<int>());
						// }
						// control_plan_data.num_minor_approaches = (int)control_plan_data.minor_approach_data_array.size();
						
						// control_plan_data.control_plan_index = control_plan->template control_plan_index<int>();
						// node_control_data.control_plan_data_array.push_back(control_plan_data);
					// }
					// node_control_data.num_control_plan_periods = (int)node_control_data.control_plan_data_array.size();
					// operation_data.node_control_data_array.push_back(node_control_data);
				// }
				// operation_data.node_control_data_size = (int)operation_data.node_control_data_array.size();

				// // initialize movement controls
				// int turn_movements_size = (int)network_reference<_Network_Interface*>()->template turn_movements_container<_Movements_Container_Interface&>().size();
				// operation_data.turn_movement_green_time_array.resize(turn_movements_size);
				// operation_data.turn_movement_merge_priority_array.resize(turn_movements_size);
				// operation_data.link_green_cycle_ratio_array.resize(turn_movements_size);

				// for (int p = 0; p < turn_movements_size; p++)
				// {
					// _Movement_Interface* movement = (_Movement_Interface*)network_reference<_Network_Interface*>()->template turn_movements_container<_Movements_Container_Interface&>()[p];
					// operation_data.turn_movement_merge_priority_array[p] = movement->template merge_priority<int>();
					// operation_data.turn_movement_green_time_array[p] = movement->template green_time<int>();
					// operation_data.link_green_cycle_ratio_array[p] = movement->template inbound_link_green_cycle_ratio<float>();
				// }
			// }


		};
	}
}

using namespace Operation_Components::Prototypes;

