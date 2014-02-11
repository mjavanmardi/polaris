#pragma once

#include "Popsyn_File_Linker_Prototype.h"



namespace PopSyn
{
	namespace Implementations
	{
		implementation struct Popsyn_File_Linker_Implementation : public Polaris_Component< MasterType,INHERIT(Popsyn_File_Linker_Implementation), Data_Object>
		{
			m_container(boost::container::vector<boost::container::vector<High_Low>>,ranges,NONE,NONE);
			m_container(boost::container::vector<int>,pums_file_link,NONE,NONE);
			m_container(boost::container::vector<boost::container::vector<int>>, sf3_file_link, NONE,NONE);
			m_container(boost::container::vector<int>, pums_file_data_cols, NONE,NONE);
			m_container(boost::container::vector<int>, person_file_data_cols, NONE,NONE);
			m_container(boost::container::vector<int>, dimension_sizes, NONE,NONE);

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
			m_data(int,number_of_dimensions,NONE,NONE);


			void set_pums_data_column(int dimension_number, int column_number)
			{
				if (dimension_number < _pums_file_link.size())
				{
					_pums_file_link[dimension_number] = column_number;
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
				if (index_in_dimension >= _ranges[dimension_number].size())
				{
					cout<<"Error: index outside of dimension bounds when setting sf3 column."<<endl; return;
				}
				_ranges[dimension_number][index_in_dimension].first=low_value; _ranges[dimension_number][index_in_dimension].second=high_value;_sf3_file_link[dimension_number][index_in_dimension]=column_number;
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

			void Initialize(string link_file_path)
			{
				_zone_id_column =-1;
				_region_id_column=-1;
				_region_in_zone_id_column=-1;
				_person_region_id_column=-1;

				_fr.Open(link_file_path, false,",\t");
				Read_Linker_File();
				_number_of_dimensions = (int)_dimension_sizes.size();
			}

			void Read_Linker_File()
			{
				typedef PopSyn::Prototypes::Popsyn_File_Linker<ComponentType> linker_itf;
				linker_itf* linker = (linker_itf*)this;

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
						_sample_file_path = _fr.Get_String(1);
					}
					else if (key == "PERSONFILE")
					{
						_person_sample_file_path = _fr.Get_String(1);
					}
					else if (key == "ZONEFILE")
					{
						_marg_file_path = _fr.Get_String(1);
					}
					else if (key == "DIMS")
					{
						int dim;
						for (int i= 1; i < _fr.Line_Length(); i++)
						{
							_fr.Get_Data<int>(dim, i);
							_dimension_sizes.push_back(dim);
						}

						for (int i=0; i<_dimension_sizes.size(); i++)
						{
							boost::container::vector<High_Low> v_i;	
							boost::container::vector<int> v_ind;

							for (int j=0; j<_dimension_sizes[i]; j++)
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
						if (_fr.Get_Data<int>(col, 1) && _fr.Get_Data<int>(col2,2) && _fr.Get_Data<int>(col3,3)) linker->set_pums_columns(col, col2, col3);
						else {cout<<"Error: region id column not set"<<endl; return;}
					}
					else if (key == "PERSON")
					{
						int col, col2;
						if (_fr.Get_Data<int>(col, 1) && _fr.Get_Data<int>(col2,2)) linker->set_person_columns(col, col2);
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
						if (_fr.Get_Data<int>(col, 1) && _fr.Get_Data<int>(col2, 2)) linker->set_sf3_columns(col, col2);
						else {cout<<"Error: zone id columns not set"<<endl; return;}
					}

					else if (key == "SAMPLEVAR")
					{
						int col, col2;
						if (_fr.Get_Data<int>(col, 1) && _fr.Get_Data<int>(col2, 2)) linker->set_pums_data_column(col, col2);
						else {cout<<"Error: region data columns not set"<<endl; return;}
					}
					else if (key == "MARGVAR")
					{
						int c1, c2, c3, c4, c5;
						if (_fr.Get_Data<int>(c1, 1) && _fr.Get_Data<int>(c2, 2) && _fr.Get_Data<int>(c3, 3) && _fr.Get_Data<int>(c4, 4) && _fr.Get_Data<int>(c5, 5)) linker->set_sf3_data_column(c1, c2,c3, c4, c5);
						else {cout<<"Error: zone data columns not set"<<endl; return;}
					}
				}
				_fr.Close();
			}
		};
	}
}
