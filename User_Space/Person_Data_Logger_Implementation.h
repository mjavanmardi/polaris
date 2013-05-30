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
				this->Logging_Interval<ComponentType,CallerType,Time_Hours>(1);
				this->Next_Logging_Time<ComponentType,CallerType,Time_Hours>(1);
				Simulation_Timestep_Increment first_time = this->Next_Logging_Time<ComponentType,CallerType,Simulation_Timestep_Increment>();
				load_event(ComponentType,Logging_Conditional,Write_Data_To_File_Event,first_time,0,NULLTYPE);

				buff = output_data_buffer;
				current = output_data;

				this->_filename = "activity_output.xls";
				this->_log.open(this->_filename);
				this->_log << "PER_ID\tACT_ID\tSTART_min\tDUR_min\tDEST_ZONE\tTTIME_min\tEXECUTED";
			}

			feature_implementation void Add_Record(TargetType act_record, bool is_executed)
			{		
				stringstream s;
				typedef Activity_Components::Prototypes::Activity_Planner<typename MasterType::activity_type> act_record_itf;
				typedef Activity_Location_Components::Prototypes::Activity_Location_Prototype<typename MasterType::activity_location_type> location_itf;
				typedef Zone_Components::Prototypes::Zone_Prototype<typename MasterType::zone_type> zone_itf;
				
				act_record_itf* act = (act_record_itf*)act_record;
				location_itf* loc = act->template Location<location_itf*>();
				zone_itf* zone = loc->template zone<zone_itf*>();

				s << act->template Parent_ID<int>() << "\t"<<act->template Activity_Plan_ID<int>() << "\t"<<act->template Start_Time<Time_Minutes>() << "\t"<<act->template Duration<Time_Minutes>() << "\t"<<zone->template uuid<int>();

				if (!is_executed)
				{
					s<<act->template Expected_Travel_Time<Time_Minutes>()<<"\t"<<0;
				}
				else
				{
					s<<act->template Actual_Travel_Time<Time_Minutes>()<<"\t"<<1;
				}
				buff[_thread_id].push_back(s.str());
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