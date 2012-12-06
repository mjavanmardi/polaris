#include "linker.h"

//int Linker::region_id_col;
//int Linker::zone_id_col;
//int Linker::region_in_zone_id_col;
//int Linker::sample_id_col;
//int Linker::sample_weight_col;
//vector<int> Linker::_pums_file_link;
//vector<int> Linker::_pums_file_data_cols;
//
//vector<vector<High_Low>> Linker::_ranges;
//vector<vector<int>> Linker::_sf3_file_link;
//File_IO::File_Reader Linker::_fr;
//string Linker::marg_file_path;
//string Linker::sample_file_path;
//
//void Linker::Init_Linker(vector<int> &dim_sizes, string link_file_path)
//{
//	zone_id_col =-1;
//	region_id_col=-1;
//	region_in_zone_id_col=-1;
//
//	_fr.Open(link_file_path, false);
//	Read_Linker_File(dim_sizes);
//
//}
//
//void Linker::Read_Linker_File(vector<int> &dim_sizes)
//{
//	bool sampleid_found = false;
//	bool margid_found = false;
//	int count_sample_vars = 0;
//	int count_marg_vars = 0;
//
//	//===============================================
//	// Linker file parsing
//	//-----------------------------------------------
//	while (_fr.Read())
//	{
//		string key;
//
//		// Get the keyword from current line
//		key = _fr.Get_String(0);
//
//
//		if (key == "REGIONFILE")
//		{
//			Linker::sample_file_path = _fr.Get_String(1);
//		}
//		else if (key == "ZONEFILE")
//		{
//			Linker::marg_file_path = _fr.Get_String(1);
//		}
//		else if (key == "DIMS")
//		{
//			int dim;
//			for (int i= 1; i < _fr.Line_Length(); i++)
//			{
//				_fr.Get_Data<int>(dim, i);
//				dim_sizes.push_back(dim);
//			}
//
//			for (int i=0; i<dim_sizes.size(); i++)
//			{
//				vector<High_Low> v_i;	
//				vector<int> v_ind;
//
//				for (int j=0; j<dim_sizes[i]; j++)
//				{
//					High_Low p_j;
//					Dim_Index p_c;
//					v_i.push_back(p_j);
//					v_ind.push_back(-1);		
//				}
//				_ranges.push_back(v_i);
//				_sf3_file_link.push_back(v_ind);
//			
//				_pums_file_link.push_back(-1);
//			}
//		}
//		else if (key == "REGION")
//		{
//			int col, col2, col3;
//			if (_fr.Get_Data<int>(col, 1) && _fr.Get_Data<int>(col2,2) && _fr.Get_Data<int>(col3,3)) set_pums_id_col(col, col2, col3);
//			else {cout<<"Error: region id column not set"<<endl; return;}
//		}
//		else if (key == "REGIONDATA")
//		{
//			int size = _fr.Line_Length();
//			for (int i = 1; i<size; i++)
//			{
//				int col;
//				_fr.Get_Data<int>(col, i);
//				_pums_file_data_cols.push_back(col);
//			}
//		}
//		else if (key == "ZONE")
//		{
//			int col, col2;
//			if (_fr.Get_Data<int>(col, 1) && _fr.Get_Data<int>(col2, 2)) set_sf3_id_col(col, col2);
//			else {cout<<"Error: zone id columns not set"<<endl; return;}
//		}
//
//		else if (key == "SAMPLEVAR")
//		{
//			int col, col2;
//			if (_fr.Get_Data<int>(col, 1) && _fr.Get_Data<int>(col2, 2)) set_pums_col(col, col2);
//			else {cout<<"Error: region data columns not set"<<endl; return;}
//		}
//		else if (key == "MARGVAR")
//		{
//			int c1, c2, c3, c4, c5;
//			if (_fr.Get_Data<int>(c1, 1) && _fr.Get_Data<int>(c2, 2) && _fr.Get_Data<int>(c3, 3) && _fr.Get_Data<int>(c4, 4) && _fr.Get_Data<int>(c5, 5)) set_sf3_col(c1, c2,c3, c4, c5);
//			else {cout<<"Error: zone data columns not set"<<endl; return;}
//		}
//	}
//}