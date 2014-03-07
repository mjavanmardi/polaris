#pragma once
#include "User_Space_Includes.h"
#include "Activity_Location_Prototype.h"

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

	namespace Prototypes
	{
		template<typename ComponentType> struct Skim_Table;
		template<typename ComponentType> struct Location_To_Zone_Map_Item;

		prototype struct Network_Skimming
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
					response.next._iteration = _iteration;
					response.next._sub_iteration = Types::SUB_ITERATIONS::INITIALIZE;
					response.result = false;
				}
				else if (sub_iteration() == Types::SUB_ITERATIONS::INITIALIZE)
				{
					response.next._iteration = _iteration;
					response.next._sub_iteration = Types::SUB_ITERATIONS::PROCESS;
					response.result = false;
				}
				else if (sub_iteration() == Types::SUB_ITERATIONS::PROCESS)
				{
					_pthis->Swap_Event((Event)&Process_Skim_Trees_Event<NULLTYPE>);
					response.next._iteration = _iteration;
					response.next._sub_iteration = Types::SUB_ITERATIONS::UPDATE;
					response.result = true;
				}
				else if (sub_iteration() == Types::SUB_ITERATIONS::UPDATE)
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
			declare_event(Process_Skim_Trees_Event)
			{
				cout<<endl<<endl<<"====================================================="<<endl<<"Updating Network Skims:"<<endl;
				typedef Network_Skimming<ComponentType> _Skim_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_Skim_Interface* this_ptr=(_Skim_Interface*)_pthis;
				this_ptr->template timer<Counter&>().Start();
			}
			declare_event(Update_Skim_Tables_Event)
			{
				typedef Network_Skimming<ComponentType> _Skim_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_Skim_Interface* this_ptr=(_Skim_Interface*)_pthis;

				this_ptr->template Update_Skim_Tables<NULLTYPE>();
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
				typedef Link_Components::Prototypes::Link<typename get_value_type(links_itf)> link_itf;
				
				typedef Random_Access_Sequence<typename link_itf::get_type_of(outbound_turn_movements)> turns_itf;
				typedef Turn_Movement_Components::Prototypes::Movement<typename get_value_type(turns_itf)> turn_itf;

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
				//TODO
//load_event(ComponentType,Skim_Table_Update_Conditional,Update_Skim_Tables_Event,0,Types::SUB_ITERATIONS::INITIALIZE,NULLTYPE);

			}
			template<typename TargetType> void Initialize(TargetType network_reference, requires(TargetType,check(TargetType, is_pointer) && check(strip_modifiers(TargetType), Network_Components::Concepts::Is_Transportation_Network_Prototype)))
			{
				// set the network references
				this->template network_reference<TargetType>(network_reference);
				
				this->template Initialize<NULLTYPE>();
			}			
			template<typename TargetType> void Initialize(TargetType network_reference, requires(TargetType,!check(TargetType, is_pointer) || !check(strip_modifiers(TargetType), Network_Components::Concepts::Is_Transportation_Network_Prototype)))
			{
				assert_check(strip_modifiers(TargetType), is_pointer,"TargetType is not a pointer" );
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
				cout << endl<<"Network Skimming run-time: " << this->template timer<Counter&>().Stop();
				return true;
			}

			//=============================================
			// Primary function accessors - used to pass through to the specific skimm table based on time-key
			//---------------------------------------------
			// This returns the travel time based on the current simulation time
			template<typename LocationType, typename ModeType, typename ReturnType> ReturnType Get_TTime(LocationType Origin, LocationType Destination, ModeType Mode_Indicator, requires(ReturnType,check(ReturnType, Basic_Units::Concepts::Is_Time_Value)))
			{	
				// call the get los function
				ReturnType return_val = this->template Get_TTime<LocationType,ModeType, Simulation_Timestep_Increment,ReturnType>(Origin, Destination, Mode_Indicator, Simulation_Time.template Current_Time<Simulation_Timestep_Increment>());

				return return_val;

			}
			// This returns the travel time during a specific time interval
			template<typename LocationType, typename ModeType, typename TimeType, typename ReturnType> ReturnType Get_TTime(LocationType Origin, LocationType Destination, ModeType Mode_Indicator, TimeType Start_Time, requires(ReturnType,check(ReturnType, Basic_Units::Concepts::Is_Time_Value)))
			{
				//typedef (_skim_container_itf, _skim_itf,typename get_type_of(skims_by_time_container),Random_Access_Sequence,Prototypes::Skim_Table);
				typedef Random_Access_Sequence<typename get_type_of(skims_by_time_container)> _skim_container_itf;
				typedef Prototypes::Skim_Table<get_value_type(_skim_container_itf)> _skim_itf;

				//typedef (_skim_matrix_itf, _los_itf,typename _skim_itf::get_type_of(skim_table),Multidimensional_Random_Access_Array,Prototypes::LOS);
				typedef Multidimensional_Random_Access_Array<typename _skim_itf::get_type_of(skim_table)> _skim_matrix_itf;
				typedef Prototypes::LOS<get_value_type(_skim_matrix_itf)> _los_itf;
				
				// call the general get los function
				_los_itf* los_value = this->template Get_LOS<LocationType,TimeType,_los_itf*>(Origin, Destination, Start_Time);

				// extract and return the auto travel time
				if (Mode_Indicator == Vehicle_Components::Types::Vehicle_Type_Keys::SOV) return los_value->auto_ttime<ReturnType>();
				else if (Mode_Indicator == Vehicle_Components::Types::Vehicle_Type_Keys::BICYCLE) return GLOBALS::Time_Converter.Convert_Value<Time_Hours,ReturnType>(los_value->auto_distance<Miles>()/15.0); // assume bike speed of 15MPH
				else if (Mode_Indicator == Vehicle_Components::Types::Vehicle_Type_Keys::WALK) return GLOBALS::Time_Converter.Convert_Value<Time_Hours,ReturnType>(los_value->auto_distance<Miles>()/3.0/0.9); // assume walk speed of 3MPH and reduce walk distance by 10%
				else return (los_value->transit_ttime<ReturnType>() + los_value->transit_wait_time<ReturnType>() + los_value->transit_walk_access_time<ReturnType>());
				
			}
			
			// This returns the full level of service information for the O/D pair at the current time
			template<typename LocationType, typename ReturnType> ReturnType Get_LOS(LocationType Origin, LocationType Destination, requires(ReturnType,check(strip_modifiers(ReturnType), Concepts::Is_LOS_Prototype)))
			{		
				// call the get los function
				ReturnType ret_value = this->template Get_LOS<LocationType, Simulation_Timestep_Increment, ReturnType>(Origin, Destination, Simulation_Time.template Current_Time<Simulation_Timestep_Increment>());

				return ret_value;
			}
			// This returns the full level of service information for the O/D pair at a specific time
			template<typename LocationType, typename TimeType, typename ReturnType> ReturnType  Get_LOS(LocationType Origin, LocationType Destination, TimeType Start_Time, requires(ReturnType,check(strip_modifiers(ReturnType), Concepts::Is_LOS_Prototype)))
			{
				// create the references to network items and create the boost::container::lists of origins/destination to route from/to
				typedef Network_Components::Prototypes::Network<typename get_type_of(network_reference)> network_itf;
				
				typedef Pair_Associative_Container<typename network_itf::get_type_of(zones_container)> zones_itf;
				typedef Zone_Components::Prototypes::Zone<typename get_data_type(zones_itf)> zone_itf;

				network_itf* network = this->template network_reference<network_itf*>();
				zones_itf* zones = network->template zones_container<zones_itf*>();
				typename zones_itf::iterator zone_itr;

				// convert the ids to indices
				zone_itf *orig_zone, *dest_zone;

				// Extract zone ID information from the input origin/destination type (either location or zone)
				int Origin_Zone_ID = this->Get_Zone_ID<LocationType>(Origin);
				int Destination_Zone_ID = this->Get_Zone_ID<LocationType>(Destination);

				// Do a lookup to make sure the zone is in the network (may be able to remove this)
				if ((zone_itr = zones->find(Origin_Zone_ID)) != zones->end()){ orig_zone = (zone_itf *)(zone_itr->second);}
				else THROW_EXCEPTION("ERROR, origin zone id: " << Origin_Zone_ID << " was not found for Origin uuid,internal_id: " << Origin->uuid<int>()<<","<<Origin->internal_id<int>());

				if ((zone_itr = zones->find(Destination_Zone_ID)) != zones->end()){ dest_zone = (zone_itf *)(zone_itr->second);}
				else THROW_EXCEPTION("ERROR, destination zone id: " << Destination_Zone_ID << " was not found for Destination uuid,internal_id: "<<Destination->uuid<int>()<<","<<Destination->internal_id<int>()<<", mem location: "<<Destination);

				//============================================================
				// Transferred code here from former mode_skim_prototype
				//-------------------------------------------------------------------
				typedef Random_Access_Sequence<typename get_type_of(skims_by_time_container)> _skim_container_itf;
				typedef Prototypes::Skim_Table<typename get_value_type(get_type_of(skims_by_time_container))> _skim_itf;

				//typedef (_skim_container_itf, _skim_itf,typename get_type_of(skims_by_time_container),Random_Access_Sequence,Prototypes::Skim_Table);
				_skim_container_itf* skims = this->skims_by_time_container<_skim_container_itf*>();
				typename _skim_container_itf::iterator itr = skims->begin();
				_skim_itf* skim_table;

				// get only the HH:MM:SS portion of requested time if Time > 1 day
				int days = ((int)(GLOBALS::Time_Converter.Convert_Value<TimeType,Time_Hours>(Start_Time))/24);
				typename TimeType rounded = GLOBALS::Time_Converter.Convert_Value<Time_Hours,TimeType>((float)days * 24.0);
				typename TimeType remain = Start_Time - rounded;
				
				// go to skim table for requested time period
				for (; itr != skims->end(); ++itr)
				{
					skim_table = *itr;
					if (skim_table->template end_time<TimeType>() > remain)
					{
						ReturnType return_val =  skim_table->template Get_LOS<int,ReturnType>(orig_zone->template internal_id<int>(), dest_zone->template internal_id<int>());
						
						return return_val;
					}
				}

				// if the code gets here, then the requested time does not fall within any skim_table time period
				cout << endl << "Get LOS failure: " <<"origin: " << Origin_Zone_ID <<", destination: " << Destination_Zone_ID<<", time: "  << Start_Time<<endl;
				assert(false);
				return false;
			}
			
			
			template<typename TargetType> bool Update_LOS()
			{
				typedef (_skim_container_itf, _skim_itf,typename get_type_of(skims_by_time_container),Random_Access_Sequence,Prototypes::Skim_Table);
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
				typedef (_skim_container_itf, _skim_itf,typename get_type_of(skims_by_time_container),Random_Access_Sequence,Prototypes::Skim_Table);
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

		prototype struct Skim_Table ADD_DEBUG_INFO
		{
			tag_as_prototype;

			// Associative Container of skim matrices, keyed on Mode Indicator values
			accessor(skim_table, NONE, NONE);
			
			// links back to the network to be skimmed
			accessor(network_reference, NONE, NONE);
			// links back to the skimming faculty
			accessor(skim_reference, NONE, NONE);

			// simulated day start and end times represented by skim_table
			accessor(start_time, NONE, NONE);
			accessor(end_time, NONE, NONE);

			accessor(weighted_deviation, NONE, NONE);
			accessor(max_deviation, NONE, NONE);

			//=============================================
			// Primary function accessors - used to calculate/return LOS values for OD pairs
			//---------------------------------------------
			template<typename TargetType> void Initialize()
			{
				// call implementation initializer
				this_component()->template Initialize<TargetType>();

				// set the network reference
				//typedef Prototypes::LOS<typename remove_pointer< typename get_type_of(skim_table)::value_type>::type> los_value_itf;
				//typedef Multidimensional_Random_Access_Array< typename get_type_of(skim_table),los_value_itf*> skim_table_itf;

				//typedef Network_Components::Prototypes::Network<typename get_type_of(network_reference)> network_itf;
				//typedef Prototypes::Network_Skimming<typename get_type_of(skim_reference)> skimmer_itf;
				//network_itf* network = this->network_reference<network_itf*>();
				//skimmer_itf* skim = this->skim_reference<skimmer_itf*>();
				//
				//// create the LOS container
				//typedef Zone_Components::Prototypes::Zone<typename remove_pointer<typename network_itf::get_type_of(zones_container)::value_type>::type> zone_itf;
				//typedef Pair_Associative_Container<typename network_itf::get_type_of(zones_container),zone_itf*> zones_itf;

				//typedef Activity_Location_Components::Prototypes::Activity_Location<typename remove_pointer<typename zone_itf::get_type_of(origin_activity_locations)::value_type>::type> location_itf;
				//typedef Random_Access_Sequence<typename zone_itf::get_type_of(origin_activity_locations),location_itf*> locations_itf;

				//typedef Activity_Location_Components::Prototypes::Activity_Location<typename remove_pointer<typename location_itf::get_type_of(origin_links)::value_type>::type> link_itf;
				//typedef Random_Access_Sequence<typename location_itf::get_type_of(origin_links),link_itf*> links_itf;

				//typedef (origin_map_itf,origin_item_itf,typename skimmer_itf::get_type_of(origin_node_to_zone_map), Pair_Associative_Container,Prototypes::Location_To_Zone_Map_Item);
				//origin_map_itf* origin_map = skim->template origin_node_to_zone_map<origin_map_itf*>();
				//zones_itf* zones_container = network->template zones_container<zones_itf*>();
				//typename origin_map_itf::iterator orig_itr = origin_map->begin();
				//locations_itf* activity_locations = network->template activity_locations_container<locations_itf*>();
		
				//// create the LOS matrix
				//typedef matrix<los_value_itf*>::size_type size_t;
				//skim_table_itf* los = this->skim_table<skim_table_itf*>();
				//los->resize(pair<size_t,size_t>((size_t)zones_container->size(),(size_t)zones_container->size()), 0);
			}			

			template<typename TargetType> void Initialize(TargetType initial_data, requires(TargetType,check(strip_modifiers(TargetType), is_pointer)))
			{
				//cout << endl << "Initializing skim table at iteration - " << iteration();


				// call implementation initializer
				this_component()->template Initialize<TargetType>(initial_data);

				// set the network reference
				//typedef Multidimensional_Random_Access_Array<typename get_type_of(skim_table),typename get_type_of(skim_table)::value_type> skim_table_itf;
				//typedef Prototypes::LOS<typename remove_pointer< typename get_type_of(skim_table)::value_type>::type> los_value_itf;
				//typedef Multidimensional_Random_Access_Array< typename get_type_of(skim_table),los_value_itf*> skim_table_itf;

				//typedef Network_Components::Prototypes::Network<typename get_type_of(network_reference)> network_itf;
				//typedef Prototypes::Network_Skimming<typename get_type_of(skim_reference)> skimmer_itf;
				//network_itf* network = this->network_reference<network_itf*>();
				//skimmer_itf* skim = this->skim_reference<skimmer_itf*>();
				//
				//// create the LOS container
				//typedef Zone_Components::Prototypes::Zone<typename remove_pointer<typename network_itf::get_type_of(zones_container)::value_type>::type> zone_itf;
				//typedef Pair_Associative_Container<typename network_itf::get_type_of(zones_container),zone_itf*> zones_itf;

				//typedef Activity_Location_Components::Prototypes::Activity_Location<typename remove_pointer<typename zone_itf::get_type_of(origin_activity_locations)::value_type>::type> location_itf;
				//typedef Random_Access_Sequence<typename zone_itf::get_type_of(origin_activity_locations),location_itf*> locations_itf;

				//typedef Activity_Location_Components::Prototypes::Activity_Location<typename remove_pointer<typename location_itf::get_type_of(origin_links)::value_type>::type> link_itf;
				//typedef Random_Access_Sequence<typename location_itf::get_type_of(origin_links),link_itf*> links_itf;

				//typedef (origin_map_itf,origin_item_itf,typename skimmer_itf::get_type_of(origin_node_to_zone_map), Pair_Associative_Container,Prototypes::Location_To_Zone_Map_Item);
				////typedef (destination_map_itf,destination_item_itf,skimmer_itf::get_type_of(destination_node_to_zone_map), Pair_Associative_Container,Prototypes::Location_To_Zone_Map_Item);
				//origin_map_itf* origin_map = skim->template origin_node_to_zone_map<origin_map_itf*>();
				////destination_map_itf* destination_map = skim->destination_node_to_zone_map<destination_map_itf*>();		
				//zones_itf* zones_container = network->template zones_container<zones_itf*>();
				//typename origin_map_itf::iterator orig_itr = origin_map->begin();
				////destination_map_itf::iterator dest_itr = destination_map->begin();
				//locations_itf* activity_locations = network->template activity_locations_container<locations_itf*>();
		
				// create the LOS matrix
				//typedef matrix<los_value_itf*>::size_type size_t;
				//skim_table_itf* los = this->skim_table<skim_table_itf*>();
				//los->Copy(pair<size_t,size_t>((size_t)zones_container->size(),(size_t)zones_container->size()), (los_value_itf*)initial_data);

			}	

			template<typename TargetType> bool Process_Skims()
			{
			}

			template<typename TargetType> bool Update_LOS()
			{
				return this_component()->Update_LOS<TargetType>();
			}

			template<typename TargetType> void Write_LOS()
			{
				this_component()->Write_LOS<TargetType>();
			}

			template<typename ParamType, typename ReturnType> ReturnType Get_LOS(ParamType Origin_Index, ParamType Destination_Index)
			{
				ReturnType ret_value = this_component()->Get_LOS<ParamType, ReturnType>(Origin_Index,Destination_Index);
				return ret_value;
			}
		};

		prototype struct LOS ADD_DEBUG_INFO
		{
			tag_as_prototype;

			accessor(auto_ttime, NONE, NONE);
			accessor(auto_tolls, NONE, NONE);
			accessor(auto_parking_cost, NONE, NONE);
			accessor(auto_distance /*transit_sov_access_time*/, NONE, NONE);

			accessor(transit_ttime, NONE, NONE);
			accessor(transit_walk_access_time, NONE, NONE);		
			accessor(transit_wait_time, NONE, NONE);
			accessor(transit_fare, NONE, NONE);
			accessor(LOS_time_invariant, NONE, NONE);
		};

	}
}

