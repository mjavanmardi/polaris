#pragma once
#include "User_Space_Includes.h"

namespace Network_Skimming_Components
{
	namespace Types
	{
		enum CALCULATION_STATUS
		{
			PROCESSING,
			COMPLETE
		};
		enum SUB_ITERATIONS
		{
			INITIALIZE = 19,
			PROCESS = 20,
			PATH_BUILDING = 21,
			UPDATE = 22
		};
	}

	namespace Concepts
	{
	} 
	
	namespace Prototypes
	{
		forward_declare_prototype struct Mode_Skim_Table_Prototype;
		forward_declare_prototype struct Skim_Table_Prototype;
		forward_declare_prototype struct Location_To_Zone_Map_Item_Prototype;

		prototype struct Network_Skimming_Prototype ADD_DEBUG_INFO
		{
			tag_as_prototype;

			//=============================================
			// Primary events
			//---------------------------------------------
			declare_feature_conditional(Skim_Table_Update_Conditional)
			{
				typedef Network_Skimming_Prototype<ComponentType, CallerType> _Skim_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_Skim_Interface* this_ptr=(_Skim_Interface*)_pthis;

				if (_sub_iteration == 0)
				{
					response.next._iteration = _iteration;
					response.next._sub_iteration = Types::SUB_ITERATIONS::INITIALIZE;
					response.result = false;
				}
				else if (_sub_iteration == Types::SUB_ITERATIONS::INITIALIZE)
				{
					response.next._iteration = _iteration;
					response.next._sub_iteration = Types::SUB_ITERATIONS::PROCESS;
					response.result = false;
				}
				else if (_sub_iteration == Types::SUB_ITERATIONS::PROCESS)
				{
					_pthis->Swap_Event((Event)&Process_Skim_Trees_Event<NULLTYPE>);
					response.next._iteration = _iteration;
					response.next._sub_iteration = Types::SUB_ITERATIONS::UPDATE;
					response.result = true;
				}
				else if (_sub_iteration == Types::SUB_ITERATIONS::UPDATE)
				{
					_pthis->Swap_Event((Event)&Update_Skim_Tables_Event<NULLTYPE>);
					response.next._iteration = Simulation_Time.template Future_Time<Simulation_Timestep_Increment,Simulation_Timestep_Increment>(this_ptr->template update_increment<Simulation_Timestep_Increment>());
					response.next._sub_iteration = 0;
					response.result = true;
				}
				else
				{
					response.next._iteration = Simulation_Time.template Future_Time<Simulation_Timestep_Increment,Simulation_Timestep_Increment>(this_ptr->template update_increment<Simulation_Timestep_Increment>());
					response.next._sub_iteration = 0;
					response.result = true;
				}
			}
			declare_feature_event(Process_Skim_Trees_Event)
			{
				cout<<endl<<endl<<"====================================================="<<endl<<"Updating Network Skims:"<<endl;
				typedef Network_Skimming_Prototype<ComponentType, CallerType> _Skim_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_Skim_Interface* this_ptr=(_Skim_Interface*)_pthis;
				this_ptr->template timer<Counter&>().Start();
			}
			declare_feature_event(Update_Skim_Tables_Event)
			{
				typedef Network_Skimming_Prototype<ComponentType, CallerType> _Skim_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_Skim_Interface* this_ptr=(_Skim_Interface*)_pthis;

				this_ptr->template Update_Skim_Tables<NULLTYPE>();
			}


			//=============================================
			// Primary data accessors
			//---------------------------------------------
			// links back to the network to be skimmed
			//feature_accessor(network_reference, check(ReturnValueType,Network_Components::Concepts::Is_Transportation_Network_Prototype), check(SetValueType,Network_Components::Concepts::Is_Transportation_Network_Prototype) || check(SetValueType,Network_Components::Concepts::Is_Transportation_Network_Prototype));
			feature_accessor(network_reference, none,none);
			// ids of the modes contained in the model, set in implementation
			feature_accessor(available_modes_container, none, none);
			// contains a map of skim_tables, one for each specified mode
			feature_accessor(mode_skim_table_container, none, none);
			// reference to the current, completed skim table from skim_tables_container, based on simulation time, where skim_table
			feature_accessor(current_skim_table, none, none);
			// time increment at which skim tables are updated - set in the initializer
			feature_accessor(update_increment,check(ReturnValueType,Basic_Units::Concepts::Is_Time_Value),check(SetValueType,Basic_Units::Concepts::Is_Time_Value));
			// scheduled time at which skim tables are updated - set in the initializer
			feature_accessor(scheduled_update_time,check(ReturnValueType,Basic_Units::Concepts::Is_Time_Value),check(SetValueType,Basic_Units::Concepts::Is_Time_Value));
			// number of nodes selected per zone used to estimate zonal travel times
			feature_accessor(nodes_per_zone,none,none);
			feature_accessor(origin_node_to_zone_map,none,none);
			feature_accessor(destination_node_to_zone_map,none,none);
			feature_accessor(zone_origins_count,none,none);
			feature_accessor(zone_destinations_count,none,none);
			feature_accessor(timer,none,none);
			feature_accessor(read_input,none,none);
			feature_accessor(write_output,none,none);
			feature_accessor(input_file,none,none);
			feature_accessor(output_file,none,none);
			feature_accessor(skim_fit_results_file,none,none);

			//=============================================
			// Primary function accessors - used to pass through to the specific skimm table based on time-key
			//---------------------------------------------
			feature_prototype void Initialize()
			{			
				// Call implementation Initializer - this sets the implementation specific data members
				this_component()->template Initialize<ComponentType,CallerType,TargetType>();

				// get network reference
				define_component_interface(network_itf,typename get_type_of(network_reference),Network_Components::Prototypes::Network_Prototype,ComponentType);
				network_itf* network = this->network_reference<network_itf*>();

				// create the references to network items and create the lists of origins/destination to route from/to
				define_container_and_value_interface(zones_itf,zone_itf,typename network_itf::get_type_of(zones_container),Associative_Container_Prototype,Zone_Components::Prototypes::Zone_Prototype,ComponentType);
				define_container_and_value_interface(locations_itf,location_itf,typename zone_itf::get_type_of(origin_activity_locations),Random_Access_Sequence_Prototype,Activity_Location_Components::Prototypes::Activity_Location_Prototype,ComponentType);
				define_container_and_value_interface(links_itf,link_itf,typename location_itf::get_type_of(origin_links),Random_Access_Sequence_Prototype,Link_Components::Prototypes::Link_Prototype,ComponentType);			
				define_container_and_value_interface(turns_itf,turn_itf,typename link_itf::get_type_of(outbound_turn_movements),Random_Access_Sequence_Prototype,Turn_Movement_Components::Prototypes::Movement_Prototype,ComponentType);
				define_container_and_value_interface(origin_map_itf,origin_item_itf,typename get_type_of(origin_node_to_zone_map), Containers::Associative_Container_Prototype,Prototypes::Location_To_Zone_Map_Item_Prototype,CallerType);
				define_container_and_value_interface(destination_map_itf,destination_item_itf,typename get_type_of(destination_node_to_zone_map), Containers::Associative_Container_Prototype,Prototypes::Location_To_Zone_Map_Item_Prototype,CallerType);
				define_simple_container_interface(zone_origins_itf,typename get_type_of(zone_origins_count), Containers::Associative_Container_Prototype,int,CallerType);
				define_simple_container_interface(zone_destinations_itf,typename get_type_of(zone_destinations_count), Containers::Associative_Container_Prototype,int,CallerType);
				origin_map_itf* origin_map = this->template origin_node_to_zone_map<origin_map_itf*>();
				destination_map_itf* destination_map = this->template destination_node_to_zone_map<destination_map_itf*>();
				zones_itf* zones_container = network->template zones_container<zones_itf*>();
				locations_itf& locations_container = network->template activity_locations_container<locations_itf&>();
				zone_origins_itf& zone_origins_count = this->template zone_origins_count<zone_origins_itf&>();
				zone_destinations_itf& zone_destinations_count = this->template zone_destinations_count<zone_destinations_itf&>();

				//=================================================================================================
				// Loop through zones, choose origin points to route from, and add to maps
				typename zones_itf::iterator itr;
				typename locations_itf::iterator loc_itr;
				for (itr= zones_container->begin();itr != zones_container->end(); ++itr)
				{
					zone_itf* orig_zone = itr->second;
					locations_itf& origin_locations = orig_zone->template origin_activity_locations<locations_itf&>();
					int num_locations = (int)origin_locations.size();
					zone_origins_count.insert(pair<long,int>(orig_zone->template internal_id<long>(),0));
					
					// Add all locations to list if less than the number required
					if (num_locations <= this->nodes_per_zone<int>())
					{
						for (int i=0; i<num_locations; i++)
						{
							location_itf* loc = origin_locations[i];
							if (!(loc->template Has_Valid_Origin_Link<bool>()) || origin_map->find(loc->template internal_id<long>()) != origin_map->end()) continue;

							origin_item_itf* orig_item = (origin_item_itf*)Allocate<typename get_type_of(origin_node_to_zone_map)::unqualified_value_type>();
							orig_item->template initialize<Target_Type<NULLTYPE,NULLTYPE,long,float>>(loc->template internal_id<long>(),orig_zone->template internal_id<long>(),1.0/this->template nodes_per_zone<float>());
					
							pair<long,origin_item_itf*> item = pair<long,origin_item_itf*>(loc->template internal_id<long>(),orig_item);
							origin_map->insert(item);
							zone_origins_count.find(orig_zone->template internal_id<long>())->second++;
						}
					}
					// otherwise choose randomly
					else
					{
						int num_successful = 0;
						// make nodes_per_zone attempts to pick origin locations
						for (int i=0; i<this->nodes_per_zone<int>(); i++)
						{
							// get interface to the location
							int rand_loc_index = (int)((GLOBALS::Uniform_RNG.Next_Rand<double>() - 0.0001) * (double)num_locations);
							location_itf* loc = origin_locations[rand_loc_index];

							// If the location does not have valid links or is already in the list, skip
							if (!(loc->template Has_Valid_Origin_Link<bool>()) || origin_map->find(loc->template internal_id<long>()) != origin_map->end()) continue;

							// Create a new node_to_zone_map item for later lookup
							origin_item_itf* orig_item = (origin_item_itf*)Allocate<typename get_type_of(origin_node_to_zone_map)::unqualified_value_type>();
							orig_item->template initialize<Target_Type<NULLTYPE,NULLTYPE,long,float>>(loc->template internal_id<long>(),orig_zone->template internal_id<long>(),1.0/this->template nodes_per_zone<float>());
					
							// Insert to the list, and update the origin node counts
							pair<long,origin_item_itf*> item = pair<long,origin_item_itf*>(loc->template internal_id<long>(),orig_item);
							origin_map->insert(item);
							zone_origins_count.find(orig_zone->template internal_id<long>())->second++;
						}
					}
					if (zone_origins_count.find(orig_zone->template internal_id<long>())->second == 0) THROW_EXCEPTION("Origin zone '" << orig_zone->template uuid<long>() << "' has no valid activity locations, can not skim to this zone.");
				}

				//=================================================================================================
				// Loop through zones, choose destination points to route to, and add to maps
				for (itr= zones_container->begin();itr != zones_container->end(); ++itr)
				{
					zone_itf* orig_zone = itr->second;
					locations_itf& origin_locations = orig_zone->template destination_activity_locations<locations_itf&>();
					int num_locations = (int)origin_locations.size();
					zone_destinations_count.insert(pair<long,int>(orig_zone->template internal_id<long>(),0));
					
					// Add all locations to list if less than the number required
					if (num_locations <= this->nodes_per_zone<int>())
					{
						for (int i=0; i<num_locations; i++)
						{
							location_itf* loc = origin_locations[i];
							if (!loc->template Has_Valid_Destination_Link<bool>() || destination_map->find(loc->template internal_id<long>()) != destination_map->end()) continue;

							origin_item_itf* orig_item = (origin_item_itf*)Allocate<typename get_type_of(origin_node_to_zone_map)::unqualified_value_type>();
							orig_item->template initialize<Target_Type<NULLTYPE,NULLTYPE,long,float>>(loc->template internal_id<long>(),orig_zone->template internal_id<long>(),1.0/this->template nodes_per_zone<float>());
					
							pair<long,origin_item_itf*> item = pair<long,origin_item_itf*>(loc->template internal_id<long>(),orig_item);
							destination_map->insert(item);
							zone_destinations_count.find(orig_zone->template internal_id<long>())->second++;
						}
					}
					// otherwise choose randomly
					else
					{
						int num_successful = 0;
						// make nodes_per_zone attempts to pick origin locations
						for (int i=0; i<this->nodes_per_zone<int>(); i++)
						{
							// get interface to the location
							int rand_loc_index = (int)((GLOBALS::Uniform_RNG.Next_Rand<double>() - 0.0001) * (double)num_locations);
							location_itf* loc = origin_locations[rand_loc_index];

							// If the location does not have valid links or is already in the list, skip
							if (!loc->template Has_Valid_Destination_Link<bool>() || destination_map->find(loc->template internal_id<long>()) != destination_map->end()) continue;

							// Create a new node_to_zone_map item for later lookup
							origin_item_itf* orig_item = (origin_item_itf*)Allocate<typename get_type_of(origin_node_to_zone_map)::unqualified_value_type>();
							orig_item->template initialize<Target_Type<NULLTYPE,NULLTYPE,long,float>>(loc->template internal_id<long>(),orig_zone->template internal_id<long>(),1.0/this->template nodes_per_zone<float>());
					
							// Insert to the list, and update the origin node counts
							pair<long,origin_item_itf*> item = pair<long,origin_item_itf*>(loc->template internal_id<long>(),orig_item);
							destination_map->insert(item);
							zone_destinations_count.find(orig_zone->template internal_id<long>())->second++;
						}
					}
					if (zone_destinations_count.find(orig_zone->template internal_id<long>())->second == 0) THROW_EXCEPTION("Origin zone '" << orig_zone->template uuid<long>() << "' has no valid activity locations, can not skim to this zone.");
				}


				////TODO: Remove when finished - temporary display of skim O/D locations
				//origin_map_itf::iterator origin_itr = origin_map->begin();
				//destination_map_itf::iterator dest_itr = destination_map->begin();
				//cout << endl << "Origin INFO (loc,zone,loc_count)" << endl;
				//for (;origin_itr != origin_map->end();++origin_itr)
				//{
				//	int loc_index = origin_itr->first;
				//	long zone_index = origin_itr->second->zone_index<long>();

				//	cout << locations_container[loc_index]->uuid<int>() << ","<<locations_container[loc_index]->zone<zone_itf*>()->uuid<int>()<<","<<zone_origins_count.find(zone_index)->second<<endl;
				//}
				//cout << endl << "Destination INFO (loc,zone,loc_count)" << endl;
				//for (;dest_itr != destination_map->end();++dest_itr)
				//{
				//	int loc_index = dest_itr->first;
				//	long zone_index = dest_itr->second->zone_index<long>();
				//	cout << locations_container[loc_index]->uuid<int>() << ","<<locations_container[loc_index]->zone<zone_itf*>()->uuid<int>()<<","<<zone_destinations_count.find(zone_index)->second<<endl;
				//}



				//========================================================================
				// Create individual mode-skim tables for each available mode
				define_container_and_value_interface(_skim_container_itf,_skim_itf,typename get_type_of(mode_skim_table_container),Containers::Associative_Container_Prototype,Prototypes::Mode_Skim_Table_Prototype,ComponentType);				
				define_simple_container_interface(_available_modes_itf,typename get_type_of(available_modes_container),Containers::Back_Insertion_Sequence_Prototype,typename get_type_of(available_modes_container)::unqualified_value_type,CallerType);
				_available_modes_itf* modes = this->template available_modes_container<_available_modes_itf*>();
				typename _available_modes_itf::iterator mode_itr = modes->begin();
				for (; mode_itr != modes->end(); ++mode_itr)
				{
					_skim_itf* skim = (_skim_itf*)Allocate<typename get_type_of(mode_skim_table_container)::unqualified_value_type>();
					skim->skim_reference(this);
					skim->network_reference(network);
					skim->mode_id(*mode_itr);
					skim->template Initialize<NULLTYPE>();

					pair<long,_skim_itf*> item = pair<long,_skim_itf*>(skim->template mode_id<long>(), skim);
					//this->template mode_skim_table_container<_skim_container_itf*>()->insert(pair<long,_skim_itf*>(skim->template mode_id<long>(), skim));
					this->template mode_skim_table_container<_skim_container_itf*>()->insert(item);
				}

				// Load the skim updating event, which recalculates network skims at every Update interval (set in implementation)
				load_event(ComponentType,Skim_Table_Update_Conditional,Update_Skim_Tables_Event,0,Types::SUB_ITERATIONS::INITIALIZE,NULLTYPE);
			}
			feature_prototype void Initialize(TargetType network_reference, requires(check_as_given(TargetType, is_pointer) && check(TargetType, Network_Components::Concepts::Is_Transportation_Network_Prototype)))
			{
				// set the network references
				this->template network_reference<TargetType>(network_reference);
				
				this->template Initialize<NULLTYPE>();
			}			
			feature_prototype void Initialize(TargetType network_reference, requires(!check_as_given(TargetType, is_pointer) || !check(TargetType, Network_Components::Concepts::Is_Transportation_Network_Prototype)))
			{
				assert_check_as_given(TargetType, is_pointer,"TargetType is not a pointer" );
				assert_check(TargetType, Network_Components::Concepts::Is_Transportation_Network_Prototype, "TargetType is not a valid Transportation_Network interface");
				assert_sub_check(TargetType, Network_Components::Concepts::Is_Transportation_Network_Prototype, is_basic_network, "TargetType is not a basic network");
				assert_sub_check(TargetType, Network_Components::Concepts::Is_Transportation_Network_Prototype, has_turns, "TargetType does not have turns accessor");
				assert_sub_check(TargetType, Network_Components::Concepts::Is_Transportation_Network_Prototype, has_locations, "TargetType does not have locations accessor");
				assert_sub_check(TargetType, Network_Components::Concepts::Is_Transportation_Network_Prototype, has_zones, "TargetType does not have zones accessor");
			}			
			feature_prototype bool Update_Skim_Tables()
			{
				define_container_and_value_interface(_skim_container_itf, _skim_itf,typename get_type_of(mode_skim_table_container),Containers::Associative_Container_Prototype,Prototypes::Mode_Skim_Table_Prototype,CallerType);
				_skim_container_itf* skim = this->template mode_skim_table_container<_skim_container_itf*>();

				// Update each modal_skim
				for (typename _skim_container_itf::iterator itr = skim->begin(); itr != skim->end(); ++itr)
				{
					itr->second->template Update_LOS<NULLTYPE>();
				}

				

				// Output network skims
				for (typename _skim_container_itf::iterator itr = skim->begin(); itr != skim->end(); ++itr)
				{
					if (this->write_output<bool>())
					{
						itr->second->template Write_LOS<NULLTYPE>();
					}
				}


				// Network Skim timer
				cout << endl<<"Network Skimming run-time: " << this->template timer<Counter&>().Stop();
				return true;
			}

			//=============================================
			// Primary function accessors - used to pass through to the specific skimm table based on time-key
			//---------------------------------------------
			// This returns the travel time based on the current simulation time
			feature_prototype typename TargetType::ReturnType Get_LOS(typename TargetType::ParamType Origin_Zone_ID, typename TargetType::ParamType Destination_Zone_ID, typename TargetType::Param2Type Mode_Indicator, requires(check(typename TargetType::ReturnType, Basic_Units::Concepts::Is_Time_Value)))
			{
				// create the references to network items and create the lists of origins/destination to route from/to
				//define_component_interface(network_itf,typename get_type_of(network_reference),Network_Components::Prototypes::Network_Prototype,ComponentType);
				//define_container_and_value_interface(zones_itf,zone_itf,typename network_itf::get_type_of(zones_container),Associative_Container_Prototype,Zone_Components::Prototypes::Zone_Prototype,ComponentType);
				//network_itf* network = this->template network_reference<network_itf*>();
				//zones_itf* zones = network->template zones_container<zones_itf*>();
				//typename zones_itf::iterator zone_itr;

				//// convert the ids to indices
				//zone_itf *orig_zone, *dest_zone;
				//if ((zone_itr = zones->find(Origin_Zone_ID)) != zones->end()){ orig_zone = zone_itr->second;}
				//else THROW_EXCEPTION("ERROR, origin zone id: " << Origin_Zone_ID << " was not found.");
				//if ((zone_itr = zones->find(Destination_Zone_ID)) != zones->end()){ dest_zone = zone_itr->second;}
				//else THROW_EXCEPTION("ERROR, destination zone id: " << Destination_Zone_ID << " was not found.");
				
				// call the get los function
				return this->template Get_LOS<Target_Type<NULLTYPE,typename TargetType::ReturnType, typename TargetType::ParamType, typename TargetType::Param2Type, Simulation_Timestep_Increment>>(Origin_Zone_ID, Destination_Zone_ID, Mode_Indicator, Simulation_Time.template Current_Time<Simulation_Timestep_Increment>());
			}
			// This returns the travel time during a specific time interval
			feature_prototype typename TargetType::ReturnType Get_LOS(typename TargetType::ParamType Origin_Zone_ID, typename TargetType::ParamType Destination_Zone_ID, typename TargetType::Param2Type Mode_Indicator, typename TargetType::Param3Type Start_Time, requires(check(typename TargetType::ReturnType, Basic_Units::Concepts::Is_Time_Value)))
			{
				define_container_and_value_interface(_skim_container_itf, _skim_itf,typename get_type_of(mode_skim_table_container),Containers::Associative_Container_Prototype,Prototypes::Mode_Skim_Table_Prototype,CallerType);
				_skim_container_itf* skim = this->mode_skim_table_container<_skim_container_itf*>();
				typename _skim_container_itf::iterator itr;

				// create the references to network items and create the lists of origins/destination to route from/to
				define_component_interface(network_itf,typename get_type_of(network_reference),Network_Components::Prototypes::Network_Prototype,ComponentType);
				define_container_and_value_interface(zones_itf,zone_itf,typename network_itf::get_type_of(zones_container),Associative_Container_Prototype,Zone_Components::Prototypes::Zone_Prototype,ComponentType);
				network_itf* network = this->template network_reference<network_itf*>();
				zones_itf* zones = network->template zones_container<zones_itf*>();
				typename zones_itf::iterator zone_itr;

				// convert the ids to indices
				zone_itf *orig_zone, *dest_zone;
				if ((zone_itr = zones->find(Origin_Zone_ID)) != zones->end()){ orig_zone = zone_itr->second;}
				else THROW_EXCEPTION("ERROR, origin zone id: " << Origin_Zone_ID << " was not found.");
				if ((zone_itr = zones->find(Destination_Zone_ID)) != zones->end()){ dest_zone = zone_itr->second;}
				else THROW_EXCEPTION("ERROR, destination zone id: " << Destination_Zone_ID << " was not found.");

				// throw error if specified mode is not available in skim table
				itr = skim->find(Mode_Indicator);
				//if ((itr = skim->find(Mode_Indicator)) == skim->end()) assert(false);

				// call mode_skim_tables get los functionality
				return itr->second->template Get_LOS<Target_Type<NULLTYPE,typename TargetType::ReturnType, typename TargetType::ParamType, typename TargetType::Param3Type>>(orig_zone->template internal_id<int>(), dest_zone->template internal_id<int>(),Start_Time);
			}

			feature_prototype static void Convert_Binary_Skimfile_To_CSV(string infilename, string outfilename)
			{
				File_IO::Binary_File_Reader infile;
				infile.Open(infilename);
				
				ofstream outfile;
				outfile.open(outfilename);

				//===========================================================================
				// Read Header
				int num_modes, num_zones, update_increment;
				infile.Read_Value<int>(num_modes);
				infile.Read_Value<int>(num_zones);
				infile.Read_Value<int>(update_increment);
				
				//===========================================================================
				// create the skim_table time periods, for basic create only a single time period skim_table
				for (Simulation_Timestep_Increment start = 0; start < GLOBALS::Time_Converter.template Convert_Value<Target_Type<NT,Simulation_Timestep_Increment,Time_Hours>>(24.0); start = start + update_increment)
				{		
					float* data = new float[num_zones*num_zones];



					infile.Read_Array<float>(data, num_zones*num_zones);


					vector<float> temp(data,data+num_zones*num_zones);

					
					outfile << "Skim table for time period starting at: " << start<<endl;

					for (int i=0; i< num_zones; i++)
					{
						for (int j=0; j< num_zones; j++)
						{
							outfile << data[i*(num_zones)+j] << ",";
						}
						outfile << endl;
					}
					cout <<endl<< "finished period "<<start<<endl;
					outfile << endl;
				}

				infile.Close();
				outfile.close();
			}
		};

