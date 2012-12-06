#pragma once

//#include "popsyn_includes.h"
//#include "zone.h"
//#include "File_Reader.h"
////
//using namespace std;
//
////===============================================
//// Data range structure for marginal categories
////-----------------------------------------------
//struct High_Low:public pair<double,double>
//{
//	double& low(){return first;}
//	double& high(){return second;}
//};
//
////======================================================
//// Structure to hold marginal file variable definitions
////------------------------------------------------------
//struct Dim_Index:public pair<int,pair<int,int>>
//{
//	int& dimension(){return first;}			// Holds the dimension of the current marginal variable (in the Region/zone MArray
//	int& index(){return second.first;}		// Holds the index within the dimension of the current marginal
//	int& column(){return second.second;}	// position of the marginal in data file
//};
//
//
////==============================================================
////
//// linker class 
////
////	HOLDS INFO TO ENTER DATA INTO REGIONS/ZONES, FROM INPUT FILES
////
////--------------------------------------------------------------
//class Linker
//{
//public:
//	static int zone_id_col;
//	static int region_id_col;
//	static int sample_id_col;
//	static int sample_weight_col;
//	static int region_in_zone_id_col;
//
//	/** Initializer*/
//	static void Init_Linker(vector<int> &dim_sizes, string link_file_path);
//	static void Read_Linker_File(vector<int> &dim_sizes);
//	static string const * const Sample_File_Path(){ return &sample_file_path;}
//	static string const * const Marg_File_Path(){ return &marg_file_path;}
//
//	/** fill ranges*/
//	static void set_pums_id_col(int column, int sampleid_column, int weight_column) {region_id_col=column; sample_id_col = sampleid_column; sample_weight_col=weight_column;}
//	static void set_pums_col(int dim, int column) {if (dim < _pums_file_link.size()) _pums_file_link[dim] = column; else {cout<<"Error: dimension outside of bounds when setting pums column."<<endl; return;}}
//	static void set_sf3_id_col(int zone, int region) {region_in_zone_id_col=region; zone_id_col=zone;}
//	static void set_sf3_col(int dim, int index, double low, double high, int column)
//	{
//		if (dim >= _ranges.size())
//		{
//			cout<<"Error: dimension outside of bounds when setting sf3 column."<<endl; return;
//		}
//		if (index >= _ranges[dim].size())
//		{
//			cout<<"Error: index outside of dimension bounds when setting sf3 column."<<endl; return;
//		}
//		_ranges[dim][index].first=low; _ranges[dim][index].second=high;_sf3_file_link[dim][index]=column;
//	}
//
//	/** Functions to get ranges for a variable dimension and index*/
//	static High_Low& range(int dim, int index) {return _ranges[dim][index];}
//	static double& low(int dim, int index) {return _ranges[dim][index].first;}
//	static double& high(int dim, int index) {return _ranges[dim][index].second;}
//
//	/** Links the columns in the data files to dimensions/indices in the Region/Zone data table*/
//	static int& get_pums_column(int dim){return _pums_file_link[dim];}
//	/** Links the columns int the data files to dimensions/indices in the Region/Zone data table*/
//	static int& get_sf3_column(int dim, int index){return _sf3_file_link[dim][index];}
//
//	static const vector<int>& get_pums_data_cols(){return _pums_file_data_cols;}
//
//	/** Find the variable index for a given dimension-value pair*/
//	static int find_index_in_dimension(int dim, double value)
//	{
//		for (int i=0; i<_ranges[dim].size(); i++)
//		{
//			if (value >= low(dim,i) && value < high(dim,i)) return i;
//		}
//		throw "Data outside of specified range was observed.";
//	}
//
//
//private:
//	static vector<vector<High_Low>> _ranges;
//	static vector<int> _pums_file_link;
//	static vector<vector<int>> _sf3_file_link;
//	static File_IO::File_Reader _fr;
//	static string marg_file_path;
//	static string sample_file_path;
//	static vector<int> _pums_file_data_cols;
//};
