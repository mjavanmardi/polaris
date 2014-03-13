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
		implementation struct LOS_Time_Invariant_Value_Implementation : public Polaris_Component<MasterType,INHERIT(LOS_Time_Invariant_Value_Implementation),Data_Object>
		{
			// Tag as implementation
			typedef typename Polaris_Component<MasterType,INHERIT(LOS_Time_Invariant_Value_Implementation),Data_Object>::Component_Type ComponentType;
			LOS_Time_Invariant_Value_Implementation()
			{
				this->auto_tolls<Basic_Units::Currency_Variables::Cents>(0.0);
				this->auto_parking_cost<Basic_Units::Currency_Variables::Cents>(0.0);
				this->transit_ttime<Simulation_Timestep_Increment>(FLT_MAX);
				this->transit_walk_access_time<Simulation_Timestep_Increment>(0.0);
				this->auto_distance/*transit_sov_access_time*/<Miles>(0.0);
				this->transit_wait_time<Simulation_Timestep_Increment>(0.0);
				this->transit_fare<Basic_Units::Currency_Variables::Cents>(0.0);
			}
			member_component_and_feature_accessor(auto_tolls, Value, Basic_Units::Prototypes::Currency,Basic_Units::Implementations::Currency_Implementation<NT>);
			member_component_and_feature_accessor(auto_parking_cost, Value, Basic_Units::Prototypes::Currency,Basic_Units::Implementations::Currency_Implementation<NT>);
			member_component_and_feature_accessor(auto_distance/*transit_sov_access_time*/, Value, Basic_Units::Prototypes::Length,Basic_Units::Implementations::Length_Implementation<NT>);
			member_component_and_feature_accessor(transit_ttime, Value, Basic_Units::Prototypes::Time,Basic_Units::Implementations::Time_Implementation<NT>);
			member_component_and_feature_accessor(transit_walk_access_time, Value, Basic_Units::Prototypes::Time,Basic_Units::Implementations::Time_Implementation<NT>);			
			member_component_and_feature_accessor(transit_wait_time, Value, Basic_Units::Prototypes::Time,Basic_Units::Implementations::Time_Implementation<NT>);
			member_component_and_feature_accessor(transit_fare, Value, Basic_Units::Prototypes::Currency,Basic_Units::Implementations::Currency_Implementation<NT>);
		};

		implementation struct LOS_Value_Implementation : public Polaris_Component<MasterType,INHERIT(LOS_Value_Implementation),Data_Object>
		{
			// Tag as implementation
			typedef typename Polaris_Component<MasterType,INHERIT(LOS_Value_Implementation),Data_Object>::Component_Type ComponentType;
			LOS_Value_Implementation()
			{
				this->auto_ttime<Simulation_Timestep_Increment>(FLT_MAX);
			}
			member_component_and_feature_accessor(auto_ttime, Value, Basic_Units::Prototypes::Time,Basic_Units::Implementations::Time_Implementation<NT>);
			m_prototype(Prototypes::LOS,LOS_Time_Invariant_Value_Implementation<NT>,LOS_time_invariant, NONE, NONE);

			// pass through feature accessors for the time invariant components, if one of these becomes time variable, replace this with the 'featurea_and_accessor' statement in the current invariant implementation
			member_component_feature(auto_tolls, LOS_time_invariant, auto_tolls, Prototypes::LOS);
			member_component_feature(auto_parking_cost, LOS_time_invariant, auto_parking_cost, Prototypes::LOS);
			member_component_feature(auto_distance/*transit_sov_access_time*/, LOS_time_invariant, auto_distance, Prototypes::LOS);
			member_component_feature(transit_ttime, LOS_time_invariant, transit_ttime, Prototypes::LOS);
			member_component_feature(transit_walk_access_time, LOS_time_invariant, transit_walk_access_time, Prototypes::LOS);
			member_component_feature(transit_wait_time, LOS_time_invariant, transit_wait_time, Prototypes::LOS);
			member_component_feature(transit_fare, LOS_time_invariant, transit_fare, Prototypes::LOS);
		};
		


		//======================================================================================
		/// Skim table which holds the values for LOS
		//--------------------------------------------------------------------------------------
		implementation struct Skim_Table_Implementation : public Polaris_Component<MasterType,INHERIT(Skim_Table_Implementation),Data_Object>
		{
			// Tag as implementation
			typedef typename Polaris_Component<MasterType,INHERIT(Skim_Table_Implementation),Data_Object>::Component_Type ComponentType;
			typedef Prototypes::LOS<typename MasterType::los_value_type> los_itf; 

			m_container(matrix<typename MasterType::los_value_type*>, skim_table, NONE, NONE);
			

			// start and end times of the period represented by the skim table
			m_data(Basic_Units::Implementations::Time_Implementation<MasterType>,_start_time, NONE, NONE);
			member_component_feature(start_time,_start_time,Value,Basic_Units::Prototypes::Time);
			m_data(Basic_Units::Implementations::Time_Implementation<MasterType>,_end_time, NONE, NONE);
			member_component_feature(end_time,_end_time,Value,Basic_Units::Prototypes::Time);

			m_data(float, weighted_deviation, NONE, NONE);
			m_data(float, max_deviation, NONE, NONE);
			
			// Link back to the parent network_skimmer class
			m_prototype(Null_Prototype,typename MasterType::network_skim_type, skim_reference, NONE, NONE);

			// links back to the network to be skimmed
			m_prototype(Null_Prototype,typename MasterType::network_type, network_reference, NONE, NONE);



			//=============================================
			// Interface typedefs
			//---------------------------------------------
			typedef Prototypes::LOS<typename remove_pointer< typename get_type_of(skim_table)::value_type>::type> los_value_itf;
			typedef Multidimensional_Random_Access_Array< typename get_type_of(skim_table),los_value_itf*> skim_table_itf;

			typedef Network_Components::Prototypes::Network<typename type_of(network_reference)> network_itf;
			typedef Prototypes::Network_Skimming<typename type_of(skim_reference)> skimmer_itf;
			typedef Zone_Components::Prototypes::Zone<typename remove_pointer<typename network_itf::get_type_of(zones_container)::mapped_type>::type> zone_itf;
			typedef Pair_Associative_Container<typename network_itf::get_type_of(zones_container),zone_itf*> zones_itf;

			typedef Activity_Location_Components::Prototypes::Activity_Location<typename remove_pointer<typename zone_itf::get_type_of(origin_activity_locations)::value_type>::type> location_itf;
			typedef Random_Access_Sequence<typename zone_itf::get_type_of(origin_activity_locations),location_itf*> locations_itf;

			typedef Activity_Location_Components::Prototypes::Activity_Location<typename remove_pointer<typename location_itf::get_type_of(origin_links)::value_type>::type> link_itf;
			typedef Random_Access_Sequence<typename location_itf::get_type_of(origin_links),link_itf*> links_itf;

			//typedef Prototypes::Location_To_Zone_Map_Item<typename remove_pointer<typename skimmer_itf::get_type_of(origin_node_to_zone_map)::value_type>::type> origin_item_itf;
			//typedef  Pair_Associative_Container<typename skimmer_itf::get_type_of(origin_node_to_zone_map),origin_item_itf*> origin_map_itf;

			typedef typename skimmer_itf::Component_Type::Stored_Time_Type Stored_Time_Type;
			

			//=============================================
			// Primary function accessors - used to calculate/return LOS values for OD pairs
			//---------------------------------------------
			template<typename TargetType> void Initialize()
			{
				typedef matrix<typename MasterType::los_value_type*>::size_type size_t;
				network_itf* network = this->network_reference<  network_itf*>();
				zones_itf* zones_container = network->template zones_container<zones_itf*>();
				this->_skim_table.resize(pair<size_t,size_t>((size_t)zones_container->size(),(size_t)zones_container->size()),0);
			}
			template<typename TargetType> void Initialize(TargetType initial_data)
			{
				typedef matrix<typename MasterType::los_value_type*>::size_type size_t;
				network_itf* network = this->network_reference<  network_itf*>();
				zones_itf* zones_container = network->template zones_container<zones_itf*>();
				this->_skim_table.Copy(pair<size_t,size_t>((size_t)zones_container->size(),(size_t)zones_container->size()), initial_data);
			}
			template<typename TargetType> bool Update_LOS()
			{
				// get the network and network element references
				typedef Prototypes::LOS<typename remove_pointer< typename type_of(skim_table)::value_type>::type> los_value_itf;
				typedef Multidimensional_Random_Access_Array< typename type_of(skim_table),los_value_itf*> skim_table_itf;

				typedef Network_Components::Prototypes::Network<typename type_of(network_reference)> network_itf;
				typedef Prototypes::Network_Skimming<typename type_of(skim_reference)> skimmer_itf;
				typedef Pair_Associative_Container<typename skimmer_itf::get_type_of(zone_origins_count),int> zone_location_count_itf;
				typedef Activity_Location_Components::Prototypes::Activity_Location<typename remove_pointer<typename network_itf::get_type_of(activity_locations_container)::value_type>::type> location_itf;
				typedef Random_Access_Sequence<typename network_itf::get_type_of(activity_locations_container),location_itf*> locations_itf;

				typedef Activity_Location_Components::Prototypes::Activity_Location<typename remove_pointer<typename location_itf::get_type_of(origin_links)::value_type>::type> link_itf;
				typedef Random_Access_Sequence<typename location_itf::get_type_of(origin_links),link_itf*> links_itf;

				typedef Scenario_Components::Prototypes::Scenario<typename ComponentType::Master_Type::scenario_type> _Scenario_Interface;
				_Scenario_Interface* scenario = (_Scenario_Interface*)_global_scenario;

				network_itf* network = this->network_reference<  network_itf*>();
				skimmer_itf* skim = this->skim_reference<  skimmer_itf*>();
				locations_itf* activity_locations = network->template activity_locations_container<locations_itf*>();
				
				// origin to zone / destination to zone mappings
				typedef (origin_locations_itf,origin_location_itf,typename skimmer_itf::get_type_of(origin_locations), Random_Access_Sequence,Activity_Location_Components::Prototypes::Activity_Location);
				typedef (destination_locations_itf,destination_location_itf,typename skimmer_itf::get_type_of(destination_locations), Random_Access_Sequence,Activity_Location_Components::Prototypes::Activity_Location);
				origin_locations_itf* origin_locations = skim->template origin_locations<origin_locations_itf*>();
				destination_locations_itf* destination_locations = skim->template destination_locations<destination_locations_itf*>();		
				typename origin_locations_itf::iterator orig_itr = origin_locations->begin();
				typename destination_locations_itf::iterator dest_itr = destination_locations->begin();
				typename zone_location_count_itf::iterator zone_origin_count_itr;
				typename zone_location_count_itf::iterator zone_destination_count_itr;

				// get reference to the routers used to create path-trees from each origin
				typedef Routing_Components::Prototypes::Routing<typename remove_pointer<typename skimmer_itf::get_type_of(path_trees_container)::value_type>::type> path_tree_itf;
				typedef Pair_Associative_Container<typename skimmer_itf::get_type_of(path_trees_container),path_tree_itf*> path_trees_itf;

				path_trees_itf* trees_container = skim->template path_trees_container<path_trees_itf*>();
				typename path_trees_itf::iterator tree_itr = trees_container->begin();	
					
				// reference to the LOS skim table and its iterator	
				matrix<typename MasterType::los_value_type*>* los = (matrix<typename MasterType::los_value_type*>*)&this->_skim_table;
				typedef typename matrix<typename MasterType::los_value_type*>::size_type size_t;

				matrix<float> los_old;
				if (scenario->template read_skim_tables<bool>())
				{
					pair<size_t,size_t> dims = los->dimensions();
					los_old.resize(dims,0);
					
					for (int i=0; i<(int)dims.first; ++i)
					{
						for (int j=0; j<(int)dims.second; ++j)
						{
							los_old[pair<size_t,size_t>(i,j)] = ((los_value_itf*)((*los)[pair<size_t,size_t>(i,j)]))->auto_ttime<Stored_Time_Type>();
							((los_value_itf*)((*los)[pair<size_t,size_t>(i,j)]))->auto_ttime<Stored_Time_Type>(0.0);
						}
					}
				}


				// fit characteristics initialized
				float sum_of_deviation=0.0;
				float sum_of_weight = 0.0;
				float max_dev = 0.0;

				// loop through each tree router, extract travel time info and place into los_skim
				for (;tree_itr != trees_container->end(); ++tree_itr)
				{
					long orig_index = tree_itr->first;
					path_tree_itf* tree = tree_itr->second;

					// get the origin activity location from the stored index in the route-tree map item
					origin_location_itf* orig_loc = origin_locations->at(orig_index);
					int orig_zone_index = orig_loc->zone<zone_itf*>()->internal_id<int>();

					// search each link for required links from destination_map and update LOS table with the results
					for (dest_itr = destination_locations->begin(); dest_itr !=destination_locations->end(); ++dest_itr)
					{
						destination_location_itf* dest_node = *dest_itr;
						int dest_zone_index = dest_node->zone<zone_itf*>()->internal_id<int>();

						long dest_link_index = (*(dest_node->template destination_links<links_itf*>()->begin()))->template internal_id<long>();
						long dest_link_id = (*(dest_node->template destination_links<links_itf*>()->begin()))->template uuid<long>();
						float time = tree->template Get_Tree_Results_For_Destination<typename skimmer_itf::Component_Type::Stored_Time_Type>(dest_link_index);
						link_itf* dest_link = network->template links_container<links_itf&>()[dest_link_index];
						
						// calculate weight based on number of routed od pairs as not all o/d zones will have the full number of origin/destination locations
						zone_origin_count_itr = skim->template zone_origins_count<zone_location_count_itf&>().find(orig_loc->zone<zone_itf*>()->internal_id<int>());
						zone_destination_count_itr = skim->template zone_destinations_count<zone_location_count_itf&>().find(dest_node->zone<zone_itf*>()->internal_id<int>());
						float weight = 1.0 / ((float)zone_origin_count_itr->second * (float)zone_destination_count_itr->second);

						// ignore location pairs were no valid route was found
						if (time > 1440)
						{
							typename links_itf::iterator link_itr = dest_node->template destination_links<links_itf*>()->begin();
							for (;link_itr != dest_node->template destination_links<links_itf*>()->end(); ++link_itr)
							{
								cout << "Skimming error, destination location ID '" << dest_node->uuid<int>() << "' of land us type '"<<dest_node->Is_Routable_Location<bool>()<<"', is inaccessible from zone id '" << orig_loc->zone<zone_itf*>()->uuid<int>() <<"', location ID '"<< orig_loc->uuid<int>()<< " .  Destination link ID: "  << (*link_itr)->uuid<long>()<<". Check network for proper connectivity." <<endl;
							}
							continue;
						}

						if (dest_zone_index == orig_zone_index) time = GLOBALS::Time_Converter.Convert_Value<Time_Minutes, typename skimmer_itf::Component_Type::Stored_Time_Type>(2.0);
						if (orig_loc == dest_node) time = 0;

						// if updating of a previous skim file is specified, include the old skim value in the weighting calculation
						if (scenario->template read_skim_tables<bool>()) 
						{
							//float time_old = ((los_value_itf*)(los_old[pair<size_t,size_t>(orig_zone_index,dest_zone_index)]))->auto_ttime<typename skimmer_itf::Component_Type::Stored_Time_Type>();
							float time_old = los_old[pair<size_t,size_t>(orig_zone_index,dest_zone_index)];
							sum_of_deviation += (time-time_old);
							sum_of_weight += time_old;
							float dev = time_old > 0.0 ? (time-time_old)/time_old : 0.0;
							if (dev > max_dev) max_dev = dev;
							float tmp = ((los_value_itf*)((*los)[pair<size_t,size_t>(orig_zone_index,dest_zone_index)]))->auto_ttime<typename skimmer_itf::Component_Type::Stored_Time_Type>();
							tmp += (0.75 * time+ 0.25 * time_old)*weight;
							((los_value_itf*)((*los)[pair<size_t,size_t>(orig_zone_index,dest_zone_index)]))->auto_ttime<typename skimmer_itf::Component_Type::Stored_Time_Type>(tmp);
						}
						// otherwise, just add current weighted value
						else
						{
							float tmp = ((los_value_itf*)((*los)[pair<size_t,size_t>(orig_zone_index,dest_zone_index)]))->auto_ttime<typename skimmer_itf::Component_Type::Stored_Time_Type>();
							tmp += time*weight;
							((los_value_itf*)((*los)[pair<size_t,size_t>(orig_zone_index,dest_zone_index)]))->auto_ttime<typename skimmer_itf::Component_Type::Stored_Time_Type>(tmp);
						}
					}
				}
				if (scenario->template read_skim_tables<bool>()) 
				{
					this->weighted_deviation<  float>(sum_of_deviation/sum_of_weight);
					this->max_deviation<  float>(max_dev);

					stringstream outline("");
					outline<<iteration()<<","<<this->weighted_deviation<  float>()<<","<<this->max_deviation<  float>();
//					File_IO::File_Writer& out_file = skim->template skim_fit_results_file<File_IO::File_Writer&>();
					out_file.Write_Line(outline);
					cout << "W.A.A.P.D from previous skim: " << this->weighted_deviation<  float>()<<endl;
					cout << "Maximum deviation from previous: " << this->max_deviation<  float>()<<endl;
				}
				los_old.clear();

				return true;
			}
			template<typename TargetType> void Write_LOS()
			{
				typedef Prototypes::LOS<typename remove_pointer< typename type_of(skim_table)::value_type>::type> los_value_itf;
				typedef Multidimensional_Random_Access_Array< typename type_of(skim_table),los_value_itf*> skim_table_itf;

				typedef Network_Components::Prototypes::Network<typename type_of(network_reference)> network_itf;
				typedef Zone_Components::Prototypes::Zone<typename remove_pointer<typename network_itf::get_type_of(zones_container)::value_type>::type> zone_itf;
				typedef Pair_Associative_Container<typename network_itf::get_type_of(zones_container),zone_itf*> zones_itf;

				typedef Prototypes::Network_Skimming<typename type_of(skim_reference)> skimmer_itf;
					
				skimmer_itf* skim = this->skim_reference<skimmer_itf*>();
				network_itf* network = this->network_reference<network_itf*>();
				zones_itf* zones = network->template zones_container<zones_itf*>();
				int num_pairs = (int)(zones->size()*zones->size());

	
				matrix<typename MasterType::los_value_type*>* los = &this->_skim_table;


				float* values = new float[num_pairs];
				for (int i=0; i < num_pairs; ++i)
				{
					typename MasterType::los_value_type* los_value = (los->get_data_pointer())[i];
					values[i] = los_value->auto_ttime<typename MasterType::los_value_type,Stored_Time_Type>();
				}


//				File_IO::Binary_File_Writer& bw = skim->template highway_output_file<File_IO::Binary_File_Writer&>();
				bw.WriteArray<float>(values, (int)(zones->size() * zones->size()));
				
				delete values;

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
			template<typename ParamType, typename ReturnType> ReturnType Get_LOS(ParamType Origin_Index, ParamType Destination_Index)
			{		
				ReturnType ret_value = (ReturnType)_skim_table[typename skim_table_itf::index_type(Origin_Index,Destination_Index)];
				return ret_value;
			}
		};



		//======================================================================================
		/// Basic skimming implementation
		//--------------------------------------------------------------------------------------
		implementation struct Basic_Network_Skimming_Implementation : public Polaris_Component<MasterType,INHERIT(Basic_Network_Skimming_Implementation),Execution_Object>
		{
			// Tag as implementation
			typedef typename Polaris_Component<MasterType,INHERIT(Basic_Network_Skimming_Implementation),Data_Object>::Component_Type ComponentType;
			typedef Prototypes::Network_Skimming<ComponentType> skimmer_itf;

			typedef Time_Minutes Stored_Time_Type;
			typedef Basic_Units::Currency_Variables::Cents Stored_Currency_Type;

			// reference to the transportation network
			m_prototype(Network_Components::Prototypes::Network,typename MasterType::network_type, network_reference, NONE, NONE);

			// time increment at which skim tables are updated - set in the initializer
			m_data(Basic_Units::Implementations::Time_Implementation<MasterType>,_update_increment, NONE, NONE);
			member_component_feature(update_increment,_update_increment,Value,Basic_Units::Prototypes::Time);

			// time at which skim tables are next updated - set in the initializer and updated every time update is called
			m_data(Basic_Units::Implementations::Time_Implementation<MasterType>,_scheduled_update_time, NONE, NONE);
			member_component_feature(scheduled_update_time,_scheduled_update_time,Value,Basic_Units::Prototypes::Time);
			m_data(long, nodes_per_zone, NONE, NONE);

			// routers to do the skimming
			m_container(concat(dense_hash_map<long,Routing_Components::Prototypes::Routing<typename MasterType::skim_routing_type>*>), path_trees_container, NONE, NONE);
			
			// link-to-zone mapping for use in skimming
			m_container(boost::container::vector<Activity_Location<typename MasterType::activity_location_type>*>,origin_locations, NONE, NONE);
			m_container(boost::container::vector<Activity_Location<typename MasterType::activity_location_type>*>,destination_locations, NONE, NONE);
			m_container(concat(dense_hash_map<long,int>),zone_origins_count, NONE, NONE);
			m_container(concat(dense_hash_map<long,int>),zone_destinations_count, NONE, NONE);

			m_data(Counter, timer, NONE, NONE);
			m_data(bool, write_output, NONE, NONE);
			m_data(bool, read_input, NONE, NONE);
			m_data(bool, read_transit, NONE, NONE);
			m_data(bool, read_highway_cost, NONE, NONE);
			m_data(File_IO::Binary_File_Writer, highway_output_file, NONE, NONE);
			m_data(File_IO::Binary_File_Reader, highway_input_file, NONE, NONE);
			m_data(File_IO::Binary_File_Writer, highway_cost_output_file, NONE, NONE);
			m_data(File_IO::Binary_File_Reader, highway_cost_input_file, NONE, NONE);
			m_data(File_IO::Binary_File_Writer, transit_output_file, NONE, NONE);
			m_data(File_IO::Binary_File_Reader, transit_input_file, NONE, NONE);
			m_data(File_IO::File_Writer, skim_fit_results_file, NONE, NONE);

			m_container(boost::container::vector<Prototypes::Skim_Table<Skim_Table_Implementation<MasterType>>*>, skims_by_time_container, NONE, NONE);
			m_container(boost::container::list<int>, available_modes_container, NONE, NONE);


			// set the network and skimmer references
			typedef Network_Components::Prototypes::Network<typename type_of(network_reference)> network_itf;
			
			typedef Pair_Associative_Container<typename network_itf::get_type_of(zones_container)> zones_itf;
			typedef Zone_Components::Prototypes::Zone<typename get_mapped_component_type(zones_itf)> zone_itf;

			typedef Activity_Location_Components::Prototypes::Activity_Location<typename remove_pointer<typename zone_itf::get_type_of(origin_activity_locations)::value_type>::type> location_itf;
			typedef Random_Access_Sequence<typename zone_itf::get_type_of(origin_activity_locations),location_itf*> locations_itf;

			typedef Link_Components::Prototypes::Link<typename remove_pointer<typename location_itf::get_type_of(origin_links)::value_type>::type> link_itf;
			typedef Random_Access_Sequence<typename location_itf::get_type_of(origin_links),link_itf*> links_itf;

			//typedef Activity_Location_Components::Prototypes::Activity_Location<typename remove_pointer<typename type_of(origin_locations)::value_type>::type> origin_location_itf;
			//typedef  Random_Access_Sequence<typename type_of(origin_locations),origin_location_itf*> origin_locations_itf;
			typedef Prototype_Random_Access_Sequence<typename type_of(origin_locations),Activity_Location_Components::Prototypes::Activity_Location> origin_locations_itf;
			typedef strip_modifiers(typename origin_locations_itf::value_type) origin_location_itf;

			//typedef Prototypes::Skim_Table<typename remove_pointer<typename type_of(skims_by_time_container)::value_type>::type> skim_table_itf;
			typedef Prototype_Random_Access_Sequence<typename type_of(skims_by_time_container),Prototypes::Skim_Table> skim_tables_itf;
			typedef strip_modifiers(typename skim_tables_itf::value_type) skim_table_itf;

			typedef Prototypes::LOS<typename remove_pointer<typename skim_table_itf::get_type_of(skim_table)::value_type>::type> los_value_itf;
			typedef  Multidimensional_Random_Access_Array<typename skim_table_itf::get_type_of(skim_table),los_value_itf*> skim_matrix_itf;

			typedef Prototypes::LOS<typename los_value_itf::get_type_of(LOS_time_invariant)> los_invariant_value_itf;
			//typedef (modes_skim_container_itf,mode_skim_itf,typename skimmer_itf::get_type_of(mode_skim_table_container), Pair_Associative_Container,Prototypes::Mode_Skim_Table);
				

			template<typename TargetType> void Initialize()
			{
				this->_path_trees_container.set_empty_key(-1);
				this->_path_trees_container.set_deleted_key(-2);

				// initialize dense_hash_map keys
				this->_zone_origins_count.set_empty_key(-1);
				this->_zone_origins_count.set_deleted_key(-2);
				this->_zone_destinations_count.set_empty_key(-1);
				this->_zone_destinations_count.set_deleted_key(-2);

				// create interface to this and set skimming parameters
				typedef Prototypes::Network_Skimming<ComponentType> this_itf;
				this_itf* pthis = (this_itf*)this;
				pthis->template update_increment<Time_Hours>(1);
				pthis->template scheduled_update_time<Simulation_Timestep_Increment>(0.0);
				pthis->template nodes_per_zone<long>(4);

				// add the available modes for the current model
				this->_available_modes_container.push_back(Vehicle_Components::Types::Vehicle_Type_Keys::SOV);
				this->_available_modes_container.push_back(Vehicle_Components::Types::Vehicle_Type_Keys::BUS);

				typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;
				_Scenario_Interface* scenario = (_Scenario_Interface*)_global_scenario;

				stringstream filename("");
				filename << scenario->template output_dir_name<string>();
				filename << "skim_file_fit_results.xls";

				this->_skim_fit_results_file.Open(filename.str());
				stringstream header("Time,WAAPD,maxPD");
				this->_skim_fit_results_file.Write_Line(header);
			}

			template<typename TargetType> void Initialize_Skims()
			{
				network_itf* network = this->template network_reference<network_itf*>();
				skimmer_itf* skim = (skimmer_itf*)this;


				// tree builder interface
				//typedef Routing_Components::Prototypes::Routing<typename remove_pointer<typename  type_of(path_trees_container)::value_type>::type>  tree_builder_itf;
				typedef Pair_Associative_Container< type_of(path_trees_container)> tree_builder_list_itf;
				typedef Routing_Components::Prototypes::Routing<typename get_mapped_component_type(tree_builder_list_itf)> tree_builder_itf;

				tree_builder_list_itf* tree_list = this->path_trees_container<tree_builder_list_itf*>();

				origin_locations_itf* origin_locations = skim->template origin_locations<origin_locations_itf*>();	
				zones_itf* zones_container = network->template zones_container<zones_itf*>();
				typename origin_locations_itf::iterator orig_itr = origin_locations->begin();
				locations_itf* activity_locations = network->template activity_locations_container<locations_itf*>();
				
				// create a tree builder for each origin to skim
				long orig_index = 0;
				for (;orig_itr != origin_locations->end(); ++orig_itr, ++orig_index)
				{

					//assert(false);

					////TODO: NEED TO REDO SKIM ROUTER TO USE THE NEW ROUTER IMPLEMENTATION
					//// get the origin/destination indices
					//origin_location_itf* orig_node = (origin_location_itf*)*orig_itr;

					//// Allocate a tree_builder for each origin node		
					//tree_builder_itf* tree_builder = (tree_builder_itf*)Allocate<typename tree_builder_itf::Component_Type>();

					//// Set the current routable origin for the tree builder
					//tree_builder->template routable_origin<link_itf*>((link_itf*)*(orig_node->template origin_links<links_itf*>()->begin()));
					//tree_builder->template network<network_itf*>(network);

					//// Set the start, end and update times for the router
					//tree_builder->template update_increment<Simulation_Timestep_Increment>(skim->template update_increment<Simulation_Timestep_Increment>());
					//tree_builder->template start_time<Simulation_Timestep_Increment>(0);//this->start_time<Simulation_Timestep_Increment>());
					//tree_builder->template end_time<Simulation_Timestep_Increment>(END);//this->end_time<Simulation_Timestep_Increment>());

					//// Add the tree_builder to the boost::container::list for future processing
					//tree_builder->template Initialize_Tree_Computation<NULLTYPE>(iteration());

					//pair<long,tree_builder_itf*> item = pair<long,tree_builder_itf*>(orig_index,tree_builder);
					//tree_list->insert(item);
				}

				Simulation_Timestep_Increment start;

				//===========================================================================
				// FILE INPUT IF REQUESTED - set up binary file readers for skim files
				int num_modes;
				int num_zones, num_zones_transit, num_zones_hcost;
				int update_increment;

				// read for time-varying highway skims
				File_IO::Binary_File_Reader& infile = skim->template highway_input_file<File_IO::Binary_File_Reader&>();
				if (skim->template read_input<bool>())
				{		
					infile.Read_Value<int>(num_modes);
					infile.Read_Value<int>(num_zones);
					infile.Read_Value<int>(update_increment);
					if (update_increment != skim->template update_increment<Simulation_Timestep_Increment>()) THROW_EXCEPTION("ERROR: Input skim file update increment does not match the update increment specified in Skim_Implementation.");
					if (num_zones != zones_container->size()) THROW_EXCEPTION("ERROR: Input skim file number of zones does not match the number of zones specified in the input database.");
				}
				else
				{
					num_zones = (int)zones_container->size();
				}
				File_IO::Binary_File_Reader& transit_infile = skim->template transit_input_file<File_IO::Binary_File_Reader&>();
				if (skim->template read_input<bool>() && skim->template read_transit<bool>())
				{		
					transit_infile.Read_Value<int>(num_zones_transit);
					if (num_zones_transit != num_zones) THROW_EXCEPTION("ERROR: Input transit skim file number of zones does not match the number of zones in the highway skim file. Transit="<<num_zones_transit<<" and Highway="<<num_zones);
				}
				File_IO::Binary_File_Reader& highway_cost_infile = skim->template highway_cost_input_file<File_IO::Binary_File_Reader&>();
				if (skim->template read_input<bool>() && skim->template read_highway_cost<bool>())
				{		
					highway_cost_infile.Read_Value<int>(num_zones_hcost);
					if (num_zones_hcost != num_zones) THROW_EXCEPTION("ERROR: Input highway cost skim file number of zones does not match the number of zones in the highway skim file. Cost="<<num_zones_transit<<" and Highway="<<num_zones);
				}

				//===========================================================================
				// create the time-invariant skim tables
				float* auto_tolls = new float[num_zones*num_zones]();
				float* auto_parking_cost = new float[num_zones*num_zones]();
				float* transit_ttime = new float[num_zones*num_zones]();
				float* transit_walk_access_time = new float[num_zones*num_zones]();
				float* auto_distance /*transit_sov_access_time */= new float[num_zones*num_zones]();
				float* transit_wait_time = new float[num_zones*num_zones]();
				float* transit_fare = new float[num_zones*num_zones]();
				if (skim->template read_input<bool>() && skim->read_transit<bool>())
				{
					// Read transit input data if required
					if (skim->read_transit<bool>())
					{		
						transit_infile.Read_Array<float>(transit_ttime, num_zones*num_zones);
						transit_infile.Read_Array<float>(transit_walk_access_time, num_zones*num_zones);
						transit_infile.Read_Array<float>(auto_distance /*transit_sov_access_time*/, num_zones*num_zones);
						transit_infile.Read_Array<float>(transit_wait_time, num_zones*num_zones);
						transit_infile.Read_Array<float>(transit_fare, num_zones*num_zones);
					}
				}
				// otherwise initialize to unavailable
				else
				{
					for (int i =0; i < num_zones*num_zones; ++i)
					{
						transit_ttime[i] = 9999.0;
						transit_walk_access_time[i] = 9999.0;
						auto_distance/*transit_sov_access_time*/[i] = 9999.0;
						transit_wait_time[i] = 9999.0;
						transit_fare[i] = 9999.0;
					}
				}
				// Get raw auto travel time data from binary file for time-invariant fields - move down into time loop below if these become time varying
				infile.Read_Array<float>(auto_tolls, num_zones*num_zones);
				infile.Read_Array<float>(auto_parking_cost, num_zones*num_zones);
				typename MasterType::los_invariant_value_type** temp_invariant_los_array = new typename MasterType::los_invariant_value_type*[num_zones*num_zones];
				for (int i =0; i < num_zones*num_zones; ++i)
				{
					typename MasterType::los_invariant_value_type* temp_los = Allocate<typename MasterType::los_invariant_value_type>();
					((los_invariant_value_itf*)temp_los)->auto_tolls<Stored_Currency_Type>(auto_tolls[i]);
					((los_invariant_value_itf*)temp_los)->auto_parking_cost<Stored_Currency_Type>(auto_parking_cost[i]);
					((los_invariant_value_itf*)temp_los)->transit_ttime<Stored_Time_Type>(transit_ttime[i]);
					((los_invariant_value_itf*)temp_los)->transit_walk_access_time<Stored_Time_Type>(transit_walk_access_time[i]);
					((los_invariant_value_itf*)temp_los)->auto_distance /*transit_sov_access_time<Stored_Time_Type>*/<Miles>(auto_distance/*transit_sov_access_time*/[i]);
					((los_invariant_value_itf*)temp_los)->transit_wait_time<Stored_Time_Type>(transit_wait_time[i]);
					((los_invariant_value_itf*)temp_los)->transit_fare<Stored_Currency_Type>(transit_fare[i]);
					temp_invariant_los_array[i] = temp_los;
				}
				delete auto_tolls;
				delete auto_parking_cost;
				delete transit_ttime;
				delete transit_walk_access_time;
				delete auto_distance /*transit_sov_access_time*/;
				delete transit_wait_time;
				delete transit_fare;

				

				//===========================================================================
				// create the skim_table time periods, for basic create only a single time period skim_table
				for (start = 0; start < GLOBALS::Time_Converter.template Convert_Value<Time_Hours,Simulation_Timestep_Increment>(24.0); start = start + skim->template update_increment<Simulation_Timestep_Increment>())
				{		
					skim_table_itf* skim_table = (skim_table_itf*)Allocate<strip_modifiers(typename type_of(skims_by_time_container)::value_type)::Component_Type>();
					skim_table->template network_reference<network_itf*>(network);
					skim_table->template skim_reference<skimmer_itf*>(skim);
					skim_table->template start_time<Simulation_Timestep_Increment>(start);
					skim_table->template end_time<Simulation_Timestep_Increment>(start + skim->template update_increment<Simulation_Timestep_Increment>());

					if (skim->template read_input<bool>())
					{
						// Get raw auto travel time data from binary file
						float* data = new float[num_zones*num_zones];
						infile.Read_Array<float>(data, num_zones*num_zones);

						// create LOS matrix using raw input data
						typename MasterType::los_value_type** temp_los_array = new typename MasterType::los_value_type*[num_zones*num_zones];
						for (int i =0; i < num_zones*num_zones; ++i)
						{
							typename MasterType::los_value_type* temp_los = Allocate<typename MasterType::los_value_type>();
							((los_value_itf*)temp_los)->auto_ttime<Stored_Time_Type>(data[i]);
							((los_value_itf*)temp_los)->LOS_time_invariant<typename MasterType::los_invariant_value_type*>(temp_invariant_los_array[i]);
							temp_los_array[i] = temp_los;
						}
						skim_table->template Initialize<typename MasterType::los_value_type**>(temp_los_array);
					}
					else
					{
						// create LOS matrix using default values
						typename MasterType::los_value_type** temp_los_array = new typename MasterType::los_value_type*[num_zones*num_zones];
						for (int i =0; i < num_zones*num_zones; ++i)
						{
							typename MasterType::los_value_type* temp_los = Allocate<typename MasterType::los_value_type>();
							((los_value_itf*)temp_los)->auto_ttime<Stored_Time_Type>(0.0f);
							((los_value_itf*)temp_los)->LOS_time_invariant<typename MasterType::los_invariant_value_type*>(temp_invariant_los_array[i]);
							temp_los_array[i] = temp_los;
						}
						skim_table->template Initialize<typename MasterType::los_value_type**>(temp_los_array);
					}

					// add time period skim tables to the container
					skim_tables_itf* skim_tables = this->template skims_by_time_container<skim_tables_itf*>();
					skim_tables->push_back(skim_table);
				}


				//===========================================================================
				// make sure intra-zonal information is initialized
				for (zones_itf::iterator zone_itr = zones_container->begin(); zone_itr != zones_container->end(); ++zone_itr)
				{
					zone_itf* zone = (zone_itf*)(zone_itr->second);
					los_value_itf* los = skim->Get_LOS<zone_itf*,los_value_itf*>(zone, zone);

					if (los->auto_distance<Miles>() > 1000)
					{
						Miles new_distance = sqrt(zone->area<Square_Miles>())/2.0*1.2;
						los->auto_distance<Miles>(new_distance);
					}
				}

				//===========================================================================
				// Initial FILE_OUTPUT IF REQUESTED
				File_IO::Binary_File_Writer& outfile = skim->template highway_output_file<File_IO::Binary_File_Writer&>();
				if (skim->template write_output<bool>() == true)
				{
					int modes = 1;//(int)(skim->template mode_skim_table_container<modes_skim_container_itf&>().size());
					outfile.Write_Value<int>(modes);
					int zones = (int)zones_container->size();
					outfile.Write_Value<int>(zones);
					int increment = skim->template update_increment<Simulation_Timestep_Increment>();
					outfile.Write_Value<int>(increment);
				}

				//===========================================================================
				// Calculate accessibilities for each zone based on initial skims
				Update_Zone_Accessibilities<TargetType>();
			
			}

			template<typename TargetType> void Update_Zone_Accessibilities()
			{
				network_itf* network = this->template network_reference<network_itf*>();
				zones_itf* zones_container = network->template zones_container<zones_itf*>();
				typedef zones_itf::iterator zone_iterator;
				skimmer_itf* skim = (skimmer_itf*)this;

				float Nz_inv = 1.0 / (zones_container->size() - 1.0);
				float alpha = -0.15;

				// loop through all zones and calculate competing destinations factors for employment categories
				for (zone_iterator o_zone_itr = zones_container->begin(); o_zone_itr != zones_container->end(); ++o_zone_itr)
				{
					// Interface to origin zone
					zone_itf* zone = (zone_itf*)(o_zone_itr->second);

					float C_gov=0;
					float C_man=0;
					float C_ret=0;
					float C_ser=0;
					float C_ind=0;
					float C_oth=0;

					float auto_ttime_avg=0, auto_ttime_avg_comp=0, auto_ttime_offpeak_avg=0, auto_ttime_offpeak_avg_comp=0, tran_ttime_avg=0;
					int count=0, count_comp=0;
					float attract_count = 0, attract_count_comp=0;
								
					// loop through all destination zones
					for (zone_iterator d_zone_itr = zones_container->begin(); d_zone_itr != zones_container->end(); ++d_zone_itr)
					{
						// interface to destination zone
						zone_itf* dzone = (zone_itf*)(d_zone_itr->second);

						// do not include the origin zone in calculations
						if (zone == dzone) continue;

						// get TTime by automobile and transit through skim interface, in minutes
						Time_Minutes ttime_auto_peak = skim->Get_TTime<zone_itf*,Vehicle_Components::Types::Vehicle_Type_Keys, Time_Hours,Time_Minutes>(zone,dzone,Vehicle_Components::Types::SOV, 9.0);
						Time_Minutes ttime_auto_offpeak = skim->Get_TTime<zone_itf*,Vehicle_Components::Types::Vehicle_Type_Keys, Time_Hours,Time_Minutes>(zone,dzone,Vehicle_Components::Types::SOV, 13.0);
						Time_Minutes ttime_transit = skim->Get_TTime<zone_itf*,Vehicle_Components::Types::Vehicle_Type_Keys,Time_Minutes>(zone,dzone,Vehicle_Components::Types::BUS);

						// update the accessibilty factors: (1/Nz-1) * (sum(Emp * exp(alpha*ttime)))
						C_gov += Nz_inv * dzone->employment_government<float>() * exp(alpha*ttime_auto_peak);
						C_man += Nz_inv * dzone->employment_manufacturing<float>() * exp(alpha*ttime_auto_peak);
						C_ret += Nz_inv * dzone->employment_retail<float>() * exp(alpha*ttime_auto_peak);
						C_ser += Nz_inv * dzone->employment_services<float>() * exp(alpha*ttime_auto_peak);
						C_ind += Nz_inv * dzone->employment_industrial<float>() * exp(alpha*ttime_auto_peak);
						C_oth += Nz_inv * dzone->employment_other<float>() * exp(alpha*ttime_auto_peak);

						float attract = dzone->employment_total<float>() + dzone->pop_persons<float>();

						// update the average travel time from 'zone' for transit and auto if transit is available
						if (ttime_transit < 1440 && ttime_auto_peak < 1440)
						{			
							auto_ttime_avg_comp += (ttime_auto_peak * attract);
							auto_ttime_offpeak_avg_comp += (ttime_auto_offpeak * attract);
							tran_ttime_avg += (ttime_transit * attract);
							count_comp++;
							attract_count_comp += attract;
						}
						// updata average auto travel times if auto is available
						if (ttime_auto_peak < 1440)
						{
							auto_ttime_avg += (ttime_auto_peak * attract);
							auto_ttime_offpeak_avg += (ttime_auto_offpeak * attract);
							count++;
							attract_count += attract;
						}
					}

					// Add accessiblity factors to origin zone
					zone->accessibility_employment_government(C_gov);
					zone->accessibility_employment_industrial(C_ind);
					zone->accessibility_employment_manufacturing(C_man);
					zone->accessibility_employment_other(C_oth);
					zone->accessibility_employment_retail(C_ret);
					zone->accessibility_employment_services(C_ser);

					//cout << C_gov <<" , " << C_ind <<" , " << C_man <<" , " << C_oth <<" , " << C_ret <<" , " << C_ser<<endl;

					// update the average travel times to all other zones for 'zone' by mode - weighted by attractiveness of zone (i.e. how well served are the attractive zones)
					zone->avg_ttime_auto_offpeak<Time_Minutes>(auto_ttime_offpeak_avg/attract_count);
					zone->avg_ttime_auto_peak<Time_Minutes>(auto_ttime_avg/attract_count);
					if (count_comp > 10) zone->avg_ttime_transit<Time_Minutes>(tran_ttime_avg/attract_count_comp);
					else zone->avg_ttime_transit<Time_Minutes>(9999);
					if (count_comp > 10) zone->avg_ttime_auto_to_transit_accessible_zones<Time_Minutes>((0.5*auto_ttime_avg_comp + 0.5*auto_ttime_offpeak_avg_comp)/attract_count_comp);
					else zone->avg_ttime_auto_to_transit_accessible_zones<Time_Minutes>(9999);

				}
			}
	
		};


		//======================================================================================
		/// More advance skimming implementation not yet implemented
		//--------------------------------------------------------------------------------------
		implementation struct Advanced_Network_Skimming_Implementation : public Basic_Network_Skimming_Implementation<MasterType, INHERIT(Advanced_Network_Skimming_Implementation)>
		{
			// Tag as implementation
			typedef typename Basic_Network_Skimming_Implementation<MasterType, INHERIT(Advanced_Network_Skimming_Implementation)>::Component_Type ComponentType;
		};
	}

}
