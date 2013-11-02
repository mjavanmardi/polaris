#ifndef gtfs
#define gtfs
#include "Shared.h"

namespace polaris{ namespace io {
//Forward declarations.
//
class Agency;
class Stop;
class Route_type;
class Route;
class Direction;
class Trip;
class Pickup_dropoff_types;
class Stop_times;
class Calendar;



#pragma db object
class Agency
{
public:
    // Default Constructor
    Agency () {}        
	Agency (string agency_id_, string agency_name_, string agency_url_, string agency_timezone_, string agency_lang_, string agency_phone_, string agency_fare_url_)
	: agency_id (agency_id_), agency_name (agency_name_), agency_url (agency_url_), agency_timezone (agency_timezone_), agency_lang (agency_lang_), agency_phone (agency_phone_), agency_fare_url (agency_fare_url_)
	{
	}
	//Accessors
	const string& getAgency_Id () const {return agency_id;}
	void setAgency_Id (const string& agency_id_) {agency_id = agency_id_;}
	const string& getAgency_Name () const {return agency_name;}
	void setAgency_Name (const string& agency_name_) {agency_name = agency_name_;}
	const string& getAgency_Url () const {return agency_url;}
	void setAgency_Url (const string& agency_url_) {agency_url = agency_url_;}
	const string& getAgency_Timezone () const {return agency_timezone;}
	void setAgency_Timezone (const string& agency_timezone_) {agency_timezone = agency_timezone_;}
	const string& getAgency_Lang () const {return agency_lang;}
	void setAgency_Lang (const string& agency_lang_) {agency_lang = agency_lang_;}
	const string& getAgency_Phone () const {return agency_phone;}
	void setAgency_Phone (const string& agency_phone_) {agency_phone = agency_phone_;}
	const string& getAgency_Fare_Url () const {return agency_fare_url;}
	void setAgency_Fare_Url (const string& agency_fare_url_) {agency_fare_url = agency_fare_url_;}
	//Data Fields
private:
	friend class odb::access;
	#pragma db id
	string agency_id;
	#pragma db not_null
	string agency_name;
	#pragma db not_null
	string agency_url;
	#pragma db not_null
	string agency_timezone;
	string agency_lang;
	string agency_phone;
	string agency_fare_url;
};


#pragma db object
class Stop
{
public:
    // Default Constructor
    Stop () {}        
	Stop (string stop_id_, string stop_code_, string stop_name_, double stop_lat_, double stop_lon_, int zone_id_, string stop_url_, int location_type_, shared_ptr<Stop> parent_station_, string stop_timezone_, int wheelchair_boarding_)
	: stop_id (stop_id_), stop_code (stop_code_), stop_name (stop_name_), stop_lat (stop_lat_), stop_lon (stop_lon_), zone_id (zone_id_), stop_url (stop_url_), location_type (location_type_), parent_station (parent_station_), stop_timezone (stop_timezone_), wheelchair_boarding (wheelchair_boarding_)
	{
	}
	Stop (string stop_id_, string stop_code_, string stop_name_, double stop_lat_, double stop_lon_, int zone_id_, string stop_url_, int location_type_, string stop_timezone_, int wheelchair_boarding_)
	: stop_id (stop_id_), stop_code (stop_code_), stop_name (stop_name_), stop_lat (stop_lat_), stop_lon (stop_lon_), zone_id (zone_id_), stop_url (stop_url_), location_type (location_type_), stop_timezone (stop_timezone_), wheelchair_boarding (wheelchair_boarding_)
	{
	}
	//Accessors
	const string& getStop_Id () const {return stop_id;}
	void setStop_Id (const string& stop_id_) {stop_id = stop_id_;}
	const string& getStop_Code () const {return stop_code;}
	void setStop_Code (const string& stop_code_) {stop_code = stop_code_;}
	const string& getStop_Name () const {return stop_name;}
	void setStop_Name (const string& stop_name_) {stop_name = stop_name_;}
	const double& getStop_Lat () const {return stop_lat;}
	void setStop_Lat (const double& stop_lat_) {stop_lat = stop_lat_;}
	const double& getStop_Lon () const {return stop_lon;}
	void setStop_Lon (const double& stop_lon_) {stop_lon = stop_lon_;}
	const int& getZone_Id () const {return zone_id;}
	void setZone_Id (const int& zone_id_) {zone_id = zone_id_;}
	const string& getStop_Url () const {return stop_url;}
	void setStop_Url (const string& stop_url_) {stop_url = stop_url_;}
	const int& getLocation_Type () const {return location_type;}
	void setLocation_Type (const int& location_type_) {location_type = location_type_;}
	const shared_ptr<Stop> getParent_Station () const {return parent_station;}
	void setParent_Station (const shared_ptr<Stop> parent_station_) {parent_station = parent_station_;}
	const string& getStop_Timezone () const {return stop_timezone;}
	void setStop_Timezone (const string& stop_timezone_) {stop_timezone = stop_timezone_;}
	const int& getWheelchair_Boarding () const {return wheelchair_boarding;}
	void setWheelchair_Boarding (const int& wheelchair_boarding_) {wheelchair_boarding = wheelchair_boarding_;}
	//Data Fields
private:
	friend class odb::access;
	#pragma db id
	string stop_id;
	string stop_code;
	#pragma db not_null
	string stop_name;
	double stop_lat;
	double stop_lon;
	int zone_id;
	string stop_url;
	int location_type;
	shared_ptr<Stop> parent_station;
	string stop_timezone;
	int wheelchair_boarding;
};


#pragma db object
class Route_type
{
public:
    // Default Constructor
    Route_type () {}        
	Route_type (int route_type_, string description_)
	: route_type (route_type_), description (description_)
	{
	}
	//Accessors
	const int& getRoute_Type () const {return route_type;}
	void setRoute_Type (const int& route_type_) {route_type = route_type_;}
	const string& getDescription () const {return description;}
	void setDescription (const string& description_) {description = description_;}
	//Data Fields
private:
	friend class odb::access;
	#pragma db id
	int route_type;
	string description;
};


#pragma db object
class Route
{
public:
    // Default Constructor
    Route () {}        
	Route (string route_id_, shared_ptr<Agency> agency_id_, string route_short_name_, string route_long_name_, string route_desc_, shared_ptr<Route_type> route_type_, string route_url_, string route_color_, string route_text_color_)
	: route_id (route_id_), agency_id (agency_id_), route_short_name (route_short_name_), route_long_name (route_long_name_), route_desc (route_desc_), route_type (route_type_), route_url (route_url_), route_color (route_color_), route_text_color (route_text_color_)
	{
	}
	Route (string route_id_, string route_short_name_, string route_long_name_, string route_desc_, string route_url_, string route_color_, string route_text_color_)
	: route_id (route_id_), route_short_name (route_short_name_), route_long_name (route_long_name_), route_desc (route_desc_), route_url (route_url_), route_color (route_color_), route_text_color (route_text_color_)
	{
	}
	//Accessors
	const string& getRoute_Id () const {return route_id;}
	void setRoute_Id (const string& route_id_) {route_id = route_id_;}
	const shared_ptr<Agency> getAgency_Id () const {return agency_id;}
	void setAgency_Id (const shared_ptr<Agency> agency_id_) {agency_id = agency_id_;}
	const string& getRoute_Short_Name () const {return route_short_name;}
	void setRoute_Short_Name (const string& route_short_name_) {route_short_name = route_short_name_;}
	const string& getRoute_Long_Name () const {return route_long_name;}
	void setRoute_Long_Name (const string& route_long_name_) {route_long_name = route_long_name_;}
	const string& getRoute_Desc () const {return route_desc;}
	void setRoute_Desc (const string& route_desc_) {route_desc = route_desc_;}
	const shared_ptr<Route_type> getRoute_Type () const {return route_type;}
	void setRoute_Type (const shared_ptr<Route_type> route_type_) {route_type = route_type_;}
	const string& getRoute_Url () const {return route_url;}
	void setRoute_Url (const string& route_url_) {route_url = route_url_;}
	const string& getRoute_Color () const {return route_color;}
	void setRoute_Color (const string& route_color_) {route_color = route_color_;}
	const string& getRoute_Text_Color () const {return route_text_color;}
	void setRoute_Text_Color (const string& route_text_color_) {route_text_color = route_text_color_;}
	//Data Fields
private:
	friend class odb::access;
	#pragma db id
	string route_id;
	shared_ptr<Agency> agency_id;
	#pragma db default("")
	string route_short_name;
	#pragma db default("")
	string route_long_name;
	string route_desc;
	shared_ptr<Route_type> route_type;
	string route_url;
	string route_color;
	string route_text_color;
};


#pragma db object
class Direction
{
public:
    // Default Constructor
    Direction () {}        
	Direction (int direction_id_, string description_)
	: direction_id (direction_id_), description (description_)
	{
	}
	//Accessors
	const int& getDirection_Id () const {return direction_id;}
	void setDirection_Id (const int& direction_id_) {direction_id = direction_id_;}
	const string& getDescription () const {return description;}
	void setDescription (const string& description_) {description = description_;}
	//Data Fields
private:
	friend class odb::access;
	#pragma db id
	int direction_id;
	string description;
};


#pragma db object
class Trip
{
public:
    // Default Constructor
    Trip () {}        
	Trip (shared_ptr<Route> route_id_, shared_ptr<Calendar> service_id_, string trip_id_, string trip_headsign_, shared_ptr<Direction> direction_id_, string block_id_, string shape_id_, int wheelchair_accessible_)
	: route_id (route_id_), service_id (service_id_), trip_id (trip_id_), trip_headsign (trip_headsign_), direction_id (direction_id_), block_id (block_id_), shape_id (shape_id_), wheelchair_accessible (wheelchair_accessible_)
	{
	}
	Trip (string trip_id_, string trip_headsign_, string block_id_, string shape_id_, int wheelchair_accessible_)
	: trip_id (trip_id_), trip_headsign (trip_headsign_), block_id (block_id_), shape_id (shape_id_), wheelchair_accessible (wheelchair_accessible_)
	{
	}
	//Accessors
	const shared_ptr<Route> getRoute_Id () const {return route_id;}
	void setRoute_Id (const shared_ptr<Route> route_id_) {route_id = route_id_;}
	const shared_ptr<Calendar> getService_Id () const {return service_id;}
	void setService_Id (const shared_ptr<Calendar> service_id_) {service_id = service_id_;}
	const string& getTrip_Id () const {return trip_id;}
	void setTrip_Id (const string& trip_id_) {trip_id = trip_id_;}
	const string& getTrip_Headsign () const {return trip_headsign;}
	void setTrip_Headsign (const string& trip_headsign_) {trip_headsign = trip_headsign_;}
	const shared_ptr<Direction> getDirection_Id () const {return direction_id;}
	void setDirection_Id (const shared_ptr<Direction> direction_id_) {direction_id = direction_id_;}
	const string& getBlock_Id () const {return block_id;}
	void setBlock_Id (const string& block_id_) {block_id = block_id_;}
	const string& getShape_Id () const {return shape_id;}
	void setShape_Id (const string& shape_id_) {shape_id = shape_id_;}
	const int& getWheelchair_Accessible () const {return wheelchair_accessible;}
	void setWheelchair_Accessible (const int& wheelchair_accessible_) {wheelchair_accessible = wheelchair_accessible_;}
	//Data Fields
private:
	friend class odb::access;
	#pragma db id
	shared_ptr<Route> route_id;
	shared_ptr<Calendar> service_id;
	#pragma db id
	string trip_id;
	string trip_headsign;
	shared_ptr<Direction> direction_id;
	string block_id;
	string shape_id;
	int wheelchair_accessible;
};


#pragma db object
class Pickup_dropoff_types
{
public:
    // Default Constructor
    Pickup_dropoff_types () {}        
	Pickup_dropoff_types (int type_id_, string description_)
	: type_id (type_id_), description (description_)
	{
	}
	//Accessors
	const int& getType_Id () const {return type_id;}
	void setType_Id (const int& type_id_) {type_id = type_id_;}
	const string& getDescription () const {return description;}
	void setDescription (const string& description_) {description = description_;}
	//Data Fields
private:
	friend class odb::access;
	#pragma db id
	int type_id;
	string description;
};


#pragma db object
class Stop_times
{
public:
    // Default Constructor
    Stop_times () {}        
	Stop_times (shared_ptr<Trip> trip_id_, string arrival_time_, string departure_time_, shared_ptr<Stop> stop_id_, int stop_sequence_, string stop_headsign_, shared_ptr<Pickup_dropoff_types> pickup_type_, shared_ptr<Pickup_dropoff_types> drop_off_type_, double shape_dist_traveled_, int arrival_time_seconds_, int departure_time_seconds_)
	: trip_id (trip_id_), arrival_time (arrival_time_), departure_time (departure_time_), stop_id (stop_id_), stop_sequence (stop_sequence_), stop_headsign (stop_headsign_), pickup_type (pickup_type_), drop_off_type (drop_off_type_), shape_dist_traveled (shape_dist_traveled_), arrival_time_seconds (arrival_time_seconds_), departure_time_seconds (departure_time_seconds_)
	{
	}
	Stop_times (string arrival_time_, string departure_time_, int stop_sequence_, string stop_headsign_, double shape_dist_traveled_, int arrival_time_seconds_, int departure_time_seconds_)
	: arrival_time (arrival_time_), departure_time (departure_time_), stop_sequence (stop_sequence_), stop_headsign (stop_headsign_), shape_dist_traveled (shape_dist_traveled_), arrival_time_seconds (arrival_time_seconds_), departure_time_seconds (departure_time_seconds_)
	{
	}
	//Accessors
	const shared_ptr<Trip> getTrip_Id () const {return trip_id;}
	void setTrip_Id (const shared_ptr<Trip> trip_id_) {trip_id = trip_id_;}
	const string& getArrival_Time () const {return arrival_time;}
	void setArrival_Time (const string& arrival_time_) {arrival_time = arrival_time_;}
	const string& getDeparture_Time () const {return departure_time;}
	void setDeparture_Time (const string& departure_time_) {departure_time = departure_time_;}
	const shared_ptr<Stop> getStop_Id () const {return stop_id;}
	void setStop_Id (const shared_ptr<Stop> stop_id_) {stop_id = stop_id_;}
	const int& getStop_Sequence () const {return stop_sequence;}
	void setStop_Sequence (const int& stop_sequence_) {stop_sequence = stop_sequence_;}
	const string& getStop_Headsign () const {return stop_headsign;}
	void setStop_Headsign (const string& stop_headsign_) {stop_headsign = stop_headsign_;}
	const shared_ptr<Pickup_dropoff_types> getPickup_Type () const {return pickup_type;}
	void setPickup_Type (const shared_ptr<Pickup_dropoff_types> pickup_type_) {pickup_type = pickup_type_;}
	const shared_ptr<Pickup_dropoff_types> getDrop_Off_Type () const {return drop_off_type;}
	void setDrop_Off_Type (const shared_ptr<Pickup_dropoff_types> drop_off_type_) {drop_off_type = drop_off_type_;}
	const double& getShape_Dist_Traveled () const {return shape_dist_traveled;}
	void setShape_Dist_Traveled (const double& shape_dist_traveled_) {shape_dist_traveled = shape_dist_traveled_;}
	const int& getArrival_Time_Seconds () const {return arrival_time_seconds;}
	void setArrival_Time_Seconds (const int& arrival_time_seconds_) {arrival_time_seconds = arrival_time_seconds_;}
	const int& getDeparture_Time_Seconds () const {return departure_time_seconds;}
	void setDeparture_Time_Seconds (const int& departure_time_seconds_) {departure_time_seconds = departure_time_seconds_;}
	//Data Fields
private:
	friend class odb::access;
	#pragma db id
	shared_ptr<Trip> trip_id;
	string arrival_time;
	string departure_time;
	shared_ptr<Stop> stop_id;
	int stop_sequence;
	string stop_headsign;
	shared_ptr<Pickup_dropoff_types> pickup_type;
	shared_ptr<Pickup_dropoff_types> drop_off_type;
	double shape_dist_traveled;
	int arrival_time_seconds;
	int departure_time_seconds;
};


#pragma db object
class Calendar
{
public:
    // Default Constructor
    Calendar () {}        
	Calendar (string service_id_, int monday_, int tuesday_, int wednesday_, int thursday_, int friday_, int saturday_, int sunday_, int start_date_, int end_date_)
	: service_id (service_id_), monday (monday_), tuesday (tuesday_), wednesday (wednesday_), thursday (thursday_), friday (friday_), saturday (saturday_), sunday (sunday_), start_date (start_date_), end_date (end_date_)
	{
	}
	//Accessors
	const string& getService_Id () const {return service_id;}
	void setService_Id (const string& service_id_) {service_id = service_id_;}
	const int& getMonday () const {return monday;}
	void setMonday (const int& monday_) {monday = monday_;}
	const int& getTuesday () const {return tuesday;}
	void setTuesday (const int& tuesday_) {tuesday = tuesday_;}
	const int& getWednesday () const {return wednesday;}
	void setWednesday (const int& wednesday_) {wednesday = wednesday_;}
	const int& getThursday () const {return thursday;}
	void setThursday (const int& thursday_) {thursday = thursday_;}
	const int& getFriday () const {return friday;}
	void setFriday (const int& friday_) {friday = friday_;}
	const int& getSaturday () const {return saturday;}
	void setSaturday (const int& saturday_) {saturday = saturday_;}
	const int& getSunday () const {return sunday;}
	void setSunday (const int& sunday_) {sunday = sunday_;}
	const int& getStart_Date () const {return start_date;}
	void setStart_Date (const int& start_date_) {start_date = start_date_;}
	const int& getEnd_Date () const {return end_date;}
	void setEnd_Date (const int& end_date_) {end_date = end_date_;}
	//Data Fields
private:
	friend class odb::access;
	#pragma db id
	string service_id;
	int monday;
	int tuesday;
	int wednesday;
	int thursday;
	int friday;
	int saturday;
	int sunday;
	int start_date;
	int end_date;
};
}}
#endif