#pragma once

#include "User_Space\User_Space_Includes.h"
#include "User_Space\Population_Synthesis\Linker_Implementations.h


namespace PopSyn
{
	namespace Concepts
	{
		concept struct Uses_Linker_File
		{
			check_typename_state(Has_Use_Linker_File_Defined, Use_Link_File, true_type);
			define_default_check(Has_Use_Linker_File_Defined);
		};
	}

	namespace Prototypes
	{
		prototype struct Population_Synthesizer_Prototype
		{
			feature_prototype bool start_popsyn(requires(check(ComponentType,Uses_Linker_File)))
			{
				int ndim, ans;
				Linker linker;
				File_IO::File_Writer fw, fw_sample;
	

				//===============================================================================================================
				// Initialize file linker
				//---------------------------------------------------------------------------------------------------------------
				vector<int> dims;
				linker.Init_Linker(dims, link_file_path);

				// Define iterators
				hash_map<int, Region> regions;
				hash_map<int, Region>::iterator region_itr;
				vector<Zone>::iterator zone_itr;


				//===============================================================================================================
				// read region file, fill sample data
				//---------------------------------------------------------------------------------------------------------------
				File_IO::File_Reader fr;
				if (!fr.Open(*linker.Sample_File_Path())) 
				{
					cout<<endl<<"Ending. Press any key."<<endl; cin>>ans; return EXIT_FAILURE;
				}

				while(fr.Read())
				{
					int ID;
					double x;
		
					// get region iterator from hash map
					if(!fr.Get_Data<int>(ID,linker.region_id_col)) break; 
					if ((region_itr = regions.find(ID)) == regions.end()) 
					{
						pair<int,Region> p = pair<int,Region>(ID,Region(ID,dims));
						region_itr = regions.insert(p).first;
					}
		
					// add household to region
					region_itr->second.add_sample_data(linker,fr);

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
					double ID;
					double RID;
					double x;
		
					// get ID values for ZONE and REGION
					if(!zone_fr.Get_Data<double>(ID,linker.zone_id_col))
					{
						cout<<"ERROR: could not retrieve zone id from zone file"; return EXIT_FAILURE;
					}
					if(!zone_fr.Get_Data<double>(RID,linker.region_in_zone_id_col))
					{
						cout<<"ERROR: could not retrieve region id from zone file"; return EXIT_FAILURE;
					}

					// get region iterator from hash map
					if ((region_itr = regions.find(RID))==regions.end())
					{
						cout<<"ERROR: Region ID not found in map.  Press a key to end.";
						cin>>RID;
						return EXIT_FAILURE;
					}

					// Read marginal data from file and add to ZONE
					Zone Z(ID, dims);
					for (int i=0; i<dims.size(); i++)
					{
						for (int j=0; j<dims[i]; j++)
						{
							if (!zone_fr.Get_Data<double>(x,linker.get_sf3_column(i,j))) break;
							Z.marginal(i,j)  = x;
						}

					}
					region_itr->second.add_zone(Z);
				}
				zone_fr.Close();


				//===============================================================================================================
				// RUN SYNTHESIS PROCESS
				//---------------------------------------------------------------------------------------------------------------
				for (region_itr = regions.begin(); region_itr != regions.end(); region_itr++)
				{
					region_itr->second.start_synthesis(0,100,10);
				}
				for (region_itr = regions.begin(); region_itr != regions.end(); region_itr++)
				{
					region_itr->second.Join(); // wait until all IPF/Synthesis is complete
				}



				//===============================================================================================================
				// OUTPUT RESULTS
				//---------------------------------------------------------------------------------------------------------------
				fw.Open("..\\output.txt");
				fw_sample.Open("..\\population.txt");
				for (region_itr = regions.begin(); region_itr != regions.end(); region_itr++) region_itr->second.write(fw, fw_sample);
				fw.Close();
				fw_sample.Close();

				cout <<endl<<"END?";
				cin >> ans;
				return 0;
			}

			feature_accessor(linker_fil_path, check_2(TargetType, char*, is_same), check_2(TargetType, char*, is_same));
		};
	}
}
