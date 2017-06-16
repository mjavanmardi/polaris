#pragma once


#include "Vehicle_Data_Logger_Prototype.h"


namespace Vehicle_Components
{
	namespace Implementations
	{
		//==================================================================================
		/// Planning classes
		//----------------------------------------------------------------------------------
		implementation struct Vehicle_Data_Logger_Implementation : public Polaris_Component<MasterType,INHERIT(Vehicle_Data_Logger_Implementation),Execution_Object>
		{
			std::vector<float>* output_data;
			std::vector<float>* output_data_buffer;

			//std::vector<char>* output_color_data;
			//std::vector<char>* output_color_data_buffer;

			std::vector<float>* buff;
			std::vector<float>* current;

			//std::vector<char>* color_buff;
			//std::vector<char>* color_current;

			dense_hash_map<int,long long> index;

			m_data(bool, Read, NONE, NONE);
			m_data(string, filename, NONE, NONE);
			m_data(long long, bytes_written, NONE,NONE);
			m_data(File_IO::Binary_File_Writer, outfile, NONE, NONE);
			m_data(File_IO::Binary_File_Reader, infile, NONE, NONE);

			m_data(File_IO::File_Writer, out_index_file, NONE, NONE);
			m_data(File_IO::File_Reader, in_index_file, NONE, NONE);
			m_data(typename MasterType::visual_vehicle_type*, vehicle_layer, NONE, NONE);

			member_component_and_feature_accessor(Logging_Interval, Value, Basic_Units::Prototypes::Time,Basic_Units::Implementations::Time_Implementation<NT>);
			member_component_and_feature_accessor(Next_Logging_Time, Value, Basic_Units::Prototypes::Time,Basic_Units::Implementations::Time_Implementation<NT>);

			template<typename TargetType> void Initialize(bool read, string file)
			{
				typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;
				_Scenario_Interface* scenario = (_Scenario_Interface*)_global_scenario;

				// initialize storage arrays
				output_data = new std::vector<float>[num_sim_threads()];
				output_data_buffer = new std::vector<float>[num_sim_threads()];

				this->_Read = read;
				this->_bytes_written = 0;

				this->Logging_Interval<Simulation_Timestep_Increment>(1);
				this->Next_Logging_Time<Simulation_Timestep_Increment>(360);
				int first_time = this->Next_Logging_Time<Simulation_Timestep_Increment>();
		

				// Initialize pointers to data buffers
				buff = output_data_buffer;
				current = output_data;

				this->index.set_empty_key(-1);
				this->index.set_deleted_key(-2);

				// Initialize log file
				if (read)
				{
					// open file
					this->_filename = file;
					if (!this->_infile.Open(this->_filename)) THROW_EXCEPTION("ERRROR, could not open input snapshot file: "<<file);
					int logging_time=0;
					this->_infile.Read_Value(logging_time);
					this->Next_Logging_Time<Simulation_Timestep_Increment>(logging_time);
					this->template Load_Event<ComponentType>(&Reading_Event_Controller,logging_time,0);

					// open input index and read in
					stringstream idx_name("");
					idx_name << this->_filename.substr(0,_filename.size()-4);
					idx_name << ".idx";
					if (!this->_in_index_file.Open(idx_name.str(),false)) THROW_EXCEPTION("ERRROR, could not open input snapshot file index: "<<idx_name.str());
					dense_hash_map<int,long long>::iterator idx_itr;
					while (this->_in_index_file.Read())
					{
						int time;
						long long bytes;
						this->_in_index_file.template Get_Data<int>(time,0);
						this->_in_index_file.Get_Data<long long>(bytes,1);
						if ((idx_itr = this->index.find(time)) == this->index.end())
						{
							this->index.insert(pair<int,long long>(time,bytes));
						}
					}
				}
				else
				{
					// open output file
					stringstream filename("");
					filename << scenario->template output_dir_name<string>();
					filename << file;
					this->_filename = filename.str();

					this->_outfile.Open(this->_filename);
					this->_outfile.Write_Value(first_time);
					this->_bytes_written += sizeof(int);
					
					this->template Load_Event<ComponentType>(&Logging_Event_Controller,first_time,0);

					// open output index
					stringstream idx_name("");
					idx_name << this->_filename.substr(0,_filename.size()-4);
					idx_name << ".idx";
					this->_out_index_file.Open(idx_name.str());
				}

				Initialize_Layer<vehicle_layer_type>(first_time);
			}

			// Initialize the associated visual vehicle layer if it is defined, otherwise do nothing
			template<typename LayerType> void Initialize_Layer(int iter, requires(LayerType,!check_2(strip_modifiers(LayerType),NT,is_same)))
			{
				typedef typename MasterType::visual_vehicle_type vehicle_interface;
				_vehicle_layer = Allocate<typename MasterType::visual_vehicle_type>();
				_vehicle_layer->template Initialize<NT>(iter);
			}
			template<typename LayerType> void Initialize_Layer(int iter, requires(LayerType,check_2(strip_modifiers(LayerType),NT,is_same)))
			{
			}

			template<typename TargetType> static void Initialize_Type(TargetType time_panel)
			{
				typedef Time_Panel<typename MasterType::time_panel_type> time_panel_itf;
				time_panel_itf* tp = (time_panel_itf*)time_panel;
				tp->reschedule_callback<reschedule_callback_type>(&Reschedule_Logging);
			}

			void Add_Record(Point_3D<MasterType>& point, True_Color_RGBA<NT>& color)
			{		
				buff[__thread_id].push_back(point._x);
				buff[__thread_id].push_back(point._y);
				float* color_byte = (float*)&color;
				buff[__thread_id].push_back(*color_byte);
				//color_buff[__thread_id].push_back(color._r);
				//color_buff[__thread_id].push_back(color._g);
				//color_buff[__thread_id].push_back(color._b);
				//color_buff[__thread_id].push_back(color._a);
			}

			void Read_Record(Point_3D<MasterType>& point, True_Color_RGBA<NT>& color)
			{		
				buff[__thread_id].push_back(point._x);
				buff[__thread_id].push_back(point._y);
				buff[__thread_id].push_back(color._r);
				buff[__thread_id].push_back(color._g);
				buff[__thread_id].push_back(color._b);
				buff[__thread_id].push_back(color._a);
			}

			static void Reschedule_Logging(void* obj, int new_iteration)
			{
				typedef typename MasterType::vehicle_data_logger_type this_type;
				cout << "Rescheduling the logging to new iteration: "<<new_iteration<<endl;
				((this_type*)obj)->template Reschedule<ComponentType>(new_iteration,0);
				((this_type*)obj)->_vehicle_layer->template Reschedule_Execution<NT>(new_iteration);
			}

			static void Logging_Event_Controller(ComponentType* _this,Event_Response& response)
			{
				typedef typename MasterType::vehicle_data_logger_type this_type;
				typedef Prototypes::Vehicle_Data_Logger<typename MasterType::vehicle_data_logger_type> _Interface;
				_Interface* this_ptr=(_Interface*)_this;
				this_type* pthis = (this_type*)_this;
				
				// set next planning time for other functions to use
				this_ptr->template Next_Logging_Time<Simulation_Timestep_Increment>(iteration() + this_ptr->template Logging_Interval<Simulation_Timestep_Increment>());

				// swap buffer and current for output strings and trip records
				if(sub_iteration() == 0)
				{				
					std::vector<float>* tmp = pthis->buff;
					pthis->buff = pthis->current;
					pthis->current = tmp;

					response.next._iteration = iteration();
					response.next._sub_iteration = 1;
					pthis->template Write_Data_To_File_Event<NT>();
				}
				else if (sub_iteration() < (int)num_sim_threads())
				{
					response.next._iteration = iteration();
					response.next._sub_iteration = sub_iteration()+1;
					pthis->template Write_Data_To_File_Event<NT>();
				}
				else
				{
					response.next._iteration = this_ptr->template Next_Logging_Time<Simulation_Timestep_Increment>();
					response.next._sub_iteration = 0;
				}
			}

			static void Reading_Event_Controller(ComponentType* _this,Event_Response& response)
			{
				typedef typename MasterType::vehicle_data_logger_type this_type;
				typedef Prototypes::Vehicle_Data_Logger<typename MasterType::vehicle_data_logger_type> _Interface;
				_Interface* this_ptr=(_Interface*)_this;
				this_type* pthis = (this_type*)_this;
				
				// set next planning time for other functions to use
				this_ptr->template Next_Logging_Time<Simulation_Timestep_Increment>(iteration() + this_ptr->template Logging_Interval<Simulation_Timestep_Increment>());

				// swap buffer and current for output strings and trip records
				if(sub_iteration() == 0)
				{				
					pthis->template Read_Data_From_File_Event<NT>();
					response.next._iteration = this_ptr->template Next_Logging_Time<Simulation_Timestep_Increment>();
					response.next._sub_iteration = 0;
				}
				else
				{
					response.next._iteration = this_ptr->template Next_Logging_Time<Simulation_Timestep_Increment>();
					response.next._sub_iteration = 0;
				}
			}

			template<typename TargetType> void Write_Data_To_File_Event()
			{
				int iter = iteration();
				int i = sub_iteration();

				// update the index if this is a logging iteration
				if (iter % 60 == 0 && i == 0)
				{
					this->_out_index_file.Write(iter);
					this->_out_index_file.Write_Line(this->_bytes_written);
				}				

				// if this is the first write operation of the iteration, write the total size of the records for current iteration to the file
				if (i == 0)
				{
					// first log the iteration number in the snapshot file, for validation purposes
					int iter = iteration();
					this->_outfile.template Write_Value<int>(iter);
					this->_bytes_written += sizeof(int);

					// next, log the size of the block to be written for this timestep, so it can be read in as one block of data later
					int size=0;
					for (int j=0; j<(int)num_sim_threads();++j) size += current[j].size();
					this->_outfile.template Write_Value<int>(size);
					this->_bytes_written += sizeof(int);
				}

				// write out strings in the current buffer to log file and clear it
				if (current[i].size()) this->_outfile.template Write_Array<float>(&(current[i])[0],current[i].size());
				this->_bytes_written += sizeof(float) * current[i].size();

				current[i].clear();
			}

			template<typename TargetType> void Read_Data_From_File_Event(requires(TargetType,!check_2(typename MasterType::visual_vehicle_type,NT,is_same)))
			{
				int i = sub_iteration();

				int time;
				int size;
				float* values=nullptr;
				
				this->_infile.Read_Value(time);

				cout <<"Reading from file at time="<<time;

				// check if the time has been switched.  If time read in from file does not match current iteration, get the new location from index, read in the new time and validate again
				if (time != iteration())
				{
					cout <<"!!!!!!!! Iteration has been reset: iteration="<<iteration()<<", time="<<time;
					dense_hash_map<int,long long>::iterator itr = this->index.find(iteration());

					// current iteration not found in index, something wrong happened with resetting the iteration
					if (itr == this->index.end())THROW_EXCEPTION("Error in resetting the iteration - current iteration not found in index for snapshot file.");

					// current iteration was found, reset file to the current location
					long long byte_loc = itr->second;

					this->_infile.Read_Value(time,byte_loc);
					cout <<" ---- New time = "<<time<<endl;

					if (time != iteration()) THROW_EXCEPTION("Error, something wrong happened again - file location was not reset to the proper time. Read_value="<<time<<", current iteration="<<iteration());
				}


				this->_infile.Read_Value(size);

				cout << ", size="<<size<<endl;

				if (size > 0)
				{
					values = new float[size];
					this->_infile.template Read_Array<float>(values,size);

					/*typedef typename MasterType::visual_vehicle_type vehicle_interface;
					vehicle_interface* veh_itf = (vehicle_interface*)Allocate<typename MasterType::visual_vehicle_type>();*/
					_vehicle_layer->template Update<NT>(size,values);
				}
			}
			template<typename TargetType> void Read_Data_From_File_Event(requires(TargetType,check_2(typename MasterType::visual_vehicle_type,NT,is_same)))
			{
				assert_check_2(typename MasterType::visual_vehicle_type,NT,is_same,"Error, cannot read data when visual_vehicle_type is not defined.");
			}
		};
	}
}
