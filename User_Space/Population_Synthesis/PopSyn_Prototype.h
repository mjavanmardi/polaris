#pragma once

#include "User_Space\User_Space_Includes.h"
#include "User_Space\Population_Synthesis\linker.h"
#include "User_Space\Population_Synthesis\Synthesis_Region_Implementation.h"


namespace PopSyn
{
	enum POPSYN_ITERATIONS
	{
		MAIN_INITIALIZE,
		MAIN_PROCESS
	};
	enum POPSYN_SUBITERATIONS
	{
		INITIALIZE,
		START_TIMING,
		PROCESS,
		STOP_TIMING,
		OUTPUT
	};
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
		prototype struct Population_Synthesizer_Prototype : public ComponentType
		{
			tag_as_prototype; // Declare class as a polaris prototype

			//----------------------------------------------------------------
			// Schedules the first event from above
			feature_prototype void Initialize()
			{
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
				cout<<endl<<"====================================================="<<endl<<"Starting synthetic population generation:"<<endl;
				//------------------------
				// TIMER
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
				define_component_interface(Rand_Interface,MasterType::RNG,RNG_Prototype,NULLTYPE);
				Rand_Interface* rand = (Rand_Interface*)Allocate<MasterType::RNG>();
				//Rand_Interface* rand = (Rand_Interface*)(new MasterType::RNG()); // ALLOCATION TEST
				rand->Initialize<double>(0);

				// IPF Solver Settings
				define_component_interface(solver_itf,MasterType::IPF_Solver_Settings,PopSyn::Prototypes::Solver_Settings_Prototype,ComponentType);
				solver_itf* solver = (solver_itf*)Allocate<MasterType::IPF_Solver_Settings>();
				//solver_itf* solver = (solver_itf*)(new MasterType::IPF_Solver_Settings()); // ALLOCATION TEST

				//===============================================================================================================
				// Initialize file linker
				//---------------------------------------------------------------------------------------------------------------
				vector<int> dims;
				linker.Init_Linker(dims, this->linker_file_path<string>().c_str());
				ndim = (int)dims.size();

				// Define iterators and get pointer to the region collection
				typedef get_type_of(Synthesis_Regions_Collection)			region_collection_type;
				typedef region_collection_type::unqualified_value_type		region_type;
				typedef region_type::Sample_Data_type						sample_collection_type;
				typedef sample_collection_type::unqualified_value_type		sample_type;
				typedef region_type::Synthesis_Zone_Collection_type			zone_collection_type;
				typedef zone_collection_type::unqualified_value_type		zone_type;

				define_container_and_value_interface(regions_itf,region_itf,region_collection_type,Associative_Container_Prototype,PopSyn::Prototypes::Synthesis_Region_Prototype,ComponentType);
				define_container_and_value_interface(zones_itf,zone_itf,zone_collection_type,Associative_Container_Prototype,PopSyn::Prototypes::Synthesis_Zone_Prototype,ComponentType);
				define_container_and_value_interface(sample_data_itf,pop_unit_itf,sample_collection_type,Associative_Container_Prototype,PopSyn::Prototypes::Population_Unit_Prototype,ComponentType);
				define_simple_container_interface(joint_itf,MasterType::region::Target_Joint_Distribution_type,Multidimensional_Random_Access_Array_Prototype, MasterType::region::Target_Joint_Distribution_type::unqualified_value_type ,NULLTYPE);
				define_simple_container_interface(marginal_itf,MasterType::region::Target_Marginal_Distribution_type,Multidimensional_Random_Access_Array_Prototype, MasterType::region::Target_Marginal_Distribution_type::unqualified_value_type ,NULLTYPE);
	
				
				regions_itf* regions = this->Synthesis_Regions_Collection<regions_itf*>();
				regions_itf::iterator region_itr;
				this->Synthesis_Regions_Collection<regions_itf*>(regions);

				//hash_map<int, Region> regions;
				//vector<Zone>::iterator zone_itr;
				joint_itf::index_type dimensions;
				joint_itf::index_type index;
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
					regions_itf::key_type ID;
		
					// read the region data from input file
					if(!fr.Get_Data<regions_itf::key_type>(ID,linker.region_id_col)) break; 

					if ((region_itr = regions->find(ID)) == regions->end())
					{
						// create new region
						new_region = (region_itf*)Allocate<region_type>();
						new_region->Initialize<NULLTYPE>();
						new_region->parent_reference(this);
						dist = new_region->Target_Joint_Distribution<joint_itf*>();
						marg = new_region->Target_Marginal_Distribution<marginal_itf*>();
						Rand_Interface* my_rand = (Rand_Interface*)Allocate<MasterType::RNG>(); // ALLOCATION TEST
						//Rand_Interface* my_rand = (Rand_Interface*)(new MasterType::RNG());
						my_rand->Initialize<double>(ID/*rand->Next_Rand<double>()*(double)SHRT_MAX*/);
						new_region->Output_Stream<ostream&>(out);
						((zone_itf*)new_region)->Rand<Rand_Interface*>(my_rand);

						//-----------------------------------------------------------------------------------------
						// Initialize the distribution and marginals

						dist->resize(dimensions,0.0);
						marg->resize(dimensions,0.0);

						new_region->ID<int>(ID);
						solver = (solver_itf*)Allocate<MasterType::IPF_Solver_Settings>(); // ALLOCATION TEST
						//solver = (solver_itf*)(new MasterType::IPF_Solver_Settings());

						solver->Initialize<Target_Type<void,double,int>>(0.05,100);
						new_region->Solver_Settings<solver_itf*>(solver);

						// add new region to the list
						regions->insert(pair<regions_itf::key_type, region_itf*>(ID, new_region));
					}
					else
					{
						new_region = region_itr->second;
					}
					// next add the sample data
					dist = new_region->Target_Joint_Distribution<joint_itf*>();
					marg = new_region->Target_Marginal_Distribution<marginal_itf*>();
					sample = new_region->Sample_Data<sample_data_itf*>();

					double x;
					for (int i=0; i<ndim; i++)
					{
						fr.Get_Data<double>(x,linker.get_pums_column(i));
						index[i] = linker.find_index_in_dimension(i,x);
					}

					sample_type::ID_type sample_id;
					sample_type::Weight_type weight;
					fr.Get_Data<sample_type::ID_type>(sample_id,linker.sample_id_col);
					fr.Get_Data<sample_type::Weight_type>(weight, linker.sample_weight_col);
					sample_type::Characteristics_type data;
					fr.Get_Data<sample_type::Characteristics_type::unqualified_value_type>(data,linker.get_pums_data_cols());

					pop_unit_itf* p = (pop_unit_itf*)Allocate<sample_type>(); //ALLOCATION_TEST
					//pop_unit_itf* p = (pop_unit_itf*)(new sample_type());
					p->ID(sample_id);
					p->Index(new_region->Get_1D_Index<Target_Type<joint_itf::size_type,joint_itf::index_type>>(index));
					p->Weight(weight);
					p->Characteristics(data);

					// Update the sample and joint distribution with the current population unit
					sample->insert(p->Index<sample_collection_type::key_type>(), p);
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
					zone_type::ID_type ID;
					regions_itf::key_type RID;
					double x;
		
					// get ID values for ZONE and REGION
					if(!zone_fr.Get_Data<zone_type::ID_type>(ID,linker.zone_id_col))
					{
						cout<<"ERROR: could not retrieve zone id from zone file"; return EXIT_FAILURE;
					}
					if(!zone_fr.Get_Data<regions_itf::key_type>(RID,linker.region_in_zone_id_col))
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
					regional_marg = region->Target_Marginal_Distribution<marginal_itf*>();

					// Read marginal data from file and add to ZONE
					zone_itf* zone = (zone_itf*)Allocate<zone_type>(); // ALLOCATION_TEST
					zone->parent_reference(region);

					zone->ID(ID);
					solver = (solver_itf*)Allocate<MasterType::IPF_Solver_Settings>(); // ALLOCATION_TEST
					//solver = (solver_itf*)(new MasterType::IPF_Solver_Settings());

					solver->Initialize<Target_Type<void,double,int>>(0.05,100);
					zone->Solver_Settings<solver_itf*>(solver);
					joint_itf* mway = zone->Target_Joint_Distribution<joint_itf*>();
					marginal_itf* marg = zone->Target_Marginal_Distribution<marginal_itf*>();
					mway->resize(dimensions,0);
					marg->resize(dimensions,0);
					Rand_Interface* my_rand = (Rand_Interface*)Allocate<MasterType::RNG>(); // ALLOCATION_TEST
					//Rand_Interface* my_rand = (Rand_Interface*)(new MasterType::RNG());
					my_rand->Initialize<double>(rand->Next_Rand<double>()*(double)SHRT_MAX);
					

					zone->Rand<Rand_Interface*>(my_rand);

					for (marginal_itf::size_type i=0; i<dimensions.size(); i++)
					{
						for (marginal_itf::size_type j=0; j<dimensions[i]; j++)
						{
							if (!zone_fr.Get_Data<double>(x,linker.get_sf3_column(i,j))) break;
							(*marg)[pair<marginal_itf::size_type,marginal_itf::size_type>(i,j)] = x;
							(*regional_marg)[pair<marginal_itf::size_type,marginal_itf::size_type>(i,j)] += x;
						}

					}
					region->Synthesis_Zone_Collection<zones_itf*>()->insert(pair<zone_type::ID_type,zone_itf*>(ID,zone));;
				}
				zone_fr.Close();

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
				cout << endl<<"freq: " << this->timer<Counter&>().get_freq_value() << ", start: "<<this->timer<Counter&>().get_start_value() <<", l: "<<this->timer<Counter&>().get_l_value();
			}
			
