#pragma once

#include "traffic_simulator\User_Space_Includes.h"


namespace PopSyn
{

//===============================================
// Data range structure for marginal categories
//-----------------------------------------------
struct High_Low:public pair<double,double>
{
	double& low(){return first;}
	double& high(){return second;}
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


//==============================================================
//
// File linker class 
//
//	HOLDS INFO TO ENTER DATA INTO REGIONS/ZONES, FROM INPUT FILES
//
//--------------------------------------------------------------
prototype struct Popsyn_File_Linker
{
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

	/// Specify column # in zone (marginal) file which contains the region id - used to link marginal to sample file so must be identically specified in both
	typed_accessor(int,region_in_zone_id_column);

	typed_accessor(string, sample_file_path);
	typed_accessor(string, person_sample_file_path);
	typed_accessor(string, marg_file_path);


	/** Initializer*/
	void Initialize(boost::container::vector<int> &dim_sizes, string link_file_path)
	{
		this_component()->Initialize(dims_sizes,link_file_path);
	}
	void Read_Linker_File(boost::container::vector<int> &dim_sizes)
	{
		this_component()->Read_Linker_File(dim_sizes);
	}
	

	/** fill ranges*/
	void set_pums_columns(int region_id_column, int sampleid_column, int weight_column) 
	{
		this->region_id_col<int>(region_id_column);
		this->sample_id_col<int>(sampleid_column);
		this->sample_weight_col<int>(weight_column);
	}
	void set_person_columns(int region_id_column, int sampleid_column) 
	{
		this->person_region_id_column<int>(region_id_column);
		this->person_sample_id_column<int>(sampleid_column);
	}
	void set_pums_data_column(int dimension_number, int column_number) 
	{
		this_component()->set_pums_data_column(dimension_number,column_number);
	}
	void set_sf3_columns(int zone_column, int region_column) 
	{
		this->region_in_zone_id_column<int>(region_column);
		this->zone_id_column<int>(zone_column);
	}
	void set_sf3_data_column(int dimension_number, int index_in_dimension, double low_value, double high_value, int column_number)
	{
		this_component()->set_pums_data_column(dimension_number,index_in_dimension, low_value, high_value, column_number);
	}

	/** Functions to get ranges for a variable dimension and index*/
	High_Low& range(int dimension, int index_in_dimension) 
	{
		return this_component()->range(dimension, index_in_dimension);
	}
	double& low(int dimension, int index_in_dimension) 
	{
		return this_component()->low(dimension, index_in_dimension);
	}
	double& high(int dimension, int index_in_dimension) 
	{
		return this_component()->high(dimension, index_in_dimension);
	}

	/** Links the columns in the data files to dimensions/indices in the Region/Zone data table*/
	int& get_pums_column(int dimension){return this_component()->get_pums_column(dimension)}
	/** Links the columns int the data files to dimensions/indices in the Region/Zone data table*/
	int& get_sf3_column(int dim, int index){return this_component()->get_sf3_column(dim, index)}

	boost::container::vector<int>& get_pums_data_columns(){return this_component()->get_pums_data_columns(dimension);}
	boost::container::vector<int>& get_person_data_columns(){return this_component()->get_person_data_columns(dimension);}

	/** Find the variable index for a given dimension-value pair*/
	int find_index_in_dimension(int dim, double value){return this_component()->find_index_in_dimension(dim, value)}
};

implementation struct Popsyn_File_Linker_Implementation : public Polaris_Component< MasterType,INHERIT(Popsyn_File_Linker_Implementation), Data_Object>
{
	m_container(boost::container::vector<boost::container::vector<High_Low>>,ranges,NONE,NONE);
	m_container(boost::container::vector<int>,pums_file_link,NONE,NONE);
	m_container(boost::container::vector<boost::container::vector<int>>, sf3_file_link, NONE,NONE);
	m_container(boost::container::vector<int>, pums_file_data_cols, NONE,NONE);
	m_container(boost::container::vector<int>, person_file_data_cols, NONE,NONE);

	m_data(File_IO::File_Reader,fr,NONE,NONE);
	m_data(string, marg_file_path,NONE,NONE);
	m_data(string, sample_file_path,NONE,NONE);
	m_data(string, person_sample_file_path,NONE,NONE);

	m_data(int,zone_id_column,NONE,NONE);
	m_data(int,region_id_column,NONE,NONE);
	m_data(int,sample_id_column,NONE,NONE);
	m_data(int,sample_weight_column,NONE,NONE);
	m_data(int,person_region_id_column,NONE,NONE);
	m_data(int,person_sample_id_column,NONE,NONE);
	m_data(int,region_in_zone_id_column,NONE,NONE);


	void set_pums_data_column(int dimension_number, int column_number)
	{
		if (dimension_number < _pums_file_link.size())
		{
			_pums_file_link[dim] = column_number;
		}
		else 
		{
			cout<<"Error: dimension outside of bounds when setting pums column."<<endl; return;
		}
	}
	void set_sf3_data_column(int dimension_number, int index_in_dimension, double low_value, double high_value, int column_number)
	{
		if (dimension_number >= _ranges.size())
		{
			cout<<"Error: dimension outside of bounds when setting sf3 column."<<endl; return;
		}
		if (index >= _ranges[dimension_number].size())
		{
			cout<<"Error: index outside of dimension bounds when setting sf3 column."<<endl; return;
		}
		_ranges[dimension_number][index_in_dimension].first=low; _ranges[dim][index].second=high;_sf3_file_link[dim][index]=column;
	}
	High_Low& range(int dim, int index) 
	{
		return _ranges[dim][index];
	}
	double& low(int dim, int index) 
	{
		return _ranges[dim][index].first;
	}
	double& high(int dim, int index) 
	{
		return _ranges[dim][index].second;
	}

	/** Links the columns in the data files to dimensions/indices in the Region/Zone data table*/
	int& get_pums_column(int dim)
	{
		return _pums_file_link[dim];
	}

	/** Links the columns int the data files to dimensions/indices in the Region/Zone data table*/
	int& get_sf3_column(int dim, int index)
	{
		return _sf3_file_link[dim][index];
	}

	boost::container::vector<int>& get_pums_data_columns()
	{
		return _pums_file_data_cols;
	}
	boost::container::vector<int>& get_person_data_columns()
	{
		return _person_file_data_cols;
	}

	/** Find the variable index for a given dimension-value pair*/
	int find_index_in_dimension(int dim, double value)
	{
		for (int i=0; i<_ranges[dim].size(); i++)
		{
			if (value >= low(dim,i) && value < high(dim,i)) return i;
		}
		throw "Data outside of specified range was observed.";
	}

	void Initialize(boost::container::vector<int> &dim_sizes, string link_file_path)
	{
		zone_id_col =-1;
		region_id_col=-1;
		region_in_zone_id_col=-1;
		person_region_id_col=-1;

		_fr.Open(link_file_path, false);
		Read_Linker_File(dim_sizes);
	}

	void Read_Linker_File(boost::container::vector<int> &dim_sizes)
	{
		bool sampleid_found = false;
		bool margid_found = false;
		int count_sample_vars = 0;
		int count_marg_vars = 0;

		//===============================================
		// Linker file parsing
		//-----------------------------------------------
		while (_fr.Read())
		{
			string key;

			// Get the keyword from current line
			key = _fr.Get_String(0);


			if (key == "REGIONFILE")
			{
				Linker::sample_file_path = _fr.Get_String(1);
			}
			else if (key == "PERSONFILE")
			{
				Linker::person_sample_file_path = _fr.Get_String(1);
			}
			else if (key == "ZONEFILE")
			{
				Linker::marg_file_path = _fr.Get_String(1);
			}
			else if (key == "DIMS")
			{
				int dim;
				for (int i= 1; i < _fr.Line_Length(); i++)
				{
					_fr.Get_Data<int>(dim, i);
					dim_sizes.push_back(dim);
				}

				for (int i=0; i<dim_sizes.size(); i++)
				{
					boost::container::vector<High_Low> v_i;	
					boost::container::vector<int> v_ind;

					for (int j=0; j<dim_sizes[i]; j++)
					{
						High_Low p_j;
						Dim_Index p_c;
						v_i.push_back(p_j);
						v_ind.push_back(-1);		
					}
					_ranges.push_back(v_i);
					_sf3_file_link.push_back(v_ind);
			
					_pums_file_link.push_back(-1);
				}
			}
			else if (key == "REGION")
			{
				int col, col2, col3;
				if (_fr.Get_Data<int>(col, 1) && _fr.Get_Data<int>(col2,2) && _fr.Get_Data<int>(col3,3)) set_pums_id_col(col, col2, col3);
				else {cout<<"Error: region id column not set"<<endl; return;}
			}
			else if (key == "PERSON")
			{
				int col, col2;
				if (_fr.Get_Data<int>(col, 1) && _fr.Get_Data<int>(col2,2)) set_person_id_col(col, col2);
				else {cout<<"Error: person region id column not set"<<endl; return;}
			}
			else if (key == "REGIONDATA")
			{
				int size = _fr.Line_Length();
				for (int i = 1; i<size; i++)
				{
					int col;
					_fr.Get_Data<int>(col, i);
					_pums_file_data_cols.push_back(col);
				}
			}
			else if (key == "PERSONDATA")
			{
				int size = _fr.Line_Length();
				for (int i = 1; i<size; i++)
				{
					int col;
					_fr.Get_Data<int>(col, i);
					_person_file_data_cols.push_back(col);
				}
			}
			else if (key == "ZONE")
			{
				int col, col2;
				if (_fr.Get_Data<int>(col, 1) && _fr.Get_Data<int>(col2, 2)) set_sf3_id_col(col, col2);
				else {cout<<"Error: zone id columns not set"<<endl; return;}
			}

			else if (key == "SAMPLEVAR")
			{
				int col, col2;
				if (_fr.Get_Data<int>(col, 1) && _fr.Get_Data<int>(col2, 2)) set_pums_col(col, col2);
				else {cout<<"Error: region data columns not set"<<endl; return;}
			}
			else if (key == "MARGVAR")
			{
				int c1, c2, c3, c4, c5;
				if (_fr.Get_Data<int>(c1, 1) && _fr.Get_Data<int>(c2, 2) && _fr.Get_Data<int>(c3, 3) && _fr.Get_Data<int>(c4, 4) && _fr.Get_Data<int>(c5, 5)) set_sf3_col(c1, c2,c3, c4, c5);
				else {cout<<"Error: zone data columns not set"<<endl; return;}
			}
		}
	}
};


}