		prototype struct Mode_Skim_Table_Prototype ADD_DEBUG_INFO
		{
			tag_as_prototype;

			// Associative Container of skim matrices, keyed on Mode Indicator values
			feature_accessor(skims_by_time_container,none,none);
			// links back to the network to be skimmed
			feature_accessor(network_reference, none, none);
			// links back to the skimming faculty
			feature_accessor(skim_reference, none, none);
			// number of nodes selected per zone used to estimate zonal travel times
			feature_accessor(mode_id,none,none);

			// Associative Container of skim matrices, keyed on Mode Indicator values
			feature_accessor(path_trees_container,none,none);
			// number of nodes selected per zone used to estimate zonal travel times
			feature_accessor(nodes_per_zone,none,none);

			//=============================================
			// Primary function accessors - used to calculate/return LOS values for OD pairs
			//---------------------------------------------
			feature_prototype void Initialize()
			{
				this_component()->template Initialize<ComponentType,CallerType,TargetType>();
			}				
			
			feature_prototype bool Update_LOS()
			{
				define_container_and_value_interface(_skim_container_itf, _skim_itf,typename get_type_of(skims_by_time_container),Containers::Random_Access_Sequence_Prototype,Prototypes::Skim_Table_Prototype,CallerType);
				_skim_container_itf* skim = this->template skims_by_time_container<_skim_container_itf*>();

				for (typename _skim_container_itf::iterator itr = skim->begin(); itr != skim->end(); ++itr)
				{
					// update the skim for current time period
					if ((*itr)->template start_time<Simulation_Timestep_Increment>() >= _iteration) 
					{
						(*itr)->template Update_LOS<NULLTYPE>();

						// If this is the first iteration and skims were not read from input initialize all of the time periods, otherwise break
						typedef Scenario_Components::Prototypes::Scenario_Prototype<typename ComponentType::Master_Type::scenario_type> _Scenario_Interface;
						_Scenario_Interface* scenario = (_Scenario_Interface*)_global_scenario;
						if (_iteration > 0 || scenario->template read_skim_tables<bool>())	break;
					}
				}
				return true;
			}
			feature_prototype void Write_LOS()
			{
				define_container_and_value_interface(_skim_container_itf, _skim_itf,typename get_type_of(skims_by_time_container),Containers::Random_Access_Sequence_Prototype,Prototypes::Skim_Table_Prototype,CallerType);
				_skim_container_itf* skim = this->skims_by_time_container<_skim_container_itf*>();

				for (typename _skim_container_itf::iterator itr = skim->begin(); itr != skim->end(); ++itr)
				{
					if ((*itr)->template start_time<Simulation_Timestep_Increment>() >= _iteration) 
					{
						(*itr)->template Write_LOS<NULLTYPE>();
						break;
					}
				}
			}
			feature_prototype typename TargetType::ReturnType Get_LOS(typename TargetType::ParamType Origin_ID, typename TargetType::ParamType Destination_ID, typename TargetType::Param2Type Time)
			{
				define_container_and_value_interface(_skim_container_itf, _skim_itf,typename get_type_of(skims_by_time_container),Containers::Random_Access_Sequence_Prototype,Prototypes::Skim_Table_Prototype,CallerType);
				_skim_container_itf* skims = this->skims_by_time_container<_skim_container_itf*>();
				typename _skim_container_itf::iterator itr = skims->begin();
				_skim_itf* skim_table;

				// get only the HH:MM:SS portion of requested time if Time > 1 day
				int days = ((int)(GLOBALS::Time_Converter.Convert_Value<Target_Type<NT,Time_Hours,typename TargetType::Param2Type>>(Time))/24);
				typename TargetType::Param2Type rounded = GLOBALS::Time_Converter.Convert_Value<Target_Type<NT,typename TargetType::Param2Type,Time_Hours>>((float)days * 24.0);
				typename TargetType::Param2Type remain = Time - rounded;
				
				for (; itr != skims->end(); ++itr)
				{
					skim_table = *itr;
					if (skim_table->template end_time<typename TargetType::Param2Type>() > remain) return skim_table->template Get_LOS<Target_Type<NULLTYPE,typename TargetType::ReturnType,typename TargetType::ParamType>>(Origin_ID,Destination_ID);
				}
				// if the code gets here, then the requested time does not fall within any skim_table time period
				cout << endl << "Get LOS failure: " <<"origin: " << Origin_ID <<", destination: " << Destination_ID<<", time: "  << Time<<endl;
				assert(false);
				return false;
			}
		};

