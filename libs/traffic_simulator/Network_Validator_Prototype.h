#pragma once
#include "User_Space_Includes.h"
#include "Activity_Location_Prototype.h"
#include "Traffic_Simulator_Concepts.h"

namespace Network_Components
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

	namespace Prototypes
	{

		prototype struct Network_Validator
		{
			tag_as_prototype;

			//=============================================
			// Primary events
			//---------------------------------------------
			static void Skim_Table_Update_Conditional(ComponentType* _this,Event_Response& response)
			{
				typedef Network_Skimming<ComponentType> _Skim_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_Skim_Interface* this_ptr=(_Skim_Interface*)_pthis;

				if (sub_iteration() == 0)
				{
					response.next._iteration = iteration();
					response.next._sub_iteration = Types::SUB_ITERATIONS::INITIALIZE;
					//response.result = false;
				}
				else if (sub_iteration() == Types::SUB_ITERATIONS::INITIALIZE)
				{
					response.next._iteration = iteration();
					response.next._sub_iteration = Types::SUB_ITERATIONS::PROCESS;
					//response.result = false;
				}
				else if (sub_iteration() == Types::SUB_ITERATIONS::PROCESS)
				{
					//_pthis->Swap_Event((Event)&Process_Skim_Trees_Event<NULLTYPE>);
					this_ptr->Process_Skim_Trees_Event<NT>();
					response.next._iteration = iteration();
					response.next._sub_iteration = Types::SUB_ITERATIONS::UPDATE;
					
					//response.result = true;
				}
				else if (sub_iteration() == Types::SUB_ITERATIONS::UPDATE)
				{
					//_pthis->Swap_Event((Event)&Update_Skim_Tables_Event<NULLTYPE>);
					this_ptr->Update_Skim_Tables_Event<NT>();
					response.next._iteration = Simulation_Time.template Future_Time<Simulation_Timestep_Increment,Simulation_Timestep_Increment>(this_ptr->template update_increment<Simulation_Timestep_Increment>());
					response.next._sub_iteration = 0;
					//response.result = true;
				}
				else
				{
					response.next._iteration = Simulation_Time.template Future_Time<Simulation_Timestep_Increment,Simulation_Timestep_Increment>(this_ptr->template update_increment<Simulation_Timestep_Increment>());
					response.next._sub_iteration = 0;
					//response.result = true;
				}
			}
			template<typename T> void Process_Skim_Trees_Event()
			{
				cout<<endl<<endl<<"====================================================="<<endl<<"Updating Network Skims:"<<endl;
				//typedef Network_Skimming<ComponentType> _Skim_Interface;
				//ComponentType* _pthis = (ComponentType*)_this;
				//_Skim_Interface* this_ptr=(_Skim_Interface*)_pthis;
				this->template timer<Counter&>().Start();
			}
			template<typename T> void Update_Skim_Tables_Event()
			{
				/*typedef Network_Skimming<ComponentType> _Skim_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_Skim_Interface* this_ptr=(_Skim_Interface*)_pthis;*/

				this->template Update_Skim_Tables<NULLTYPE>();
			}


			//=============================================
			// Primary data accessors
			//---------------------------------------------
			// links back to the network to be skimmed
			//accessor(network_reference, check(strip_modifiers(TargetType),Network_Components::Concepts::Is_Transportation_Network_Prototype), check(strip_modifiers(TargetType),Network_Components::Concepts::Is_Transportation_Network_Prototype) || check(strip_modifiers(TargetType),Network_Components::Concepts::Is_Transportation_Network_Prototype));
			accessor(network_reference, NONE, NONE);
			// ids of the modes contained in the model, set in implementation
			accessor(available_modes_container, NONE, NONE);
			// contains a map of skim_tables, one for each specified mode
			accessor(skims_by_time_container, NONE, NONE);
			// reference to the current, completed skim table from skim_tables_container, based on simulation time, where skim_table
			accessor(current_skim_table, NONE, NONE);
			// time increment at which skim tables are updated - set in the initializer
			accessor(update_increment,check(strip_modifiers(TargetType),Basic_Units::Concepts::Is_Time_Value),check(strip_modifiers(TargetType),Basic_Units::Concepts::Is_Time_Value));
			// scheduled time at which skim tables are updated - set in the initializer
			accessor(scheduled_update_time,check(strip_modifiers(TargetType),Basic_Units::Concepts::Is_Time_Value),check(strip_modifiers(TargetType),Basic_Units::Concepts::Is_Time_Value));
			// Associative Container of skim matrices, keyed on Mode Indicator values
			accessor(path_trees_container, NONE, NONE);
			// number of nodes selected per zone used to estimate zonal travel times
			accessor(nodes_per_zone, NONE, NONE);
			accessor(origin_locations, NONE, NONE);
			accessor(destination_locations, NONE, NONE);
			accessor(zone_origins_count, NONE, NONE);
			accessor(zone_destinations_count, NONE, NONE);
			accessor(timer, NONE, NONE);
			accessor(read_input, NONE, NONE);
			accessor(read_transit, NONE, NONE);
			accessor(read_highway_cost, NONE, NONE);
			accessor(write_output, NONE, NONE);
			accessor(highway_input_file, NONE, NONE);
			accessor(highway_output_file, NONE, NONE);
			accessor(highway_cost_input_file, NONE, NONE);
			accessor(highway_cost_output_file, NONE, NONE);
			accessor(transit_input_file, NONE, NONE);
			accessor(transit_output_file, NONE, NONE);
			accessor(skim_fit_results_file, NONE, NONE);

			//=============================================
			// Primary function accessors - used to pass through to the specific skimm table based on time-key
			//---------------------------------------------
			template<typename TargetType> void Initialize()
			{			
				this_component()->template Initialize<TargetType>();

				// get network reference
				typedef Network_Components::Prototypes::Network<typename get_type_of(network_reference)> network_itf;
				network_itf* network = this->network_reference<network_itf*>();

				// create the references to network items and create the boost::container::lists of origins/destination to route from/to
				typedef Zone_Components::Prototypes::Zone<typename remove_pointer<typename network_itf::get_type_of(zones_container)::mapped_type>::type> zone_itf;
				typedef Pair_Associative_Container<typename network_itf::get_type_of(zones_container),int,zone_itf*> zones_itf;

				typedef Activity_Location_Components::Prototypes::Activity_Location<typename remove_pointer<typename zone_itf::get_type_of(origin_activity_locations)::value_type>::type> location_itf;
				typedef Random_Access_Sequence<typename zone_itf::get_type_of(origin_activity_locations),location_itf*> locations_itf;
			
				typedef Random_Access_Sequence<typename location_itf::get_type_of(origin_links)> links_itf;
				typedef Link_Components::Prototypes::Link<typename get_component_type(links_itf)> link_itf;
				
				typedef Random_Access_Sequence<typename link_itf::get_type_of(outbound_turn_movements)> turns_itf;
				typedef Turn_Movement_Components::Prototypes::Movement<typename get_component_type(turns_itf)> turn_itf;

				/*typedef Activity_Location_Components::Prototypes::Activity_Location<typename remove_pointer<typename get_type_of(origin_locations)::value_type>::type> origin_location_itf;
				typedef Random_Access_Sequence<typename get_type_of(origin_locations),origin_location_itf*> origin_locations_itf;*/
				typedef Prototype_Random_Access_Sequence<typename get_type_of(origin_locations),Activity_Location_Components::Prototypes::Activity_Location> origin_locations_itf;
				typedef strip_modifiers(typename origin_locations_itf::value_type) origin_location_itf;

				/*typedef Activity_Location_Components::Prototypes::Activity_Location<typename remove_pointer<typename get_type_of(destination_locations)::value_type>::type> destination_location_itf;
				typedef Random_Access_Sequence<typename get_type_of(destination_locations),origin_location_itf*> destination_locations_itf;*/
				typedef Prototype_Random_Access_Sequence<typename get_type_of(destination_locations),Activity_Location_Components::Prototypes::Activity_Location> destination_locations_itf;
				typedef strip_modifiers(typename destination_locations_itf::value_type) destination_location_itf;

				typedef Pair_Associative_Container<typename get_type_of(zone_origins_count),int> zone_origins_itf;
				typedef Pair_Associative_Container<typename get_type_of(zone_destinations_count),int> zone_destinations_itf;

				origin_locations_itf* origin_locations = this->template origin_locations<origin_locations_itf*>();
				destination_locations_itf* destination_locations = this->template destination_locations<destination_locations_itf*>();
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
					zone_itf* orig_zone = (zone_itf*)(itr->second);
					locations_itf& available_locations = orig_zone->template origin_activity_locations<locations_itf&>();
					int num_locations = (int)available_locations.size();
					zone_origins_count.insert(pair<long,int>(orig_zone->template internal_id<long>(),0));
					
					// Add all locations to boost::container::list if less than the number required
					if (num_locations <= this->nodes_per_zone<int>())
					{
						for (int i=0; i<num_locations; i++)
						{
							origin_location_itf* loc = (origin_location_itf*)available_locations[i];
							if (loc->template Is_Routable_Location<bool>())
							{
								origin_locations->push_back(loc);
								zone_origins_count.find(orig_zone->template internal_id<long>())->second++;
							}
						}
					}
					// otherwise choose randomly
					else
					{
						int num_successful = 0;

						// make nodes_per_zone attempts to pick origin locations
						boost::container::vector<origin_location_itf*> available_locations_temp;
						for (int i=0; i<num_locations; i++) available_locations_temp.push_back((origin_location_itf*)available_locations[i]);

						// continue trying to add locations randomly while less than the required number have been added, until all locations have been tried
						while (available_locations_temp.size() > 0 && num_successful < this->nodes_per_zone<int>())
						{
							// get interface to the location
							int rand_loc_index = (int)(max<double>(0,(GLOBALS::Uniform_RNG.Next_Rand<double>() - 0.0001)) * (double)available_locations_temp.size());
							origin_location_itf* loc = (origin_location_itf*)available_locations_temp[rand_loc_index];

							// If the location does not have valid links or is already in the boost::container::list, skip
							if (loc->template Is_Routable_Location<bool>())
							{
								origin_locations->push_back(loc);
								zone_origins_count.find(orig_zone->template internal_id<long>())->second++;
								num_successful++;
							}
							available_locations_temp.erase(available_locations_temp.begin()+rand_loc_index);
						}


						//for (int i=0; i<this->nodes_per_zone<int>(); i++)
						//{
						//	// get interface to the location
						//	int rand_loc_index = (int)((GLOBALS::Uniform_RNG.Next_Rand<double>() - 0.0001) * (double)num_locations);
						//	origin_location_itf* loc = (origin_location_itf*)available_locations[rand_loc_index];

						//	// If the location does not have valid links or is already in the boost::container::list, skip
						//	if (!loc->template Is_Routable_Location<bool>())
						//	{
						//		cout << "Location not routable: " << loc->uuid<int>() <<", land use type: " << loc->land_use_type<Activity_Location_Components::Types::LAND_USE>()<<endl;
						//		i--; num_unsuccessful++;
						//	}
						//	origin_locations->push_back(loc);
						//	zone_origins_count.find(orig_zone->template internal_id<long>())->second++;
						//}
					}
					if (zone_origins_count.find(orig_zone->template internal_id<long>())->second == 0)
					{
						THROW_EXCEPTION("Origin zone '" << orig_zone->template uuid<long>() << "' has no valid activity locations, can not skim from this zone. Location count = "<< available_locations.size());
					}
				}

				//=================================================================================================
				// Loop through zones, choose destination points to route to, and add to maps
				for (itr= zones_container->begin();itr != zones_container->end(); ++itr)
				{
					zone_itf* dest_zone = (zone_itf*)(itr->second);
					locations_itf& available_locations = dest_zone->template destination_activity_locations<locations_itf&>();
					int num_locations = (int)available_locations.size();
					zone_destinations_count.insert(pair<long,int>(dest_zone->template internal_id<long>(),0));
					
					// Add all locations to boost::container::list if less than the number required
					if (num_locations <= this->nodes_per_zone<int>())
					{
						for (int i=0; i<num_locations; i++)
						{
							destination_location_itf* loc = (destination_location_itf*)available_locations[i];

							// Ignore if not routable or already selected
							if (loc->template Is_Routable_Location<bool>())
							{
								destination_locations->push_back(loc);
								zone_destinations_count.find(dest_zone->template internal_id<long>())->second++;
							}
						}
					}
					// otherwise choose randomly
					else
					{
						int num_successful = 0;

						// make nodes_per_zone attempts to pick origin locations
						boost::container::vector<destination_location_itf*> available_locations_temp;
						for (int i=0; i<num_locations; i++) available_locations_temp.push_back((destination_location_itf*)available_locations[i]);

						// continue trying to add locations randomly while less than the required number have been added, until all locations have been tried
						while (available_locations_temp.size() > 0 && num_successful < this->nodes_per_zone<int>())
						{
							// get interface to the location
							int rand_loc_index = (int)(max<double>(0,(GLOBALS::Uniform_RNG.Next_Rand<double>() - 0.0001)) * (double)available_locations_temp.size());
							destination_location_itf* loc = (destination_location_itf*)available_locations_temp[rand_loc_index];

							// If the location does not have valid links or is already in the boost::container::list, skip
							if (loc->template Is_Routable_Location<bool>())
							{
								destination_locations->push_back(loc);
								zone_destinations_count.find(dest_zone->template internal_id<long>())->second++;
								num_successful++;
							}
							available_locations_temp.erase(available_locations_temp.begin()+rand_loc_index);
						}

						//int num_successful = 0;
						//// make nodes_per_zone attempts to pick origin locations
						//for (int i=0; i<this->nodes_per_zone<int>(); i++)
						//{
						//	// get interface to the location
						//	int rand_loc_index = (int)((GLOBALS::Uniform_RNG.Next_Rand<double>() - 0.0001) * (double)num_locations);
						//	destination_location_itf* loc = (destination_location_itf*)available_locations[rand_loc_index];

						//	// If the location does not have valid links or is already in the boost::container::list, skip
						//	if (!loc->template Is_Routable_Location<bool>())
						//	{
						//		cout << "Location not routable: " << loc->uuid<int>() <<", land use type: " << loc->land_use_type<Activity_Location_Components::Types::LAND_USE>()<<endl;
						//		continue;
						//	}
						//	destination_locations->push_back(loc);
						//	zone_destinations_count.find(orig_zone->template internal_id<long>())->second++;
						//}
					}
					if (zone_destinations_count.find(dest_zone->template internal_id<long>())->second == 0) THROW_EXCEPTION("destination zone '" << dest_zone->template uuid<long>() << "' has no valid activity locations, can not skim to this zone.");
				}

				// Based on the above selected O/D routing pairs, initialize the skim routers
				this_component()->Initialize_Skims<TargetType>();

				// Load the skim updating event, which recalculates network skims at every Update interval (set in implementation)
				this_component()->Load_Event<ComponentType>(Skim_Table_Update_Conditional,0,Types::SUB_ITERATIONS::INITIALIZE);

			}
			template<typename TargetType> void Initialize(TargetType network_reference, requires(TargetType,check(TargetType, is_pointer) && check(strip_modifiers(TargetType), Network_Components::Concepts::Is_Transportation_Network_Prototype)))
			{
				// set the network references
				this->template network_reference<TargetType>(network_reference);
				
				this->template Initialize<NULLTYPE>();
			}			
			template<typename TargetType> void Initialize(TargetType network_reference, requires(TargetType,!check(TargetType, is_pointer) || !check(strip_modifiers(TargetType), Network_Components::Concepts::Is_Transportation_Network_Prototype)))
			{
				assert_check(TargetType, is_pointer,"TargetType is not a pointer" );
				assert_check(strip_modifiers(TargetType), Network_Components::Concepts::Is_Transportation_Network_Prototype, "TargetType is not a valid Transportation_Network interface");
				assert_sub_check(strip_modifiers(TargetType), Network_Components::Concepts::Is_Transportation_Network_Prototype, is_basic_network, "TargetType is not a basic network");
				assert_sub_check(strip_modifiers(TargetType), Network_Components::Concepts::Is_Transportation_Network_Prototype, has_turns, "TargetType does not have turns accessor");
				assert_sub_check(strip_modifiers(TargetType), Network_Components::Concepts::Is_Transportation_Network_Prototype, has_locations, "TargetType does not have locations accessor");
				assert_sub_check(strip_modifiers(TargetType), Network_Components::Concepts::Is_Transportation_Network_Prototype, has_zones, "TargetType does not have zones accessor");
			}			
			template<typename TargetType> bool Update_Skim_Tables()
			{
				//// Update each modal_skim
				this->Update_LOS<NULLTYPE>();

				// Output network skims
				if (this->write_output<bool>())
				{
					this->Write_LOS<NULLTYPE>();
				}

				// Network Skim timer
				cout << endl<<"Network Skimming run-time: " << this->template timer<Counter&>().Stop()<<endl<<endl;
				return true;
			}

			//=============================================
			// Primary function accessors - used to pass through to the specific skimm table based on time-key
			//--------------------------------------------


			template<typename TargetType> bool Update_LOS()
			{
				typedef Random_Access_Sequence<typename get_type_of(skims_by_time_container)> _skim_container_itf;
				typedef Prototypes::Skim_Table<typename get_component_type(_skim_container_itf)> _skim_itf;

				_skim_container_itf* skim = this->template skims_by_time_container<_skim_container_itf*>();
				
				for (typename _skim_container_itf::iterator itr = skim->begin(); itr != skim->end(); ++itr)
				{
					// update the skim for current time period
					if ((*itr)->template start_time<Simulation_Timestep_Increment>() >= iteration()) 
					{
						cout << endl << "Updating skim starting at iteration: " << (*itr)->template start_time<Simulation_Timestep_Increment>() << endl;
						(*itr)->template Update_LOS<NULLTYPE>();

						// If this is the first iteration and skims were not read from input initialize all of the time periods, otherwise break
						typedef Scenario_Components::Prototypes::Scenario<typename ComponentType::Master_Type::scenario_type> _Scenario_Interface;
						_Scenario_Interface* scenario = (_Scenario_Interface*)_global_scenario;
						if (iteration() > 0 || scenario->template read_skim_tables<bool>())	break;
					}
				}
				return true;
			}
			template<typename TargetType> void Write_LOS()
			{
				typedef Random_Access_Sequence<typename get_type_of(skims_by_time_container)> _skim_container_itf;
				typedef Prototypes::Skim_Table<typename get_component_type(_skim_container_itf)> _skim_itf;

				_skim_container_itf* skim = this->skims_by_time_container<_skim_container_itf*>();

				for (typename _skim_container_itf::iterator itr = skim->begin(); itr != skim->end(); ++itr)
				{
					if ((*itr)->template start_time<Simulation_Timestep_Increment>() >= iteration()) 
					{
						(*itr)->template Write_LOS<NULLTYPE>();
						break;
					}
				}
			}
			template<typename TargetType> static void Convert_Binary_Skimfile_To_CSV(string infilename, string outfilename)
			{
//				File_IO::Binary_File_Reader infile;
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
				for (Simulation_Timestep_Increment start = 0; start < GLOBALS::Time_Converter.template Convert_Value<Time_Hours,Simulation_Timestep_Increment>(24.0); start = start + update_increment)
				{		
					float* data = new float[num_zones*num_zones];



					infile.Read_Array<float>(data, num_zones*num_zones);


					boost::container::vector<float> temp(data,data+num_zones*num_zones);

					
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

			template<typename TargetType> int Get_Zone_ID(TargetType area_type_interface_ptr, requires(TargetType,check(TargetType, is_pointer) && check(strip_modifiers(TargetType), Activity_Location_Components::Concepts::Is_Activity_Location_Prototype)))
			{
				typedef Activity_Location_Components::Prototypes::Activity_Location<typename MasterType::activity_location_type> _location_interface;
				typedef Zone_Components::Prototypes::Zone<typename MasterType::zone_type> _zone_interface;
				_location_interface* loc = (_location_interface*)area_type_interface_ptr;
				return loc->template zone<_zone_interface*>()->template uuid<int>();
			}
			template<typename TargetType> int Get_Zone_ID(TargetType area_type_interface_ptr, requires(TargetType,check(TargetType, is_pointer) && check(strip_modifiers(TargetType), Zone_Components::Concepts::Is_Zone_Prototype)))
			{
				typedef Zone_Components::Prototypes::Zone<typename MasterType::zone_type> _zone_interface;
				_zone_interface* zone = (_zone_interface*)area_type_interface_ptr;
				return zone->template uuid<int>();
			}
			template<typename TargetType> int Get_Zone_ID(TargetType area_type_interface_ptr, requires(TargetType,!check(TargetType, is_pointer) || (!check(strip_modifiers(TargetType), Zone_Components::Concepts::Is_Zone_Prototype) && !check(strip_modifiers(TargetType), Activity_Location_Components::Concepts::Is_Activity_Location_Prototype))))
			{
				assert_check(TargetType, is_pointer, "Error, TargetType must be a pointer type.");
				assert_check(strip_modifiers(TargetType), Zone_Components::Concepts::Is_Zone_Prototype, "Error, TargetType must be either a Zone_Prototype or ");
				assert_check(strip_modifiers(TargetType), Activity_Location_Components::Concepts::Is_Activity_Location_Prototype, "TargetType must be an Activity_Location_Prototype.");
			}
		};

		prototype struct Network_Validation_Unit
		{
			tag_as_prototype;

			//=============================================
			// Primary events
			//---------------------------------------------
			static void Skim_Table_Update_Conditional(ComponentType* _this,Event_Response& response)
			{
				typedef Network_Skimming<ComponentType> _Skim_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_Skim_Interface* this_ptr=(_Skim_Interface*)_pthis;

				if (sub_iteration() == 0)
				{
					response.next._iteration = iteration();
					response.next._sub_iteration = Types::SUB_ITERATIONS::INITIALIZE;
					//response.result = false;
				}
				else if (sub_iteration() == Types::SUB_ITERATIONS::INITIALIZE)
				{
					response.next._iteration = iteration();
					response.next._sub_iteration = Types::SUB_ITERATIONS::PROCESS;
					//response.result = false;
				}
				else if (sub_iteration() == Types::SUB_ITERATIONS::PROCESS)
				{
					//_pthis->Swap_Event((Event)&Process_Skim_Trees_Event<NULLTYPE>);
					this_ptr->Process_Skim_Trees_Event<NT>();
					response.next._iteration = iteration();
					response.next._sub_iteration = Types::SUB_ITERATIONS::UPDATE;
					
					//response.result = true;
				}
				else if (sub_iteration() == Types::SUB_ITERATIONS::UPDATE)
				{
					//_pthis->Swap_Event((Event)&Update_Skim_Tables_Event<NULLTYPE>);
					this_ptr->Update_Skim_Tables_Event<NT>();
					response.next._iteration = Simulation_Time.template Future_Time<Simulation_Timestep_Increment,Simulation_Timestep_Increment>(this_ptr->template update_increment<Simulation_Timestep_Increment>());
					response.next._sub_iteration = 0;
					//response.result = true;
				}
				else
				{
					response.next._iteration = Simulation_Time.template Future_Time<Simulation_Timestep_Increment,Simulation_Timestep_Increment>(this_ptr->template update_increment<Simulation_Timestep_Increment>());
					response.next._sub_iteration = 0;
					//response.result = true;
				}
			}
			template<typename T> void Process_Skim_Trees_Event()
			{
				cout<<endl<<endl<<"====================================================="<<endl<<"Updating Network Skims:"<<endl;
				//typedef Network_Skimming<ComponentType> _Skim_Interface;
				//ComponentType* _pthis = (ComponentType*)_this;
				//_Skim_Interface* this_ptr=(_Skim_Interface*)_pthis;
				this->template timer<Counter&>().Start();
			}
			template<typename T> void Update_Skim_Tables_Event()
			{
				/*typedef Network_Skimming<ComponentType> _Skim_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_Skim_Interface* this_ptr=(_Skim_Interface*)_pthis;*/

				this->template Update_Skim_Tables<NULLTYPE>();
			}


			//=============================================
			// Primary data accessors
			//---------------------------------------------
			// links back to the network to be skimmed
			accessor(network_reference, NONE, NONE);
			// ids of the modes contained in the model, set in implementation
			accessor(available_modes_container, NONE, NONE);
			// contains a map of skim_tables, one for each specified mode
			accessor(location,NONE,NONE);
			accessor(skims_by_time_container, NONE, NONE);
			// reference to the current, completed skim table from skim_tables_container, based on simulation time, where skim_table
			accessor(current_skim_table, NONE, NONE);
			// time increment at which skim tables are updated - set in the initializer
			accessor(update_increment,check(strip_modifiers(TargetType),Basic_Units::Concepts::Is_Time_Value),check(strip_modifiers(TargetType),Basic_Units::Concepts::Is_Time_Value));
			// scheduled time at which skim tables are updated - set in the initializer
			accessor(scheduled_update_time,check(strip_modifiers(TargetType),Basic_Units::Concepts::Is_Time_Value),check(strip_modifiers(TargetType),Basic_Units::Concepts::Is_Time_Value));
			// Associative Container of skim matrices, keyed on Mode Indicator values
			accessor(path_trees_container, NONE, NONE);
			// number of nodes selected per zone used to estimate zonal travel times
			accessor(nodes_per_zone, NONE, NONE);
			accessor(origin_locations, NONE, NONE);
			accessor(destination_locations, NONE, NONE);
			accessor(zone_origins_count, NONE, NONE);
			accessor(zone_destinations_count, NONE, NONE);
			accessor(timer, NONE, NONE);
			accessor(read_input, NONE, NONE);
			accessor(read_transit, NONE, NONE);
			accessor(read_highway_cost, NONE, NONE);
			accessor(write_output, NONE, NONE);
			accessor(highway_input_file, NONE, NONE);
			accessor(highway_output_file, NONE, NONE);
			accessor(highway_cost_input_file, NONE, NONE);
			accessor(highway_cost_output_file, NONE, NONE);
			accessor(transit_input_file, NONE, NONE);
			accessor(transit_output_file, NONE, NONE);
			accessor(skim_fit_results_file, NONE, NONE);

			//=============================================
			// Primary function accessors - used to pass through to the specific skimm table based on time-key
			//---------------------------------------------
			template<typename TargetType> void Initialize()
			{			
				this_component()->template Initialize<TargetType>();

				// get network reference
				typedef Network_Components::Prototypes::Network<typename get_type_of(network_reference)> network_itf;
				network_itf* network = this->network_reference<network_itf*>();

				// create the references to network items and create the boost::container::lists of origins/destination to route from/to
				typedef Zone_Components::Prototypes::Zone<typename remove_pointer<typename network_itf::get_type_of(zones_container)::mapped_type>::type> zone_itf;
				typedef Pair_Associative_Container<typename network_itf::get_type_of(zones_container),int,zone_itf*> zones_itf;

				typedef Activity_Location_Components::Prototypes::Activity_Location<typename remove_pointer<typename zone_itf::get_type_of(origin_activity_locations)::value_type>::type> location_itf;
				typedef Random_Access_Sequence<typename zone_itf::get_type_of(origin_activity_locations),location_itf*> locations_itf;
			
				typedef Random_Access_Sequence<typename location_itf::get_type_of(origin_links)> links_itf;
				typedef Link_Components::Prototypes::Link<typename get_component_type(links_itf)> link_itf;
				
				typedef Random_Access_Sequence<typename link_itf::get_type_of(outbound_turn_movements)> turns_itf;
				typedef Turn_Movement_Components::Prototypes::Movement<typename get_component_type(turns_itf)> turn_itf;

				/*typedef Activity_Location_Components::Prototypes::Activity_Location<typename remove_pointer<typename get_type_of(origin_locations)::value_type>::type> origin_location_itf;
				typedef Random_Access_Sequence<typename get_type_of(origin_locations),origin_location_itf*> origin_locations_itf;*/
				typedef Prototype_Random_Access_Sequence<typename get_type_of(origin_locations),Activity_Location_Components::Prototypes::Activity_Location> origin_locations_itf;
				typedef strip_modifiers(typename origin_locations_itf::value_type) origin_location_itf;

				/*typedef Activity_Location_Components::Prototypes::Activity_Location<typename remove_pointer<typename get_type_of(destination_locations)::value_type>::type> destination_location_itf;
				typedef Random_Access_Sequence<typename get_type_of(destination_locations),origin_location_itf*> destination_locations_itf;*/
				typedef Prototype_Random_Access_Sequence<typename get_type_of(destination_locations),Activity_Location_Components::Prototypes::Activity_Location> destination_locations_itf;
				typedef strip_modifiers(typename destination_locations_itf::value_type) destination_location_itf;

				typedef Pair_Associative_Container<typename get_type_of(zone_origins_count),int> zone_origins_itf;
				typedef Pair_Associative_Container<typename get_type_of(zone_destinations_count),int> zone_destinations_itf;

				origin_locations_itf* origin_locations = this->template origin_locations<origin_locations_itf*>();
				destination_locations_itf* destination_locations = this->template destination_locations<destination_locations_itf*>();
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
					zone_itf* orig_zone = (zone_itf*)(itr->second);
					locations_itf& available_locations = orig_zone->template origin_activity_locations<locations_itf&>();
					int num_locations = (int)available_locations.size();
					zone_origins_count.insert(pair<long,int>(orig_zone->template internal_id<long>(),0));
					
					// Add all locations to boost::container::list if less than the number required
					if (num_locations <= this->nodes_per_zone<int>())
					{
						for (int i=0; i<num_locations; i++)
						{
							origin_location_itf* loc = (origin_location_itf*)available_locations[i];
							if (loc->template Is_Routable_Location<bool>())
							{
								origin_locations->push_back(loc);
								zone_origins_count.find(orig_zone->template internal_id<long>())->second++;
							}
						}
					}
					// otherwise choose randomly
					else
					{
						int num_successful = 0;

						// make nodes_per_zone attempts to pick origin locations
						boost::container::vector<origin_location_itf*> available_locations_temp;
						for (int i=0; i<num_locations; i++) available_locations_temp.push_back((origin_location_itf*)available_locations[i]);

						// continue trying to add locations randomly while less than the required number have been added, until all locations have been tried
						while (available_locations_temp.size() > 0 && num_successful < this->nodes_per_zone<int>())
						{
							// get interface to the location
							int rand_loc_index = (int)(max<double>(0,(GLOBALS::Uniform_RNG.Next_Rand<double>() - 0.0001)) * (double)available_locations_temp.size());
							origin_location_itf* loc = (origin_location_itf*)available_locations_temp[rand_loc_index];

							// If the location does not have valid links or is already in the boost::container::list, skip
							if (loc->template Is_Routable_Location<bool>())
							{
								origin_locations->push_back(loc);
								zone_origins_count.find(orig_zone->template internal_id<long>())->second++;
								num_successful++;
							}
							available_locations_temp.erase(available_locations_temp.begin()+rand_loc_index);
						}


						//for (int i=0; i<this->nodes_per_zone<int>(); i++)
						//{
						//	// get interface to the location
						//	int rand_loc_index = (int)((GLOBALS::Uniform_RNG.Next_Rand<double>() - 0.0001) * (double)num_locations);
						//	origin_location_itf* loc = (origin_location_itf*)available_locations[rand_loc_index];

						//	// If the location does not have valid links or is already in the boost::container::list, skip
						//	if (!loc->template Is_Routable_Location<bool>())
						//	{
						//		cout << "Location not routable: " << loc->uuid<int>() <<", land use type: " << loc->land_use_type<Activity_Location_Components::Types::LAND_USE>()<<endl;
						//		i--; num_unsuccessful++;
						//	}
						//	origin_locations->push_back(loc);
						//	zone_origins_count.find(orig_zone->template internal_id<long>())->second++;
						//}
					}
					if (zone_origins_count.find(orig_zone->template internal_id<long>())->second == 0)
					{
						THROW_EXCEPTION("Origin zone '" << orig_zone->template uuid<long>() << "' has no valid activity locations, can not skim from this zone. Location count = "<< available_locations.size());
					}
				}

				//=================================================================================================
				// Loop through zones, choose destination points to route to, and add to maps
				for (itr= zones_container->begin();itr != zones_container->end(); ++itr)
				{
					zone_itf* dest_zone = (zone_itf*)(itr->second);
					locations_itf& available_locations = dest_zone->template destination_activity_locations<locations_itf&>();
					int num_locations = (int)available_locations.size();
					zone_destinations_count.insert(pair<long,int>(dest_zone->template internal_id<long>(),0));
					
					// Add all locations to boost::container::list if less than the number required
					if (num_locations <= this->nodes_per_zone<int>())
					{
						for (int i=0; i<num_locations; i++)
						{
							destination_location_itf* loc = (destination_location_itf*)available_locations[i];

							// Ignore if not routable or already selected
							if (loc->template Is_Routable_Location<bool>())
							{
								destination_locations->push_back(loc);
								zone_destinations_count.find(dest_zone->template internal_id<long>())->second++;
							}
						}
					}
					// otherwise choose randomly
					else
					{
						int num_successful = 0;

						// make nodes_per_zone attempts to pick origin locations
						boost::container::vector<destination_location_itf*> available_locations_temp;
						for (int i=0; i<num_locations; i++) available_locations_temp.push_back((destination_location_itf*)available_locations[i]);

						// continue trying to add locations randomly while less than the required number have been added, until all locations have been tried
						while (available_locations_temp.size() > 0 && num_successful < this->nodes_per_zone<int>())
						{
							// get interface to the location
							int rand_loc_index = (int)(max<double>(0,(GLOBALS::Uniform_RNG.Next_Rand<double>() - 0.0001)) * (double)available_locations_temp.size());
							destination_location_itf* loc = (destination_location_itf*)available_locations_temp[rand_loc_index];

							// If the location does not have valid links or is already in the boost::container::list, skip
							if (loc->template Is_Routable_Location<bool>())
							{
								destination_locations->push_back(loc);
								zone_destinations_count.find(dest_zone->template internal_id<long>())->second++;
								num_successful++;
							}
							available_locations_temp.erase(available_locations_temp.begin()+rand_loc_index);
						}

						//int num_successful = 0;
						//// make nodes_per_zone attempts to pick origin locations
						//for (int i=0; i<this->nodes_per_zone<int>(); i++)
						//{
						//	// get interface to the location
						//	int rand_loc_index = (int)((GLOBALS::Uniform_RNG.Next_Rand<double>() - 0.0001) * (double)num_locations);
						//	destination_location_itf* loc = (destination_location_itf*)available_locations[rand_loc_index];

						//	// If the location does not have valid links or is already in the boost::container::list, skip
						//	if (!loc->template Is_Routable_Location<bool>())
						//	{
						//		cout << "Location not routable: " << loc->uuid<int>() <<", land use type: " << loc->land_use_type<Activity_Location_Components::Types::LAND_USE>()<<endl;
						//		continue;
						//	}
						//	destination_locations->push_back(loc);
						//	zone_destinations_count.find(orig_zone->template internal_id<long>())->second++;
						//}
					}
					if (zone_destinations_count.find(dest_zone->template internal_id<long>())->second == 0) THROW_EXCEPTION("destination zone '" << dest_zone->template uuid<long>() << "' has no valid activity locations, can not skim to this zone.");
				}

				// Based on the above selected O/D routing pairs, initialize the skim routers
				this_component()->Initialize_Skims<TargetType>();

				// Load the skim updating event, which recalculates network skims at every Update interval (set in implementation)
				this_component()->Load_Event<ComponentType>(Skim_Table_Update_Conditional,0,Types::SUB_ITERATIONS::INITIALIZE);

			}
			template<typename TargetType> void Initialize(TargetType location, requires(TargetType,check(TargetType, is_pointer) && check(strip_modifiers(TargetType), Activity_Location_Components::Concepts::Is_Activity_Location_Prototype)))
			{
				this_component()->Initialize<TargetType>(location);
			}			
			template<typename TargetType> void Initialize(TargetType network_reference, requires(TargetType,!check(TargetType, is_pointer) || !check(strip_modifiers(TargetType), Activity_Location_Components::Concepts::Is_Activity_Location_Prototype)))
			{
				assert_check(TargetType, is_pointer,"TargetType is not a pointer" );
				assert_check(strip_modifiers(TargetType), Network_Components::Concepts::Is_Transportation_Network_Prototype, "TargetType is not a valid Transportation_Network interface");
				assert_sub_check(strip_modifiers(TargetType), Network_Components::Concepts::Is_Transportation_Network_Prototype, is_basic_network, "TargetType is not a basic network");
				assert_sub_check(strip_modifiers(TargetType), Network_Components::Concepts::Is_Transportation_Network_Prototype, has_turns, "TargetType does not have turns accessor");
				assert_sub_check(strip_modifiers(TargetType), Network_Components::Concepts::Is_Transportation_Network_Prototype, has_locations, "TargetType does not have locations accessor");
				assert_sub_check(strip_modifiers(TargetType), Network_Components::Concepts::Is_Transportation_Network_Prototype, has_zones, "TargetType does not have zones accessor");
			}			
			template<typename TargetType> bool Update_Skim_Tables()
			{
				//// Update each modal_skim
				this->Update_LOS<NULLTYPE>();

				// Output network skims
				if (this->write_output<bool>())
				{
					this->Write_LOS<NULLTYPE>();
				}

				// Network Skim timer
				cout << endl<<"Network Skimming run-time: " << this->template timer<Counter&>().Stop()<<endl<<endl;
				return true;
			}

			//=============================================
			// Primary function accessors - used to pass through to the specific skimm table based on time-key
			//---------------------------------------------


			template<typename TargetType> bool Update_LOS()
			{
				typedef Random_Access_Sequence<typename get_type_of(skims_by_time_container)> _skim_container_itf;
				typedef Prototypes::Skim_Table<typename get_component_type(_skim_container_itf)> _skim_itf;

				_skim_container_itf* skim = this->template skims_by_time_container<_skim_container_itf*>();
				
				for (typename _skim_container_itf::iterator itr = skim->begin(); itr != skim->end(); ++itr)
				{
					// update the skim for current time period
					if ((*itr)->template start_time<Simulation_Timestep_Increment>() >= iteration()) 
					{
						cout << endl << "Updating skim starting at iteration: " << (*itr)->template start_time<Simulation_Timestep_Increment>() << endl;
						(*itr)->template Update_LOS<NULLTYPE>();

						// If this is the first iteration and skims were not read from input initialize all of the time periods, otherwise break
						typedef Scenario_Components::Prototypes::Scenario<typename ComponentType::Master_Type::scenario_type> _Scenario_Interface;
						_Scenario_Interface* scenario = (_Scenario_Interface*)_global_scenario;
						if (iteration() > 0 || scenario->template read_skim_tables<bool>())	break;
					}
				}
				return true;
			}
			template<typename TargetType> void Write_LOS()
			{
				typedef Random_Access_Sequence<typename get_type_of(skims_by_time_container)> _skim_container_itf;
				typedef Prototypes::Skim_Table<typename get_component_type(_skim_container_itf)> _skim_itf;

				_skim_container_itf* skim = this->skims_by_time_container<_skim_container_itf*>();

				for (typename _skim_container_itf::iterator itr = skim->begin(); itr != skim->end(); ++itr)
				{
					if ((*itr)->template start_time<Simulation_Timestep_Increment>() >= iteration()) 
					{
						(*itr)->template Write_LOS<NULLTYPE>();
						break;
					}
				}
			}
			template<typename TargetType> static void Convert_Binary_Skimfile_To_CSV(string infilename, string outfilename)
			{
//				File_IO::Binary_File_Reader infile;
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
				for (Simulation_Timestep_Increment start = 0; start < GLOBALS::Time_Converter.template Convert_Value<Time_Hours,Simulation_Timestep_Increment>(24.0); start = start + update_increment)
				{		
					float* data = new float[num_zones*num_zones];



					infile.Read_Array<float>(data, num_zones*num_zones);


					boost::container::vector<float> temp(data,data+num_zones*num_zones);

					
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

			template<typename TargetType> int Get_Zone_ID(TargetType area_type_interface_ptr, requires(TargetType,check(TargetType, is_pointer) && check(strip_modifiers(TargetType), Activity_Location_Components::Concepts::Is_Activity_Location_Prototype)))
			{
				typedef Activity_Location_Components::Prototypes::Activity_Location<typename MasterType::activity_location_type> _location_interface;
				typedef Zone_Components::Prototypes::Zone<typename MasterType::zone_type> _zone_interface;
				_location_interface* loc = (_location_interface*)area_type_interface_ptr;
				return loc->template zone<_zone_interface*>()->template uuid<int>();
			}
			template<typename TargetType> int Get_Zone_ID(TargetType area_type_interface_ptr, requires(TargetType,check(TargetType, is_pointer) && check(strip_modifiers(TargetType), Zone_Components::Concepts::Is_Zone_Prototype)))
			{
				typedef Zone_Components::Prototypes::Zone<typename MasterType::zone_type> _zone_interface;
				_zone_interface* zone = (_zone_interface*)area_type_interface_ptr;
				return zone->template uuid<int>();
			}
			template<typename TargetType> int Get_Zone_ID(TargetType area_type_interface_ptr, requires(TargetType,!check(TargetType, is_pointer) || (!check(strip_modifiers(TargetType), Zone_Components::Concepts::Is_Zone_Prototype) && !check(strip_modifiers(TargetType), Activity_Location_Components::Concepts::Is_Activity_Location_Prototype))))
			{
				assert_check(TargetType, is_pointer, "Error, TargetType must be a pointer type.");
				assert_check(strip_modifiers(TargetType), Zone_Components::Concepts::Is_Zone_Prototype, "Error, TargetType must be either a Zone_Prototype or ");
				assert_check(strip_modifiers(TargetType), Activity_Location_Components::Concepts::Is_Activity_Location_Prototype, "TargetType must be an Activity_Location_Prototype.");
			}
		};

	}
}

