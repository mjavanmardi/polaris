#pragma once

#include "User_Space_Includes.h"
#include "Synthesis_Region_Implementation.h"



namespace PopSyn
{
	namespace Types
	{
		enum POPSYN_ITERATIONS
		{
			MAIN_INITIALIZE,
			MAIN_PROCESS
		};
		enum POPSYN_SUBITERATIONS
		{
			INITIALIZE=23,
			START_TIMING,
			PROCESS,
			STOP_TIMING,
			OUTPUT
		};
	} using namespace Types;

	namespace Concepts
	{
		concept struct Uses_Linker_File
		{
			check_named_member(Has_Use_Linker_File_Defined, _linker_file_path);
			define_default_check(Has_Use_Linker_File_Defined);
		};
	}

	namespace Prototypes
	{
		prototype struct Population_Synthesizer_Prototype ADD_DEBUG_INFO
		{
			tag_as_prototype; // Declare class as a polaris prototype

			//----------------------------------------------------------------
			// Schedules the first event from above
			feature_prototype void Initialize()
			{
				this_component()->Initialize<ComponentType,CallerType,TargetType>();

				load_event(ComponentType,Start_Popsyn_Conditional,Start_Popsyn_Event,POPSYN_ITERATIONS::MAIN_INITIALIZE,POPSYN_SUBITERATIONS::INITIALIZE,NULLTYPE);
				//load_event(ComponentType,Start_Main_Timer_Conditional,Start_Main_Timer,4,NULLTYPE);
			}

			//----------------------------------------------------------------
			// Main analysis loop events, used to separate operations into different timesteps
			//----------------------------------------------------------------
			// 1.) Startup Event - Reads inputs and allocates analysis objects (at timestep 1)
			declare_feature_conditional(Start_Popsyn_Conditional)
			{
				ComponentType* pthis = (ComponentType*)_this;
				switch (_sub_iteration)
				{
				case POPSYN_SUBITERATIONS::INITIALIZE:
					response.result = true;
					response.next._iteration = _iteration;
					response.next._sub_iteration = POPSYN_SUBITERATIONS::START_TIMING;
					break;
				case POPSYN_SUBITERATIONS::START_TIMING:
					response.result = true;
					pthis->Swap_Event(&Start_Main_Timer<NULLTYPE>);
					response.next._iteration = POPSYN_ITERATIONS::MAIN_PROCESS;
					response.next._sub_iteration = POPSYN_SUBITERATIONS::STOP_TIMING;
					break;
				case POPSYN_SUBITERATIONS::STOP_TIMING:
					response.result = true;
					pthis->Swap_Event(&Stop_Main_Timer<NULLTYPE>);
					response.next._iteration = _iteration;
					response.next._sub_iteration = POPSYN_SUBITERATIONS::OUTPUT;
					break;
				case POPSYN_SUBITERATIONS::OUTPUT:
					response.result = true;
					pthis->Swap_Event(&Output_Popsyn_Event<NULLTYPE>);
					response.next._iteration = END;
					response.next._sub_iteration = 0;
					break;
				default:
					response.result = false;
					response.next._iteration = END;
					response.next._sub_iteration = 0;
				}
				
				
			}
			declare_feature_event(Start_Popsyn_Event)
			{
				((Population_Synthesizer_Prototype<ComponentType,CallerType>*) _this)->Start_Popsyn<NULLTYPE>();
			}
			feature_prototype bool Start_Popsyn(requires(check(ComponentType,Concepts::Uses_Linker_File)))
			{
				//------------------------
				// TIMER
				cout<<endl<<"====================================================="<<endl<<"Starting synthetic population generation:"<<endl;
				Counter timer;
				timer.Start();
				//------------------------

				int ndim, ans;
				Linker linker = Linker();
				File_IO::File_Writer fw, fw_sample;
				ostream& out = this->Output_Stream<ostream&>();


				//===============================================================================================================
				// Initialize settings
				//---------------------------------------------------------------------------------------------------------------
				// CREATE RNG for later use
				define_component_interface(Rand_Interface,typename ComponentType::Master_Type::RNG,RNG_Prototype,NULLTYPE);
				Rand_Interface* rand = (Rand_Interface*)Allocate<typename ComponentType::Master_Type::RNG>();
				rand->Initialize<double>(0);

				// IPF Solver Settings
				define_component_interface(solver_itf,typename get_type_of(Solution_Settings),PopSyn::Prototypes::Solver_Settings_Prototype,ComponentType);
				solver_itf* solver = this->Solution_Settings<solver_itf*>();

				//===============================================================================================================
				// Initialize file linker
				//---------------------------------------------------------------------------------------------------------------
				vector<int> dims;
				linker.Init_Linker(dims, this->linker_file_path<string>().c_str());
				ndim = (int)dims.size();

				// Define iterators and get pointer to the region collection
				typedef typename get_type_of(Synthesis_Regions_Collection)				region_collection_type;
				typedef typename region_collection_type::unqualified_value_type			region_type;
				typedef typename region_type::Sample_Data_type							sample_collection_type;
				typedef typename sample_collection_type::unqualified_value_type			sample_type;
				typedef typename region_type::Synthesis_Zone_Collection_type			zone_collection_type;
				typedef typename zone_collection_type::unqualified_value_type			zone_type;
				typedef typename region_type::get_type_of(Target_Joint_Distribution)	joint_dist_type;
				typedef typename region_type::get_type_of(Target_Marginal_Distribution)	marg_dist_type;

				define_container_and_value_interface_unqualified_container(regions_itf,region_itf,region_collection_type,Associative_Container_Prototype,PopSyn::Prototypes::Synthesis_Region_Prototype,ComponentType);
				define_container_and_value_interface_unqualified_container(zones_itf,zone_itf,zone_collection_type,Associative_Container_Prototype,PopSyn::Prototypes::Synthesis_Zone_Prototype,ComponentType);
				define_container_and_value_interface_unqualified_container(sample_data_itf,pop_unit_itf,sample_collection_type,Associative_Container_Prototype,PopSyn::Prototypes::Population_Unit_Prototype,ComponentType);
				define_simple_container_interface(joint_itf,joint_dist_type,Multidimensional_Random_Access_Array_Prototype, typename joint_dist_type::unqualified_value_type ,NULLTYPE);
				define_simple_container_interface(marginal_itf,marg_dist_type,Multidimensional_Random_Access_Array_Prototype, typename marg_dist_type::unqualified_value_type ,NULLTYPE);
	
				
				regions_itf* regions = this->Synthesis_Regions_Collection<regions_itf*>();
				typename regions_itf::iterator region_itr;
				this->Synthesis_Regions_Collection<regions_itf*>(regions);

				typename joint_itf::index_type dimensions;
				typename joint_itf::index_type index;
				for (vector<int>::iterator i = dims.begin(); i != dims.end(); ++i) 
				{
					dimensions.push_back(*i);
					index.push_back(0);
				}


				//===============================================================================================================
				// read region file, fill sample data
				//---------------------------------------------------------------------------------------------------------------
				File_IO::File_Reader fr;
				if (!fr.Open(*linker.Sample_File_Path())) 
				{
					cout<<endl<<"Ending. Press any key."<<endl; cin>>ans; return EXIT_FAILURE;
				}

				// references to the various region components
				region_itf* new_region;
				joint_itf* dist;
				marginal_itf* marg,  *regional_marg;
				sample_data_itf* sample;

				while(fr.Read())
				{
					typename regions_itf::key_type ID;
		
					// read the region data from input file
					if(!fr.Get_Data<typename regions_itf::key_type>(ID,linker.region_id_col)) break; 

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

						region_solver->template Initialize<Target_Type<NULLTYPE,void,double,int>>(solver->template Tolerance<double>(),solver->template Percentage_to_synthesize<double>(),solver->template Iterations<int>());
						new_region->template Solver_Settings<solver_itf*>(region_solver);

						// add new region to the list
						pair<typename regions_itf::key_type, region_itf*> item = pair<typename regions_itf::key_type, region_itf*>(ID, new_region);
						//regions->insert(pair<typename regions_itf::key_type, region_itf*>(ID, new_region));
						regions->insert(item);
					}
					else
					{
						new_region = region_itr->second;
					}
					// next add the sample data
					dist = new_region->template Target_Joint_Distribution<joint_itf*>();
					marg = new_region->template Target_Marginal_Distribution<marginal_itf*>();
					sample = new_region->template Sample_Data<sample_data_itf*>();

					double x;
					for (int i=0; i<ndim; i++)
					{
						fr.Get_Data<double>(x,linker.get_pums_column(i));
						index[i] = linker.find_index_in_dimension(i,x);
					}

					typename sample_type::ID_type sample_id;
					typename sample_type::Weight_type weight;
					fr.Get_Data<typename sample_type::ID_type>(sample_id,linker.sample_id_col);
					fr.Get_Data<typename sample_type::Weight_type>(weight, linker.sample_weight_col);
					typename sample_type::Characteristics_type data;
					fr.Get_Data<typename sample_type::Characteristics_type::unqualified_value_type>(data,linker.get_pums_data_cols());

					pop_unit_itf* p = (pop_unit_itf*)Allocate<sample_type>();
					p->ID(sample_id);
					p->Index(new_region->template Get_1D_Index<Target_Type<NULLTYPE,typename joint_itf::size_type,typename joint_itf::index_type>>(index));
					p->Weight(weight);
					p->Characteristics(data);

					// Update the sample and joint distribution with the current population unit
					sample->insert(p->template Index<typename sample_collection_type::key_type>(), p);
					(*dist)[index] += weight;
				}
				fr.Close();


				//===============================================================================================================
				// Read zone file, fill marginal data
				//---------------------------------------------------------------------------------------------------------------
				File_IO::File_Reader zone_fr;
				if (!zone_fr.Open(*linker.Marg_File_Path())) 
				{
					cout<<endl<<"Ending. Press any key."<<endl;	cin>>ans; return EXIT_FAILURE;
				}
				while(zone_fr.Read())
				{
					typename zone_type::ID_type ID;
					typename regions_itf::key_type RID;
					double x;
		
					// get ID values for ZONE and REGION
					if(!zone_fr.Get_Data<typename zone_type::ID_type>(ID,linker.zone_id_col))
					{
						cout<<"ERROR: could not retrieve zone id from zone file"; return EXIT_FAILURE;
					}
					if(!zone_fr.Get_Data<typename regions_itf::key_type>(RID,linker.region_in_zone_id_col))
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
					region_itf* region = region_itr->second;
					regional_marg = region->template Target_Marginal_Distribution<marginal_itf*>();

					// Read marginal data from file and add to ZONE
					zone_itf* zone = (zone_itf*)Allocate<zone_type>();
					zone->template Initialize<NULLTYPE>();
					zone->parent_reference(region);

					zone->ID(ID);
					solver_itf* zone_solver = (solver_itf*)Allocate<typename get_type_of(Solution_Settings)>(); // ALLOCATION_TEST
					//solver = (solver_itf*)(new MasterType::IPF_Solver_Settings());

					zone_solver->Initialize<Target_Type<NULLTYPE,void,double,int>>(solver->template Tolerance<double>(),solver->template Percentage_to_synthesize<double>(), solver->template Iterations<int>());
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
							if (!zone_fr.Get_Data<double>(x,linker.get_sf3_column(i,j))) break;
							(*marg)[pair<typename marginal_itf::size_type,typename marginal_itf::size_type>(i,j)] = x;
							(*regional_marg)[pair<typename marginal_itf::size_type,typename marginal_itf::size_type>(i,j)] += x;
						}

					}
					pair<typename zone_type::ID_type,zone_itf*> item = pair<typename zone_type::ID_type,zone_itf*>(ID,zone);
					//region->template Synthesis_Zone_Collection<zones_itf*>()->insert(pair<typename zone_type::ID_type,zone_itf*>(ID,zone));
					region->template Synthesis_Zone_Collection<zones_itf*>()->insert(item);
				}
				zone_fr.Close();


