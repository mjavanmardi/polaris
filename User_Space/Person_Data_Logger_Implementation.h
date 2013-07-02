#pragma once

#include "Io/Io.h"
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
			vector<int> executed_acts[_num_threads];
			vector<int> ttime_distribution[_num_threads];
			vector<string> output_data[_num_threads];
			vector<string> output_data_buffer[_num_threads];
			vector<string>* buff;
			vector<string>* current;
			member_data(ofstream, log, none, none);
			member_data(string, filename, none, none);
			member_data(ofstream, ttime_file, none,none);
			member_data(ofstream, executed_acts_file, none,none);
			member_data(ofstream, external_demand_output_file,none,none);
			member_pointer(unique_ptr<odb::database>, db_ptr, none,none);
			member_pointer(odb::transaction, t, none,none);

			member_component_and_feature_accessor(Logging_Interval, Value, Basic_Units::Prototypes::Time_Prototype,Basic_Units::Implementations::Time_Implementation<NT>);
			member_component_and_feature_accessor(Next_Logging_Time, Value, Basic_Units::Prototypes::Time_Prototype,Basic_Units::Implementations::Time_Implementation<NT>);

			feature_implementation void Initialize()
			{
				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type> _Scenario_Interface;
				_Scenario_Interface* scenario = (_Scenario_Interface*)_global_scenario;

				// don't do logging if not specified in scenario
				//if (!scenario->template write_activity_output<bool>())
				//{
				//	this->Logging_Interval<ComponentType,CallerType,Time_Minutes>(END);
				//	this->Next_Logging_Time<ComponentType,CallerType,Time_Minutes>(END);
				//	load_event(ComponentType,Logging_Conditional,Write_Data_To_File_Event,END,0,NULLTYPE);
				//	return;
				//}

				if (scenario->write_demand_to_database<bool>())
				{
					string name(scenario->template database_name<string&>());
					this->_db_ptr = new unique_ptr<odb::database>(open_sqlite_database (name));
					this->_t = new odb::transaction((*this->_db_ptr)->begin());
				}


				this->Logging_Interval<ComponentType,CallerType,Time_Minutes>(15);
				this->Next_Logging_Time<ComponentType,CallerType,Time_Minutes>(15);
				Simulation_Timestep_Increment first_time = this->Next_Logging_Time<ComponentType,CallerType,Simulation_Timestep_Increment>();
				load_event(ComponentType,Logging_Conditional,Write_Data_To_File_Event,first_time,0,NULLTYPE);

				buff = output_data_buffer;
				current = output_data;

				// Initialize log file
				stringstream filename("");
				filename << scenario->template output_dir_name<string>();
				filename << "activity_output.xls";
				this->_filename = filename.str();
				this->_log.open(this->_filename);
				this->_log << "PER_ID\tACT_ID\tACT_TYP\tSTART_min\tDUR_min\tDEST_ZONE\tMODE\tplanning_info\tTTIME_min\tEXECUTED";

				// Initialize data counter files
				stringstream filename_ttime("");
				filename_ttime << scenario->template output_dir_name<string>();
				filename_ttime << "ttime_distribution.csv";
				this->_ttime_file.open(filename_ttime.str());
				if (!this->_ttime_file.is_open()) THROW_EXCEPTION("ERROR: ttime distribution file could not be created.");
				this->_ttime_file <<"TIME,5,10,15,20,25,30,35,40";
				
				stringstream filename_acts("");
				filename_acts << scenario->template output_dir_name<string>();
				filename_acts << "executed_activities.csv";
				this->_executed_acts_file.open(filename_acts.str());
				if (!this->_executed_acts_file.is_open())THROW_EXCEPTION("ERROR: executed activity distribution file could not be created.");
				this->_executed_acts_file <<"TIME,0,1,2,3,4,5,6,7";

				// Initialize data count containers
				num_acts.resize(20,0);
				for (int i=0; i<_num_threads;++i)
				{
					ttime_distribution[i].resize(25,0); 
					executed_acts[i].resize(20,0);
				}

				stringstream filename_demand("");
				filename_demand << scenario->template output_dir_name<string>();
				filename_demand << "external_demand.csv";
				this->_external_demand_output_file.open(filename_demand.str());
				if (!this->_external_demand_output_file.is_open())THROW_EXCEPTION("ERROR: executed activity distribution file could not be created.");
				//this->_external_demand_output_file <<"TIME,0,1,2,3,4,5,6,7";
			}

			feature_implementation void Add_Record(TargetType act_record, bool is_executed)
			{		
				// update the summary information
				this->Add_Summary_Record<ComponentType,CallerType,TargetType>(act_record, is_executed);


				stringstream s;
				typedef Activity_Components::Prototypes::Activity_Planner<typename MasterType::activity_type> act_record_itf;
				typedef Activity_Location_Components::Prototypes::Activity_Location_Prototype<typename MasterType::activity_location_type> location_itf;
				typedef Zone_Components::Prototypes::Zone_Prototype<typename MasterType::zone_type> zone_itf;
				
				// don't do logging if not specified in scenario
				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type> _Scenario_Interface;
				_Scenario_Interface* scenario = (_Scenario_Interface*)_global_scenario;
				if (!scenario->template write_activity_output<bool>()) return;
				
				act_record_itf* act = (act_record_itf*)act_record;
				location_itf* loc = act->template Location<location_itf*>();

				// count the number of acts added
				if (!is_executed) num_acts[act->template Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>()] +=1;


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

				define_component_interface(movement_itf,typename act_record_itf::get_type_of(movement_plan),Movement_Plan_Components::Prototypes::Movement_Plan_Prototype,ComponentType);
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
				if (scenario->write_demand_to_database<bool>())
				{
					define_component_interface(movement_itf,typename act_record_itf::get_type_of(movement_plan),Movement_Plan_Components::Prototypes::Movement_Plan_Prototype,ComponentType);
					define_component_interface(planner_itf,typename act_record_itf::get_type_of(Parent_Planner),Prototypes::Person_Planner,ComponentType);
					define_component_interface(person_itf,typename planner_itf::get_type_of(Parent_Person),Prototypes::Person,ComponentType);
					movement_itf* move = act->template movement_plan<movement_itf*>();
					zone_itf* orig = move->template origin<location_itf*>()->template zone<zone_itf*>();
					zone_itf* dest = move->template destination<location_itf*>()->template zone<zone_itf*>();
					planner_itf* planner = act->template Parent_Planner<planner_itf*>();
					person_itf* person = planner->template Parent_Person<person_itf*>();
					zone_itf* home = person->Home_Location<zone_itf*>();
					int O, D, H;
					O = orig->template uuid<int>();
					D = dest->template uuid<int>();
					H = home->template uuid<int>();
					if(is_external_trip(O,D,H)) Push_To_Demand_Database<ComponentType,CallerType,TargetType>(act_record);
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

			declare_feature_conditional(Logging_Conditional)
			{
				typedef typename MasterType::person_data_logger_type this_type;
				typedef Prototypes::Person_Data_Logger<typename MasterType::person_data_logger_type> _Interface;
				_Interface* this_ptr=(_Interface*)_this;
				this_type* pthis = (this_type*)_this;
				
				// set next planning time for other functions to use
				this_ptr->template Next_Logging_Time<Simulation_Timestep_Increment>(_iteration + this_ptr->template Logging_Interval<Simulation_Timestep_Increment>());

				if(_sub_iteration == 0)
				{
					// swap buffer and current
					vector<string>* tmp = pthis->buff;
					pthis->buff = pthis->current;
					pthis->current = tmp;

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
				//for (int i = 0; i < 20; ++i)
				//{
				//	cout << endl << "Number of activity type : " << num_acts[i];
				//}

				int i = _sub_iteration;
				for (vector<string>::iterator itr = current[i].begin(); itr != current[i].end(); ++itr)
				{
					this->_log << '\n' << ": " << *itr;
				}
				current[i].clear();

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


				// database write for external trips
				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type> _Scenario_Interface;
				_Scenario_Interface* scenario = (_Scenario_Interface*)_global_scenario;
				if (scenario->write_demand_to_database<bool>())
				{
					this->_t->commit();
					this->_t->reset((*this->_db_ptr)->begin());
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

			bool is_external_trip(int O, int D, int H)
			{
				//TODO: remove when done
				return true;

				bool o_is_internal = is_internal(O);
				bool d_is_internal = is_internal(D);
				bool h_is_internal = is_internal(H);

				// not external trip - lives in CBD
				if (h_is_internal) return false;

				if (o_is_internal && !d_is_internal || !o_is_internal && d_is_internal) return true;
				else return false;
			}
			bool is_internal(int id)
			{
				bool is_internal = false;
				if (id <= 88 || (id >= 166 && id <= 168) || (id >= 193 && id <= 196)) is_internal = true; 
				return is_internal;
			}



			feature_implementation void Push_To_Demand_Database(TargetType act_record)
			{
				typedef Activity_Components::Prototypes::Activity_Planner<typename MasterType::activity_type> act_itf;
				typedef Activity_Location_Components::Prototypes::Activity_Location_Prototype<typename MasterType::activity_location_type> location_itf;
				typedef Zone_Components::Prototypes::Zone_Prototype<typename MasterType::zone_type> zone_itf;
				define_component_interface(movement_itf,typename act_itf::get_type_of(movement_plan),Movement_Plan_Components::Prototypes::Movement_Plan_Prototype,ComponentType);
				define_component_interface(planner_itf,typename act_itf::get_type_of(Parent_Planner),Prototypes::Person_Planner,ComponentType);
				define_component_interface(person_itf,typename planner_itf::get_type_of(Parent_Person),Prototypes::Person,ComponentType);

				act_itf* act = (act_itf*)act_record;
				movement_itf* move = act->template movement_plan<movement_itf*>();
				location_itf* orig = move->template origin<location_itf*>();
				location_itf* dest = move->template destination<location_itf*>();
				planner_itf* planner = act->template Parent_Planner<planner_itf*>();
				person_itf* person = planner->template Parent_Person<person_itf*>();		

				// Do Stuff to write trip to demand database....
				
				shared_ptr<polaris::io::Trip> trip_rec(new polaris::io::Trip());
				trip_rec->setConstraint(0);
				trip_rec->setDestination(dest->template uuid<int>());
				trip_rec->setDuration(act->Duration<Time_Seconds>());
				trip_rec->setEnd(act->End_Time<Time_Seconds>());
				trip_rec->setHhold(person->uuid<int>());
				trip_rec->setMode(0);
				trip_rec->setOrigin(orig->uuid<int>());
				trip_rec->setPartition(0);
				trip_rec->setPassengers(0);
				trip_rec->setPurpose(0);
				trip_rec->setStart(act->Start_Time<Time_Seconds>());
				trip_rec->setTour(0);

				(*this->_db_ptr)->persist(trip_rec);
				//this->_external_demand_output_file <<person->uuid<int>() <<","<< orig->uuid<int>() << ","<<dest->uuid<int>()<<","<<act->Start_Time<Time_Seconds>()<<","<<act->End_Time<Time_Seconds>() <<endl;
			}
		};
	}
}