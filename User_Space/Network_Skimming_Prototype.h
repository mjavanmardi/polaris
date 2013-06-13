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
			feature_accessor(timer,none,none);
			feature_accessor(read_input,none,none);
			feature_accessor(write_output,none,none);
			feature_accessor(input_file,none,none);
			feature_accessor(output_file,none,none);

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
				origin_map_itf* origin_map = this->template origin_node_to_zone_map<origin_map_itf*>();
				destination_map_itf* destination_map = this->template destination_node_to_zone_map<destination_map_itf*>();
				zones_itf* zones_container = network->template zones_container<zones_itf*>();

				// Loop through zones, choose origin,destination points to route from/to, and add to maps
				typename zones_itf::iterator itr;
				typename locations_itf::iterator loc_itr;
				for (itr= zones_container->begin();itr != zones_container->end(); ++itr)
				{
					zone_itf* orig_zone = itr->second;

					// get the first location in the zone with a valid origin link
					loc_itr = orig_zone->template origin_activity_locations<locations_itf*>()->begin();
					while (loc_itr != orig_zone->template origin_activity_locations<locations_itf*>()->end())
					{
						location_itf* loc = *loc_itr;
						typename links_itf::iterator link_itr = loc->template origin_links<links_itf*>()->begin();
						if (link_itr != loc->template origin_links<links_itf*>()->end()	)
						{
							link_itf* link = *link_itr;
							if (link->template outbound_turn_movements<turns_itf*>()->size() > 0) break;
						}
						++loc_itr;
					}

					if (loc_itr == orig_zone->template origin_activity_locations<locations_itf*>()->end()) 
					{
						//THROW_WARNING("warning, origin zone " << orig_zone->template internal_id<int>() << "has no activity locations associated with it, or all activity locations have no valid origin links associated with them.");
					}
					else
					{
						location_itf* orig_node = *loc_itr;
						origin_item_itf* orig_item = (origin_item_itf*)Allocate<typename get_type_of(origin_node_to_zone_map)::unqualified_value_type>();
						orig_item->template initialize<Target_Type<NULLTYPE,NULLTYPE,long,float>>(orig_node->template internal_id<long>(),orig_zone->template internal_id<long>(),this->template nodes_per_zone<float>());
					
						pair<long,origin_item_itf*> item = pair<long,origin_item_itf*>(orig_node->template internal_id<long>(),orig_item);
						//origin_map->insert(pair<long,origin_item_itf*>(orig_node->template internal_id<long>(),orig_item));
						//cout << endl << "Insert item: "<<orig_node->template internal_id<long>() << ", uuid is "<<orig_node->uuid<long>();
						origin_map->insert(item);
					}
				}
				for (itr = zones_container->begin();itr != zones_container->end(); ++itr)
				{
					zone_itf* dest_zone = itr->second;

					loc_itr = dest_zone->template origin_activity_locations<locations_itf*>()->begin();
					while (loc_itr != dest_zone->template origin_activity_locations<locations_itf*>()->end())
					{
						location_itf* loc = *loc_itr;
						typename links_itf::iterator link_itr = loc->template origin_links<links_itf*>()->begin();
						if (link_itr != loc->template origin_links<links_itf*>()->end()	)
						{
							link_itf* link = *link_itr;
							if (link->template outbound_turn_movements<turns_itf*>()->size() > 0) break;
						}
						++loc_itr;
					}

					if (loc_itr == dest_zone->template origin_activity_locations<locations_itf*>()->end()) 
					{
						//THROW_WARNING("warning, destination zone " << dest_zone->template internal_id<int>() << "has no activity locations associated with it");
					}
					else
					{
						location_itf* dest_node = *(dest_zone->template origin_activity_locations<locations_itf*>()->begin());
						destination_item_itf* dest_item = (destination_item_itf*)Allocate<typename get_type_of(destination_node_to_zone_map)::unqualified_value_type>();
						dest_item->template initialize<Target_Type<NULLTYPE,NULLTYPE,long,float>>(dest_node->template internal_id<long>(),dest_zone->template internal_id<long>(),this->template nodes_per_zone<float>());
					
						pair<long,destination_item_itf*> item = pair<long,destination_item_itf*>(dest_node->template internal_id<long>(),dest_item);
						//destination_map->insert(pair<long,destination_item_itf*>(dest_node->template internal_id<long>(),dest_item));
						destination_map->insert(item);
					}
				}

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

				// Network Skim timer
				cout << endl<<"Network Skimming run-time: " << this->template timer<Counter&>().Stop();

				// Output network skims
				for (typename _skim_container_itf::iterator itr = skim->begin(); itr != skim->end(); ++itr)
				{
					if (this->write_output<bool>())
					{
						itr->second->template Write_LOS<NULLTYPE>();
					}
				}

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
					if ((*itr)->template start_time<Simulation_Timestep_Increment>() >= _iteration) 
					{
						(*itr)->template Update_LOS<NULLTYPE>();
						break;
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
				define_component_interface(mode_skimmer_itf,typename get_type_of(mode_skim_reference),Prototypes::Mode_Skim_Table_Prototype,ComponentType);
				define_container_and_value_interface(locations_itf,location_itf,typename network_itf::get_type_of(activity_locations_container),Random_Access_Sequence_Prototype,Activity_Location_Components::Prototypes::Activity_Location_Prototype,ComponentType);
				/*define_container_and_value_interface(destinations_itf,destination_itf,zone_itf::get_type_of(destination_activity_locations),Random_Access_Sequence_Prototype,Link_Components::Prototypes::Link_Prototype,ComponentType);*/
				define_container_and_value_interface(links_itf,link_itf,typename location_itf::get_type_of(origin_links),Random_Access_Sequence_Prototype,Activity_Location_Components::Prototypes::Activity_Location_Prototype,ComponentType);
				
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

				// get reference to the routers used to create path-trees from each origin
				define_container_and_value_interface(path_trees_itf,path_tree_itf,typename mode_skimmer_itf::get_type_of(path_trees_container),Associative_Container_Prototype,Routing_Components::Prototypes::Routing_Prototype,ComponentType);
				path_trees_itf* trees_container = mode_skim->template path_trees_container<path_trees_itf*>();
				typename path_trees_itf::iterator tree_itr = trees_container->begin();	
					
				// reference to the LOS skim table and its iterator	
				skim_table_itf* los = this->skim_table<skim_table_itf*>();
				typedef typename skim_table_itf::size_type size_t;
				typename skim_table_itf::iterator skim_table_itr = los->begin();

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
						long dest_zone_index = dest->template zone_index<long>();
						(*los)[pair<size_t,size_t>(orig_zone_index,dest_zone_index)] = tree->template Get_Tree_Results_For_Destination<typename skimmer_itf::Component_Type::Stored_Time_Type>(dest_link_index);

						//cout << (*activity_locations)[orig_itr->second->loc_index<long>()]->uuid<int>() << ", " << (*activity_locations)[dest_node_index]->uuid<int>() << ", ";
						//cout << network->links_container<links_itf&>()[dest_link_index]->uuid<int>() << ", ";
						//cout << (*los)[pair<size_t,size_t>(orig_zone_index,dest_zone_index)]<<endl;
					}
				}

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
