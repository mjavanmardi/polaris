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
			member_container(matrix<double>, skim_table, none,none);
			member_associative_container(concat(dense_hash_map<long,typename MasterType::routing_type*>), path_trees_container,none,none);

			// start and end times of the period represented by the skim table
			member_data_component(Basic_Units::Implementations::Time_Implementation<MasterType>,_start_time,none,none);
			member_component_feature(start_time,_start_time,Value,Basic_Units::Prototypes::Time_Prototype);
			member_data_component(Basic_Units::Implementations::Time_Implementation<MasterType>,_end_time,none,none);
			member_component_feature(end_time,_end_time,Value,Basic_Units::Prototypes::Time_Prototype);
			
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
			}
			feature_implementation bool Update_LOS()
			{
				//// get the network reference
				//define_simple_container_interface(skim_tables_itf,type_of(skim_tables_container),Containers::Associative_Container_Prototype,matrix<double>,ComponentType);
				//define_component_interface(network_itf,type_of(network_reference),Network_Components::Prototypes::Network_Prototype,ComponentType);
				//define_container_and_value_interface(zones_itf,zone_itf,network_itf::get_type_of(zones_container),Random_Access_Sequence_Prototype,Zone_Components::Prototypes::Zone_Prototype,ComponentType);
				//define_container_and_value_interface(locations_itf,location_itf,zone_itf::get_type_of(origin_activity_locations),Random_Access_Sequence_Prototype,Activity_Location_Components::Prototypes::Activity_Location_Prototype,ComponentType);
				//define_container_and_value_interface(destinations_itf,destination_itf,zone_itf::get_type_of(destination_activity_locations),Random_Access_Sequence_Prototype,Link_Components::Prototypes::Link_Prototype,ComponentType);
				//define_container_and_value_interface(links_itf,link_itf,location_itf::get_type_of(origin_links),Random_Access_Sequence_Prototype,Activity_Location_Components::Prototypes::Activity_Location_Prototype,ComponentType);
				//network_itf* network = this->network_reference<ComponentType,CallerType,network_itf*>();

				//// get reference to the routers used to create path-trees from each origin
				//define_container_and_value_interface(path_trees_itf,path_tree_itf,type_of(path_trees_container),Associative_Container_Prototype,Routing_Components::Prototypes::Routing_Prototype,ComponentType);
				//	
				//typedef Network_Components::Prototypes::Network_Prototype<typename path_tree_itf::Component_Type::routable_network_type, ComponentType> _Routable_Network_Interface;
			
				//define_container_and_value_interface(_Routable_Links_Container_Interface,_Routable_Link_Interface,_Routable_Network_Interface::get_type_of(links_container),Random_Access_Sequence_Prototype,Link_Components::Prototypes::Link_Prototype,ComponentType);
				//path_trees_itf* trees_container = this->path_trees_container<ComponentType,CallerType,path_trees_itf*>();
				//path_trees_itf::iterator tree_itr = trees_container->begin();

				//// get reference to zones_container
				//define_container_and_value_interface(zones_itf,zone_itf,network_itf::get_type_of(zones_container),Random_Access_Sequence_Prototype,Zone_Components::Prototypes::Zone_Prototype,ComponentType);
				//zones_itf* zones_container = network->zones_container<zones_itf*>();
				//zones_itf::iterator orig_itr = zones_container->begin();
				//zones_itf::iterator dest_itr = zones_container->begin();
				//	
				//skim_tables_itf* skim_tables = this->skim_tables_container<ComponentType,CallerType,skim_tables_itf*>();
				//skim_tables_itf::iterator skim_table_itr = skim_tables->begin();

				//// loop through all mode skim tables
				//for (; skim_table_itr != skim_tables->end(); ++skim_table_itr)
				//{
				//	// LOS Table reference for current mode
				//	matrix<double>& los_skim = skim_table_itr->second;

				//	for (;tree_itr != trees_container->end(); ++tree_itr)
				//	{
				//		long orig = tree_itr->first;
				//		path_tree_itf* tree = tree_itr->second;

				//		_Routable_Links_Container_Interface::iterator link_itr;
				//		for(link_itr=tree->routable_network<_Routable_Network_Interface*>()->links_container<_Routable_Links_Container_Interface&>().begin();link_itr!=tree->routable_network<_Routable_Network_Interface*>()->links_container<_Routable_Links_Container_Interface&>().end();link_itr++)
				//		{
				//			_Routable_Link_Interface* link_ptr = (_Routable_Link_Interface*)(*link_itr);
				//			cout << "link " << link_ptr->internal_id<int>() << ", path_cost=" << link_ptr->label_cost<float>() << endl;
				//		}
				//		
				//		//for (dest_itr = zones_container->begin(); dest_itr != zones_container->end(); ++dest_itr)
				//		//{
				//		//	long dest;
				//		//	zone_itf* dest_zone = *dest_itr;
				//		//	dest = dest_zone->internal_id<long>();
				//		//	dest_zone->destination_activity_locations<destinations_itf*>();
				//		//	tree->
				//		//	los_skim(orig,dest) = sqrt((double)(orig*orig + dest*dest));
				//		//}
				//	}

				//	//for (;orig_itr != zones_container->end(); ++orig_itr)
				//	//{
				//	//	long orig;
				//	//	zone_itf* orig_zone = *orig_itr;
				//	//	orig = orig_zone->internal_id<long>();

				//	//	for (dest_itr = zones_container->begin(); dest_itr != zones_container->end(); ++dest_itr)
				//	//	{
				//	//		long dest;
				//	//		zone_itf* dest_zone = *dest_itr;
				//	//		dest = dest_zone->internal_id<long>();
				//	//		los_skim(orig,dest) = sqrt((double)(orig*orig + dest*dest));
				//	//	}
				//	//}

				//	//cout << endl<<endl;
				//	los_skim.print(cout);
				//}



				//return true;
			}
			feature_implementation typename TargetType::ReturnType Get_LOS(typename TargetType::ParamType Origin_ID, typename TargetType::ParamType Destination_ID, typename TargetType::Param2Type Mode_Indicator)
			{		
				return (TargetType::ReturnType)(Basic_Units::Time_Variables::Time_Minutes(5));
			}
		};


		//======================================================================================
		/// A collection of skim_tables for a given mode
		//--------------------------------------------------------------------------------------
		implementation struct Mode_Skim_Table_Implementation : public Polaris_Component<APPEND_CHILD(Mode_Skim_Table_Implementation),MasterType,Data_Object,ParentType>
		{
			member_data(long, mode_id, check(ReturnValueType,is_arithmetic),  check(SetValueType,is_arithmetic));
			member_container(vector<Skim_Table_Implementation<MasterType>*>, skims_by_time_container,none,none);
					
			// Link back to the parent network_skimmer class
			member_component(typename MasterType::network_skim_type, skim_reference, none, none);

			// links back to the network to be skimmed
			member_component(typename MasterType::network_type, network_reference, none, none);

			//=============================================
			// Primary function accessors - used to calculate/return LOS values for OD pairs
			//---------------------------------------------
			feature_implementation void Initialize()
			{
				// set the network and skimmer references
				define_container_and_value_interface_unqualified_container(skim_tables_itf,skim_table_itf,type_of(skims_by_time_container),Containers::Random_Access_Sequence_Prototype,Prototypes::Skim_Table_Prototype,ComponentType);
				define_component_interface(network_itf,type_of(network_reference),Network_Components::Prototypes::Network_Prototype,ComponentType);
				define_component_interface(skimmer_itf,type_of(skim_reference),Prototypes::Network_Skimming_Prototype,ComponentType);
				network_itf* network = this->template network_reference<ComponentType,CallerType,network_itf*>();
				skimmer_itf* skim = this->template skim_reference<ComponentType,CallerType,skimmer_itf*>();

				
				// create the skim_table time periods, for basic create only a single time period skim_table
				skim_table_itf* skim_table = (skim_table_itf*)Allocate<typename type_of(skims_by_time_container)::unqualified_value_type>();
				skim_table->template network_reference<network_itf*>(network);
				skim_table->template skim_reference<skimmer_itf*>(skim);
				skim_table->template start_time<Simulation_Timestep_Increment>(0);
				skim_table->template end_time<Simulation_Timestep_Increment>(END);
				skim_table->template Initialize<NULLTYPE>();

				// add time period skim tables to the container
				skim_tables_itf* skim_tables = this->template skims_by_time_container<ComponentType,CallerType,skim_tables_itf*>();
				skim_tables->push_back(skim_table);


			}

			feature_implementation typename TargetType::ReturnType Get_LOS(typename TargetType::ParamType Origin_ID, typename TargetType::ParamType Destination_ID, typename TargetType::Param2Type Mode_Indicator)
			{		
				return (TargetType::ReturnType)(Basic_Units::Time_Variables::Time_Minutes(5));
			}
		};


		//======================================================================================
		/// Basic skimming implementation
		//--------------------------------------------------------------------------------------
		implementation struct Basic_Network_Skimming_Implementation : public Polaris_Component<APPEND_CHILD(Basic_Network_Skimming_Implementation),MasterType,Execution_Object,ParentType>
		{
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
				pthis->template scheduled_update_time<Simulation_Timestep_Increment>(0);
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

		};
	}

}