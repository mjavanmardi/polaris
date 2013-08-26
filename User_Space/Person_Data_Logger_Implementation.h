#pragma once

//#include "Io/Io.h"
#include "Person_Data_Logger_Prototype.h"
#include "Activity_Implementations.h"

namespace Person_Components
{
	namespace Implementations
	{
		//==================================================================================
		/// Planning classes
		//----------------------------------------------------------------------------------
		implementation struct Person_Data_Logger_Implementation : public Polaris_Component<APPEND_CHILD(Person_Data_Logger_Implementation),MasterType,Execution_Object,ParentType>
		{
			vector<int> num_acts;
			vector<int> planned_acts[_num_threads];
			vector<int> executed_acts[_num_threads];
			vector<int> ttime_distribution[_num_threads];
			vector<string> output_data[_num_threads];
			vector<string> output_data_buffer[_num_threads];
			vector<shared_ptr<polaris::io::Trip>> trip_records[_num_threads];
			vector<shared_ptr<polaris::io::Trip>> trip_records_buffer[_num_threads];
			vector<shared_ptr<polaris::io::Activity>> activity_records[_num_threads];
			vector<shared_ptr<polaris::io::Activity>> activity_records_buffer[_num_threads];

			float expected_ttime[_num_threads];
			float routed_ttime[_num_threads];
			float actual_ttime[_num_threads];
			int num_acts_in_interval[_num_threads];

			vector<string>* buff;
			vector<string>* current;
			vector<shared_ptr<polaris::io::Trip>>* trip_buff;
			vector<shared_ptr<polaris::io::Trip>>* trip_current;
			vector<shared_ptr<polaris::io::Activity>>* activity_buff;
			vector<shared_ptr<polaris::io::Activity>>* activity_current;

			member_data(ofstream, log, none, none);
			member_data(string, filename, none, none);
			member_data(ofstream, ttime_file, none,none);
			member_data(ofstream, executed_acts_file, none,none);
			member_data(ofstream, external_demand_output_file,none,none);
			member_data(ofstream, demand_moe_file,none,none);
			member_data(shared_ptr<odb::database>, db_ptr, none,none);

			// GRAPHICAL DATA MEMBERS
			member_data(float, activity_time_lost,none,none);
			member_data(float, cancelled_activities,none,none);
			member_data(float, replanned_activities,none,none);

			member_component_and_feature_accessor(Logging_Interval, Value, Basic_Units::Prototypes::Time_Prototype,Basic_Units::Implementations::Time_Implementation<NT>);
			member_component_and_feature_accessor(Next_Logging_Time, Value, Basic_Units::Prototypes::Time_Prototype,Basic_Units::Implementations::Time_Implementation<NT>);

			feature_implementation void Initialize()
			{
				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type> _Scenario_Interface;
				_Scenario_Interface* scenario = (_Scenario_Interface*)_global_scenario;
				this->_cancelled_activities = 0;
				this->_activity_time_lost = 0;


				if (scenario->template write_demand_to_database<bool>())
				{
					string name(scenario->template output_demand_database_name<string&>());
					this->_db_ptr = open_sqlite_database_single<shared_ptr<odb::database> > (name);
					odb::transaction t(this->_db_ptr->begin());
					this->_db_ptr->execute("delete from trip");
					t.commit();
				}


				this->Logging_Interval<ComponentType,CallerType,Time_Minutes>(5);
				this->Next_Logging_Time<ComponentType,CallerType,Time_Minutes>(5);
				Simulation_Timestep_Increment first_time = this->Next_Logging_Time<ComponentType,CallerType,Simulation_Timestep_Increment>();
				load_event(ComponentType,Logging_Conditional,Write_Data_To_File_Event,first_time,0,NULLTYPE);


				// Initialize pointers to data buffers
				buff = output_data_buffer;
				current = output_data;

				trip_buff = trip_records_buffer;
				trip_current = trip_records;

				activity_buff = activity_records_buffer;
				activity_current = activity_records;

				// Initialize log file
				stringstream filename("");
				filename << scenario->template output_dir_name<string>();
				filename << "activity_output.xls";
				this->_filename = filename.str();
				this->_log.open(this->_filename);
				this->_log << "PER_ID\tACT_ID\tACT_TYP\tSTART_min\tDUR_min\tDEST_ZONE\tMODE\tplanning_info\tTTIME_min\tEXECUTED"<<endl;

				// Initialize data counter files
				stringstream filename_ttime("");
				filename_ttime << scenario->template output_dir_name<string>();
				filename_ttime << "ttime_distribution.csv";
				this->_ttime_file.open(filename_ttime.str());
				if (!this->_ttime_file.is_open()) THROW_EXCEPTION("ERROR: ttime distribution file could not be created.");
				this->_ttime_file <<"TIME(s),5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,105,110,115,120,120+"<<endl;
				
				// Initialize executed activities file
				stringstream filename_acts("");
				filename_acts << scenario->template output_dir_name<string>();
				filename_acts << "executed_activities.csv";
				this->_executed_acts_file.open(filename_acts.str());
				if (!this->_executed_acts_file.is_open())THROW_EXCEPTION("ERROR: executed activity distribution file could not be created.");
				this->_executed_acts_file <<"TIME(s),0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18"<<endl;

				// Initialize data count containers
				num_acts.resize(20,0);
				for (int i=0; i<_num_threads;++i)
				{
					ttime_distribution[i].resize(25,0); 
					executed_acts[i].resize(20,0);
					planned_acts[i].resize(20,0);
				}

				// Initialize demand MOE file
				stringstream filename_moe("");
				filename_moe << scenario->template output_dir_name<string>();
				filename_moe << "moe_demand.csv";
				this->_demand_moe_file.open(filename_moe.str());
				if (!this->_demand_moe_file.is_open())THROW_EXCEPTION("ERROR: demand moe file could not be created.");
				this->_demand_moe_file <<"TIME(s),LOST_ACTIVITY_TIME(min),Cancelled_Acts_Count,acts_completed,avg_expected_ttime,avg_routed_ttime,avg_actual_ttime"<<endl;
			}

			feature_implementation void Add_Record(TargetType act_record, bool is_executed)
			{		
				// update the summary information
				this->Add_Summary_Record<ComponentType,CallerType,TargetType>(act_record, is_executed);


				stringstream s;
				typedef Activity_Components::Prototypes::Activity_Planner<typename MasterType::activity_type> act_record_itf;
				typedef Movement_Plan_Components::Prototypes::Movement_Plan_Prototype<typename MasterType::movement_plan_type> movement_itf;
				typedef Activity_Location_Components::Prototypes::Activity_Location_Prototype<typename MasterType::activity_location_type> location_itf;
				typedef Zone_Components::Prototypes::Zone_Prototype<typename MasterType::zone_type> zone_itf;
				
				// don't do logging if not specified in scenario
				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type> _Scenario_Interface;
				_Scenario_Interface* scenario = (_Scenario_Interface*)_global_scenario;
				
				
				act_record_itf* act = (act_record_itf*)act_record;
				location_itf* loc = act->template Location<location_itf*>();

				// determine delay, using current iteration as the arrival time
				if (is_executed)
				{
					float expected_start = act->template Start_Time<Time_Minutes>();
					float actual_start = GLOBALS::Simulation_Time.template Current_Time<Time_Minutes>();
					float delay = std::max<float>(actual_start - expected_start,0.0f);
					this->_activity_time_lost += delay;

					expected_ttime[_thread_id] += (float)act->template Expected_Travel_Time<Time_Seconds>();
					actual_ttime[_thread_id] += (float)act->template Actual_Travel_Time<Time_Seconds>();
					routed_ttime[_thread_id] += (float)act->template movement_plan<movement_itf*>()->template routed_travel_time<float>();
					num_acts_in_interval[_thread_id]++;
				}


				// count the number of acts added
				if (!is_executed) planned_acts[_thread_id][act->template Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>()] +=1;


				// exit if not writing output
				if (!scenario->template write_activity_output<bool>()) return;


				


				if (loc == nullptr)
				{
					s << act->template Parent_ID<int>() << "\t"<<act->template Activity_Plan_ID<int>()<<"\t" << act->template Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>();
					//THROW_WARNING("Warning, null location pointer for activity record: " << act->template Parent_ID<int>() << "." << act->template Activity_Plan_ID<int>()); 
					s<<"\tNull location pointer - act not planned?\t";
					Revision& location = act->template Location_Planning_Time<Revision&>();
					Revision& start = act->template Start_Time_Planning_Time<Revision&>();
					Revision& route = act->template Route_Planning_Time<Revision&>();
					s << "\tPlan times (loc,start,route): "<<location._iteration<<"."<<location._sub_iteration<<" , " << start._iteration <<"."<<start._sub_iteration<<" , " << route._iteration<<"."<<route._sub_iteration;
					buff[_thread_id].push_back(s.str());
					return;
				}
				zone_itf* zone = loc->template zone<zone_itf*>();

				Revision& location = act->template Stored_Location_Planning_Time<Revision&>();
				Revision& start = act->template Stored_Start_Time_Planning_Time<Revision&>();
				Revision& route = act->template Stored_Route_Planning_Time<Revision&>();
				s << act->template Parent_ID<int>() << "\t"<<act->template Activity_Plan_ID<int>()<<"\t" << act->template Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>() << "\t"<<act->template Start_Time<Time_Minutes>() << "\t"<<act->template Duration<Time_Minutes>() << "\t"<<zone->template uuid<int>()<<"\t"<<act->template Mode<Vehicle_Components::Types::Vehicle_Type_Keys>();
				s << "\tPlan times (loc,start,route): "<<location._iteration<<"."<<location._sub_iteration<<" , " << start._iteration <<"."<<start._sub_iteration<<" , " << route._iteration<<"."<<route._sub_iteration;
					
				if (!is_executed)
				{
					s<<"\t"<<act->template Expected_Travel_Time<Time_Minutes>()<<"\t"<<0;
				}
				else
				{
					Time_Minutes ttime = act->template Actual_Travel_Time<Time_Minutes>();
					s<<"\t"<<ttime<<"\t"<<1;
				}
				buff[_thread_id].push_back(s.str());

				// Movement plans

				//define_component_interface(movement_itf,typename act_record_itf::get_type_of(movement_plan),Movement_Plan_Components::Prototypes::Movement_Plan_Prototype,ComponentType);
				movement_itf* move = act->template movement_plan<movement_itf*>();

				stringstream m;
				m << "MOVEMENT(Parent,Actid,orig,dest,departure_time,start_time,end_time,mode:" << act->template Parent_ID<int>() << "\t"<<act->template Activity_Plan_ID<int>()<<"\t";
				m <<move->template origin<location_itf*>()->template uuid<int>()<< "\t"<<move->template destination<location_itf*>()->template uuid<int>()<< "\t";
				m <<move->template departed_time<Time_Seconds>()<< "\t" << act->template Start_Time<Time_Seconds>()<< "\t" << act->template End_Time<Time_Seconds>()<< "\t" << act->template Mode<int>();
				buff[_thread_id].push_back(m.str());
			}
			feature_implementation void Add_Summary_Record(TargetType act_record, bool is_executed)
			{		
				if (!is_executed) return;

				stringstream s;
				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type> _Scenario_Interface;
				_Scenario_Interface* scenario = (_Scenario_Interface*)_global_scenario;
				typedef Activity_Components::Prototypes::Activity_Planner<typename MasterType::activity_type> act_record_itf;
				typedef Activity_Location_Components::Prototypes::Activity_Location_Prototype<typename MasterType::activity_location_type> location_itf;
				typedef Zone_Components::Prototypes::Zone_Prototype<typename MasterType::zone_type> zone_itf;
				
				
				act_record_itf* act = (act_record_itf*)act_record;
				location_itf* loc = act->template Location<location_itf*>();

				//==========================================================
				//----------------------------------------------------------
				if (scenario->template write_demand_to_database<bool>())
				{
					define_component_interface(movement_itf,typename act_record_itf::get_type_of(movement_plan),Movement_Plan_Components::Prototypes::Movement_Plan_Prototype,ComponentType);
					define_component_interface(planner_itf,typename act_record_itf::get_type_of(Parent_Planner),Prototypes::Person_Planner,ComponentType);
					define_component_interface(person_itf,typename planner_itf::get_type_of(Parent_Person),Prototypes::Person,ComponentType);

					Push_To_Demand_Database<ComponentType,CallerType,TargetType>(act_record);

					//movement_itf* move = act->template movement_plan<movement_itf*>();
					//zone_itf* orig = move->template origin<location_itf*>()->template zone<zone_itf*>();
					//zone_itf* dest = move->template destination<location_itf*>()->template zone<zone_itf*>();
					//planner_itf* planner = act->template Parent_Planner<planner_itf*>();
					//person_itf* person = planner->template Parent_Person<person_itf*>();
					//zone_itf* home = person->template Home_Location<zone_itf*>();
					//int O, D, H;
					//O = orig->template uuid<int>();
					//D = dest->template uuid<int>();
					//H = home->template uuid<int>();
					//if(is_external_internal_trip(O,D,H)) 
					//{
					//	int new_origin = get_nearest_external_location<ComponentType,CallerType,zone_itf*>(orig);
					//	Push_To_Demand_Database<ComponentType,CallerType,TargetType>(act_record, new_origin);
					//}
					//else if (is_internal_external_trip(O,D,H))
					//{
					//	int new_destination = get_nearest_external_location<ComponentType,CallerType,zone_itf*>(dest);
					//	Push_To_Demand_Database<ComponentType,CallerType,TargetType>(act_record, -1, new_destination);
					//}
				}
				//----------------------------------------------------------
				//==========================================================


				executed_acts[_thread_id][act->template Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>()] +=1;
			
				// update travel time distributions
				Time_Minutes ttime = act->template Actual_Travel_Time<Time_Minutes>();
				if (ttime < 5) ttime_distribution[_thread_id][0]++;
				else if (ttime < 10) ttime_distribution[_thread_id][1]++;
				else if (ttime < 15) ttime_distribution[_thread_id][2]++;
				else if (ttime < 20) ttime_distribution[_thread_id][3]++;
				else if (ttime < 25) ttime_distribution[_thread_id][4]++;
				else if (ttime < 30) ttime_distribution[_thread_id][5]++;
				else if (ttime < 35) ttime_distribution[_thread_id][6]++;
				else if (ttime < 40) ttime_distribution[_thread_id][7]++;
				else if (ttime < 45) ttime_distribution[_thread_id][8]++;
				else if (ttime < 50) ttime_distribution[_thread_id][9]++;
				else if (ttime < 55) ttime_distribution[_thread_id][10]++;
				else if (ttime < 60) ttime_distribution[_thread_id][11]++;
				else if (ttime < 65) ttime_distribution[_thread_id][12]++;
				else if (ttime < 70) ttime_distribution[_thread_id][13]++;
				else if (ttime < 75) ttime_distribution[_thread_id][14]++;
				else if (ttime < 80) ttime_distribution[_thread_id][15]++;
				else if (ttime < 85) ttime_distribution[_thread_id][16]++;
				else if (ttime < 90) ttime_distribution[_thread_id][17]++;
				else if (ttime < 95) ttime_distribution[_thread_id][18]++;
				else if (ttime < 100) ttime_distribution[_thread_id][19]++;
				else if (ttime < 105) ttime_distribution[_thread_id][20]++;
				else if (ttime < 110) ttime_distribution[_thread_id][21]++;
				else if (ttime < 115) ttime_distribution[_thread_id][22]++;
				else if (ttime < 120) ttime_distribution[_thread_id][23]++;
				else  ttime_distribution[_thread_id][24]++;
			}
			feature_implementation void Increment_Cancelled_Activities()
			{
				this->_cancelled_activities++;
			}
			feature_implementation void Increment_Replanned_Activities()
			{
				this->_replanned_activities++;
			}

			declare_feature_conditional(Logging_Conditional)
			{
				typedef typename MasterType::person_data_logger_type this_type;
				typedef Prototypes::Person_Data_Logger<typename MasterType::person_data_logger_type> _Interface;
				_Interface* this_ptr=(_Interface*)_this;
				this_type* pthis = (this_type*)_this;
				
				// set next planning time for other functions to use
				this_ptr->template Next_Logging_Time<Simulation_Timestep_Increment>(_iteration + this_ptr->template Logging_Interval<Simulation_Timestep_Increment>());

				// swap buffer and current for output strings and trip records
				if(_sub_iteration == 0)
				{				
					vector<string>* tmp = pthis->buff;
					pthis->buff = pthis->current;
					pthis->current = tmp;

					vector<shared_ptr<polaris::io::Trip>>* tmp_trip = pthis->trip_buff;
					pthis->trip_buff = pthis->trip_current;
					pthis->trip_current = tmp_trip;

					vector<shared_ptr<polaris::io::Activity>>* tmp_act = pthis->activity_buff;
					pthis->activity_buff = pthis->activity_current;
					pthis->activity_current = tmp_act;

					response.next._iteration = _iteration;
					response.next._sub_iteration = 1;
					response.result = true;
				}
				else if (_sub_iteration < _num_threads)
				{
					response.next._iteration = _iteration;
					response.next._sub_iteration = _sub_iteration+1;
					response.result = true;
				}
				else
				{
					response.next._iteration = this_ptr->template Next_Logging_Time<Simulation_Timestep_Increment>();
					response.next._sub_iteration = 0;
					response.result = false;
				}
			}
			declare_feature_event(Write_Data_To_File_Event)
			{
				typedef Prototypes::Person_Data_Logger<typename MasterType::person_data_logger_type> _Interface;
				_Interface* _this_ptr=(_Interface*)_this;
				_this_ptr->template Write_Data_To_File<NT>();
			}
			feature_implementation void Write_Data_To_File()
			{
				int i = _sub_iteration;

				// write out strings in the current buffer to log file and clear it
				for (vector<string>::iterator itr = current[i].begin(); itr != current[i].end(); ++itr)
				{
					this->_log << '\n' << ": " << *itr;
				}
				current[i].clear();

				// do aggregated file writing on first subiteration
				if (i == 0)
				{
					// display the ttime and executed activity count distributions once
					this->_ttime_file << _iteration;
					for (int j=0; j < 25; j++)
					{
						int count = 0;
						for (int k=0; k < _num_threads; k++) // collect value over all threads
						{
							count += ttime_distribution[k][j];
						}
						this->_ttime_file <<","<< count;
					}
					this->_ttime_file << endl;

					this->_executed_acts_file << _iteration;
					for (int j=0; j < 20; j++)
					{
						int count = 0;
						for (int k=0; k < _num_threads; k++) // collect value over all threads
						{
							count += executed_acts[k][j];
						}
						this->_executed_acts_file <<","<< count;
					}
					this->_executed_acts_file << endl;


					//=================================================================================
					// write to the Demand MOE File
					this->_demand_moe_file << _iteration << "," << this->_activity_time_lost;

					// write the cancelled activities 
					this->_demand_moe_file << "," << this->_cancelled_activities;

					// write the average expected and actual travel times over the current interval
					float actual_ttime_sum=0;
					float routed_ttime_sum=0;
					float expected_ttime_sum=0;
					float acts_in_interval = 0;
					for (int i=0; i < _num_threads; i++)
					{
						actual_ttime_sum += actual_ttime[i];
						expected_ttime_sum += expected_ttime[i];
						routed_ttime_sum += routed_ttime[i];
						acts_in_interval += num_acts_in_interval[i];
						actual_ttime[i]=0;
						expected_ttime[i]=0;
						routed_ttime[i]=0;
						num_acts_in_interval[i]=0;
					}
					this->_demand_moe_file << "," << acts_in_interval;
					this->_demand_moe_file << "," << expected_ttime_sum / acts_in_interval;
					this->_demand_moe_file << "," << routed_ttime_sum / acts_in_interval;
					this->_demand_moe_file << "," << actual_ttime_sum / acts_in_interval<<endl;
				}


				// database write for external trips
				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type> _Scenario_Interface;
				_Scenario_Interface* scenario = (_Scenario_Interface*)_global_scenario;
				if (scenario->template write_demand_to_database<bool>())
				{
					try
					{
					odb::transaction t(this->_db_ptr->begin());
					for (vector<shared_ptr<polaris::io::Trip>>::iterator itr = trip_current[i].begin(); itr != trip_current[i].end(); ++itr)
					{
						this->_db_ptr->persist(*itr);
					}
					for (vector<shared_ptr<polaris::io::Activity>>::iterator itr = activity_current[i].begin(); itr != activity_current[i].end(); ++itr)
					{
						this->_db_ptr->persist(*itr);
					}
					t.commit();
					}
					catch (odb::sqlite::database_exception ex)
					{
						cout << ex.message()<<endl;
					}

					trip_current[i].clear();
					activity_current[i].clear();
				}
			}
			feature_implementation void Write_Summary_Data_To_File()
			{
				//for (int i = 0; i < 20; ++i)
				//{
				//	cout << endl << "Number of activity type : " << num_acts[i];
				//}

				int i = _sub_iteration;

				// display the ttime and executed activity count distributions once
				if (i == 0)
				{
					this->_ttime_file << _iteration;
					for (int j=0; j < 25; j++)
					{
						int count = 0;
						for (int k=0; k < _num_threads; k++) // collect value over all threads
						{
							count += ttime_distribution[k][j];
						}
						this->_ttime_file <<","<< count;
					}
					this->_ttime_file << endl;

					this->_executed_acts_file << _iteration;
					for (int j=0; j < 20; j++)
					{
						int count = 0;
						for (int k=0; k < _num_threads; k++) // collect value over all threads
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
			feature_implementation int get_nearest_external_location(TargetType zone)
			{
				typedef Network_Components::Prototypes::Network_Prototype<typename MasterType::network_type> _Network_Interface;
				typedef Activity_Location_Components::Prototypes::Activity_Location_Prototype<typename MasterType::activity_location_type> location_itf;
				define_container_and_value_interface(zones_container_itf, zone_itf,typename _Network_Interface::get_type_of(zones_container), Containers::Associative_Container_Prototype,Zone_Components::Prototypes::Zone_Prototype,ComponentType);
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



			feature_implementation void Push_To_Demand_Database(TargetType act_record, int new_origin=-1, int new_destination=-1)
			{
				typedef Activity_Components::Prototypes::Activity_Planner<typename MasterType::activity_type> act_itf;
				typedef Activity_Location_Components::Prototypes::Activity_Location_Prototype<typename MasterType::activity_location_type> location_itf;
				typedef Zone_Components::Prototypes::Zone_Prototype<typename MasterType::zone_type> zone_itf;
				define_component_interface(movement_itf,typename act_itf::get_type_of(movement_plan),Movement_Plan_Components::Prototypes::Movement_Plan_Prototype,ComponentType);
				define_component_interface(planner_itf,typename act_itf::get_type_of(Parent_Planner),Prototypes::Person_Planner,ComponentType);
				define_component_interface(person_itf,typename planner_itf::get_type_of(Parent_Person),Prototypes::Person,ComponentType);
				define_component_interface(household_itf,typename person_itf::get_type_of(Household),Household_Components::Prototypes::Household,ComponentType);

				act_itf* act = (act_itf*)act_record;
				movement_itf* move = act->template movement_plan<movement_itf*>();
				location_itf* orig = move->template origin<location_itf*>();
				location_itf* dest = move->template destination<location_itf*>();
				planner_itf* planner = act->template Parent_Planner<planner_itf*>();
				person_itf* person = planner->template Parent_Person<person_itf*>();		
				household_itf* hh = person->template Household<household_itf*>();

			

				//==============================================================================================
				// write trips, only if auto mode used
				shared_ptr<polaris::io::Trip> trip_rec(new polaris::io::Trip());
				if (act->template Mode<Vehicle_Components::Types::Vehicle_Type_Keys>() == Vehicle_Components::Types::Vehicle_Type_Keys::SOV)
				{				
					trip_rec->setConstraint(0);
					trip_rec->setPerson(person->template uuid<int>());
					trip_rec->setTrip(act->template Activity_Plan_ID<int>());
					if (new_destination<0) trip_rec->setDestination(dest->template uuid<int>());
					else trip_rec->setDestination(new_destination);
					trip_rec->setDuration(act->template Duration<Time_Seconds>());
					//trip_rec->setEnd(act->template End_Time<Time_Seconds>());
					trip_rec->setEnd(move->template arrived_time<Time_Seconds>());
					trip_rec->setHhold(hh->template uuid<int>());
					trip_rec->setMode(0);
					if (new_origin <0) trip_rec->setOrigin(orig->template uuid<int>());
					else trip_rec->setOrigin(new_origin);
					trip_rec->setPartition(0);
					trip_rec->setPassengers(0);
					trip_rec->setPurpose(0);
					//trip_rec->setStart(act->template Start_Time<Time_Seconds>());
					trip_rec->setStart(move->template departed_time<Time_Seconds>());
					trip_rec->setTour(0);
					trip_rec->setPriority(0);
					trip_rec->setVehicle(1);
					trip_rec->setType(0);
					// add trip to the buffer for the current thread
					trip_buff[_thread_id].push_back(trip_rec);
				}


				//==============================================================================================
				// write activity to database
				shared_ptr<polaris::io::Activity> act_rec(new polaris::io::Activity());
				if (new_destination<0)
					act_rec->setLocation_Id(dest->uuid<int>());
				else 
					act_rec->setLocation_Id(new_destination);
				act_rec->setStart_Time (act->template Start_Time<Time_Seconds>());
				act_rec->setDuration (act->template Duration<Time_Seconds>());
				if (act->template Mode<Vehicle_Components::Types::Vehicle_Type_Keys>() == Vehicle_Components::Types::Vehicle_Type_Keys::SOV)
					act_rec->setMode ("AUTO");
				else
					act_rec->setMode ("TRANSIT");
				act_rec->setType (act->Get_Type_String<NT>());
				act_rec->setPerson (person->template person_record<shared_ptr<polaris::io::Person>>());
				if (act->template Mode<Vehicle_Components::Types::Vehicle_Type_Keys>() == Vehicle_Components::Types::Vehicle_Type_Keys::SOV) act_rec->setTrip (trip_rec);			
				activity_buff[_thread_id].push_back(act_rec);

			}
		};
	}
}