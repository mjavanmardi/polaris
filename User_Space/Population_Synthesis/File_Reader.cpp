#include "File_Reader.h"

namespace File_IO
{

	bool File_Reader::Open(string filepath, bool header)
	{
		// Attempt to open file
		_file.open(filepath);
		if (!_file.is_open())
		{
			cout<<"Error, file '" + filepath + "' could not be opened";
			_open=false;
		}
		else _open=true;

		// get header row
		if (header)
		{
			string line;
			getline(_file,line);
			tokenize(line,_header);
		}
		return _open;

	}

	void File_Reader::Close(void)
	{
		_file.close();
		_open=false;
	}

	bool File_Reader::Read(void)
	{
		if (!_open || _file.eof()) return false;
		string line;
		getline(_file,line);
		_string_data.clear();
		tokenize(line, _string_data, _delims);
	}

	const string& File_Reader::Get_String(int column)
	{
		return _string_data[column];
	}

	void File_Reader::tokenize(const std::string& str, vector<string>& tokens,
				  const std::string& delimiters, const bool trimEmpty)
	{
	   std::string::size_type pos, lastPos = 0;
	   while(true)
	   {
		  pos = str.find_first_of(delimiters, lastPos);
		  if(pos == std::string::npos)
		  {
			 pos = str.length();

			 if(pos != lastPos || !trimEmpty)
				tokens.push_back(vector<string>::value_type(str.data()+lastPos,
					  (vector<string>::value_type::size_type)pos-lastPos ));

			 break;
		  }
		  else
		  {
			 if(pos != lastPos || !trimEmpty)
				tokens.push_back(vector<string>::value_type(str.data()+lastPos,
					  (vector<string>::value_type::size_type)pos-lastPos ));
		  }

		  lastPos = pos + 1;
	   }
	};

}