		prototype struct Skim_Table_Prototype ADD_DEBUG_INFO
		{
			tag_as_prototype;

			// Associative Container of skim matrices, keyed on Mode Indicator values
			feature_accessor(skim_table,none,none);
			
			// links back to the network to be skimmed
			feature_accessor(network_reference, none, none);
			// links back to the skimming faculty
			feature_accessor(skim_reference, none, none);
			// links back to the skim-by-mode table
			feature_accessor(mode_skim_reference, none, none);

			// simulated day start and end times represented by skim_table
			feature_accessor(start_time,none,none);
			feature_accessor(end_time,none,none);

			feature_accessor(weighted_deviation,none,none);
			feature_accessor(max_deviation,none,none);

			//=============================================
			// Primary function accessors - used to calculate/return LOS values for OD pairs
			//---------------------------------------------
			feature_prototype void Initialize()
			{
				//cout << endl << "Initializing skim table at iteration - " << _iteration;


				// call implementation initializer
				this_component()->template Initialize<ComponentType,CallerType,TargetType>();

				// set the network reference
				define_simple_container_interface(skim_table_itf,typename get_type_of(skim_table),Containers::Multidimensional_Random_Access_Array_Prototype,typename get_type_of(skim_table)::unqualified_value_type,ComponentType);
				define_component_interface(network_itf,typename get_type_of(network_reference),Network_Components::Prototypes::Network_Prototype,ComponentType);
				define_component_interface(skimmer_itf,typename get_type_of(skim_reference),Prototypes::Network_Skimming_Prototype,ComponentType);
				network_itf* network = this->network_reference<network_itf*>();
				skimmer_itf* skim = this->skim_reference<skimmer_itf*>();
				
				// create the LOS container
				define_container_and_value_interface(zones_itf,zone_itf,typename network_itf::get_type_of(zones_container),Associative_Container_Prototype,Zone_Components::Prototypes::Zone_Prototype,ComponentType);
				define_container_and_value_interface(locations_itf,location_itf,typename zone_itf::get_type_of(origin_activity_locations),Random_Access_Sequence_Prototype,Activity_Location_Components::Prototypes::Activity_Location_Prototype,ComponentType);
				define_container_and_value_interface(links_itf,link_itf,typename location_itf::get_type_of(origin_links),Random_Access_Sequence_Prototype,Activity_Location_Components::Prototypes::Activity_Location_Prototype,ComponentType);
				define_container_and_value_interface(origin_map_itf,origin_item_itf,typename skimmer_itf::get_type_of(origin_node_to_zone_map), Containers::Associative_Container_Prototype,Prototypes::Location_To_Zone_Map_Item_Prototype,CallerType);
				//define_container_and_value_interface(destination_map_itf,destination_item_itf,skimmer_itf::get_type_of(destination_node_to_zone_map), Containers::Associative_Container_Prototype,Prototypes::Location_To_Zone_Map_Item_Prototype,CallerType);
				origin_map_itf* origin_map = skim->template origin_node_to_zone_map<origin_map_itf*>();
				//destination_map_itf* destination_map = skim->destination_node_to_zone_map<destination_map_itf*>();		
				zones_itf* zones_container = network->template zones_container<zones_itf*>();
				typename origin_map_itf::iterator orig_itr = origin_map->begin();
				//destination_map_itf::iterator dest_itr = destination_map->begin();
				locations_itf* activity_locations = network->template activity_locations_container<locations_itf*>();
		
				// create the LOS matrix
				typedef matrix<float>::size_type size_t;
				skim_table_itf* los = this->skim_table<skim_table_itf*>();
				los->resize(pair<size_t,size_t>((size_t)zones_container->size(),(size_t)zones_container->size()), 0);
			}			

			feature_prototype void Initialize(TargetType initial_data, requires(check(TargetType, is_arithmetic) && check_as_given(TargetType, is_pointer)))
			{
				//cout << endl << "Initializing skim table at iteration - " << _iteration;


				// call implementation initializer
				this_component()->template Initialize<ComponentType,CallerType,TargetType>();

				// set the network reference
				define_simple_container_interface(skim_table_itf,typename get_type_of(skim_table),Containers::Multidimensional_Random_Access_Array_Prototype,typename get_type_of(skim_table)::unqualified_value_type,ComponentType);
				define_component_interface(network_itf,typename get_type_of(network_reference),Network_Components::Prototypes::Network_Prototype,ComponentType);
				define_component_interface(skimmer_itf,typename get_type_of(skim_reference),Prototypes::Network_Skimming_Prototype,ComponentType);
				network_itf* network = this->network_reference<network_itf*>();
				skimmer_itf* skim = this->skim_reference<skimmer_itf*>();
				
				// create the LOS container
				define_container_and_value_interface(zones_itf,zone_itf,typename network_itf::get_type_of(zones_container),Associative_Container_Prototype,Zone_Components::Prototypes::Zone_Prototype,ComponentType);
				define_container_and_value_interface(locations_itf,location_itf,typename zone_itf::get_type_of(origin_activity_locations),Random_Access_Sequence_Prototype,Activity_Location_Components::Prototypes::Activity_Location_Prototype,ComponentType);
				define_container_and_value_interface(links_itf,link_itf,typename location_itf::get_type_of(origin_links),Random_Access_Sequence_Prototype,Activity_Location_Components::Prototypes::Activity_Location_Prototype,ComponentType);
				define_container_and_value_interface(origin_map_itf,origin_item_itf,typename skimmer_itf::get_type_of(origin_node_to_zone_map), Containers::Associative_Container_Prototype,Prototypes::Location_To_Zone_Map_Item_Prototype,CallerType);
				//define_container_and_value_interface(destination_map_itf,destination_item_itf,skimmer_itf::get_type_of(destination_node_to_zone_map), Containers::Associative_Container_Prototype,Prototypes::Location_To_Zone_Map_Item_Prototype,CallerType);
				origin_map_itf* origin_map = skim->template origin_node_to_zone_map<origin_map_itf*>();
				//destination_map_itf* destination_map = skim->destination_node_to_zone_map<destination_map_itf*>();		
				zones_itf* zones_container = network->template zones_container<zones_itf*>();
				typename origin_map_itf::iterator orig_itr = origin_map->begin();
				//destination_map_itf::iterator dest_itr = destination_map->begin();
				locations_itf* activity_locations = network->template activity_locations_container<locations_itf*>();
		
				// create the LOS matrix
				typedef matrix<float>::size_type size_t;
				skim_table_itf* los = this->skim_table<skim_table_itf*>();
				los->Copy(pair<size_t,size_t>((size_t)zones_container->size(),(size_t)zones_container->size()), initial_data);
			}	

			feature_prototype bool Process_Skims()
			{
			}

			feature_prototype bool Update_LOS()
			{
				// get the network and network element references
				define_simple_container_interface(skim_table_itf,typename get_type_of(skim_table),Containers::Multidimensional_Random_Access_Array_Prototype,typename get_type_of(skim_table)::unqualified_value_type,ComponentType);
				define_component_interface(network_itf,typename get_type_of(network_reference),Network_Components::Prototypes::Network_Prototype,ComponentType);
				define_component_interface(skimmer_itf,typename get_type_of(skim_reference),Prototypes::Network_Skimming_Prototype,ComponentType);
				define_simple_container_interface(zone_location_count_itf,typename skimmer_itf::get_type_of(zone_origins_count),Containers::Associative_Container_Prototype,int,ComponentType);
				define_component_interface(mode_skimmer_itf,typename get_type_of(mode_skim_reference),Prototypes::Mode_Skim_Table_Prototype,ComponentType);
				define_container_and_value_interface(locations_itf,location_itf,typename network_itf::get_type_of(activity_locations_container),Random_Access_Sequence_Prototype,Activity_Location_Components::Prototypes::Activity_Location_Prototype,ComponentType);
				/*define_container_and_value_interface(destinations_itf,destination_itf,zone_itf::get_type_of(destination_activity_locations),Random_Access_Sequence_Prototype,Link_Components::Prototypes::Link_Prototype,ComponentType);*/
				define_container_and_value_interface(links_itf,link_itf,typename location_itf::get_type_of(origin_links),Random_Access_Sequence_Prototype,Activity_Location_Components::Prototypes::Activity_Location_Prototype,ComponentType);
				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename ComponentType::Master_Type::scenario_type> _Scenario_Interface;
				_Scenario_Interface* scenario = (_Scenario_Interface*)_global_scenario;

				network_itf* network = this->network_reference<network_itf*>();
				skimmer_itf* skim = this->skim_reference<skimmer_itf*>();
				mode_skimmer_itf* mode_skim = this->mode_skim_reference<mode_skimmer_itf*>();
				locations_itf* activity_locations = network->template activity_locations_container<locations_itf*>();
				
				// origin to zone / destination to zone mappings
				define_container_and_value_interface(origin_map_itf,origin_item_itf,typename skimmer_itf::get_type_of(origin_node_to_zone_map), Containers::Associative_Container_Prototype,Prototypes::Location_To_Zone_Map_Item_Prototype,CallerType);
				define_container_and_value_interface(destination_map_itf,destination_item_itf,typename skimmer_itf::get_type_of(destination_node_to_zone_map), Containers::Associative_Container_Prototype,Prototypes::Location_To_Zone_Map_Item_Prototype,CallerType);
				origin_map_itf* origin_map = skim->template origin_node_to_zone_map<origin_map_itf*>();
				destination_map_itf* destination_map = skim->template destination_node_to_zone_map<destination_map_itf*>();		
				typename origin_map_itf::iterator orig_itr = origin_map->begin();
				typename destination_map_itf::iterator dest_itr = destination_map->begin();
				typename zone_location_count_itf::iterator zone_origin_count_itr;
				typename zone_location_count_itf::iterator zone_destination_count_itr;

				// get reference to the routers used to create path-trees from each origin
				define_container_and_value_interface(path_trees_itf,path_tree_itf,typename mode_skimmer_itf::get_type_of(path_trees_container),Associative_Container_Prototype,Routing_Components::Prototypes::Routing_Prototype,ComponentType);
				path_trees_itf* trees_container = mode_skim->template path_trees_container<path_trees_itf*>();
				typename path_trees_itf::iterator tree_itr = trees_container->begin();	
					
				// reference to the LOS skim table and its iterator	
				skim_table_itf* los = this->skim_table<skim_table_itf*>();
				matrix<float>* los_ptr = (matrix<float>*)los;
				typedef typename skim_table_itf::size_type size_t;
				typename skim_table_itf::iterator skim_table_itr = los->begin();
				matrix<float> los_old;
				if (scenario->template read_skim_tables<bool>()) los_old.Copy(*los_ptr);
				los->resize(los->dimensions(),0);

				// fit characteristics initialized
				float sum_of_deviation=0.0;
				float sum_of_weight = 0.0;
				float max_dev = 0.0;

				// loop through each tree router, extract travel time info and place into los_skim
				for (;tree_itr != trees_container->end(); ++tree_itr)
				{
					long orig = tree_itr->first;
					path_tree_itf* tree = tree_itr->second;

					// find the origin reference in origin map and get the zone index of the origin
					if ((orig_itr = origin_map->find(orig)) == origin_map->end()) assert(false);
					long orig_zone_index = orig_itr->second->template zone_index<long>();
					

					// search each link for required links from destination_map and update LOS table with the results
					for (dest_itr = destination_map->begin(); dest_itr !=destination_map->end(); ++dest_itr)
					{
						destination_item_itf* dest = dest_itr->second;
						long dest_node_index = dest->template loc_index<long>();
						location_itf* dest_node = activity_locations->at(dest_node_index);
						long dest_link_index = (*(dest_node->template destination_links<links_itf*>()->begin()))->template internal_id<long>();
						long dest_link_id = (*(dest_node->template destination_links<links_itf*>()->begin()))->template uuid<long>();
						long dest_zone_index = dest->template zone_index<long>();
						float time = tree->template Get_Tree_Results_For_Destination<typename skimmer_itf::Component_Type::Stored_Time_Type>(dest_link_index);
						link_itf* dest_link = network->template links_container<links_itf&>()[dest_link_index];
						
						// calculate weight based onnumber of routed od pairs
						zone_origin_count_itr = skim->template zone_origins_count<zone_location_count_itf&>().find(orig_zone_index);
						zone_destination_count_itr = skim->template zone_destinations_count<zone_location_count_itf&>().find(dest_zone_index);

						float weight = 1.0 / ((float)zone_origin_count_itr->second * (float)zone_destination_count_itr->second);

						if (time > 1440)
						{
							typename links_itf::iterator link_itr = dest_node->template destination_links<links_itf*>()->begin();
							for (;link_itr != dest_node->template destination_links<links_itf*>()->end(); ++link_itr)
							{
								THROW_WARNING("Skimming error, destination ID '" << dest_node->uuid<int>() << "', is inaccessible from zone index '" << orig_zone_index <<"' .  Destination link ID: "  << (*link_itr)->uuid<long>()<<". Check network for proper connectivity." <<endl);
							}
						}

						if (dest_zone_index == orig_zone_index) time = GLOBALS::Time_Converter.Convert_Value<Target_Type<NT,typename skimmer_itf::Component_Type::Stored_Time_Type,Time_Minutes>>(2.0);
						if (scenario->template read_skim_tables<bool>()) 
						{
							float time_old = los_old[pair<size_t,size_t>(orig_zone_index,dest_zone_index)];
							sum_of_deviation += (time-time_old);
							sum_of_weight += time_old;
							float dev = time_old > 0.0 ? (time-time_old)/time_old : 0.0;
							if (dev > max_dev) max_dev = dev;
							(*los)[pair<size_t,size_t>(orig_zone_index,dest_zone_index)] += (0.75 * time+ 0.25 * time_old)*weight;

							//if (_iteration > 28000) cout << orig_zone_index <<","<< dest_zone_index <<","<< time <<","<< time_old <<","<< weight<<","<<endl; 
						}
						else
							(*los)[pair<size_t,size_t>(orig_zone_index,dest_zone_index)] += time*weight;
					}
				}
				if (scenario->template read_skim_tables<bool>()) 
				{
					this->weighted_deviation<float>(sum_of_deviation/sum_of_weight);
					this->max_deviation<float>(max_dev);

					stringstream outline("");
					outline<<_iteration<<","<<this->weighted_deviation<float>()<<","<<this->max_deviation<float>();
					File_IO::File_Writer& out_file = skim->template skim_fit_results_file<File_IO::File_Writer&>();
					out_file.Write_Line(outline);
					cout << "W.A.A.P.D from previous skim: " << this->weighted_deviation<float>()<<endl;
					cout << "Maximum deviation from previous: " << this->max_deviation<float>()<<endl;
				}
				los_old.clear();

				return true;
			}

			feature_prototype void Write_LOS()
			{
				define_simple_container_interface(skim_table_itf,typename get_type_of(skim_table),Containers::Multidimensional_Random_Access_Array_Prototype,typename get_type_of(skim_table)::unqualified_value_type,ComponentType);
				define_component_interface(network_itf,typename get_type_of(network_reference),Network_Components::Prototypes::Network_Prototype,ComponentType);
				define_container_and_value_interface(zones_itf,zone_itf,typename network_itf::get_type_of(zones_container),Associative_Container_Prototype,Zone_Components::Prototypes::Zone_Prototype,ComponentType);
				define_component_interface(skimmer_itf,typename get_type_of(skim_reference),Prototypes::Network_Skimming_Prototype,ComponentType);
				define_component_interface(mode_skimmer_itf,typename get_type_of(mode_skim_reference),Prototypes::Mode_Skim_Table_Prototype,ComponentType);
				define_simple_container_interface(skim_table_itf,typename get_type_of(skim_table),Containers::Multidimensional_Random_Access_Array_Prototype,typename get_type_of(skim_table)::unqualified_value_type,ComponentType);
				
				skimmer_itf* skim = this->skim_reference<skimmer_itf*>();
				network_itf* network = this->network_reference<network_itf*>();
				zones_itf* zones = network->template zones_container<zones_itf*>();

				File_IO::Binary_File_Writer& bw = skim->template output_file<File_IO::Binary_File_Writer&>();
				
				skim_table_itf* los = this->skim_table<skim_table_itf*>();


				vector<float> temp(los->get_data_pointer(), los->get_data_pointer() + (int)(zones->size() * zones->size()));


				bw.WriteArray<float>(los->get_data_pointer(), (int)(zones->size() * zones->size()));
				

				//ofstream outfile;
				//stringstream filename;
				//filename <<"los_file_hour_"<< Simulation_Time.template Current_Time<Time_Hours>() << ".xls";
				//outfile.open(filename.str().c_str(),ios_base::out);
				//
				//outfile << endl <<endl << "---------------------------------------------------";
				//outfile << endl << "LOS at hour " << Simulation_Time.template Current_Time<Time_Hours>();
				//los->write(outfile);
				//outfile << endl << "---------------------------------------------------"<<endl;

				//outfile.close();
			}

			feature_prototype typename TargetType::ReturnType Get_LOS(typename TargetType::ParamType Origin_ID, typename TargetType::ParamType Destination_ID)
			{
				define_simple_container_interface(skim_table_itf,typename get_type_of(skim_table),Containers::Multidimensional_Random_Access_Array_Prototype,typename get_type_of(skim_table)::unqualified_value_type,ComponentType);
				define_component_interface(skimmer_itf,typename get_type_of(skim_reference),Prototypes::Network_Skimming_Prototype,ComponentType);
				skim_table_itf* los = this->skim_table<skim_table_itf*>();
				typename skimmer_itf::Component_Type::Stored_Time_Type value = (*los)[typename skim_table_itf::index_type(Origin_ID,Destination_ID)];
				return Time_Prototype<Basic_Time>::Convert_Value<Target_Type<NULLTYPE,typename TargetType::ReturnType,typename skimmer_itf::Component_Type::Stored_Time_Type>>(value);
			}
		};

		prototype struct Location_To_Zone_Map_Item_Prototype
		{
			tag_as_prototype;
			feature_prototype void initialize(typename TargetType::ParamType loc_index, typename TargetType::ParamType zone_index, typename TargetType::Param2Type weight)
			{
				this_component()->template initialize<ComponentType,CallerType,TargetType>(loc_index,zone_index,weight);
			}
			feature_accessor(loc_index,none,none);
			feature_accessor(zone_index,none,none);
			feature_accessor(weight,none,none);
		};
	}
}
