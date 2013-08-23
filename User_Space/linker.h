#pragma once

#include "User_Space_Includes.h"


using namespace std;

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
struct Dim_Index:public pair<int,pair<int,int>>
{
	int& dimension(){return first;}			// Holds the dimension of the current marginal variable (in the Region/zone MArray
	int& index(){return second.first;}		// Holds the index within the dimension of the current marginal
	int& column(){return second.second;}	// position of the marginal in data file
};


//==============================================================
//
// linker class 
//
//	HOLDS INFO TO ENTER DATA INTO REGIONS/ZONES, FROM INPUT FILES
//
//--------------------------------------------------------------
class Linker
{
public:
	static int zone_id_col;
	static int region_id_col;
	static int sample_id_col;
	static int sample_weight_col;

	static int person_region_id_col;
	static int person_sample_id_col;
	
	static int region_in_zone_id_col;

	/** Initializer*/
	static void Init_Linker(vector<int> &dim_sizes, string link_file_path);
	static void Read_Linker_File(vector<int> &dim_sizes);
	static string const * const Sample_File_Path(){ return &sample_file_path;}
	static string const * const Person_File_Path(){ return &person_sample_file_path;}
	static string const * const Marg_File_Path(){ return &marg_file_path;}

	/** fill ranges*/
	static void set_pums_id_col(int column, int sampleid_column, int weight_column) {region_id_col=column; sample_id_col = sampleid_column; sample_weight_col=weight_column;}
	static void set_person_id_col(int column, int sampleid_column) {person_region_id_col=column; person_sample_id_col = sampleid_column;}
	static void set_pums_col(int dim, int column) {if (dim < _pums_file_link.size()) _pums_file_link[dim] = column; else {cout<<"Error: dimension outside of bounds when setting pums column."<<endl; return;}}
	static void set_sf3_id_col(int zone, int region) {region_in_zone_id_col=region; zone_id_col=zone;}
	static void set_sf3_col(int dim, int index, double low, double high, int column)
	{
		if (dim >= _ranges.size())
		{
			cout<<"Error: dimension outside of bounds when setting sf3 column."<<endl; return;
		}
		if (index >= _ranges[dim].size())
		{
			cout<<"Error: index outside of dimension bounds when setting sf3 column."<<endl; return;
		}
		_ranges[dim][index].first=low; _ranges[dim][index].second=high;_sf3_file_link[dim][index]=column;
	}

	/** Functions to get ranges for a variable dimension and index*/
	static High_Low& range(int dim, int index) {return _ranges[dim][index];}
	static double& low(int dim, int index) {return _ranges[dim][index].first;}
	static double& high(int dim, int index) {return _ranges[dim][index].second;}

	/** Links the columns in the data files to dimensions/indices in the Region/Zone data table*/
	static int& get_pums_column(int dim){return _pums_file_link[dim];}
	/** Links the columns int the data files to dimensions/indices in the Region/Zone data table*/
	static int& get_sf3_column(int dim, int index){return _sf3_file_link[dim][index];}

	static const vector<int>& get_pums_data_cols(){return _pums_file_data_cols;}
	static const vector<int>& get_person_data_cols(){return _person_file_data_cols;}

	/** Find the variable index for a given dimension-value pair*/
	static int find_index_in_dimension(int dim, double value)
	{
		for (int i=0; i<_ranges[dim].size(); i++)
		{
			if (value >= low(dim,i) && value < high(dim,i)) return i;
		}
		throw "Data outside of specified range was observed.";
	}


private:
	static vector<vector<High_Low>> _ranges;
	static vector<int> _pums_file_link;
	static vector<vector<int>> _sf3_file_link;
	static File_IO::File_Reader _fr;
	static string marg_file_path;
	static string sample_file_path;
	static vector<int> _pums_file_data_cols;

	static string person_sample_file_path;
	static vector<int> _person_file_data_cols;
};


int Linker::region_id_col;
int Linker::zone_id_col;
int Linker::region_in_zone_id_col;
int Linker::sample_id_col;
int Linker::sample_weight_col;
int Linker::person_region_id_col;
int Linker::person_sample_id_col;
vector<int> Linker::_pums_file_link;
vector<int> Linker::_pums_file_data_cols;
vector<int> Linker::_person_file_data_cols;

vector<vector<High_Low>> Linker::_ranges;
vector<vector<int>> Linker::_sf3_file_link;
File_IO::File_Reader Linker::_fr;
string Linker::marg_file_path;
string Linker::sample_file_path;
string Linker::person_sample_file_path;

void Linker::Init_Linker(vector<int> &dim_sizes, string link_file_path)
{
	zone_id_col =-1;
	region_id_col=-1;
	region_in_zone_id_col=-1;
	person_region_id_col=-1;

	_fr.Open(link_file_path, false);
	Read_Linker_File(dim_sizes);
}

void Linker::Read_Linker_File(vector<int> &dim_sizes)
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
				vector<High_Low> v_i;	
				vector<int> v_ind;

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