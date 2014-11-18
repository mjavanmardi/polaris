#pragma once
#include "Repository_Includes.h"
#include <iostream>
#include <fstream>

namespace File_IO
{

	//---------------------------------------------------------
	//	FILE READER 
	//---------------------------------------------------------

	class Binary_File_Reader
	{
	public:
		bool Open(string filepath)
		{
			// Attempt to open file
			_file.open(filepath, ios_base::binary);
			if (!_file.is_open())
			{
				cout<<"Error, file '" + filepath + "' could not be opened";
				_open=false;
			}
			else _open=true;

			return _open;
		}
		void Begin()
		{
			_file.seekg(0);
		}
		void Close()
		{
			_file.close();
			_open=false;
		}
		template<class T>
		bool Read_Value(T& t)
		{
			_file.read((char*)&t, sizeof(T));
			if (!_file){ _file.clear(); return false;}
			return true;
		}
		template<class T>
		bool Read_Value(T& t, long long seek_byte)
		{
			_file.seekg(seek_byte);
			_file.read((char*)&t, sizeof(T));
			if (!_file){ _file.clear(); return false;}
			return true;
		}
		template<class T>
		bool Read_Value(T& t, int offset, ios_base::seekdir dir)
		{
			_file.seekg(offset, dir);
			_file.read((char*)&t, sizeof(T));
			if (!_file){ _file.clear(); return false;}
			return true;
		}
		template<class T>
		bool Read_Array(T* t, int num_to_read)
		{
			int size = sizeof(T) * num_to_read;
			_file.read((char*)t, size);
			if (!_file){ _file.clear(); return false;}
			return true;
		}

		char Version(){return _version;}
		void Version(char version){_version=version;}

	protected:
		ifstream _file;
		bool _open;
		char _version;
	};

	class File_Reader
	{
	public:
		bool Open(string filepath, bool header=true, string delims = ",\t")
		{
			_filename = filepath;
			_open=false;
			_delims = delims;
			//Attempt to open file
			_file.open(filepath);
			if (!_file.is_open())
			{
				THROW_EXCEPTION("Error, file '" + filepath + "' could not be opened");
				_open=false;
			}
			else _open=true;

			//get header row
			if (header)
			{
				_header.clear();
				string line;
				getline(_file,line);
				tokenize(line,_header);
			}
			return _open;

		}
		void Close()
		{
			_file.close();
			_open=false;
		}
		bool Read(void)
		{
			bool ret_val = true;
			if (!_open || _file.eof()) 
			{
				ret_val = false;
				return ret_val;
			}
			string line;
			std::getline(_file,line);
			_string_data.clear();
			tokenize(line, _string_data, _delims);
			if (_string_data.size() < _header.size())
			{
				return Read();
			}
			return ret_val;
		}
		int Get_Int(int column);
		template<class T>
		bool Get_Data(T& t, int column)
		{
			if (column >= _string_data.size()) 
			{
				return false;
			}
			istringstream iss(_string_data[column]);
			iss>>t;
			return true;
		}
		template<class T>
		bool Get_Data(boost::container::vector<T> &data, boost::container::vector<int> columns)
		{
			T t;
			for (boost::container::vector<int>::iterator itr = columns.begin(); itr != columns.end(); itr++)
			{
				if (Get_Data<T>(t,*itr)) data.push_back(t);
				else {/*cout<<"ERROR: could not get data from column"; */return false;}
			}
			return true;

		}
		template<class T>
		bool Get_Data(boost::container::vector<T> &data, std::vector<int> columns)
		{
			T t;
			for (std::vector<int>::iterator itr = columns.begin(); itr != columns.end(); itr++)
			{
				if (Get_Data<T>(t,*itr)) data.push_back(t);
				else {/*cout<<"ERROR: could not get data from column"; */return false;}
			}
			return true;

		}
		template<class T>
		bool Get_Data(std::vector<T> &data, std::vector<int> columns)
		{
			T t;
			for (std::vector<int>::iterator itr = columns.begin(); itr != columns.end(); itr++)
			{
				if (Get_Data<T>(t,*itr)) data.push_back(t);
				else {/*cout<<"ERROR: could not get data from column"; */return false;}
			}
			return true;

		}
		const string& Get_String(int column)
		{
			return _string_data[column];
		}
		int Line_Length(){return (int)_string_data.size();}

	protected:
		ifstream _file;
		string _filename;
		boost::container::vector<double> _data;
		boost::container::vector<string> _string_data;
		boost::container::vector<string> _header;
		bool _open;
		string _delims;
		void tokenize(const string& str, boost::container::vector<string>& tokens, const string& delimiters = ",\t\r", const bool trimEmpty = false)
		{
			string::size_type pos, lastPos = 0;
			while(true)
			{
				pos = str.find_first_of(delimiters, lastPos);
				if(pos == string::npos)
				{
					pos = str.length();
					if(pos != lastPos || !trimEmpty)
					tokens.push_back(boost::container::vector<string>::value_type(str.data()+lastPos,
							(boost::container::vector<string>::value_type::size_type)pos-lastPos ));

					break;
				}
				else
				{
					if(pos != lastPos || !trimEmpty)
					tokens.push_back(boost::container::vector<string>::value_type(str.data()+lastPos,
							(boost::container::vector<string>::value_type::size_type)pos-lastPos ));
				}

				lastPos = pos + 1;
			}
		};
	};

}
