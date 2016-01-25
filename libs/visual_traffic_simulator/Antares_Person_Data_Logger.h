//*********************************************************
//	Antares_Person_Data_Logger_Implementation.h - Used for visualizing aggregate demand results
//*********************************************************

#pragma once
#include "Dependencies.h"
#include "Activity_Simulator\Person_Data_Logger_Implementation.h"
#include "Canvas.h"
#include "Information_Panel.h"

namespace Person_Components
{
	namespace Implementations
	{
		implementation struct Antares_Person_Data_Logger_Implementation:public Person_Data_Logger_Implementation<MasterType,INHERIT(Antares_Person_Data_Logger_Implementation)>
		{
			//==================================================================================
			// Data caches for current and historical plot points
			std::vector<Point_2D<MasterType>> _replanned_activity_count_cache;
			std::vector<Point_2D<MasterType>> _activity_time_lost_cache;
			std::vector<Point_2D<MasterType>> _cancelled_activities_cache;
			std::vector<Point_2D<MasterType>> _ttime_distribution_cache;
			std::vector<Point_2D<MasterType>> _generated_activity_distribution_cache;
			std::vector<Point_2D<MasterType>> _executed_activity_distribution_cache;

			std::vector<Point_2D<MasterType>> _ref_activity_time_lost_cache;
			std::vector<Point_2D<MasterType>> _ref_cancelled_activities_cache;
			std::vector<Point_2D<MasterType>> _ref_ttime_distribution_cache;
			std::vector<Point_2D<MasterType>> _ref_generated_activity_distribution_cache;
			std::vector<Point_2D<MasterType>> _ref_executed_activity_distribution_cache;

			//==================================================================================
			// Vectors of historical data read from file on initialization
			typedef dense_hash_map<int,float> float_map;
			typedef typename float_map::iterator float_map_itr;
			float_map reference_activity_time_lost;
			float_map reference_cancelled_activities;
			typedef dense_hash_map<int,std::vector<float>> vector_map;
			typedef typename std::vector_map::iterator vector_map_itr;
			std::vector_map reference_ttime_distribution;
			std::vector_map reference_executed_activity_distribution;

			//==================================================================================
			// 2D plotting layers
			m_prototype(Antares_Layer,typename MasterType::antares_layer_type,replanned_activities_layer, NONE, NONE);
			m_prototype(Antares_Layer,typename MasterType::antares_layer_type,activity_time_lost_layer, NONE, NONE);
			m_prototype(Antares_Layer,typename MasterType::antares_layer_type,cancelled_activities_layer, NONE, NONE);
			m_prototype(Antares_Layer,typename MasterType::antares_layer_type,ttime_distribution_layer, NONE, NONE);
			m_prototype(Antares_Layer,typename MasterType::antares_layer_type,activity_type_distribution_layer, NONE, NONE);

			bool _draw_reference;

			template<typename TargetType> void Initialize()
			{
				cout << endl << "Initializing demand display...";

				_draw_reference = false;

				// Basic initialization for person data logger
				((Person_Data_Logger_Implementation<MasterType,INHERIT(Person_Data_Logger_Implementation)>*)this)->Initialize<TargetType>();

				// Initialization of 2D plotting layers
				initialize_demand_moe_plotting_layers<TargetType>();

				// Initialize historic data if directory provided
				initialize_reference_data<TargetType>();
			}


			template<typename TargetType> void initialize_reference_data()
			{
				//==================================================================================
				// Initialize historic data if directory provided
				typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;
				_Scenario_Interface* scenario = (_Scenario_Interface*)_global_scenario;
				stringstream moe_directory("");
				moe_directory << scenario->historic_demand_moe_directory<string>();
				// Exit if no historical directory provided
				if (moe_directory.str() == "") return;	
				_draw_reference = true;			

				//==================================================================================
				// open the demand moe file and input to array
				reference_activity_time_lost.clear(); reference_activity_time_lost.set_deleted_key(-2); reference_activity_time_lost.set_empty_key(-1);
				reference_cancelled_activities.clear(); reference_cancelled_activities.set_deleted_key(-2); reference_cancelled_activities.set_empty_key(-1);
				stringstream demand_moe_filename("");
				demand_moe_filename << moe_directory.str() << "//moe_demand.csv";
				fstream demand_moe_reference_file;
				demand_moe_reference_file.open(demand_moe_filename.str());
				if (demand_moe_reference_file.is_open())
				{
					
					std::vector<string> tokens;
					string line;
					int token_size = 3;
					getline(demand_moe_reference_file,line); // skip the first line
					while (demand_moe_reference_file.good())
					{
						getline(demand_moe_reference_file,line);
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
							float cancelled_acts = stof(tokens[2]);
							reference_activity_time_lost.insert(pair<int,float>(time,lost_time));
							reference_cancelled_activities.insert(pair<int,float>(time,cancelled_acts));
						}
					}
				}

				//==================================================================================
				// open the travel time distribution reference file and input to array
				reference_ttime_distribution.clear();
				reference_ttime_distribution.set_deleted_key(-2);
				reference_ttime_distribution.set_empty_key(-1);
				stringstream ttime_distribution_filename("");
				ttime_distribution_filename << moe_directory.str() << "//ttime_distribution.csv";
				fstream ttime_distribution_file;
				ttime_distribution_file.open(ttime_distribution_filename.str());
				if (ttime_distribution_file.is_open())
				{
					
					std::vector<string> tokens;
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
							std::vector<float> ttimes;
							for (int i = 1; i < 26; i++)
							{
								float ttime = stof(tokens[i]);
								ttimes.push_back(ttime);
							}
							reference_ttime_distribution.insert(pair<int,std::vector<float>>(time,ttimes));
						}
					}
				}

