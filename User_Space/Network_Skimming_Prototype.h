#pragma once
#include "User_Space.h"

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

		prototype struct Network_Skimming_Prototype : ComponentType
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
					response.next._iteration = Simulation_Time.Future_Time<Simulation_Timestep_Increment,Simulation_Timestep_Increment>(this_ptr->update_increment<Simulation_Timestep_Increment>());
					response.next._sub_iteration = 0;
					response.result = true;
				}
				else
				{
					response.next._iteration = Simulation_Time.Future_Time<Simulation_Timestep_Increment,Simulation_Timestep_Increment>(this_ptr->update_increment<Simulation_Timestep_Increment>());
					response.next._sub_iteration = 0;
					response.result = true;
				}
			}
			declare_feature_event(Process_Skim_Trees_Event)
			{

			}
			declare_feature_event(Update_Skim_Tables_Event)
			{
				typedef Network_Skimming_Prototype<ComponentType, CallerType> _Skim_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_Skim_Interface* this_ptr=(_Skim_Interface*)_pthis;

				this_ptr->Update_Skim_Tables<NULLTYPE>();
			}


			//=============================================
			// Primary data accessors
			//---------------------------------------------
			// links back to the network to be skimmed
			feature_accessor(network_reference, check(ReturnValueType,Network_Components::Concepts::Is_Transportation_Network_Prototype), check(SetValueType,Network_Components::Concepts::Is_Transportation_Network_Prototype) || check(SetValueType,Network_Components::Concepts::Is_Transportation_Network_Prototype));
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
			feature_accessor(destination_link_to_zone_map,none,none);

			//=============================================
			// Primary function accessors - used to pass through to the specific skimm table based on time-key
			//---------------------------------------------
			feature_prototype void Initialize()
			{
				// create the lists of origins/destination to route from/to
				define_component_interface(network_itf,get_type_of(network_reference),Network_Components::Prototypes::Network_Prototype,ComponentType);
				network_itf* network = this->network_reference<network_itf*>();

				// create the references to network items
				define_container_and_value_interface(zones_itf,zone_itf,network_itf::get_type_of(zones_container),Random_Access_Sequence_Prototype,Zone_Components::Prototypes::Zone_Prototype,ComponentType);
				define_container_and_value_interface(locations_itf,location_itf,zone_itf::get_type_of(origin_activity_locations),Random_Access_Sequence_Prototype,Activity_Location_Components::Prototypes::Activity_Location_Prototype,ComponentType);
				define_container_and_value_interface(links_itf,link_itf,location_itf::get_type_of(origin_links),Random_Access_Sequence_Prototype,Activity_Location_Components::Prototypes::Activity_Location_Prototype,ComponentType);
				define_container_and_value_interface(origin_map_itf,origin_item_itf,get_type_of(origin_node_to_zone_map), Containers::Associative_Container_Prototype,Prototypes::Location_To_Zone_Map_Item_Prototype,CallerType);
				define_container_and_value_interface(destination_map_itf,destination_item_itf,get_type_of(destination_link_to_zone_map), Containers::Associative_Container_Prototype,Prototypes::Location_To_Zone_Map_Item_Prototype,CallerType);
				origin_map_itf* origin_map = this->origin_node_to_zone_map<origin_map_itf*>();
				destination_map_itf* destination_map = this->destination_link_to_zone_map<destination_map_itf*>();
				zones_itf* zones_container = network->zones_container<zones_itf*>();

				// Loop through zones, choose origin,destination points to route from, and add to maps
				zones_itf::iterator itr;
				for (itr= zones_container->begin();itr != zones_container->end(); ++itr)
				{
					zone_itf* orig_zone = *itr;
					location_itf* orig_node = *(orig_zone->origin_activity_locations<locations_itf*>()->begin());
					origin_item_itf* orig_item = (origin_item_itf*)Allocate<get_type_of(origin_node_to_zone_map)::unqualified_value_type>();
					orig_item->initialize<Target_Type<NULLTYPE,long,float>>(orig_node->internal_id<long>(),orig_zone->internal_id<long>(),this->nodes_per_zone<float>());
					origin_map->insert(pair<long,origin_item_itf*>(orig_node->internal_id<long>(),orig_item));
				}
				for (itr = zones_container->begin();itr != zones_container->end(); ++itr)
				{
					zone_itf* dest_zone = *itr;
					location_itf* dest_node = *(dest_zone->origin_activity_locations<locations_itf*>()->begin());
					link_itf* dest_link = *(dest_node->destination_links<links_itf*>()->begin());
					destination_item_itf* dest_item = (destination_item_itf*)Allocate<get_type_of(destination_link_to_zone_map)::unqualified_value_type>();
					dest_item->initialize<Target_Type<NULLTYPE,long,float>>(dest_link->internal_id<long>(),dest_zone->internal_id<long>(),this->nodes_per_zone<float>());
					destination_map->insert(pair<long,destination_item_itf*>(dest_link->internal_id<long>(),dest_item));
				}

				// this sets the implementation specific data members
				this_component()->Initialize<ComponentType,CallerType,TargetType>();

				load_event(ComponentType,Skim_Table_Update_Conditional,Update_Skim_Tables_Event,0,Types::SUB_ITERATIONS::INITIALIZE,NULLTYPE);
			}
			feature_prototype void Initialize(TargetType network_reference, requires(check_as_given(TargetType, is_pointer) && check(TargetType, Network_Components::Concepts::Is_Transportation_Network_Prototype)))
			{
				// set the network references
				this->network_reference<TargetType>(network_reference);
				
				this->Initialize<NULLTYPE>();
			}			
			feature_prototype void Initialize(TargetType network_reference, requires(!check_as_given(TargetType, is_pointer) || !check(TargetType, Network_Components::Concepts::Is_Transportation_Network_Prototype)))
			{
				assert_check_as_given(TargetType, is_pointer,"TargetType is not a pointer");
				assert_check(TargetType, Network_Components::Concepts::Is_Transportation_Network_Prototype, "TargetType is not a valid Transportation_Network interface");
				assert_sub_check(TargetType, Network_Components::Concepts::Is_Transportation_Network_Prototype, is_basic_network, "TargetType is not a basic network");
				assert_sub_check(TargetType, Network_Components::Concepts::Is_Transportation_Network_Prototype, has_turns, "TargetType does not have turns accessor");
				assert_sub_check(TargetType, Network_Components::Concepts::Is_Transportation_Network_Prototype, has_locations, "TargetType does not have locations accessor");
				assert_sub_check(TargetType, Network_Components::Concepts::Is_Transportation_Network_Prototype, has_zones, "TargetType does not have zones accessor");
			}			
			feature_prototype void Initialize(typename TargetType::ParamType input_file, typename TargetType::Param2Type network_reference, requires(check_as_given(typename TargetType::Param2Type, is_pointer) && check(typename TargetType::Param2Type, Network_Components::Concepts::Is_Transportation_Network_Prototype)))
			{
				//this->Initialize<NULLTYPE>();
				//this_component()->Initialize<ComponentType,CallerType,TargetType>(input_file, network_reference);
				//load_event(ComponentType,Skim_Table_Update_Conditional,Update_Skims_Event,0,Types::SUB_ITERATIONS::PROCESS,NULLTYPE);
			}
			feature_prototype void Initialize(typename TargetType::ParamType input_file, typename TargetType::Param2Type network_reference, requires(!check_as_given(typename TargetType::Param2Type, is_pointer) || !check(typename TargetType::Param2Type, Network_Components::Concepts::Is_Transportation_Network_Prototype)))
			{
				assert_check_as_given(TargetType, is_pointer,"TargetType is not a pointer");
				assert_check(TargetType, Network_Components::Concepts::Is_Transportation_Network_Prototype, "TargetType is not a valid Transportation_Network interface");
			}
			
			feature_prototype bool Process_Skim_Trees()
			{
				return this_component()->Process_Skim_Trees<ComponentType,CallerType,TargetType>();
			}
			feature_prototype bool Update_Skim_Tables()
			{
				define_container_and_value_interface(_skim_container_itf, _skim_itf,get_type_of(mode_skim_table_container),Containers::Associative_Container_Prototype,Prototypes::Mode_Skim_Table_Prototype,CallerType);
				_skim_container_itf* skim = this->mode_skim_table_container<_skim_container_itf*>();

				for (_skim_container_itf::iterator itr = skim->begin(); itr != skim->end(); ++itr)
				{
					itr->second->Update_LOS<NULLTYPE>();
				}
				return true;
			}

			//=============================================
			// Primary function accessors - used to pass through to the specific skimm table based on time-key
			//---------------------------------------------
			// This returns the travel time based on free-flow speed
			feature_prototype typename TargetType::ReturnType Get_LOS(typename TargetType::ParamType Origin_ID, typename TargetType::ParamType Destination_ID, typename TargetType::Param2Type Mode_Indicator, requires(check(typename TargetType::ReturnType, Basic_Units::Concepts::Is_Time_Value)))
			{
			}
			// This returns the travel time based on the current simulation time
			feature_prototype typename TargetType::ReturnType Get_Current_LOS(typename TargetType::ParamType Origin_ID, typename TargetType::ParamType Destination_ID, typename TargetType::Param2Type Mode_Indicator, requires(check(typename TargetType::ReturnType, Basic_Units::Concepts::Is_Time_Value)))
			{
			}
			// This returns the travel time during a specific time interval
			feature_prototype typename TargetType::ReturnType Get_LOS(typename TargetType::ParamType Origin_ID, typename TargetType::ParamType Destination_ID, typename TargetType::Param2Type Mode_Indicator, typename TargetType::Param3Type Start_Time, requires(check(typename TargetType::ReturnType, Basic_Units::Concepts::Is_Time_Value)))
			{
				// example fo a possible function to direct to appropriate skim_table (needs to be updated with interfaces, etc.
				//this->skim_tables_container<skim_tables_container::iterator&>().find(Intervalized_Start_Time)->second->Get_LOS<TargetType>(Origin_ID, Destination_ID,Mode_Indicator);
			}

		};

		prototype struct Mode_Skim_Table_Prototype
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

			//=============================================
			// Primary function accessors - used to calculate/return LOS values for OD pairs
			//---------------------------------------------
			feature_prototype void Initialize()
			{
				this_component()->Initialize<ComponentType,CallerType,TargetType>();
			}				
			feature_prototype void Initialize(typename TargetType::ParamType input_file, typename TargetType::Param2Type, requires(check_as_given(typename TargetType::Param2Type, is_pointer) && check(typename TargetType::Param2Type, Network_Components::Concepts::Is_Transportation_Network_Prototype)))
			{
				this_component()->Initialize<ComponentType,CallerType,TargetType>(input_file, network_reference);
			}
			feature_prototype void Initialize(typename TargetType::ParamType input_file, typename TargetType::Param2Type, requires(!check_as_given(typename TargetType::Param2Type, is_pointer) || !check(typename TargetType::Param2Type, Network_Components::Concepts::Is_Transportation_Network_Prototype)))
			{
				assert_check_as_given(TargetType, is_pointer,"TargetType is not a pointer");
				assert_check(TargetType, Network_Components::Concepts::Is_Transportation_Network_Prototype, "TargetType is not a valid Transportation_Network interface");
			}
			
			feature_prototype bool Process_Skims()
			{
			}
			feature_prototype bool Update_LOS()
			{
				define_container_and_value_interface(_skim_container_itf, _skim_itf,get_type_of(skims_by_time_container),Containers::Random_Access_Sequence_Prototype,Prototypes::Skim_Table_Prototype,CallerType);
				_skim_container_itf* skim = this->skims_by_time_container<_skim_container_itf*>();

				for (_skim_container_itf::iterator itr = skim->begin(); itr != skim->end(); ++itr)
				{
					(*itr)->Update_LOS<NULLTYPE>();
				}
				return true;
			}
			feature_prototype typename TargetType::ReturnType Get_LOS(typename TargetType::ParamType Origin_ID, typename TargetType::ParamType Destination_ID, typename TargetType::Param2Type Mode_Indicator)
			{

			}
		};

		prototype struct Skim_Table_Prototype
		{
			tag_as_prototype;

			// Associative Container of skim matrices, keyed on Mode Indicator values
			feature_accessor(skim_table,none,none);
			// Associative Container of skim matrices, keyed on Mode Indicator values
			feature_accessor(path_trees_container,none,none);
			// links back to the network to be skimmed
			feature_accessor(network_reference, none, none);
			// links back to the skimming faculty
			feature_accessor(skim_reference, none, none);
			// number of nodes selected per zone used to estimate zonal travel times
			feature_accessor(nodes_per_zone,none,none);
			// simulated day start and end times represented by skim_table
			feature_accessor(start_time,none,none);
			feature_accessor(end_time,none,none);

			//=============================================
			// Primary function accessors - used to calculate/return LOS values for OD pairs
			//---------------------------------------------
			feature_prototype void Initialize()
			{
				// set the network reference
				define_simple_container_interface(skim_table_itf,get_type_of(skim_table),Containers::Multidimensional_Random_Access_Array_Prototype,get_type_of(skim_table)::unqualified_value_type,ComponentType);
				define_component_interface(network_itf,get_type_of(network_reference),Network_Components::Prototypes::Network_Prototype,ComponentType);
				define_component_interface(skimmer_itf,get_type_of(skim_reference),Prototypes::Network_Skimming_Prototype,ComponentType);
				network_itf* network = this->network_reference<network_itf*>();
				skimmer_itf* skim = this->skim_reference<skimmer_itf*>();
				
				// create the LOS container
				define_container_and_value_interface(zones_itf,zone_itf,network_itf::get_type_of(zones_container),Random_Access_Sequence_Prototype,Zone_Components::Prototypes::Zone_Prototype,ComponentType);
				define_container_and_value_interface(locations_itf,location_itf,zone_itf::get_type_of(origin_activity_locations),Random_Access_Sequence_Prototype,Activity_Location_Components::Prototypes::Activity_Location_Prototype,ComponentType);
				define_container_and_value_interface(links_itf,link_itf,location_itf::get_type_of(origin_links),Random_Access_Sequence_Prototype,Activity_Location_Components::Prototypes::Activity_Location_Prototype,ComponentType);
				define_container_and_value_interface(origin_map_itf,origin_item_itf,skimmer_itf::get_type_of(origin_node_to_zone_map), Containers::Associative_Container_Prototype,Prototypes::Location_To_Zone_Map_Item_Prototype,CallerType);
				define_container_and_value_interface(destination_map_itf,destination_item_itf,skimmer_itf::get_type_of(destination_link_to_zone_map), Containers::Associative_Container_Prototype,Prototypes::Location_To_Zone_Map_Item_Prototype,CallerType);
				origin_map_itf* origin_map = skim->origin_node_to_zone_map<origin_map_itf*>();
				destination_map_itf* destination_map = skim->destination_link_to_zone_map<destination_map_itf*>();		
				zones_itf* zones_container = network->zones_container<zones_itf*>();
				origin_map_itf::iterator orig_itr = origin_map->begin();
				destination_map_itf::iterator dest_itr = destination_map->begin();
				locations_itf* activity_locations = network->activity_locations_container<locations_itf*>();


				// create the LOS matrix
				typedef matrix<double>::size_type size_t;
				skim_table_itf* los = this->skim_table<skim_table_itf*>();
				los->resize(pair<size_t,size_t>((size_t)zones_container->size(),(size_t)zones_container->size()), 0);

				// tree builder interface
				define_container_and_value_interface(tree_builder_list_itf, tree_builder_itf, get_type_of(path_trees_container),Associative_Container_Prototype,Routing_Components::Prototypes::Routing_Prototype,ComponentType);
				tree_builder_list_itf* tree_list = this->path_trees_container<tree_builder_list_itf*>();
				
				// create a tree builder for each origin to skim
				for (;orig_itr != origin_map->end(); ++orig_itr)
				{
					// get the origin/destination indices
					origin_item_itf* orig_item = orig_itr->second;			
					long orig = orig_item->loc_index<long>();
					location_itf* orig_node = activity_locations->at(orig);

					// Allocate a tree_builder for each origin node		
					tree_builder_itf* tree_builder = (tree_builder_itf*)Allocate<MasterType::routing_type>();

					// Set the current routable origin for the tree builder
					tree_builder->routable_origin<link_itf*>(*(orig_node->origin_links<links_itf*>()->begin()));
					tree_builder->network<network_itf*>(network);

					// Set the start, end and update times for the router
					tree_builder->update_increment<Simulation_Timestep_Increment>(skim->update_increment<Simulation_Timestep_Increment>());
					tree_builder->start_time<Simulation_Timestep_Increment>(this->start_time<Simulation_Timestep_Increment>());
					tree_builder->end_time<Simulation_Timestep_Increment>(this->end_time<Simulation_Timestep_Increment>());

					// Add the tree_builder to the list for future processing
					tree_builder->Initialize_Tree_Computation<NULLTYPE>(_iteration);
					tree_list->insert(pair<long,tree_builder_itf*>(orig,tree_builder));
				}
			}			

			feature_prototype bool Process_Skims()
			{
			}

			feature_prototype bool Update_LOS()
			{
				// get the network and network element references
				define_simple_container_interface(skim_table_itf,get_type_of(skim_table),Containers::Multidimensional_Random_Access_Array_Prototype,get_type_of(skim_table)::unqualified_value_type,ComponentType);
				define_component_interface(network_itf,get_type_of(network_reference),Network_Components::Prototypes::Network_Prototype,ComponentType);
				define_component_interface(skimmer_itf,get_type_of(skim_reference),Prototypes::Network_Skimming_Prototype,ComponentType);
				/*define_container_and_value_interface(locations_itf,location_itf,zone_itf::get_type_of(origin_activity_locations),Random_Access_Sequence_Prototype,Activity_Location_Components::Prototypes::Activity_Location_Prototype,ComponentType);
				define_container_and_value_interface(destinations_itf,destination_itf,zone_itf::get_type_of(destination_activity_locations),Random_Access_Sequence_Prototype,Link_Components::Prototypes::Link_Prototype,ComponentType);
				define_container_and_value_interface(links_itf,link_itf,location_itf::get_type_of(origin_links),Random_Access_Sequence_Prototype,Activity_Location_Components::Prototypes::Activity_Location_Prototype,ComponentType);
				*/
				network_itf* network = this->network_reference<network_itf*>();
				skimmer_itf* skim = this->skim_reference<skimmer_itf*>();

				// origin to zone / destination to zone mapping
				define_container_and_value_interface(origin_map_itf,origin_item_itf,skimmer_itf::get_type_of(origin_node_to_zone_map), Containers::Associative_Container_Prototype,Prototypes::Location_To_Zone_Map_Item_Prototype,CallerType);
				define_container_and_value_interface(destination_map_itf,destination_item_itf,skimmer_itf::get_type_of(destination_link_to_zone_map), Containers::Associative_Container_Prototype,Prototypes::Location_To_Zone_Map_Item_Prototype,CallerType);
				origin_map_itf* origin_map = skim->origin_node_to_zone_map<origin_map_itf*>();
				destination_map_itf* destination_map = skim->destination_link_to_zone_map<destination_map_itf*>();		
				origin_map_itf::iterator orig_itr = origin_map->begin();
				destination_map_itf::iterator dest_itr = destination_map->begin();

				// get reference to the routers used to create path-trees from each origin
				define_container_and_value_interface(path_trees_itf,path_tree_itf,get_type_of(path_trees_container),Associative_Container_Prototype,Routing_Components::Prototypes::Routing_Prototype,ComponentType);
				path_trees_itf* trees_container = this->path_trees_container<path_trees_itf*>();
				path_trees_itf::iterator tree_itr = trees_container->begin();	
					
				// reference to the LOS skim table and its iterator	
				skim_table_itf* los = this->skim_table<skim_table_itf*>();
				skim_table_itf::iterator skim_table_itr = los->begin();

				// loop through each tree router, extract travel time info and place into los_skim
				for (;tree_itr != trees_container->end(); ++tree_itr)
				{
					long orig = tree_itr->first;
					path_tree_itf* tree = tree_itr->second;

					// find the origin reference in origin map and get the zone index of the origin
					if ((orig_itr = origin_map->find(orig)) == origin_map->end()) assert(true);
					long orig_zone_index = orig_itr->second->zone_index<long>();
					

					// search each link for required links from destination_map and update LOS table with the results
					for (dest_itr = destination_map->begin(); dest_itr !=destination_map->end(); ++dest_itr)
					{
						long dest_zone_index = dest_itr->second->zone_index<long>();
						(*los)[pair<size_t,size_t>(orig_zone_index,dest_zone_index)] = tree->Get_Tree_Results_For_Destination<Time_Minutes>(dest_zone_index);
					}
				}

				los->print(cout);

				return true;
			}

			feature_prototype typename TargetType::ReturnType Get_LOS(typename TargetType::ParamType Origin_ID, typename TargetType::ParamType Destination_ID, typename TargetType::Param2Type Mode_Indicator)
			{
			}
		};


		prototype struct Location_To_Zone_Map_Item_Prototype : ComponentType
		{
			tag_as_prototype;
			feature_prototype void initialize(typename TargetType::ParamType loc_index, typename TargetType::ParamType zone_index, typename TargetType::Param2Type weight)
			{
				this_component()->initialize<ComponentType,CallerType,TargetType>(loc_index,zone_index,weight);
			}
			feature_accessor(loc_index,none,none);
			feature_accessor(zone_index,none,none);
			feature_accessor(weight,none,none);
		};
	}
}