			// 4.) Output results event - called after timing is stopped (at timestep 7)
			declare_feature_event(Output_Popsyn_Event)
			{
				Counter timer;
				timer.Start();

				Population_Synthesizer_Prototype<ComponentType,CallerType>* pthis = (Population_Synthesizer_Prototype<ComponentType,CallerType>*)_this;
				
				ostream& sample_out = pthis->Output_Stream<ostream&>();
				ostream& marg_out = pthis->Marginal_Output_Stream<ostream&>();
				
				// Define iterators and get pointer to the region collection
				typedef get_type_of(Synthesis_Regions_Collection)			region_collection_type;
				typedef region_collection_type::unqualified_value_type		region_type;
				typedef region_type::Sample_Data_type						sample_collection_type;
				typedef sample_collection_type::unqualified_value_type		sample_type;
				typedef region_type::Synthesis_Zone_Collection_type			zone_collection_type;
				typedef zone_collection_type::unqualified_value_type		zone_type;

				define_container_and_value_interface(regions_itf,region_itf,region_collection_type,Associative_Container_Prototype,PopSyn::Prototypes::Synthesis_Region_Prototype,ComponentType);
				define_container_and_value_interface(zones_itf,zone_itf,zone_collection_type,Associative_Container_Prototype,PopSyn::Prototypes::Synthesis_Zone_Prototype,ComponentType);
				define_container_and_value_interface(sample_data_itf,pop_unit_itf,sample_collection_type,Associative_Container_Prototype,PopSyn::Prototypes::Population_Unit_Prototype,ComponentType);
				define_simple_container_interface(joint_itf,MasterType::region::Target_Joint_Distribution_type,Multidimensional_Random_Access_Array_Prototype, MasterType::region::Target_Joint_Distribution_type::unqualified_value_type ,NULLTYPE);
				define_simple_container_interface(marginal_itf,MasterType::region::Target_Marginal_Distribution_type,Multidimensional_Random_Access_Array_Prototype, MasterType::region::Target_Marginal_Distribution_type::unqualified_value_type ,NULLTYPE);
	
				regions_itf* regions = pthis->Synthesis_Regions_Collection<regions_itf*>();
				regions_itf::iterator r_itr;
				zones_itf::iterator z_itr;

				for (r_itr = regions->begin(); r_itr != regions->end(); ++r_itr)
				{
					region_itf* region = r_itr->second;
					zones_itf* zones = region->Synthesis_Zone_Collection<zones_itf*>();
					for (z_itr = zones->begin(); z_itr != zones->end(); ++z_itr)
					{
						zone_itf* zone = z_itr->second;
						marg_out <<endl<<endl<<"ZONE_ID: "<<zone->ID<long long int>();
						zone->Target_Joint_Distribution<joint_itf*>()->write(marg_out);
						marg_out <<endl;
						zone->Target_Marginal_Distribution<marginal_itf*>()->write(marg_out);
						marg_out <<endl;

						sample_data_itf* sample = zone->Sample_Data<sample_data_itf*>();
						
						sample_out << endl<<endl<<"ZONE_ID: "<<zone->ID<long long int>();
						for (sample_data_itf::iterator s_itr = sample->begin(); s_itr != sample->end(); ++s_itr)
						{
							sample_out << endl << "ID: " << s_itr->second->ID<uint>() << ",  weight: "<<s_itr->second->Weight<float>() <<", index: "<<s_itr->second->Index<uint>();
						}

					}
				}

				cout <<endl<<"File I/O Runtime: "<<timer.Stop();
			}
			
			//----------------------------------------------------------------
			// Required Features - necessary for any synthesis routine
			feature_accessor(Synthesis_Regions_Collection,none,none);
			feature_accessor(Solution_Settings,none,none);
			feature_accessor(scenario_reference, none, none);
			feature_accessor(network_reference, none, none);
			feature_accessor(timer,none,none);

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
