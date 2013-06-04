#pragma once
#include "Repository_Includes.h"

namespace File_IO
{

	//---------------------------------------------------------
	//	FILE READER (i.e. PUMA, SF3, etc.)
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
		bool Read_Array(T* t, int num_to_read)
		{
			_file.read((char*)t, sizeof(T) * num_to_read);
			if (!_file){ _file.clear(); return false;}
			return true;
		}
	protected:
		ifstream _file;
		bool _open;
	};

}