#pragma once
#include "Traffic_Simulator_Includes.h"
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

		prototype struct Network_Skimming ADD_DEBUG_INFO
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
					this_ptr->template Process_Skim_Trees_Event<NT>();
					response.next._iteration = iteration();
					response.next._sub_iteration = Types::SUB_ITERATIONS::UPDATE;
					
					//response.result = true;
				}
				else if (sub_iteration() == Types::SUB_ITERATIONS::UPDATE)
				{
					this_ptr->template Update_Skim_Tables_Event<NT>();
					
					//response.next._iteration = Simulation_Time.template Future_Time<Simulation_Timestep_Increment,Simulation_Timestep_Increment>(this_ptr->template update_increment<Simulation_Timestep_Increment>());
					response.next._iteration = this_ptr->template update_increment<Simulation_Timestep_Increment>();
					response.next._sub_iteration = 0;
					
					this_ptr->current_increment_index<int&>()++;
				}
				else
				{
					this_ptr->current_increment_index<int&>()++;
					//response.next._iteration = Simulation_Time.template Future_Time<Simulation_Timestep_Increment,Simulation_Timestep_Increment>(this_ptr->template update_increment<Simulation_Timestep_Increment>());
					response.next._iteration = this_ptr->template update_increment<Simulation_Timestep_Increment>();
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
			//accessor(update_increment,check(strip_modifiers(TargetType),Basic_Units::Concepts::Is_Time_Value),check(strip_modifiers(TargetType),Basic_Units::Concepts::Is_Time_Value));
			accessor(update_interval_endpoints, NONE, NONE);
			accessor(current_increment_index,NONE,NONE);
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

				// create the references to network items and create the std::lists of origins/destination to route from/to
				typedef Zone_Components::Prototypes::Zone<typename remove_pointer<typename network_itf::get_type_of(zones_container)::mapped_type>::type> zone_itf;
				typedef Pair_Associative_Container<typename network_itf::get_type_of(zones_container),int,zone_itf*> zones_itf;

				typedef Activity_Location_Components::Prototypes::Activity_Location<typename remove_pointer<typename zone_itf::get_type_of(origin_activity_locations)::value_type>::type> location_itf;
				typedef Random_Access_Sequence<typename zone_itf::get_type_of(origin_activity_locations),location_itf*> locations_itf;
			
				typedef Random_Access_Sequence<typename location_itf::get_type_of(origin_links)> links_itf;
				typedef Link_Components::Prototypes::Link<get_component_type(links_itf)> link_itf;
				
				typedef Random_Access_Sequence<typename link_itf::get_type_of(outbound_turn_movements)> turns_itf;
				typedef Turn_Movement_Components::Prototypes::Movement<get_component_type(turns_itf)> turn_itf;

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

				bool valid_skim = true;
				stringstream errors;

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
					
					// Add all locations to std::list if less than the number required
					if (num_locations <= this->template nodes_per_zone<int>())
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
						std::vector<origin_location_itf*> available_locations_temp;
						for (int i=0; i<num_locations; i++) available_locations_temp.push_back((origin_location_itf*)available_locations[i]);

						// continue trying to add locations randomly while less than the required number have been added, until all locations have been tried
						while (available_locations_temp.size() > 0 && num_successful < this->template nodes_per_zone<int>())
						{
							// get interface to the location
							int rand_loc_index = (int)(max<double>(0,(GLOBALS::Uniform_RNG.template Next_Rand<double>() - 0.0001)) * (double)available_locations_temp.size());
							origin_location_itf* loc = (origin_location_itf*)available_locations_temp[rand_loc_index];

							// If the location does not have valid links or is already in the std::list, skip
							if (loc->template Is_Routable_Location<bool>())
							{
								origin_locations->push_back(loc);
								zone_origins_count.find(orig_zone->template internal_id<long>())->second++;
								num_successful++;
							}
							available_locations_temp.erase(available_locations_temp.begin()+rand_loc_index);
						}


						//for (int i=0; i<this->template nodes_per_zone<int>(); i++)
						//{
						//	// get interface to the location
						//	int rand_loc_index = (int)((GLOBALS::Uniform_RNG.template Next_Rand<double>() - 0.0001) * (double)num_locations);
						//	origin_location_itf* loc = (origin_location_itf*)available_locations[rand_loc_index];

						//	// If the location does not have valid links or is already in the std::list, skip
						//	if (!loc->template Is_Routable_Location<bool>())
						//	{
						//		cout << "Location not routable: " << loc->template uuid<int>() <<", land use type: " << loc->land_use_type<Activity_Location_Components::Types::LAND_USE>()<<endl;
						//		i--; num_unsuccessful++;
						//	}
						//	origin_locations->push_back(loc);
						//	zone_origins_count.find(orig_zone->template internal_id<long>())->second++;
						//}
					}
					if (zone_origins_count.find(orig_zone->template internal_id<long>())->second == 0)
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
						valid_skim = false;
						errors<<"Origin zone '" << orig_zone->template uuid<long>() << "' has no valid activity locations, can not skim from this zone. Location count = "<< available_locations.size()<<endl;
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
					
					// Add all locations to std::list if less than the number required
					if (num_locations <= this->template nodes_per_zone<int>())
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
						std::vector<destination_location_itf*> available_locations_temp;
						for (int i=0; i<num_locations; i++) available_locations_temp.push_back((destination_location_itf*)available_locations[i]);

						// continue trying to add locations randomly while less than the required number have been added, until all locations have been tried
						while (available_locations_temp.size() > 0 && num_successful < this->template nodes_per_zone<int>())
						{
							// get interface to the location
							int rand_loc_index = (int)(max<double>(0,(GLOBALS::Uniform_RNG.template Next_Rand<double>() - 0.0001)) * (double)available_locations_temp.size());
							destination_location_itf* loc = (destination_location_itf*)available_locations_temp[rand_loc_index];

							// If the location does not have valid links or is already in the std::list, skip
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
						//for (int i=0; i<this->template nodes_per_zone<int>(); i++)
						//{
						//	// get interface to the location
						//	int rand_loc_index = (int)((GLOBALS::Uniform_RNG.template Next_Rand<double>() - 0.0001) * (double)num_locations);
						//	destination_location_itf* loc = (destination_location_itf*)available_locations[rand_loc_index];

						//	// If the location does not have valid links or is already in the std::list, skip
						//	if (!loc->template Is_Routable_Location<bool>())
						//	{
						//		cout << "Location not routable: " << loc->template uuid<int>() <<", land use type: " << loc->land_use_type<Activity_Location_Components::Types::LAND_USE>()<<endl;
						//		continue;
						//	}
						//	destination_locations->push_back(loc);
						//	zone_destinations_count.find(orig_zone->template internal_id<long>())->second++;
						//}
					}
					if (zone_destinations_count.find(dest_zone->template internal_id<long>())->second == 0)
					{
						valid_skim = false;
						errors<<"destination zone '" << dest_zone->template uuid<long>() << "' has no valid activity locations, can not skim to this zone."<<endl;
					}
				}

				if (!valid_skim)
				{
//					THROW_EXCEPTION(errors.str());
				}

				// Based on the above selected O/D routing pairs, initialize the skim routers
				this_component()->template Initialize_Skims<TargetType>();

				// Load the skim updating event, which recalculates network skims at every Update interval (set in implementation)
				this_component()->template Load_Event<ComponentType>(Skim_Table_Update_Conditional,0,Types::SUB_ITERATIONS::INITIALIZE);

			}

			template<typename NetworkType, requires(NetworkType, check(NetworkType, is_pointer) && check_stripped_type(NetworkType, Network_Components::Concepts::Is_Transportation_Network_Prototype))>
			void Initialize(NetworkType network_pointer)
			{
				// set the network references
				this->template network_reference<NetworkType>(network_pointer);
				
				this->template Initialize<NT>();
			}

			template<typename NetworkType, requires(NetworkType, !check(NetworkType, is_pointer) || !check_stripped_type(NetworkType, Network_Components::Concepts::Is_Transportation_Network_Prototype))>
			void Initialize(NetworkType network_reference)
			{
				assert_check(NetworkType, is_pointer,"TargetType is not a pointer" );
				assert_check(strip_modifiers(NetworkType), Network_Components::Concepts::Is_Transportation_Network_Prototype, "TargetType is not a valid Transportation_Network interface");
				assert_sub_check(strip_modifiers(NetworkType), Network_Components::Concepts::Is_Transportation_Network_Prototype, is_basic_network, "TargetType is not a basic network");
				assert_sub_check(strip_modifiers(NetworkType), Network_Components::Concepts::Is_Transportation_Network_Prototype, has_turns, "TargetType does not have turns accessor");
				assert_sub_check(strip_modifiers(NetworkType), Network_Components::Concepts::Is_Transportation_Network_Prototype, has_locations, "TargetType does not have locations accessor");
				assert_sub_check(strip_modifiers(NetworkType), Network_Components::Concepts::Is_Transportation_Network_Prototype, has_zones, "TargetType does not have zones accessor");
			}

			template<typename TargetType> bool Update_Skim_Tables()
			{
				//// Update each modal_skim
				this->template Update_LOS<NULLTYPE>();

				// Output network skims
				if (this->template write_output<bool>())
				{
					this->template Write_LOS<NULLTYPE>();
				}

				// Network Skim timer
				cout << endl<<"Network Skimming run-time: " << this->template timer<Counter&>().Stop()<<endl<<endl;
				return true;
			}

			template<typename TargetType> TargetType update_increment()
			{
				return this_component()->template update_increment<TargetType>();
			}
			template<typename TargetType> TargetType update_increment(int interval)
			{
				return this_component()->template update_increment<TargetType>(interval);
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
				typedef Prototypes::Skim_Table<get_component_type(_skim_container_itf)> _skim_itf;

				//typedef (_skim_matrix_itf, _los_itf,typename _skim_itf::get_type_of(skim_table),Multidimensional_Random_Access_Array,Prototypes::LOS);
				typedef Multidimensional_Random_Access_Array<typename _skim_itf::get_type_of(skim_table)> _skim_matrix_itf;
				typedef Prototypes::LOS<get_component_type(_skim_matrix_itf)> _los_itf;
				
				// call the general get los function
				_los_itf* los_value = this->template Get_LOS<LocationType,TimeType,_los_itf*>(Origin, Destination, Start_Time);

				//Check for bad travel time
				if (los_value->template auto_ttime<Time_Seconds>() > END || los_value->template auto_ttime<Time_Seconds>() <0 || ISNAN(los_value->template auto_ttime<Time_Seconds>()))
				{
					int O = this->template Get_Zone_ID<LocationType>(Origin);
					int D = this->template Get_Zone_ID<LocationType>(Destination);
					THROW_WARNING("Error: travel time is invalid for auto mode, ttime="<<los_value->auto_ttime<Time_Seconds>()<<", O="<<O<<", D="<<D<<endl);
				}

				// extract and return the auto travel time
				if (Mode_Indicator == Vehicle_Components::Types::Vehicle_Type_Keys::SOV) return los_value->template auto_ttime<ReturnType>();
				else if (Mode_Indicator == Vehicle_Components::Types::Vehicle_Type_Keys::BICYCLE) return GLOBALS::Time_Converter.Convert_Value<Time_Hours,ReturnType>(los_value->template auto_distance<Miles>()/15.0); // assume bike speed of 15MPH
				else if (Mode_Indicator == Vehicle_Components::Types::Vehicle_Type_Keys::WALK) return GLOBALS::Time_Converter.Convert_Value<Time_Hours,ReturnType>(los_value->template auto_distance<Miles>()/3.0/0.9); // assume walk speed of 3MPH and reduce walk distance by 10%
				else return (los_value->template transit_ttime<ReturnType>() + los_value->template transit_wait_time<ReturnType>() + los_value->template transit_walk_access_time<ReturnType>());
				
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
				// create the references to network items and create the std::lists of origins/destination to route from/to
				typedef Network_Components::Prototypes::Network<typename get_type_of(network_reference)> network_itf;
				
				typedef Pair_Associative_Container<typename network_itf::get_type_of(zones_container)> zones_itf;
				typedef Zone_Components::Prototypes::Zone<get_mapped_component_type(zones_itf)> zone_itf;

				network_itf* network = this->template network_reference<network_itf*>();
				zones_itf* zones = network->template zones_container<zones_itf*>();
				typename zones_itf::iterator zone_itr;

				// convert the ids to indices
				zone_itf *orig_zone, *dest_zone;

				// Extract zone ID information from the input origin/destination type (either location or zone)
				int Origin_Zone_ID = this->template Get_Zone_ID<LocationType>(Origin);
				int Destination_Zone_ID = this->template Get_Zone_ID<LocationType>(Destination);

				// Do a lookup to make sure the zone is in the network (may be able to remove this)
				if ((zone_itr = zones->find(Origin_Zone_ID)) != zones->end()){ orig_zone = (zone_itf *)(zone_itr->second);}
				else THROW_EXCEPTION("ERROR, origin zone id: " << Origin_Zone_ID << " was not found for Origin uuid,internal_id: " << Origin->template uuid<int>()<<","<<Origin->template internal_id<int>());

				if ((zone_itr = zones->find(Destination_Zone_ID)) != zones->end()){ dest_zone = (zone_itf *)(zone_itr->second);}
				else THROW_EXCEPTION("ERROR, destination zone id: " << Destination_Zone_ID << " was not found for Destination uuid,internal_id: "<<Destination->template uuid<int>()<<","<<Destination->template internal_id<int>()<<", mem location: "<<Destination);

				//============================================================
				// Transferred code here from former mode_skim_prototype
				//-------------------------------------------------------------------
				typedef Random_Access_Sequence<typename get_type_of(skims_by_time_container)> _skim_container_itf;
				typedef Prototypes::Skim_Table<get_component_type(get_type_of(skims_by_time_container))> _skim_itf;

				//typedef (_skim_container_itf, _skim_itf,typename get_type_of(skims_by_time_container),Random_Access_Sequence,Prototypes::Skim_Table);
				_skim_container_itf* skims = this->skims_by_time_container<_skim_container_itf*>();
				typename _skim_container_itf::iterator itr = skims->begin();
				_skim_itf* skim_table;

				// get only the HH:MM:SS portion of requested time if Time > 1 day
				int days = ((int)(GLOBALS::Time_Converter.Convert_Value<TimeType,Time_Hours>(Start_Time))/24);
				TimeType rounded = GLOBALS::Time_Converter.Convert_Value<Time_Hours,TimeType>((float)days * 24.0);
				TimeType remain = Start_Time - rounded;
				
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
				cout << endl << "Get LOS failure: " <<"origin: " << Origin_Zone_ID <<", destination: " << Destination_Zone_ID<<", time: "  << Start_Time<<", remain time="<<remain<<", rounded="<<rounded<<endl;
				assert(false);
				return nullptr;
			}
			
			//---------------------------------------------
			// This function returns a list of zone-ids within range of a given origin at a specified time Get_Location_Indices_Within_Range 
			template<typename LocationType, typename TimeType, typename ModeType, typename ReturnLocationType> void Get_Locations_Within_Range(std::vector<ReturnLocationType>& available_set, LocationType origin, TimeType start_time, TimeType min_time, TimeType max_time, ModeType mode_indicator, bool search_forward=true, requires(ReturnLocationType, check(ReturnLocationType, is_pointer)))
			{
				available_set.clear();

				// create the references to network items and create the std::lists of origins/destination to route from/to
				typedef Network_Components::Prototypes::Network<typename get_type_of(network_reference)> network_itf;		
				typedef Pair_Associative_Container<typename network_itf::get_type_of(zones_container)> zones_itf;
				typedef Zone_Components::Prototypes::Zone<get_mapped_component_type(zones_itf)> zone_itf;

				network_itf* network = this->template network_reference<network_itf*>();
				zones_itf* zones = network->template zones_container<zones_itf*>();
				typename zones_itf::iterator zone_itr;

				// convert the ids to indices
				zone_itf *orig_zone;

				// Extract zone ID information from the input origin/destination type (either location or zone)
				int Origin_Zone_ID = this->template Get_Zone_ID<LocationType>(origin);

				//TODO: remove when done test.  Do a lookup to make sure the zone is in the network (may be able to remove this)
				if ((zone_itr = zones->find(Origin_Zone_ID)) != zones->end()){ orig_zone = (zone_itf *)(zone_itr->second);}
				else THROW_EXCEPTION("ERROR, origin zone id: " << Origin_Zone_ID << " was not found for Origin uuid,internal_id: " << origin->template uuid<int>()<<","<<origin->template internal_id<int>());

				//============================================================
				// Transferred code here from former mode_skim_prototype
				//-------------------------------------------------------------------
				typedef Random_Access_Sequence<typename get_type_of(skims_by_time_container)> _skim_container_itf;
				typedef Prototypes::Skim_Table<get_component_type(get_type_of(skims_by_time_container))> _skim_itf;

				//typedef (_skim_container_itf, _skim_itf,typename get_type_of(skims_by_time_container),Random_Access_Sequence,Prototypes::Skim_Table);
				_skim_container_itf* skims = this->skims_by_time_container<_skim_container_itf*>();
				typename _skim_container_itf::iterator itr = skims->begin();
				_skim_itf* skim_table;

				// get only the HH:MM:SS portion of requested time if Time > 1 day
				int days = ((int)(GLOBALS::Time_Converter.Convert_Value<TimeType,Time_Hours>(start_time))/24);
				TimeType rounded = GLOBALS::Time_Converter.Convert_Value<Time_Hours,TimeType>((float)days * 24.0);
				TimeType remain = start_time - rounded;
				
				// go to skim table for requested time period
				for (; itr != skims->end(); ++itr)
				{
					skim_table = *itr;
					if (skim_table->template end_time<TimeType>() > remain)
					{
						////
						skim_table->Get_Locations_Within_Range<TimeType,ModeType,ReturnLocationType>(available_set,orig_zone->template internal_id<int>(),min_time,max_time, mode_indicator, search_forward);
						
						return;
					}
				}
			}

			template<typename TargetType> bool Update_LOS()
			{
				typedef Random_Access_Sequence<typename get_type_of(skims_by_time_container)> _skim_container_itf;
				typedef Prototypes::Skim_Table<get_component_type(_skim_container_itf)> _skim_itf;

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

				// we know the initial skim update is done or read is done by here, so now update the accessibilities
				if (iteration()==0) this->Update_Zone_Accessibilities<TargetType>();

				return true;
			}
			template<typename TargetType> void Write_LOS()
			{
				typedef Random_Access_Sequence<typename get_type_of(skims_by_time_container)> _skim_container_itf;
				typedef Prototypes::Skim_Table<get_component_type(_skim_container_itf)> _skim_itf;

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
			template<typename TargetType> void Update_Zone_Accessibilities()
			{
				this_component()->Update_Zone_Accessibilities<TargetType>();
			}

			template<typename TargetType> void Read_Binary_Headers(int& num_modes, int& num_zones, TargetType intervals, bool perform_checks)
			{
				this_component()->Read_Binary_Headers(num_modes, num_zones, intervals, perform_checks);
			}
			template<typename TargetType> void Read_Binary_Data(TargetType data_ptr, File_IO::Binary_File_Reader& file, int num_zones, requires(TargetType,check(TargetType,is_pointer) && check(strip_modifiers(TargetType),is_arithmetic)))
			{
				this_component()->template Read_Binary_Data<TargetType>(data_ptr, file, num_zones);
			}

			typedef typename ComponentType::Master_Type MasterType;

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
			}			

			template<typename TargetType> void Initialize(TargetType initial_data, requires(TargetType,check(TargetType, is_pointer)))
			{
				// call implementation initializer
				this_component()->template Initialize<TargetType>(initial_data);
			}	

			template<typename TargetType> bool Process_Skims()
			{
			}

			template<typename TargetType> bool Update_LOS()
			{
				return this_component()->template Update_LOS<TargetType>();
			}

			template<typename TargetType> void Write_LOS()
			{
				this_component()->template Write_LOS<TargetType>();
			}

			template<typename ParamType, typename ReturnType> ReturnType Get_LOS(ParamType Origin_Index, ParamType Destination_Index)
			{
				ReturnType ret_value = this_component()->Get_LOS<ParamType, ReturnType>(Origin_Index,Destination_Index);
				return ret_value;
			}

			template<typename TimeType, typename ModeType, typename ReturnLocationType> void Get_Locations_Within_Range(std::vector<ReturnLocationType>& available_set, int origin_index, TimeType min_time, TimeType max_time, ModeType mode_indicator, bool search_forward, requires(ReturnLocationType, check(ReturnLocationType, is_pointer)))
			{
				this_component()->Get_Locations_Within_Range<TimeType, ModeType, ReturnLocationType>(available_set, origin_index, min_time, max_time, mode_indicator, search_forward);
			}
			//template<typename TimeType, typename ModeType, typename ReturnLocationType> void Get_Locations_Within_Range(std::vector<Pair<float,ReturnLocationType>>& available_set, int& avail_index_low, int& avail_index_high, int origin_index, TimeType min_time, TimeType max_time, ModeType mode_indicator, bool search_forward, requires(ReturnLocationType, check(ReturnLocationType, is_pointer)))
			//{
			//	this_component()->Get_Locations_Within_Range<TimeType, ModeType, ReturnLocationType>(available_set, avail_index_low, avail_index_high, origin_index, min_time, max_time, mode_indicator, search_forward);
			//}

		};


	}
}

