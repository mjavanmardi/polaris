#pragma once

#include "Population_Synthesis_Includes.h"

//---------------------------------------------------------
//	POP_UNIT OBJECT CLASS (i.e. HH, PERSON, VEHICLE, FIRM, etc.)
//---------------------------------------------------------

namespace PopSyn
{
	//===============================================
	// Data range structure for marginal categories
	//-----------------------------------------------
	struct High_Low:public pair<double,double>
	{
		double low(){return first;}
		double high(){return second;}
	};


	//======================================================
	// Structure to hold marginal file variable definitions
	//------------------------------------------------------
	struct Dim_Index:public pair<int,pair<int,int> >
	{
		int& dimension(){return first;}			// Holds the dimension of the current marginal variable (in the Region/zone MArray
		int& index(){return second.first;}		// Holds the index within the dimension of the current marginal
		int& column(){return second.second;}	// position of the marginal in data file
	};


	namespace Prototypes
	{
		//==============================================================
		//
		// File linker class 
		//
		//	HOLDS INFO TO ENTER DATA INTO REGIONS/ZONES, FROM INPUT FILES
		//
		//--------------------------------------------------------------
		prototype struct Popsyn_File_Linker ADD_DEBUG_INFO
		{
			tag_as_prototype;

			/// Specify column # in zone (marginal) file which contains the zone id
			typed_accessor(int,zone_id_column);
			/// Specify column # in region (sample) file which contains the region id
			typed_accessor(int,region_id_column);
			/// Specify column # in region (sample) file which contains the sample observation id (hhid)
			typed_accessor(int,sample_id_column);
			/// Specify column # in region (sample) file which contains the sample weight
			typed_accessor(int,sample_weight_column);

			/// Specify column # in region (sample) person file which contains the region id
			typed_accessor(int,person_region_id_column);
			/// Specify column # in region (sample) person file which contains the sample id (hhid)
			typed_accessor(int,person_sample_id_column);
			/// Specify column # in region (sample) person file which contains the sample id (hhid)
			typed_accessor(int,person_weight_column);

			/// Specify column # in zone (marginal) file which contains the region id - used to link marginal to sample file so must be identically specified in both
			typed_accessor(int,region_in_zone_id_column);

			typed_accessor(string&, sample_file_path);
			typed_accessor(string&, person_sample_file_path);
			typed_accessor(string&, marg_file_path);

			typed_accessor(std::vector<int>&, hh_dimension_sizes);
			typed_accessor(std::vector<int>&, person_dimension_sizes);
			typed_accessor(int, test_dimension_size);
			typed_accessor(int, test_person_dimension_size);
			typed_accessor(int, number_of_hh_dimensions);
			typed_accessor(int, number_of_person_dimensions);

			/** Initializer*/
			void Initialize(string& link_file_path)
			{
				this_component()->Initialize(link_file_path);
			}


			/** fill ranges*/
			void set_pums_columns(int region_id_column, int sampleid_column, int weight_column) 
			{
				this->region_id_column(region_id_column);
				this->sample_id_column(sampleid_column);
				this->sample_weight_column(weight_column);
			}
			void set_person_columns(int region_id_column, int sampleid_column, int weight_column) 
			{
				this->person_region_id_column(region_id_column);
				this->person_sample_id_column(sampleid_column);
				this->person_weight_column(weight_column);
			}
			void set_sf3_columns(int zone_column, int region_column) 
			{
				this->region_in_zone_id_column(region_column);
				this->zone_id_column(zone_column);
			}

			void set_pums_data_column(int dimension_number, int column_number, bool household=true, bool test_variable=false) 
			{
				this_component()->set_pums_data_column(dimension_number,column_number, household, test_variable);
			}
			void set_sf3_data_column(int dimension_number, int index_in_dimension, double low_value, double high_value, int column_number, bool household=true, bool test_variable=false)
			{
				this_component()->set_sf3_data_column(dimension_number,index_in_dimension, low_value, high_value, column_number, household, test_variable);
			}

			/** Functions to get ranges for a variable dimension and index*/
			High_Low range(int dimension, int index_in_dimension, bool household=true) 
			{
				return this_component()->range(dimension, index_in_dimension);
			}
			double low(int dimension, int index_in_dimension, bool household=true, bool test_variable=false) 
			{
				return this_component()->low(dimension, index_in_dimension,household,test_variable);
			}
			double high(int dimension, int index_in_dimension, bool household=true, bool test_variable=false) 
			{
				return this_component()->high(dimension, index_in_dimension,household,test_variable);
			}

			/** Links the columns in the data files to dimensions/indices in the Region/Zone data table*/
			int get_pums_column(int dimension, bool household=true, bool test_variable=false){return this_component()->get_pums_column(dimension,household,test_variable);}
			/** Links the columns int the data files to dimensions/indices in the Region/Zone data table*/
			int get_sf3_column(int dim, int index, bool household=true, bool test_variable=false){return this_component()->get_sf3_column(dim, index, household,test_variable);}

			const std::vector<int>& get_pums_data_columns(){return this_component()->get_pums_data_columns();}
			const std::vector<int>& get_person_data_columns(){return this_component()->get_person_data_columns();}

			/** Find the variable index for a given dimension-value pair*/
			int find_index_in_dimension(int dim, double value, bool household=true, bool test_variable=false){return this_component()->find_index_in_dimension(dim, value, household, test_variable);}
		};
	}
}


