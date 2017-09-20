#pragma once
#include "Repository_Includes.h"
#include <iostream>
#include <fstream>

namespace File_IO
{
	struct File_Info
	{
		File_Info(char* filepath)
		{
			_full_name.assign(filepath);
			//std::replace(_full_name.begin(), _full_name.end(),'\\','/');
			int len = _full_name.length();

			// get path
			int pathpos = _full_name.find_last_of("\\/");
			if (pathpos != _full_name.npos)
			{
				_path = _full_name.substr(0,pathpos);
			}
			else
			{
				pathpos = -1;
				_path.assign("");
			}

			// get extension	
			int pos = _full_name.find('.');
			if (pos != std::string::npos)
			{
				if (pos <= pathpos){THROW_EXCEPTION("ERROR: file "<<filepath<<" improperly specified.");}			
				_extension = _full_name.substr(pos+1,len-pos-1);
			}
			else
			{
				pos = len;
				_extension.assign("");
			}

			_name = _full_name.substr(pathpos+1,pos-pathpos-1);

			// if sqlite database, find higher level db name (i.e. strip off -Supply, -Result, etc.)
			if (Is_File_Type("sqlite"))
			{
				int dbpos = _full_name.find('-');
				if (dbpos != std::string::npos)
				{
					_db_name = _full_name.substr(0,dbpos);
				}
				else
				{
					_db_name.assign("");
				}
			}
		}
		bool Is_File_Type(const char* extension)
		{
			return (strcmp(_extension.c_str(),extension) == 0);
		}
		bool Has_Extension()
		{
			return !_extension.empty();
		}
		string full_name()
		{
			return _full_name;
		}
		string path()
		{
			return _path;
		}
		string name()
		{
			return _name;
		}
		string db_name()
		{
			return _db_name;
		}
		string extension()
		{
			return _extension;
		}
	private:
		string _path;
		string _name;
		string _extension;
		string _full_name;
		string _db_name;
	};

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
			if (_string_data.size() < _header.size() && _header.size() > 0)
			{
				return Read();
			}
			return ret_val;
		}
		int Get_Int(int column)
		{
			int x;
			if (column >= _string_data.size()) 
			{
				return false;
			}
			istringstream iss(_string_data[column]);
			iss>>x;
			return x;
		}
		float Get_Float(int column)
		{
			int x;
			if (column >= _string_data.size())
			{
				THROW_EXCEPTION("ERROR, requested column is out of bounds for file");
			}
			istringstream iss(_string_data[column]);
			iss >> x;
			return x;
		}
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
		T Get_Data(int column)
		{
			T ret_val;
			if (column >= _string_data.size())
			{
				THROW_EXCEPTION("ERROR, requested column is out of bounds for file");
			}
			istringstream iss(_string_data[column]);
			iss >> ret_val;
			return ret_val;
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
		std::vector<double> _data;
		std::vector<string> _string_data;
		std::vector<string> _header;
		bool _open;
		string _delims;
		void tokenize(const string& str, std::vector<string>& tokens, const string& delimiters = ",\t\r", const bool trimEmpty = false)
		{
			string::size_type pos, lastPos = 0;
			while(true)
			{
				pos = str.find_first_of(delimiters, lastPos);
				if(pos == string::npos)
				{
					pos = str.length();
					if(pos != lastPos || !trimEmpty)
					tokens.push_back(std::vector<string>::value_type(str.data()+lastPos,
							(std::vector<string>::value_type::size_type)pos-lastPos ));

					break;
				}
				else
				{
					if(pos != lastPos || !trimEmpty)
					tokens.push_back(std::vector<string>::value_type(str.data()+lastPos,
							(std::vector<string>::value_type::size_type)pos-lastPos ));
				}

				lastPos = pos + 1;
			}
		};
	};

}
