#pragma once

//#include "Person_Prototype.h"
//#include "Household_Prototype.h"
#include "traffic_simulator\User_Space_Includes.h"
#include "Popsyn_File_Linker_Implementations.h"
//#include "Synthesis_Region_Implementation.h"
#include "Network_Prototype.h"
//#include "Scenario_Prototype.h"


namespace PopSyn
{
	namespace Types
	{

	}

	namespace Concepts
	{
		concept struct Uses_Linker_File
		{
			check_data_member_name(Has_Use_Linker_File_Defined, _linker_file_path);
			define_default_check(Has_Use_Linker_File_Defined);
		};
		concept struct Scenario_Has_Popsyn_Configuration_Data
		{
			check_template_method_name(has_percent_to_synthesize_p, Component_Type::percent_to_synthesize);
			check_template_method_name(has_ipf_tolerance_p, Component_Type::ipf_tolerance);
			check_template_method_name(has_marginal_tolerance_p, Component_Type::marginal_tolerance);
			check_template_method_name(has_maximum_iterations_p, Component_Type::maximum_iterations);
			check_template_method_name(has_write_marginal_output_p, Component_Type::write_marginal_output);
			check_template_method_name(has_write_full_output_p, Component_Type::write_full_output);
			check_template_method_name(has_popsyn_control_file_name_p, Component_Type::popsyn_control_file_name);

			check_template_method_name(has_percent_to_synthesize, percent_to_synthesize);
			check_template_method_name(has_ipf_tolerance, ipf_tolerance);
			check_template_method_name(has_marginal_tolerance, marginal_tolerance);
			check_template_method_name(has_maximum_iterations, maximum_iterations);
			check_template_method_name(has_write_marginal_output,write_marginal_output);
			check_template_method_name(has_write_full_output,write_full_output);
			check_template_method_name(has_popsyn_control_file_name,popsyn_control_file_name);
			define_sub_check(Has_Popsyn_Configuration_Data_Prototype, has_percent_to_synthesize_p && has_ipf_tolerance_p && has_marginal_tolerance_p && has_maximum_iterations_p && has_write_marginal_output_p && has_write_full_output_p && has_popsyn_control_file_name_p);
			define_sub_check(Has_Popsyn_Configuration_Data_Component, has_percent_to_synthesize && has_ipf_tolerance && has_marginal_tolerance && has_maximum_iterations && has_write_marginal_output && has_write_full_output && has_popsyn_control_file_name);
			define_default_check(Has_Popsyn_Configuration_Data_Prototype || Has_Popsyn_Configuration_Data_Component);
		};
	}

