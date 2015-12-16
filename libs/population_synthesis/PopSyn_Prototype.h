#pragma once

#include "Population_Synthesis_Includes.h"
#include "activity_simulator\Household_Prototype.h"
#include "activity_simulator\Person_Prototype.h"
#include <iomanip>

namespace PopSyn
{
	namespace Prototypes
	{
		prototype struct Population_Synthesizer ADD_DEBUG_INFO
		{
			tag_as_prototype; // Declare class as a polaris prototype

			//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
			// Initializers, with and without setting a network reference.  If the version without networktype is used the events which require network information are skipped
			//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
			template<typename ScenarioType> void Initialize(ScenarioType scenario, requires(ScenarioType, check_stripped_type(ScenarioType, Concepts::Scenario_Has_Popsyn_Configuration_Data)))
			{
				// Add references to other objects to the population synthesizer
				typedef Scenario_Components::Prototypes::Scenario<typename get_type_of(scenario_reference)> scenario_itf;
				this->scenario_reference<ScenarioType>(scenario);
				this->network_reference<Null_Prototype<get_type_of(network_reference)>*>(nullptr); // no network object
				
				// Allocate IPF Solver and get Settings from scenario reference
				typedef PopSyn::Prototypes::Solver_Settings<typename get_type_of(Solution_Settings)> solver_itf;
				solver_itf* solver = (solver_itf*)Allocate<typename get_type_of(Solution_Settings)>();
				solver->template Initialize<ScenarioType>(scenario);
				this->Solution_Settings<solver_itf*>(solver);

				// create file linker and initialize with the filepath
				typedef PopSyn::Prototypes::Popsyn_File_Linker<typename get_type_of(file_linker)> linker_itf;
				linker_itf* linker = (linker_itf*)Allocate<typename get_type_of(file_linker)>();
				this->file_linker<linker_itf*>(linker);
				linker->Initialize(scenario->template popsyn_control_file_name<string>());

				// set up output files
				if (scenario->write_full_output<bool>())
				{
					stringstream pop_filename("");
					pop_filename << scenario->template output_dir_name<string>();
					pop_filename << "joint_distributions.csv";
					this->Output_Stream<ofstream&>().open(pop_filename.str(),ios_base::out);
					this->Output_Stream<ofstream&>() << "Target and Synthesized Joint distributions for HH and Person Level for each zone:"<<endl;
				}
								
				if (scenario->write_marginal_output<bool>())
				{
					stringstream marg_filename("");
					marg_filename << scenario->template output_dir_name<string>();
					marg_filename << "marginal_distributions.csv";
					this->Marginal_Output_Stream<ofstream&>().open(marg_filename.str(),ios_base::out);	
				}
				
				stringstream log_filename("");
				log_filename << scenario->template output_dir_name<string>();
				log_filename << "popsyn_fit_results.csv";
				this->Log_File<ofstream&>().open(log_filename.str(),ios_base::out);	

				this_component()->template Initialize<NT>();

				((ComponentType*)this)->Load_Event<ComponentType>(&Popsyn_Event_Controller,POPSYN_ITERATIONS::MAIN_INITIALIZE,POPSYN_SUBITERATIONS::INITIALIZE);
			}
			template<typename NetworkType, typename ScenarioType> void Initialize(NetworkType network, ScenarioType scenario, requires(NetworkType, check_stripped_type(NetworkType, Network_Components::Concepts::Is_Transportation_Network) && check_stripped_type(ScenarioType, Concepts::Scenario_Has_Popsyn_Configuration_Data)))
			{
				this->Initialize<ScenarioType>(scenario);
				this->network_reference<NetworkType>(network);		
			}
			template<typename NetworkType, typename ScenarioType> void Initialize(NetworkType network, ScenarioType scenario, requires(NetworkType, !check_stripped_type(NetworkType, Network_Components::Concepts::Is_Transportation_Network) || !check_stripped_type(ScenarioType, Concepts::Scenario_Has_Popsyn_Configuration_Data)))
			{
				assert_check(NetworkType, Network_Components::Concepts::Is_Transportation_Network, "Error, the specified NetworkType is not a valid Transportation network.");
				assert_check(ScenarioType, Scenario_Components::Concepts::Has_Popsyn_Configuration_Data, "Error, the specified ScenarioType is not a valid Scenario reference.");
			}
			

			//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
			// Main analysis loop events, used to separate operations into different timesteps
			//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
			static void Popsyn_Event_Controller(ComponentType* _this,Event_Response& response)
			{
				typedef typename get_type_of(Synthesis_Regions_Collection)						region_collection_type;
				typedef get_mapped_component_type(region_collection_type)						region_type;
				typedef typename region_type::Sample_Data_type									sample_collection_type;
				typedef get_mapped_component_type(sample_collection_type)						sample_type;
				typedef typename region_type::Temporary_Sample_Data_type						temporary_sample_collection_type;
				typedef get_mapped_component_type(temporary_sample_collection_type)				temp_sample_type;
				typedef typename region_type::Synthesis_Zone_Collection_type					zone_collection_type;
				typedef get_mapped_component_type(zone_collection_type)							zone_type;

				Population_Synthesizer<ComponentType>* pthis = (Population_Synthesizer<ComponentType>*)_this;

				if (iteration() == 0 || iteration() == 1)
				{
					switch (sub_iteration())
					{
					case POPSYN_SUBITERATIONS::INITIALIZE:
						pthis->Read_Input_Popsyn_Event<NT>();
						response.next._iteration = iteration();
						response.next._sub_iteration = POPSYN_SUBITERATIONS::START_TIMING;
						break;
					case POPSYN_SUBITERATIONS::START_TIMING:
						pthis->Start_Timer_Event<NT>();
						response.next._iteration = POPSYN_ITERATIONS::MAIN_PROCESS;
						response.next._sub_iteration = POPSYN_SUBITERATIONS::STOP_TIMING;
						break;
					case POPSYN_SUBITERATIONS::STOP_TIMING:
						pthis->Stop_Timer_Event<NT>();
						response.next._iteration = iteration();
						response.next._sub_iteration = POPSYN_SUBITERATIONS::OUTPUT;
						break;
					case POPSYN_SUBITERATIONS::OUTPUT:
						pthis->Create_Agents_Event<NT>();
						pthis->Write_Files_Event<NT>();
						pthis->Write_Fit_Results<zone_type>();
						response.next._iteration = 3;
						response.next._sub_iteration = 0;
						break;
					default:
						response.next._iteration = END;
						response.next._sub_iteration = 0;
					}
				}
				else if (iteration() == 3)
				{
					pthis->Output_Popsyn_To_DB_Event<NT>();
					response.next._iteration = END;
					response.next._sub_iteration = 0;
				}
				else
				{
					//response.result = false;
					response.next._iteration = END;
					response.next._sub_iteration = 0;
				}
				
				
			}

			// 1.) Startup Event - Reads inputs and allocates analysis objects (at Iteration = 0, timestep 1)
			template<typename TargetType> bool Read_Input_Popsyn_Event(requires(TargetType,check_stripped_type(ComponentType,Concepts::Uses_Linker_File)))
			{
				cout<<endl<<"====================================================="<<endl<<"Starting synthetic population generation:"<<endl;

				ofstream& out = this->Output_Stream<ofstream&>();
				
				//===============================================================================================================
				// Initialize settings
				//---------------------------------------------------------------------------------------------------------------
				// Solver Settings
				typedef PopSyn::Prototypes::Solver_Settings<typename get_type_of(Solution_Settings)> solver_itf;
				solver_itf* solver = this->Solution_Settings<solver_itf*>();

				//===============================================================================================================
				// Initialize file linker
				//---------------------------------------------------------------------------------------------------------------
				typedef PopSyn::Prototypes::Popsyn_File_Linker<typename get_type_of(file_linker)> linker_itf;
				linker_itf* linker = this->file_linker<linker_itf*>();
				std::vector<int>& dims_hh = linker->hh_dimension_sizes();
				std::vector<int>& dims_per = linker->person_dimension_sizes();
				int dims_test_hh = linker->test_dimension_size();
				int dims_test_per = linker->test_person_dimension_size();

				//===============================================================================================================
				#pragma region Define interfaces
				//---------------------------------------------------------------------------------------------------------------
				typedef typename get_type_of(Synthesis_Regions_Collection)								region_collection_type;
				typedef get_mapped_component_type(region_collection_type)								region_type;
				typedef Pair_Associative_Container<region_collection_type>								regions_itf;
				typedef PopSyn::Prototypes::Synthesis_Region<region_type>								region_itf;		
				typedef typename region_type::Synthesis_Zone_Collection_type							zone_collection_type;
				typedef get_mapped_component_type(zone_collection_type)									zone_type;
				typedef Pair_Associative_Container<zone_collection_type,zone_collection_type::key_type> zones_itf;
				typedef PopSyn::Prototypes::Synthesis_Zone<zone_type>									zone_itf;			
				#pragma endregion


				regions_itf* regions = this->Synthesis_Regions_Collection<regions_itf*>();
				typename regions_itf::iterator region_itr;
				typename zones_itf::iterator z_itr;
				region_itf* new_region;
				

				//===============================================================================================================
				// read region household file, fill sample data
				//---------------------------------------------------------------------------------------------------------------			
				File_IO::File_Reader fr;
				fr.Open(linker->sample_file_path(),true,",\t");
				while(fr.Read())
				{
					// read the region data from input file
					typename regions_itf::key_type ID;
					if(!fr.Get_Data<typename regions_itf::key_type>(ID,linker->region_id_column())) break; 

					// get reference to existing region for the data item, or create new region if not yet existing
					if ((region_itr = regions->find(ID)) == regions->end())
					{
						// create new region
						new_region = (region_itf*)Allocate<region_type>();
						new_region->template Initialize<std::vector<int>&>(dims_hh,dims_per);
						new_region->parent_reference(this);
						new_region->template Output_Stream<ostream&>(out);
						new_region->template ID<int>(ID);
						new_region->template Solver_Settings<solver_itf*>(solver);

						// add new region to the std::list
						pair<typename regions_itf::key_type, region_itf*> item = pair<typename regions_itf::key_type, region_itf*>(ID, new_region);
						regions->insert(item);
					}
					else new_region = (region_itf*)region_itr->second;

					// Add the sample data from file to the region sample collection - use custom method tailored to solution algorithm
					new_region->Add_Household_Sample<linker_itf*>(fr,linker);
				}
				fr.Close();



				//===============================================================================================================
				// read region person file, fill sample data
				//---------------------------------------------------------------------------------------------------------------
				fr.Open(linker->person_sample_file_path(),true,",\t"); 
				while(fr.Read())
				{
					// read the region data from input file
					typename regions_itf::key_type ID;		
					if(!fr.Get_Data<typename regions_itf::key_type>(ID,linker->person_region_id_column())) break; 
					if ((region_itr = regions->find(ID)) == regions->end()) {THROW_WARNING("Sample from person file has non-existent region id.");}
					else
					{
						// Get interface to the region
						new_region = (region_itf*)region_itr->second;
						// next add the person sample data to the region
						new_region->Add_Person_Sample<linker_itf*>(fr,linker);						
					}
				}
				fr.Close();



				//===============================================================================================================
				// Read zone file, fill marginal data
				//---------------------------------------------------------------------------------------------------------------
				File_IO::File_Reader zone_fr;
				zone_fr.Open(linker->marg_file_path(),true,",\t");
				while(zone_fr.Read())
				{
					// get ID values for ZONE and REGION
					typename zone_type::ID_type ID;
					if(!zone_fr.Get_Data<typename zone_type::ID_type>(ID,linker->zone_id_column())){THROW_EXCEPTION("ERROR: could not retrieve zone id from zone file");}
					typename regions_itf::key_type RID;
					if(!zone_fr.Get_Data<typename regions_itf::key_type>(RID,linker->region_in_zone_id_column())){THROW_EXCEPTION("ERROR: could not retrieve region id from zone file");}		
					
					// get region iterator from hash map
					if ((region_itr = regions->find(RID))==regions->end()) {THROW_EXCEPTION("ERROR: Region ID '"<<RID <<"' not found in map.  Press a key to end.");}
					region_itf* region = (region_itf*)region_itr->second;

					// Initialize the new zone
					zone_itf* zone = (zone_itf*)Allocate<zone_type>();
					zone->template Initialize<std::vector<int>&>(dims_hh,dims_per,dims_test_hh,dims_test_per);
					zone->ID(ID);
					zone->parent_reference(region);
					zone->template Solver_Settings<solver_itf*>(solver);

					// Read data from file and add to the zone marginal data container
					zone->Add_Marginal_Data<linker_itf*>(zone_fr,linker);			

					// Add the zone to the current associated region's zone list
					pair<typename zone_type::ID_type,zone_itf*> item = pair<typename zone_type::ID_type,zone_itf*>(ID,zone);
					region->template Synthesis_Zone_Collection<zones_itf*>()->insert(item);
				}
				zone_fr.Close();

				//===============================================================================================================
				// Link the synthesizer zones to the activity locations in the network_reference, if it exists, otherwise ignore
				this->Link_Zones_To_Network_Locations<get_type_of(network_reference)>();				
				//---------------------------------------------------------------------------------------------------------------

				return true;
			}
			template<typename TargetType> bool Read_Input_Popsyn_Event(requires(TargetType,check_stripped_type(ComponentType,!Concepts::Uses_Linker_File)))
			{
				assert_check(ComponentType,Concepts::Uses_Linker_File,"This popsyn type does not use linker file setup.");
			}
			
			template<typename NetworkType> bool Link_Zones_To_Network_Locations(requires(NetworkType,check(NetworkType, Network_Components::Concepts::Is_Transportation_Network)))
			{
				//===============================================================================================================
				// Fill zonal activity_locations std::list from network reference
				//---------------------------------------------------------------------------------------------------------------

				//===============================================================================================================
				#pragma region Define interfaces
				//---------------------------------------------------------------------------------------------------------------
				typedef typename get_type_of(Synthesis_Regions_Collection)						region_collection_type;
				typedef get_mapped_component_type(region_collection_type)						region_type;
				typedef typename region_type::Sample_Data_type									sample_collection_type;
				typedef get_mapped_component_type(sample_collection_type)						sample_type;
				typedef typename region_type::Temporary_Sample_Data_type						temporary_sample_collection_type;
				typedef get_mapped_component_type(temporary_sample_collection_type)				temp_sample_type;
				typedef typename region_type::Synthesis_Zone_Collection_type					zone_collection_type;
				typedef get_mapped_component_type(zone_collection_type)							zone_type;
				typedef Network_Components::Prototypes::Network<typename get_type_of(network_reference)> network_itf;
				typedef Random_Access_Sequence<typename network_itf::get_type_of(activity_locations_container)> locations_container_itf;
				typedef Activity_Location_Components::Prototypes::Activity_Location<typename get_component_type(locations_container_itf)>  location_itf;
				typedef Pair_Associative_Container<region_collection_type> regions_itf;
				typedef PopSyn::Prototypes::Synthesis_Region<region_type> region_itf;		
				typedef Pair_Associative_Container<zone_collection_type,zone_collection_type::key_type> zones_itf;
				typedef PopSyn::Prototypes::Synthesis_Zone<zone_type> zone_itf;
				typedef Random_Access_Sequence<typename zone_itf::get_type_of(Activity_Locations_Container)> location_id_container_itf;
				#pragma endregion

				//===============================================================================================================
				network_itf* network = this->network_reference<network_itf*>();
				locations_container_itf* locations = network->template activity_locations_container<locations_container_itf*>();
				location_itf* location;
				region_itf* region;
				zone_itf* zone;
				typename locations_container_itf::iterator loc_itr;
				typename zones_itf::iterator zone_itr;
				typename regions_itf::iterator region_itr;

				regions_itf* regions = this->Synthesis_Regions_Collection<regions_itf*>();

				// Loop over all activity locations in network
				for (loc_itr = locations->begin(); loc_itr != locations->end(); ++loc_itr)
				{
					location = (location_itf*)(*loc_itr);

					// Get the census zone id from the location - this is used to link the location to the zone and must be defined accordingly in the database
					long long zone_id = location->template census_zone_id<long long >();
					
					// ignore non-residential land use locations
					if (!location->is_residential_location<bool>()) continue;

					// Look through all regions to find the census zone to which the location belongs
					for (region_itr = regions->begin(); region_itr != regions->end(); ++region_itr)
					{
						region = region_itr->second;

						zones_itf* zones = region->template Synthesis_Zone_Collection<zones_itf*>();
						
						zone_itr = zones->find(zone_id);

						// Zone found, assign location to zone and break out of loop, go to next location
						if (zone_itr != zones->end())
						{
							zone = zone_itr->second;
							zone->template Activity_Locations_Container<location_id_container_itf*>()->push_back(location->template internal_id<int>());
							break;
						}
					}
				}

				return true;
			}
			template<typename NetworkType> bool Link_Zones_To_Network_Locations(requires(NetworkType,!check(NetworkType, Network_Components::Concepts::Is_Transportation_Network)))
			{
				return false;
			}

			// 2.) Start timing event - called before individual objects begin processing (at Iteration = 0, timestep 3)
			template<typename TargetType> void Start_Timer_Event()
			{
				this->timer<Counter&>().Start();
			}		

			/// Individual Synthesis_Regions processing happens here - see Synthesis_Region_Prototype class for details
			
			// 3.) Stop timing event - called after individual objects end processing (at Iteration = 0, timestep 5)
			template<typename TargetType> void Stop_Timer_Event()
			{
				cout << endl<<"Main Algorithm run-time (s): " << this->timer<Counter&>().Stop()/1000.0<<endl;
			}
			
			// 4.) Create agents from synthesized results and output results - called after timing is stopped (at Iteration = 0, timestep 7)
			template<typename TargetType> void Create_Agents_Event()
			{
				//=============================================================================================
				#pragma region Define interfaces
				//---------------------------------------------------------------------------------------------
				// Type defines for sub_objects
				// Define iterators and get pointer to the region collection
				typedef typename get_type_of(Synthesis_Regions_Collection)						region_collection_type;
				typedef get_mapped_component_type(region_collection_type)						region_type;
				typedef typename region_type::Sample_Data_type									sample_collection_type;
				typedef get_mapped_component_type(sample_collection_type)						sample_type;
				typedef typename region_type::Synthesis_Zone_Collection_type					zone_collection_type;
				typedef get_mapped_component_type(zone_collection_type)							zone_type;
				typedef typename zone_type::get_type_of(Synthetic_Households_Container)			household_collection_type;
				typedef get_component_type(household_collection_type)							household_type;

				//---------------------------------------------------------------------------------------------
				// Interface defines for sub_objects
				typedef PopSyn::Prototypes::Synthesis_Region<region_type> region_itf;
				typedef Pair_Associative_Container<region_collection_type,region_collection_type::key_type, region_collection_type::mapped_type> regions_itf;
				typedef PopSyn::Prototypes::Synthesis_Zone<zone_type> zone_itf;
				typedef Pair_Associative_Container<zone_collection_type,zone_collection_type::key_type,zone_itf*> zones_itf;
				typedef Household_Components::Prototypes::Household_Properties<sample_type> pop_unit_itf;
				typedef Pair_Associative_Container<sample_collection_type,sample_collection_type::key_type, pop_unit_itf*> sample_data_itf;
				typedef Person_Components::Prototypes::Person_Properties<typename remove_pointer<typename pop_unit_itf::get_type_of(Persons_Container)::value_type>::type> person_unit_itf;
				typedef Random_Access_Sequence<typename pop_unit_itf::get_type_of(Persons_Container),person_unit_itf*> person_sample_data_itf;
				typedef Random_Access_Sequence<typename zone_type::type_of(Synthetic_Households_Container)> household_collection_itf;
				typedef Household_Components::Prototypes::Household<typename get_component_type(household_collection_itf)>  household_itf;		
				typedef Random_Access_Sequence<typename household_itf::get_type_of(Persons_Container)> person_collection_itf;
				typedef Person_Components::Prototypes::Person<typename get_component_type(person_collection_itf)>  person_itf;
				typedef Network_Components::Prototypes::Network<typename get_type_of(network_reference)> network_itf;
				typedef Scenario_Components::Prototypes::Scenario<typename get_type_of(scenario_reference)> scenario_itf;
				#pragma endregion
				
				cout<<endl;
			
				//---------------------------------------------------------------------------------------------
				// Start main object creation loop
				regions_itf* regions = this->Synthesis_Regions_Collection<regions_itf*>();
				network_itf* network = this->network_reference<network_itf*>();
				scenario_itf* scenario = this->scenario_reference<scenario_itf*>();
				
								
				// initialize all of the synthesized individuals and assign unique ids
				long uuid = 0; // globally unique person id
				int counter = 0;

				// Loop through all regions
				for (typename regions_itf::iterator r_itr = regions->begin(); r_itr != regions->end(); ++r_itr)
				{
					region_itf* region = (region_itf*)r_itr->second;
					zones_itf* zones = region->template Synthesis_Zone_Collection<zones_itf*>();

					// loop through zones in each region
					for (typename zones_itf::iterator z_itr = zones->begin(); z_itr != zones->end(); ++z_itr)
					{
						zone_itf* zone = (zone_itf*)z_itr->second;

						// loop through each synthesized person
						household_collection_itf* households = zone->template Synthetic_Households_Container<household_collection_itf*>();
						for (typename household_collection_itf::iterator h_itr = households->begin(); h_itr != households->end(); ++h_itr)
						{
							// initialize the hh - allocates all hh subcomponents
							household_itf* hh = (household_itf*)*h_itr;

							// call the object initialization handler which passes the object the appropriate base class, based on the analysis type
							// i.e. creates full agents when connected to an ABM and NetworkType is defined, otherwise does nothing.
							Object_Initialization_Handler<household_itf*,zone_itf*,network_itf*,scenario_itf*>(uuid,hh,zone,network,scenario);

							// Loop through each person in the household
							person_collection_itf* persons = hh->template Persons_Container<person_collection_itf*>();
							long perid=0;
							for (typename person_collection_itf::iterator p_itr = persons->begin(); p_itr != persons->end(); ++p_itr)
							{		
								// update synthesizing persons counter
								if (counter % 10000 == 0) cout << '\r' << "Initializing Agents: " << counter;

								person_itf* person = (person_itf*)(*p_itr);
								// Same as above, creates person agents when connected to an ABM through the NetworkType definition
								Object_Initialization_Handler<person_itf*,zone_itf*,network_itf*,scenario_itf*>(perid,person,zone,network,scenario);
								++perid;
								++counter;

#ifdef DEBUG_1
								++r_itr;++z_itr;++h_itr;++p_itr;
								regions->erase(r_itr,regions->end());
								zones->erase(z_itr,zones->end());
								households->erase(h_itr,households->end());
								persons->erase(p_itr,persons->end());
								return;
#endif
							}


							++uuid;
						}
					}
				}
				cout <<endl<<endl<<"Total Households Synthesized: "<<uuid;
				cout <<endl		 <<"Total Persons Synthesized: "<<counter<<endl<<endl;
			}
			template<typename HHItfType, typename ZoneItfType, typename NetworkItfType, typename ScenarioItfType> void Object_Initialization_Handler(long id, HHItfType hh, ZoneItfType zone, NetworkItfType network, ScenarioItfType scenario, requires(NetworkItfType, check(typename get_type_of(network_reference),Network_Components::Concepts::Is_Transportation_Network)))
			{
				hh->template Initialize<long,ZoneItfType, NetworkItfType, ScenarioItfType>(id, zone, network, scenario);
			}
			template<typename HHItfType, typename ZoneItfType, typename NetworkItfType, typename ScenarioItfType> void Object_Initialization_Handler(long id, HHItfType hh, ZoneItfType zone, NetworkItfType network, ScenarioItfType scenario, requires(NetworkItfType, !check(typename get_type_of(network_reference),Network_Components::Concepts::Is_Transportation_Network)))
			{
			}
			template<typename TargetType> void Write_Files_Event()			
			{
				this->timer<Counter&>().Start();
				ofstream& sample_out = this->Output_Stream<ofstream&>();
				ofstream& marg_out = this->Marginal_Output_Stream<ofstream&>();
				ofstream& popsyn_log = this->Log_File<ofstream&>();

				//=============================================================================================
				#pragma region Define interfaces
				//---------------------------------------------------------------------------------------------
				// Type defines for sub_objects
				typedef typename get_type_of(Synthesis_Regions_Collection)						region_collection_type;
				typedef get_mapped_component_type(region_collection_type)						region_type;
				typedef typename region_type::Synthesis_Zone_Collection_type					zone_collection_type;
				typedef get_mapped_component_type(zone_collection_type)							zone_type;
				typedef PopSyn::Prototypes::Synthesis_Region<region_type> region_itf;
				typedef Pair_Associative_Container<region_collection_type,region_collection_type::key_type, region_collection_type::mapped_type> regions_itf;
				typedef PopSyn::Prototypes::Synthesis_Zone<zone_type> zone_itf;
				typedef Pair_Associative_Container<zone_collection_type,zone_collection_type::key_type,zone_itf*> zones_itf;
				typedef PopSyn::Prototypes::Popsyn_File_Linker<typename get_type_of(file_linker)> linker_itf;
				#pragma endregion
					
				//==============================================================================================
				// Write Marginal file header
				//==============================================================================================
				// get problem dimensions to use in printing header
				linker_itf* linker = this->file_linker<linker_itf*>();
				std::vector<int>& dims_hh = linker->hh_dimension_sizes();
				std::vector<int>& dims_per = linker->person_dimension_sizes();
				int dims_test_hh = linker->test_dimension_size();
				int dims_test_per = linker->test_person_dimension_size();

				// print output file header
				marg_out<<"Zone\t";
				int dim_count=0;
				for (vector<int>::iterator i = dims_hh.begin(); i != dims_hh.end(); ++i, ++dim_count)
				{
					for (int j=0; j<*i; ++j) marg_out<<"HH_D"<<dim_count<<"_"<<j<<"_orig\t";
				}
				marg_out<<"\t";
				dim_count=0;
				for (vector<int>::iterator i = dims_hh.begin(); i != dims_hh.end(); ++i, ++dim_count)
				{
					for (int j=0; j<*i; ++j) marg_out<<"HH_D"<<dim_count<<"_"<<j<<"_sim\t";
				}
				marg_out<<"\t";
				dim_count=0;
				for (vector<int>::iterator i = dims_hh.begin(); i != dims_hh.end(); ++i, ++dim_count)
				{
					for (int j=0; j<*i; ++j) marg_out<<"HH_D"<<dim_count<<"_"<<j<<"_APD\t";
				}
				marg_out <<"\t\tHH_WAAPD\t\t\t";
				dim_count=0;
				for (vector<int>::iterator i = dims_per.begin(); i != dims_per.end(); ++i, ++dim_count)
				{
					for (int j=0; j<*i; ++j) marg_out<<"PER_D"<<dim_count<<"_"<<j<<"_orig\t";
				}
				marg_out<<"\t";
				dim_count=0;
				for (vector<int>::iterator i = dims_per.begin(); i != dims_per.end(); ++i, ++dim_count)
				{
					for (int j=0; j<*i; ++j) marg_out<<"PER_D"<<dim_count<<"_"<<j<<"_sim\t";
				}
				marg_out<<"\t";
				dim_count=0;
				for (vector<int>::iterator i = dims_per.begin(); i != dims_per.end(); ++i, ++dim_count)
				{
					for (int j=0; j<*i; ++j) marg_out<<"PER_D"<<dim_count<<"_"<<j<<"_APD\t";
				}
				marg_out <<"\t\tPER_WAAPD\n";

				//==============================================================================================
				// Loop through all regions/zones and handle file output if needed
				//==============================================================================================
				regions_itf* regions = this->Synthesis_Regions_Collection<regions_itf*>();
				for (typename regions_itf::iterator r_itr = regions->begin(); r_itr != regions->end(); ++r_itr)
				{
					region_itf* region = r_itr->second;
					zones_itf* zones = region->template Synthesis_Zone_Collection<zones_itf*>();
					for (typename zones_itf::iterator z_itr = zones->begin(); z_itr != zones->end(); ++z_itr)
					{
						zone_itf* zone = z_itr->second;
						zone->Write_Distribution_Results<NT>(marg_out,sample_out);
					}
				}
				sample_out.close();
				marg_out.close();
			}
			template<typename ZoneType> void Write_Fit_Results(requires(ZoneType,check(ZoneType, Concepts::Is_IPF_Compatible)))
			{
				this->timer<Counter&>().Start();
				ofstream& popsyn_log = this->Log_File<ofstream&>();

				//=============================================================================================
				#pragma region Define interfaces
				//---------------------------------------------------------------------------------------------
				// Type defines for sub_objects
				// Define iterators and get pointer to the region collection
				typedef typename get_type_of(Synthesis_Regions_Collection)						region_collection_type;
				typedef get_mapped_component_type(region_collection_type)						region_type;
				typedef typename region_type::Sample_Data_type									sample_collection_type;
				typedef get_mapped_component_type(sample_collection_type)						sample_type;
				typedef typename region_type::Temporary_Sample_Data_type						temporary_sample_collection_type;
				typedef get_mapped_component_type(temporary_sample_collection_type)				temp_sample_type;
				typedef typename region_type::Synthesis_Zone_Collection_type					zone_collection_type;
				typedef get_mapped_component_type(zone_collection_type)							zone_type;
				typedef typename region_type::get_type_of(Target_Joint_Distribution)			joint_dist_type;
				typedef typename region_type::get_type_of(Target_Marginal_Distribution)			marg_dist_type;

				typedef PopSyn::Prototypes::Synthesis_Region<region_type> region_itf;
				typedef Pair_Associative_Container<region_collection_type,region_collection_type::key_type, region_collection_type::mapped_type> regions_itf;
				typedef PopSyn::Prototypes::Synthesis_Zone<zone_type> zone_itf;
				typedef Pair_Associative_Container<zone_collection_type,zone_collection_type::key_type,zone_itf*> zones_itf;
				typedef Random_Access_Sequence<typename zone_type::type_of(Synthetic_Households_Container)> households_container_itf;
				typedef Household_Components::Prototypes::Household_Properties<typename get_component_type(typename zone_type::type_of(Synthetic_Households_Container))> household_itf;
				typedef Random_Access_Sequence<typename zone_type::type_of(Synthetic_Persons_Container)> persons_container_itf;
				typedef Person_Components::Prototypes::Person_Properties<typename get_component_type(typename zone_type::type_of(Synthetic_Persons_Container))> person_itf;
				typedef Multidimensional_Random_Access_Array<joint_dist_type>	joint_itf;
				typedef Multidimensional_Random_Access_Array<marg_dist_type>	marginal_itf;
				typedef typename marginal_itf::index_type index;
				typedef Scenario_Components::Prototypes::Scenario<typename get_type_of(scenario_reference)> scenario_itf;
				#pragma endregion
					
				scenario_itf* scenario = this->scenario_reference<scenario_itf*>();

				regions_itf* regions = this->Synthesis_Regions_Collection<regions_itf*>();

				int zone_count = 0;
				
				//=============================================================================================
				// create containers to hold results
				//---------------------------------------------------------------------------------------------
				s_array<double> marginal_hh_error;
				s_array<double> marginal_hh_sum;
				s_array<double> marginal_per_error;
				s_array<double> marginal_per_sum;
				s_array<double> test_marginal_hh_error;
				s_array<double> test_marginal_hh_sum;
				s_array<double> test_marginal_per_error;
				s_array<double> test_marginal_per_sum;

				m_array<double> joint_hh_error;
				m_array<double> joint_hh_sum;
				m_array<double> joint_per_error;
				m_array<double> joint_per_sum;
				m_array<double> rounding_hh_error;
				m_array<double> rounding_per_error;

				s_array<double>::const_dimensional_type marg_dims = regions->begin()->second->Target_Marginal_Distribution<marginal_itf&>().dimensions();
				marginal_hh_error.resize(marg_dims,0);
				marginal_hh_sum.resize(marg_dims,0);
				marginal_per_error.resize(regions->begin()->second->Target_Person_Marginal_Distribution<marginal_itf&>().dimensions(),0);
				marginal_per_sum.resize(regions->begin()->second->Target_Person_Marginal_Distribution<marginal_itf&>().dimensions(),0);

				test_marginal_hh_error.resize(regions->begin()->second->Synthesis_Zone_Collection<zones_itf*>()->begin()->second->Test_Marginal_Distribution<marginal_itf&>().dimensions(),0);
				test_marginal_hh_sum.resize(regions->begin()->second->Synthesis_Zone_Collection<zones_itf*>()->begin()->second->Test_Marginal_Distribution<marginal_itf&>().dimensions(),0);
				test_marginal_per_error.resize(regions->begin()->second->Synthesis_Zone_Collection<zones_itf*>()->begin()->second->Test_Person_Marginal_Distribution<marginal_itf&>().dimensions(),0);
				test_marginal_per_sum.resize(regions->begin()->second->Synthesis_Zone_Collection<zones_itf*>()->begin()->second->Test_Person_Marginal_Distribution<marginal_itf&>().dimensions(),0);

				joint_hh_error.resize(regions->begin()->second->Target_Joint_Distribution<joint_itf&>().dimensions(),0);
				joint_hh_sum.resize(regions->begin()->second->Target_Joint_Distribution<joint_itf&>().dimensions(),0);
				joint_per_error.resize(regions->begin()->second->Target_Person_Joint_Distribution<joint_itf&>().dimensions(),0);
				joint_per_sum.resize(regions->begin()->second->Target_Person_Joint_Distribution<joint_itf&>().dimensions(),0);

				rounding_hh_error.resize(regions->begin()->second->Target_Joint_Distribution<joint_itf&>().dimensions(),0);
				rounding_per_error.resize(regions->begin()->second->Target_Person_Joint_Distribution<joint_itf&>().dimensions(),0);

				//=============================================================================================
				// Loop through all regions/zones and handle file output if needed
				//---------------------------------------------------------------------------------------------
				for (typename regions_itf::iterator r_itr = regions->begin(); r_itr != regions->end(); ++r_itr)
				{
					region_itf* region = r_itr->second;
					zones_itf* zones = region->template Synthesis_Zone_Collection<zones_itf*>();
					for (typename zones_itf::iterator z_itr = zones->begin(); z_itr != zones->end(); ++z_itr)
					{
						zone_itf* zone = z_itr->second;
						++zone_count;

						// get the marginal fit results
						marginal_itf& marg_hh =		zone->template Target_Marginal_Distribution<marginal_itf&>();
						marginal_itf& marg_per =	zone->template Target_Person_Marginal_Distribution<marginal_itf&>();
						marginal_itf& syn_marg_hh = zone->template Synthesized_Marginal_Distribution<marginal_itf&>();
						marginal_itf& syn_marg_per= zone->template Synthesized_Person_Marginal_Distribution<marginal_itf&>();
							
						marginal_itf& test_marg_hh =		zone->template Test_Marginal_Distribution<marginal_itf&>();
						marginal_itf& test_marg_per =		zone->template Test_Person_Marginal_Distribution<marginal_itf&>();
						marginal_itf& syn_test_marg_hh =	zone->template Synthesized_Test_Marginal_Distribution<marginal_itf&>();
						marginal_itf& syn_test_marg_per=	zone->template Synthesized_Test_Person_Marginal_Distribution<marginal_itf&>();

						for (int i = 0; i < (int)marg_hh.num_dimensions(); ++i)
						{
							for (int d = 0; d < (int)marg_hh.dimensions()[i]; ++d)
							{
								marginal_hh_error[index(i,d)] += abs(syn_marg_hh[index(i,d)] - marg_hh[index(i,d)]);
								marginal_hh_sum[index(i,d)] += marg_hh[index(i,d)];
							}
						}
						for (int i = 0; i < (int)marg_per.num_dimensions(); ++i)
						{
							for (int d = 0; d < (int)marg_per.dimensions()[i]; ++d)
							{
								marginal_per_error[index(i,d)] += abs(syn_marg_per[index(i,d)] - marg_per[index(i,d)]);
								marginal_per_sum[index(i,d)] += marg_per[index(i,d)];
							}
						}

						for (int i = 0; i < (int)test_marg_hh.num_dimensions(); ++i)
						{
							for (int d = 0; d < (int)test_marg_hh.dimensions()[i]; ++d)
							{
								test_marginal_hh_error[index(i,d)] += abs(syn_test_marg_hh[index(i,d)] - test_marg_hh[index(i,d)]);
								test_marginal_hh_sum[index(i,d)] += test_marg_hh[index(i,d)];
							}
						}
						for (int i = 0; i < (int)test_marg_per.num_dimensions(); ++i)
						{
							for (int d = 0; d < (int)test_marg_per.dimensions()[i]; ++d)
							{
								test_marginal_per_error[index(i,d)] += abs(syn_test_marg_per[index(i,d)] - test_marg_per[index(i,d)]);
								test_marginal_per_sum[index(i,d)] += test_marg_per[index(i,d)];
							}
						}
						// get the joint distribution fit results
						joint_itf& joint_hh =		zone->template Target_Joint_Distribution<joint_itf&>();
						joint_itf& joint_per =		zone->template Target_Person_Joint_Distribution<joint_itf&>();
						joint_itf& syn_joint_hh =	zone->template Synthesized_Joint_Distribution<joint_itf&>();
						joint_itf& syn_joint_per=	zone->template Synthesized_Person_Joint_Distribution<joint_itf&>();

						for (int i = 0; i < (int)joint_hh.size(); ++i)
						{
							if (joint_hh[i] > 0) joint_hh_error[i] += abs(syn_joint_hh[i] - joint_hh[i])/joint_hh[i];
							joint_hh_sum[i] += joint_hh[i];
							//theoretical rounding error
							double p = joint_hh[i] - (double)((int)joint_hh[i]);
							if (joint_hh[i] > 0) rounding_hh_error[i] += 2.0*p*(1.0-p)/joint_hh[i];
						}
						for (int i = 0; i < (int)joint_per.size(); ++i)
						{
							if (joint_per[i] > 0) joint_per_error[i] += abs(syn_joint_per[i] - joint_per[i])/joint_per[i];
							joint_per_sum[i] += joint_per[i];
							//theoretical rounding error
							double p = joint_per[i] - (double)((int)joint_per[i]);
							if (joint_per[i] > 0) rounding_per_error[i] += 2.0*p*(1.0-p)/joint_per[i];
						}
					}
				}

				//=============================================================================================
				// Write output to results file
				//---------------------------------------------------------------------------------------------
				double total_hh_error = 0;
				double total_hh_sum = 0;
				popsyn_log <<"WAAPD value for household marginals:"<<setprecision(2)<<endl;
				popsyn_log <<"Dimension,Category,WAAPD%"<<endl;
				for (int i = 0; i < (int)marginal_hh_error.num_dimensions(); ++i)
				{
					for (int d = 0; d < (int)marginal_hh_error.dimensions()[i]; ++d)
					{
						popsyn_log <<i<<","<<d<<","<<fixed<<marginal_hh_error[index(i,d)]/marginal_hh_sum[index(i,d)]*100.0<<"%"<<endl;

						total_hh_error += marginal_hh_error[index(i,d)];
						total_hh_sum +=marginal_hh_sum[index(i,d)];
					}
				}
				popsyn_log <<"Total,,"<<fixed<<total_hh_error/total_hh_sum*100.0<<"%"<<endl<<endl;
				popsyn_log <<"WAAPD value for person marginals:"<<endl;
				popsyn_log <<"Dimension,Category,WAAPD%"<<endl;
				double total_per_error = 0;
				double total_per_sum = 0;
				for (int i = 0; i < (int)marginal_per_error.num_dimensions(); ++i)
				{
					for (int d = 0; d < (int)marginal_per_error.dimensions()[i]; ++d)
					{
						popsyn_log <<i<<","<<d<<","<<fixed<<marginal_per_error[index(i,d)]/marginal_per_sum[index(i,d)]*100.0<<"%"<<endl;

						total_per_error += marginal_per_error[index(i,d)];
						total_per_sum +=marginal_per_sum[index(i,d)];
					}
				}
				popsyn_log <<"Total,,"<<fixed<<total_per_error/total_per_sum*100.0<<"%"<<endl;
				popsyn_log <<"WAAPD value for TEST household marginals:"<<setprecision(2)<<endl;
				popsyn_log <<"Dimension,Category,WAAPD%"<<endl;
				double total_test_hh_error = 0;
				double total_test_hh_sum = 0;
				for (int i = 0; i < (int)test_marginal_hh_error.num_dimensions(); ++i)
				{
					for (int d = 0; d < (int)test_marginal_hh_error.dimensions()[i]; ++d)
					{
						popsyn_log <<i<<","<<d<<","<<fixed<<test_marginal_hh_error[index(i,d)]/test_marginal_hh_sum[index(i,d)]*100.0<<"%"<<endl;

						total_test_hh_error += test_marginal_hh_error[index(i,d)];
						total_test_hh_sum +=test_marginal_hh_sum[index(i,d)];
					}
				}
				popsyn_log <<"Total,,"<<fixed<<total_test_hh_error/total_test_hh_sum*100.0<<"%"<<endl<<endl;
				popsyn_log <<"WAAPD value for TEST person marginals:"<<endl;
				popsyn_log <<"Dimension,Category,WAAPD%"<<endl;
				double total_test_per_error = 0;
				double total_test_per_sum = 0;
				for (int i = 0; i < (int)test_marginal_per_error.num_dimensions(); ++i)
				{
					for (int d = 0; d < (int)test_marginal_per_error.dimensions()[i]; ++d)
					{
						popsyn_log <<i<<","<<d<<","<<fixed<<test_marginal_per_error[index(i,d)]/test_marginal_per_sum[index(i,d)]*100.0<<"%"<<endl;

						total_test_per_error += test_marginal_per_error[index(i,d)];
						total_test_per_sum +=test_marginal_per_sum[index(i,d)];
					}
				}
				popsyn_log <<"Total,,"<<fixed<<total_test_per_error/total_test_per_sum*100.0<<"%"<<endl;

				popsyn_log <<"\n\nHH AAPD results:"<<endl;
				popsyn_log <<"HH_CELL_ID,AAPD_sim,AAPD_rounding,Avg_Size"<<endl;
				for (int i = 0; i < (int)joint_hh_error.size(); ++i)
				{
					joint_itf::index_type idx = joint_hh_error.get_index(i);
					for (joint_itf::index_type::iterator itr = idx.begin(); itr != idx.end(); ++itr) popsyn_log <<*itr<<"-";
					popsyn_log <<","<<fixed<<joint_hh_error[i]/zone_count*100.0<<"%,";
					popsyn_log <<fixed<<rounding_hh_error[i]/zone_count*100.0<<"%,";
					popsyn_log <<joint_hh_sum[i]/zone_count<<endl;
				}
				popsyn_log <<"\n\nPER AAPD results:"<<endl;
				popsyn_log <<"PER_CELL_ID,AAPD_sim,AAPD_rounding,Avg_Size"<<endl;
				for (int i = 0; i < (int)joint_per_error.size(); ++i)
				{
					joint_itf::index_type idx = joint_per_error.get_index(i);
					for (joint_itf::index_type::iterator itr = idx.begin(); itr != idx.end(); ++itr) popsyn_log <<*itr<<"-";
					popsyn_log <<","<<fixed<<joint_per_error[i]/zone_count*100.0<<"%,";
					popsyn_log <<fixed<<rounding_per_error[i]/zone_count*100.0<<"%,";
					popsyn_log <<joint_per_sum[i]/zone_count<<endl;
				}

				popsyn_log.close();
			}
			template<typename ZoneType> void Write_Fit_Results(requires(ZoneType,check(ZoneType, Concepts::Is_IPU_Compatible)))
			{
				this->timer<Counter&>().Start();
				ofstream& popsyn_log = this->Log_File<ofstream&>();

				//=============================================================================================
				#pragma region Define interfaces
				//---------------------------------------------------------------------------------------------
				// Type defines for sub_objects
				// Define iterators and get pointer to the region collection
				typedef typename get_type_of(Synthesis_Regions_Collection)						region_collection_type;
				typedef get_mapped_component_type(region_collection_type)						region_type;
				typedef typename region_type::Sample_Data_type									sample_collection_type;
				typedef get_mapped_component_type(sample_collection_type)						sample_type;
				typedef typename region_type::Temporary_Sample_Data_type						temporary_sample_collection_type;
				typedef get_mapped_component_type(temporary_sample_collection_type)				temp_sample_type;
				typedef typename region_type::Synthesis_Zone_Collection_type					zone_collection_type;
				typedef get_mapped_component_type(zone_collection_type)							zone_type;
				typedef typename region_type::get_type_of(Target_Joint_Distribution)			joint_dist_type;
				typedef typename region_type::get_type_of(Target_Marginal_Distribution)			marg_dist_type;

				typedef PopSyn::Prototypes::Synthesis_Region<region_type> region_itf;
				typedef Pair_Associative_Container<region_collection_type,region_collection_type::key_type, region_collection_type::mapped_type> regions_itf;
				typedef PopSyn::Prototypes::Synthesis_Zone<zone_type> zone_itf;
				typedef Pair_Associative_Container<zone_collection_type,zone_collection_type::key_type,zone_itf*> zones_itf;
				typedef Random_Access_Sequence<typename zone_type::type_of(Synthetic_Households_Container)> households_container_itf;
				typedef Household_Components::Prototypes::Household_Properties<typename get_component_type(typename zone_type::type_of(Synthetic_Households_Container))> household_itf;
				typedef Random_Access_Sequence<typename zone_type::type_of(Synthetic_Persons_Container)> persons_container_itf;
				typedef Person_Components::Prototypes::Person_Properties<typename get_component_type(typename zone_type::type_of(Synthetic_Persons_Container))> person_itf;
				typedef Multidimensional_Random_Access_Array<joint_dist_type>	joint_itf;
				typedef Multidimensional_Random_Access_Array<marg_dist_type>	marginal_itf;
				typedef typename marginal_itf::index_type index;
				typedef Scenario_Components::Prototypes::Scenario<typename get_type_of(scenario_reference)> scenario_itf;
				#pragma endregion
					
				THROW_EXCEPTION("ERROR: FIT RESULT ESTIMATION HAS NOT BEEN IMPLEMENTED FOR IPU ALGORITHM - ADD FUNCTIONALITY TO POPSYN_PROTOTYPE.");
			}
			template<typename ZoneType> void Write_Fit_Results(requires(ZoneType,check(ZoneType, !Concepts::Is_IPF_Compatible) && !check(ZoneType, Concepts::Is_IPU_Compatible)))
			{
				assert_check(ZoneType, Concepts::Is_IPF_Compatible, "Error, ZoneType must be either IPF compatible...");
				assert_check(ZoneType, Concepts::Is_IPU_Compatible, "or ZoneType must be IPU compatible.");
			}
	
			// 5.) Write output to database (at Iteration 2) - the routine differs if writing for a full abm or for stand-alone popsyn with no network
			template<typename TargetType> void Output_Popsyn_To_DB_Event(/*requires(TargetType,!check(typename get_type_of(network_reference), Network_Components::Concepts::Is_Transportation_Network))*/)
			{
				//=============================================================================================
				#pragma region Define interfaces
				//---------------------------------------------------------------------------------------------
				// Type defines for sub_objects
				typedef typename get_type_of(Synthesis_Regions_Collection)				region_collection_type;
				typedef typename get_mapped_component_type(region_collection_type)		region_type;
				typedef typename region_type::type_of(Sample_Data)						sample_collection_type;
				typedef typename get_mapped_component_type(sample_collection_type)		sample_type;
				typedef typename region_type::type_of(Synthesis_Zone_Collection)		zone_collection_type;
				typedef typename get_mapped_component_type(zone_collection_type)		zone_type;
				typedef typename zone_type::get_type_of(Synthetic_Households_Container)	household_collection_type;
				typedef typename get_component_type(household_collection_type)			household_type;
				typedef typename household_type::get_type_of(Persons_Container)			person_collection_type;
				typedef typename get_component_type(person_collection_type)				person_type;
				typedef typename region_type::get_type_of(Target_Joint_Distribution)	joint_dist_type;
				typedef typename region_type::get_type_of(Target_Marginal_Distribution)	marg_dist_type;

				//---------------------------------------------------------------------------------------------
				// Interface defines for sub_objects
				typedef Pair_Associative_Container<region_collection_type> regions_itf;
				typedef PopSyn::Prototypes::Synthesis_Region<region_type> region_itf;		
				typedef Pair_Associative_Container<zone_collection_type> zones_itf;
				typedef PopSyn::Prototypes::Synthesis_Zone<zone_type> zone_itf;	
				typedef Pair_Associative_Container<sample_collection_type> sample_data_itf;
				typedef Household_Components::Prototypes::Household_Properties<sample_type> pop_unit_itf;
				typedef Random_Access_Sequence<typename pop_unit_itf::get_type_of(Persons_Container)> person_sample_data_itf;
				typedef Person_Components::Prototypes::Person_Properties<typename get_component_type(person_sample_data_itf)> person_unit_itf;
				typedef Random_Access_Sequence<typename zone_type::type_of(Synthetic_Households_Container)> household_collection_itf;
				typedef Household_Components::Prototypes::Household_Properties<typename get_component_type(typename zone_type::type_of(Synthetic_Households_Container))> household_itf;
				/*typedef Random_Access_Sequence<typename household_itf::type_of(Persons_Container)> person_collection_itf;
				typedef Person_Components::Prototypes::Person_Properties<typename get_component_type(typename zone_type::type_of(Synthetic_Persons_Container))> person_itf;*/
				typedef Random_Access_Sequence<person_collection_type> person_collection_itf;
				//typedef Person_Components::Prototypes::Person<typename get_component_type(person_collection_itf)>  person_itf;
				typedef Multidimensional_Random_Access_Array<joint_dist_type> joint_itf;
				typedef Multidimensional_Random_Access_Array<marg_dist_type> marginal_itf;
				typedef Scenario_Components::Prototypes::Scenario<typename get_type_of(scenario_reference)> scenario_itf;
				#pragma endregion
				//---------------------------------------------------------------------------------------------

				//=============================================================================================
				// Loop through each region/zone and write synthesized agents to database
				regions_itf* regions = this->Synthesis_Regions_Collection<regions_itf*>();
				scenario_itf* scenario = this->scenario_reference<scenario_itf*>();
				
				// EXIT if no request to write the demand to database
				if (!scenario->template write_demand_to_database<bool>()) return;
	
				long uuid = 1;
				long perid = 1;

				try
				{
					// Start database transaction
					unique_ptr<odb::database> db (open_sqlite_database_single<unique_ptr<odb::database> >(Get_Output_DB_Name<scenario_itf*>(scenario)));
					odb::transaction t(db->begin());
				

					typename regions_itf::iterator r_itr;
					typename zones_itf::iterator z_itr;
					typename household_collection_itf::iterator p_itr;
					int counter = 0;

					// Loop through all regions
					for (r_itr = regions->begin(); r_itr != regions->end(); ++r_itr)
					{
						region_itf* region = r_itr->second;
						zones_itf* zones = region->template Synthesis_Zone_Collection<zones_itf*>();
						// loop through zones in each region
						for (z_itr = zones->begin(); z_itr != zones->end(); ++z_itr)
						{
							zone_itf* zone = z_itr->second;
							
							// loop through each synthesized person
							household_collection_itf* households = zone->template Synthetic_Households_Container<household_collection_itf*>();
							for (p_itr = households->begin(); p_itr != households->end(); ++p_itr)
							{
								household_type* hh = (household_type*)*p_itr;

								// create household record using the ACS properties
								shared_ptr<MasterType::hh_db_rec_type> hh_rec(new MasterType::hh_db_rec_type());
								hh_rec->setHhold(uuid);
								Set_HH_Record_Location<shared_ptr<MasterType::hh_db_rec_type>,household_type*,zone_itf*>(hh_rec,hh,zone);
								Fill_HH_Record<typename get_type_of(network_reference),shared_ptr<MasterType::hh_db_rec_type>,household_type*,zone_itf*>(hh_rec,hh,zone);

								//push to database
								db->persist(hh_rec);

								person_collection_itf* persons = hh->template Persons_Container<person_collection_itf*>();
								perid=1;
								for (typename person_collection_itf::iterator p_itr = persons->begin(); p_itr != persons->end(); ++p_itr)
								{		
									// update synthesizing persons counter
									if (counter % 10000 == 0) cout << '\r' << "Writing Agents to database:           " << counter;

									person_type* person = (person_type*)(*p_itr);

									shared_ptr<MasterType::person_db_rec_type> per_rec(new MasterType::person_db_rec_type());
									per_rec->setId(perid);
									per_rec->setHousehold(hh_rec);
									Set_Person_Record_Locations<shared_ptr<MasterType::person_db_rec_type>, person_type*,zone_itf*>(per_rec,person,zone);
									Fill_Person_Record<typename get_type_of(network_reference),shared_ptr<MasterType::person_db_rec_type>,person_type*,zone_itf*>(per_rec,person,zone);
									
									//push to database
									db->persist(per_rec);
									counter++;
									++perid;
								}

								++uuid;
								++perid;

							}
						}
					}
					t.commit();
				}
				catch (odb::sqlite::database_exception ex)
				{
					cout << endl << ex.what()<<". DB error in popsyn_prototype.h, line 954."<<endl;
				}

				cout << endl<<"Results output runtime (s): " << this->timer<Counter&>().Stop()/1000.0<<endl;
			}
			template<typename ScenarioType> string Get_Output_DB_Name(ScenarioType scenario, requires(ScenarioType,check(typename get_type_of(network_reference), Network_Components::Concepts::Is_Transportation_Network) && check(ScenarioType, is_pointer)))
			{
				string name(scenario->template output_demand_database_name<string&>());
				return name;
			}
			template<typename ScenarioType> string Get_Output_DB_Name(ScenarioType scenario, requires(ScenarioType,!check(typename get_type_of(network_reference), Network_Components::Concepts::Is_Transportation_Network) && check(ScenarioType, is_pointer)))
			{
				string name(scenario->template output_popsyn_database_name<string&>());
				return name;
			}
			template<typename NetworkType, typename HHRecType, typename HHType, typename ZoneType> void Fill_HH_Record(HHRecType hh_rec, HHType hh, ZoneType zone, requires(NetworkType,check(NetworkType, Network_Components::Concepts::Is_Transportation_Network)))
			{
				typedef Network_Components::Prototypes::Network<typename get_type_of(network_reference)> network_itf;
				typedef Random_Access_Sequence<typename network_itf::get_type_of(activity_locations_container)> activity_locations_itf;
				typedef Activity_Location_Components::Prototypes::Activity_Location<typename get_component_type(activity_locations_itf)>  activity_location_itf;
				typedef Household_Components::Prototypes::Household_Properties<typename strip_modifiers(HHType)::get_type_of(Static_Properties)> household_itf;

				Fill_HH_Record<NT,HHRecType,household_itf*,ZoneType>(hh_rec,hh->Static_Properties<household_itf*>(),zone);
			}
			template<typename NetworkType, typename HHRecType, typename HHType, typename ZoneType> void Fill_HH_Record(HHRecType hh_rec, HHType hh, ZoneType zone, requires(NetworkType,!check(NetworkType, Network_Components::Concepts::Is_Transportation_Network)))
			{
				hh_rec->setPersons(hh->template Household_size<int>());
				hh_rec->setWorkers(hh->template Number_of_workers<int>());
				hh_rec->setVehicles(hh->template Number_of_vehicles<int>());
				hh_rec->setIncome(hh->Income<Basic_Units::Currency_Variables::Dollars>());
				hh_rec->setType(hh->Household_type<int>());
			}
			template<typename HHRecType, typename HHType, typename ZoneType> void Set_HH_Record_Location(HHRecType hh_rec, HHType hh, ZoneType zone, requires(ZoneType,check(typename get_type_of(network_reference), Network_Components::Concepts::Is_Transportation_Network)))
			{
				typedef Network_Components::Prototypes::Network<typename get_type_of(network_reference)> network_itf;
				typedef Random_Access_Sequence<typename network_itf::get_type_of(activity_locations_container)> activity_locations_itf;
				typedef Activity_Location_Components::Prototypes::Activity_Location<typename get_component_type(activity_locations_itf)>  activity_location_itf;
				typedef Household_Components::Prototypes::Household<typename strip_modifiers(HHType)> household_itf;
				
				household_itf* hh_itf = (household_itf*)hh;
				hh_rec->setLocation(hh_itf->Home_Location<activity_location_itf*>()->template uuid<int>());
			}
			template<typename HHRecType, typename HHType, typename ZoneType> void Set_HH_Record_Location(HHRecType hh_rec, HHType hh, ZoneType zone, requires(ZoneType,!check(typename get_type_of(network_reference), Network_Components::Concepts::Is_Transportation_Network)))
			{
				hh_rec->setLocation(zone->ID<long long>());
			}
			template<typename NetworkType, typename PerRecType, typename PerType, typename ZoneType> void Fill_Person_Record(PerRecType per_rec, PerType person, ZoneType zone, requires(NetworkType,check(NetworkType, Network_Components::Concepts::Is_Transportation_Network)))
			{
				typedef Network_Components::Prototypes::Network<typename get_type_of(network_reference)> network_itf;
				typedef Random_Access_Sequence<typename network_itf::get_type_of(activity_locations_container)> activity_locations_itf;
				typedef Activity_Location_Components::Prototypes::Activity_Location<typename get_component_type(activity_locations_itf)>  activity_location_itf;
				typedef Person_Components::Prototypes::Person_Properties<typename strip_modifiers(PerType)::get_type_of(Static_Properties)> person_itf;

				Fill_Person_Record<NT,PerRecType,person_itf*,ZoneType>(per_rec,person->Static_Properties<person_itf*>(),zone);
				person->template person_record<shared_ptr<MasterType::person_db_rec_type>>(per_rec);
			}
			template<typename NetworkType, typename PerRecType, typename PerType, typename ZoneType> void Fill_Person_Record(PerRecType per_rec, PerType person, ZoneType zone, requires(NetworkType,!check(NetworkType, Network_Components::Concepts::Is_Transportation_Network)))
			{
				per_rec->setAge(person->Age<int>());
				per_rec->setEducation(person->Educational_Attainment<int>());
				per_rec->setEmployment(person->Employment_Status<int>());
				per_rec->setGender(person->Gender<int>());
				per_rec->setIncome(person->Income<Dollars>());
				per_rec->setIndustry(person->Employment_Industry<int>());
				per_rec->setJourney_to_work_arrival_time(person->Journey_To_Work_Arrival_Time<Time_Minutes>());
				per_rec->setJourney_to_work_mode(person->Journey_To_Work_Mode<int>());
				per_rec->setJourney_to_work_travel_time(person->Journey_To_Work_Travel_Time<Time_Minutes>());
				per_rec->setJourney_to_work_vehicle_occupancy(person->Journey_To_Work_Vehicle_Occupancy<int>());
				per_rec->setMarital_status(person->Marital_Status<int>());
				per_rec->setRace(person->Race<int>());
				per_rec->setSchool_enrollment(person->School_Enrollment<int>());
				per_rec->setSchool_grade_level(person->School_Grade_Level<int>());
				per_rec->setWorker_class(person->Class_of_worker<int>());
				per_rec->setWork_hours(person->Work_Hours<Time_Hours>());
			}
			template<typename PerRecType, typename PerType, typename ZoneType> void Set_Person_Record_Locations(PerRecType per_rec, PerType person, ZoneType zone, requires(ZoneType,check(typename get_type_of(network_reference), Network_Components::Concepts::Is_Transportation_Network)))
			{
				typedef Network_Components::Prototypes::Network<typename get_type_of(network_reference)> network_itf;
				typedef Random_Access_Sequence<typename network_itf::get_type_of(activity_locations_container)> activity_locations_itf;
				typedef Activity_Location_Components::Prototypes::Activity_Location<typename get_component_type(activity_locations_itf)>  activity_location_itf;
				typedef Person_Components::Prototypes::Person<typename strip_modifiers(PerType)> person_itf;
				person_itf* per_itf = (person_itf*)person;

				if (per_itf->template School_Location<int>() >= 0)
					per_rec->setSchool_Location_Id(per_itf->template School_Location<activity_location_itf*>()->template uuid<int>());
				else
					per_rec->setSchool_Location_Id(0);
				if (per_itf->template Work_Location<int>() >= 0)
					per_rec->setWork_Location_Id(per_itf->template Work_Location<activity_location_itf*>()->template uuid<int>());
				else
					per_rec->setWork_Location_Id(0);
			}
			template<typename PerRecType, typename PerType, typename ZoneType> void Set_Person_Record_Locations(PerRecType per_rec, PerType person, ZoneType zone, requires(ZoneType,!check(typename get_type_of(network_reference), Network_Components::Concepts::Is_Transportation_Network)))
			{
				per_rec->setSchool_Location_Id(0);
				per_rec->setWork_Location_Id(0);
			}

			//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
			// Required Features - necessary for any synthesis routine
			//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
			accessor(Synthesis_Regions_Collection, NONE, NONE);
			accessor(Solution_Settings, NONE, NONE);
			accessor(scenario_reference, NONE, NONE);
			accessor(network_reference, NONE, NONE);
			accessor(file_linker, NONE,NONE);
			accessor(timer, NONE, NONE);
			

			//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
			// Output features - optional, used to write intermediate and final results
			//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
			accessor(Output_Stream,check_2(strip_modifiers(TargetType),ofstream, is_same), check_2(strip_modifiers(TargetType),ofstream, is_same));
			accessor(Marginal_Output_Stream,check_2(strip_modifiers(TargetType),ofstream, is_same), check_2(strip_modifiers(TargetType),ofstream, is_same));
			accessor(Log_File,check_2(strip_modifiers(TargetType),ofstream, is_same), check_2(strip_modifiers(TargetType),ofstream, is_same));
		};
	}
}

