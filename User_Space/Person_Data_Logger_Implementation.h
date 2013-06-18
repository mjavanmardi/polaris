#pragma once

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
			vector<string> output_data[_num_threads];
			vector<string> output_data_buffer[_num_threads];
			vector<string>* buff;
			vector<string>* current;
			member_data(ofstream, log, none, none);
			member_data(string, filename, none, none);

			member_component_and_feature_accessor(Logging_Interval, Value, Basic_Units::Prototypes::Time_Prototype,Basic_Units::Implementations::Time_Implementation<NT>);
			member_component_and_feature_accessor(Next_Logging_Time, Value, Basic_Units::Prototypes::Time_Prototype,Basic_Units::Implementations::Time_Implementation<NT>);

			feature_implementation void Initialize()
			{
				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type> _Scenario_Interface;
				_Scenario_Interface* scenario = (_Scenario_Interface*)_global_scenario;

				// don't do logging if not specified in scenario
				if (!scenario->template write_activity_output<bool>())
				{
					this->Logging_Interval<ComponentType,CallerType,Time_Minutes>(END);
					this->Next_Logging_Time<ComponentType,CallerType,Time_Minutes>(END);
					load_event(ComponentType,Logging_Conditional,Write_Data_To_File_Event,END,0,NULLTYPE);
					return;
				}


				this->Logging_Interval<ComponentType,CallerType,Time_Minutes>(15);
				this->Next_Logging_Time<ComponentType,CallerType,Time_Minutes>(15);
				Simulation_Timestep_Increment first_time = this->Next_Logging_Time<ComponentType,CallerType,Simulation_Timestep_Increment>();
				load_event(ComponentType,Logging_Conditional,Write_Data_To_File_Event,first_time,0,NULLTYPE);

				buff = output_data_buffer;
				current = output_data;

				stringstream filename("");
				filename << scenario->template output_dir_name<string>();
				filename << "activity_output.xls";

				this->_filename = filename.str();
				this->_log.open(this->_filename);
				this->_log << "PER_ID\tACT_ID\tACT_TYP\tSTART_min\tDUR_min\tDEST_ZONE\tMODE\tplanning_info\tTTIME_min\tEXECUTED";

				num_acts.resize(20,0);
			}

			feature_implementation void Add_Record(TargetType act_record, bool is_executed)
			{		
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
					s<<"\t"<<act->template Actual_Travel_Time<Time_Minutes>()<<"\t"<<1;
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
			}
		};
	}
}