				//===============================================================================================================
				// Fill zonal activity_locations list from network reference
				//---------------------------------------------------------------------------------------------------------------
				define_component_interface(network_itf,typename get_type_of(network_reference),Network_Components::Prototypes::Network_Prototype,ComponentType);
				define_container_and_value_interface(locations_container_itf, location_itf,typename network_itf::get_type_of(activity_locations_container),Containers::Random_Access_Sequence_Prototype,Activity_Location_Components::Prototypes::Activity_Location_Prototype,ComponentType);
				define_simple_container_interface(location_id_container_itf,typename zone_itf::get_type_of(Activity_Locations_Container),Containers::Random_Access_Sequence_Prototype,int,ComponentType);
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
					long long zone_id = location->census_zone_id<long long >();

					for (region_itr = regions->begin(); region_itr != regions->end(); ++region_itr)
					{
						region = region_itr->second;

						zones_itf* zones = region->Synthesis_Zone_Collection<zones_itf*>();
						
						zone_itr = zones->find(zone_id);

						if (zone_itr != zones->end())
						{
							zone = zone_itr->second;
							zone->Activity_Locations_Container<location_id_container_itf*>()->push_back(location->internal_id<int>());
							break;
						}
					}
				}


				//------------------------
				// TIMER
				cout <<"Setup Runtime (ms): "<<timer.Stop();
				//------------------------

				return true;
			}
			feature_prototype bool Start_Popsyn(requires(check(ComponentType,!Concepts::Uses_Linker_File)))
			{
				assert_check(ComponentType,Concepts::Uses_Linker_File,"This popsyn type does not use linker file setup.");
			}
			
			// 2.) Start timing event - called before individual objects begin processing (at timestep 3)
			declare_feature_event(Start_Main_Timer)
			{
				Population_Synthesizer_Prototype<ComponentType,CallerType>* pthis = (Population_Synthesizer_Prototype<ComponentType,CallerType>*)_this;
				pthis->Start_Timer<NULLTYPE>();
			}
			feature_prototype void Start_Timer()
			{
				this->timer<Counter&>().Start();
			}
			
			// 3.) Stop timing event - called after individual objects end processing (at timestep 5)
			declare_feature_event(Stop_Main_Timer)
			{
				Population_Synthesizer_Prototype<ComponentType,CallerType>* pthis = (Population_Synthesizer_Prototype<ComponentType,CallerType>*)_this;
				pthis->Stop_Timer<NULLTYPE>();
			}
			feature_prototype void Stop_Timer()
			{
				cout << endl<<"Main Algorithm run-time: " << this->timer<Counter&>().Stop();
				//cout << endl<<"freq: " << this->timer<Counter&>().get_freq_value() << ", start: "<<this->timer<Counter&>().get_start_value() <<", l: "<<this->timer<Counter&>().get_l_value();
			}
			
			// 4.) Output results event - called after timing is stopped (at timestep 7)
			declare_feature_event(Output_Popsyn_Event)
			{
				Population_Synthesizer_Prototype<ComponentType,CallerType>* pthis = (Population_Synthesizer_Prototype<ComponentType,CallerType>*)_this;
				ostream& sample_out = pthis->Output_Stream<ostream&>();
				ostream& marg_out = pthis->Marginal_Output_Stream<ostream&>();

				//---------------------------------------------------------------------------------------------
				// Type defines for sub_objects
				typedef typename get_type_of(Synthesis_Regions_Collection)				region_collection_type;
				typedef typename region_collection_type::unqualified_value_type			region_type;
				typedef typename region_type::Sample_Data_type							sample_collection_type;
				typedef typename sample_collection_type::unqualified_value_type			sample_type;
				typedef typename region_type::Synthesis_Zone_Collection_type			zone_collection_type;
				typedef typename zone_collection_type::unqualified_value_type			zone_type;
				typedef typename zone_type::get_type_of(Synthetic_Persons_Container)	person_collection_type;
				typedef typename person_collection_type::unqualified_value_type			person_type;
				typedef typename region_type::get_type_of(Target_Joint_Distribution)	joint_dist_type;
				typedef typename region_type::get_type_of(Target_Marginal_Distribution)	marg_dist_type;
				//---------------------------------------------------------------------------------------------
				// Interface defines for sub_objects
				define_container_and_value_interface_unqualified_container(regions_itf,region_itf,region_collection_type,Associative_Container_Prototype,PopSyn::Prototypes::Synthesis_Region_Prototype,ComponentType);
				define_container_and_value_interface_unqualified_container(zones_itf,zone_itf,zone_collection_type,Associative_Container_Prototype,PopSyn::Prototypes::Synthesis_Zone_Prototype,ComponentType);
				define_container_and_value_interface_unqualified_container(sample_data_itf,pop_unit_itf,sample_collection_type,Associative_Container_Prototype,PopSyn::Prototypes::Population_Unit_Prototype,ComponentType);
				define_simple_container_interface(joint_itf,joint_dist_type,Multidimensional_Random_Access_Array_Prototype, typename joint_dist_type::unqualified_value_type ,NULLTYPE);
				define_simple_container_interface(marginal_itf,marg_dist_type,Multidimensional_Random_Access_Array_Prototype, typename marg_dist_type::unqualified_value_type ,NULLTYPE);
				define_container_and_value_interface_unqualified_container(persons_collection_itf, person_itf,person_collection_type,Random_Access_Sequence_Prototype,Person_Components::Prototypes::Person,ComponentType);
				define_simple_container_interface(activity_location_ids_itf,typename zone_itf::get_type_of(Activity_Locations_Container),Containers::Random_Access_Sequence_Prototype,int,ComponentType);
				regions_itf* regions = pthis->Synthesis_Regions_Collection<regions_itf*>();
				define_component_interface(network_itf,typename get_type_of(network_reference),Network_Components::Prototypes::Network_Prototype,ComponentType);
				define_container_and_value_interface(activity_locations_itf, activity_location_itf,typename network_itf::get_type_of(activity_locations_container),Containers::Random_Access_Sequence_Prototype, Activity_Location_Components::Prototypes::Activity_Location_Prototype,ComponentType);
				define_container_and_value_interface(_Zone_Container_Interface, _Zone_Interface,typename network_itf::get_type_of(zones_container),Containers::Associative_Container_Prototype, Zone_Components::Prototypes::Zone_Prototype,ComponentType);
				network_itf* network = pthis->network_reference<network_itf*>();
				activity_locations_itf* activity_locations = network->template activity_locations_container<activity_locations_itf*>();

				typename regions_itf::iterator r_itr;
				typename zones_itf::iterator z_itr;
				typename persons_collection_itf::iterator p_itr;

				// initialize all of the synthesized individuals and assign unique ids
				long uuid = 0; // globally unique person id
				// Loop through all regions
				for (r_itr = regions->begin(); r_itr != regions->end(); ++r_itr)
				{
					region_itf* region = r_itr->second;
					zones_itf* zones = region->template Synthesis_Zone_Collection<zones_itf*>();
					// loop through zones in each region
					for (z_itr = zones->begin(); z_itr != zones->end(); ++z_itr)
					{
						zone_itf* zone = z_itr->second;
						activity_location_ids_itf* loc_indices = zone->Activity_Locations_Container<activity_location_ids_itf*>();

						// loop through each synthesized person
						persons_collection_itf* persons = zone->template Synthetic_Persons_Container<persons_collection_itf*>();
						for (p_itr = persons->begin(); p_itr != persons->end(); ++p_itr)
						{
							person_itf* person = *p_itr;

							// initialize the person - allocates all person subcomponents
							person->Initialize<long>(uuid);
							int home_loc_index;

							// assign person to a random activity location in the zone				
							if (loc_indices->size() == 0)
							{
								home_loc_index = (int)((GLOBALS::Uniform_RNG.Next_Rand<float>()*0.9999999) * activity_locations->size());
								person->Home_Location<int>(home_loc_index);

								//sample_out << zone->ID<long long>() << ","<<activity_locations->at(home_loc_index)->uuid<int>();
							}
							else
							{
								home_loc_index = (int)((GLOBALS::Uniform_RNG.Next_Rand<float>()*0.9999999) * loc_indices->size());
								person->Home_Location<int>(loc_indices->at(home_loc_index));

								//sample_out << zone->ID<long long>() << ","<<activity_locations->at(loc_indices->at(home_loc_index))->uuid<int>();
							}

							// get the polaris zone of the synthesized person and increment its population counter;
							_Zone_Interface* pzone = person->Home_Location<_Zone_Interface*>();
							pzone->population<int&>()++;

							//sample_out <<","<<pzone->uuid<int>()<<endl;

							++uuid;
						}
					}
				}
				cout <<endl<<endl<<"Total Persons Synthesized: "<<uuid<<endl<<endl;


				// Handle file output if needed
				if (pthis->write_output_flag<bool>() == true)
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
							marg_out <<endl<<endl<<"ZONE_ID: "<<zone->template ID<long long int>();
							zone->template Target_Joint_Distribution<joint_itf*>()->write(marg_out);
							marg_out <<endl;
							zone->template Target_Marginal_Distribution<marginal_itf*>()->write(marg_out);
							marg_out <<endl;

							sample_data_itf* sample = zone->template Sample_Data<sample_data_itf*>();
						
							sample_out << endl<<endl<<"ZONE_ID: "<<zone->template ID<long long int>();
							for (typename sample_data_itf::iterator s_itr = sample->begin(); s_itr != sample->end(); ++s_itr)
							{
								sample_out << endl << "ID: " << s_itr->second->template ID<uint>() << ",  weight: "<<s_itr->second->template Weight<float>() <<", index: "<<s_itr->second->template Index<uint>();
							}

						}
					}

					cout <<endl<<"File I/O Runtime: "<<timer.Stop();
				}
			}
			
			//----------------------------------------------------------------
			// Required Features - necessary for any synthesis routine
			feature_accessor(Synthesis_Regions_Collection,none,none);
			feature_accessor(Solution_Settings,none,none);
			feature_accessor(scenario_reference, none, none);
			feature_accessor(network_reference, check(ReturnValueType,Network_Components::Concepts::Is_Transportation_Network), check(SetValueType,Network_Components::Concepts::Is_Transportation_Network));
			feature_accessor(timer,none,none);
			feature_accessor(write_output_flag,none,none);

			//----------------------------------------------------------------
			// Optional Features - used for specific solution types
			feature_accessor(linker_file_path, none,none);
			
			//----------------------------------------------------------------
			// Output features - optional, used to write intermediate and final results
			feature_accessor(Output_Stream,check_2(strip_modifiers(ReturnValueType),ostream, is_same), check_2(strip_modifiers(SetValueType),ostream, is_same));
			feature_accessor(Marginal_Output_Stream,check_2(strip_modifiers(ReturnValueType),ostream, is_same), check_2(strip_modifiers(SetValueType),ostream, is_same));
		};
	}
}
