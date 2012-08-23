#ifndef DEMAND_API_H
#define DEMAND_API_H
#include <vector>
#include <hash_map>

using namespace std;


/// Time class used in simulation
class Time
{
public:
	Time();
	Time(int tenth_seconds);
	Time(double decimal_day);
	Time(string HHMMSS);

	bool operator>(const Time& obj);
	bool operator<(const Time& obj);
	bool operator==(const Time& obj);
	bool operator!=(const Time& obj);

	string To_String();

	char Day();
	char Hour();
	char Minute();
	char Second();
	double Total_Seconds();

	void Day(char day);
	void Hour(char hr);
	void Minute(char min);
	void Second(char sec);
private:
	double _t;
	void _calc_t();
	char _day, _hr, _min, _sec;
	void _convert_decimal_time(double d);
};


/// Trip_Info Class is used to convert from Transims Trip_Data class into Polaris Trip class
class Trip_Info
{
public:
	int hh_id;
	int vehicle_id;
	int Start_Location;
	int End_Location;
	int Start_Zone;
	int End_Zone;
	Time Departure_Time;
};


/// Intialize_Demand_Generator reads the given control file and uses convert_trips.exe to build the trip vector for polaris
/// Use convert_trips = true to run convert trips, with file = convert trips control file.
/// If convert_trips = false, file should point to an existing trip file
void Initialize_Demand_Generator(char* file, bool convert_trips=false, string file_format="TAB_DELIMITED");


/// Get_Trips fills the referenced trip vector with Trip_Info objects that start within the given time range
void Get_Trips(Time start, Time end, vector<Trip_Info*>& new_Trips);


#endif