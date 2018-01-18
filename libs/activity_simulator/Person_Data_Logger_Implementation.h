#pragma once

//#include "Io/Io_db.h"
#include "Person_Data_Logger_Prototype.h"
#include "Activity_Implementations.h"

namespace Person_Components
{
	namespace Implementations
	{
		//==================================================================================
		/// Planning classes
		//----------------------------------------------------------------------------------
		implementation struct Person_Data_Logger_Implementation : public Polaris_Component<MasterType,INHERIT(Person_Data_Logger_Implementation),Execution_Object>
		{
			typedef typename Polaris_Component<MasterType,INHERIT(Person_Data_Logger_Implementation),Execution_Object>::Component_Type ComponentType;
			std::vector<int>* planned_acts;
			std::vector<int>* executed_acts;
			std::vector<int>* ttime_distribution;
			std::vector<string>* output_data;
			std::vector<string>* output_data_buffer;
			std::vector<pair<polaris::io::Trip,polaris::io::Activity>>* activity_records;
			std::vector<pair<polaris::io::Trip,polaris::io::Activity>>* activity_records_buffer;
			std::vector<polaris::io::multimodalSPLabels>* multimodalSPLabels;
			std::vector<polaris::io::multimodalSPLabels>* multimodalSPLabels_buffer;



			float* expected_ttime;
			float* routed_ttime;
			float* actual_ttime;
			int* num_acts_in_interval;


			std::vector<string>* buff;
			std::vector<string>* current;


			m_data(ofstream, log, NONE, NONE);
			m_data(string, filename, NONE, NONE);
			m_data(ofstream, ttime_file, NONE, NONE);
			m_data(ofstream, executed_acts_file, NONE, NONE);
			m_data(ofstream, external_demand_output_file, NONE, NONE);
			m_data(ofstream, demand_moe_file, NONE, NONE);
			m_data(shared_ptr<odb::database>, db_ptr, NONE, NONE);

			// GRAPHICAL DATA MEMBERS
			m_data(float, activity_time_lost, NONE, NONE);
			m_data(float, cancelled_activities, NONE, NONE);
			m_data(float, replanned_activities, NONE, NONE);

			member_component_and_feature_accessor(Logging_Interval, Value, Basic_Units::Prototypes::Time,Basic_Units::Implementations::Time_Implementation<NT>);
			member_component_and_feature_accessor(Next_Logging_Time, Value, Basic_Units::Prototypes::Time,Basic_Units::Implementations::Time_Implementation<NT>);

			template<typename TargetType> void Initialize()
			{
				typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;
				_Scenario_Interface* scenario = (_Scenario_Interface*)_global_scenario;
				this->_cancelled_activities = 0;
				this->_activity_time_lost = 0;

				// initialize storage arrays
				planned_acts = new std::vector<int>[num_sim_threads()];
				executed_acts = new std::vector<int>[num_sim_threads()];
				ttime_distribution = new std::vector<int>[num_sim_threads()];
				output_data = new std::vector<string>[num_sim_threads()];
				output_data_buffer = new std::vector<string>[num_sim_threads()];

				//trip_records = new std::vector<shared_ptr<polaris::io::Trip>>[num_sim_threads()];
				//trip_records_buffer = new std::vector<shared_ptr<polaris::io::Trip>>[num_sim_threads()];
				//activity_records = new std::vector<shared_ptr<polaris::io::Activity>>[num_sim_threads()];
				//activity_records_buffer = new std::vector<shared_ptr<polaris::io::Activity>>[num_sim_threads()];
				//trip_records = new std::vector<polaris::io::Trip*>[num_sim_threads()];
				//trip_records_buffer = new std::vector<polaris::io::Trip*>[num_sim_threads()];
				activity_records = new std::vector<pair<polaris::io::Trip,polaris::io::Activity>>[num_sim_threads()];
				activity_records_buffer = new std::vector<pair<polaris::io::Trip,polaris::io::Activity>>[num_sim_threads()];
				multimodalSPLabels = new std::vector<polaris::io::multimodalSPLabels>[num_sim_threads()];
				multimodalSPLabels_buffer = new std::vector<polaris::io::multimodalSPLabels>[num_sim_threads()];

				expected_ttime = new float[num_sim_threads()];
				routed_ttime = new float[num_sim_threads()];
				actual_ttime = new float[num_sim_threads()];
				num_acts_in_interval = new int[num_sim_threads()];

				if (scenario->template write_demand_to_database<bool>())
				{
					string name(scenario->template output_demand_database_name<string&>());
					try
					{
						this->_db_ptr = open_sqlite_database_single<shared_ptr<odb::database> > (name);
						odb::transaction t(this->_db_ptr->begin());
						this->_db_ptr->execute("delete from trip");
						t.commit();
					}
					catch (odb::sqlite::database_exception ex)
					{
						cout << ex.message()<<endl;
						THROW_EXCEPTION("DB error in accessing demand database '"<<name<<"'.")
					}
				}

				this->template Logging_Interval<Time_Minutes>(5);
				this->template Next_Logging_Time<Time_Minutes>(5);
				Simulation_Timestep_Increment first_time = this->template Next_Logging_Time<Simulation_Timestep_Increment>();
				

				//load_event(ComponentType,Logging_Conditional,Write_Data_To_File_Event,first_time,0,NULLTYPE);
				this->template Load_Event<ComponentType>(&Logging_Event_Controller,first_time,Scenario_Components::Types::OUTPUT_WRITING_SUB_ITERATION);

				// Initialize pointers to data buffers
				buff = output_data_buffer;
				current = output_data;

				//trip_buff = trip_records_buffer;
				//trip_current = trip_records;

				//activity_buff = activity_records_buffer;
				//activity_current = activity_records;


				// Initialize log file
				stringstream filename("");
				filename << scenario->template output_dir_name<string>();
				filename << "activity_output.xls";
				this->_filename = filename.str();
				this->_log.open(this->_filename);
				this->_log << "PER_ID\tACT_ID\tACT_TYP\tDEST_ZONE\tplanning_info\tTTIME_min\tEXECUTED\tORIG\tDEST\tDEPART\tARRIVE\tSTART\tEND\tMODE"<<endl;


				// Initialize data counter files
				stringstream filename_ttime("");
				filename_ttime << scenario->template output_dir_name<string>();
				filename_ttime << "ttime_distribution.csv";
				this->_ttime_file.open(filename_ttime.str());
				if (!this->_ttime_file.is_open()) THROW_EXCEPTION("ERROR: ttime distribution file could not be created.");
				this->_ttime_file <<"TIME(s),5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,105,110,115,120,120+";
				
				// Initialize executed activities file
				stringstream filename_acts("");
				filename_acts << scenario->template output_dir_name<string>();
				filename_acts << "executed_activities.csv";
				this->_executed_acts_file.open(filename_acts.str());
				if (!this->_executed_acts_file.is_open())THROW_EXCEPTION("ERROR: executed activity distribution file could not be created.");
				this->_executed_acts_file <<"TIME(s),0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,23"<<endl;

				// Initialize data count containers
				for (int i=0; i<(int)num_sim_threads();++i)
				{
					ttime_distribution[i].resize(25,0); 
					executed_acts[i].resize(Activity_Components::Types::ACTIVITY_TYPES::Last+1);
					planned_acts[i].resize(Activity_Components::Types::ACTIVITY_TYPES::Last + 1);
				}

				// Initialize demand MOE file
				stringstream filename_moe("");
				filename_moe << scenario->template output_dir_name<string>();
				filename_moe << "moe_demand.csv";
				this->_demand_moe_file.open(filename_moe.str());
				if (!this->_demand_moe_file.is_open())THROW_EXCEPTION("ERROR: demand moe file could not be created.");
				this->_demand_moe_file <<"TIME(s),LOST_ACTIVITY_TIME(min),Cancelled_Acts_Count,acts_completed,avg_expected_ttime,avg_routed_ttime,avg_actual_ttime"<<endl;
			}

			template<typename TargetType> void Add_Record(TargetType act_record, bool is_executed)
			{		
				// update the summary information
				this->Add_Summary_Record<TargetType>(act_record, is_executed);


				stringstream s;
				typedef Activity_Components::Prototypes::Activity_Planner<typename MasterType::activity_type> act_record_itf;
				typedef Movement_Plan_Components::Prototypes::Movement_Plan<typename MasterType::movement_plan_type> movement_itf;
				typedef Activity_Location_Components::Prototypes::Activity_Location<typename MasterType::activity_location_type> location_itf;
				typedef Zone_Components::Prototypes::Zone<typename MasterType::zone_type> zone_itf;
				typedef Household_Components::Prototypes::Household<typename MasterType::household_type> household_itf;
				typedef Person_Components::Prototypes::Person<typename MasterType::person_type> person_itf;
				typedef Person_Components::Prototypes::Person_Planner<typename person_itf::get_type_of(Planning_Faculty)> person_planner_itf;
				
				// don't do logging if not specified in scenario
				typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;
				_Scenario_Interface* scenario = (_Scenario_Interface*)_global_scenario;
				
				
				act_record_itf* act = (act_record_itf*)act_record;
				person_planner_itf* planner = act->template Parent_Planner<person_planner_itf*>();
				person_itf* person = planner->template Parent_Person<person_itf*>();
				household_itf* hh = person->person_itf::template Household<household_itf*>();

				location_itf* loc = act->template Location<location_itf*>();

				

				// determine delay, using current iteration as the arrival time
				if (is_executed)
				{
					float expected_start = act->template Start_Time<Time_Minutes>();
					float actual_start = GLOBALS::Simulation_Time.template Current_Time<Time_Minutes>();
					float delay = max<float>(actual_start - expected_start,0.0f);
					this->_activity_time_lost += delay;

					expected_ttime[__thread_id] += (float)act->template Expected_Travel_Time<Time_Seconds>();
					actual_ttime[__thread_id] += (float)act->template Actual_Travel_Time<Time_Seconds>();
					routed_ttime[__thread_id] += (float)act->template movement_plan<movement_itf*>()->template routed_travel_time<float>();
					num_acts_in_interval[__thread_id]++;
				}


				// count the number of acts added
				if (!is_executed) planned_acts[__thread_id][act->template Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>()] +=1;


				// exit if not writing output
				if (!scenario->template write_activity_output<bool>()) return;
				return;


				stringstream PERSON_ID;
				PERSON_ID << hh->template uuid<float>();
				PERSON_ID << ".";
				PERSON_ID << person->template uuid<float>();


				if (loc == nullptr)
				{
					s << PERSON_ID.str() << "\t"<<act->template Activity_Plan_ID<int>()<<"\t" << act->template Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>();
					//THROW_WARNING("Warning, null location pointer for activity record: " << act->template Parent_ID<int>() << "." << act->template Activity_Plan_ID<int>()); 
					s<<"\tNull location pointer - act not planned?\t";
					Revision& location = act->template Location_Planning_Time<Revision&>();
					Revision& start = act->template Start_Time_Planning_Time<Revision&>();
					Revision& route = act->template Route_Planning_Time<Revision&>();
					s << "\tPlan times (loc,start,route): "<<location._iteration<<"."<<location._sub_iteration<<" , " << start._iteration <<"."<<start._sub_iteration<<" , " << route._iteration<<"."<<route._sub_iteration;
					buff[__thread_id].push_back(s.str());
					return;
				}
				zone_itf* zone = loc->template zone<zone_itf*>();

				Revision& location = act->template Location_Planning_Time<Revision&>();
				Revision& start = act->template Start_Time_Planning_Time<Revision&>();
				Revision& route = act->template Route_Planning_Time<Revision&>();
				s << PERSON_ID.str()  << "\t"<<act->template Activity_Plan_ID<int>()<<"\t" << act->Get_Type_String() << "\t"<<zone->template uuid<int>()<<"\t";
				s << "Plan times (loc,start,route): "<<location._iteration<<"."<<location._sub_iteration<<" , " << start._iteration <<"."<<start._sub_iteration<<" , " << route._iteration<<"."<<route._sub_iteration;
					
				if (!is_executed)
				{
					s<<"\t"<<act->template Expected_Travel_Time<Time_Minutes>()<<"\t"<<0;
				}
				else
				{
					Time_Minutes ttime = act->template Actual_Travel_Time<Time_Minutes>();
					s<<"\t"<<ttime<<"\t"<<1;
				}
				

				// Movement plans
				movement_itf* move = act->template movement_plan<movement_itf*>();

				s << "\t";
				s <<move->template origin<location_itf*>()->template uuid<int>()<< "\t"<<move->template destination<location_itf*>()->template uuid<int>()<< "\t";
				s <<move->template departed_time<Time_Days>()<< "\t"<<move->template arrived_time<Time_Days>()<< "\t"<<act->template Start_Time<Time_Days>() << "\t"<<act->template End_Time<Time_Days>() << "\t"<< act->template Mode<int>();
				buff[__thread_id].push_back(s.str());
			}
			template<typename TargetType> void Add_Summary_Record(TargetType act_record, bool is_executed)
			{		
				stringstream s;
				typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;
				_Scenario_Interface* scenario = (_Scenario_Interface*)_global_scenario;
				typedef Activity_Components::Prototypes::Activity_Planner<typename MasterType::activity_type> act_record_itf;
				typedef Activity_Location_Components::Prototypes::Activity_Location<typename MasterType::activity_location_type> location_itf;
				typedef Zone_Components::Prototypes::Zone<typename MasterType::zone_type> zone_itf;
				
				
				act_record_itf* act = (act_record_itf*)act_record;
				location_itf* loc = act->template Location<location_itf*>();

				//==========================================================
				//----------------------------------------------------------
				if (scenario->template write_demand_to_database<bool>())
				{
					typedef Movement_Plan_Components::Prototypes::Movement_Plan<typename act_record_itf::get_type_of(movement_plan)> movement_itf;
					typedef Prototypes::Person_Planner<typename act_record_itf::get_type_of(Parent_Planner)> planner_itf;
					typedef Prototypes::Person<typename planner_itf::get_type_of(Parent_Person)> person_itf;

					Push_To_Demand_Database<TargetType>(act_record, is_executed);

				}

				typedef Movement_Plan_Components::Prototypes::Movement_Plan<typename act_record_itf::get_type_of(movement_plan)> movement_itf;
				movement_itf* move = act->template movement_plan<movement_itf*>();
				if (scenario->template multimodal_routing<bool>() && Routing_Components::Implementations::Routable_Network_Implementation<MasterType>::debug_route<bool>() && (move->template mode<Vehicle_Components::Types::Vehicle_Type_Keys>() == Vehicle_Components::Types::Vehicle_Type_Keys::BUS || move->template mode<Vehicle_Components::Types::Vehicle_Type_Keys>() == Vehicle_Components::Types::Vehicle_Type_Keys::WALK || move->template mode<Vehicle_Components::Types::Vehicle_Type_Keys>() == Vehicle_Components::Types::Vehicle_Type_Keys::BICYCLE || move->template mode<Vehicle_Components::Types::Vehicle_Type_Keys>() == Vehicle_Components::Types::Vehicle_Type_Keys::PARK_AND_RIDE))
				{
					/*typedef Movement_Plan_Components::Prototypes::Movement_Plan<typename act_record_itf::get_type_of(movement_plan)> movement_itf;
					typedef Prototypes::Person_Planner<typename act_record_itf::get_type_of(Parent_Planner)> planner_itf;
					typedef Prototypes::Person<typename planner_itf::get_type_of(Parent_Person)> person_itf;*/
					set_multimodal_trajectory<TargetType>(move->template summary_string<std::string>());
				}

				if (!is_executed) return;

				//----------------------------------------------------------
				//==========================================================

				//std::cout << "Incrementing executed_acts: thread_id=" << __thread_id << " Activity index=" << act->template Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>() << std::endl;
				executed_acts[__thread_id][act->template Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>()] +=1;
			
				// update travel time distributions
				Time_Minutes ttime = act->template Actual_Travel_Time<Time_Minutes>();
				if (ttime < 5) ttime_distribution[__thread_id][0]++;
				else if (ttime < 10) ttime_distribution[__thread_id][1]++;
				else if (ttime < 15) ttime_distribution[__thread_id][2]++;
				else if (ttime < 20) ttime_distribution[__thread_id][3]++;
				else if (ttime < 25) ttime_distribution[__thread_id][4]++;
				else if (ttime < 30) ttime_distribution[__thread_id][5]++;
				else if (ttime < 35) ttime_distribution[__thread_id][6]++;
				else if (ttime < 40) ttime_distribution[__thread_id][7]++;
				else if (ttime < 45) ttime_distribution[__thread_id][8]++;
				else if (ttime < 50) ttime_distribution[__thread_id][9]++;
				else if (ttime < 55) ttime_distribution[__thread_id][10]++;
				else if (ttime < 60) ttime_distribution[__thread_id][11]++;
				else if (ttime < 65) ttime_distribution[__thread_id][12]++;
				else if (ttime < 70) ttime_distribution[__thread_id][13]++;
				else if (ttime < 75) ttime_distribution[__thread_id][14]++;
				else if (ttime < 80) ttime_distribution[__thread_id][15]++;
				else if (ttime < 85) ttime_distribution[__thread_id][16]++;
				else if (ttime < 90) ttime_distribution[__thread_id][17]++;
				else if (ttime < 95) ttime_distribution[__thread_id][18]++;
				else if (ttime < 100) ttime_distribution[__thread_id][19]++;
				else if (ttime < 105) ttime_distribution[__thread_id][20]++;
				else if (ttime < 110) ttime_distribution[__thread_id][21]++;
				else if (ttime < 115) ttime_distribution[__thread_id][22]++;
				else if (ttime < 120) ttime_distribution[__thread_id][23]++;
				else  ttime_distribution[__thread_id][24]++;
			}
			template<typename TargetType> void Increment_Cancelled_Activities()
			{
				this->_cancelled_activities++;
			}
			template<typename TargetType> void Increment_Replanned_Activities()
			{
				this->_replanned_activities++;
			}

			static void Logging_Event_Controller(ComponentType* _this,Event_Response& response)
			{
				typedef typename MasterType::person_data_logger_type this_type;
				typedef Prototypes::Person_Data_Logger<typename MasterType::person_data_logger_type> _Interface;
				_Interface* this_ptr=(_Interface*)_this;
				this_type* pthis = (this_type*)_this;
				
				// set next planning time for other functions to use
				this_ptr->template Next_Logging_Time<Simulation_Timestep_Increment>(iteration() + this_ptr->template Logging_Interval<Simulation_Timestep_Increment>());

				// swap buffer and current for output strings and trip records
				if(sub_iteration() == 0)
				{				
					std::vector<string>* tmp = pthis->buff;
					pthis->buff = pthis->current;
					pthis->current = tmp;

					//std::vector<shared_ptr<polaris::io::Trip>>* tmp_trip = pthis->trip_buff;
					//std::vector<polaris::io::Trip*>* tmp_trip = pthis->trip_records_buffer;
					//pthis->trip_records_buffer = pthis->trip_records;
					//pthis->trip_records = tmp_trip;

					//std::vector<shared_ptr<polaris::io::Activity>>* tmp_act = pthis->activity_buff;
					std::vector<pair<polaris::io::Trip,polaris::io::Activity>>* tmp_act = pthis->activity_records_buffer;
					pthis->activity_records_buffer = pthis->activity_records;
					pthis->activity_records = tmp_act;

					//OMER mimicking above
					std::vector<polaris::io::multimodalSPLabels>* tmp_mm = pthis->multimodalSPLabels_buffer;
					pthis->multimodalSPLabels_buffer = pthis->multimodalSPLabels;
					pthis->multimodalSPLabels = tmp_mm;

					response.next._iteration = this_ptr->template Next_Logging_Time<Simulation_Timestep_Increment>();
					response.next._sub_iteration = 0;
					pthis->Write_Data_To_File_Event<NT>();
				}
				//else if (sub_iteration() < (int)num_sim_threads())
				//{
				//	response.next._iteration = iteration();
				//	response.next._sub_iteration = sub_iteration()+1;
				//	pthis->Write_Data_To_File_Event<NT>();
				//}
				else
				{
					response.next._iteration = this_ptr->template Next_Logging_Time<Simulation_Timestep_Increment>();
					response.next._sub_iteration = 0;
				}
			}

			template<typename TargetType> void Write_Data_To_File_Event()
			{
				for (int i=0; i< (int)num_sim_threads(); ++i)
				{

					// write out strings in the current buffer to log file and clear it
					for (std::vector<string>::iterator itr = current[i].begin(); itr != current[i].end(); ++itr)
					{
						this->_log << '\n' << *itr;
					}
					current[i].clear();

					// do aggregated file writing on first subiteration
					if (i == 0)
					{
						// display the ttime and executed activity count distributions once
						this->_ttime_file << iteration();
						for (int j=0; j < 25; j++)
						{
							int count = 0;
							for (int k=0; k < (int)num_sim_threads(); k++) // collect value over all threads
							{
								count += ttime_distribution[k][j];
							}
							this->_ttime_file <<","<< count;
						}
						this->_ttime_file << endl;

						this->_executed_acts_file << iteration();
						for (int j=0; j < 20; j++)
						{
							int count = 0;
							for (int k=0; k < (int)num_sim_threads(); k++) // collect value over all threads
							{
								count += executed_acts[k][j];
							}
							this->_executed_acts_file <<","<< count;
						}
						this->_executed_acts_file << endl;


						//=================================================================================
						// write to the Demand MOE File
						this->_demand_moe_file << iteration() << "," << this->_activity_time_lost;

						// write the cancelled activities 
						this->_demand_moe_file << "," << this->_cancelled_activities;

						// write the average expected and actual travel times over the current interval
						float actual_ttime_sum=0;
						float routed_ttime_sum=0;
						float expected_ttime_sum=0;
						float acts_in_interval = 0;
						for (int j=0; j < (int)num_sim_threads(); j++)
						{
							actual_ttime_sum += actual_ttime[j];
							expected_ttime_sum += expected_ttime[j];
							routed_ttime_sum += routed_ttime[j];
							acts_in_interval += num_acts_in_interval[j];
							actual_ttime[j]=0;
							expected_ttime[j]=0;
							routed_ttime[j]=0;
							num_acts_in_interval[j]=0;
						}
						this->_demand_moe_file << "," << acts_in_interval;
						this->_demand_moe_file << "," << expected_ttime_sum / acts_in_interval;
						this->_demand_moe_file << "," << routed_ttime_sum / acts_in_interval;
						this->_demand_moe_file << "," << actual_ttime_sum / acts_in_interval<<endl;
					}


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
							for (std::vector<pair<polaris::io::Trip,polaris::io::Activity>>::iterator itr = activity_records[i].begin(); itr != activity_records[i].end(); ++itr)
							{
								polaris::io::Trip& t = itr->first;
								polaris::io::Activity& a = itr->second;

								unsigned long t_id;
								// only write trip if the type is not -1 (i.e. for executed trips)
								if (t.getType() != -1) t_id = this->_db_ptr->persist(t);

								else t_id = 0;
								a.setTrip (t_id);
								this->_db_ptr->persist(a);
								count++;
							}
							t.commit();
						}
						catch (const odb::exception& e)
						{
							cout << e.what()<<". DB error in person_data_logger_implementation, line 493.  count="<<count<<endl;
							pair<polaris::io::Trip,polaris::io::Activity> p = activity_records[i][count];
							
						}
						catch (std::exception& e)
						{
							cout << e.what() << ". DB error in person_data_logger_implementation, line 499.  count=" << count << endl;
						}
						catch (...)
						{
							cout << "some other error in database writing" << endl;
						}

						// erase buffer
						activity_records[i].clear();
					}
				}
			}
			template<typename TargetType> void Write_Summary_Data_To_File()
			{
				//for (int i = 0; i < 20; ++i)
				//{
				//	cout << endl << "Number of activity type : " << num_acts[i];
				//}

				int i = sub_iteration();

				// display the ttime and executed activity count distributions once
				if (i == 0)
				{
					this->_ttime_file << iteration();
					for (int j=0; j < 25; j++)
					{
						int count = 0;
						for (int k=0; k < num_sim_threads(); k++) // collect value over all threads
						{
							count += ttime_distribution[k][j];
						}
						this->_ttime_file <<","<< count;
					}
					this->_ttime_file << endl;

					this->_executed_acts_file << iteration();
					for (int j=0; j < 20; j++)
					{
						int count = 0;
						for (int k=0; k < num_sim_threads(); k++) // collect value over all threads
						{
							count += executed_acts[k][j];
						}
						this->_executed_acts_file <<","<< count;
					}
					this->_executed_acts_file << endl;
				}
			}

