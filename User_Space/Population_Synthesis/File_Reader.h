#pragma once

using namespace std;

namespace File_IO
{

	//---------------------------------------------------------
	//	FILE READER (i.e. PUMA, SF3, etc.)
	//---------------------------------------------------------

	class File_Reader
	{
	public:
		File_Reader(string delims = ",\t"){_open=false;_delims = delims;}
		bool Open(string filepath, bool header=true);
		void Close();
		bool Read(void);
		int Get_Int(int column);
		template<class T>
		bool Get_Data(T& t, int column);
		template<class T>
		bool Get_Data(vector<T> &data, vector<int> columns);
		const string& Get_String(int column);
		int Line_Length(){return _string_data.size();}

	protected:
		ifstream _file;
		vector<double> _data;
		vector<string> _string_data;
		vector<string> _header;
		bool _open;
		string _delims;
		void tokenize(const string& str, vector<string>& tokens, const string& delimiters = ",\t", const bool trimEmpty = false);
	};

	template<class T>
	bool File_Reader::Get_Data(T& t, int column)
	{
		if (column >= _string_data.size()) 
		{
			return false;
		}
		istringstream iss(_string_data[column]);
		return iss>>t;
	}

	template<class T>
	bool File_Reader::Get_Data(vector<T> &data, vector<int> columns)
	{
		T t;
		for (vector<int>::iterator itr = columns.begin(); itr != columns.end(); itr++)
		{
			if (Get_Data<T>(t,*itr)) data.push_back(t);
			else {cout<<"ERROR: could not get data from column"; return false;}
		}
		return true;

	}
}
