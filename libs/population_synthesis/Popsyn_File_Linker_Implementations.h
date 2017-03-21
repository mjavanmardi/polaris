#pragma once

#include "Popsyn_File_Linker_Prototype.h"



namespace PopSyn
{
	namespace Implementations
	{
		implementation struct Popsyn_File_Linker_Implementation : public Polaris_Component< MasterType,INHERIT(Popsyn_File_Linker_Implementation), Data_Object>
		{
			typedef Polaris_Component< MasterType,INHERIT(Popsyn_File_Linker_Implementation), Data_Object> BaseType;
			using typename BaseType::Component_Type;
			typedef Component_Type ComponentType;

			// Variables to store the links between the SF and PUMS files at the household level
			m_container(std::vector<std::vector<High_Low>>,ranges,NONE,NONE);
			m_container(std::vector<int>,pums_file_link,NONE,NONE);
			m_container(std::vector<std::vector<int>>, sf3_file_link, NONE,NONE);				
			m_container(std::vector<int>, hh_dimension_sizes, NONE,NONE);
			m_container(std::vector<int>, pums_file_data_cols, NONE,NONE);

			// HH level test variable definition
			m_container(std::vector<High_Low>,test_ranges,NONE,NONE);
			m_container(int,test_pums_file_link,NONE,NONE);
			m_container(std::vector<int>, test_sf3_file_link, NONE,NONE);	
			m_container(int, test_dimension_size, NONE,NONE);

			// Variables to store the links between the SF and PUMS files at the person level
			m_container(std::vector<std::vector<High_Low>>,person_ranges,NONE,NONE);
			m_container(std::vector<int>,person_pums_file_link,NONE,NONE);
			m_container(std::vector<std::vector<int>>, person_sf3_file_link, NONE,NONE);	
			m_container(std::vector<int>, person_dimension_sizes, NONE,NONE);
			m_container(std::vector<int>, person_file_data_cols, NONE,NONE);

			// Person level test variable definition
			m_container(std::vector<High_Low>,test_person_ranges,NONE,NONE);
			m_container(int,test_person_pums_file_link,NONE,NONE);
			m_container(std::vector<int>, test_person_sf3_file_link, NONE,NONE);	
			m_container(int, test_person_dimension_size, NONE,NONE);


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
			m_data(int,person_weight_column,NONE,NONE);
			m_data(int,region_in_zone_id_column,NONE,NONE);
			m_data(int,number_of_hh_dimensions,NONE,NONE);
			m_data(int,number_of_person_dimensions,NONE,NONE);

			void set_pums_data_column(int dimension_number, int column_number, bool household, bool test_variable)
			{
				if (household)
				{
					if (!test_variable)
					{
						if (dimension_number < _pums_file_link.size())
						{
							_pums_file_link[dimension_number] = column_number;
						}
						else 
						{
							cout<<"Error: dimension number '"<<dimension_number<<"' outside of bounds '"<<_pums_file_link.size() <<"' when setting pums column."<<endl; return;
						}
					}
					else
					{
						_test_pums_file_link = column_number;
					}
				}
				else
				{
					if (!test_variable)
					{
						if (dimension_number < _person_pums_file_link.size())
						{
							_person_pums_file_link[dimension_number] = column_number;
						}
						else 
						{
							cout<<"Error: dimension'"<<dimension_number<<"' outside of bounds '"<<_person_pums_file_link.size() <<"' when setting person pums column."<<endl; return;
						}
					}
					else
					{
						_test_person_pums_file_link = column_number;
					}
				}
			}
			void set_sf3_data_column(int dimension_number, int index_in_dimension, double low_value, double high_value, int column_number, bool household, bool test_variable)
			{
				if (household)
				{
					if (!test_variable)
					{
						if (dimension_number >= _ranges.size()){THROW_EXCEPTION("Error: dimension '"<<dimension_number<<"' outside of bounds '"<<_ranges.size() <<"' when setting sf3 column.");}
						if (index_in_dimension >= _ranges[dimension_number].size()){THROW_EXCEPTION("Error: index '"<<index_in_dimension<<"' outside of dimension bounds '"<<_ranges[dimension_number].size() <<"' when setting sf3 column for dimension '"<<dimension_number<<"'.");}
						_ranges[dimension_number][index_in_dimension].first=low_value; 
						_ranges[dimension_number][index_in_dimension].second=high_value;
						_sf3_file_link[dimension_number][index_in_dimension]=column_number;
					}
					else
					{
						if (index_in_dimension >= _test_ranges.size()) {THROW_EXCEPTION("Error: index outside of dimension bounds when setting test sf3 column.");}
						_test_ranges[index_in_dimension].first=low_value; 
						_test_ranges[index_in_dimension].second=high_value;
						_test_sf3_file_link[index_in_dimension]=column_number;
					}
				}
				else
				{
					if (!test_variable)
					{
						if (dimension_number >= _person_ranges.size()){THROW_EXCEPTION("Error: dimension '"<<dimension_number<<"' outside of bounds '"<<_person_ranges.size() <<"' when setting person sf3 column.");}
						if (index_in_dimension >= _person_ranges[dimension_number].size()){THROW_EXCEPTION("Error: index '"<<index_in_dimension<<"' outside of dimension bounds '"<<_person_ranges[dimension_number].size() <<"' when setting person sf3 column for dimension '"<<dimension_number<<"'.");}
						_person_ranges[dimension_number][index_in_dimension].first=low_value; 
						_person_ranges[dimension_number][index_in_dimension].second=high_value;
						_person_sf3_file_link[dimension_number][index_in_dimension]=column_number;
					}
					else
					{
						if (index_in_dimension >= _test_person_ranges.size()) {THROW_EXCEPTION("Error: index outside of dimension bounds when setting test sf3 column.");}
						_test_person_ranges[index_in_dimension].first=low_value; 
						_test_person_ranges[index_in_dimension].second=high_value;
						_test_person_sf3_file_link[index_in_dimension]=column_number;
					}
				}
			}

			High_Low& range(int dim, int index, bool household) 
			{
				if (household)
				{
					_ranges[dim][index];
				}
				else
				{
					_person_ranges[dim][index];
				}
			}
			double low(int dim, int index, bool household, bool test_variable) 
			{
				if (!test_variable)
				{
					if (household)	return _ranges[dim][index].first;
					else			return _person_ranges[dim][index].first;
				}
				else
				{
					if (household)	return _test_ranges[index].first;
					else			return _test_person_ranges[index].first;
				}
			}
			double high(int dim, int index, bool household, bool test_variable) 
			{
				if (!test_variable)
				{
					if (household)	return _ranges[dim][index].second;
					else			return _person_ranges[dim][index].second;
				}
				else
				{
					if (household)	return _test_ranges[index].second;
					else			return _test_person_ranges[index].second;
				}
			}

			/** Links the columns in the data files to dimensions/indices in the Region/Zone data table*/
			int get_pums_column(int dim, bool household, bool test_variable)
			{
				if (household)
				{
					if (!test_variable) return _pums_file_link[dim];
					else return _test_pums_file_link;
				}
				else
				{
					if (!test_variable) return _person_pums_file_link[dim];
					else return _test_person_pums_file_link;
				}
			}

			/** Links the columns int the data files to dimensions/indices in the Region/Zone data table*/
			int get_sf3_column(int dim, int index, bool household, bool test_variable)
			{
				if (household)
				{
					if (!test_variable) return _sf3_file_link[dim][index];
					else return _test_sf3_file_link[index];
				}
				else
				{
					if (!test_variable) return _person_sf3_file_link[dim][index];
					else return _test_person_sf3_file_link[index];
				}
			}

			const std::vector<int>& get_pums_data_columns()
			{
				return _pums_file_data_cols;
			}
			const std::vector<int>& get_person_data_columns()
			{
				return _person_file_data_cols;
			}

			/** Find the variable index for a given dimension-value pair*/
			int find_index_in_dimension(int dim, double value, bool household, bool test_variable)
			{
				if (household)
				{
					if (!test_variable)
					{
						for (int i=0; i<_ranges[dim].size(); i++)
						{
							if (value >= low(dim,i, household,false) && value < high(dim,i,household,false)) return i;
						}
					}
					else
					{
						for (int i=0; i<_test_ranges.size(); i++)
						{
							if (value >= low(dim,i, household,true) && value < high(dim,i,household,true)) return i;
						}
					}
				}
				else
				{
					if (!test_variable)
					{
						for (int i=0; i<_person_ranges[dim].size(); i++)
						{
							if (value >= low(dim,i, household,false) && value < high(dim,i,household,false)) return i;
						}
					}
					else
					{
						for (int i=0; i<_test_person_ranges.size(); i++)
						{
							if (value >= low(dim,i, household,true) && value < high(dim,i,household,true)) return i;
						}
					}
				}
				THROW_EXCEPTION( "Data outside of specified range was observed for dimension='"<<dim<<"' and value='"<<value<<"'.");
				return -1;
			}

			void Initialize(string& link_file_path)
			{
				_zone_id_column =-1;
				_region_id_column=-1;
				_region_in_zone_id_column=-1;
				_person_region_id_column=-1;
				_test_dimension_size = 0;
				_test_person_dimension_size=0;
				_test_pums_file_link = -1;
				_test_person_pums_file_link = -1;

				_fr.Open(link_file_path, false,",\t");
				Read_Linker_File();
				_number_of_hh_dimensions = (int)_hh_dimension_sizes.size();
				_number_of_person_dimensions = (int)_person_dimension_sizes.size();
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


					if (key == "HHFILE")
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
					else if (key == "HHDIMS")
					{
						int dim;
						for (int i= 1; i < _fr.Line_Length(); i++)
						{
							_fr.Get_Data<int>(dim, i);
							_hh_dimension_sizes.push_back(dim);
						}

						for (int i=0; i<_hh_dimension_sizes.size(); i++)
						{
							std::vector<High_Low> v_i;	
							std::vector<int> v_ind;

							for (int j=0; j<_hh_dimension_sizes[i]; j++)
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
					else if (key == "TESTHHDIMS")
					{
						int dim;
						for (int i= 1; i < _fr.Line_Length(); i++)
						{
							_fr.Get_Data<int>(dim, i);
							_test_dimension_size = dim;
						}

						for (int j=0; j<_test_dimension_size; j++)
						{
							High_Low p_j;
							Dim_Index p_c;
							_test_ranges.push_back(p_j);
							_test_sf3_file_link.push_back(-1);		
						}
			
						_test_pums_file_link = -1;

					}
					else if (key == "PERSONDIMS")
					{
						int dim;
						for (int i= 1; i < _fr.Line_Length(); i++)
						{
							_fr.Get_Data<int>(dim, i);
							_person_dimension_sizes.push_back(dim);
						}

						for (int i=0; i<_person_dimension_sizes.size(); i++)
						{
							std::vector<High_Low> v_i;	
							std::vector<int> v_ind;

							for (int j=0; j<_person_dimension_sizes[i]; j++)
							{
								High_Low p_j;
								Dim_Index p_c;
								v_i.push_back(p_j);
								v_ind.push_back(-1);		
							}
							_person_ranges.push_back(v_i);
							_person_sf3_file_link.push_back(v_ind);
			
							_person_pums_file_link.push_back(-1);
						}
					}
					else if (key == "TESTPERSONDIMS")
					{
						int dim;
						for (int i= 1; i < _fr.Line_Length(); i++)
						{
							_fr.Get_Data<int>(dim, i);
							_test_person_dimension_size = dim;
						}

						for (int j=0; j< _test_person_dimension_size; j++)
						{
							High_Low p_j;
							Dim_Index p_c;
							_test_person_ranges.push_back(p_j);
							_test_person_sf3_file_link.push_back(-1);		
						}

						_test_person_pums_file_link = -1;

					}
					else if (key == "REGION")
					{
						int col, col2, col3;
						if (_fr.Get_Data<int>(col, 1) && _fr.Get_Data<int>(col2,2) && _fr.Get_Data<int>(col3,3)) linker->set_pums_columns(col, col2, col3);
						else {cout<<"Error: region id column not set"<<endl; return;}
					}
					else if (key == "PERSON")
					{
						int col, col2, col3;
						if (_fr.Get_Data<int>(col, 1) && _fr.Get_Data<int>(col2,2) && _fr.Get_Data<int>(col3,3)) linker->set_person_columns(col, col2, col3);
						else {cout<<"Error: person region, sample and weight column not set"<<endl; return;}
					}
					else if (key == "HHDATA")
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
					else if (key == "HHVAR")
					{
						int col, col2;
						if (_fr.Get_Data<int>(col, 1) && _fr.Get_Data<int>(col2, 2)) linker->set_pums_data_column(col, col2);
						else {cout<<"Error: region data columns not set"<<endl; return;}
					}
					else if (key == "TESTHHVAR")
					{
						int col, col2;
						if (_fr.Get_Data<int>(col, 1) && _fr.Get_Data<int>(col2, 2)) linker->set_pums_data_column(col, col2,true,true);
						else {cout<<"Error: household test variable"<<endl; return;}
					}
					else if (key == "PERSONVAR")
					{
						int col, col2;
						if (_fr.Get_Data<int>(col, 1) && _fr.Get_Data<int>(col2, 2)) linker->set_pums_data_column(col, col2,false);
						else {cout<<"Error: region data columns not set"<<endl; return;}
					}
					else if (key == "TESTPERSONVAR")
					{
						int col, col2;
						if (_fr.Get_Data<int>(col, 1) && _fr.Get_Data<int>(col2, 2)) linker->set_pums_data_column(col, col2,false,true);
						else {cout<<"Error: person test variable"<<endl; return;}
					}
					else if (key == "HHMARGVAR")
					{
						int c1, c2, c3, c4, c5;
						if (_fr.Get_Data<int>(c1, 1) && _fr.Get_Data<int>(c2, 2) && _fr.Get_Data<int>(c3, 3) && _fr.Get_Data<int>(c4, 4) && _fr.Get_Data<int>(c5, 5)) linker->set_sf3_data_column(c1, c2,c3, c4, c5);
						else {cout<<"Error: zone data columns not set"<<endl; return;}
					}
					else if (key == "TESTHHMARGVAR")
					{
						int c1, c2, c3, c4, c5;
						if (_fr.Get_Data<int>(c1, 1) && _fr.Get_Data<int>(c2, 2) && _fr.Get_Data<int>(c3, 3) && _fr.Get_Data<int>(c4, 4) && _fr.Get_Data<int>(c5, 5)) linker->set_sf3_data_column(c1, c2,c3, c4, c5,true,true);
						else {cout<<"Error: zone data columns not set"<<endl; return;}
					}
					else if (key == "PERSONMARGVAR")
					{
						int c1, c2, c3, c4, c5;
						if (_fr.Get_Data<int>(c1, 1) && _fr.Get_Data<int>(c2, 2) && _fr.Get_Data<int>(c3, 3) && _fr.Get_Data<int>(c4, 4) && _fr.Get_Data<int>(c5, 5)) linker->set_sf3_data_column(c1, c2,c3, c4, c5,false);
						else {cout<<"Error: zone data columns not set"<<endl; return;}
					}
					else if (key == "TESTPERSONMARGVAR")
					{
						int c1, c2, c3, c4, c5;
						if (_fr.Get_Data<int>(c1, 1) && _fr.Get_Data<int>(c2, 2) && _fr.Get_Data<int>(c3, 3) && _fr.Get_Data<int>(c4, 4) && _fr.Get_Data<int>(c5, 5)) linker->set_sf3_data_column(c1, c2,c3, c4, c5,false,true);
						else {cout<<"Error: zone data columns not set"<<endl; return;}
					}
					else if (key.empty()){}
					else if (key.front() == '#'){}
					else
					{
						THROW_EXCEPTION("Error, unrecognized keyword '" << key <<"' in population synthesis settings file.");
					}
				}
				_fr.Close();

				// Validate household variable definitions
				for (int i=0; i < this->_pums_file_link.size();++i)
				{
					if (this->_pums_file_link[i] < 0) THROW_EXCEPTION("Error, household pums file dimension '"<<i<<"' was not defined.  Please add to linker file using the 'HHVAR' keyword");
				}
				for (int i=0; i < this->_sf3_file_link.size();++i)
				{
					for (int j=0; j < this->_sf3_file_link[i].size(); ++j)
					{
						if (this->_sf3_file_link[i][j] < 0) THROW_EXCEPTION("Error, household sf3 file dimension,index= '"<<i<<","<<j<<"' was not defined.  Please add to linker file using the 'HHMARGVAR' keyword");
					}
				}

				// Validate person variable definitions
				for (int i=0; i < this->_person_pums_file_link.size();++i)
				{
					if (this->_person_pums_file_link[i] < 0) THROW_EXCEPTION("Error, person pums file dimension '"<<i<<"' was not defined.  Please add to linker file using the 'PERSONVAR' keyword");
				}
				for (int i=0; i < this->_person_sf3_file_link.size();++i)
				{
					for (int j=0; j < this->_person_sf3_file_link[i].size(); ++j)
					{
						if (this->_person_sf3_file_link[i][j] < 0) THROW_EXCEPTION("Error, household sf3 file dimension,index= '"<<i<<","<<j<<"' was not defined.  Please add to linker file using the 'PERSONMARGVAR' keyword");
					}
				}
			}
		};
	}
}