			bool is_external_internal_trip(int O, int D, int H)
			{
				bool o_is_internal = is_internal(O);
				bool d_is_internal = is_internal(D);
				bool h_is_internal = is_internal(H);

				// not external trip - lives in CBD
				if (h_is_internal) return false;

				if (!o_is_internal && d_is_internal) return true;
				else return false;
			}
			bool is_internal_external_trip(int O, int D, int H)
			{
				bool o_is_internal = is_internal(O);
				bool d_is_internal = is_internal(D);
				bool h_is_internal = is_internal(H);

				// not external trip - lives in CBD
				if (h_is_internal) return false;

				if (o_is_internal && !d_is_internal) return true;
				else return false;
			}
			bool is_internal(int id)
			{
				bool is_internal = false;
				if (id <= 88 || (id >= 166 && id <= 168) || (id >= 193 && id <= 196)) is_internal = true; 
				return is_internal;
			}
			template<typename TargetType> int get_nearest_external_location(TargetType zone)
			{
				typedef Network_Components::Prototypes::Network<typename MasterType::network_type> _Network_Interface;
				typedef Activity_Location_Components::Prototypes::Activity_Location<typename MasterType::activity_location_type> location_itf;
				typedef Zone_Components::Prototypes::Zone<typename remove_pointer<typename _Network_Interface::get_type_of(zones_container)::value_type>::type>  zone_itf;
				typedef  Pair_Associative_Container<typename _Network_Interface::get_type_of(zones_container), zone_itf*> zones_container_itf;

				_Network_Interface* network = (_Network_Interface*)_global_network;
				zones_container_itf* zones_list = network->template zones_container<zones_container_itf*>();

				int zone_ids[7] = {143,149,165,183,192,223,225};

				float min_distance = FLT_MAX;

				float distance;

				zone_itf* original_zone = (zone_itf*)zone;
				float orig_x = original_zone->template X<float>();
				float orig_y = original_zone->template Y<float>();

				zone_itf* external_zone, *closest_zone;
				float new_x, new_y;

				typename zones_container_itf::iterator zone_itr;
				for (int i = 0; i < 7; i++)
				{
					zone_itr = zones_list->find(zone_ids[i]);
					if (zone_itr != zones_list->end()) external_zone = zone_itr->second;
					new_x = external_zone->template X<float>();
					new_y = external_zone->template Y<float>();

					distance = sqrt(pow(orig_x - new_x,2) + pow(orig_y - new_y,2));

					if (distance < min_distance)
					{
						min_distance = distance;
						closest_zone = external_zone;
					}
				}

				location_itf* closest_loc = closest_zone->template Get_Random_Location<location_itf*>();

				//cout <<endl<<"Original zone: " << original_zone->template uuid<int>() << ", closes zone: " << closest_zone->template uuid<int>() << ", new location id: " << closest_loc->template uuid<int>();

				//return closest_loc->template uuid<int>();
				return closest_zone->template uuid<int>();
			}



