#ifndef FILE_WRITER_H
#define FILE_WRITER_H

#include "File_IO_includes.h"


using namespace std;

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

}
#endif