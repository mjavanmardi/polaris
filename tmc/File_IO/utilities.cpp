#include "utilities.h"
using namespace std;

//left substring
string left(string s, int pos)
{
	s = s.substr(0,pos);
	return s;
};

//right substring
string right(string s, int pos)
{
	s = s.substr(pos,(int)s.size());
	return s;
};

//extract substring
string substring(string s, int pos1, int pos2)
{
	s = s.substr(pos1, pos2);
	return s;
};

//convert hh:mm:ss to seconds
int convert_hhmmss_to_seconds(string hhmmss)
{
	string hh,mm,ss;
	//int pos = (int) hhmmss.find(":");
	int pos = 2;
	hh = left(hhmmss,pos);
	hhmmss = right(hhmmss,pos+1);

	//pos = (int) hhmmss.find(":");
	pos = 2;
	mm = left(hhmmss,pos);
	ss = right(hhmmss,pos+1);
	int h = stoi(hh);
	int m = stoi(mm);
	int s = stoi(ss);

	int time_in_seconds = h*3600 + m*60 + s;
	
	return time_in_seconds;
};

//convert hh:mm:ss to seconds
int convert_hhmm_to_seconds(string hhmm)
{
	string hh,mm;
	int pos = 2;

	hh = left(hhmm,pos);
	mm = right(hhmm,pos+1);
	
	int h = stoi(hh);
	int m = stoi(mm);

	int time_in_seconds = h*3600 + m*60;
	
	return time_in_seconds;
};

void string_split(std::vector<std::string>& results, const std::string &source, const int fields)
{
    results.clear();
    results.resize(fields);

    const char* s = source.c_str();
	const char* s2;
    int count;
    int pos = 0;
    while (pos < fields)
    {
        count = 0;
        while(*s != '\0' && (*s == '\t' || *s == ' ' || *s == '\r' || *s == ',')) s++;
        if (*s == '\0')
        {
            break;
        }
        s2 = s;
        while(*s2 != '\0' && *s2 != '\t' && *s2 != ' ' && *s2 != '\r' && *s2 != ',')
        {
            s2++;
            count++;
        }
        results[pos].assign(s, count);
        s = s2;
        pos++;
    }
};

void string_split(std::vector<std::string>& results, const std::string &source)
{
    results.clear();

    const char* s = source.c_str();
	const char* s2;
    int count;
    while (true)
    {
        count = 0;
        while(*s != '\0' && (*s == '\t' || *s == ' ' || *s == '\r' || *s == ',')) s++;
        if (*s == '\0')
        {
            break;
        }
        s2 = s;
        while(*s2 != '\0' && *s2 != '\t' && *s2 != ' ' && *s2 != '\r' && *s2 != ',')
        {
            s2++;
            count++;
        }
        std::string str;
        str.assign(s, count);
        results.push_back(str);
        s = s2;
    }
};

//convert seconds to hh:mm:ss
string convert_seconds_to_hhmmss(int time_in_seconds)
{
	long long h = time_in_seconds/3600;
	time_in_seconds = time_in_seconds%3600;
	long long m = time_in_seconds/60;
	time_in_seconds = time_in_seconds%60;
	long long s = time_in_seconds;
	string hh = to_string(h);
	if (hh.size()==1)
	{
		hh = "0" + hh;
	}
	string mm = to_string(m);
	if (mm.size()==1)
	{
		mm = "0" + mm;
	}

	string ss = to_string(s);
	if (ss.size()==1)
	{
		ss = "0" + ss;
	}

	string hhmmss = hh + ":" + mm + ":" + ss;
	return hhmmss;
};

string convert_seconds_to_hhmm(int time_in_seconds)
{
	long long h = time_in_seconds/3600;
	time_in_seconds = time_in_seconds%3600;
	long long m = time_in_seconds/60;
	time_in_seconds = time_in_seconds%60;
	long long s = time_in_seconds;
	string hh = to_string(h);
	if (hh.size()==1)
	{
		hh = "0" + hh;
	}
	string mm = to_string(m);
	if (mm.size()==1)
	{
		mm = "0" + mm;
	}

	string ss = to_string(s);
	if (ss.size()==1)
	{
		ss = "0" + ss;
	}

	string hhmm = hh + ":" + mm;
	return hhmm;
};

void calculate_mean_standard_deviation(const vector<float>& data_array, float& mean, float& standard_deviation)
{
	int array_size = int(data_array.size());
	if (array_size>1)
	{
		mean = 0.0f;
		for (int i=0;i<array_size;i++)
		{
			mean += data_array[i];
		}
		mean = float (mean/(float(array_size)));
		standard_deviation = 0.0f;
		for (int i=0;i<array_size;i++)
		{
			standard_deviation += (data_array[i] - mean)*(data_array[i] - mean);
		}

		standard_deviation = float(sqrt(double(standard_deviation/float(array_size-1))));
	}
	else
	{
		if (array_size ==1)
		{
			mean = data_array[0];
			standard_deviation = 0.0f;
		}
		else
		{
			mean = 0.0f;
			standard_deviation = 0.0f;
		}
	}
};

void calculate_mean(const vector<float>& data_array, float& mean)
{
	int array_size = int(data_array.size());
	if (array_size>1)
	{
		mean = 0.0f;
		for (int i=0;i<array_size;i++)
		{
			mean += data_array[i];
		}
		mean = float (mean/(float(array_size)));
	}
	else
	{
		if (array_size ==1)
		{
			mean = data_array[0];
		}
		else
		{
			mean = 0.0f;
		}
	}
};

#ifdef LINUX
	double get_current_cpu_time_in_seconds()
	{
		struct timespec current_cpu_time;
		double current_cpu_time_in_seconds;
		clock_gettime(CLOCK_REALTIME, &current_cpu_time);

		current_cpu_time_in_seconds = (double)((current_cpu_time.tv_sec * 1000000000 + current_cpu_time.tv_nsec)) / 1000000000.0;
		return current_cpu_time_in_seconds;
	};
#else
	double get_current_cpu_time_in_seconds()
	{
		LARGE_INTEGER current_cpu_time;
		LARGE_INTEGER query_performance_frequency;
		double current_cpu_time_in_seconds;

		QueryPerformanceFrequency(&query_performance_frequency);
		QueryPerformanceCounter(&current_cpu_time);

		current_cpu_time_in_seconds =(double) (current_cpu_time.QuadPart/((double)query_performance_frequency.QuadPart));	
		return current_cpu_time_in_seconds;
	};
#endif