			template<typename TargetType> void Push_To_Demand_Database(TargetType act_record, bool is_executed, int new_origin=-1, int new_destination=-1)
			{
				typedef Activity_Components::Prototypes::Activity_Planner<typename MasterType::activity_type> act_itf;
				typedef Activity_Location_Components::Prototypes::Activity_Location<typename MasterType::activity_location_type> location_itf;
				typedef Zone_Components::Prototypes::Zone<typename MasterType::zone_type> zone_itf;
				typedef Movement_Plan_Components::Prototypes::Movement_Plan<typename act_itf::get_type_of(movement_plan)> movement_itf;
				typedef Prototypes::Person_Planner<typename act_itf::get_type_of(Parent_Planner)> planner_itf;
				typedef Prototypes::Person<typename planner_itf::get_type_of(Parent_Person)> person_itf;
				typedef Household_Components::Prototypes::Household<typename person_itf::get_type_of(Household)> household_itf;
				typedef Random_Access_Sequence<typename household_itf::get_type_of(Vehicles_Container)> vehicles_container_itf;
				typedef Vehicle_Components::Prototypes::Vehicle<typename get_component_type(vehicles_container_itf)> vehicle_itf;

				act_itf* act = (act_itf*)act_record;
				movement_itf* move = act->template movement_plan<movement_itf*>();
				location_itf* orig = move->template origin<location_itf*>();
				location_itf* dest = move->template destination<location_itf*>();
				planner_itf* planner = act->template Parent_Planner<planner_itf*>();
				person_itf* person = planner->template Parent_Person<person_itf*>();		
				household_itf* hh = person->person_itf::template Household<household_itf*>();


				//==============================================================================================
				// create trip record, only if it represents a valid movement (i.e. not the null first trip of the day)		
				//shared_ptr<polaris::io::Trip> trip_rec(new polaris::io::Trip());
				//polaris::io::Trip* trip_rec = new polaris::io::Trip();
				polaris::io::Trip trip_rec;
				if (is_executed)
				{
					trip_rec.setConstraint(0);
					trip_rec.setPerson(person->person_record<shared_ptr<polaris::io::Person>>());
					trip_rec.setTrip(act->template Activity_Plan_ID<int>());
					if (new_destination < 0) trip_rec.setDestination(dest->template uuid<int>());
					else trip_rec.setDestination(new_destination);
					trip_rec.setDuration(act->template Duration<Time_Seconds>());
					//trip_rec.setEnd(act->template End_Time<Time_Seconds>());
					trip_rec.setEnd(move->template arrived_time<Time_Seconds>());
					trip_rec.setHhold(0);
					if (act->template Mode<Vehicle_Components::Types::Vehicle_Type_Keys>() == Vehicle_Components::Types::Vehicle_Type_Keys::SOV)
					{
						//trip_rec.setMode(Vehicle_Components::Types::Vehicle_Type_Keys::SOV);
						trip_rec.setVehicle(person->vehicle<vehicle_itf*>()->vehicle_ptr<shared_ptr<polaris::io::Vehicle>>());
					}
					trip_rec.setMode(act->template Mode<Vehicle_Components::Types::Vehicle_Type_Keys>()); //TODO - changed so that trip record in demand database uses the same mode codes as the activity table so it is consistent throughout code
					/*else if (act->template Mode<Vehicle_Components::Types::Vehicle_Type_Keys>() == Vehicle_Components::Types::Vehicle_Type_Keys::HOV) trip_rec.setMode(Vehicle_Components::Types::Vehicle_Type_Keys::HOV);
					else if (act->template Mode<Vehicle_Components::Types::Vehicle_Type_Keys>() == Vehicle_Components::Types::Vehicle_Type_Keys::BUS) trip_rec.setMode(Vehicle_Components::Types::Vehicle_Type_Keys::BUS);
					else trip_rec.setMode(3);*/
					if (new_origin < 0) trip_rec.setOrigin(orig->template uuid<int>());
					else trip_rec.setOrigin(new_origin);
					trip_rec.setPartition(move->template routed_travel_time<int>());
					trip_rec.setPassengers(0);
					trip_rec.setPurpose(0);
					//trip_rec.setStart(act->template Start_Time<Time_Seconds>());
					trip_rec.setStart(move->template departed_time<Time_Seconds>());
					trip_rec.setTour(0);
					trip_rec.setPriority(0);
					//				trip_rec.setVehicle(9);
					trip_rec.setType(1);
				}
				else
				{
					trip_rec.setType(-1); // use this to flag a null trip when writing to database
				}
					

				//==============================================================================================
				// create activity record
				//shared_ptr<polaris::io::Activity> act_rec(new polaris::io::Activity());
				//polaris::io::Activity* act_rec = new polaris::io::Activity();
				polaris::io::Activity act_rec;
				act_rec.setSeq_num(act->Activity_Plan_ID<int>());
				if (new_destination<0)
					act_rec.setLocation_Id(dest->template uuid<int>());
				else 
					act_rec.setLocation_Id(new_destination);
				act_rec.setStart_Time (act->template Start_Time<Time_Seconds>());
				act_rec.setDuration (act->template Duration<Time_Seconds>());
				if (act->template Mode<Vehicle_Components::Types::Vehicle_Type_Keys>() == Vehicle_Components::Types::Vehicle_Type_Keys::SOV)
					act_rec.setMode("SOV");
				else if (act->template Mode<Vehicle_Components::Types::Vehicle_Type_Keys>() == Vehicle_Components::Types::Vehicle_Type_Keys::HOV)
					act_rec.setMode("HOV");
				else if (act->template Mode<Vehicle_Components::Types::Vehicle_Type_Keys>() == Vehicle_Components::Types::Vehicle_Type_Keys::TAXI)
					act_rec.setMode("TAXI");
				else if (act->template Mode<Vehicle_Components::Types::Vehicle_Type_Keys>() == Vehicle_Components::Types::Vehicle_Type_Keys::WALK)
					act_rec.setMode("WALK");
				else if (act->template Mode<Vehicle_Components::Types::Vehicle_Type_Keys>() == Vehicle_Components::Types::Vehicle_Type_Keys::BICYCLE)
					act_rec.setMode("BIKE");
				else if (act->template Mode<Vehicle_Components::Types::Vehicle_Type_Keys>() == Vehicle_Components::Types::Vehicle_Type_Keys::SCHOOLBUS)
					act_rec.setMode("SCHOOLBUS");
				else if (act->template Mode<Vehicle_Components::Types::Vehicle_Type_Keys>() == Vehicle_Components::Types::Vehicle_Type_Keys::PARK_AND_RIDE)
					act_rec.setMode("PARK_AND_RIDE");
				else if (act->template Mode<Vehicle_Components::Types::Vehicle_Type_Keys>() == Vehicle_Components::Types::Vehicle_Type_Keys::BUS)
					act_rec.setMode("TRANSIT");
				else if (act->template Mode<Vehicle_Components::Types::Vehicle_Type_Keys>() == Vehicle_Components::Types::Vehicle_Type_Keys::TRUCK)
					act_rec.setMode("TRUCK");
				else
				{
					stringstream s;
					s << "FAIL: " << act->template Mode<Vehicle_Components::Types::Vehicle_Type_Keys>();
					act_rec.setMode(s.str());
				}
				act_rec.setType (act->Get_Type_String());
				//act_rec.setPerson (person->template person_record<shared_ptr<polaris::io::Person>>());
				act_rec.setPerson (person->template person_record<shared_ptr<polaris::io::Person>>());
				//act_rec.setTrip (trip_rec);	
				act_rec.setTrip (0/*trip_rec->getTrip_Id()*/);	
				
				if (new_origin < 0) act_rec.setOrigin_Id(orig->template uuid<int>());
				else act_rec.setOrigin_Id(new_origin);


				// add trip and activity to the buffer for the current thread
				//trip_records_buffer[__thread_id].push_back(trip_rec);
				activity_records_buffer[__thread_id].push_back(make_pair(trip_rec,act_rec));
			}

