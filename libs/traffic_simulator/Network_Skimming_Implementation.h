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

			// Table containing los values
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
			typedef Prototypes::LOS<typename remove_pointer< typename type_of(skim_table)::value_type>::type> los_value_itf;
			typedef Multidimensional_Random_Access_Array< type_of(skim_table),los_value_itf*> skim_table_itf;

			typedef Network_Components::Prototypes::Network<type_of(network_reference)> network_itf;
			typedef Prototypes::Network_Skimming<type_of(skim_reference)> skimmer_itf;
			typedef Zone_Components::Prototypes::Zone<typename remove_pointer<typename network_itf::get_type_of(zones_container)::mapped_type>::type> zone_itf;
			typedef Pair_Associative_Container<typename network_itf::get_type_of(zones_container)> zones_itf;

			typedef Activity_Location_Components::Prototypes::Activity_Location<typename remove_pointer<typename zone_itf::get_type_of(origin_activity_locations)::value_type>::type> location_itf;
			typedef Random_Access_Sequence<typename zone_itf::get_type_of(origin_activity_locations),location_itf*> locations_itf;

			typedef Activity_Location_Components::Prototypes::Activity_Location<typename remove_pointer<typename location_itf::get_type_of(origin_links)::value_type>::type> link_itf;
			typedef Random_Access_Sequence<typename location_itf::get_type_of(origin_links),link_itf*> links_itf;

			//typedef Prototypes::Location_To_Zone_Map_Item<typename remove_pointer<typename skimmer_itf::get_type_of(origin_node_to_zone_map)::value_type>::type> origin_item_itf;
			//typedef  Pair_Associative_Container<typename skimmer_itf::get_type_of(origin_node_to_zone_map),origin_item_itf*> origin_map_itf;

			typedef typename skim_reference_component_type::Stored_Time_Type Stored_Time_Type;
			

			// vector of multimaps, one for each origin zone, which contains travel times to destination zones, sorted in ascending order
			//m_container(concat(std::vector<std::vector<std::pair<float, zone_itf*>>>), auto_travel_time_sorter, NONE, NONE);
			//m_container(concat(std::vector<std::vector<std::pair<float, zone_itf*>>>), transit_travel_time_sorter, NONE, NONE);
			m_container(concat(std::vector<std::vector<std::pair<float, unsigned short>>>), auto_travel_time_sorter, NONE, NONE);
			m_container(concat(std::vector<std::vector<std::pair<float, unsigned short>>>), transit_travel_time_sorter, NONE, NONE);

			//=============================================
			// Primary function accessors - used to calculate/return LOS values for OD pairs
			//---------------------------------------------
			template<typename TargetType> void Initialize()
			{
				typedef typename matrix<typename MasterType::los_value_type*>::size_type size_t;
				network_itf* network = this->template network_reference<  network_itf*>();
				zones_itf* zones_container = network->template zones_container<zones_itf*>();

				// This is not needed as we use the copy function in the initializer below which automatically resizes the array
				//this->_skim_table.resize(pair<size_t,size_t>((size_t)zones_container->size(),(size_t)zones_container->size()),0);

				// initialize the travel time sorter
				for (int i=0; i < zones_container->size(); i++)
				{
					this->_auto_travel_time_sorter.push_back(std::vector<std::pair<float, unsigned short>>());
					this->_transit_travel_time_sorter.push_back(std::vector<std::pair<float, unsigned short>>());
				}
			}
			template<typename TargetType> void Initialize(TargetType initial_data)
			{
				this->template Initialize<NT>();

				typedef typename matrix<typename MasterType::los_value_type*>::size_type size_t;
				network_itf* network = this->template network_reference<  network_itf*>();
				zones_itf* zones_container = network->template zones_container<zones_itf*>();

				this->_skim_table.Copy(pair<size_t,size_t>((size_t)zones_container->size(),(size_t)zones_container->size()), initial_data);

				// initialize the travel time sorters with the initial data
				for (auto o_itr=zones_container->begin(); o_itr!=zones_container->end(); ++o_itr)
				{
					zone_itf* orig_zone = (zone_itf*)o_itr->second;
					int o_index = orig_zone->template internal_id<int>();

					this->_auto_travel_time_sorter[o_index].clear();
					this->_transit_travel_time_sorter[o_index].clear();

					for (auto d_itr=zones_container->begin(); d_itr!=zones_container->end(); ++d_itr)
					{
						zone_itf* dest_zone = (zone_itf*)d_itr->second;
						int d_index = dest_zone->template internal_id<int>();
						int d_id = dest_zone->template uuid<int>();

						los_itf* los = this->template Get_LOS<int,los_itf*>(o_index,d_index);
						float auto_ttime = los->template auto_ttime<Time_Minutes>();
						float transit_ttime = los->template transit_ttime<Time_Minutes>();

						this->_auto_travel_time_sorter[o_index].push_back(pair<float,unsigned short>(auto_ttime,(unsigned short)d_id/*d_index*/));
						this->_transit_travel_time_sorter[o_index].push_back(pair<float,unsigned short>(transit_ttime,(unsigned short)d_id/*d_index*/));
					}
					sort(this->_auto_travel_time_sorter[o_index].begin(), this->_auto_travel_time_sorter[o_index].end(), Pair_Comparer<float,unsigned short>);
					sort(this->_transit_travel_time_sorter[o_index].begin(), this->_transit_travel_time_sorter[o_index].end(), Pair_Comparer<float,unsigned short>);
				}
			}
			template<typename TargetType> bool Update_LOS()
			{
				// get the network and network element references
				typedef Prototypes::LOS<typename remove_pointer< typename type_of(skim_table)::value_type>::type> los_value_itf;
				typedef Multidimensional_Random_Access_Array< type_of(skim_table),los_value_itf*> skim_table_itf;

				typedef Network_Components::Prototypes::Network<type_of(network_reference)> network_itf;
				typedef Prototypes::Network_Skimming<type_of(skim_reference)> skimmer_itf;
				typedef Pair_Associative_Container<typename skimmer_itf::get_type_of(zone_origins_count),int> zone_location_count_itf;
				
				typedef Random_Access_Sequence<typename network_itf::get_type_of(activity_locations_container)> locations_itf;
				typedef Activity_Location_Components::Prototypes::Activity_Location<get_component_type(locations_itf)> location_itf;
				
				typedef Random_Access_Sequence<typename location_itf::get_type_of(origin_links)> links_itf;
				typedef Link_Components::Prototypes::Link<get_component_type(links_itf)> link_itf;
				
				typedef Scenario_Components::Prototypes::Scenario<typename ComponentType::Master_Type::scenario_type> _Scenario_Interface;
				_Scenario_Interface* scenario = (_Scenario_Interface*)_global_scenario;

				network_itf* network = this->network_reference<  network_itf*>();
				skimmer_itf* skim = this->skim_reference<  skimmer_itf*>();
				locations_itf* activity_locations = network->template activity_locations_container<locations_itf*>();
				zones_itf* zones_container = network->template zones_container<zones_itf*>();
				
				// origin to zone / destination to zone mappings
				typedef Random_Access_Sequence<typename skimmer_itf::get_type_of(origin_locations)> origin_locations_itf;
				typedef Activity_Location_Components::Prototypes::Activity_Location<get_component_type(origin_locations_itf)> origin_location_itf;
				
				typedef Random_Access_Sequence<typename skimmer_itf::get_type_of(origin_locations)> destination_locations_itf;
				typedef Activity_Location_Components::Prototypes::Activity_Location<get_component_type(origin_locations_itf)> destination_location_itf;
				
				origin_locations_itf* origin_locations = skim->template origin_locations<origin_locations_itf*>();
				destination_locations_itf* destination_locations = skim->template destination_locations<destination_locations_itf*>();		
				typename origin_locations_itf::iterator orig_itr = origin_locations->begin();
				typename destination_locations_itf::iterator dest_itr = destination_locations->begin();
				typename zone_location_count_itf::iterator zone_origin_count_itr;
				typename zone_location_count_itf::iterator zone_destination_count_itr;

				// get reference to the routers used to create path-trees from each origin
				typedef Pair_Associative_Container<typename skimmer_itf::get_type_of(path_trees_container)> path_trees_itf;
				typedef Routing_Components::Prototypes::Routing<get_mapped_component_type(path_trees_itf)> path_tree_itf;
				
				path_trees_itf* trees_container = skim->template path_trees_container<path_trees_itf*>();
				typename path_trees_itf::iterator tree_itr = trees_container->begin();	
					
				// reference to the LOS skim table and its iterator	
				matrix<typename MasterType::los_value_type*>* los = (matrix<typename MasterType::los_value_type*>*)&this->_skim_table;
				typedef typename matrix<typename MasterType::los_value_type*>::size_type size_t;

				// copy old values into temporary and reset the current values to zero
				float new_skim_weight = scenario->skim_averaging_factor<float>();
				matrix<float> los_old;
				//if (scenario->template read_skim_tables<bool>())
				//{
					pair<size_t,size_t> dims = los->dimensions();
					los_old.resize(dims,0);
					
					for (int i=0; i<(int)dims.first; ++i)
					{
						for (int j=0; j<(int)dims.second; ++j)
						{
							los_old[pair<size_t,size_t>(i,j)] = ((los_value_itf*)((*los)[pair<size_t,size_t>(i,j)]))->template auto_ttime<Stored_Time_Type>();
							((los_value_itf*)((*los)[pair<size_t,size_t>(i,j)]))->template auto_ttime<Stored_Time_Type>(0.0);
						}
					}
				//}


				// fit characteristics initialized
				float sum_of_deviation=0.0;
				float sum_of_weight = 0.0;
				float max_dev = 0.0;

				stringstream skim_errors("");
				bool has_skim_errors=false;


				// loop through each tree router, extract travel time info and place into los_skim
				for (;tree_itr != trees_container->end(); ++tree_itr)
				{
					long orig_index = tree_itr->first;
					path_tree_itf* tree = (path_tree_itf*)(tree_itr->second);

					// get the origin activity location from the stored index in the route-tree map item
					origin_location_itf* orig_loc = origin_locations->at(orig_index);
					int orig_zone_index = orig_loc->template zone<zone_itf*>()->template internal_id<int>();

					// search each link for required links from destination_map and update LOS table with the results
					for (dest_itr = destination_locations->begin(); dest_itr !=destination_locations->end(); ++dest_itr)
					{
						destination_location_itf* dest_node = *dest_itr;
						int dest_zone_index = dest_node->template zone<zone_itf*>()->template internal_id<int>();

						long dest_link_index = (*(dest_node->template destination_links<links_itf*>()->begin()))->template internal_id<long>();
						long dest_link_id = (*(dest_node->template destination_links<links_itf*>()->begin()))->template uuid<long>();
						float time = tree->template Get_Tree_Results_For_Destination<typename skimmer_itf::Component_Type::Stored_Time_Type>(dest_link_index);
						link_itf* dest_link = network->template links_container<links_itf&>()[dest_link_index];
						
						// calculate weight based on number of routed od pairs as not all o/d zones will have the full number of origin/destination locations
						zone_origin_count_itr = skim->template zone_origins_count<zone_location_count_itf&>().find(orig_loc->template zone<zone_itf*>()->template internal_id<int>());
						zone_destination_count_itr = skim->template zone_destinations_count<zone_location_count_itf&>().find(dest_node->template zone<zone_itf*>()->template internal_id<int>());
						float weight = 1.0 / ((float)zone_origin_count_itr->second * (float)zone_destination_count_itr->second);

						// ignore location pairs where no valid route was found travel time over 10 days
						if (time > FLT_MAX/1000.0f)
						{
							typename links_itf::iterator link_itr = dest_node->template destination_links<links_itf*>()->begin();
							for (;link_itr != dest_node->template destination_links<links_itf*>()->end(); ++link_itr)
							{
								typename links_itf::iterator orig_link_itr = orig_loc->template origin_links<links_itf*>()->begin();
								has_skim_errors=true;
								skim_errors << "SKIMMING ERROR, destination location ID=" << dest_node->template uuid<int>() << ", link ID="  << (*link_itr)->template uuid<long>()<<", is inaccessible from origin location ID="<< orig_loc->template uuid<int>()<< ", link ID="  << (*orig_link_itr)->template uuid<long>()<<". Check network connectivity. Estimated ttime="<<time <<endl;
							}
							continue;
						}

						//if (dest_zone_index == orig_zone_index) time = GLOBALS::Time_Converter.Convert_Value<Time_Minutes, typename skimmer_itf::Component_Type::Stored_Time_Type>(2.0);
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
							float tmp = ((los_value_itf*)((*los)[pair<size_t,size_t>(orig_zone_index,dest_zone_index)]))->template auto_ttime<typename skimmer_itf::Component_Type::Stored_Time_Type>();
							tmp += (new_skim_weight * time+ (1.0-new_skim_weight) * time_old)*weight;
							((los_value_itf*)((*los)[pair<size_t,size_t>(orig_zone_index,dest_zone_index)]))->template auto_ttime<typename skimmer_itf::Component_Type::Stored_Time_Type>(tmp);
						}
						// otherwise, just add current weighted value
						else
						{
							float tmp = ((los_value_itf*)((*los)[pair<size_t,size_t>(orig_zone_index,dest_zone_index)]))->template auto_ttime<typename skimmer_itf::Component_Type::Stored_Time_Type>();
							tmp += time*weight;
							((los_value_itf*)((*los)[pair<size_t,size_t>(orig_zone_index,dest_zone_index)]))->template auto_ttime<typename skimmer_itf::Component_Type::Stored_Time_Type>(tmp);
						}
					}
				}
				if (has_skim_errors)
				{
					File_IO::File_Writer fw;
					fw.Open("skimming_errors.csv");
					fw.Write(skim_errors);
					fw.Close();
					THROW_EXCEPTION("Error: skimming errors found. check 'skimming_errors.csv' log file for more information.");
				}
				if (scenario->template read_skim_tables<bool>()) 
				{
					this->weighted_deviation<  float>(sum_of_deviation/sum_of_weight);
					this->max_deviation<  float>(max_dev);

					stringstream outline("");
					outline<<iteration()<<","<<this->weighted_deviation<  float>()<<","<<this->max_deviation<  float>();
					File_IO::File_Writer& out_file = skim->template skim_fit_results_file<File_IO::File_Writer&>();
					out_file.Write_Line(outline);
					cout << "W.A.A.P.D from previous skim: " << this->weighted_deviation<  float>()<<endl;
					cout << "Maximum deviation from previous: " << this->max_deviation<  float>()<<endl;
				}
				los_old.clear();


				// update the travel time sorters with the initial data
				for (auto o_itr=zones_container->begin(); o_itr!=zones_container->end(); ++o_itr)
				{
					zone_itf* orig_zone = (zone_itf*)(o_itr->second);
					int o_index = orig_zone->template internal_id<int>();

					this->_auto_travel_time_sorter[o_index].clear();
					this->_transit_travel_time_sorter[o_index].clear();

					for (auto d_itr=zones_container->begin(); d_itr!=zones_container->end(); ++d_itr)
					{
						zone_itf* dest_zone = (zone_itf*)(d_itr->second);
						int d_index = dest_zone->template internal_id<int>();

						los_itf* los = this->Get_LOS<int,los_itf*>(o_index,d_index);
						float auto_ttime = los->template auto_ttime<Time_Minutes>();
						float transit_ttime = los->template transit_ttime<Time_Minutes>();

						/*this->_auto_travel_time_sorter[o_index].push_back(pair<float,zone_itf*>(auto_ttime,dest_zone));
						this->_transit_travel_time_sorter[o_index].push_back(pair<float,zone_itf*>(transit_ttime,dest_zone));*/
						this->_auto_travel_time_sorter[o_index].push_back(pair<float,unsigned short>(auto_ttime,dest_zone->template uuid<unsigned short>()));
						this->_transit_travel_time_sorter[o_index].push_back(pair<float,unsigned short>(transit_ttime,dest_zone->template uuid<unsigned short>()));
					}

					sort(this->_auto_travel_time_sorter[o_index].begin(), this->_auto_travel_time_sorter[o_index].end(), Pair_Comparer<float,unsigned short>);
					sort(this->_transit_travel_time_sorter[o_index].begin(), this->_transit_travel_time_sorter[o_index].end(), Pair_Comparer<float,unsigned short>);
				}

				return true;
			}
			template<typename TargetType> void Write_LOS()
			{
				typedef Prototypes::LOS<typename remove_pointer< typename type_of(skim_table)::value_type>::type> los_value_itf;
				typedef Multidimensional_Random_Access_Array< type_of(skim_table),los_value_itf*> skim_table_itf;

				typedef Network_Components::Prototypes::Network<type_of(network_reference)> network_itf;
				typedef Zone_Components::Prototypes::Zone<typename remove_pointer<typename network_itf::get_type_of(zones_container)::value_type>::type> zone_itf;
				typedef Pair_Associative_Container<typename network_itf::get_type_of(zones_container),zone_itf*> zones_itf;

				typedef Prototypes::Network_Skimming<type_of(skim_reference)> skimmer_itf;
					
				skimmer_itf* skim = this->skim_reference<skimmer_itf*>();
				network_itf* network = this->network_reference<network_itf*>();
				zones_itf* zones = network->template zones_container<zones_itf*>();
				int num_pairs = (int)(zones->size()*zones->size());

	
				matrix<typename MasterType::los_value_type*>* los = &this->_skim_table;


				float* values = new float[num_pairs];
				for (int i=0; i < num_pairs; ++i)
				{
					typename MasterType::los_value_type* los_value = (los->get_data_pointer())[i];
					values[i] = los_value->template auto_ttime<Stored_Time_Type>();
				}


				File_IO::Binary_File_Writer& bw = skim->template highway_output_file<File_IO::Binary_File_Writer&>();
				char btag[] = "BMAT";
				//char* btag = "BMAT";
				bw.template Write_Array<char>(btag,4);
				bw.template Write_Array<float>(values, (int)(zones->size() * zones->size()));
				char etag[] = "EMAT";
				bw.template Write_Array<char>(etag,4);
				delete[] values;

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
			template<typename TimeType, typename ModeType, typename ReturnLocationType> void Get_Locations_Within_Range(std::vector<ReturnLocationType>& available_set, int origin_index, TimeType min_time, TimeType max_time, ModeType mode_indicator, bool search_forward, requires(ReturnLocationType, check(ReturnLocationType, is_pointer)))
			{
				network_itf* network = this->network_reference<  network_itf*>();
				zones_itf* zones_container = network->template zones_container<zones_itf*>();

				// get appropriate sorted ttimes based on mode indicator
				std::vector<pair<float, unsigned short>>& ttime_sorter = this->_transit_travel_time_sorter[origin_index];

				if (mode_indicator == Vehicle_Components::Types::Vehicle_Type_Keys::SOV)
				{
					ttime_sorter= this->_auto_travel_time_sorter[origin_index];
				}

	
				// forward search - best when using small min_time
				if (search_forward)
				{
					for (std::vector<pair<float, unsigned short>>::iterator d_itr=ttime_sorter.begin(); d_itr!=ttime_sorter.end(); ++d_itr)
					{
						//zone_itf* dest_zone = (zone_itf*)d_itr->second;
						unsigned short dest_id = d_itr->second;
						typename zones_itf::iterator z_itr;
						if ((z_itr = zones_container->find(dest_id))==zones_container->end()){THROW_EXCEPTION("Error, destination zone id '"<<dest_id<<"' in travel time sorter not found in zones_container.")}
						zone_itf* dest_zone = (zone_itf*)(z_itr->second);

						float ttime=d_itr->first;

						if (ttime < GLOBALS::Time_Converter.Convert_Value<TimeType,Time_Minutes>(max_time))
						{
							if (ttime >= GLOBALS::Time_Converter.Convert_Value<TimeType,Time_Minutes>(min_time)) available_set.push_back(Get_Location_As_Needed<ReturnLocationType>(dest_zone));
						}
						else
						{
							return;
						}
					}
				}
				// backward search - best when using large min_time
				else
				{
					for (std::vector<pair<float, unsigned short>>::reverse_iterator d_itr=ttime_sorter.rbegin(); d_itr!=ttime_sorter.rend(); ++d_itr)
					{
						/*zone_itf* dest_zone = (zone_itf*)d_itr->second;*/
						unsigned short dest_id = d_itr->second;
						typename zones_itf::iterator z_itr;
						if ((z_itr = zones_container->find(dest_id))==zones_container->end()){THROW_EXCEPTION("Error, destination zone id '"<<dest_id<<"' in travel time sorter not found in zones_container.")}
						zone_itf* dest_zone = (zone_itf*)(z_itr->second);

						float ttime=d_itr->first;

						if (ttime >= GLOBALS::Time_Converter.Convert_Value<TimeType,Time_Minutes>(min_time))
						{
							if (ttime < GLOBALS::Time_Converter.Convert_Value<TimeType,Time_Minutes>(max_time)) available_set.push_back(Get_Location_As_Needed<ReturnLocationType>(dest_zone));
						}
						else
						{
							return;
						}
					}
				}
			}

			template<typename ReturnLocationType> ReturnLocationType Get_Location_As_Needed(zone_itf* zone, requires(ReturnLocationType, check(strip_modifiers(ReturnLocationType), Zone_Components::Concepts::Is_Zone)))
			{
				return zone;
			}
			template<typename ReturnLocationType> ReturnLocationType Get_Location_As_Needed(zone_itf* zone, requires(ReturnLocationType, check(strip_modifiers(ReturnLocationType), Activity_Location_Components::Concepts::Is_Activity_Location)))
			{
				zone->template Get_Random_Location<ReturnLocationType>();
				return ;
			}
		};



		//======================================================================================
		/// Basic skimming implementation
		//--------------------------------------------------------------------------------------
		implementation struct Basic_Network_Skimming_Implementation : public Polaris_Component<MasterType,INHERIT(Basic_Network_Skimming_Implementation),Execution_Object>
		{
			// Tag as implementation
			typedef typename Polaris_Component<MasterType,INHERIT(Basic_Network_Skimming_Implementation),Execution_Object>::Component_Type ComponentType;
			typedef Prototypes::Network_Skimming<ComponentType> skimmer_itf;

			typedef Time_Minutes Stored_Time_Type;
			typedef Basic_Units::Currency_Variables::Cents Stored_Currency_Type;

			// reference to the transportation network
			m_prototype(Network_Components::Prototypes::Network,typename MasterType::network_type, network_reference, NONE, NONE);

			// time increment at which skim tables are updated - set in the initializer
			template <typename TargetType> TargetType update_increment()
			{
				return update_increment<TargetType>(_current_increment_index);
			}
			template <typename TargetType> TargetType update_increment(int interval)
			{
				return GLOBALS::Time_Converter.Convert_Value<Time_Minutes,TargetType>(_update_interval_endpoints[interval]);
			}

			m_container(std::vector<Time_Minutes>,update_interval_endpoints,NONE,NONE);
			m_data(int, current_increment_index,NONE,NONE);

			// time at which skim tables are next updated - set in the initializer and updated every time update is called
			m_data(Basic_Units::Implementations::Time_Implementation<MasterType>,_scheduled_update_time, NONE, NONE);
			member_component_feature(scheduled_update_time,_scheduled_update_time,Value,Basic_Units::Prototypes::Time);
			m_data(long, nodes_per_zone, NONE, NONE);

			// routers to do the skimming
			m_container(concat(dense_hash_map<long,Routing_Components::Prototypes::Routing<typename MasterType::skim_routing_type>*>), path_trees_container, NONE, NONE);
			
			// link-to-zone mapping for use in skimming
			m_container(std::vector<Activity_Location<typename MasterType::activity_location_type>*>,origin_locations, NONE, NONE);
			m_container(std::vector<Activity_Location<typename MasterType::activity_location_type>*>,destination_locations, NONE, NONE);
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

			m_container(std::vector<Prototypes::Skim_Table<Skim_Table_Implementation<MasterType>>*>, skims_by_time_container, NONE, NONE);
			m_container(std::list<int>, available_modes_container, NONE, NONE);


			// set the network and skimmer references
			typedef Network_Components::Prototypes::Network<type_of(network_reference)> network_itf;
			
			typedef Pair_Associative_Container<typename network_itf::get_type_of(zones_container)> zones_itf;
			typedef Zone_Components::Prototypes::Zone<get_mapped_component_type(zones_itf)> zone_itf;

			typedef Activity_Location_Components::Prototypes::Activity_Location<typename remove_pointer<typename zone_itf::get_type_of(origin_activity_locations)::value_type>::type> location_itf;
			typedef Random_Access_Sequence<typename zone_itf::get_type_of(origin_activity_locations),location_itf*> locations_itf;

			typedef Link_Components::Prototypes::Link<typename remove_pointer<typename location_itf::get_type_of(origin_links)::value_type>::type> link_itf;
			typedef Random_Access_Sequence<typename location_itf::get_type_of(origin_links),link_itf*> links_itf;

			//typedef Activity_Location_Components::Prototypes::Activity_Location<typename remove_pointer<typename type_of(origin_locations)::value_type>::type> origin_location_itf;
			//typedef  Random_Access_Sequence<typename type_of(origin_locations),origin_location_itf*> origin_locations_itf;
			typedef Prototype_Random_Access_Sequence<type_of(origin_locations),Activity_Location_Components::Prototypes::Activity_Location> origin_locations_itf;
			typedef strip_modifiers(typename origin_locations_itf::value_type) origin_location_itf;

			//typedef Prototypes::Skim_Table<typename remove_pointer<typename type_of(skims_by_time_container)::value_type>::type> skim_table_itf;
			typedef Prototype_Random_Access_Sequence<type_of(skims_by_time_container),Prototypes::Skim_Table> skim_tables_itf;
			typedef strip_modifiers(typename skim_tables_itf::value_type) skim_table_itf;

			typedef Prototypes::LOS<typename remove_pointer<typename skim_table_itf::get_type_of(skim_table)::value_type>::type> los_value_itf;
			typedef  Multidimensional_Random_Access_Array<typename skim_table_itf::get_type_of(skim_table),los_value_itf*> skim_matrix_itf;

			typedef Prototypes::LOS<typename los_value_itf::get_type_of(LOS_time_invariant)> los_invariant_value_itf;
			//typedef (modes_skim_container_itf,mode_skim_itf,typename skimmer_itf::get_type_of(mode_skim_table_container), Pair_Associative_Container,Prototypes::Mode_Skim_Table);
				

			template<typename TargetType> void Initialize()
			{
				// set the versioning info
				this->_highway_input_file.Version('1');
				this->_transit_input_file.Version('1');

				// get scenario interface
				typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;
				_Scenario_Interface* scenario = (_Scenario_Interface*)_global_scenario;

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

				// set up the update intervals
				this->_current_increment_index=0;
				if (scenario->template use_skim_intervals_from_previous<bool>())
				{

				}
				else if (scenario->template use_skim_intervals<bool>())
				{
					std::vector<float>* intervals = scenario->template skim_interval_endpoint_minutes<std::vector<float>*>();
					for (std::vector<float>::iterator itr = intervals->begin(); itr != intervals->end(); ++itr)
					{
						this->_update_interval_endpoints.push_back(*itr);
					}
					if (this->_update_interval_endpoints.back() != Round<int,float>(GLOBALS::Time_Converter.Convert_Value<Simulation_Timestep_Increment, Time_Minutes>(END - 1)))
					{
						this->_update_interval_endpoints.push_back(Round<int, float>(GLOBALS::Time_Converter.Convert_Value<Simulation_Timestep_Increment, Time_Minutes>(END - 1)));
					}
				}
				else
				{
					// Get interval length from the scenario
					Simulation_Timestep_Increment increment = Round<int, float>(GLOBALS::Time_Converter.Convert_Value<Time_Minutes, Simulation_Timestep_Increment>((Time_Minutes)scenario->template skim_interval_length_minutes<int>()));

					// Add endpoints to the endpoint list based on interval length
					for (Simulation_Timestep_Increment start = 0; start + increment < END - 1; start = start + increment)
					{
						this->_update_interval_endpoints.push_back(Round<int, float>(GLOBALS::Time_Converter.Convert_Value<Simulation_Timestep_Increment, Time_Minutes>(start + increment)));
					}
					// Add the END endpoint if not added above
					if (this->_update_interval_endpoints.back() != Round<int, float>(GLOBALS::Time_Converter.Convert_Value<Simulation_Timestep_Increment, Time_Minutes>(END - 1)))
					{
						this->_update_interval_endpoints.push_back(Round<int, float>(GLOBALS::Time_Converter.Convert_Value<Simulation_Timestep_Increment, Time_Minutes>(END - 1)));
					}
				}

				pthis->template scheduled_update_time<Simulation_Timestep_Increment>(0.0);
				pthis->template nodes_per_zone<long>(1);

				// add the available modes for the current model
				this->_available_modes_container.push_back(Vehicle_Components::Types::Vehicle_Type_Keys::SOV);
				//this->_available_modes_container.push_back(Vehicle_Components::Types::Vehicle_Type_Keys::BUS);
		

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
				typedef Pair_Associative_Container< type_of(path_trees_container)> tree_builder_list_itf;
				typedef Routing_Components::Prototypes::Routing<get_mapped_component_type(tree_builder_list_itf)> tree_builder_itf;
				tree_builder_list_itf* tree_list = this->path_trees_container<tree_builder_list_itf*>();

				origin_locations_itf* origin_locations = skim->template origin_locations<origin_locations_itf*>();	
				zones_itf* zones_container = network->template zones_container<zones_itf*>();
				typename origin_locations_itf::iterator orig_itr = origin_locations->begin();
				locations_itf* activity_locations = network->template activity_locations_container<locations_itf*>();
				
				// create a tree builder for each origin to skim
				long orig_index = 0;
				for (;orig_itr != origin_locations->end(); ++orig_itr, ++orig_index)
				{
					// get the origin/destination indices
					origin_location_itf* orig_node = (origin_location_itf*)*orig_itr;

					// Allocate a tree_builder for each origin node		
					tree_builder_itf* tree_builder = (tree_builder_itf*)Allocate<typename tree_builder_itf::Component_Type>();
					tree_builder->template parent_skimmer<skimmer_itf*>(skim);

					// Set the current routable origin for the tree builder
					tree_builder->template origin_link<link_itf*>((link_itf*)*(orig_node->template origin_links<links_itf*>()->begin()));
					tree_builder->template network<network_itf*>(network);

					// Set the start, end and update times for the router
					//tree_builder->template update_increment<Simulation_Timestep_Increment>(skim->template update_increment<Simulation_Timestep_Increment>());
					tree_builder->template start_time<Simulation_Timestep_Increment>(0);//this->start_time<Simulation_Timestep_Increment>());
					tree_builder->template end_time<Simulation_Timestep_Increment>(END);//this->end_time<Simulation_Timestep_Increment>());
			
					// Add the tree_builder to the std::list for future processing
					tree_builder->Schedule_Route_Computation(iteration());

					pair<long,tree_builder_itf*> item = pair<long,tree_builder_itf*>(orig_index,tree_builder);
					tree_list->insert(item);
				}

				Simulation_Timestep_Increment start, end;
				int num_modes;
				int num_zones;

				
				//===========================================================================
				// FILE INPUT IF REQUESTED - set up binary file readers for skim files
				File_IO::Binary_File_Reader& infile = skim->template highway_input_file<File_IO::Binary_File_Reader&>();
				File_IO::Binary_File_Reader& transit_infile = skim->template transit_input_file<File_IO::Binary_File_Reader&>();
				File_IO::Binary_File_Reader& highway_cost_infile = skim->template highway_cost_input_file<File_IO::Binary_File_Reader&>();
				if (skim->template read_input<bool>()) Read_Binary_Headers(num_modes,num_zones);
				else num_zones = (int)zones_container->size();


				//===========================================================================
				// create the time-invariant skim tables
				float* auto_tolls = new float[num_zones*num_zones]();
				float* auto_parking_cost = new float[num_zones*num_zones]();
				float* transit_ttime = new float[num_zones*num_zones]();
				float* transit_walk_access_time = new float[num_zones*num_zones]();
				float* auto_distance /*transit_sov_access_time */= new float[num_zones*num_zones]();
				float* transit_wait_time = new float[num_zones*num_zones]();
				float* transit_fare = new float[num_zones*num_zones]();

				//-------------------------------------------------
				//TODO: remove when done testing:
				cout <<"Done initializing invariant matrices..."<<endl;
				//cin >> test;
				//cout <<test;
				//-------------------------------------------------

				// Read transit input data if required
				if (skim->template read_transit<bool>())
				{	
					this->template Read_Binary_Data<float*>(transit_ttime, transit_infile, num_zones);
					this->template Read_Binary_Data<float*>(transit_walk_access_time, transit_infile, num_zones);
					this->template Read_Binary_Data<float*>(auto_distance, transit_infile, num_zones);
					this->template Read_Binary_Data<float*>(transit_wait_time, transit_infile, num_zones);
					this->template Read_Binary_Data<float*>(transit_fare, transit_infile, num_zones);

					//transit_infile.template Read_Array<float>(transit_ttime, num_zones*num_zones);
					//transit_infile.template Read_Array<float>(transit_walk_access_time, num_zones*num_zones);
					//transit_infile.template Read_Array<float>(auto_distance /*transit_sov_access_time*/, num_zones*num_zones);
					//transit_infile.template Read_Array<float>(transit_wait_time, num_zones*num_zones);
					//transit_infile.template Read_Array<float>(transit_fare, num_zones*num_zones);

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
				if (skim->template read_highway_cost<bool>() && skim->template read_input<bool>())
				{
					highway_cost_infile.template Read_Array<float>(auto_tolls, num_zones*num_zones);
					highway_cost_infile.template Read_Array<float>(auto_parking_cost, num_zones*num_zones);
				}
				else
				{
					for (int i =0; i < num_zones*num_zones; ++i)
					{
						auto_tolls[i] = 0.0;
						auto_parking_cost[i] = 0.0;
					}
				}

				// Store in the invariant los array
				typename MasterType::los_invariant_value_type** temp_invariant_los_array = new typename MasterType::los_invariant_value_type*[num_zones*num_zones];
				for (int i =0; i < num_zones*num_zones; ++i)
				{
					typename MasterType::los_invariant_value_type* temp_los = Allocate<typename MasterType::los_invariant_value_type>();
					((los_invariant_value_itf*)temp_los)->template auto_tolls<Stored_Currency_Type>(auto_tolls[i]);
					((los_invariant_value_itf*)temp_los)->template auto_parking_cost<Stored_Currency_Type>(auto_parking_cost[i]);
					((los_invariant_value_itf*)temp_los)->template transit_ttime<Stored_Time_Type>(transit_ttime[i]);
					((los_invariant_value_itf*)temp_los)->template transit_walk_access_time<Stored_Time_Type>(transit_walk_access_time[i]);
					((los_invariant_value_itf*)temp_los)->template auto_distance /*transit_sov_access_time<Stored_Time_Type>*/<Kilometers>(auto_distance/*transit_sov_access_time*/[i]);
					((los_invariant_value_itf*)temp_los)->template transit_wait_time<Stored_Time_Type>(transit_wait_time[i]);
					((los_invariant_value_itf*)temp_los)->template transit_fare<Stored_Currency_Type>(transit_fare[i]);
					temp_invariant_los_array[i] = temp_los;
				}

				// Delete the temp tables to free memory
				delete[] auto_tolls;
				delete[] auto_parking_cost;
				delete[] transit_ttime;
				delete[] transit_walk_access_time;
				delete[] auto_distance /*transit_sov_access_time*/;
				delete[] transit_wait_time;
				delete[] transit_fare;
				

				

				//===========================================================================
				// create the skim_table time periods, for basic create only a single time period skim_table
				//for (start = 0; start < GLOBALS::Time_Converter.template Convert_Value<Time_Hours,Simulation_Timestep_Increment>(24.0); start = start + skim->template update_increment<Simulation_Timestep_Increment>())
				for (int i=0; i<this->_update_interval_endpoints.size(); ++i)
				{		
					if (i>0) start = this->update_increment<Simulation_Timestep_Increment>(i-1);
					else start = 0;
					end = this->update_increment<Simulation_Timestep_Increment>(i);

					skim_table_itf* skim_table = (skim_table_itf*)Allocate<strip_modifiers(typename type_of(skims_by_time_container)::value_type)::Component_Type>();
					skim_table->template network_reference<network_itf*>(network);
					skim_table->template skim_reference<skimmer_itf*>(skim);
					skim_table->template start_time<Simulation_Timestep_Increment>(start);
					skim_table->template end_time<Simulation_Timestep_Increment>(end);

					if (skim->template read_input<bool>())
					{
						// Get raw auto travel time data from binary file
						float* data = new float[num_zones*num_zones];
						this->template Read_Binary_Data<float*>(data,infile,num_zones);
						//infile.template Read_Array<float>(data, num_zones*num_zones);

						// create LOS matrix using raw input data
						typename MasterType::los_value_type** temp_los_array = new typename MasterType::los_value_type*[num_zones*num_zones];
						for (int i =0; i < num_zones*num_zones; ++i)
						{
							typename MasterType::los_value_type* temp_los = Allocate<typename MasterType::los_value_type>();
							// get, validate and store the auto ttime.  make sure not greater than max expected
							Stored_Time_Type a_ttime = (Stored_Time_Type)data[i];
							if (a_ttime > GLOBALS::Time_Converter.Convert_Value<Time_Hours, Stored_Time_Type>(100.0) || a_ttime < 0 || ISNAN(a_ttime))
							{
								THROW_WARNING("Warning, invalid travel auto travel time value="<<a_ttime<<" found when reading skim file, travel time reset to default value=180.0.");
								a_ttime = GLOBALS::Time_Converter.Convert_Value<Time_Minutes, Stored_Time_Type>(180.0);
							}
							((los_value_itf*)temp_los)->template auto_ttime<Stored_Time_Type>(a_ttime);
							// store the other time invariant los characteristics for the current los record
							((los_value_itf*)temp_los)->template LOS_time_invariant<typename MasterType::los_invariant_value_type*>(temp_invariant_los_array[i]);
							temp_los_array[i] = temp_los;
						}
						skim_table->template Initialize<typename MasterType::los_value_type**>(temp_los_array);
						delete[] data;
					}
					else
					{
						// create LOS matrix using default values
						//-------------------------------------------------
						//TODO: remove when done testing:
						cout <<"Preallocation for start = " <<start <<" ...";
						/*cin >> test;
						cout <<test;*/
						//-------------------------------------------------
						typename MasterType::los_value_type** temp_los_array = new typename MasterType::los_value_type*[num_zones*num_zones];
						//typename MasterType::los_value_type* temp_los_array = new typename MasterType::los_value_type[num_zones*num_zones];
						//-------------------------------------------------
						//TODO: remove when done testing:
						cout <<" array allocated."<<endl;
						//cin >> test;
						//cout <<test;
						//-------------------------------------------------
						for (int i =0; i < num_zones*num_zones; ++i)
						{
							typename MasterType::los_value_type* temp_los = Allocate<typename MasterType::los_value_type>();
							((los_value_itf*)temp_los)->template auto_ttime<Time_Hours>(temp_invariant_los_array[i]->auto_distance<Miles>()/30.0f);
							((los_value_itf*)temp_los)->template LOS_time_invariant<typename MasterType::los_invariant_value_type*>(temp_invariant_los_array[i]);
							temp_los_array[i] = temp_los;
						}
						//-------------------------------------------------
						//TODO: remove when done testing:
						cout <<" Record memory allocated."<<endl;
						//cin >> test;
						//cout <<test;
						//-------------------------------------------------
						skim_table->template Initialize<typename MasterType::los_value_type**>(temp_los_array);
						//-------------------------------------------------
						//TODO: remove when done testing:
						cout <<" Skim table copy complete."<<endl;
						//cin >> test;
						//cout <<test;
						//-------------------------------------------------
					}

					// add time period skim tables to the container
					skim_tables_itf* skim_tables = this->template skims_by_time_container<skim_tables_itf*>();
					skim_tables->push_back(skim_table);
				}


				//===========================================================================
				// make sure intra-zonal information is initialized
				for (auto zone_itr = zones_container->begin(); zone_itr != zones_container->end(); ++zone_itr)
				{
					zone_itf* zone = (zone_itf*)(zone_itr->second);
					los_value_itf* los = skim->template Get_LOS<zone_itf*,los_value_itf*>(zone, zone);

					if (los->template auto_distance<Miles>() > 1000)
					{
						Miles new_distance = sqrt(zone->template area<Square_Miles>())/2.0*1.2;
						los->template auto_distance<Miles>(new_distance);
					}
				}

				//===========================================================================
				// Initial FILE_OUTPUT IF REQUESTED
				if (skim->template write_output<bool>() == true)
				{
					Write_Binary_Header();	
				}

				//===========================================================================
				// Calculate accessibilities for each zone based on initial skims - move until after the initial update is done.
				//Update_Zone_Accessibilities<TargetType>();
			
			}

			template<typename TargetType> void Update_Zone_Accessibilities()
			{
				network_itf* network = this->template network_reference<network_itf*>();
				zones_itf* zones_container = network->template zones_container<zones_itf*>();
				typedef typename zones_itf::iterator zone_iterator;
				skimmer_itf* skim = (skimmer_itf*)this;

				float Nz_inv = 1.0 / (zones_container->size() - 1.0);
				float alpha = -0.15;
				float min_transit_ovtt = FLT_MAX;

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

					float auto_ttime_ampeak_max_avg = 0, auto_ttime_ampeak_min_avg = 0, auto_ttime_pmpeak_max_avg = 0, auto_ttime_pmpeak_min_avg = 0, auto_ttime_offpeak_max_avg = 0, auto_ttime_offpeak_min_avg = 0, auto_ttime_night_max_avg = 0, auto_ttime_night_min_avg = 0, distance_avg = 0;
					float tran_ttime_avg = 0, tran_ovtt_avg = 0, tran_wait_avg = 0, tran_fare_avg = 0;
					float auto_ttime_avg_comp = 0, auto_ttime_offpeak_avg_comp = 0;
					int count=0, count_transit=0;
					float attract_count = 0, attract_count_transit=0;
								
					// loop through all destination zones
					for (zone_iterator d_zone_itr = zones_container->begin(); d_zone_itr != zones_container->end(); ++d_zone_itr)
					{
						// interface to destination zone
						zone_itf* dzone = (zone_itf*)(d_zone_itr->second);

						// do not include the origin zone in calculations
						if (zone == dzone) continue;

						// get TTime by automobile and transit through skim interface, in minutes
						Time_Minutes ttime_auto_ampeak_max = skim->template Get_TTime<zone_itf*,Vehicle_Components::Types::Vehicle_Type_Keys, Time_Hours,Time_Minutes>(zone,dzone,Vehicle_Components::Types::Vehicle_Type_Keys::SOV, 7.5);
						Time_Minutes ttime_auto_ampeak_min = skim->template Get_TTime<zone_itf*, Vehicle_Components::Types::Vehicle_Type_Keys, Time_Hours, Time_Minutes>(zone, dzone, Vehicle_Components::Types::Vehicle_Type_Keys::SOV, 6.0);
						Time_Minutes ttime_auto_offpeak_max = skim->template Get_TTime<zone_itf*,Vehicle_Components::Types::Vehicle_Type_Keys, Time_Hours,Time_Minutes>(zone,dzone,Vehicle_Components::Types::Vehicle_Type_Keys::SOV, 14.0);
						Time_Minutes ttime_auto_offpeak_min = skim->template Get_TTime<zone_itf*, Vehicle_Components::Types::Vehicle_Type_Keys, Time_Hours, Time_Minutes>(zone, dzone, Vehicle_Components::Types::Vehicle_Type_Keys::SOV, 12.0);
						Time_Minutes ttime_auto_pmpeak_max = skim->template Get_TTime<zone_itf*, Vehicle_Components::Types::Vehicle_Type_Keys, Time_Hours, Time_Minutes>(zone, dzone, Vehicle_Components::Types::Vehicle_Type_Keys::SOV, 17.5);
						Time_Minutes ttime_auto_pmpeak_min = skim->template Get_TTime<zone_itf*, Vehicle_Components::Types::Vehicle_Type_Keys, Time_Hours, Time_Minutes>(zone, dzone, Vehicle_Components::Types::Vehicle_Type_Keys::SOV, 19.0);
						Time_Minutes ttime_auto_night_max = skim->template Get_TTime<zone_itf*, Vehicle_Components::Types::Vehicle_Type_Keys, Time_Hours, Time_Minutes>(zone, dzone, Vehicle_Components::Types::Vehicle_Type_Keys::SOV, 20.0);
						Time_Minutes ttime_auto_night_min = skim->template Get_TTime<zone_itf*, Vehicle_Components::Types::Vehicle_Type_Keys, Time_Hours, Time_Minutes>(zone, dzone, Vehicle_Components::Types::Vehicle_Type_Keys::SOV, 1.0);

						Time_Minutes ttime_transit = skim->template Get_TTime<zone_itf*,Vehicle_Components::Types::Vehicle_Type_Keys,Time_Minutes>(zone,dzone,Vehicle_Components::Types::Vehicle_Type_Keys::BUS);
						los_value_itf* los_value = skim->Get_LOS<zone_itf*, Time_Minutes, los_value_itf*>(zone, dzone, 9.0);
						Time_Minutes ovtt_transit = los_value->transit_walk_access_time<Time_Minutes>();
						Time_Minutes wait_transit = los_value->transit_wait_time<Time_Minutes>();
						Dollars fare_transit = los_value->transit_fare<Dollars>();
						Miles distance = los_value->auto_distance<Miles>();

						// update the accessibilty factors: (1/Nz-1) * (sum(Emp * exp(alpha*ttime)))
						C_gov += Nz_inv * dzone->template employment_government<float>() * exp(alpha*ttime_auto_ampeak_max);
						C_man += Nz_inv * dzone->template employment_manufacturing<float>() * exp(alpha*ttime_auto_ampeak_max);
						C_ret += Nz_inv * dzone->template employment_retail<float>() * exp(alpha*ttime_auto_ampeak_max);
						C_ser += Nz_inv * dzone->template employment_services<float>() * exp(alpha*ttime_auto_ampeak_max);
						C_ind += Nz_inv * dzone->template employment_industrial<float>() * exp(alpha*ttime_auto_ampeak_max);
						C_oth += Nz_inv * dzone->template employment_other<float>() * exp(alpha*ttime_auto_ampeak_max);

						float attract = dzone->template employment_total<float>() + dzone->template pop_persons<float>();

						// update the average travel time from 'zone' for transit and auto if transit is available
						if (ttime_transit < 1440 && ttime_transit > 0) // exponential decay in weighting of transit wait, ovtt, fare with transit travel time
						{			
							tran_ttime_avg += attract * exp(alpha * ttime_transit);
							tran_fare_avg += attract * exp(alpha * ttime_transit) * fare_transit;
							tran_ovtt_avg += attract * exp(alpha * ttime_transit) * ovtt_transit;
							tran_wait_avg += attract * exp(alpha * ttime_transit) * wait_transit;
							count_transit++;
							attract_count_transit += attract;
						}
						// updata average auto travel times if auto is available
						if (ttime_auto_ampeak_max < 1440 && ttime_auto_ampeak_max > 0)
						{
							auto_ttime_ampeak_max_avg += attract * exp(alpha * ttime_auto_ampeak_max);
							auto_ttime_pmpeak_max_avg += attract * exp(alpha * ttime_auto_pmpeak_max);
							auto_ttime_offpeak_max_avg += attract * exp(alpha * ttime_auto_offpeak_max);
							auto_ttime_night_max_avg += attract * exp(alpha * ttime_auto_night_max);
							auto_ttime_ampeak_min_avg += attract * exp(alpha * ttime_auto_ampeak_min);
							auto_ttime_pmpeak_min_avg += attract * exp(alpha * ttime_auto_pmpeak_min);
							auto_ttime_offpeak_min_avg += attract * exp(alpha * ttime_auto_offpeak_min);
							auto_ttime_night_min_avg += attract * exp(alpha * ttime_auto_night_min);
							distance_avg += attract * exp(alpha * ttime_auto_ampeak_max) * distance;
							count++;
							attract_count += attract;
						}
						if (ovtt_transit < min_transit_ovtt) min_transit_ovtt = ovtt_transit;
					}

					// Add accessiblity factors to origin zone
					zone->accessibility_employment_government(C_gov);
					zone->accessibility_employment_industrial(C_ind);
					zone->accessibility_employment_manufacturing(C_man);
					zone->accessibility_employment_other(C_oth);
					zone->accessibility_employment_retail(C_ret);
					zone->accessibility_employment_services(C_ser);

					// update the average travel times to all other zones for 'zone' by mode - weighted by attractiveness of zone (i.e. how well served are the attractive zones)
					zone->template avg_ttime_auto_ampeak<Time_Minutes>(0.5 / alpha * log(auto_ttime_ampeak_max_avg / attract_count) + 0.5 / alpha * log(auto_ttime_ampeak_min_avg / attract_count));
					zone->template avg_ttime_auto_pmpeak<Time_Minutes>(0.5 / alpha * log(auto_ttime_pmpeak_max_avg / attract_count) + 0.5 / alpha * log(auto_ttime_pmpeak_min_avg / attract_count));
					zone->template avg_ttime_auto_offpeak<Time_Minutes>(0.5 / alpha * log(auto_ttime_offpeak_max_avg / attract_count) + 0.5 / alpha * log(auto_ttime_offpeak_min_avg / attract_count));
					zone->template avg_ttime_auto_night<Time_Minutes>(0.5 / alpha * log(auto_ttime_night_max_avg / attract_count) + 0.5 / alpha * log(auto_ttime_night_min_avg / attract_count));

					zone->template avg_ttime_var_auto_ampeak<float>(abs(0.5 / alpha * log(auto_ttime_ampeak_max_avg / attract_count) - 0.5 / alpha * log(auto_ttime_ampeak_min_avg / attract_count)) / zone->template avg_ttime_auto_ampeak<Time_Minutes>());
					zone->template avg_ttime_var_auto_pmpeak<float>(abs(0.5 / alpha * log(auto_ttime_pmpeak_max_avg / attract_count) - 0.5 / alpha * log(auto_ttime_pmpeak_min_avg / attract_count)) / zone->template avg_ttime_auto_pmpeak<Time_Minutes>());
					zone->template avg_ttime_var_auto_offpeak<float>(abs(0.5 / alpha * log(auto_ttime_offpeak_max_avg / attract_count) - 0.5 / alpha * log(auto_ttime_offpeak_min_avg / attract_count)) / zone->template avg_ttime_auto_offpeak<Time_Minutes>());
					zone->template avg_ttime_var_auto_night<float>(abs(0.5 / alpha * log(auto_ttime_night_max_avg / attract_count) - 0.5 / alpha * log(auto_ttime_night_min_avg / attract_count)) / zone->template avg_ttime_auto_night<Time_Minutes>());
					
					zone->template avg_distance<Miles>(distance_avg / auto_ttime_ampeak_max_avg);
					zone->template min_ovtt_transit<Time_Minutes>(min_transit_ovtt);
					if (count_transit > 2)
					{
						zone->template avg_ttime_transit<Time_Minutes>(1.0 / alpha * log(tran_ttime_avg / attract_count_transit));
						zone->template avg_ovtt_transit<Time_Minutes>(tran_ovtt_avg / tran_ttime_avg);
						zone->template avg_fare_transit<Dollars>(tran_fare_avg / tran_ttime_avg);
						zone->template avg_wait_transit<Time_Minutes>(tran_wait_avg / tran_ttime_avg);
					}
					else
					{
						zone->template avg_ttime_transit<Time_Minutes>(9999);
						zone->template avg_ovtt_transit<Time_Minutes>(9999);
						zone->template avg_fare_transit<Time_Minutes>(9999);
						zone->template avg_wait_transit<Time_Minutes>(9999);
					}
					//if (count_comp > 10) zone->template avg_ttime_auto_to_transit_accessible_zones<Time_Minutes>((0.5*auto_ttime_avg_comp + 0.5*auto_ttime_offpeak_avg_comp)/attract_count_comp);
					//else zone->template avg_ttime_auto_to_transit_accessible_zones<Time_Minutes>(9999);

					//cout << zone->uuid<int>() << ","
					//	<< zone->template avg_ttime_auto_peak<Time_Minutes>() << ","
					//	<< zone->template avg_distance<Miles>() << ","
					//	<< zone->template avg_ttime_transit<Time_Minutes>() << ","
					//	<< zone->template avg_ovtt_transit<Time_Minutes>() << ","
					//	<< zone->template avg_wait_transit<Time_Minutes>() << ","
					//	<< zone->template avg_fare_transit<Dollars>() << endl;

				}
				
			}

			void Write_Binary_Header()
			{
				network_itf* network = this->template network_reference<network_itf*>();
				skimmer_itf* skim = (skimmer_itf*)this;
				zones_itf* zones_container = network->template zones_container<zones_itf*>();

				File_IO::Binary_File_Writer& outfile = skim->template highway_output_file<File_IO::Binary_File_Writer&>();
				char version[] = "SKIM:V01";
				outfile.template Write_Array<char>(&version[0],8);
				char begin_zones[] = "BZON";
				outfile.template Write_Array<char>(&begin_zones[0],4);
				int zones = (int)zones_container->size();
				outfile.template Write_Value<int>(zones);
				for (auto itr = zones_container->begin(); itr != zones_container->end(); ++itr)
				{
					zone_itf* zone = (zone_itf*)(itr->second);
					outfile.template Write_Value<int>(zone->template uuid<int&>());
					outfile.template Write_Value<int>(zone->template internal_id<int&>());
				}
				char end_zones[] = "EZON";
				outfile.template Write_Array<char>(&end_zones[0],4);
				std::vector<Time_Minutes>* intervals = skim->template update_interval_endpoints<std::vector<Time_Minutes>*>();
				char begin_intervals[] = "BINT";
				outfile.template Write_Array<char>(&begin_intervals[0],4);
				int num_increment = intervals->size();
				outfile.template Write_Value<int>(num_increment);
				for (int i=0; i<num_increment; ++i)
				{
					int incr = (*intervals)[i];
					outfile.template Write_Value<int>(incr);
				}
				char end_intervals[] = "EINT";
				outfile.template Write_Array<char>(&end_intervals[0],4);
			}

			void Read_Binary_Headers(int& num_modes, int& num_zones, update_interval_endpoints_type* return_intervals=nullptr, bool perform_checks=true)
			{
				// read for time-varying highway skims
				File_IO::Binary_File_Reader& infile = highway_input_file<File_IO::Binary_File_Reader&>();

				// get versioning info
				char version[9];
				infile.template Read_Array<char>(version, 8);
				version[8]='\0';


				// call associated read function
				if (strcmp(version,"SKIM:V01")==0)
				{
					this->_highway_input_file.Version('1');
					Read_Binary_Headers_V1(num_modes,num_zones,return_intervals,perform_checks);
				}
				else
				{
					this->_highway_input_file.Version('0');
					this->_transit_input_file.Version('0');
					Read_Binary_Headers_Unversioned(num_modes,num_zones,return_intervals,perform_checks);
				}
			}
	
			void Read_Binary_Headers_Unversioned(int& num_modes, int& num_zones, update_interval_endpoints_type* return_intervals=nullptr,bool perform_checks=true)
			{

				int update_increment;//, num_zones_transit, num_zones_hcost;

				// read for time-varying highway skims
				//File_IO::Binary_File_Reader& infile = skim->template highway_input_file<File_IO::Binary_File_Reader&>();
				_highway_input_file.Begin();

				// get versioning info
				_highway_input_file.template Read_Value<int>(num_modes);
				_highway_input_file.template Read_Value<int>(num_zones);
				_highway_input_file.template Read_Value<int>(update_increment);

				//if (perform_checks) Do_Header_Validation_Checks(num_modes, nullptr, nullptr, nullptr, update_increment);
				network_itf* network = this->template network_reference<network_itf*>();
				skimmer_itf* skim = (skimmer_itf*)this;
				zones_itf* zones_container = network->template zones_container<zones_itf*>();
				int skimmer_num_zones = zones_container->size();
				if (num_zones != skimmer_num_zones) THROW_EXCEPTION("ERROR: Input skim file number of zones does not match the number of zones specified in the input database.");
				Simulation_Timestep_Increment skimmer_update_increment = skim->template update_increment<Simulation_Timestep_Increment>();
				if (update_increment != skimmer_update_increment) THROW_EXCEPTION("ERROR: Input skim file update increment does not match the update increment specified in Skim_Implementation.");
			}

			void Read_Binary_Headers_V1(int& num_modes, int& num_zones, update_interval_endpoints_type* return_intervals=nullptr, bool perform_checks=true)
			{
				// get scenario interface
				typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;
				_Scenario_Interface* scenario = (_Scenario_Interface*)_global_scenario;

				//=======================================
				// READ HIGHWAY HEADER
				//---------------------------------------
				int num_intervals;
				//, num_zones_hcost;

				update_interval_endpoints_type intervals;
				std::vector<pair<int,int>> return_zones_container;

				char tag[5]; tag[4]='\0';

				// check if zone tag exists
				_highway_input_file.template Read_Array<char>(tag,4);
				if (strcmp(tag,"BZON")==0)
				{
					_highway_input_file.template Read_Value<int>(num_zones);
					for (int i=0; i<num_zones; i++)
					{
						int id, index;
						_highway_input_file.template Read_Value<int>(id);
						_highway_input_file.template Read_Value<int>(index);
						return_zones_container.push_back(pair<int,int>(id,index));
					}
					_highway_input_file.template Read_Array<char>(tag,4);
					if (strcmp(tag,"EZON")!=0) THROW_EXCEPTION("ERROR: End zone information tag 'EZON' missing from binary file, possible versioning issue.");
				}
				else
				{
					// header info
					_highway_input_file.template Read_Value<int>(num_modes,-4,ios_base::cur);
					_highway_input_file.template Read_Value<int>(num_zones);
				}

				// header validation checks - read each interval endpoint entry and verify that the are the same as specified in scenario
				_highway_input_file.template Read_Array<char>(tag,4);
				if (strcmp(tag,"BINT")!=0) THROW_EXCEPTION("ERROR: Begin intervals tag 'BINT' missing from binary file, possible versioning issue.");

				_highway_input_file.template Read_Value<int>(num_intervals);

				for (int i=0; i<num_intervals; i++)
				{
					int endpoint;
					_highway_input_file.template Read_Value<int>(endpoint);
					intervals.push_back(endpoint);
					
					if (scenario->use_skim_intervals_from_previous<bool>()) _update_interval_endpoints.push_back(endpoint);
				}
				_highway_input_file.template Read_Array<char>(tag,4);
				if (strcmp(tag,"EINT")!=0) THROW_EXCEPTION("ERROR: End intervals tag 'EINT' missing from binary file, possible versioning issue.");



				//==============================================
				// READ TRANSIT HEADER
				//----------------------------------------------
				std::vector<pair<int, int>> return_transit_zones_container;
				if (read_transit<bool>())
				{
					// get versioning info
					char version[9]; version[8] = '\0';
					_transit_input_file.template Read_Array<char>(version, 8);
					// call associated read function
					if (strcmp(version, "SKIM:V01") != 0)
					{
						THROW_EXCEPTION("ERROR: Transit skim binary does not seem to be in V01 format. Use skim_analyzer.pyw to verify.");
					}
					else this->_transit_input_file.Version('1');

					_transit_input_file.template Read_Array<char>(tag, 4);
					if (strcmp(tag, "BZON") == 0)
					{
						_transit_input_file.template Read_Value<int>(num_zones);
						for (int i = 0; i<num_zones; i++)
						{
							int id, index;
							_transit_input_file.template Read_Value<int>(id);
							_transit_input_file.template Read_Value<int>(index);
							return_transit_zones_container.push_back(pair<int, int>(id, index));
						}
						_transit_input_file.template Read_Array<char>(tag, 4);
						if (strcmp(tag, "EZON") != 0) THROW_EXCEPTION("ERROR: End zone information tag 'EZON' missing from binary file, possible versioning issue.");
					}
					else THROW_EXCEPTION("ERROR: Start zone information tag 'BZON' missing from binary trasnit skim file, possible versioning issue.");
				}


				//==============================================
				// VALIDATION
				//----------------------------------------------
				if (perform_checks) Do_Header_Validation_Checks(num_modes, &intervals, &return_zones_container, &return_transit_zones_container);

				// return the read in interval values if requested
				if (return_intervals != nullptr)
				{
					return_intervals->clear();
					for (int i=0; i<intervals.size(); i++) return_intervals->push_back(intervals[i]);
				}
			}

			void Do_Header_Validation_Checks(int num_modes, update_interval_endpoints_type* intervals, std::vector<pair<int,int>>* return_zones_container, std::vector<pair<int, int>>* return_transit_zones_container, int increment=0)
			{
				int num_zones = return_zones_container->size();
				network_itf* network = this->template network_reference<network_itf*>();
				skimmer_itf* skim = (skimmer_itf*)this;
				zones_itf* zones_container = network->template zones_container<zones_itf*>();

				// header validation checks - number of zones
				int skimmer_num_zones = zones_container->size();
				if (num_zones != skimmer_num_zones) THROW_EXCEPTION("ERROR: Input skim file number of zones does not match the number of zones specified in the input database.");

				// header validation checks - number of update increments or length of update increment
				if (intervals != nullptr)
				{
					if (intervals->size() != _update_interval_endpoints.size()) THROW_EXCEPTION("ERROR: Number of intervals in Input skim file does not match the intervals specified in scenario.");
					for (int i=0; i<intervals->size(); i++)
					{
						if ((*intervals)[i] != (int)_update_interval_endpoints[i]) THROW_EXCEPTION("ERROR: Input skim file update endpoint does not match the endpoint specified in scenario at position "<<i<<".");
					}
				}
				else
				{
					Simulation_Timestep_Increment skimmer_update_increment = skim->template update_increment<Simulation_Timestep_Increment>();
					if (increment != skimmer_update_increment) THROW_EXCEPTION("ERROR: Input skim file update increment does not match the update increment specified in Skim_Implementation.");
				}

				// zone id validation checks
				if (return_zones_container->size() != 0)
				{
					for (std::vector<pair<int,int>>::iterator itr = return_zones_container->begin(); itr != return_zones_container->end(); ++itr)
					{
						int id = (*itr).first;
						int index = (*itr).second;

						typename zones_itf::iterator zitr;
						if ((zitr = zones_container->find(id)) == zones_container->end()) THROW_EXCEPTION("ERROR: Zone id from skim file not found in network zones container.");
						if (zitr->second->template internal_id<int>() != index) THROW_EXCEPTION("ERROR: Zone id in skim file has different index than same zone id in network zones container.");
					}
				}

				// header validation checks for transit and cost skims - make sure number of zones specified correctly
				if (read_transit<bool>())
				{		
					int num_zones_transit = return_transit_zones_container->size();
					if (num_zones_transit != num_zones) THROW_EXCEPTION("ERROR: Input transit skim file number of zones does not match the number of zones in the highway skim file. Transit="<<num_zones_transit<<" and Highway="<<num_zones);
				}
				if (read_highway_cost<bool>())
				{		
					int num_zones_hcost;
					_highway_cost_input_file.template Read_Value<int>(num_zones_hcost);
					if (num_zones_hcost != num_zones) THROW_EXCEPTION("ERROR: Input highway cost skim file number of zones does not match the number of zones in the highway skim file. Cost="<<num_zones_hcost<<" and Highway="<<num_zones);
				}
			}

			template<typename TargetType> void Read_Binary_Data(TargetType data_ptr, File_IO::Binary_File_Reader& file, int num_zones, requires(TargetType,check(TargetType,is_pointer) && check(strip_modifiers(TargetType),is_arithmetic)))
			{
				char tag[5]; tag[4]='\0';

				// Read validation tag before and after data read
				if (file.Version() != '0')
				{
					file.template Read_Array<char>(tag,4);
					if (strcmp(tag,"BMAT")!=0) THROW_EXCEPTION("ERROR: Begin matrix tag 'BMAT' missing from binary file, possible versioning issue.");
				}

				// Main data read
				file.Read_Array<strip_modifiers(TargetType)>(data_ptr, num_zones*num_zones);

				// Read validation tag before and after data read
				if (file.Version() != '0')
				{
					file.template Read_Array<char>(tag,4);
					if (strcmp(tag,"EMAT")!=0) THROW_EXCEPTION("ERROR: End matrix tag 'EMAT' missing from binary file, possible versioning issue.");
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
