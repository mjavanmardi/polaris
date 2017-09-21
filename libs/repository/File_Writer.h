#pragma once

#include "Repository_Includes.h"


namespace File_IO
{
	class Binary_File_Writer
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
		void Close()
		{
			_file.close();
			_open=false;
		}

		template<class T>
		bool Write_Value(T& t)
		{
			_file.write((char*)&t, sizeof(T));
			//if (!_file){ _file.clear(); return false;}
			return true;
		}
		template<class T>
		bool Write_Array(T* t, int num_to_write)
		{
			_file.write((char*)t, sizeof(T) * num_to_write);
			_file.flush();
			//if (!_file){ _file.clear(); return false;}
			return true;
		}

	protected:
		ofstream _file;
		bool _open;
	
	};

	class File_Writer
	{
	public:
		File_Writer(){_open=false; }
		bool Open(string filepath)
		{
			// Attempt to open file
			_file.open(filepath, ios::out);
			if (!_file.is_open())
			{
				cout<<"Error, file '" + filepath + "' could not be opened";
				_open=false;
			}
			else _open=true;

			return _open;

		}
		void Close()
		{
			_file.close();
			_open=false;
		}
		void Write(stringstream &S)
		{
			_file<<S.str()<<DELIM;
			S.str("");
			S.clear();
		}
		//template<typename T>
		//void Write(T &data)
		//{
		//	stringstream S("");
		//	S<<data;
		//	_file<<S.str()<<DELIM;
		//}
		template<typename T>
		void Write(T data)
		{
			stringstream S("");
			S<<data;
			_file<<S.str()<<DELIM;
		}
		void Write_NoDelim(stringstream& S)
		{
			_file << S.str();
			S.str("");
			S.clear();
		}
		void Write_Line(stringstream &S)
		{
			_file<<S.str()<<endl;
			S.str("");
			S.clear();
		}
		template<typename T>
		void Write_Line(T &data)
		{
			stringstream S("");
			S<<data;
			_file<<S.str()<<endl;
		}
		void Write_Line(const char* line)
		{
			_file<<line<<endl;
		}
		void Write_Line(void)
		{_file<<endl;}
		static const char DELIM = '\t';
		ofstream& file_ptr()
		{
			return _file;
		}

	protected:
		ofstream _file;
		bool _open;
	
	};
}