				//==================================================================================
				// open the executed activitity distribution reference file and input to array
				reference_executed_activity_distribution.clear();
				reference_executed_activity_distribution.set_deleted_key(-2);
				reference_executed_activity_distribution.set_empty_key(-1);
				stringstream executed_activity_filename("");
				executed_activity_filename << moe_directory.str() << "//executed_activities.csv";
				fstream executed_activity_file;
				executed_activity_file.open(executed_activity_filename.str());
				if (executed_activity_file.is_open())
				{
					
					std::vector<string> tokens;
					string line;
					int token_size = 26;
					getline(executed_activity_file,line); // skip the first line
					while (executed_activity_file.good())
					{
						getline(executed_activity_file,line);

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
							std::vector<float> gen_acts;
							for (int i = 1; i < 20; i++)
							{
								float acts = stof(tokens[i]);
								gen_acts.push_back(acts);
							}
							reference_executed_activity_distribution.insert(pair<int,std::vector<float>>(time,gen_acts));
						}
					}
				}
			}


			template<typename TargetType> void initialize_demand_moe_plotting_layers()
			{
				// configure plot layer
				typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;

				Antares_Layer_Configuration pcfg;

				pcfg.Configure_Plot();
				pcfg.storage_period = 900;
				pcfg.storage_offset = 0;
				pcfg.target_sub_iteration = Scenario_Components::Types::END_OF_ITERATION + 1;
				pcfg.grouped = true;
				pcfg.group_color = true;

				pcfg.x_label = "time (second)";
				
				pcfg.y_label = "Time lost (minutes)";
				_activity_time_lost_layer=Allocate_New_Plot_Layer<MT>(string("Time Lost"));
				_activity_time_lost_layer->Initialize<NULLTYPE>(pcfg);

				pcfg.y_label = "Activity count";
				_replanned_activities_layer=Allocate_New_Plot_Layer<MT>(string("Replanned Activities"));
				_replanned_activities_layer->Initialize<NULLTYPE>(pcfg);

				pcfg.y_label = "Activities";
				_cancelled_activities_layer=Allocate_New_Plot_Layer<MT>(string("Cancelled Activities"));
				_cancelled_activities_layer->Initialize<NULLTYPE>(pcfg);

				pcfg.x_label = "time (minutes)";
				pcfg.y_label = "Travel Time Density";

				_ttime_distribution_layer=Allocate_New_Plot_Layer<MT>(string("Travel Time Distribution"));
				_ttime_distribution_layer->Initialize<NULLTYPE>(pcfg);
				

				pcfg.x_label = "Work-home          Home          Work          Other-Work          School          MajShop          Shop          Service          Health          Personal          Errand          Pick/drop          Meal          Civic          Leisure          Recreation          Social          Transport          Other";
				pcfg.y_label = "Activity counts";
				
				_activity_type_distribution_layer=Allocate_New_Plot_Layer<MT>(string("Activity Type Distribution"));
				_activity_type_distribution_layer->Initialize<NULLTYPE>(pcfg);
			}



			template<typename TargetType> void Write_Data_To_File() 
			{
				((Person_Data_Logger_Implementation<MasterType,INHERIT(Person_Data_Logger_Implementation)>*)this)->Write_Data_To_File<TargetType>();
				if (sub_iteration() == 0) plot_demand_moes();
				//plot_reference_network_moes();
			}

			void plot_demand_moes()
			{
				//cout << endl << "Plot demand MOEs";
				typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;
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
				submission._x=iteration();
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
					float_map_itr itr = this->reference_activity_time_lost.find(iteration());
					if (itr != this->reference_activity_time_lost.end())
					{
						float ref_activity_time_lost = itr->second;
						submission._y = ref_activity_time_lost;
						_ref_activity_time_lost_cache.push_back(submission);
						ref_element.num_primitives = _ref_activity_time_lost_cache.size();
						ref_element.points = &_ref_activity_time_lost_cache.front();
						_activity_time_lost_layer->Push_Element<Regular_Element>((void*)&ref_element);
					}
				}
				
				//==============================================================
				// plot replanned activities
				//------------------------
				submission._y=_replanned_activities;
				_replanned_activity_count_cache.push_back(submission);
				element.num_primitives = _replanned_activity_count_cache.size();
				element.points = &_replanned_activity_count_cache.front();
				_replanned_activities_layer->Push_Element<Regular_Element>((void*)&element);

				//==============================================================
				// plot cancelled acts
				submission._y=_cancelled_activities;
				_cancelled_activities_cache.push_back(submission);
				element.num_primitives = _cancelled_activities_cache.size();
				element.points = &_cancelled_activities_cache.front();
				_cancelled_activities_layer->Push_Element<Regular_Element>((void*)&element);
				// plot reference cancelled acts
				//--------------------------
				if (this->_draw_reference)
				{
					float_map_itr itr = this->reference_cancelled_activities.find(iteration());
					if (itr != this->reference_cancelled_activities.end())
					{
						float ref_cancelled_acts = itr->second;
						submission._y = ref_cancelled_acts;
						_ref_cancelled_activities_cache.push_back(submission);
						ref_element.num_primitives = _ref_cancelled_activities_cache.size();
						ref_element.points = &_ref_cancelled_activities_cache.front();
						_activity_time_lost_layer->Push_Element<Regular_Element>((void*)&ref_element);
					}
				}


				//==============================================================
				// plot travel time distribution
				//-----------------------------------
				// Current model
				_ttime_distribution_cache.clear();
				std::vector<int> counts;
				int count =0;
				int total =0;
				for (int j=0; j < ttime_bins; j++)
				{
					int count = 0;
					for (int k=0; k < num_sim_threads(); k++) // collect value over all threads
					{
						count += ttime_distribution[k][j];
					}
					counts.push_back(count);
					total += count;
				}
				submission._x = 0;
				submission._y = 0;
				_ttime_distribution_cache.push_back(submission);	
				for (int j=0; j < ttime_bins; j++)
				{
					submission._x = ttime_interval_length*(j+1);
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
					std::vector_map_itr itr = this->reference_ttime_distribution.find(iteration());
					if (itr != this->reference_ttime_distribution.end())
					{
						for (int j=0; j < ttime_bins; j++)
						{
							count += itr->second[j];
						}
						submission._x = 0;
						submission._y = 0;
						_ref_ttime_distribution_cache.push_back(submission);	
						for (int j=0; j < ttime_bins; j++)
						{
							submission._x = ttime_interval_length*(j+1);
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
					if (j==1) continue;

					int count = 0;
					for (int k=0; k < num_sim_threads(); k++) // collect value over all threads
					{
						count += planned_acts[k][j];
					}
					submission._x = (float)j - 0.27;
					submission._y = 0;
					_generated_activity_distribution_cache.push_back(submission);
					submission._x = (float)j - 0.27;
					submission._y = (float)count;
					_generated_activity_distribution_cache.push_back(submission);	
					submission._x = (float)j -0.03;
					submission._y = (float)count;
					_generated_activity_distribution_cache.push_back(submission);	
					submission._x = (float)j -0.03;
					submission._y = 0;
					_generated_activity_distribution_cache.push_back(submission);
				}			
				element.num_primitives = _generated_activity_distribution_cache.size();
				element.points = &_generated_activity_distribution_cache.front();
				_activity_type_distribution_layer->Push_Element<Regular_Element>((void*)&element);
				//-------------------------------------------
				// next do executed activities
				_executed_activity_distribution_cache.clear();
				submission._x = 0;
				submission._y = 0;
				_executed_activity_distribution_cache.push_back(submission);	
				for (int j=0; j < 19; j++)
				{
					if (j==1) continue;

					int count = 0;
					for (int k=0; k < num_sim_threads(); k++) // collect value over all threads
					{
						count += executed_acts[k][j];
					}
					submission._x = (float)j - 0.25;
					submission._y = 0;
					_executed_activity_distribution_cache.push_back(submission);
					submission._x = (float)j - 0.25;
					submission._y = (float)count;
					_executed_activity_distribution_cache.push_back(submission);	
					submission._x = (float)j - 0.05;
					submission._y = (float)count;
					_executed_activity_distribution_cache.push_back(submission);	
					submission._x = (float)j - 0.05;
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
				//---------------------------------------------
				// plot reference activity generation data
				if (this->_draw_reference)
				{
					_ref_executed_activity_distribution_cache.clear();
					std::vector_map_itr itr = this->reference_executed_activity_distribution.find(iteration());
					if (itr != this->reference_executed_activity_distribution.end())
					{
						submission._x = 0;
						submission._y = 0;
						_ref_executed_activity_distribution_cache.push_back(submission);	
						for (int j=0; j < 19; j++)
						{
							float count = (float)itr->second[j];

							submission._x = (float)j + 0.05;
							submission._y = 0;
							_ref_executed_activity_distribution_cache.push_back(submission);
							submission._x = (float)j + 0.05;
							submission._y = (float)count;
							_ref_executed_activity_distribution_cache.push_back(submission);	
							submission._x = (float)j + 0.25;
							submission._y = (float)count;
							_ref_executed_activity_distribution_cache.push_back(submission);	
							submission._x = (float)j + 0.25;
							submission._y = 0;
							_ref_executed_activity_distribution_cache.push_back(submission);	
						}			
						Plot_Element ref_element_act;
						ref_element_act.color._r = 0;
						ref_element_act.color._g = 255;
						ref_element_act.color._b = 255;
						ref_element_act.color._a = 255;
						ref_element_act.num_primitives = _ref_executed_activity_distribution_cache.size();
						ref_element_act.points = &_ref_executed_activity_distribution_cache.front();
						_activity_type_distribution_layer->Push_Element<Regular_Element>((void*)&ref_element_act);
					}
					// replot the generated activities curve for comparison
					for (std::vector<Point_2D<MasterType>>::iterator vitr = _generated_activity_distribution_cache.begin(); vitr != _generated_activity_distribution_cache.end(); ++vitr)
					{
						submission._x = vitr->_x + 0.3;
						submission._y = vitr->_y;
						_ref_generated_activity_distribution_cache.push_back(submission);
					}
					Plot_Element ref_element_gen;
					ref_element_gen.color._r = 255;
					ref_element_gen.color._g = 0;
					ref_element_gen.color._b = 255;
					ref_element_gen.color._a = 255;
					ref_element_gen.num_primitives = _ref_generated_activity_distribution_cache.size();
					ref_element_gen.points = &_ref_generated_activity_distribution_cache.front();
					_activity_type_distribution_layer->Push_Element<Regular_Element>((void*)&ref_element_gen);
				}
			}
		};

	}
}
