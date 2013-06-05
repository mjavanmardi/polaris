#pragma once

#include "Network_Skimming_Prototype.h"


namespace Network_Skimming_Components
{
	namespace Types
	{

	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{
		implementation struct Location_To_Zone_Map_Item : public Polaris_Component<APPEND_CHILD(Location_To_Zone_Map_Item),MasterType,Data_Object,ParentType>
		{
			// Tag as implementation
			typedef typename Polaris_Component<APPEND_CHILD(Location_To_Zone_Map_Item),MasterType,Data_Object>::Component_Type ComponentType;

			feature_implementation void initialize(typename TargetType::ParamType loc_index, typename TargetType::ParamType zone_index, typename TargetType::Param2Type weight)
			{
				this->template loc_index<ComponentType,ComponentType,typename TargetType::ParamType>(loc_index);
				this->template zone_index<ComponentType,ComponentType,typename TargetType::ParamType>(zone_index);
				this->template weight<ComponentType,ComponentType,typename TargetType::ParamType>(weight);
			}
			member_data(long, loc_index,check(ReturnValueType,is_integral),none);
			member_data(long, zone_index,check(ReturnValueType,is_integral),none);
			member_data(double, weight,check(ReturnValueType,is_arithmetic),none);
		};

		//======================================================================================
		/// Skim table which holds the values for LOS
		//--------------------------------------------------------------------------------------
		implementation struct Skim_Table_Implementation : public Polaris_Component<APPEND_CHILD(Skim_Table_Implementation),MasterType,Data_Object,ParentType>
		{
			// Tag as implementation
			typedef typename Polaris_Component<APPEND_CHILD(Skim_Table_Implementation),MasterType,Data_Object>::Component_Type ComponentType;


			member_container(matrix<float>, skim_table, none,none);
			

			// start and end times of the period represented by the skim table
			member_data_component(Basic_Units::Implementations::Time_Implementation<MasterType>,_start_time,none,none);
			member_component_feature(start_time,_start_time,Value,Basic_Units::Prototypes::Time_Prototype);
			member_data_component(Basic_Units::Implementations::Time_Implementation<MasterType>,_end_time,none,none);
			member_component_feature(end_time,_end_time,Value,Basic_Units::Prototypes::Time_Prototype);
			
			// Link back to the parent network_skimmer class
			member_component(typename MasterType::network_skim_type, skim_reference, none, none);
			member_prototype(Prototypes::Mode_Skim_Table_Prototype, mode_skim_reference, typename MasterType::network_mode_skim_type,none, none);

			// links back to the network to be skimmed
			member_component(typename MasterType::network_type, network_reference, none, none);

			//=============================================
			// Primary function accessors - used to calculate/return LOS values for OD pairs
			//---------------------------------------------
			feature_implementation void Initialize()
			{
			}
			feature_implementation bool Update_LOS()
			{
			}
			feature_implementation typename TargetType::ReturnType Get_LOS(typename TargetType::ParamType Origin_ID, typename TargetType::ParamType Destination_ID, typename TargetType::Param2Type Mode_Indicator)
			{		
			}
		};


		//======================================================================================
		/// A collection of skim_tables for a given mode
		//--------------------------------------------------------------------------------------
		implementation struct Mode_Skim_Table_Implementation : public Polaris_Component<APPEND_CHILD(Mode_Skim_Table_Implementation),MasterType,Data_Object,ParentType>
		{
			// Tag as implementation
			typedef typename Polaris_Component<APPEND_CHILD(Mode_Skim_Table_Implementation),MasterType,Data_Object>::Component_Type ComponentType;

			member_associative_container(concat(dense_hash_map<long,typename MasterType::skim_routing_type*>), path_trees_container,none,none);

			member_data(long, mode_id, check(ReturnValueType,is_arithmetic),  check(SetValueType,is_arithmetic));
			member_container(vector<Skim_Table_Implementation<MasterType>*>, skims_by_time_container,none,none);
					
			// Link back to the parent network_skimmer class
			member_component(typename MasterType::network_skim_type, skim_reference, none, none);
			member_component_feature(nodes_per_zone,skim_reference,nodes_per_zone,Prototypes::Network_Skimming_Prototype);

			// links back to the network to be skimmed
			member_component(typename MasterType::network_type, network_reference, none, none);

			//=============================================
			// Primary function accessors - used to calculate/return LOS values for OD pairs
			//---------------------------------------------
			feature_implementation void Initialize()
			{
				this->_path_trees_container.set_empty_key(-1);
				this->_path_trees_container.set_deleted_key(-2);


				// set the network and skimmer references
				define_container_and_value_interface_unqualified_container(skim_tables_itf,skim_table_itf,type_of(skims_by_time_container),Containers::Random_Access_Sequence_Prototype,Prototypes::Skim_Table_Prototype,ComponentType);
				define_component_interface(network_itf,type_of(network_reference),Network_Components::Prototypes::Network_Prototype,ComponentType);
				define_component_interface(skimmer_itf,type_of(skim_reference),Prototypes::Network_Skimming_Prototype,ComponentType);
				define_container_and_value_interface(zones_itf,zone_itf,typename network_itf::get_type_of(zones_container),Associative_Container_Prototype,Zone_Components::Prototypes::Zone_Prototype,ComponentType);
				define_container_and_value_interface(locations_itf,location_itf,typename zone_itf::get_type_of(origin_activity_locations),Random_Access_Sequence_Prototype,Activity_Location_Components::Prototypes::Activity_Location_Prototype,ComponentType);
				define_container_and_value_interface(links_itf,link_itf,typename location_itf::get_type_of(origin_links),Random_Access_Sequence_Prototype,Activity_Location_Components::Prototypes::Activity_Location_Prototype,ComponentType);
				define_container_and_value_interface(origin_map_itf,origin_item_itf,typename skimmer_itf::get_type_of(origin_node_to_zone_map), Containers::Associative_Container_Prototype,Prototypes::Location_To_Zone_Map_Item_Prototype,CallerType);
				define_container_and_value_interface(modes_skim_container_itf,mode_skim_itf,typename skimmer_itf::get_type_of(mode_skim_table_container), Containers::Associative_Container_Prototype,Prototypes::Mode_Skim_Table_Prototype,CallerType);
				network_itf* network = this->template network_reference<ComponentType,CallerType,network_itf*>();
				skimmer_itf* skim = this->template skim_reference<ComponentType,CallerType,skimmer_itf*>();


				// tree builder interface
				define_container_and_value_interface_unqualified_container(tree_builder_list_itf, tree_builder_itf, type_of(path_trees_container),Associative_Container_Prototype,Routing_Components::Prototypes::Routing_Prototype,ComponentType);
				tree_builder_list_itf* tree_list = this->path_trees_container<ComponentType,CallerType,tree_builder_list_itf*>();

				origin_map_itf* origin_map = skim->template origin_node_to_zone_map<origin_map_itf*>();	
				zones_itf* zones_container = network->template zones_container<zones_itf*>();
				typename origin_map_itf::iterator orig_itr = origin_map->begin();
				locations_itf* activity_locations = network->template activity_locations_container<locations_itf*>();
				
				// create a tree builder for each origin to skim
				for (;orig_itr != origin_map->end(); ++orig_itr)
				{
					// get the origin/destination indices
					origin_item_itf* orig_item = orig_itr->second;			
					long orig = orig_item->template loc_index<long>();
					location_itf* orig_node = activity_locations->at(orig);

					// Allocate a tree_builder for each origin node		
					tree_builder_itf* tree_builder = (tree_builder_itf*)Allocate<typename tree_builder_itf::Component_Type>();

					// Set the current routable origin for the tree builder
					tree_builder->template routable_origin<link_itf*>(*(orig_node->template origin_links<links_itf*>()->begin()));
					tree_builder->template network<network_itf*>(network);

					// Set the start, end and update times for the router
					tree_builder->template update_increment<Simulation_Timestep_Increment>(skim->template update_increment<Simulation_Timestep_Increment>());
					tree_builder->template start_time<Simulation_Timestep_Increment>(0);//this->start_time<Simulation_Timestep_Increment>());
					tree_builder->template end_time<Simulation_Timestep_Increment>(END);//this->end_time<Simulation_Timestep_Increment>());

					// Add the tree_builder to the list for future processing
					tree_builder->template Initialize_Tree_Computation<NULLTYPE>(_iteration);

					pair<long,tree_builder_itf*> item = pair<long,tree_builder_itf*>(orig,tree_builder);
					tree_list->insert(item);
				}


				Simulation_Timestep_Increment start;

				//===========================================================================
				// FILE INPUT IF REQUESTED
				int num_modes;
				int num_zones;
				int update_increment;

				File_IO::Binary_File_Reader& infile = skim->template input_file<File_IO::Binary_File_Reader&>();
				if (skim->template read_input<bool>() == true)
				{		
					infile.Read_Value<int>(num_modes);
					infile.Read_Value<int>(num_zones);
					infile.Read_Value<int>(update_increment);
					if (update_increment != skim->template update_increment<Simulation_Timestep_Increment>())
					{
						THROW_EXCEPTION("ERROR: Input skim file update increment does not match the update increment specified in Polaris_Skim_Implementation.");
					}
				}
				
				//===========================================================================
				// create the skim_table time periods, for basic create only a single time period skim_table
				for (start = 0; start < GLOBALS::Time_Converter.template Convert_Value<Target_Type<NT,Simulation_Timestep_Increment,Time_Hours>>(24.0); start = start + skim->template update_increment<Simulation_Timestep_Increment>())
				{		
					skim_table_itf* skim_table = (skim_table_itf*)Allocate<typename type_of(skims_by_time_container)::unqualified_value_type>();
					skim_table->template network_reference<network_itf*>(network);
					skim_table->template skim_reference<skimmer_itf*>(skim);
					skim_table->template start_time<Simulation_Timestep_Increment>(start);
					skim_table->template end_time<Simulation_Timestep_Increment>(start + skim->template update_increment<Simulation_Timestep_Increment>());

					skim_table->template mode_skim_reference<ComponentType*>(this);

					if (skim->template read_input<bool>() == true)
					{
						float* data = new float[num_zones*num_zones];
						infile.Read_Array<float>(data, num_zones*num_zones);
						skim_table->template Initialize<float*>(data);
					}
					else
					{
						skim_table->template Initialize<NULLTYPE>();
					}

					// add time period skim tables to the container
					skim_tables_itf* skim_tables = this->template skims_by_time_container<ComponentType,CallerType,skim_tables_itf*>();
					skim_tables->push_back(skim_table);
				}

				//===========================================================================
				// Initial FILE_OUTPUT IF REQUESTED
				File_IO::Binary_File_Writer& outfile = skim->template output_file<File_IO::Binary_File_Writer&>();
				if (skim->template write_output<bool>() == true)
				{
					int modes = skim->template mode_skim_table_container<modes_skim_container_itf&>().size();
					outfile.Write_Value<int>(modes);
					int zones = zones_container->size();
					outfile.Write_Value<int>(zones);
					int increment = skim->template update_increment<Simulation_Timestep_Increment>();
					outfile.Write_Value<int>(increment);
				}
			}

			feature_implementation typename TargetType::ReturnType Get_LOS(typename TargetType::ParamType Origin_ID, typename TargetType::ParamType Destination_ID, typename TargetType::Param2Type Mode_Indicator)
			{		
			}
		};


		//======================================================================================
		/// Basic skimming implementation
		//--------------------------------------------------------------------------------------
		implementation struct Basic_Network_Skimming_Implementation : public Polaris_Component<APPEND_CHILD(Basic_Network_Skimming_Implementation),MasterType,Execution_Object,ParentType>
		{
			// Tag as implementation
			typedef typename Polaris_Component<APPEND_CHILD(Basic_Network_Skimming_Implementation),MasterType,Data_Object>::Component_Type ComponentType;

			typedef Time_Minutes Stored_Time_Type;

			// reference to the transportation network
			member_component(typename MasterType::network_type, network_reference, none, none);

			// time increment at which skim tables are updated - set in the initializer
			member_data_component(Basic_Units::Implementations::Time_Implementation<MasterType>,_update_increment,none,none);
			member_component_feature(update_increment,_update_increment,Value,Basic_Units::Prototypes::Time_Prototype);

			// time at which skim tables are next updated - set in the initializer and updated every time update is called
			member_data_component(Basic_Units::Implementations::Time_Implementation<MasterType>,_scheduled_update_time,none,none);
			member_component_feature(scheduled_update_time,_scheduled_update_time,Value,Basic_Units::Prototypes::Time_Prototype);
			member_data(long, nodes_per_zone, check(ReturnValueType,is_arithmetic),check(SetValueType,is_arithmetic));

			// link-to-zone mapping for use in skimming
			member_associative_container(concat(dense_hash_map<long,Location_To_Zone_Map_Item<NULLTYPE>*>),origin_node_to_zone_map,none,none);
			member_associative_container(concat(dense_hash_map<long,Location_To_Zone_Map_Item<NULLTYPE>*>),destination_node_to_zone_map,none,none);

			member_data(Counter, timer,none,none);
			member_data(bool, write_output, none,none);
			member_data(bool, read_input, none,none);
			member_data(File_IO::Binary_File_Writer, output_file,none,none);
			member_data(File_IO::Binary_File_Reader, input_file,none,none);

			member_associative_container(concat(dense_hash_map<long,Mode_Skim_Table_Implementation<MasterType,ParentType>*>), mode_skim_table_container, none, none);

			member_container(list<int>, available_modes_container, none, none);

			feature_implementation void Initialize()
			{
				this->_origin_node_to_zone_map.set_empty_key(-1);
				this->_origin_node_to_zone_map.set_deleted_key(-2);
				this->_destination_node_to_zone_map.set_empty_key(-1);
				this->_destination_node_to_zone_map.set_deleted_key(-2);

				// initialize dense_hash_map keys
				this->_mode_skim_table_container.set_empty_key(-1);
				this->_mode_skim_table_container.set_deleted_key(-2);

				// create interface to this and set skimming parameters
				typedef Prototypes::Network_Skimming_Prototype<ComponentType,ComponentType> this_itf;
				this_itf* pthis = (this_itf*)this;
				pthis->template update_increment<Time_Hours>(1);
				pthis->template scheduled_update_time<Simulation_Timestep_Increment>(0.0);
				pthis->template nodes_per_zone<long>(1);

				// add the available modes for the current model
				this->_available_modes_container.push_back(Vehicle_Components::Types::Vehicle_Type_Keys::SOV);
			}
		};


		//======================================================================================
		/// More advance skimming implementation not yet implemented
		//--------------------------------------------------------------------------------------
		implementation struct Advanced_Network_Skimming_Implementation : public Basic_Network_Skimming_Implementation<MasterType, ParentType,APPEND_CHILD(Advanced_Network_Skimming_Implementation)>
		{
			// Tag as implementation
			typedef typename Basic_Network_Skimming_Implementation<MasterType, ParentType,APPEND_CHILD(Advanced_Network_Skimming_Implementation)>::Component_Type ComponentType;
		};
	}

}