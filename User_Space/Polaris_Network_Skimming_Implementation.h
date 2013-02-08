#pragma once

#include "Network_Skimming_Prototype.h"
#include "Polaris_Network_Implementation.h"
#include "Polaris_Zone_Implementation.h"

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
		implementation struct Location_To_Zone_Map_Item : public Polaris_Component_Class<Location_To_Zone_Map_Item,MasterType,Data_Object,ParentType>
		{
			feature_implementation void initialize(typename TargetType::ParamType loc_index, typename TargetType::ParamType zone_index, typename TargetType::Param2Type weight)
			{
				this->loc_index<ComponentType,ComponentType,TargetType::ParamType>(loc_index);
				this->zone_index<ComponentType,ComponentType,TargetType::ParamType>(zone_index);
				this->weight<ComponentType,ComponentType,TargetType::ParamType>(weight);
			}
			member_data(long, loc_index,check(ReturnValueType,is_integral),check_2(ComponentType,CallerType, Is_Same_Component));
			member_data(long, zone_index,check(ReturnValueType,is_integral),check_2(ComponentType,CallerType, Is_Same_Component));
			member_data(double, weight,check(ReturnValueType,is_arithmetic),check_2(ComponentType,CallerType, Is_Same_Component));
		};

		//======================================================================================
		/// Common elements to all network skimming components
		//--------------------------------------------------------------------------------------
		implementation struct _Basic_Network_Skimming_Elements
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
			member_associative_container(concat(hash_map<long,Location_To_Zone_Map_Item<NULLTYPE>*>),origin_node_to_zone_map,none,none);
			member_associative_container(concat(hash_map<long,Location_To_Zone_Map_Item<NULLTYPE>*>),destination_node_to_zone_map,none,none);

			member_data(Counter, timer,none,none);
		};


		//======================================================================================
		/// Skim table which holds the values for LOS
		//--------------------------------------------------------------------------------------
		implementation struct Skim_Table_Implementation : public Polaris_Component_Class<Skim_Table_Implementation,MasterType,Data_Object,ParentType>
		{
			member_container(matrix<double>, skim_table, none,none);
			member_associative_container(concat(hash_map<long,typename MasterType::routing_type*>), path_trees_container,none,none);

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
				//// set the network reference
				//define_simple_container_interface(skim_table_itf,type_of(skim_table),Containers::Multidimensional_Random_Access_Array_Prototype,double,ComponentType);
				//define_component_interface(network_itf,type_of(network_reference),Network_Components::Prototypes::Network_Prototype,ComponentType);
				//define_component_interface(skimmer_itf,type_of(skim_reference),Prototypes::Network_Skimming_Prototype,ComponentType);
				//network_itf* network = this->network_reference<ComponentType,CallerType,network_itf*>();
				//skimmer_itf* skim = this->skim_reference<ComponentType,CallerType,skimmer_itf*>();

				//// create the LOS container
				//define_container_and_value_interface(zones_itf,zone_itf,network_itf::get_type_of(zones_container),Random_Access_Sequence_Prototype,Zone_Components::Prototypes::Zone_Prototype,ComponentType);
				//define_container_and_value_interface(locations_itf,location_itf,zone_itf::get_type_of(origin_activity_locations),Random_Access_Sequence_Prototype,Activity_Location_Components::Prototypes::Activity_Location_Prototype,ComponentType);
				//define_container_and_value_interface(links_itf,link_itf,location_itf::get_type_of(origin_links),Random_Access_Sequence_Prototype,Activity_Location_Components::Prototypes::Activity_Location_Prototype,ComponentType);
				//zones_itf* zones_container = network->zones_container<zones_itf*>();
				//zones_itf::iterator orig_itr = zones_container->begin();
				//zones_itf::iterator dest_itr = zones_container->begin();

				//
				//// create the LOS matrix
				//typedef matrix<double>::size_type size_t;
				//matrix<double>* vehicle_los = new matrix<double>(pair<size_t,size_t>((size_t)zones_container->size(),(size_t)zones_container->size()), 0);

				//// tree builder interface
				//define_container_and_value_interface(tree_builder_list_itf, tree_builder_itf, type_of(path_trees_container),Associative_Container_Prototype,Routing_Components::Prototypes::Routing_Prototype,ComponentType);
				//tree_builder_list_itf* tree_list = this->path_trees_container<ComponentType,CallerType,tree_builder_list_itf*>();
				//
				//for (;orig_itr != zones_container->end(); ++orig_itr)
				//{
				//	long orig;
				//	zone_itf* orig_zone = *orig_itr;
				//	orig = orig_zone->internal_id<long>();

				//	// Allocate a tree_builder for each origin node
				//	locations_itf* activity_locations = orig_zone->origin_activity_locations<locations_itf*>();	
				//	location_itf* orig_node = *(activity_locations->begin());
				//	tree_builder_itf* tree_builder = (tree_builder_itf*)Allocate<MasterType::routing_type>();
				//	// Set the current origin for the tree builder
				//	tree_builder->routable_origin<link_itf*>(*(orig_node->origin_links<links_itf*>()->begin()));
				//	tree_builder->network<network_itf*>(network);
				//	// Set the start, end and update times for the router
				//	tree_builder->update_increment<Simulation_Timestep_Increment>(skim->update_increment<Simulation_Timestep_Increment>());
				//	tree_builder->start_time<Simulation_Timestep_Increment>(this->start_time<ComponentType,CallerType,Simulation_Timestep_Increment>());
				//	tree_builder->end_time<Simulation_Timestep_Increment>(this->end_time<ComponentType,CallerType,Simulation_Timestep_Increment>());
				//	// Add the tree_builder to the list for future processing
				//	tree_builder->Initialize_Tree_Computation<NULLTYPE>(_iteration);
				//	tree_list->insert(pair<long,tree_builder_itf*>(orig,tree_builder));

				//	// initialize LOS table values
				//	for (dest_itr = zones_container->begin(); dest_itr != zones_container->end(); ++dest_itr)
				//	{
				//		long dest;
				//		zone_itf* dest_zone = *dest_itr;
				//		dest = dest_zone->internal_id<long>();

				//		locations_itf* activity_locations = dest_zone->origin_activity_locations<locations_itf*>();	
				//		location_itf* dest_node = *(activity_locations->begin());

				//		(*vehicle_los)(orig,dest) = 0;
				//	}
				//}

				//skim_tables_itf* skim_tables = this->skim_tables_container<ComponentType,CallerType,skim_tables_itf*>();
				////pair<long,matrix<double>*> p = pair<long,matrix<double>*>(Vehicle_Components::Types::Vehicle_Type_Keys::SOV,vehicle_los);
				//skim_tables->insert(pair<long,matrix<double>*>(Vehicle_Components::Types::Vehicle_Type_Keys::SOV,vehicle_los));
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
		implementation struct Mode_Skim_Table_Implementation : public Polaris_Component_Class<Mode_Skim_Table_Implementation,MasterType,Data_Object,ParentType>
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
				define_container_and_value_interface(skim_tables_itf,skim_table_itf,type_of(skims_by_time_container),Containers::Random_Access_Sequence_Prototype,Prototypes::Skim_Table_Prototype,ComponentType);
				define_component_interface(network_itf,type_of(network_reference),Network_Components::Prototypes::Network_Prototype,ComponentType);
				define_component_interface(skimmer_itf,type_of(skim_reference),Prototypes::Network_Skimming_Prototype,ComponentType);
				network_itf* network = this->network_reference<ComponentType,CallerType,network_itf*>();
				skimmer_itf* skim = this->skim_reference<ComponentType,CallerType,skimmer_itf*>();

				
				// create the skim_table time periods, for basic create only a single time period skim_table
				skim_table_itf* skim_table = (skim_table_itf*)Allocate<type_of(skims_by_time_container)::unqualified_value_type>();
				skim_table->network_reference<network_itf*>(network);
				skim_table->skim_reference<skimmer_itf*>(skim);
				skim_table->start_time<Simulation_Timestep_Increment>(0);
				skim_table->end_time<Simulation_Timestep_Increment>(END);
				skim_table->Initialize<NULLTYPE>();

				// add time period skim tables to the container
				skim_tables_itf* skim_tables = this->skims_by_time_container<ComponentType,CallerType,skim_tables_itf*>();
				skim_tables->push_back(skim_table);


			}
			feature_implementation bool Update_LOS()
			{
				// get the network reference
				define_simple_container_interface(skim_tables_itf,type_of(skim_tables_container),Containers::Associative_Container_Prototype,matrix<double>,ComponentType);
				define_component_interface(network_itf,type_of(network_reference),Network_Components::Prototypes::Network_Prototype,ComponentType);
				define_container_and_value_interface(zones_itf,zone_itf,network_itf::get_type_of(zones_container),Random_Access_Sequence_Prototype,Zone_Components::Prototypes::Zone_Prototype,ComponentType);
				define_container_and_value_interface(locations_itf,location_itf,zone_itf::get_type_of(origin_activity_locations),Random_Access_Sequence_Prototype,Activity_Location_Components::Prototypes::Activity_Location_Prototype,ComponentType);
				define_container_and_value_interface(destinations_itf,destination_itf,zone_itf::get_type_of(destination_activity_locations),Random_Access_Sequence_Prototype,Link_Components::Prototypes::Link_Prototype,ComponentType);
				define_container_and_value_interface(links_itf,link_itf,location_itf::get_type_of(origin_links),Random_Access_Sequence_Prototype,Activity_Location_Components::Prototypes::Activity_Location_Prototype,ComponentType);
				network_itf* network = this->network_reference<ComponentType,CallerType,network_itf*>();

				// get reference to the routers used to create path-trees from each origin
				define_container_and_value_interface(path_trees_itf,path_tree_itf,type_of(path_trees_container),Associative_Container_Prototype,Routing_Components::Prototypes::Routing_Prototype,ComponentType);
					
				typedef Network_Components::Prototypes::Network_Prototype<typename path_tree_itf::Component_Type::routable_network_type, ComponentType> _Routable_Network_Interface;
			
				define_container_and_value_interface(_Routable_Links_Container_Interface,_Routable_Link_Interface,_Routable_Network_Interface::get_type_of(links_container),Random_Access_Sequence_Prototype,Link_Components::Prototypes::Link_Prototype,ComponentType);
				path_trees_itf* trees_container = this->path_trees_container<ComponentType,CallerType,path_trees_itf*>();
				path_trees_itf::iterator tree_itr = trees_container->begin();

				// get reference to zones_container
				define_container_and_value_interface(zones_itf,zone_itf,network_itf::get_type_of(zones_container),Random_Access_Sequence_Prototype,Zone_Components::Prototypes::Zone_Prototype,ComponentType);
				zones_itf* zones_container = network->zones_container<zones_itf*>();
				zones_itf::iterator orig_itr = zones_container->begin();
				zones_itf::iterator dest_itr = zones_container->begin();
					
				skim_tables_itf* skim_tables = this->skim_tables_container<ComponentType,CallerType,skim_tables_itf*>();
				skim_tables_itf::iterator skim_table_itr = skim_tables->begin();

				// loop through all mode skim tables
				for (; skim_table_itr != skim_tables->end(); ++skim_table_itr)
				{
					// LOS Table reference for current mode
					matrix<double>& los_skim = skim_table_itr->second;

					for (;tree_itr != trees_container->end(); ++tree_itr)
					{
						long orig = tree_itr->first;
						path_tree_itf* tree = tree_itr->second;

						_Routable_Links_Container_Interface::iterator link_itr;
						for(link_itr=tree->routable_network<_Routable_Network_Interface*>()->links_container<_Routable_Links_Container_Interface&>().begin();link_itr!=tree->routable_network<_Routable_Network_Interface*>()->links_container<_Routable_Links_Container_Interface&>().end();link_itr++)
						{
							_Routable_Link_Interface* link_ptr = (_Routable_Link_Interface*)(*link_itr);
							cout << "link " << link_ptr->internal_id<int>() << ", path_cost=" << link_ptr->label_cost<float>() << endl;
						}
					}

					los_skim.print(cout);
				}



				return true;
			}
			feature_implementation typename TargetType::ReturnType Get_LOS(typename TargetType::ParamType Origin_ID, typename TargetType::ParamType Destination_ID, typename TargetType::Param2Type Mode_Indicator)
			{		
				return (TargetType::ReturnType)(Basic_Units::Time_Variables::Time_Minutes(5));
			}
		};


		//======================================================================================
		/// Basic skimming implementation with only the current LOS being stored
		//--------------------------------------------------------------------------------------
		implementation struct Basic_Network_Skimming_Implementation : public Polaris_Component_Class<Basic_Network_Skimming_Implementation,MasterType,Execution_Object,ParentType>, public _Basic_Network_Skimming_Elements<MasterType>
		{
			member_associative_container(concat(hash_map<long,Mode_Skim_Table_Implementation<MasterType,ParentType>*>), mode_skim_table_container, none, none);

			feature_implementation void Initialize()
			{
				typedef Prototypes::Network_Skimming_Prototype<ComponentType,ComponentType> this_itf;
				this_itf* pthis = (this_itf*)this;
				pthis->update_increment<Time_Hours>(1);
				pthis->scheduled_update_time<Simulation_Timestep_Increment>(0);
				pthis->nodes_per_zone<long>(1);

				// add a mode_skim_table for each mode available in simulation - for basic, only add SOV container
				define_container_and_value_interface(_skim_container_itf,_skim_itf,type_of(mode_skim_table_container),Containers::Associative_Container_Prototype,Prototypes::Mode_Skim_Table_Prototype,ComponentType);				
				define_component_interface(_Network_Itf, type_of(network_reference),Network_Components::Prototypes::Network_Prototype,ComponentType);
				_skim_itf* skim = (_skim_itf*)Allocate<type_of(mode_skim_table_container)::unqualified_value_type>();
				skim->skim_reference<ComponentType*>(this);
				skim->network_reference<_Network_Itf*>(pthis->network_reference<_Network_Itf*>());
				skim->mode_id<long>(Vehicle_Components::Types::Vehicle_Type_Keys::SOV);
				skim->Initialize<NULLTYPE>();
				pthis->mode_skim_table_container<_skim_container_itf*>()->insert(pair<long,_skim_itf*>(skim->mode_id<long>(), skim));

			}
			feature_implementation bool Process_Skim_Trees()
			{
				//define_component_interface(_skim_interface,type_of(current_skim_table),Prototypes::Skim_Table_Prototype,ComponentType);
				//_skim_interface* skim = this->current_skim_table<ComponentType,CallerType,_skim_interface*>();
				//return skim->Process_Skims<NULLTYPE>();
			return true;
			}

			feature_implementation typename TargetType::ReturnType Get_Current_LOS(typename TargetType::ParamType Origin_ID, typename TargetType::ParamType Destination_ID, typename TargetType::Param2Type Mode_Indicator/*, requires(check(typename TargetType::ReturnType, Basic_Units::Concepts::Is_Time_Value))*/)
			{
				define_component_interface(_skim_interface,type_of(current_skim_table),Prototypes::Skim_Table_Prototype,ComponentType);
				_skim_interface* skim = this->current_skim_table<ComponentType,CallerType,_skim_interface*>();
				return skim->Get_LOS<TargetType>(Origin_ID, Destination_ID, Mode_Indicator);
			}	
		};


		implementation struct Advanced_Network_Skimming_Implementation : public _Basic_Network_Skimming_Elements<MasterType>, public Polaris_Component_Class<Advanced_Network_Skimming_Implementation,MasterType,Execution_Object,ParentType>
		{

		};
	}

}