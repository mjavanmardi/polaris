#include "File_Writer.h"
//
//namespace File_IO
//{
//
//	bool File_Writer::Open(string filepath)
//	{
//		// Attempt to open file
//		_file.open(filepath, ios::out);
//		if (!_file.is_open())
//		{
//			cout<<"Error, file '" + filepath + "' could not be opened";
//			_open=false;
//		}
//		else _open=true;
//
//		return _open;
//
//	}
//
//	void File_Writer::Close(void)
//	{
//		_file.close();
//		_open=false;
//	}
//
//	void File_Writer::Write(stringstream &S)
//	{
//		_file<<S.str()<<DELIM;
//		S.str("");
//	}
//
//	void File_Writer::Write_Line(stringstream &S)
//	{
//	
//		_file<<S.str()<<endl;
//		S.str("");
//	}
//
//}
