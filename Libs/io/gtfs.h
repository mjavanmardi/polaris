#ifndef gtfs
#define gtfs
#include "Shared.h"

namespace polaris{ namespace io {
//Forward declarations.
//
class Gtfs_Agency;
class Gtfs_Stop;
class Gtfs_Route_type;
class Gtfs_Route;
class Gtfs_Direction;
class Gtfs_Trip;
class Gtfs_Pickup_dropoff_types;
class Gtfs_Stop_times;
class Gtfs_Calendar;



#pragma db object
class Gtfs_Agency
{
public:
    // Default Constructor
    Gtfs_Agency () {}        
	Gtfs_Agency (std::string agency_id_, std::string agency_name_, std::string agency_url_, std::string agency_timezone_, std::string agency_lang_, std::string agency_phone_, std::string agency_fare_url_)
	: agency_id (agency_id_), agency_name (agency_name_), agency_url (agency_url_), agency_timezone (agency_timezone_), agency_lang (agency_lang_), agency_phone (agency_phone_), agency_fare_url (agency_fare_url_)
	{
	}
	//Accessors
	const std::string& getAgency_Id () const {return agency_id;}
	void setAgency_Id (const std::string& agency_id_) {agency_id = agency_id_;}
	const std::string& getAgency_Name () const {return agency_name;}
	void setAgency_Name (const std::string& agency_name_) {agency_name = agency_name_;}
	const std::string& getAgency_Url () const {return agency_url;}
	void setAgency_Url (const std::string& agency_url_) {agency_url = agency_url_;}
	const std::string& getAgency_Timezone () const {return agency_timezone;}
	void setAgency_Timezone (const std::string& agency_timezone_) {agency_timezone = agency_timezone_;}
	const std::string& getAgency_Lang () const {return agency_lang;}
	void setAgency_Lang (const std::string& agency_lang_) {agency_lang = agency_lang_;}
	const std::string& getAgency_Phone () const {return agency_phone;}
	void setAgency_Phone (const std::string& agency_phone_) {agency_phone = agency_phone_;}
	const std::string& getAgency_Fare_Url () const {return agency_fare_url;}
	void setAgency_Fare_Url (const std::string& agency_fare_url_) {agency_fare_url = agency_fare_url_;}
	//Data Fields
private:
	friend class odb::access;
	#pragma db id
	std::string agency_id;
	#pragma db not_null
	std::string agency_name;
	#pragma db not_null
	std::string agency_url;
	#pragma db not_null
	std::string agency_timezone;
	std::string agency_lang;
	std::string agency_phone;
	std::string agency_fare_url;
};


#pragma db object table("gtfs_stops")
class Gtfs_Stop
{
public:
    // Default Constructor
    Gtfs_Stop () {}        
	Gtfs_Stop (std::string stop_id_, std::string stop_code_, std::string stop_name_, double stop_lat_, double stop_lon_, int zone_id_, std::string stop_url_, int location_type_, shared_ptr<Gtfs_Stop> parent_station_, std::string stop_timezone_, int wheelchair_boarding_)
	: stop_id (stop_id_), stop_code (stop_code_), stop_name (stop_name_), stop_lat (stop_lat_), stop_lon (stop_lon_), zone_id (zone_id_), stop_url (stop_url_), location_type (location_type_), parent_station (parent_station_), stop_timezone (stop_timezone_), wheelchair_boarding (wheelchair_boarding_)
	{
	}
	Gtfs_Stop (std::string stop_id_, std::string stop_code_, std::string stop_name_, double stop_lat_, double stop_lon_, int zone_id_, std::string stop_url_, int location_type_, std::string stop_timezone_, int wheelchair_boarding_)
	: stop_id (stop_id_), stop_code (stop_code_), stop_name (stop_name_), stop_lat (stop_lat_), stop_lon (stop_lon_), zone_id (zone_id_), stop_url (stop_url_), location_type (location_type_), stop_timezone (stop_timezone_), wheelchair_boarding (wheelchair_boarding_)
	{
	}
	//Accessors
	const std::string& getStop_Id () const {return stop_id;}
	void setStop_Id (const std::string& stop_id_) {stop_id = stop_id_;}
	const std::string& getStop_Code () const {return stop_code;}
	void setStop_Code (const std::string& stop_code_) {stop_code = stop_code_;}
	const std::string& getStop_Name () const {return stop_name;}
	void setStop_Name (const std::string& stop_name_) {stop_name = stop_name_;}
	const double& getStop_Lat () const {return stop_lat;}
	void setStop_Lat (const double& stop_lat_) {stop_lat = stop_lat_;}
	const double& getStop_Lon () const {return stop_lon;}
	void setStop_Lon (const double& stop_lon_) {stop_lon = stop_lon_;}
	const int& getZone_Id () const {return zone_id;}
	void setZone_Id (const int& zone_id_) {zone_id = zone_id_;}
	const std::string& getStop_Url () const {return stop_url;}
	void setStop_Url (const std::string& stop_url_) {stop_url = stop_url_;}
	const int& getLocation_Type () const {return location_type;}
	void setLocation_Type (const int& location_type_) {location_type = location_type_;}
	const shared_ptr<Gtfs_Stop> getParent_Station () const {return parent_station;}
	void setParent_Station (const shared_ptr<Gtfs_Stop> parent_station_) {parent_station = parent_station_;}
	const std::string& getStop_Timezone () const {return stop_timezone;}
	void setStop_Timezone (const std::string& stop_timezone_) {stop_timezone = stop_timezone_;}
	const int& getWheelchair_Boarding () const {return wheelchair_boarding;}
	void setWheelchair_Boarding (const int& wheelchair_boarding_) {wheelchair_boarding = wheelchair_boarding_;}
	//Data Fields
private:
	friend class odb::access;
	#pragma db id
	std::string stop_id;
	std::string stop_code;
	#pragma db not_null
	std::string stop_name;
	double stop_lat;
	double stop_lon;
	int zone_id;
	std::string stop_url;
	int location_type;
	shared_ptr<Gtfs_Stop> parent_station;
	std::string stop_timezone;
	int wheelchair_boarding;
};


#pragma db object
class Gtfs_Route_type
{
public:
    // Default Constructor
    Gtfs_Route_type () {}        
	Gtfs_Route_type (int route_type_, std::string description_)
	: route_type (route_type_), description (description_)
	{
	}
	//Accessors
	const int& getRoute_Type () const {return route_type;}
	void setRoute_Type (const int& route_type_) {route_type = route_type_;}
	const std::string& getDescription () const {return description;}
	void setDescription (const std::string& description_) {description = description_;}
	//Data Fields
private:
	friend class odb::access;
	#pragma db id
	int route_type;
	std::string description;
};


#pragma db object table("gtfs_routes")
class Gtfs_Route
{
public:
    // Default Constructor
    Gtfs_Route () {}        
	Gtfs_Route (std::string route_id_, shared_ptr<Gtfs_Agency> agency_id_, std::string route_short_name_, std::string route_long_name_, std::string route_desc_, shared_ptr<Gtfs_Route_type> route_type_, std::string route_url_, std::string route_color_, std::string route_text_color_)
	: route_id (route_id_), agency_id (agency_id_), route_short_name (route_short_name_), route_long_name (route_long_name_), route_desc (route_desc_), route_type (route_type_), route_url (route_url_), route_color (route_color_), route_text_color (route_text_color_)
	{
	}
	Gtfs_Route (std::string route_id_, std::string route_short_name_, std::string route_long_name_, std::string route_desc_, std::string route_url_, std::string route_color_, std::string route_text_color_)
	: route_id (route_id_), route_short_name (route_short_name_), route_long_name (route_long_name_), route_desc (route_desc_), route_url (route_url_), route_color (route_color_), route_text_color (route_text_color_)
	{
	}
	//Accessors
	const std::string& getRoute_Id () const {return route_id;}
	void setRoute_Id (const std::string& route_id_) {route_id = route_id_;}
	const shared_ptr<Gtfs_Agency> getAgency_Id () const {return agency_id;}
	void setAgency_Id (const shared_ptr<Gtfs_Agency> agency_id_) {agency_id = agency_id_;}
	const std::string& getRoute_Short_Name () const {return route_short_name;}
	void setRoute_Short_Name (const std::string& route_short_name_) {route_short_name = route_short_name_;}
	const std::string& getRoute_Long_Name () const {return route_long_name;}
	void setRoute_Long_Name (const std::string& route_long_name_) {route_long_name = route_long_name_;}
	const std::string& getRoute_Desc () const {return route_desc;}
	void setRoute_Desc (const std::string& route_desc_) {route_desc = route_desc_;}
	const shared_ptr<Gtfs_Route_type> getRoute_Type () const {return route_type;}
	void setRoute_Type (const shared_ptr<Gtfs_Route_type> route_type_) {route_type = route_type_;}
	const std::string& getRoute_Url () const {return route_url;}
	void setRoute_Url (const std::string& route_url_) {route_url = route_url_;}
	const std::string& getRoute_Color () const {return route_color;}
	void setRoute_Color (const std::string& route_color_) {route_color = route_color_;}
	const std::string& getRoute_Text_Color () const {return route_text_color;}
	void setRoute_Text_Color (const std::string& route_text_color_) {route_text_color = route_text_color_;}
	//Data Fields
private:
	friend class odb::access;
	#pragma db id
	std::string route_id;
	shared_ptr<Gtfs_Agency> agency_id;
	#pragma db default("")
	std::string route_short_name;
	#pragma db default("")
	std::string route_long_name;
	std::string route_desc;
	shared_ptr<Gtfs_Route_type> route_type;
	std::string route_url;
	std::string route_color;
	std::string route_text_color;
};


#pragma db object
class Gtfs_Direction
{
public:
    // Default Constructor
    Gtfs_Direction () {}        
	Gtfs_Direction (int direction_id_, std::string description_)
	: direction_id (direction_id_), description (description_)
	{
	}
	//Accessors
	const int& getDirection_Id () const {return direction_id;}
	void setDirection_Id (const int& direction_id_) {direction_id = direction_id_;}
	const std::string& getDescription () const {return description;}
	void setDescription (const std::string& description_) {description = description_;}
	//Data Fields
private:
	friend class odb::access;
	#pragma db id
	int direction_id;
	std::string description;
};


#pragma db object table("gtfs_trips")
class Gtfs_Trip
{
public:
    // Default Constructor
    Gtfs_Trip () {}        
	Gtfs_Trip (shared_ptr<Gtfs_Route> route_id_, shared_ptr<Gtfs_Calendar> service_id_, std::string trip_id_, std::string trip_headsign_, shared_ptr<Gtfs_Direction> direction_id_, std::string block_id_, std::string shape_id_, int wheelchair_accessible_)
	: route_id (route_id_), service_id (service_id_), trip_id (trip_id_), trip_headsign (trip_headsign_), direction_id (direction_id_), block_id (block_id_), shape_id (shape_id_), wheelchair_accessible (wheelchair_accessible_)
	{
	}
	Gtfs_Trip (std::string trip_id_, std::string trip_headsign_, std::string block_id_, std::string shape_id_, int wheelchair_accessible_)
	: trip_id (trip_id_), trip_headsign (trip_headsign_), block_id (block_id_), shape_id (shape_id_), wheelchair_accessible (wheelchair_accessible_)
	{
	}
	//Accessors
	const shared_ptr<Gtfs_Route> getRoute_Id () const {return route_id;}
	void setRoute_Id (const shared_ptr<Gtfs_Route> route_id_) {route_id = route_id_;}
	const shared_ptr<Gtfs_Calendar> getService_Id () const {return service_id;}
	void setService_Id (const shared_ptr<Gtfs_Calendar> service_id_) {service_id = service_id_;}
	const std::string& getTrip_Id () const {return trip_id;}
	void setTrip_Id (const std::string& trip_id_) {trip_id = trip_id_;}
	const std::string& getTrip_Headsign () const {return trip_headsign;}
	void setTrip_Headsign (const std::string& trip_headsign_) {trip_headsign = trip_headsign_;}
	const shared_ptr<Gtfs_Direction> getDirection_Id () const {return direction_id;}
	void setDirection_Id (const shared_ptr<Gtfs_Direction> direction_id_) {direction_id = direction_id_;}
	const std::string& getBlock_Id () const {return block_id;}
	void setBlock_Id (const std::string& block_id_) {block_id = block_id_;}
	const std::string& getShape_Id () const {return shape_id;}
	void setShape_Id (const std::string& shape_id_) {shape_id = shape_id_;}
	const int& getWheelchair_Accessible () const {return wheelchair_accessible;}
	void setWheelchair_Accessible (const int& wheelchair_accessible_) {wheelchair_accessible = wheelchair_accessible_;}
	//Data Fields
private:
	friend class odb::access;
	shared_ptr<Gtfs_Route> route_id;
	shared_ptr<Gtfs_Calendar> service_id;
	#pragma db id
	std::string trip_id;
	std::string trip_headsign;
	shared_ptr<Gtfs_Direction> direction_id;
	std::string block_id;
	std::string shape_id;
	int wheelchair_accessible;
};


#pragma db object
class Gtfs_Pickup_dropoff_types
{
public:
    // Default Constructor
    Gtfs_Pickup_dropoff_types () {}        
	Gtfs_Pickup_dropoff_types (int type_id_, std::string description_)
	: type_id (type_id_), description (description_)
	{
	}
	//Accessors
	const int& getType_Id () const {return type_id;}
	void setType_Id (const int& type_id_) {type_id = type_id_;}
	const std::string& getDescription () const {return description;}
	void setDescription (const std::string& description_) {description = description_;}
	//Data Fields
private:
	friend class odb::access;
	#pragma db id
	int type_id;
	std::string description;
};


#pragma db object no_id
class Gtfs_Stop_times
{
public:
    // Default Constructor
    Gtfs_Stop_times () {}        
	Gtfs_Stop_times (shared_ptr<Gtfs_Trip> trip_id_, std::string arrival_time_, std::string departure_time_, shared_ptr<Gtfs_Stop> stop_id_, int stop_sequence_, std::string stop_headsign_, shared_ptr<Gtfs_Pickup_dropoff_types> pickup_type_, shared_ptr<Gtfs_Pickup_dropoff_types> drop_off_type_, double shape_dist_traveled_, int arrival_time_seconds_, int departure_time_seconds_)
	: trip_id (trip_id_), arrival_time (arrival_time_), departure_time (departure_time_), stop_id (stop_id_), stop_sequence (stop_sequence_), stop_headsign (stop_headsign_), pickup_type (pickup_type_), drop_off_type (drop_off_type_), shape_dist_traveled (shape_dist_traveled_), arrival_time_seconds (arrival_time_seconds_), departure_time_seconds (departure_time_seconds_)
	{
	}
	Gtfs_Stop_times (std::string arrival_time_, std::string departure_time_, int stop_sequence_, std::string stop_headsign_, double shape_dist_traveled_, int arrival_time_seconds_, int departure_time_seconds_)
	: arrival_time (arrival_time_), departure_time (departure_time_), stop_sequence (stop_sequence_), stop_headsign (stop_headsign_), shape_dist_traveled (shape_dist_traveled_), arrival_time_seconds (arrival_time_seconds_), departure_time_seconds (departure_time_seconds_)
	{
	}
	//Accessors
	const shared_ptr<Gtfs_Trip> getTrip_Id () const {return trip_id;}
	void setTrip_Id (const shared_ptr<Gtfs_Trip> trip_id_) {trip_id = trip_id_;}
	const std::string& getArrival_Time () const {return arrival_time;}
	void setArrival_Time (const std::string& arrival_time_) {arrival_time = arrival_time_;}
	const std::string& getDeparture_Time () const {return departure_time;}
	void setDeparture_Time (const std::string& departure_time_) {departure_time = departure_time_;}
	const shared_ptr<Gtfs_Stop> getStop_Id () const {return stop_id;}
	void setStop_Id (const shared_ptr<Gtfs_Stop> stop_id_) {stop_id = stop_id_;}
	const int& getStop_Sequence () const {return stop_sequence;}
	void setStop_Sequence (const int& stop_sequence_) {stop_sequence = stop_sequence_;}
	const std::string& getStop_Headsign () const {return stop_headsign;}
	void setStop_Headsign (const std::string& stop_headsign_) {stop_headsign = stop_headsign_;}
	const shared_ptr<Gtfs_Pickup_dropoff_types> getPickup_Type () const {return pickup_type;}
	void setPickup_Type (const shared_ptr<Gtfs_Pickup_dropoff_types> pickup_type_) {pickup_type = pickup_type_;}
	const shared_ptr<Gtfs_Pickup_dropoff_types> getDrop_Off_Type () const {return drop_off_type;}
	void setDrop_Off_Type (const shared_ptr<Gtfs_Pickup_dropoff_types> drop_off_type_) {drop_off_type = drop_off_type_;}
	const double& getShape_Dist_Traveled () const {return shape_dist_traveled;}
	void setShape_Dist_Traveled (const double& shape_dist_traveled_) {shape_dist_traveled = shape_dist_traveled_;}
	const int& getArrival_Time_Seconds () const {return arrival_time_seconds;}
	void setArrival_Time_Seconds (const int& arrival_time_seconds_) {arrival_time_seconds = arrival_time_seconds_;}
	const int& getDeparture_Time_Seconds () const {return departure_time_seconds;}
	void setDeparture_Time_Seconds (const int& departure_time_seconds_) {departure_time_seconds = departure_time_seconds_;}
	//Data Fields
private:
	friend class odb::access;
	shared_ptr<Gtfs_Trip> trip_id;
	std::string arrival_time;
	std::string departure_time;
	shared_ptr<Gtfs_Stop> stop_id;
	int stop_sequence;
	std::string stop_headsign;
	shared_ptr<Gtfs_Pickup_dropoff_types> pickup_type;
	shared_ptr<Gtfs_Pickup_dropoff_types> drop_off_type;
	double shape_dist_traveled;
	int arrival_time_seconds;
	int departure_time_seconds;
};


#pragma db object
class Gtfs_Calendar
{
public:
    // Default Constructor
    Gtfs_Calendar () {}        
	Gtfs_Calendar (std::string service_id_, int monday_, int tuesday_, int wednesday_, int thursday_, int friday_, int saturday_, int sunday_, int start_date_, int end_date_)
	: service_id (service_id_), monday (monday_), tuesday (tuesday_), wednesday (wednesday_), thursday (thursday_), friday (friday_), saturday (saturday_), sunday (sunday_), start_date (start_date_), end_date (end_date_)
	{
	}
	//Accessors
	const std::string& getService_Id () const {return service_id;}
	void setService_Id (const std::string& service_id_) {service_id = service_id_;}
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
	std::string service_id;
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