			template<typename TargetType> void set_multimodal_trajectory(std::string& summary_string)
			{
				/*typedef Scenario<typename MasterType::scenario_type> _Scenario_Interface;
				shared_ptr<odb::database> db_ptr = ((_Scenario_Interface*)_global_scenario)->template result_db_ptr<shared_ptr<odb::database>>();
				odb::transaction t(db_ptr->begin());*/
				
				polaris::io::multimodalSPLabels multimodal_labels_record;

				//shared_ptr<polaris::io::multimodalSPLabels> multimodal_labels_record(new polaris::io::multimodalSPLabels());

				std::istringstream ss(summary_string);
				std::string sub_string;

				std::getline(ss, sub_string, '\t');
				multimodal_labels_record.setOrigin_ID(stoi(sub_string));
				std::getline(ss, sub_string, '\t');
				multimodal_labels_record.setDestination_ID(stoi(sub_string));
				std::getline(ss, sub_string, '\t');
				multimodal_labels_record.setDeparture_Time(stoi(sub_string));
				std::getline(ss, sub_string, '\t');
				multimodal_labels_record.setSub_Mode(stoi(sub_string));
				std::getline(ss, sub_string, '\t');
				multimodal_labels_record.setArrival_Time(stod(sub_string));
				std::getline(ss, sub_string, '\t');
				multimodal_labels_record.setGen_Cost(stod(sub_string));
				std::getline(ss, sub_string, '\t');
				multimodal_labels_record.setDuration(stod(sub_string));
				std::getline(ss, sub_string, '\t');
				multimodal_labels_record.setWait_Count(stoi(sub_string));
				std::getline(ss, sub_string, '\t');
				multimodal_labels_record.setWait_Time(stod(sub_string));
				std::getline(ss, sub_string, '\t');
				multimodal_labels_record.setWalk_Time(stod(sub_string));
				std::getline(ss, sub_string, '\t');
				multimodal_labels_record.setBike_Time(stod(sub_string));
				std::getline(ss, sub_string, '\t');
				multimodal_labels_record.setIVTT(stod(sub_string));
				std::getline(ss, sub_string, '\t');
				multimodal_labels_record.setCar_Time(stod(sub_string));
				std::getline(ss, sub_string, '\t');
				multimodal_labels_record.setTransfer_Pen(stod(sub_string));
				std::getline(ss, sub_string, '\t');
				multimodal_labels_record.setEst_Cost(stod(sub_string));
				std::getline(ss, sub_string, '\t');
				multimodal_labels_record.setScan_Count(stoi(sub_string));
				std::getline(ss, sub_string, '\t');
				multimodal_labels_record.setaStar_Time(stod(sub_string));
				std::getline(ss, sub_string, '\t');
				multimodal_labels_record.setvisit_Time(stod(sub_string));
				std::getline(ss, sub_string, '\t');
				multimodal_labels_record.setSuccess_Status(sub_string);
				std::getline(ss, sub_string, '\n');
				multimodal_labels_record.setEuc_Dist_km(stod(sub_string));

				multimodalSPLabels_buffer[__thread_id].push_back(multimodal_labels_record);
				/*db_ptr->persist(multimodal_labels_record);
				t.commit();*/
			}
		};
	}
}
