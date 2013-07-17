//*********************************************************
//	Antares_Person_Data_Logger_Implementation.h - Used for visualizing aggregate demand results
//*********************************************************

#pragma once
#include "Antares_Includes.h"
#include "User_Space\Person_Data_Logger_Implementation.h"
#include "Canvas.h"
#include "Information_Panel.h"

namespace Person_Components
{
	namespace Implementations
	{
		implementation struct Antares_Person_Data_Logger_Implementation:public Person_Data_Logger_Implementation<MasterType,ParentType,APPEND_CHILD(Antares_Person_Data_Logger_Implementation)>
		{
			vector<Point_2D<MasterType>> _activity_time_lost_cache;
			vector<Point_2D<MasterType>> _cancelled_activities_cache;
			vector<Point_2D<MasterType>> _ttime_distribution_cache;
			vector<Point_2D<MasterType>> _generated_activity_distribution_cache;
			vector<Point_2D<MasterType>> _executed_activity_distribution_cache;

			vector<Point_2D<MasterType>> _ref_activity_time_lost_cache;
			vector<Point_2D<MasterType>> _ref_ttime_distribution_cache;

			typedef dense_hash_map<int,float> float_map;
			typedef typename float_map::iterator float_map_itr;
			float_map reference_activity_time_lost;
			typedef dense_hash_map<int,vector<float>> vector_map;
			typedef typename vector_map::iterator vector_map_itr;
			vector_map reference_ttime_distribution;

			member_prototype(Antares_Layer,activity_time_lost_layer,typename type_of(MasterType::antares_layer),none,none);
			member_prototype(Antares_Layer,cancelled_activities_layer,typename type_of(MasterType::antares_layer),none,none);
			member_prototype(Antares_Layer,ttime_distribution_layer,typename type_of(MasterType::antares_layer),none,none);
			member_prototype(Antares_Layer,activity_type_distribution_layer,typename type_of(MasterType::antares_layer),none,none);

			bool _draw_reference;

			feature_implementation void Initialize()
			{
				cout << endl << "Initializing demand display...";

				_draw_reference = false;

				// Basic initialization for person data logger
				((Person_Data_Logger_Implementation<MasterType,ParentType,APPEND_CHILD(Person_Data_Logger_Implementation)>*)this)->Initialize<ComponentType,CallerType,TargetType>();

				// Initialization of 2D plotting layers
				initialize_demand_moe_plotting_layers<ComponentType,CallerType,TargetType>();

				// Initialize historic data if directory provided
				initialize_reference_data<ComponentType,CallerType,TargetType>();
			}

			feature_implementation void initialize_reference_data()
			{
				// Initialize historic data if directory provided
				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type> _Scenario_Interface;
				_Scenario_Interface* scenario = (_Scenario_Interface*)_global_scenario;
				stringstream moe_directory("");
				moe_directory << scenario->historic_demand_moe_directory<string>();


				if (moe_directory.str() == "") return;	

				_draw_reference = true;			


				// open the lost activity time reference file and input to array
				reference_activity_time_lost.clear();
				reference_activity_time_lost.set_deleted_key(-2);
				reference_activity_time_lost.set_empty_key(-1);
				stringstream activity_time_lost_filename("");
				activity_time_lost_filename << moe_directory.str() << "//lost_activity_time.csv";
				fstream activity_time_lost_reference_file;
				activity_time_lost_reference_file.open(activity_time_lost_filename.str());
				if (activity_time_lost_reference_file.is_open())
				{
					
					vector<string> tokens;
					string line;
					int token_size = 2;
					getline(activity_time_lost_reference_file,line); // skip the first line
					while (activity_time_lost_reference_file.good())
					{
						getline(activity_time_lost_reference_file,line);
						string_split(tokens, line, token_size);
						if (tokens[0].length() == 0)
						{
							break;
						}
						int time = stoi(tokens[0]);
						if (time < ((_Scenario_Interface*)_global_scenario)->template simulation_start_time<int>())
						{
							continue;
						}
						else if (time > ((_Scenario_Interface*)_global_scenario)->template simulation_end_time<int>() + ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>())
						{
							break;
						}
						else
						{
							float lost_time = stof(tokens[1]);
							reference_activity_time_lost.insert(pair<int,float>(time,lost_time));
						}
					}
				}

				// open the lost activity time reference file and input to array
				reference_ttime_distribution.clear();
				reference_ttime_distribution.set_deleted_key(-2);
				reference_ttime_distribution.set_empty_key(-1);
				stringstream ttime_distribution_filename("");
				ttime_distribution_filename << moe_directory.str() << "//ttime_distribution.csv";
				fstream ttime_distribution_file;
				ttime_distribution_file.open(ttime_distribution_filename.str());
				if (ttime_distribution_file.is_open())
				{
					
					vector<string> tokens;
					string line;
					int token_size = 26;
					getline(ttime_distribution_file,line); // skip the first line
					while (ttime_distribution_file.good())
					{
						getline(ttime_distribution_file,line);

						string_split(tokens, line, token_size);
						if (tokens[0].length() == 0)
						{
							break;
						}
						int time = stoi(tokens[0]);
						if (time < ((_Scenario_Interface*)_global_scenario)->template simulation_start_time<int>())
						{
							continue;
						}
						else if (time > ((_Scenario_Interface*)_global_scenario)->template simulation_end_time<int>() + ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>())
						{
							break;
						}
						else
						{
							vector<float> ttimes;
							for (int i = 1; i < 26; i++)
							{
								float ttime = stof(tokens[i]);
								ttimes.push_back(ttime);
							}
							reference_ttime_distribution.insert(pair<int,vector<float>>(time,ttimes));
						}
					}
				}
			}


			feature_implementation void initialize_demand_moe_plotting_layers()
			{
				// configure plot layer
				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type,ComponentType> _Scenario_Interface;

				Antares_Layer_Configuration pcfg;

				pcfg.Configure_Plot();
				pcfg.storage_period = 900;
				pcfg.storage_offset = 0;
				pcfg.target_sub_iteration = Scenario_Components::Types::END_OF_ITERATION + 1;
				pcfg.grouped = true;
				pcfg.group_color = true;

				pcfg.x_label = "time (second)";
				
				pcfg.y_label = "Time lost (minutes)";
				_activity_time_lost_layer=Allocate_New_Plot_Layer< typename MasterType::type_of(information_panel),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(activity_time_lost_layer),Antares_Person_Data_Logger_Implementation>*, string& > >(string("Time Lost"));
				_activity_time_lost_layer->Initialize<NULLTYPE>(pcfg);

				pcfg.y_label = "Activities";
				_cancelled_activities_layer=Allocate_New_Plot_Layer< typename MasterType::type_of(information_panel),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(cancelled_activities_layer),Antares_Person_Data_Logger_Implementation>*, string& > >(string("Cancelled Activities"));
				_cancelled_activities_layer->Initialize<NULLTYPE>(pcfg);

				pcfg.x_label = "time (minutes)";
				pcfg.y_label = "Travel Time Density";
				_ttime_distribution_layer=Allocate_New_Plot_Layer< typename MasterType::type_of(information_panel),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(ttime_distribution_layer),Antares_Person_Data_Logger_Implementation>*, string& > >(string("Travel Time Distribution"));
				_ttime_distribution_layer->Initialize<NULLTYPE>(pcfg);
				

				pcfg.x_label = "Work-home          Home          Work          Other-Work          School          MajShop          Shop          Service          Health          Personal          Errand          Pick/drop          Meal          Civic          Leisure          Recreation          Social          Transport          Other";
				pcfg.y_label = "Activity counts";
				_activity_type_distribution_layer=Allocate_New_Plot_Layer< typename MasterType::type_of(information_panel),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(activity_type_distribution_layer),Antares_Person_Data_Logger_Implementation>*, string& > >(string("Activity Type Distribution"));
				_activity_type_distribution_layer->Initialize<NULLTYPE>(pcfg);
			}



			feature_implementation void Write_Data_To_File() 
			{
				((Person_Data_Logger_Implementation<MasterType,ParentType,APPEND_CHILD(Person_Data_Logger_Implementation)>*)this)->Write_Data_To_File<ComponentType,CallerType,TargetType>();
				if (_sub_iteration == 0) plot_demand_moes();
				//plot_reference_network_moes();
			}

			void plot_demand_moes()
			{
				cout << endl << "Plot demand MOEs";
				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type> _Scenario_Interface;
#pragma pack(push,1)
				struct Plot_Element
				{
					True_Color_RGBA<NT> color;
					int num_primitives;
					Point_2D<MasterType>* points;
				};
#pragma pack(pop)
				Point_2D<MasterType> submission;
				Plot_Element element;
				element.color._r = 255;
				element.color._g = 0;
				element.color._b = 0;
				element.color._a = 255;
				submission._x=_iteration;
				Plot_Element ref_element;
				ref_element.color._r = 0;
				ref_element.color._g = 0;
				ref_element.color._b = 255;
				ref_element.color._a = 255;


				//==============================================================
				// plot activity_time_lost
				//------------------------
				submission._y=_activity_time_lost;
				_activity_time_lost_cache.push_back(submission);
				element.num_primitives = _activity_time_lost_cache.size();
				element.points = &_activity_time_lost_cache.front();
				_activity_time_lost_layer->Push_Element<Regular_Element>((void*)&element);
				// plot reference activity time lost
				//--------------------------
				if (this->_draw_reference)
				{
					cout << "Draw reference data"<<endl;
					float_map_itr itr = this->reference_activity_time_lost.find(_iteration);
					if (itr != this->reference_activity_time_lost.end())
					{
						float ref_activity_time_lost = itr->second;
						submission._y = ref_activity_time_lost;
						cout << "Reference point: " << submission._x << ", " << submission._y<<endl;
						_ref_activity_time_lost_cache.push_back(submission);
						ref_element.num_primitives = _ref_activity_time_lost_cache.size();
						ref_element.points = &_ref_activity_time_lost_cache.front();
						_activity_time_lost_layer->Push_Element<Regular_Element>((void*)&ref_element);
					}
				}
				

				//==============================================================
				// plot cancelled acts
				submission._y=_cancelled_activities;
				_cancelled_activities_cache.push_back(submission);
				element.num_primitives = _cancelled_activities_cache.size();
				element.points = &_cancelled_activities_cache.front();
				_cancelled_activities_layer->Push_Element<Regular_Element>((void*)&element);

				//==============================================================
				// plot travel time distribution
				//-----------------------------------
				// Current model
				_ttime_distribution_cache.clear();
				vector<int> counts;
				int count =0;
				int total =0;
				for (int j=0; j < 25; j++)
				{
					int count = 0;
					for (int k=0; k < _num_threads; k++) // collect value over all threads
					{
						count += ttime_distribution[k][j];
					}
					counts.push_back(count);
					total += count;
				}
				submission._x = 0;
				submission._y = 0;
				_ttime_distribution_cache.push_back(submission);	
				for (int j=0; j < 25; j++)
				{
					submission._x = 5*(j+1);
					submission._y = (float)counts[j]/(float)total;
					_ttime_distribution_cache.push_back(submission);	
				}			
				element.num_primitives = _ttime_distribution_cache.size();
				element.points = &_ttime_distribution_cache.front();
				_ttime_distribution_layer->Push_Element<Regular_Element>((void*)&element);
				//------------------------------------------
				//reference data
				if (this->_draw_reference)
				{
					_ref_ttime_distribution_cache.clear();
					int count =0;
					vector_map_itr itr = this->reference_ttime_distribution.find(_iteration);
					if (itr != this->reference_ttime_distribution.end())
					{
						for (int j=0; j < 25; j++)
						{
							count += itr->second[j];
						}
						submission._x = 0;
						submission._y = 0;
						_ref_ttime_distribution_cache.push_back(submission);	
						for (int j=0; j < 25; j++)
						{
							submission._x = 5*(j+1);
							submission._y = (float)itr->second[j]/(float)count;
							_ref_ttime_distribution_cache.push_back(submission);	
						}			
						ref_element.num_primitives = _ref_ttime_distribution_cache.size();
						ref_element.points = &_ref_ttime_distribution_cache.front();
						_ttime_distribution_layer->Push_Element<Regular_Element>((void*)&ref_element);
					}
					
				}

				//==============================================================
				// plot activity generation curves
				//-------------------------------------------
				// first do planned activities
				_generated_activity_distribution_cache.clear();
				submission._x = 0;
				submission._y = 0;
				_generated_activity_distribution_cache.push_back(submission);	
				for (int j=0; j < 19; j++)
				{
					int count = 0;
					for (int k=0; k < _num_threads; k++) // collect value over all threads
					{
						count += planned_acts[k][j];
					}
					submission._x = (float)j - 0.12;
					submission._y = 0;
					_generated_activity_distribution_cache.push_back(submission);
					submission._x = (float)j - 0.12;
					submission._y = (float)count;
					_generated_activity_distribution_cache.push_back(submission);	
					submission._x = (float)j + 0.12;
					submission._y = (float)count;
					_generated_activity_distribution_cache.push_back(submission);	
					submission._x = (float)j + 0.12;
					submission._y = 0;
					_generated_activity_distribution_cache.push_back(submission);
				}			
				element.num_primitives = _generated_activity_distribution_cache.size();
				element.points = &_generated_activity_distribution_cache.front();
				_activity_type_distribution_layer->Push_Element<Regular_Element>((void*)&element);
				//-------------------------------------------
				// next do planned activities
				_executed_activity_distribution_cache.clear();
				submission._x = 0;
				submission._y = 0;
				_executed_activity_distribution_cache.push_back(submission);	
				for (int j=0; j < 19; j++)
				{
					int count = 0;
					for (int k=0; k < _num_threads; k++) // collect value over all threads
					{
						count += executed_acts[k][j];
					}
					submission._x = (float)j - 0.1;
					submission._y = 0;
					_executed_activity_distribution_cache.push_back(submission);
					submission._x = (float)j - 0.1;
					submission._y = (float)count;
					_executed_activity_distribution_cache.push_back(submission);	
					submission._x = (float)j + 0.1;
					submission._y = (float)count;
					_executed_activity_distribution_cache.push_back(submission);	
					submission._x = (float)j + 0.1;
					submission._y = 0;
					_executed_activity_distribution_cache.push_back(submission);
				}
				Plot_Element element2;
				element2.color._r = 0;
				element2.color._g = 0;
				element2.color._b = 255;
				element2.color._a = 255;
		
				element2.num_primitives = _executed_activity_distribution_cache.size();
				element2.points = &_executed_activity_distribution_cache.front();
				_activity_type_distribution_layer->Push_Element<Regular_Element>((void*)&element2);
			}

//			void plot_reference_network_moes()
//			{
//				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type> _Scenario_Interface;
//				typedef Network_Components::Prototypes::Network_Prototype<typename MasterType::network_type> _Network_Interface;
//
//				int time_index = ((_Network_Interface*)this)->template current_simulation_interval_index<int>();
//				if (time_index >= (int)(reference_td_network_moe_data_array.size())) 
//				{
//					return;
//				}
//				Extended_MOE_Data reference_moe_data = reference_td_network_moe_data_array[time_index];
//				if (time_index < (int)(reference_td_network_moe_data_array.size() - 1))
//				{
//					reference_moe_data.network_vmt =  reference_td_network_moe_data_array[time_index+1].network_vmt;
//					reference_moe_data.network_vht =  reference_td_network_moe_data_array[time_index+1].network_vht;
//				}
//#pragma pack(push,1)
//				struct Plot_Element
//				{
//					True_Color_RGBA<NT> color;
//					int num_primitives;
//					Point_2D<MasterType>* points;
//				};
//#pragma pack(pop)
//				Point_2D<MasterType> submission;
//				Plot_Element element;
//				element.color._r = 128;
//				element.color._g = 128;
//				element.color._b = 128;
//				element.color._a = 255;
//				submission._x=_iteration;
//
//				// plot vmt
//				submission._y=reference_moe_data.network_vmt;
//				_reference_network_vmt_cache.push_back(submission);
//				element.num_primitives = _reference_network_vmt_cache.size();
//				element.points = &_reference_network_vmt_cache.front();
//				_network_vmt_layer->Push_Element<Regular_Element>((void*)&element);
//
//				// plot vht
//				submission._y=reference_moe_data.network_vht;
//				_reference_network_vht_cache.push_back(submission);
//				element.num_primitives = _reference_network_vht_cache.size();
//				element.points = &_reference_network_vht_cache.front();
//				_network_vht_layer->Push_Element<Regular_Element>((void*)&element);
//
//				//// plot loaded vehicles
//				//submission._y=reference_moe_data.network_cumulative_loaded_vehicles;
//				//_reference_network_cumulative_loaded_vehicles_cache.push_back(submission);
//				//element.num_primitives = _reference_network_cumulative_loaded_vehicles_cache.size();
//				//element.points = &_reference_network_cumulative_loaded_vehicles_cache.front();
//				//_network_cumulative_loaded_vehicles_layer->Push_Element<Regular_Element>((void*)&element);
//
//				// plot departed vehicles
//				submission._y=reference_moe_data.network_cumulative_departed_vehicles;
//				_reference_network_cumulative_departed_vehicles_cache.push_back(submission);
//				element.num_primitives = _reference_network_cumulative_departed_vehicles_cache.size();
//				element.points = &_reference_network_cumulative_departed_vehicles_cache.front();
//				_network_cumulative_departed_vehicles_layer->Push_Element<Regular_Element>((void*)&element);
//
//				// plot arrived vehicles
//				submission._y=reference_moe_data.network_cumulative_arrived_vehicles;
//				_reference_network_cumulative_arrived_vehicles_cache.push_back(submission);
//				element.num_primitives = _reference_network_cumulative_arrived_vehicles_cache.size();
//				element.points = &_reference_network_cumulative_arrived_vehicles_cache.front();
//				_network_cumulative_arrived_vehicles_layer->Push_Element<Regular_Element>((void*)&element);
//
//				// plot in_network vehicles
//				submission._y=reference_moe_data.network_in_network_vehicles;
//				_reference_network_in_network_vehicles_cache.push_back(submission);
//				element.num_primitives = _reference_network_in_network_vehicles_cache.size();
//				element.points = &_reference_network_in_network_vehicles_cache.front();
//				_network_in_network_vehicles_layer->Push_Element<Regular_Element>((void*)&element);
//
//				// plot network_avg_link_travel_time
//				submission._y=reference_moe_data.network_avg_link_travel_time;
//				_reference_network_avg_link_travel_time_cache.push_back(submission);
//				element.num_primitives = _reference_network_avg_link_travel_time_cache.size();
//				element.points = &_reference_network_avg_link_travel_time_cache.front();
//				_network_avg_link_travel_time_layer->Push_Element<Regular_Element>((void*)&element);
//
//				// plot _network_avg_link_speed_cache
//				submission._y=reference_moe_data.network_avg_link_speed;
//				_reference_network_avg_link_speed_cache.push_back(submission);
//				element.num_primitives = _reference_network_avg_link_speed_cache.size();
//				element.points = &_reference_network_avg_link_speed_cache.front();
//				_network_avg_link_speed_layer->Push_Element<Regular_Element>((void*)&element);
//
//
//				// plot _network_avg_link_density_cache
//				submission._y=reference_moe_data.network_avg_link_density;
//				_reference_network_avg_link_density_cache.push_back(submission);
//				element.num_primitives = _reference_network_avg_link_density_cache.size();
//				element.points = &_reference_network_avg_link_density_cache.front();
//				_network_avg_link_density_layer->Push_Element<Regular_Element>((void*)&element);
//
//				//// plot _network_avg_link_travel_time_ratio
//				//submission._y=reference_moe_data.network_avg_link_travel_time_ratio;
//				//_reference_network_avg_link_travel_time_ratio_cache.push_back(submission);
//				//element.num_primitives = _reference_network_avg_link_travel_time_ratio_cache.size();
//				//element.points = &_reference_network_avg_link_travel_time_ratio_cache.front();
//				//_network_avg_link_travel_time_ratio_layer->Push_Element<Regular_Element>((void*)&element);
//
//
//				//// plot _network_avg_link_speed_ratio_cache
//				//submission._y=reference_moe_data.network_avg_link_speed_ratio;
//				//_reference_network_avg_link_speed_ratio_cache.push_back(submission);
//				//element.num_primitives = _reference_network_avg_link_speed_ratio_cache.size();
//				//element.points = &_reference_network_avg_link_speed_ratio_cache.front();
//				//_network_avg_link_speed_ratio_layer->Push_Element<Regular_Element>((void*)&element);
//
//
//				//// plot _network_avg_link_density_ratio_cache
//				//submission._y=reference_moe_data.network_avg_link_density_ratio;
//				//_reference_network_avg_link_density_ratio_cache.push_back(submission);
//				//element.num_primitives = _reference_network_avg_link_density_ratio_cache.size();
//				//element.points = &_reference_network_avg_link_density_ratio_cache.front();
//				//_network_avg_link_density_ratio_layer->Push_Element<Regular_Element>((void*)&element);
//
//				//// plot _network_avg_link_queue_length_cache
//				//submission._y=reference_moe_data.network_avg_link_queue_length;
//				//_reference_network_avg_link_queue_length_cache.push_back(submission);
//				//element.num_primitives = _reference_network_avg_link_queue_length_cache.size();
//				//element.points = &_reference_network_avg_link_queue_length_cache.front();
//				//_network_avg_link_queue_length_layer->Push_Element<Regular_Element>((void*)&element);
//			}
//
			//void read_historic_link_moe()
			//{
			//	define_container_and_value_interface_unqualified_container(_Links_Container_Interface, _Link_Interface, type_of(links_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, NULLTYPE);
			//	typedef Network_Components::Prototypes::Network_Prototype<typename MasterType::network_type> _Network_Interface;
			//	typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type> _Scenario_Interface;

			//	fstream& historic_link_moe_file = ((_Scenario_Interface*)_global_scenario)->template historic_link_moe_file<fstream&>();
			//	if (!historic_link_moe_file.is_open())
			//	{
			//		THROW_EXCEPTION(endl << "Link MOE reference file cannot be opened" << endl);
			//	}
			//	struct Link_MOE_Data link_moe_data;
			//	vector<string> tokens;
			//	string line;
			//	int current_time = ((_Network_Interface*)this)->template start_of_current_simulation_interval_absolute<int>();
			//	int record_time;
			//	typename MasterType::link_type* link;
			//	Network_Components::Types::Link_ID_Dir link_id_dir;
			//	for (int i = 0; i < (int)_links_container.size(); i++)
			//	{
			//		getline(historic_link_moe_file, line); 
			//		string_split(tokens, line);
			//		if (tokens.size() == 0)
			//		{
			//			break;
			//		}
			//		record_time = stoi(tokens[1]);
			//		if (record_time < current_time)
			//		{
			//			// skip until current time
			//			while(historic_link_moe_file.good())
			//			{
			//				getline(historic_link_moe_file, line);
			//				string_split(tokens, line);
			//				record_time = stoi(tokens[1]);
			//				if (record_time >= current_time)
			//				{
			//					break;
			//				}
			//			}
			//		}
			//		link_id_dir.id = stol(tokens[3]);
			//		link_id_dir.dir = stol(tokens[4]);
			//		link = (typename MasterType::link_type*)(_link_dbid_dir_to_ptr_map[link_id_dir.id_dir]);
			//		int offset = 8;
			//		link_moe_data.link_travel_time = stof(tokens[offset++]);
			//		link_moe_data.link_travel_time_standard_deviation = stof(tokens[offset++]);
			//		link_moe_data.link_queue_length = stof(tokens[offset++]);
			//		link_moe_data.link_travel_delay = stof(tokens[offset++]);
			//		link_moe_data.link_travel_delay_standard_deviation = stof(tokens[offset++]);
			//		link_moe_data.link_speed = stof(tokens[offset++]);
			//		link_moe_data.link_density = stof(tokens[offset++]);
			//		link_moe_data.link_in_flow_rate = stof(tokens[offset++]);
			//		link_moe_data.link_out_flow_rate = stof(tokens[offset++]);
			//		link_moe_data.link_in_volume = stof(tokens[offset++]);
			//		link_moe_data.link_out_volume = stof(tokens[offset++]);
			//		link_moe_data.link_speed_ratio = stof(tokens[offset++]);
			//		link_moe_data.link_in_flow_ratio = stof(tokens[offset++]);
			//		link_moe_data.link_out_flow_ratio = stof(tokens[offset++]);
			//		link_moe_data.link_density_ratio = stof(tokens[offset++]);
			//		link_moe_data.link_travel_time_ratio = stof(tokens[offset++]);
			//		link_moe_data.num_vehicles_in_link = stof(tokens[offset++]);
			//		link->historic_link_moe_data = link_moe_data;
			//	}
			//}

		};

	}
}