	namespace Prototypes
	{
		prototype struct Population_Synthesizer ADD_DEBUG_INFO
		{
			tag_as_prototype; // Declare class as a polaris prototype

			//----------------------------------------------------------------
			// Initializers, with and without setting a network reference.  If the version without networktype is used the events which require network information are skipped
			template<typename ScenarioType> void Initialize(ScenarioType scenario, requires(ScenarioType, check_stripped_type(ScenarioType, Concepts::Scenario_Has_Popsyn_Configuration_Data)))
			{
				// Allocate IPF Solver and get Settings from scenario reference
				typedef PopSyn::Prototypes::Solver_Settings<typename get_type_of(Solution_Settings)> solver_itf;
				solver_itf* solver = (solver_itf*)Allocate<typename get_type_of(Solution_Settings)>();
				solver->template Initialize<double>(scenario->template ipf_tolerance<double>(),scenario->template percent_to_synthesize<double>(),scenario->template maximum_iterations<int>());

				// get output control params from scenario
				this->write_marginal_output_flag<bool>(scenario->template write_marginal_output<bool>());
				this->write_full_output_flag<bool>(scenario->template write_full_output<bool>());
				this->linker_file_path<string>(scenario->template popsyn_control_file_name<string>());

				// create file linker and initialize with the filepath
				typedef PopSyn::Prototypes::Popsyn_File_Linker<typename get_type_of(file_linker)> linker_itf;
				linker_itf* linker = (linker_itf*)Allocate<typename get_type_of(file_linker)>();
				linker->Initialize(this->linker_file_path<string>());

				// Add references to other objects to the population synthesizer
				this->Solution_Settings<solver_itf*>(solver);
				this->scenario_reference<ScenarioType>(scenario);
				this->network_reference<Null_Prototype<get_type_of(network_reference)>*>(nullptr);

				// set up output files
				stringstream pop_filename("");
				pop_filename << scenario->template output_dir_name<string>();
				pop_filename << "full_population.csv";
				this->Output_Stream<ofstream&>().open(pop_filename.str(),ios_base::out);

				stringstream log_filename("");
				log_filename << scenario->template output_dir_name<string>();
				log_filename << "popsyn_log.csv";
				this->Log_File<ofstream&>().open(log_filename.str(),ios_base::out);

				stringstream marg_filename("");
				marg_filename << scenario->template output_dir_name<string>();
				marg_filename << "marginals_and_distributions.csv";
				this->Log_File<ofstream&>().open(marg_filename.str(),ios_base::out);		
		

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
			

			//----------------------------------------------------------------
			// Main analysis loop events, used to separate operations into different timesteps
			//----------------------------------------------------------------
			static void Popsyn_Event_Controller(ComponentType* _this,Event_Response& response)
			{
				Population_Synthesizer<ComponentType>* pthis = (Population_Synthesizer<ComponentType>*)_this;

				if (iteration() == 0 || iteration() == 1)
				{
					switch (sub_iteration())
					{
					case POPSYN_SUBITERATIONS::INITIALIZE:
						pthis->Start_Popsyn_Event<NT>();
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
						pthis->Output_Popsyn_Event<NT>();
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
					//pthis->Output_Popsyn_To_DB_Event<NT>();
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
			template<typename TargetType> bool Start_Popsyn_Event(requires(TargetType,check_stripped_type(ComponentType,Concepts::Uses_Linker_File)))
			{
				//------------------------
				// TIMER
				cout<<endl<<"====================================================="<<endl<<"Starting synthetic population generation:"<<endl;
				Counter timer;
				timer.Start();
				//------------------------


//				File_IO::File_Writer fw, fw_sample;
				ofstream& out = this->Output_Stream<ofstream&>();
				
				//===============================================================================================================
				// Initialize settings
				//---------------------------------------------------------------------------------------------------------------
				// CREATE RNG for later use
				typedef RNG<typename ComponentType::Master_Type::RNG> Rand_Interface;
				Rand_Interface* rand = (Rand_Interface*)Allocate<typename ComponentType::Master_Type::RNG>();
				rand->template Initialize<double>(0.0);

				// IPF Solver Settings
				typedef PopSyn::Prototypes::Solver_Settings<typename get_type_of(Solution_Settings)> solver_itf;
				solver_itf* solver = this->Solution_Settings<solver_itf*>();

				//===============================================================================================================
				// Initialize file linker
				//---------------------------------------------------------------------------------------------------------------
				int ndim;
				typedef PopSyn::Prototypes::Popsyn_File_Linker<typename get_type_of(file_linker)> linker_itf;
				linker_itf* linker = this->file_linker<linker_itf*>();
				ndim = linker->number_of_dimensions();
				boost::container::vector<int>& dims = linker->dimension_sizes();

				// Define iterators and get pointer to the region collection
				typedef typename get_type_of(Synthesis_Regions_Collection)						region_collection_type;
				typedef typename strip_modifiers(region_collection_type::mapped_type)			region_type;
				typedef typename region_type::Sample_Data_type									sample_collection_type;
				typedef typename strip_modifiers(sample_collection_type::mapped_type)			sample_type;
				typedef typename region_type::Temporary_Sample_Data_type						temporary_sample_collection_type;
				typedef typename strip_modifiers(temporary_sample_collection_type::mapped_type)	temp_sample_type;
				typedef typename region_type::Synthesis_Zone_Collection_type					zone_collection_type;
				typedef typename strip_modifiers(zone_collection_type::mapped_type)				zone_type;
				typedef typename region_type::get_type_of(Target_Joint_Distribution)			joint_dist_type;
				typedef typename region_type::get_type_of(Target_Marginal_Distribution)			marg_dist_type;

				typedef PopSyn::Prototypes::Synthesis_Region<region_type> region_itf;
				typedef Pair_Associative_Container<region_collection_type,region_collection_type::key_type, region_itf*> regions_itf;

				typedef PopSyn::Prototypes::Synthesis_Zone<zone_type> zone_itf;
				typedef Pair_Associative_Container<zone_collection_type,zone_collection_type::key_type,zone_itf*> zones_itf;

				typedef Household_Components::Prototypes::Household_Properties<sample_type> pop_unit_itf;
				typedef Pair_Associative_Container<sample_collection_type,sample_collection_type::key_type,pop_unit_itf*> sample_data_itf;

				typedef Household_Components::Prototypes::Household_Properties<temp_sample_type> temp_pop_unit_itf;
				typedef Pair_Associative_Container<temporary_sample_collection_type,temporary_sample_collection_type::key_type,temp_pop_unit_itf*> temp_sample_data_itf;

				typedef Person_Components::Prototypes::Person_Properties<typename remove_pointer<typename pop_unit_itf::get_type_of(Persons_Container)::value_type>::type> person_unit_itf;
				typedef Random_Access_Sequence<typename pop_unit_itf::get_type_of(Persons_Container),person_unit_itf*> person_sample_data_itf;

				typedef Multidimensional_Random_Access_Array<joint_dist_type, typename joint_dist_type::value_type > joint_itf;
				typedef Multidimensional_Random_Access_Array<marg_dist_type, typename marg_dist_type::value_type > marginal_itf;
	
				
				regions_itf* regions = this->Synthesis_Regions_Collection<regions_itf*>();
				typename regions_itf::iterator region_itr;
				this->Synthesis_Regions_Collection<regions_itf*>(regions);

				typename joint_itf::index_type dimensions;
				typename joint_itf::index_type index;
				for (boost::container::vector<int>::iterator i = dims.begin(); i != dims.end(); ++i) 
				{
					dimensions.push_back(*i);
					index.push_back(0);
				}


				//===============================================================================================================
				// read region household file, fill sample data
				//---------------------------------------------------------------------------------------------------------------
				File_IO::File_Reader fr;
				if (!fr.Open(linker->sample_file_path(),true,",\t")) 
				{
					char ans;
					cout<<endl<<"Could not open region sample file path. Press any key."<<endl; cin>>ans; return EXIT_FAILURE;
				}

				// references to the various region components
				region_itf* new_region;
				joint_itf* dist;
				marginal_itf* marg,  *regional_marg;
				sample_data_itf* sample;
				temp_sample_data_itf* temp_sample;

				while(fr.Read())
				{
					typename regions_itf::key_type ID;
		
					// read the region data from input file
					if(!fr.Get_Data<typename regions_itf::key_type>(ID,linker->region_id_column())) break; 

					if ((region_itr = regions->find(ID)) == regions->end())
					{
						// create new region
						new_region = (region_itf*)Allocate<region_type>();
						new_region->template Initialize<NULLTYPE>();
						new_region->parent_reference(this);
						dist = new_region->template Target_Joint_Distribution<joint_itf*>();
						marg = new_region->template Target_Marginal_Distribution<marginal_itf*>();
						Rand_Interface* my_rand = (Rand_Interface*)Allocate<typename zone_itf::get_type_of(Rand)>(); // ALLOCATION TEST
						my_rand->template Initialize<double>(ID);
						new_region->template Output_Stream<ostream&>(out);
						((zone_itf*)new_region)->template Rand<Rand_Interface*>(my_rand);

						//-----------------------------------------------------------------------------------------
						// Initialize the distribution and marginals
						dist->resize(dimensions,0.0);
						marg->resize(dimensions,0.0);

						new_region->template ID<int>(ID);
						solver_itf* region_solver = (solver_itf*)Allocate<typename get_type_of(Solution_Settings)>(); // ALLOCATION TEST

						region_solver->template Initialize<double>(solver->template Tolerance<double>(),solver->template Percentage_to_synthesize<double>(),solver->template Iterations<double>());
						new_region->template Solver_Settings<solver_itf*>(region_solver);

						// add new region to the boost::container::list
						pair<typename regions_itf::key_type, region_itf*> item = pair<typename regions_itf::key_type, region_itf*>(ID, new_region);
						regions->insert(item);
					}
					else
					{
						new_region = (region_itf*)region_itr->second;
					}
					// next add the sample data
					dist = new_region->template Target_Joint_Distribution<joint_itf*>();
					marg = new_region->template Target_Marginal_Distribution<marginal_itf*>();
					sample = new_region->template Sample_Data<sample_data_itf*>();

					
					double x;
					for (int i=0; i<ndim; i++)
					{
						fr.Get_Data<double>(x,linker->get_pums_column(i));
						index[i] = linker->find_index_in_dimension(i,x);
					}

					typename sample_type::ID_type sample_id;
					typename sample_type::Weight_type weight;
					fr.Get_Data<typename sample_type::ID_type>(sample_id,linker->sample_id_column());
					fr.Get_Data<typename sample_type::Weight_type>(weight, linker->sample_weight_column());
					boost::container::vector<double> data;
					fr.Get_Data<double>(data,linker->get_pums_data_columns());

					pop_unit_itf* p = (pop_unit_itf*)Allocate<sample_type>();
					p->ID(sample_id);
					p->Index(new_region->template Get_1D_Index<typename joint_itf::index_type,typename joint_itf::size_type>(index));
					p->Weight(weight);
					p->template Characteristics<boost::container::vector<double>*>(&data);

					// Update the sample and joint distribution with the current population unit
					sample->insert(p->template Index<typename sample_collection_type::key_type>(), p);
					(*dist)[index] += weight;
					
					// also add to temporary sample data, so that the person file records can attach to household records
					//temp_sample->insert((typename temporary_sample_collection_type::key_type)sample_id,p);
					pair<typename temporary_sample_collection_type::key_type,pop_unit_itf*> item = pair<typename temporary_sample_collection_type::key_type,pop_unit_itf*>(sample_id,p);
					new_region->template Temporary_Sample_Data<temp_sample_data_itf*>()->insert(item);
				}
				fr.Close();



				//===============================================================================================================
				// read region person file, fill sample data
				//---------------------------------------------------------------------------------------------------------------
				if (!fr.Open(linker->person_sample_file_path(),true,",\t")) 
				{
					char ans;
					cout<<endl<<"Could not open person file path. Press any key."<<endl; cin>>ans; return EXIT_FAILURE;
				}

				// references to the various region components
				typename temp_sample_data_itf::iterator sample_itr;

				while(fr.Read())
				{
					typename regions_itf::key_type ID;
		
					// read the region data from input file
					if(!fr.Get_Data<typename regions_itf::key_type>(ID,linker->person_region_id_column())) break; 

					if ((region_itr = regions->find(ID)) == regions->end())
					{
						THROW_WARNING("Sample from person file has non-existent region id.");
					}
					else
					{
						new_region = (region_itf*)region_itr->second;

						// next add the sample data
						temp_sample = new_region->template Temporary_Sample_Data<temp_sample_data_itf*>();

						// get characteristics from file
						typename person_unit_itf::get_type_of(ID) sample_id;
						fr.Get_Data<typename person_unit_itf::get_type_of(ID)>(sample_id,linker->person_sample_id_column());
						boost::container::vector<double> data;
						fr.Get_Data<double>(data,linker->get_person_data_columns());

						// create new person unit
						person_unit_itf* p = (person_unit_itf*)Allocate<typename person_unit_itf::Component_Type>();
						p->ID(sample_id);
						p->template Characteristics<boost::container::vector<double>*>(&data);

						// find the household that the person belongs to and add
						sample_itr = temp_sample->find(sample_id);
						if (sample_itr != temp_sample->end())
						{
							pop_unit_itf* pop_unit = (pop_unit_itf*)sample_itr->second;
							pop_unit->template Persons_Container<person_sample_data_itf*>()->push_back(p);
						}
						else
						{
							THROW_WARNING("Warning: person sample unit refers to non-existent household sample unit, observation ignored.");
						}
						
					}
				}
				fr.Close();



				//===============================================================================================================
				// Read zone file, fill marginal data
				//---------------------------------------------------------------------------------------------------------------
				File_IO::File_Reader zone_fr;
				if (!zone_fr.Open(linker->marg_file_path(),true,",\t")) 
				{
					char ans;
					cout<<endl<<"Ending. Press any key."<<endl;	cin>>ans; return EXIT_FAILURE;
				}
				while(zone_fr.Read())
				{
					typename zone_type::ID_type ID;
					typename regions_itf::key_type RID;
					double x;
		
					// get ID values for ZONE and REGION
					if(!zone_fr.Get_Data<typename zone_type::ID_type>(ID,linker->zone_id_column()))
					{
						cout<<"ERROR: could not retrieve zone id from zone file"; return EXIT_FAILURE;
					}
					if(!zone_fr.Get_Data<typename regions_itf::key_type>(RID,linker->region_in_zone_id_column()))
					{
						cout<<"ERROR: could not retrieve region id from zone file"; return EXIT_FAILURE;
					}

					// get region iterator from hash map
					if ((region_itr = regions->find(RID))==regions->end())
					{
						cout<<"ERROR: Region ID not found in map.  Press a key to end.";
						cin>>RID;
						return EXIT_FAILURE;
					}
					region_itf* region = (region_itf*)region_itr->second;
					regional_marg = region->template Target_Marginal_Distribution<marginal_itf*>();

					// Read marginal data from file and add to ZONE
					zone_itf* zone = (zone_itf*)Allocate<zone_type>();
					zone->template Initialize<NULLTYPE>();
					zone->parent_reference(region);

					zone->ID(ID);
					solver_itf* zone_solver = (solver_itf*)Allocate<typename get_type_of(Solution_Settings)>(); 

					zone_solver->Initialize<double>(solver->template Tolerance<double>(),solver->template Percentage_to_synthesize<double>(), solver->template Iterations<double>());
					zone->template Solver_Settings<solver_itf*>(zone_solver);
					joint_itf* mway = zone->template Target_Joint_Distribution<joint_itf*>();
					marginal_itf* marg = zone->template Target_Marginal_Distribution<marginal_itf*>();
					mway->resize(dimensions,0);
					marg->resize(dimensions,0);
					Rand_Interface* my_rand = (Rand_Interface*)Allocate<typename zone_itf::get_type_of(Rand)>();
					my_rand->template Initialize<double>(rand->template Next_Rand<double>()*(double)SHRT_MAX);
					

					zone->template Rand<Rand_Interface*>(my_rand);

					for (typename marginal_itf::size_type i=0; i<dimensions.size(); i++)
					{
						for (typename marginal_itf::size_type j=0; j<dimensions[i]; j++)
						{
							if (!zone_fr.Get_Data<double>(x,linker->get_sf3_column(i,j))) break;
							(*marg)[pair<typename marginal_itf::size_type,typename marginal_itf::size_type>(i,j)] = x;
							(*regional_marg)[pair<typename marginal_itf::size_type,typename marginal_itf::size_type>(i,j)] += x;
						}

					}
					pair<typename zone_type::ID_type,zone_itf*> item = pair<typename zone_type::ID_type,zone_itf*>(ID,zone);
					region->template Synthesis_Zone_Collection<zones_itf*>()->insert(item);
				}
				zone_fr.Close();

				//===============================================================================================================
				// Link the synthesizer zones to the activity locations in the network_reference, if it exists, otherwise ignore
				this->Link_Zones_To_Network_Locations<NT>();				
				//---------------------------------------------------------------------------------------------------------------

				//------------------------
				// TIMER
				cout <<"Setup Runtime (ms): "<<timer.Stop();
				//------------------------

				return true;
			}
			template<typename TargetType> bool Start_Popsyn_Event(requires(TargetType,check_stripped_type(ComponentType,!Concepts::Uses_Linker_File)))
			{
				assert_check(ComponentType,Concepts::Uses_Linker_File,"This popsyn type does not use linker file setup.");
			}
			template<typename NetworkType> bool Link_Zones_To_Network_Locations(requires(ComponentType,check(NetworkType, Network_Components::Concepts::Is_Transportation_Network)))
			{
				//===============================================================================================================
				// Fill zonal activity_locations boost::container::list from network reference
				//---------------------------------------------------------------------------------------------------------------
				typedef Network_Components::Prototypes::Network<typename get_type_of(network_reference)> network_itf;
				typedef Activity_Location_Components::Prototypes::Activity_Location<typename remove_pointer<typename network_itf::get_type_of(activity_locations_container)::value_type>::type>  location_itf;
				typedef Random_Access_Sequence<typename network_itf::get_type_of(activity_locations_container), location_itf*> locations_container_itf;

				typedef Random_Access_Sequence<typename zone_itf::get_type_of(Activity_Locations_Container),int> location_id_container_itf;
				network_itf* network = this->network_reference<network_itf*>();
				locations_container_itf* locations = network->template activity_locations_container<locations_container_itf*>();
				location_itf* location;
				region_itf* region;
				zone_itf* zone;
				typename locations_container_itf::iterator loc_itr;
				typename zones_itf::iterator zone_itr;

				for (loc_itr = locations->begin(); loc_itr != locations->end(); ++loc_itr)
				{
					location = *loc_itr;
					long long zone_id = location->template census_zone_id<long long >();
					
					// ignore non-residential land use locations
					if (!location->is_residential_location<bool>()) continue;

					for (region_itr = regions->begin(); region_itr != regions->end(); ++region_itr)
					{
						region = region_itr->second;

						zones_itf* zones = region->template Synthesis_Zone_Collection<zones_itf*>();
						
						zone_itr = zones->find(zone_id);

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
			template<typename NetworkType> bool Link_Zones_To_Network_Locations(requires(ComponentType,!check(NetworkType, Network_Components::Concepts::Is_Transportation_Network)))
			{
				return false;
			}

			// 2.) Start timing event - called before individual objects begin processing (at Iteration = 0, timestep 3)
			template<typename TargetType> void Start_Timer_Event()
			{
				this->timer<Counter&>().Start();
			}
			
			// 3.) Stop timing event - called after individual objects end processing (at Iteration = 0, timestep 5)
			template<typename TargetType> void Stop_Timer_Event()
			{
				cout << endl<<"Main Algorithm run-time: " << this->timer<Counter&>().Stop();
				//cout << endl<<"freq: " << this->timer<Counter&>().get_freq_value() << ", start: "<<this->timer<Counter&>().get_start_value() <<", l: "<<this->timer<Counter&>().get_l_value();
			}
			
			// 4.) Output results event - called after timing is stopped (at Iteration = 0, timestep 7)
			template<typename TargetType> void Output_Popsyn_Event()
			{
				ofstream& sample_out = this->Output_Stream<ofstream&>();
				ofstream& marg_out = this->Marginal_Output_Stream<ofstream&>();
				ofstream& popsyn_log = this->Log_File<ofstream&>();

				//---------------------------------------------------------------------------------------------
				// Type defines for sub_objects
				typedef typename get_type_of(Synthesis_Regions_Collection)				region_collection_type;
				typedef typename strip_modifiers(region_collection_type::mapped_type)	region_type;
				typedef typename region_type::Sample_Data_type							sample_collection_type;
				typedef typename strip_modifiers(sample_collection_type::mapped_type)	sample_type;
				typedef typename region_type::Synthesis_Zone_Collection_type			zone_collection_type;
				typedef typename strip_modifiers(zone_collection_type::mapped_type)		zone_type;
				typedef typename zone_type::get_type_of(Synthetic_Households_Container)	household_collection_type;
				typedef typename strip_modifiers(household_collection_type::value_type) household_type;
				typedef typename region_type::get_type_of(Target_Joint_Distribution)	joint_dist_type;
				typedef typename region_type::get_type_of(Target_Marginal_Distribution)	marg_dist_type;
				//---------------------------------------------------------------------------------------------
				// Interface defines for sub_objects
				typedef PopSyn::Prototypes::Synthesis_Region<region_type> region_itf;
				typedef Pair_Associative_Container<region_collection_type,region_collection_type::key_type, region_itf*> regions_itf;

				typedef PopSyn::Prototypes::Synthesis_Zone<zone_type> zone_itf;
				typedef Pair_Associative_Container<zone_collection_type,zone_collection_type::key_type,zone_itf*> zones_itf;

				typedef Household_Components::Prototypes::Household_Properties<household_type> pop_unit_itf;
				typedef Pair_Associative_Container<sample_collection_type,sample_collection_type::key_type, pop_unit_itf*> sample_data_itf;

				typedef Person_Components::Prototypes::Person_Properties<typename remove_pointer<typename pop_unit_itf::get_type_of(Persons_Container)::value_type>::type> person_unit_itf;
				typedef Random_Access_Sequence<typename pop_unit_itf::get_type_of(Persons_Container),person_unit_itf*> person_sample_data_itf;

				typedef Multidimensional_Random_Access_Array<joint_dist_type, typename joint_dist_type::value_type > joint_itf;
				typedef Multidimensional_Random_Access_Array<marg_dist_type, typename marg_dist_type::value_type > marginal_itf;
				typedef Household_Components::Prototypes::Household<typename remove_pointer<typename household_collection_type::value_type>::type>  household_itf;
				typedef Random_Access_Sequence<household_collection_type, household_itf*> household_collection_itf;

				typedef Person_Components::Prototypes::Person<typename remove_pointer<typename household_itf::get_type_of(Persons_Container)::value_type>::type>  person_itf;
				typedef Random_Access_Sequence<typename household_itf::get_type_of(Persons_Container), person_itf*> person_collection_itf;

				typedef Random_Access_Sequence<typename zone_itf::get_type_of(Activity_Locations_Container),int> activity_location_ids_itf;
				typedef Network_Components::Prototypes::Network<typename get_type_of(network_reference)> network_itf;
				typedef Scenario_Components::Prototypes::Scenario<typename get_type_of(scenario_reference)> scenario_itf;
				typedef  Activity_Location_Components::Prototypes::Activity_Location<typename remove_pointer<typename network_itf::get_type_of(activity_locations_container)::value_type>::type>  activity_location_itf;
				typedef Random_Access_Sequence<typename network_itf::get_type_of(activity_locations_container), activity_location_itf*> activity_locations_itf;

				typedef  Zone_Components::Prototypes::Zone<typename remove_pointer<typename network_itf::get_type_of(zones_container)::value_type>::type>  _Zone_Interface;
				typedef Pair_Associative_Container<typename network_itf::get_type_of(zones_container), _Zone_Interface*> _Zone_Container_Interface;

				
				regions_itf* regions = this->Synthesis_Regions_Collection<regions_itf*>();
				network_itf* network = this->network_reference<network_itf*>();
				scenario_itf* scenario = this->scenario_reference<scenario_itf*>();
				activity_locations_itf* activity_locations = network->template activity_locations_container<activity_locations_itf*>();
				

				typename regions_itf::iterator r_itr;
				typename zones_itf::iterator z_itr;
				typename household_collection_itf::iterator p_itr;

				// initialize all of the synthesized individuals and assign unique ids
				long uuid = 0; // globally unique person id

				int counter = 0;

				cout << endl << endl;

				// Loop through all regions
				for (r_itr = regions->begin(); r_itr != regions->end(); ++r_itr)
				{
					region_itf* region = r_itr->second;
					zones_itf* zones = region->template Synthesis_Zone_Collection<zones_itf*>();
					// loop through zones in each region
					for (z_itr = zones->begin(); z_itr != zones->end(); ++z_itr)
					{
						zone_itf* zone = z_itr->second;
						activity_location_ids_itf* loc_indices = zone->template Activity_Locations_Container<activity_location_ids_itf*>();

						// loop through each synthesized person
						household_collection_itf* households = zone->template Synthetic_Households_Container<household_collection_itf*>();
						for (p_itr = households->begin(); p_itr != households->end(); ++p_itr)
						{
							// update synthesizing persons counter
							if (counter % 10000 == 0) cout << '\r' << "Initializing Agents: " << counter;
							household_itf* hh = *p_itr;

							// initialize the hh - allocates all hh subcomponents
							hh->template Initialize<long,zone_itf*, network_itf*, scenario_itf*>(uuid, zone, network, scenario);

							person_collection_itf* persons = hh->template Persons_Container<person_collection_itf*>();

							long perid=0;
							for (typename person_collection_itf::iterator p_itr = persons->begin(); p_itr != persons->end(); ++p_itr)
							{		
								person_itf* person = (person_itf*)(*p_itr);
								person->template Initialize<long,zone_itf*, network_itf*, scenario_itf*>(perid, zone, network, scenario);
								++perid;
							}

							++uuid;
							++counter;
						}
					}
				}
				cout <<endl<<endl<<"Total Households Synthesized: "<<uuid<<endl<<endl;


				// Handle file output if needed
				if (pthis->write_marginal_output_flag<bool>() == true || pthis->write_full_output_flag<bool>() == true)
				{

					Counter timer;
					timer.Start();

					for (r_itr = regions->begin(); r_itr != regions->end(); ++r_itr)
					{
						region_itf* region = r_itr->second;
						zones_itf* zones = region->template Synthesis_Zone_Collection<zones_itf*>();
						for (z_itr = zones->begin(); z_itr != zones->end(); ++z_itr)
						{
							zone_itf* zone = z_itr->second;

							// write the marginal results
							if (pthis->write_marginal_output_flag<bool>() == true )
							{
								marg_out <<endl<<endl<<"ZONE_ID: "<<zone->template ID<long long int>();
								//zone->template Target_Joint_Distribution<joint_itf*>()->write(marg_out);
								marg_out <<endl;
								zone->template Target_Marginal_Distribution<marginal_itf*>()->write(marg_out);
								marg_out <<endl;
							}

							// write th full population results
							if (pthis->write_full_output_flag<bool>() == true)
							{
								household_collection_itf* sample = zone->template Synthetic_Households_Container<household_collection_itf*>();	
								household_itf* hh;
								for (typename household_collection_itf::iterator s_itr = sample->begin(); s_itr != sample->end(); ++s_itr)
								{
									hh = *s_itr;
									pop_unit_itf* p = hh->template Static_Properties<pop_unit_itf*>();
									sample_out << "ZONE_ID: "<<zone->template ID<long long int>() << endl << "ID: " << hh->template uuid<uint>() << ", ACS_ID: " << p->template ID<double>() << ",  weight: "<<p->template Weight<float>() <<", index: "<<p->template Index<uint>() << ", Gender: "<<p->template Household_type<Household_Components::Types::HHTYPE>();
								}
							}
						}
					}

					cout <<endl<<"File I/O Runtime: "<<timer.Stop();
				}
				sample_out.close();
				marg_out.close();
				popsyn_log.close();
			}
	
			// 5.) Write output to database (at Iteration 2)
			//declare_event(Output_Popsyn_To_DB_Event)
			//{
			//	Population_Synthesizer<ComponentType>* pthis = (Population_Synthesizer<ComponentType>*)_this;

			//	//---------------------------------------------------------------------------------------------
			//	// Type defines for sub_objects
			//	typedef typename get_type_of(Synthesis_Regions_Collection)				region_collection_type;
			//	typedef typename region_collection_type::value_type			region_type;
			//	typedef typename region_type::Sample_Data_type							sample_collection_type;
			//	typedef typename sample_collection_type::value_type			sample_type;
			//	typedef typename region_type::Synthesis_Zone_Collection_type			zone_collection_type;
			//	typedef typename zone_collection_type::value_type			zone_type;
			//	typedef typename zone_type::get_type_of(Synthetic_Households_Container)	household_collection_type;
			//	typedef typename household_collection_type::value_type		household_type;
			//	typedef typename region_type::get_type_of(Target_Joint_Distribution)	joint_dist_type;
			//	typedef typename region_type::get_type_of(Target_Marginal_Distribution)	marg_dist_type;
			//	//---------------------------------------------------------------------------------------------
			//	// Interface defines for sub_objects
			//	typedef PopSyn::Prototypes::Synthesis_Region<typename remove_pointer<typename region_collection_type::value_type>::type> region_itf;
			//	typedef Pair_Associative_Container<region_collection_type,region_itf*> regions_itf;

			//	typedef PopSyn::Prototypes::Synthesis_Zone<typename remove_pointer<typename zone_collection_type::value_type>::type> zone_itf;
			//	typedef Pair_Associative_Container<zone_collection_type,zone_itf*> zones_itf;

			//	typedef Household_Components::Prototypes::Household_Properties<typename remove_pointer<typename sample_collection_type::value_type>::type> pop_unit_itf;
			//	typedef Pair_Associative_Container<sample_collection_type,pop_unit_itf*> sample_data_itf;

			//	typedef Household_Components::Prototypes::Household_Properties<typename remove_pointer<typename pop_unit_itf::get_type_of(Persons_Container)::value_type>::type> person_unit_itf;
			//	typedef Random_Access_Sequence<typename pop_unit_itf::get_type_of(Persons_Container),person_unit_itf*> person_sample_data_itf;

			//	typedef Multidimensional_Random_Access_Array<joint_dist_type, typename joint_dist_type::value_type > joint_itf;
			//	typedef Multidimensional_Random_Access_Array<marg_dist_type, typename marg_dist_type::value_type > marginal_itf;
			//	typedef Household_Components::Prototypes::Household<typename remove_pointer<typename household_collection_type::value_type>::type>  household_itf;
			//	typedef Random_Access_Sequence<household_collection_type, household_itf*> household_collection_itf;

			//	typedef Person_Components::Prototypes::Person<typename remove_pointer<typename household_itf::get_type_of(Persons_Container)::value_type>::type>  person_itf;
			//	typedef Random_Access_Sequence<typename household_itf::get_type_of(Persons_Container), person_itf*> person_collection_itf;

			//	typedef Random_Access_Sequence<typename zone_itf::get_type_of(Activity_Locations_Container),int> activity_location_ids_itf;
			//	typedef Network_Components::Prototypes::Network<typename get_type_of(network_reference)> network_itf;
			//	typedef Scenario_Components::Prototypes::Scenario<typename get_type_of(scenario_reference)> scenario_itf;
			//	typedef  Activity_Location_Components::Prototypes::Activity_Location<typename remove_pointer<typename network_itf::get_type_of(activity_locations_container)::value_type>::type>  activity_location_itf;
			//	typedef Random_Access_Sequence<typename network_itf::get_type_of(activity_locations_container), activity_location_itf*> activity_locations_itf;

			//	typedef  Zone_Components::Prototypes::Zone<typename remove_pointer<typename network_itf::get_type_of(zones_container)::value_type>::type>  _Zone_Interface;
			//	typedef Pair_Associative_Container<typename network_itf::get_type_of(zones_container), _Zone_Interface*> _Zone_Container_Interface;

			//	
			//	regions_itf* regions = pthis->Synthesis_Regions_Collection<regions_itf*>();
			//	network_itf* network = pthis->network_reference<network_itf*>();
			//	scenario_itf* scenario = pthis->scenario_reference<scenario_itf*>();
			//	activity_locations_itf* activity_locations = network->template activity_locations_container<activity_locations_itf*>();
			//	

			//	// EXIT if no request to write the demand to database
			//	if (!scenario->template write_demand_to_database<bool>()) return;

			//	// Start database transaction
			//	try
			//	{
			//		string name(scenario->template output_demand_database_name<string&>());
			//		unique_ptr<odb::database> db (open_sqlite_database_single<unique_ptr<odb::database> >(name));
			//		odb::transaction t(db->begin());
			//	

			//		typename regions_itf::iterator r_itr;
			//		typename zones_itf::iterator z_itr;
			//		typename household_collection_itf::iterator p_itr;
			//		int counter = 0;

			//		// Loop through all regions
			//		for (r_itr = regions->begin(); r_itr != regions->end(); ++r_itr)
			//		{
			//			region_itf* region = r_itr->second;
			//			zones_itf* zones = region->template Synthesis_Zone_Collection<zones_itf*>();
			//			// loop through zones in each region
			//			for (z_itr = zones->begin(); z_itr != zones->end(); ++z_itr)
			//			{
			//				zone_itf* zone = z_itr->second;
			//				activity_location_ids_itf* loc_indices = zone->template Activity_Locations_Container<activity_location_ids_itf*>();

			//				// loop through each synthesized person
			//				household_collection_itf* households = zone->template Synthetic_Households_Container<household_collection_itf*>();
			//				for (p_itr = households->begin(); p_itr != households->end(); ++p_itr)
			//				{
			//					// update synthesizing persons counter
			//					if (counter % 10000 == 0) cout << '\r' << "Writing Agents to database:           " << counter;
			//					household_itf* hh = *p_itr;
			//					pop_unit_itf* hh_unit = hh->template Static_Properties<pop_unit_itf*>();
			//				
			//					// create household record
			//					shared_ptr<polaris::io::Household> hh_rec(new polaris::io::Household());
			//					hh_rec->setHhold(hh->template uuid<int>());
			//					hh_rec->setPersons(hh_unit->template Household_size<int>());
			//					hh_rec->setWorkers(hh_unit->template Number_of_workers<int>());
			//					hh_rec->setVehicles(hh_unit->template Number_of_vehicles<int>());
			//					hh_rec->setLocation(hh->template Home_Location<activity_location_itf*>()->template uuid<int>());
			//					//push to database
			//					db->persist(hh_rec);

			//					person_collection_itf* persons = hh->template Persons_Container<person_collection_itf*>();

			//					for (typename person_collection_itf::iterator p_itr = persons->begin(); p_itr != persons->end(); ++p_itr)
			//					{		
			//						person_itf* person = (person_itf*)(*p_itr);

			//						shared_ptr<polaris::io::Person> per_rec(new polaris::io::Person());
			//						per_rec->setId(person->template uuid<int>());
			//						if (person->template School_Location<int>() >= 0)
			//							per_rec->setSchool_Location_Id(person->template School_Location<activity_location_itf*>()->template uuid<int>());
			//						if (person->template Work_Location<int>() >= 0)
			//							per_rec->setWork_Location_Id(person->template Work_Location<activity_location_itf*>()->template uuid<int>());
			//						per_rec->setHousehold(hh_rec);
			//						//push to database
			//						db->persist(per_rec);

			//						person->template person_record<shared_ptr<polaris::io::Person>>(per_rec);

			//						counter++;
			//					}

			//				}
			//			}
			//		}
			//		t.commit();
			//	}
			//	catch (odb::sqlite::database_exception ex)
			//	{
			//		cout << endl << ex.what();
			//	}

			//	
			//}

			//----------------------------------------------------------------
			// Required Forms - necessary for any synthesis routine

			accessor(Synthesis_Regions_Collection, NONE, NONE);
			accessor(Solution_Settings, NONE, NONE);
			accessor(scenario_reference, NONE, NONE);
			accessor(network_reference, NONE, NONE);
			accessor(file_linker, NONE,NONE);
			accessor(timer, NONE, NONE);
			accessor(write_marginal_output_flag, NONE, NONE);
			accessor(write_full_output_flag, NONE, NONE);

			//----------------------------------------------------------------
			// Optional Forms - used for specific solution types
			accessor(linker_file_path, NONE, NONE);
			
			//----------------------------------------------------------------
			// Output features - optional, used to write intermediate and final results
			accessor(Output_Stream,check_2(strip_modifiers(TargetType),ofstream, is_same), check_2(strip_modifiers(TargetType),ofstream, is_same));
			accessor(Marginal_Output_Stream,check_2(strip_modifiers(TargetType),ofstream, is_same), check_2(strip_modifiers(TargetType),ofstream, is_same));
			accessor(Log_File,check_2(strip_modifiers(TargetType),ofstream, is_same), check_2(strip_modifiers(TargetType),ofstream, is_same));
		};
	}
}

