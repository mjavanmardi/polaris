#pragma once

#include "User_Space\User_Space_Includes.h"
#include "User_Space\Population_Synthesis\linker.h"
#include "User_Space\Population_Synthesis\Synthesis_Region_Implementation.h"


namespace PopSyn
{
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
			feature_prototype bool Start_Popsyn(requires(check(ComponentType,Concepts::Uses_Linker_File)))
			{
				tag_as_prototype;

				int ndim, ans;
				Linker linker;
				File_IO::File_Writer fw, fw_sample;


				//===============================================================================================================
				// Initialize settings
				//---------------------------------------------------------------------------------------------------------------
				// CREATE RNG for later use
				define_component_interface(Rand_Interface,MasterType::RNG,RNG_Prototype,NULLTYPE);
				Rand_Interface* rand = (Rand_Interface*)Allocate<MasterType::RNG>();
				rand->Initialize<double>(0);

				// IPF Solver Settings
				define_component_interface(solver_itf,MasterType::IPF_Solver_Settings,PopSyn::Prototypes::Solver_Settings_Prototype,ComponentType);
				solver_itf* solver = (solver_itf*)Allocate<MasterType::IPF_Solver_Settings>();
				solver->Initialize<Target_Type<void,double,int>>(0.05,100);
	

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
				marginal_itf* marg;
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
						dist = new_region->Target_Joint_Distribution<joint_itf*>();
						marg = new_region->Target_Marginal_Distribution<marginal_itf*>();

						//-----------------------------------------------------------------------------------------
						// Initialize the distribution and marginals

						dist->resize(dimensions,0.0);
						marg->resize(dimensions,0.0);

						new_region->ID<int>(ID);

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

					pop_unit_itf* p = (pop_unit_itf*)Allocate<sample_type>();
					p->ID(sample_id);
					p->Weight(weight);
					p->Characteristics(data);

					// Update the sample and joint distribution with the current population unit
					sample->insert(p->ID<sample_collection_type::key_type>(), p);
					(*dist)[index] += weight;
				}
				fr.Close();

				for (region_itr = regions->begin(); region_itr != regions->end(); ++region_itr)
				{
					// print the distributions to output file
				}



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

					// Read marginal data from file and add to ZONE
					zone_itf* zone = (zone_itf*)Allocate<zone_type>();
					zone->ID(ID);
					joint_itf* mway = zone->Target_Joint_Distribution<joint_itf*>();
					marginal_itf* marg = zone->Target_Marginal_Distribution<marginal_itf*>();
					mway->resize(index,0);

					for (marginal_itf::size_type i=0; i<index.size(); i++)
					{
						for (marginal_itf::size_type j=0; j<index[i]; j++)
						{
							if (!zone_fr.Get_Data<double>(x,linker.get_sf3_column(i,j))) break;
							(*marg)[pair<marginal_itf::size_type,marginal_itf::size_type>(i,j)] = x;
						}

					}
					region->Synthesis_Zone_Collection<zones_itf*>()->insert(pair<zone_type::ID_type,zone_itf*>(ID,zone));;
				}
				zone_fr.Close();


				//===============================================================================================================
				// RUN SYNTHESIS PROCESS
				//---------------------------------------------------------------------------------------------------------------
				for (region_itr = regions->begin(); region_itr != regions->end(); region_itr++)
				{
					region_itf* region = region_itr->second;
					region->Synthesize_Population<NULLTYPE>();
				}

				//===============================================================================================================
				// OUTPUT RESULTS
				//---------------------------------------------------------------------------------------------------------------
				//fw.Open("..\\output.txt");
				//fw_sample.Open("..\\population.txt");
				//for (region_itr = regions.begin(); region_itr != regions.end(); region_itr++) region_itr->second.write(fw, fw_sample);
				//fw.Close();
				//fw_sample.Close();

				cout <<endl<<"END?";
				cin >> ans;
				return 0;
			}

			feature_prototype bool Start_Popsyn(requires(check(ComponentType,!Concepts::Uses_Linker_File)))
			{
				assert_check(ComponentType,Concepts::Uses_Linker_File,"This popsyn type does not use linker file setup.");
			}
			
			feature_accessor(linker_file_path, none,none);

			feature_accessor(Synthesis_Regions_Collection,none,none);

			feature_accessor(Solution_Settings,none,none);
		};
	}
}
