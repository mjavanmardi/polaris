#pragma once

#include "User_Space_Includes.h"


namespace File_IO
{

	//---------------------------------------------------------
	//	FILE READER (i.e. PUMA, SF3, etc.)
	//---------------------------------------------------------

	class File_Writer
	{
	public:
		File_Writer(){_open=false; }
		bool Open(string filepath);
		void Close();
		void Write(stringstream &S);
		void Write_Line(stringstream &S);
		void Write_Line(void){_file<<endl;}
		static const char DELIM = '\t';

	protected:
		ofstream _file;
		bool _open;
	
	};

	bool File_Writer::Open(string filepath)
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

	void File_Writer::Close(void)
	{
		_file.close();
		_open=false;
	}

	void File_Writer::Write(stringstream &S)
	{
		_file<<S.str()<<DELIM;
		S.str("");
	}

	void File_Writer::Write_Line(stringstream &S)
	{
	
		_file<<S.str()<<endl;
		S.str("");
	}
}