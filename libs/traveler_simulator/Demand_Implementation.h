#pragma once
#include "Demand_Prototype.h"

namespace Demand_Components
{
	namespace Types
	{


	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{
		implementation struct Demand_Implementation:public Polaris_Component<MasterType,INHERIT(Demand_Implementation),Execution_Object>
		{
			typedef typename Polaris_Component<MasterType, INHERIT(Demand_Implementation), Execution_Object>::Component_Type ComponentType;
			typedef typename MasterType::movement_plan_type movement_plan_type;
			typedef std::pair<typename MasterType::movement_plan_type*, bool> movement_plan_pair_type;
			typedef std::vector<movement_plan_pair_type> movement_plans_vector_type;

			m_prototype(Null_Prototype,typename MasterType::scenario_type, scenario_reference, NONE, NONE);
			m_prototype(Null_Prototype,typename MasterType::network_type, network_reference, NONE, NONE);		
			m_container(std::vector<typename MasterType::vehicle_type*>, vehicles_container, NONE, NONE);
			m_container(std::vector<typename MasterType::vehicle_characteristics_type*>, vehicle_types_container, NONE, NONE);


			m_data(int, first_vehicle_departure_time, NONE, NONE);
			m_data(int, last_vehicle_departure_time, NONE, NONE);
			m_data(shared_ptr<odb::database>, db_ptr, NONE, NONE);

			member_component_and_feature_accessor(Logging_Interval, Value, Basic_Units::Prototypes::Time, Basic_Units::Implementations::Time_Implementation<NT>);
			member_component_and_feature_accessor(Next_Logging_Time, Value, Basic_Units::Prototypes::Time, Basic_Units::Implementations::Time_Implementation<NT>);

			typedef typename MasterType::traveler_type traveler_type;

			//TODO: Omer
			typedef typename MasterType::person_type person_type;

			template<typename TargetType> void Initialize()
			{
				typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;
				_Scenario_Interface* scenario = (_Scenario_Interface*)_global_scenario;

				// initialize storage arrays
				movement_plans = new movement_plans_vector_type[num_sim_threads()];
				movement_plans_buffer = new movement_plans_vector_type[num_sim_threads()];
				//trip_records = new std::vector<polaris::io::Trip>[num_sim_threads()];
				//trip_records_buffer = new std::vector<polaris::io::Trip>[num_sim_threads()];

				if (scenario->template write_demand_to_database<bool>())
				{
					string name(scenario->template output_demand_database_name<string&>());
					try
					{
						this->_db_ptr = open_sqlite_database_single<shared_ptr<odb::database> >(name);
						odb::transaction t(this->_db_ptr->begin());
						this->_db_ptr->execute("delete from trip");
						t.commit();
					}
					catch (odb::sqlite::database_exception ex)
					{
						cout << ex.message() << endl;
						THROW_EXCEPTION("DB error in accessing demand database '" << name << "'.")
					}
				}

				this->template Logging_Interval<Time_Minutes>(5);
				this->template Next_Logging_Time<Time_Minutes>(5);
				Simulation_Timestep_Increment first_time = this->template Next_Logging_Time<Simulation_Timestep_Increment>();

				this->template Load_Event<ComponentType>(&Logging_Event_Controller, first_time, Scenario_Components::Types::OUTPUT_WRITING_SUB_ITERATION);
			}


			//==============================================================
			// TRIP RECORD LOGGING FOR FIXED DEMAND.......
			//--------------------------------------------------------------

			movement_plans_vector_type* movement_plans;
			movement_plans_vector_type* movement_plans_buffer;

			static void Logging_Event_Controller(ComponentType* _this, Event_Response& response)
			{
				typedef typename MasterType::demand_type this_type;
				typedef Prototypes::Demand<this_type> _Interface;
				_Interface* this_ptr = (_Interface*)_this;
				this_type* pthis = (this_type*)_this;

				int iter = iteration();
				int subiter = sub_iteration();

				// set next planning time for other functions to use
				if (iteration() >= END - 2)
				{
					pthis->template Next_Logging_Time<Simulation_Timestep_Increment>(END);
				}
				else if (iteration() + pthis->template Logging_Interval<Simulation_Timestep_Increment>() < END - 2)
				{
					pthis->template Next_Logging_Time<Simulation_Timestep_Increment>(iteration() + pthis->template Logging_Interval<Simulation_Timestep_Increment>());
				}
				else
				{
					pthis->template Next_Logging_Time<Simulation_Timestep_Increment>(END-2);
				}

				// swap buffer and current for output strings and trip records
				if (sub_iteration() == Scenario_Components::Types::Type_Sub_Iteration_keys::OUTPUT_WRITING_SUB_ITERATION)
				{
					movement_plans_vector_type* tmp = pthis->movement_plans_buffer;
					pthis->movement_plans_buffer = pthis->movement_plans;
					pthis->movement_plans = tmp;

					response.next._iteration = this_ptr->template Next_Logging_Time<Simulation_Timestep_Increment>();
					response.next._sub_iteration = Scenario_Components::Types::Type_Sub_Iteration_keys::OUTPUT_WRITING_SUB_ITERATION;
					pthis->Write_Trips_To_Database<NT>();
				}
				else
				{
					response.next._iteration = this_ptr->template Next_Logging_Time<Simulation_Timestep_Increment>();
					response.next._sub_iteration = Scenario_Components::Types::Type_Sub_Iteration_keys::OUTPUT_WRITING_SUB_ITERATION;;
				}
			}

			template<typename TargetType> void Add_Trip_Record(TargetType movement_plan, bool write_trajectory)
			{
				movement_plans_buffer[__thread_id].push_back(movement_plan_pair_type(static_cast<movement_plan_type*>(movement_plan),write_trajectory));
			}

			template<typename TargetType> void Write_Trips_To_Database()
			{
				typedef Movement_Plan_Components::Prototypes::Movement_Plan<typename MasterType::movement_plan_type> movement_itf;
				typedef Link_Components::Prototypes::Link<typename movement_itf::get_type_of(origin)> link_itf;
				typedef Movement_Plan_Components::Prototypes::Trajectory_Unit<typename remove_pointer< typename movement_itf::get_type_of(trajectory_container)::value_type>::type>  _Trajectory_Unit_Interface;
				typedef Random_Access_Sequence< typename movement_itf::get_type_of(trajectory_container), _Trajectory_Unit_Interface*> _Trajectory_Container_Interface;
				typedef Activity_Location_Components::Prototypes::Activity_Location<typename MasterType::activity_location_type> location_itf;
				typedef Zone_Components::Prototypes::Zone<typename MasterType::zone_type> zone_itf;
				typedef Activity_Location_Components::Prototypes::Activity_Location<typename MasterType::activity_location_type> location_itf;
				typedef Zone_Components::Prototypes::Zone<typename MasterType::zone_type> zone_itf;
				typedef Movement_Plan_Components::Prototypes::Movement_Plan<typename MasterType::movement_plan_type> movement_itf;
				typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;
				_Scenario_Interface* scenario = (_Scenario_Interface*)_global_scenario;

				if (!scenario->template write_demand_to_database<bool>()) return;

				for (int i = 0; i< (int)num_sim_threads(); ++i)
				{
					// database write for trips
					typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;
					_Scenario_Interface* scenario = (_Scenario_Interface*)_global_scenario;
					if (scenario->template write_demand_to_database<bool>())
					{
						int count = 0;

						try
						{
							odb::transaction t(this->_db_ptr->begin());

							// iterate over trip-activity pairs and push to database
							/*for (std::vector<polaris::io::Trip>::iterator itr = trip_records[i].begin(); itr != trip_records[i].end(); ++itr)
							{
								polaris::io::Trip& t = *itr;
								this->_db_ptr->persist(t);
								count++;
							}*/
							for (movement_plans_vector_type::iterator itr = movement_plans[i].begin(); itr != movement_plans[i].end(); ++itr)
							{
								shared_ptr<polaris::io::Path> path_db_record;

								movement_itf* move = (movement_itf*)itr->first;
								location_itf* orig = move->template origin<location_itf*>();
								location_itf* dest = move->template destination<location_itf*>();
								bool write_trajectory = itr->second;

								//==============================================================================================
								// write the trajectory first, if requested	
								if (write_trajectory)
								{
									// Fill the PATH DB record
									path_db_record = make_shared<polaris::io::Path>();
									//path_db_record->setVehicle(vehicle->vehicle_ptr<shared_ptr<polaris::io::Vehicle>>());
									path_db_record->setTraveler_ID(move->traveler_id<int>());
									path_db_record->setOrigin_Activity_Location(move->template origin<location_itf*>()->template uuid<int>());
									path_db_record->setDestination_Activity_Location(move->template destination<location_itf*>()->template uuid<int>());
									path_db_record->setOrigin_Link(move->template origin<link_itf*>()->template uuid<int>());
									path_db_record->setDestination_Link(move->template destination<link_itf*>()->template uuid<int>());
									path_db_record->setNum_Links(move->template trajectory_container<_Trajectory_Container_Interface&>().size());
									path_db_record->setDeparture_Time(move->template departed_time<Time_Seconds>());
									path_db_record->setTravel_Time(move->template arrived_time<Time_Seconds>() - move->template departed_time<Time_Seconds>());
									path_db_record->setRouted_Time(move->template estimated_travel_time_when_departed<float>());

									_Trajectory_Container_Interface& trajectory = ((movement_itf*)move)->template trajectory_container<_Trajectory_Container_Interface&>();
									float start = 0;
									int route_link_counter = 0;
									for (auto link_itr = trajectory.begin(); link_itr != trajectory.end(); ++link_itr, ++route_link_counter)
									{
										// FIll the path link DB record for each step of the path
										polaris::io::link_travel path_link_record;
										_Trajectory_Unit_Interface* trajectory_unit = (_Trajectory_Unit_Interface*)(*link_itr);
										link_itf* route_link = trajectory_unit->template link<link_itf*>();
										int route_link_id = route_link->template uuid<int>();
										int route_link_enter_time = trajectory_unit->template enter_time<int>();
										float route_link_delayed_time = float(trajectory_unit->template intersection_delay_time<float>());
										int route_link_exit_time = move->template get_route_link_exit_time<NULLTYPE>(route_link_counter);
										float route_link_travel_time = float((route_link_exit_time - route_link_enter_time));

										path_link_record.setLink(route_link->dbid<int>());
										path_link_record.setDir(route_link->direction<int>());
										path_link_record.setEntering_Time(route_link_enter_time);
										path_link_record.setTravel_Time(route_link_travel_time);
										path_link_record.setDelayed_Time(route_link_delayed_time);
										path_link_record.setExit_Position(start += GLOBALS::Convert_Units<Feet, Meters>(route_link->template length<float>()));
										path_db_record->setLink(path_link_record);
									}
								}

								if (path_db_record) this->_db_ptr->persist(path_db_record);

								//==============================================================================================
								// create trip record, only if it represents a valid movement (i.e. not the null first trip of the day)		
								polaris::io::Trip trip_rec;
								trip_rec.setConstraint(0);
								trip_rec.setPerson(0);
								trip_rec.setTrip(move->traveler_id<int>());
								trip_rec.setDestination(dest->template uuid<int>());
								trip_rec.setDuration(0);
								if (move->routed_travel_time<Time_Seconds>() > 0) trip_rec.setGap(max(float((move->arrived_time<Time_Seconds>() - move->departed_time<Time_Seconds>()) / move->routed_travel_time<Time_Seconds>() - 1.0f), 0.0f));
								else trip_rec.setGap(0.0f);
								trip_rec.setEnd(move->template arrived_time<Time_Seconds>());
								trip_rec.setHhold(0);
								trip_rec.setMode(Vehicle_Components::Types::Vehicle_Type_Keys::SOV);
								trip_rec.setOrigin(orig->template uuid<int>());
								trip_rec.setPartition(move->template routed_travel_time<int>());
								trip_rec.setPassengers(0);
								trip_rec.setPurpose(0);
								trip_rec.setStart(move->template departed_time<Time_Seconds>());
								trip_rec.setTour(0);
								trip_rec.setPriority(0);
								trip_rec.setType(1);
								trip_rec.setPath(path_db_record);
								this->_db_ptr->persist(trip_rec);
								count++;
							}
							t.commit();
						}
						catch (const odb::exception& e)
						{
							cout << e.what() << ". DB error in demand_implementation, line 235.  count=" << count << endl;
							//polaris::io::Trip p = trip_records[i][count];

						}
						catch (std::exception& e)
						{
							cout << e.what() << ". DB error in demand_implementation, line 241.  count=" << count << endl;
						}
						catch (...)
						{
							cout << "some other error in database writing" << endl;
						}

						// erase buffer
						//trip_records[i].clear();
						movement_plans[i].clear();
					}
				}
			}


		};


	}

}
