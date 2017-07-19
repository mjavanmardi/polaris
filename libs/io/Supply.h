#ifndef IOSupply
#define IOSupply

#include "Shared.h"



namespace polaris
{
namespace io
{
//Forward declarations.
//
//class MetaData;
//class Tags;
class Node;
class Zone;
//class ZoneLandUse;
//class Shape;
class Link;
class Transit_Links;
class Transit_Patterns;
class Transit_Routes;
class Transit_Stops;
class Transit_Trips;
class Transit_Walk;
class Pocket;
class Lane_Use;
class Connect;
//class Turn_Pen;
class Parking;
class Location;
//class Access;
class Sign;
class Signal;
class Timing;
class Phasing;
//class Detector;
//class Stop;
//class Fare;
//class Line;
//class Schedule;
//class Driver;
//class Route_Nodes;
//class Ridership;
//class Veh_Type;
class Area_Type;
class Link_Type;

//class LinkList;
//class Component;
class VMS;
//class Fixed_Sensor;
class HAR;
class VSS;
//class Depot;
//class OpenShoulder;
//class Action;
//class Action_Key;
//class Microsoft_Event;
//class InputContainer;


typedef shared_ptr<Node> node_ptr;
typedef shared_ptr<Transit_Stops> transit_node_ptr;
typedef shared_ptr<Transit_Walk> walk_link_ptr;
typedef shared_ptr<Link_Type> link_type_ptr;
typedef shared_ptr<Area_Type> area_type_ptr;
typedef shared_ptr<Transit_Routes> route_ptr;
typedef shared_ptr<Transit_Patterns> pattern_ptr;
#pragma db value(node_ptr) type("INTEGER") not_null
#pragma db value(transit_node_ptr) type("TEXT") not_null
#pragma db value(walk_link_ptr) type("INTEGER") not_null
#pragma db value(link_type_ptr) type("INTEGER") not_null
#pragma db value(area_type_ptr) type("INTEGER") not_null
#pragma db value(route_ptr) type("INTEGER") not_null
#pragma db value(pattern_ptr) type("INTEGER") not_null

#pragma db value
class timing_phase
{
public:
	int phase;
	int barrier;
	int ring;
	int position;
	int minimum;
	int maximum;
	int extend;
	int yellow;
	int red;
};
#pragma db value
class shape_geometry
{
public:
	double x;
	double y;
	double z;
};
#pragma db value
class signal_time
{
public:
	double start;
	double end;
	int timing;
	int phasing;
};
#pragma db value
class phase_movement
{
public:
	std::string movement;
	shared_ptr<Link> link;
	int dir;
	shared_ptr<Link> to_link;
	std::string protect;
};
//Input Container.
//
class InputContainer 
{
public:
	std::map<int, node_ptr > Nodes;
	std::map<std::string, transit_node_ptr > TransitStops;
	std::map<int, walk_link_ptr > Transit_Walk;
	std::map<std::string, route_ptr > TransitRoutes;
	std::map<std::string, pattern_ptr > TransitPatterns;
	std::map<int,shared_ptr<Zone> > Zones;
	std::map<int,shared_ptr<Link> > Links;
	std::map<int,shared_ptr<Parking> > Parkings;
	std::map<int,shared_ptr<Location> > Locations;
	std::map<int,shared_ptr<Signal> > Signals;
	//std::map<int,shared_ptr<Stop> > Stops;
	//std::map<int,shared_ptr<Veh_Type> > Veh_Types;
	std::map<int,area_type_ptr > Area_Types;
	std::map<std::string,shared_ptr<Link_Type> > Link_Types;
};

/*
#pragma db object
class MetaData
{
public:
	MetaData() {}
	MetaData(std::string key_, std::string value_)
		: key(key_), value(value_)
	{
	}
public:
	#pragma db id
	std::string key;
	std::string value;
};
#pragma db object
class Tags
{
public:
	Tags() {};
	#pragma db id auto
	int id;
	std::string table;
	std::string record_id;
	std::string key;
	std::string value;
};
*/

#pragma db object //table("NODE")
class Node
{
public:
	// Default Constructor
	Node () {}	
	//Constructor
	Node ( int node_, double x_, double y_, double z_, int subarea_, int part_, shared_ptr<Zone> zone_ )
	: node (node_), x (x_), y (y_), z (z_), subarea (subarea_), part (part_), zone(zone_)
	{
	}
	//Accessors
	const int& getNode () const {return node;}
	void setNode (const int& node_){node = node_;}
	const double& getX () const {return x;}
	void setX (const double& x_){x = x_;}
	const double& getY () const {return y;}
	void setY (const double& y_){y = y_;}
	const double& getZ () const {return z;}
	void setZ (const double& z_){z = z_;}
	const int& getSubarea () const {return subarea;}
	void setSubarea (const int& subarea_){subarea = subarea_;}
	const int& getPart () const {return part;}
	void setPart (const int& part_){part = part_;}
	const int& getPrimaryKey () const {return node;}
	const shared_ptr<Zone>& getZone() const { return zone; }
	void setNode_A(const shared_ptr<Zone>& zone_) { zone = zone_; }
	void setNode_A(const int& zone_, InputContainer& container) { zone = container.Zones[zone_]; }
	
//Data Fields
private:
	friend class odb::access;
#pragma db id
	int node;
#pragma db null
	double x;
	double y;
	double z;
	int subarea;
	int part;
	shared_ptr<Zone> zone;
#pragma db index member(node)

};

#pragma db object //table("Transit_Node")
class Transit_Stops
{
public:
	// Default Constructor
	Transit_Stops() {}
	//Constructor
	Transit_Stops(std::string stop_, int link_, int dir_, double offset_, double setback_, double x_, double y_, double z_, std::string agency_, std::string name_, std::string description_, std::string street_, shared_ptr<Zone> zone_)
		: stop(stop_), link(link_), dir(dir_), offset(offset_), setback(setback_), x(x_), y(y_), z(z_), agency(agency_), name(name_), description(description_), street(street_), zone(zone_)
	{
	}
	//Accessors
	const std::string& getStop() const { return stop; }
	void setStop(const std::string& stop_) { stop = stop_; }	
	const int& getLink() const { return link; }
	void setLink(const int& link_) { link = link_; }
	const int& getDir() const { return dir; }
	void setDir(const int& dir_) { dir = dir_; }
	const double& getOffset() const { return offset; }
	void setOffset(const double& offset_) { offset = offset_; }
	const double& getSetback() const { return setback; }
	void setSetback(const double& setback_) { setback = setback_; }
	const double& getX() const { return x; }
	void setX(const double& x_) { x = x_; }
	const double& getY() const { return y; }
	void setY(const double& y_) { y = y_; }
	const double& getZ() const { return z; }
	void setZ(const double& z_) { z = z_; }
	const std::string& getAgency() const { return agency; }
	void setAgency(const std::string& agency_) { agency = agency_; }
	const std::string& getName() const { return name; }
	void setName(const std::string& name_) { street = name_; }
	const std::string& getDescription() const { return description; }
	void setDescription(const std::string& description_) { description = description_; }
	const std::string& getStreet() const { return street; }
	void setStreet(const std::string& street_) { street = street_; }
	const shared_ptr<Zone>& getZone() const { return zone; }
	void setNode_A(const shared_ptr<Zone>& zone_) { zone = zone_; }
	void setNode_A(const int& zone_, InputContainer& container) { zone = container.Zones[zone_]; }

	const std::string& getPrimaryKey() const { return stop; }

	//Data Fields
private:
	friend class odb::access;

//#pragma db null
#pragma db id
	std::string stop;
	int link;
	int dir;
	double offset;
	double setback;
	double x;
	double y;
	double z;
	std::string agency;
	std::string name;
	std::string description;
	std::string street;
	shared_ptr<Zone> zone;
//#pragma db index member(stop)
};

#pragma db object no_id //table("Transit_Links")
class Transit_Links
{
public:
	// Default Constructor
	Transit_Links() {}
	//Constructor
	Transit_Links(transit_node_ptr from_node_, transit_node_ptr to_node_, double length_, int type_, std::string triplist_, std::string indexlist_)
		: from_node(from_node_), to_node(to_node_), length(length_), type(type_), triplist(triplist_), indexlist(indexlist_)
	{
	}
	//Accessors	
	const transit_node_ptr& getNode_A() const { return from_node; }
	void setNode_A(const transit_node_ptr& from_node_) { from_node = from_node_; }
	void setNode_A(const std::string& from_node_, InputContainer& container) { from_node = container.TransitStops[from_node_]; }
	const transit_node_ptr& getNode_B() const { return to_node; }
	void setNode_B(const transit_node_ptr& to_node_) { to_node = to_node_; }
	void setNode_B(const std::string& to_node_, InputContainer& container) { to_node = container.TransitStops[to_node_]; }	
	const double& getLength() const { return length; }
	void setLength(const double& length_) { length = length_; }
	const int& getType() const { return type; }
	void setType(const int& type_) { type = type_; }
	const std::string& getTriplist() const { return triplist; }
	void setTriplist(const std::string& triplist_) { triplist = triplist_; }
	const std::string& getIndexlist() const { return indexlist; }
	void setIndexlist(const std::string& indexlist_) { indexlist = indexlist_; }
	//Data Fields
private:
	friend class odb::access;

//#pragma db null
	transit_node_ptr from_node;
	transit_node_ptr to_node;
	double length;
	int type;
	std::string triplist;
	std::string indexlist;
};

#pragma db object no_id //table("Transit_Walk")
class Transit_Walk
{
public:
	// Default Constructor
	Transit_Walk() {}
	//Constructor
	Transit_Walk(std::string from_node_, std::string to_node_, double length_, int walk_link_)
		: from_node(from_node_), to_node(to_node_), length(length_), walk_link(walk_link_)
	{
	}
	//Accessors	
	const std::string& getNode_A() const { return from_node; }
	void setNode_A(const std::string& from_node_) { from_node = from_node_; }
	const std::string& getNode_B() const { return to_node; }
	void setNode_B(const std::string& to_node_) { to_node = to_node_; }

	/*const node_ptr& getNode_A() const { return node_a; }
	void setNode_A(const node_ptr& node_a_) { node_a = node_a_; }
	void setNode_A(const int& node_a_, InputContainer& container) { node_a = container.Nodes[node_a_]; }
	const node_ptr& getNode_B() const { return node_b; }
	void setNode_B(const node_ptr& node_b_) { node_b = node_b_; }
	void setNode_B(const int& node_b_, InputContainer& container) { node_b = container.Nodes[node_b_]; }*/
	const double& getLength() const { return length; }
	void setLength(const double& length_) { length = length_; }	
	const int& getLink() const { return walk_link; }
	void setLink(const int& walk_link_) { walk_link = walk_link_; }

	const int& getPrimaryKey() const { return walk_link; }
	//Data Fields
private:
	friend class odb::access;
#pragma db id
	int walk_link;
#pragma db null	
	std::string from_node;
	std::string to_node;
	double length;
	
};

#pragma db object //table("Transit_Routes")
class Transit_Routes
{
public:
	// Default Constructor
	Transit_Routes() {}
	//Constructor
	Transit_Routes(std::string route_, std::string agency_, std::string shortname_, std::string longname_, std::string description_, int type_)
		: route(route_), agency(agency_), shortname(shortname_), longname(longname_), description(description_), type(type_)
	{
	}
	//Accessors
	const std::string& getRoute() const { return route; }
	void setRoute(const std::string& route_) { route = route_; }
	const std::string& getAgency() const { return agency; }
	void setAgency(const std::string& agency_) { agency = agency_; }
	const std::string& getShortname() const { return shortname; }
	void setShortname(const std::string& shortname_) { shortname = shortname_; }
	const std::string& getLongname() const { return longname; }
	void setLongname(const std::string& longname_) { longname = longname_; }
	const std::string& getDescription() const { return description; }
	void setDescription(const std::string& description_) { description = description_; }
	const int& getType() const { return type; }
	void setType(const int& type_) { type = type_; }

	const std::string& getPrimaryKey() const { return route; }

	//Data Fields
private:
	friend class odb::access;

#pragma db id
	std::string route;
	std::string agency;
	std::string shortname;
	std::string longname;
	std::string description;
	int type;
};

#pragma db object //table("Transit_Patterns")
class Transit_Patterns
{
public:
	// Default Constructor
	Transit_Patterns() {}
	//Constructor
	Transit_Patterns(std::string pattern_, route_ptr route_, std::string stoplist_)
		: pattern(pattern_), route(route_), stoplist(stoplist_)
	{
	}
	//Accessors
	const std::string& getPattern() const { return pattern; }
	void setPattern(const std::string& pattern_) { pattern = pattern_; }
	const route_ptr& getRoute() const { return route; }
	void setRoute(const route_ptr& route_) { route = route_; }
	void setRoute(const std::string& route_, InputContainer& container) { route = container.TransitRoutes[route_]; }
	const std::string& getStoplist() const { return stoplist; }
	void setStoplist(const std::string& stoplist_) { stoplist = stoplist_; }
	const std::string& getPrimaryKey() const { return pattern; }

	//Data Fields
private:
	friend class odb::access;

#pragma db id
	std::string pattern;
	route_ptr route;
	std::string stoplist;
};

#pragma db object no_id //table("Transit_Trips")
class Transit_Trips
{
public:
	// Default Constructor
	Transit_Trips() {}
	//Constructor
	Transit_Trips(std::string trip_, int dir_, std::string agency_, pattern_ptr pattern_, route_ptr route_, std::string arrivals_, std::string departures_)
		: trip(trip_), dir(dir_), agency(agency_), pattern(pattern_), route(route_), arrivals(arrivals_), departures(departures_)
	{
	}
	//Accessors
	const std::string& getTrip() const { return trip; }
	void setTrip(const std::string& trip_) { trip = trip_; }
	const int& getDir() const { return dir; }
	void setDir(const int& dir_) { dir = dir_; }
	const std::string& getAgency() const { return agency; }
	void setAgency(const std::string& agency_) { agency = agency_; }
	const pattern_ptr& getPattern() const { return pattern; }
	void setPattern(const pattern_ptr& pattern_) { pattern = pattern_; }
	void setPattern(const std::string& pattern_, InputContainer& container) { pattern = container.TransitPatterns[pattern_]; }
	const route_ptr& getRoute() const { return route; }
	void setRoute(const route_ptr& route_) { route = route_; }
	void setRoute(const std::string& route_, InputContainer& container) { route = container.TransitRoutes[route_]; }
	const std::string& getArrivals() const { return arrivals; }
	void setArrivals(const std::string& arrivals_) { arrivals = arrivals_; }
	const std::string& getDepartures() const { return departures; }
	void setDepartures(const std::string& departures_) { departures = departures_; }
	const std::string& getPrimaryKey() const { return trip; }

	//Data Fields
private:
	friend class odb::access;

//#pragma db null
	std::string trip;
	int	dir;
	std::string agency;
	pattern_ptr pattern;
	route_ptr route;
	std::string arrivals;
	std::string departures;
};


#pragma db object
class Zone
{
public:
	// Default Constructor
	Zone () {}        
	Zone (int zone_, /*shared_ptr<ZoneLandUse> zone_land_use_,*/ double x_, double y_, double z_, shared_ptr<Area_Type> area_type_, double min_x_, double min_y_, double max_x_, double max_y_, float area_,  float entertainment_area_,  float industrial_area_,  float institutional_area_,  float mixed_use_area_,  float office_area_,  float other_area_,  float residential_area_,  float retail_area_,  float school_area_,  int pop_households_,  int pop_persons_,  int pop_group_quarters_,  int employment_total_,  int employment_retail_,  int employment_government_,  int employment_manufacturing_,  int employment_services_,  int employment_industrial_,  int employment_other_)
	: zone (zone_), /*zone_land_use (zone_land_use_),*/ x (x_), y (y_), z (z_), area_type (area_type_), min_x (min_x_), min_y (min_y_), max_x (max_x_), max_y (max_y_), area(area_), entertainment_area(entertainment_area_), industrial_area(industrial_area_), institutional_area(institutional_area_), mixed_use_area(mixed_use_area_), office_area(office_area_), other_area(other_area_), residential_area(residential_area_), retail_area(retail_area_), school_area(school_area_), pop_households(pop_households_), pop_persons(pop_persons_), pop_group_quarters(pop_group_quarters_), employment_total(employment_total_), employment_retail(employment_retail_), employment_government(employment_government_), employment_manufacturing(employment_manufacturing_), employment_services(employment_services_), employment_industrial(employment_industrial_), employment_other(employment_other_)
	{
	}
	//Accessors
	const int& getPrimaryKey () const {return zone;}
	const int& getZone () const {return zone;}
	void setZone (const int& zone_) {zone = zone_;}
	//const shared_ptr<ZoneLandUse>& getZone_Land_Use () const {return zone_land_use;}
	//void setZone_Land_Use (const shared_ptr<ZoneLandUse>& zone_land_use_) {zone_land_use = zone_land_use_;}
	const double& getX () const {return x;}
	void setX (const double& x_) {x = x_;}
	const double& getY () const {return y;}
	void setY (const double& y_) {y = y_;}
	const double& getZ () const {return z;}
	void setZ (const double& z_) {z = z_;}
	void setArea_type (const int& area_type_, InputContainer& container) {area_type = container.Area_Types[area_type_];}
	const shared_ptr<Area_Type>& getArea_type () const {return area_type;}
	void setArea_type (const shared_ptr<Area_Type>& area_type_) {area_type = area_type_;}
	const double& getMin_X () const {return min_x;}
	void setMin_X (const double& min_x_) {min_x = min_x_;}
	const double& getMin_Y () const {return min_y;}
	void setMin_Y (const double& min_y_) {min_y = min_y_;}
	const double& getMax_X () const {return max_x;}
	void setMax_X (const double& max_x_) {max_x = max_x_;}
	const double& getMax_Y () const {return max_y;}
	void setMax_Y (const double& max_y_) {max_y = max_y_;}
	const float& getPercent_white() const{return percent_white;}
	void setPercent_white(const float& percent_white_){percent_white= percent_white_;}
	const float& getPercent_black() const{return percent_black;}
	void setPercent_black(const float& percent_black_){percent_black= percent_black_;}
	const float& getHH_inc_avg() const{return hh_inc_avg;}
	void setHH_inc_avg(const float& hh_inc_avg_){hh_inc_avg= hh_inc_avg_;}

	const float& getArea() const{return area;}
	void setArea(const float& area_){area= area_;}
	const float& getEntertainment_area() const{return entertainment_area;}
	void setEntertainment_area(const float& entertainment_area_){entertainment_area= entertainment_area_;}
	const float& getIndustrial_area() const{return industrial_area;}
	void setIndustrial_area(const float& industrial_area_){industrial_area= industrial_area_;}
	const float& getInstitutional_area() const{return institutional_area;}
	void setInstitutional_area(const float& institutional_area_){institutional_area= institutional_area_;}
	const float& getMixed_use_area() const{return mixed_use_area;}
	void setMixed_use_area(const float& mixed_use_area_){mixed_use_area= mixed_use_area_;}
	const float& getOffice_area() const{return office_area;}
	void setOffice_area(const float& office_area_){office_area= office_area_;}
	const float& getOther_area() const{return other_area;}
	void setOther_area(const float& other_area_){other_area= other_area_;}
	const float& getResidential_area() const{return residential_area;}
	void setResidential_area(const float& residential_area_){residential_area= residential_area_;}
	const float& getRetail_area() const{return retail_area;}
	void setRetail_area(const float& retail_area_){retail_area= retail_area_;}
	const float& getSchool_area() const{return school_area;}
	void setSchool_area(const float& school_area_){school_area= school_area_;}
	const int& getPop_households() const{return pop_households;}
	void setPop_households(const int& pop_households_){pop_households= pop_households_;}
	const int& getPop_persons() const{return pop_persons;}
	void setPop_persons(const int& pop_persons_){pop_persons= pop_persons_;}
	const int& getPop_group_quarters() const{return pop_group_quarters;}
	void setPop_group_quarters(const int& pop_group_quarters_){pop_group_quarters= pop_group_quarters_;}
	const int& getEmployment_total() const{return employment_total;}
	void setEmployment_total(const int& employment_total_){employment_total= employment_total_;}
	const int& getEmployment_retail() const{return employment_retail;}
	void setEmployment_retail(const int& employment_retail_){employment_retail= employment_retail_;}
	const int& getEmployment_government() const{return employment_government;}
	void setEmployment_government(const int& employment_government_){employment_government= employment_government_;}
	const int& getEmployment_manufacturing() const{return employment_manufacturing;}
	void setEmployment_manufacturing(const int& employment_manufacturing_){employment_manufacturing= employment_manufacturing_;}
	const int& getEmployment_services() const{return employment_services;}
	void setEmployment_services(const int& employment_services_){employment_services= employment_services_;}
	const int& getEmployment_industrial() const{return employment_industrial;}
	void setEmployment_industrial(const int& employment_industrial_){employment_industrial= employment_industrial_;}
	const int& getEmployment_other() const{return employment_other;}
	void setEmployment_other(const int& employment_other_){employment_other= employment_other_;}
//Data Fields
private:
	friend class odb::access;
	#pragma db id
	int zone;
	//shared_ptr<ZoneLandUse> zone_land_use;
	double x;
	double y;
	double z;
	shared_ptr<Area_Type> area_type;
	double min_x;
	double min_y;
	double max_x;
	double max_y;
	float percent_white;
	float percent_black;
	float hh_inc_avg;
	float area;
	float entertainment_area;
	float industrial_area;
	float institutional_area;
	float mixed_use_area;
	float office_area;
	float other_area;
	float residential_area;
	float retail_area;
	float school_area;
	int pop_households;
	int pop_persons;
	int pop_group_quarters;
	int employment_total;
	int employment_retail;
	int employment_government;
	int employment_manufacturing;
	int employment_services;
	int employment_industrial;
	int employment_other;
	#pragma db index member(zone)
};

//#pragma db object
//class ZoneLandUse
//{
//public:
//	// Default Constructor
//	ZoneLandUse () {}        
//	ZoneLandUse (int zone_,  float area_,  float entertainment_area_,  float industrial_area_,  float institutional_area_,  float mixed_use_area_,  float office_area_,  float other_area_,  float residential_area_,  float retail_area_,  float school_area_,  int pop_households_,  int pop_persons_,  int pop_group_quarters_,  int employment_total_,  int employment_retail_,  int employment_government_,  int employment_manufacturing_,  int employment_services_,  int employment_industrial_,  int employment_other_)
//	: zone(zone_), area(area_), entertainment_area(entertainment_area_), industrial_area(industrial_area_), institutional_area(institutional_area_), mixed_use_area(mixed_use_area_), office_area(office_area_), other_area(other_area_), residential_area(residential_area_), retail_area(retail_area_), school_area(school_area_), pop_households(pop_households_), pop_persons(pop_persons_), pop_group_quarters(pop_group_quarters_), employment_total(employment_total_), employment_retail(employment_retail_), employment_government(employment_government_), employment_manufacturing(employment_manufacturing_), employment_services(employment_services_), employment_industrial(employment_industrial_), employment_other(employment_other_)
//	{
//	}
//	//Accessors
//
//	const int& getPrimaryKey () const {return zone;}
//	const int& getZone() const{return zone;}
//	void setZone(const int& zone_){zone= zone_;}
//
//	const float& getPercent_white() const{return percent_white;}
//	void setPercent_white(const float& percent_white_){percent_white= percent_white_;}
//	const float& getPercent_black() const{return percent_black;}
//	void setPercent_black(const float& percent_black_){percent_black= percent_black_;}
//	const float& getHH_inc_avg() const{return hh_inc_avg;}
//	void setHH_inc_avg(const float& hh_inc_avg_){hh_inc_avg= hh_inc_avg_;}
//
//	const float& getArea() const{return area;}
//	void setArea(const float& area_){area= area_;}
//	const float& getEntertainment_area() const{return entertainment_area;}
//	void setEntertainment_area(const float& entertainment_area_){entertainment_area= entertainment_area_;}
//	const float& getIndustrial_area() const{return industrial_area;}
//	void setIndustrial_area(const float& industrial_area_){industrial_area= industrial_area_;}
//	const float& getInstitutional_area() const{return institutional_area;}
//	void setInstitutional_area(const float& institutional_area_){institutional_area= institutional_area_;}
//	const float& getMixed_use_area() const{return mixed_use_area;}
//	void setMixed_use_area(const float& mixed_use_area_){mixed_use_area= mixed_use_area_;}
//	const float& getOffice_area() const{return office_area;}
//	void setOffice_area(const float& office_area_){office_area= office_area_;}
//	const float& getOther_area() const{return other_area;}
//	void setOther_area(const float& other_area_){other_area= other_area_;}
//	const float& getResidential_area() const{return residential_area;}
//	void setResidential_area(const float& residential_area_){residential_area= residential_area_;}
//	const float& getRetail_area() const{return retail_area;}
//	void setRetail_area(const float& retail_area_){retail_area= retail_area_;}
//	const float& getSchool_area() const{return school_area;}
//	void setSchool_area(const float& school_area_){school_area= school_area_;}
//	const int& getPop_households() const{return pop_households;}
//	void setPop_households(const int& pop_households_){pop_households= pop_households_;}
//	const int& getPop_persons() const{return pop_persons;}
//	void setPop_persons(const int& pop_persons_){pop_persons= pop_persons_;}
//	const int& getPop_group_quarters() const{return pop_group_quarters;}
//	void setPop_group_quarters(const int& pop_group_quarters_){pop_group_quarters= pop_group_quarters_;}
//	const int& getEmployment_total() const{return employment_total;}
//	void setEmployment_total(const int& employment_total_){employment_total= employment_total_;}
//	const int& getEmployment_retail() const{return employment_retail;}
//	void setEmployment_retail(const int& employment_retail_){employment_retail= employment_retail_;}
//	const int& getEmployment_government() const{return employment_government;}
//	void setEmployment_government(const int& employment_government_){employment_government= employment_government_;}
//	const int& getEmployment_manufacturing() const{return employment_manufacturing;}
//	void setEmployment_manufacturing(const int& employment_manufacturing_){employment_manufacturing= employment_manufacturing_;}
//	const int& getEmployment_services() const{return employment_services;}
//	void setEmployment_services(const int& employment_services_){employment_services= employment_services_;}
//	const int& getEmployment_industrial() const{return employment_industrial;}
//	void setEmployment_industrial(const int& employment_industrial_){employment_industrial= employment_industrial_;}
//	const int& getEmployment_other() const{return employment_other;}
//	void setEmployment_other(const int& employment_other_){employment_other= employment_other_;}
//
//	
////Data Fields	
//private:	
//	friend class odb::access;
//	#pragma db id
//	int zone;
//	float percent_white;
//	float percent_black;
//	float hh_inc_avg;
//	float area;
//	float entertainment_area;
//	float industrial_area;
//	float institutional_area;
//	float mixed_use_area;
//	float office_area;
//	float other_area;
//	float residential_area;
//	float retail_area;
//	float school_area;
//	int pop_households;
//	int pop_persons;
//	int pop_group_quarters;
//	int employment_total;
//	int employment_retail;
//	int employment_government;
//	int employment_manufacturing;
//	int employment_services;
//	int employment_industrial;
//	int employment_other;
//};

/*
#pragma db object //table("SHAPE")
class Shape
{
public:
	// Default Constructor
	Shape () {}	
	//Constructor
	Shape ( shared_ptr<Link> link_, int points_ )  
	: link (link_), points (points_)
	{
	}
	//Accessors
	const shared_ptr<Link>& getLink () const {return link;}
	void setLink (const shared_ptr<Link>& link_){link = link_;}
	void setLink (const int& link_, InputContainer& container){link = container.Links[link_];}
	const int& getPoints () const {return points;}
	void setPoints (const int& points_){points = points_;}
	const unsigned long& getPrimaryKey () const {return zone;}
	const unsigned long& getZone () const {return zone;}


	//Vector that contains the associated nested records
	 std::vector<shape_geometry> nested_records;

//Data Fields
private:
	friend class odb::access;
	#pragma db id auto
	unsigned long zone;
	shared_ptr<Link> link;
	int points;
	#pragma db index member(link)

};
*/

#pragma db object //table("LINK")
class Link
{
public:
	// Default Constructor
	Link () {}	
	//Constructor
	Link ( int link_, std::string name_, node_ptr node_a_, node_ptr node_b_, double length_, double setback_a_, double setback_b_, int bearing_a_, int bearing_b_, std::string type_, int divided_, shared_ptr<Area_Type> area_type_, std::string use_, double grade_, int lanes_ab_, double speed_ab_, double fspd_ab_, int cap_ab_, int lanes_ba_, double speed_ba_, double fspd_ba_, int cap_ba_, int left_ab_, int right_ab_, int left_ba_, int right_ba_ )  
	: link (link_), name (name_), node_a (node_a_), node_b (node_b_), length (length_), setback_a (setback_a_), setback_b (setback_b_), bearing_a (bearing_a_), bearing_b (bearing_b_), type (type_), divided (divided_), area_type (area_type_), use (use_), grade (grade_), lanes_ab (lanes_ab_), speed_ab (speed_ab_), fspd_ab (fspd_ab_), cap_ab (cap_ab_), lanes_ba (lanes_ba_), speed_ba (speed_ba_), fspd_ba (fspd_ba_), cap_ba (cap_ba_), left_ab (left_ab_), right_ab (right_ab_), left_ba (left_ba_), right_ba (right_ba_)
	{
	}
	//Accessors
	const int& getLink () const {return link;}
	void setLink (const int& link_){link = link_;}
	const std::string& getName () const {return name;}
	void setName (const std::string& name_){name = name_;}
	const node_ptr& getNode_A () const {return node_a;}
	void setNode_A (const node_ptr& node_a_){node_a = node_a_;}
	void setNode_A (const int& node_a_, InputContainer& container){node_a = container.Nodes[node_a_];}
	const node_ptr& getNode_B () const {return node_b;}
	void setNode_B (const node_ptr& node_b_){node_b = node_b_;}
	void setNode_B (const int& node_b_, InputContainer& container){node_b = container.Nodes[node_b_];}
	const double& getLength () const {return length;}
	void setLength (const double& length_){length = length_;}
	const double& getSetback_A () const {return setback_a;}
	void setSetback_A (const double& setback_a_){setback_a = setback_a_;}
	const double& getSetback_B () const {return setback_b;}
	void setSetback_B (const double& setback_b_){setback_b = setback_b_;}
	const int& getBearing_A () const {return bearing_a;}
	void setBearing_A (const int& bearing_a_){bearing_a = bearing_a_;}
	const int& getBearing_B () const {return bearing_b;}
	void setBearing_B (const int& bearing_b_){bearing_b = bearing_b_;}
	//const shared_ptr<Link_Type>& getType () const {return type;}
	//void setType (const shared_ptr<Link_Type>& type_){type = type_;}
	//void setType (const std::string& type_, InputContainer& container){type = container.Link_Types[type_];}
	const std::string& getType() const { return type; }
	void setType(const std::string& type_) { type = type_; }
	const int& getDivided () const {return divided;}
	void setDivided (const int& divided_){divided = divided_;}
	const shared_ptr<Area_Type>& getArea_Type () const {return area_type;}
	void setArea_Type (const shared_ptr<Area_Type>& area_type_){area_type = area_type_;}
	void setArea_Type (const int& area_type_,  InputContainer& container){area_type = container.Area_Types[area_type_];}
	const std::string& getUse () const {return use;}
	void setUse (const std::string& use_){use = use_;}
	const double& getGrade () const {return grade;}
	void setGrade (const double& grade_){grade = grade_;}
	const int& getLanes_Ab () const {return lanes_ab;}
	void setLanes_Ab (const int& lanes_ab_){lanes_ab = lanes_ab_;}
	const double& getSpeed_Ab () const {return speed_ab;}
	void setSpeed_Ab (const double& speed_ab_){speed_ab = speed_ab_;}
	const double& getFspd_Ab () const {return fspd_ab;}
	void setFspd_Ab (const double& fspd_ab_){fspd_ab = fspd_ab_;}
	const int& getCap_Ab () const {return cap_ab;}
	void setCap_Ab (const int& cap_ab_){cap_ab = cap_ab_;}
	const int& getLanes_Ba () const {return lanes_ba;}
	void setLanes_Ba (const int& lanes_ba_){lanes_ba = lanes_ba_;}
	const double& getSpeed_Ba () const {return speed_ba;}
	void setSpeed_Ba (const double& speed_ba_){speed_ba = speed_ba_;}
	const double& getFspd_Ba () const {return fspd_ba;}
	void setFspd_Ba (const double& fspd_ba_){fspd_ba = fspd_ba_;}
	const int& getCap_Ba () const {return cap_ba;}
	void setCap_Ba (const int& cap_ba_){cap_ba = cap_ba_;}
	const int& getLeft_Ab () const {return left_ab;}
	void setLeft_Ab (const int& left_ab_){left_ab = left_ab_;}
	const int& getRight_Ab () const {return right_ab;}
	void setRight_Ab (const int& right_ab_){right_ab = right_ab_;}
	const int& getLeft_Ba () const {return left_ba;}
	void setLeft_Ba (const int& left_ba_){left_ba = left_ba_;}
	const int& getRight_Ba () const {return right_ba;}
	void setRight_Ba (const int& right_ba_){right_ba = right_ba_;}
	const int& getPrimaryKey () const {return link;}

//Data Fields
private:
	friend class odb::access;
	#pragma db id
	int link;
#pragma db null
	std::string name;
	node_ptr node_a;
	node_ptr node_b;
	double length;
	double setback_a;
	double setback_b;
	int bearing_a;
	int bearing_b;
	std::string type;
	int divided;
	shared_ptr<Area_Type> area_type;
	std::string use;
	double grade;
	int lanes_ab;
	double speed_ab;
	double fspd_ab;
	int cap_ab;
	int lanes_ba;
	double speed_ba;
	double fspd_ba;
	int cap_ba;
	int left_ab;
	int right_ab;
	int left_ba;
	int right_ba;
};


#pragma db object //table("POCKET")
class Pocket
{
public:
	// Default Constructor
	Pocket () {}	
	//Constructor
	Pocket ( shared_ptr<Link> link_, int dir_, std::string type_, int lanes_, double length_, double offset_ )  
	: link (link_), dir (dir_), type (type_), lanes (lanes_), length (length_), offset (offset_)
	{
	}
	//Accessors
	const shared_ptr<Link>& getLink () const {return link;}
	void setLink (const shared_ptr<Link>& link_){link = link_;}
	void setLink (const int& link_, InputContainer& container){link = container.Links[link_];}
	const int& getDir () const {return dir;}
	void setDir (const int& dir_){dir = dir_;}
	const std::string& getType () const {return type;}
	void setType (const std::string& type_){type = type_;}
	const int& getLanes () const {return lanes;}
	void setLanes (const int& lanes_){lanes = lanes_;}
	const double& getLength () const {return length;}
	void setLength (const double& length_){length = length_;}
	const double& getOffset () const {return offset;}
	void setOffset (const double& offset_){offset = offset_;}
	const unsigned long& getPrimaryKey () const {return pocket;}
	const unsigned long& getPocket () const {return pocket;}

//Data Fields
private:
	friend class odb::access;
	#pragma db id auto
	unsigned long pocket;
	shared_ptr<Link> link;
	int dir;
	std::string type;
	int lanes;
	double length;
	double offset;

};

#pragma db object //table("LANE_USE")
class Lane_Use
{
public:
	// Default Constructor
	Lane_Use () {}	
	//Constructor
	Lane_Use ( shared_ptr<Link> link_, int dir_, int lanes_, int use_, int type_, int min_type_, int max_type_, int min_trav_, int max_trav_, double start_, double end_, double offset_, double length_, int toll_, double rate_, double min_delay_, double max_delay_ )  
	: link (link_), dir (dir_), lanes (lanes_), use (use_), type (type_), min_type (min_type_), max_type (max_type_), min_trav (min_trav_), max_trav (max_trav_), start (start_), end (end_), offset (offset_), length (length_), toll (toll_), rate (rate_), min_delay (min_delay_), max_delay (max_delay_)
	{
	}
	//Accessors
	const shared_ptr<Link>& getLink () const {return link;}
	void setLink (const shared_ptr<Link>& link_){link = link_;}
	void setLink (const int& link_, InputContainer& container){link = container.Links[link_];}
	const int& getDir () const {return dir;}
	void setDir (const int& dir_){dir = dir_;}
	const int& getLanes () const {return lanes;}
	void setLanes (const int& lanes_){lanes = lanes_;}
	const int& getUse () const {return use;}
	void setUse (const int& use_){use = use_;}
	const int& getType () const {return type;}
	void setType (const int& type_){type = type_;}
	const int& getMin_Type () const {return min_type;}
	void setMin_Type (const int& min_type_){min_type = min_type_;}
	const int& getMax_Type () const {return max_type;}
	void setMax_Type (const int& max_type_){max_type = max_type_;}
	const int& getMin_Trav () const {return min_trav;}
	void setMin_Trav (const int& min_trav_){min_trav = min_trav_;}
	const int& getMax_Trav () const {return max_trav;}
	void setMax_Trav (const int& max_trav_){max_trav = max_trav_;}
	const double& getStart () const {return start;}
	void setStart (const double& start_){start = start_;}
	const double& getEnd () const {return end;}
	void setEnd (const double& end_){end = end_;}
	const double& getOffset () const {return offset;}
	void setOffset (const double& offset_){offset = offset_;}
	const double& getLength () const {return length;}
	void setLength (const double& length_){length = length_;}
	const int& getToll () const {return toll;}
	void setToll (const int& toll_){toll = toll_;}
	const double& getRate () const {return rate;}
	void setRate (const double& rate_){rate = rate_;}
	const double& getMin_Delay () const {return min_delay;}
	void setMin_Delay (const double& min_delay_){min_delay = min_delay_;}
	const double& getMax_Delay () const {return max_delay;}
	void setMax_Delay (const double& max_delay_){max_delay = max_delay_;}
	const unsigned long& getPrimaryKey () const {return lane_use;}
	const unsigned long& getLane_Use () const {return lane_use;}

//Data Fields
private:
	friend class odb::access;
	#pragma db id auto
	unsigned long lane_use;
	shared_ptr<Link> link;
	int dir;
	int lanes;
	int use;
	int type;
	int min_type;
	int max_type;
	int min_trav;
	int max_trav;
	double start;
	double end;
	double offset;
	double length;
	int toll;
	double rate;
	double min_delay;
	double max_delay;

};

#pragma db object table("Connection")
class Connect
{
public:
	// Default Constructor
	Connect () {}	
	//Constructor
	Connect ( shared_ptr<Link> link_, int dir_, shared_ptr<Link> to_link_, std::string lanes_, std::string to_lanes_, std::string type_, int penalty_, double speed_, int capacity_, int in_high_, int out_high_ )  
	: link (link_), dir (dir_), to_link (to_link_), lanes (lanes_), to_lanes (to_lanes_), type (type_), penalty (penalty_), speed (speed_), capacity (capacity_), in_high (in_high_), out_high (out_high_)
	{
	}
	//Accessors
	const shared_ptr<Link>& getLink () const {return link;}
	void setLink (const shared_ptr<Link>& link_){link = link_;}
	void setLink (const int& link_, InputContainer& container){link = container.Links[link_];}
	const int& getDir () const {return dir;}
	void setDir (const int& dir_){dir = dir_;}
	const shared_ptr<Link>& getTo_Link () const {return to_link;}
	void setTo_Link (const shared_ptr<Link>& to_link_){to_link = to_link_;}
	void setTo_Link (const int& to_link_, InputContainer& container){to_link = container.Links[to_link_];}
	const std::string& getLanes () const {return lanes;}
	void setLanes (const std::string& lanes_){lanes = lanes_;}
	const std::string& getTo_Lanes () const {return to_lanes;}
	void setTo_Lanes (const std::string& to_lanes_){to_lanes = to_lanes_;}
	const std::string& getType () const {return type;}
	void setType (const std::string& type_){type = type_;}
	const int& getPenalty () const {return penalty;}
	void setPenalty (const int& penalty_){penalty = penalty_;}
	const double& getSpeed () const {return speed;}
	void setSpeed (const double& speed_){speed = speed_;}
	const int& getCapacity () const {return capacity;}
	void setCapacity (const int& capacity_){capacity = capacity_;}
	const int& getIn_High () const {return in_high;}
	void setIn_High (const int& in_high_){in_high = in_high_;}
	const int& getOut_High () const {return out_high;}
	void setOut_High (const int& out_high_){out_high = out_high_;}
	const unsigned long& getPrimaryKey () const {return conn;}
	const unsigned long& getConn () const {return conn;}

//Data Fields
private:
	friend class odb::access;
	#pragma db id auto
	unsigned long conn;
	shared_ptr<Link> link;
	int dir;
	shared_ptr<Link> to_link;
	std::string lanes;
	std::string to_lanes;
	std::string type;
	int penalty;
	double speed;
	int capacity;
	int in_high;
	int out_high;

};

/*#pragma db object //table("TURN_PEN")
class Turn_Pen
{
public:
	// Default Constructor
	Turn_Pen () {}	
	//Constructor
	Turn_Pen ( shared_ptr<Link> link_, int dir_, shared_ptr<Link> to_link_, double start_, double end_, int use_, int min_type_, int max_type_, int penalty_, node_ptr in_node_, node_ptr out_node_ )  
	: link (link_), dir (dir_), to_link (to_link_), start (start_), end (end_), use (use_), min_type (min_type_), max_type (max_type_), penalty (penalty_), in_node (in_node_), out_node (out_node_)
	{
	}
	//Accessors
	const shared_ptr<Link>& getLink () const {return link;}
	void setLink (const shared_ptr<Link>& link_){link = link_;}
	void setLink (const int& link_, InputContainer& container){link = container.Links[link_];}
	const int& getDir () const {return dir;}
	void setDir (const int& dir_){dir = dir_;}
	const shared_ptr<Link>& getTo_Link () const {return to_link;}
	void setTo_Link (const shared_ptr<Link>& to_link_){to_link = to_link_;}
	void setTo_Link (const int& to_link_, InputContainer& container){to_link = container.Links[to_link_];}
	const double& getStart () const {return start;}
	void setStart (const double& start_){start = start_;}
	const double& getEnd () const {return end;}
	void setEnd (const double& end_){end = end_;}
	const int& getUse () const {return use;}
	void setUse (const int& use_){use = use_;}
	const int& getMin_Type () const {return min_type;}
	void setMin_Type (const int& min_type_){min_type = min_type_;}
	const int& getMax_Type () const {return max_type;}
	void setMax_Type (const int& max_type_){max_type = max_type_;}
	const int& getPenalty () const {return penalty;}
	void setPenalty (const int& penalty_){penalty = penalty_;}
	const node_ptr& getIn_Node () const {return in_node;}
	void setIn_Node (const node_ptr& in_node_){in_node = in_node_;}
	void setIn_Node (const int& in_node_, InputContainer& container){in_node = container.Nodes[in_node_];}
	const node_ptr& getOut_Node () const {return out_node;}
	void setOut_Node (const node_ptr& out_node_){out_node = out_node_;}
	void setOut_Node (const int& out_node_, InputContainer& container){out_node = container.Nodes[out_node_];}
	const unsigned long& getPrimaryKey () const {return turn_pen;}
	const unsigned long& getTurn_Pen () const {return turn_pen;}

//Data Fields
private:
	friend class odb::access;
	#pragma db id auto
	unsigned long turn_pen;
	shared_ptr<Link> link;
	int dir;
	shared_ptr<Link> to_link;
	double start;
	double end;
	int use;
	int min_type;
	int max_type;
	int penalty;
	node_ptr in_node;
	node_ptr out_node;

};*/

#pragma db object //table("PARKING")
class Parking
{
public:
	// Default Constructor
	Parking () {}	
	//Constructor
	Parking ( int parking_, shared_ptr<Link> link_, int dir_, double offset_, int type_, int use_, double start_, double end_, int space_, double time_in_, double time_out_, int hourly_, int daily_ )  
	: parking (parking_), link (link_), dir (dir_), offset (offset_), type (type_), use (use_), start (start_), end (end_), space (space_), time_in (time_in_), time_out (time_out_), hourly (hourly_), daily (daily_)
	{
	}
	//Accessors
	const int& getParking () const {return parking;}
	void setParking (const int& parking_){parking = parking_;}
	const shared_ptr<Link>& getLink () const {return link;}
	void setLink (const shared_ptr<Link>& link_){link = link_;}
	void setLink (const int& link_, InputContainer& container){link = container.Links[link_];}
	const int& getDir () const {return dir;}
	void setDir (const int& dir_){dir = dir_;}
	const double& getOffset () const {return offset;}
	void setOffset (const double& offset_){offset = offset_;}
	const int& getType () const {return type;}
	void setType (const int& type_){type = type_;}
	const int& getUse () const {return use;}
	void setUse (const int& use_){use = use_;}
	const double& getStart () const {return start;}
	void setStart (const double& start_){start = start_;}
	const double& getEnd () const {return end;}
	void setEnd (const double& end_){end = end_;}
	const int& getSpace () const {return space;}
	void setSpace (const int& space_){space = space_;}
	const double& getTime_In () const {return time_in;}
	void setTime_In (const double& time_in_){time_in = time_in_;}
	const double& getTime_Out () const {return time_out;}
	void setTime_Out (const double& time_out_){time_out = time_out_;}
	const int& getHourly () const {return hourly;}
	void setHourly (const int& hourly_){hourly = hourly_;}
	const int& getDaily () const {return daily;}
	void setDaily (const int& daily_){daily = daily_;}
	const int& getPrimaryKey () const {return parking;}

//Data Fields
private:
	friend class odb::access;
	#pragma db id
	int parking;
	shared_ptr<Link> link;
	int dir;
	double offset;
	int type;
	int use;
	double start;
	double end;
	int space;
	double time_in;
	double time_out;
	int hourly;
	int daily;
	#pragma db index member(parking)

};

#pragma db object
class Location
{
public:
	// Default Constructor
	Location () {}        
	Location (int location_, shared_ptr<Link> link_, int dir_, double offset_, double setback_, shared_ptr<Zone> zone_, /*shared_ptr<LocationData> location_data_,*/
			 int truck_org_, int truck_des_, int auto_org_, int auto_des_, int transit_, int areatype_, std::string notes_, double census_zone_, double x_, double y_, std::string land_use_, walk_link_ptr walk_link_)
	: location (location_), link (link_), dir (dir_), offset (offset_), setback (setback_), zone (zone_), /*location_data (location_data_)*/
	  truck_org (truck_org_), truck_des (truck_des_), auto_org (auto_org_), auto_des (auto_des_), transit (transit_), area_type (areatype_), notes (notes_), census_zone (census_zone_), x (x_), y (y_), land_use (land_use_), walk_link (walk_link_)
	{
	}
	//Accessors
	const int& getPrimaryKey () const {return location;}
	const int& getLocation () const {return location;}
	void setLocation (const int& location_) {location = location_;}
	void setLink (const int& link_, InputContainer& container) {link = container.Links[link_];}
	const shared_ptr<Link>& getLink () const {return link;}
	void setLink (const shared_ptr<Link>& link_) {link = link_;}
	const int& getDir () const {return dir;}
	void setDir (const int& dir_) {dir = dir_;}
	const double& getOffset () const {return offset;}
	void setOffset (const double& offset_) {offset = offset_;}
	const double& getSetback () const {return setback;}
	void setSetback (const double& setback_) {setback = setback_;}
	void setZone (const int& zone_, InputContainer& container) {zone = container.Zones[zone_];}
	const shared_ptr<Zone>& getZone () const {return zone;}
	void setZone (const shared_ptr<Zone>& zone_) {zone = zone_;}
	const int& getTruck_Org () const {return truck_org;}
	void setTruck_Org (const int& truck_org_) {truck_org = truck_org_;}
	const int& getTruck_Des () const {return truck_des;}
	void setTruck_Des (const int& truck_des_) {truck_des = truck_des_;}
	const int& getAuto_Org () const {return auto_org;}
	void setAuto_Org (const int& auto_org_) {auto_org = auto_org_;}
	const int& getAuto_Des () const {return auto_des;}
	void setAuto_Des (const int& auto_des_) {auto_des = auto_des_;}
	const int& getTransit () const {return transit;}
	void setTransit (const int& transit_) {transit = transit_;}
	const int& getAreatype () const {return area_type;}
	void setAreatype (const int& areatype_) {area_type = areatype_;}
	const std::string& getNotes () const {return notes;}
	void setNotes (const std::string& notes_) {notes = notes_;}
	const double& getCensus_Zone () const {return census_zone;}
	void setCensus_Zone (const double& census_zone_) {census_zone = census_zone_;}
	const double& getX () const {return x;}
	void setX (const double& x_) {x = x_;}
	const double& getY () const {return y;}
	void setY (const double& y_) {y = y_;}
	const std::string& getLand_Use () const {return land_use;}
	void setLand_Use (const std::string& land_use_) {land_use = land_use_;}
	void setWalkLink(const int& walk_link_, InputContainer& container) { walk_link = container.Transit_Walk[walk_link_]; }
	const walk_link_ptr& getWalkLink() const { return walk_link; }
	void setWalkLink(const walk_link_ptr& walk_link_) { walk_link = walk_link_; }
	//const shared_ptr<LocationData>& getLocation_Data () const {return location_data;}
	//void setLocation_Data (const shared_ptr<LocationData>& location_data_) {location_data = location_data_;}
//Data Fields
private:
	friend class odb::access;
	#pragma db id
	int location;
	shared_ptr<Link> link;
	int dir;
	double offset;
	double setback;
	shared_ptr<Zone> zone;
	int truck_org;
	int truck_des;
	int auto_org;
	int auto_des;
	int transit;
	int area_type;
	std::string notes;
	double census_zone;
	double x;
	double y;
	std::string land_use;
	walk_link_ptr walk_link;
	//shared_ptr<LocationData> location_data;
	#pragma db index member(location)
};

//#pragma db object
//class LocationData
//{
//public:
//	// Default Constructor
//	LocationData () {}        
//	LocationData (int location_, int truck_org_, int truck_des_, int auto_org_, int auto_des_, int transit_, int areatype_, std::string notes_, double census_zone_, double x_, double y_, std::string land_use_)
//	: location (location_), truck_org (truck_org_), truck_des (truck_des_), auto_org (auto_org_), auto_des (auto_des_), transit (transit_), areatype (areatype_), notes (notes_), census_zone (census_zone_), x (x_), y (y_), land_use (land_use_)
//	{
//	}
//	//Accessors
//	const int& getPrimaryKey () const {return location;}
//	const int& getLocation () const {return location;}
//	void setLocation (const int& location_) {location = location_;}
//	const int& getTruck_Org () const {return truck_org;}
//	void setTruck_Org (const int& truck_org_) {truck_org = truck_org_;}
//	const int& getTruck_Des () const {return truck_des;}
//	void setTruck_Des (const int& truck_des_) {truck_des = truck_des_;}
//	const int& getAuto_Org () const {return auto_org;}
//	void setAuto_Org (const int& auto_org_) {auto_org = auto_org_;}
//	const int& getAuto_Des () const {return auto_des;}
//	void setAuto_Des (const int& auto_des_) {auto_des = auto_des_;}
//	const int& getTransit () const {return transit;}
//	void setTransit (const int& transit_) {transit = transit_;}
//	const int& getAreatype () const {return areatype;}
//	void setAreatype (const int& areatype_) {areatype = areatype_;}
//	const std::string& getNotes () const {return notes;}
//	void setNotes (const std::string& notes_) {notes = notes_;}
//	const double& getCensus_Zone () const {return census_zone;}
//	void setCensus_Zone (const double& census_zone_) {census_zone = census_zone_;}
//	const double& getX () const {return x;}
//	void setX (const double& x_) {x = x_;}
//	const double& getY () const {return y;}
//	void setY (const double& y_) {y = y_;}
//	const std::string& getLand_Use () const {return land_use;}
//	void setLand_Use (const std::string& land_use_) {land_use = land_use_;}
////Data Fields
//private:
//	friend class odb::access;
//	#pragma db id
//	int location;
//	int truck_org;
//	int truck_des;
//	int auto_org;
//	int auto_des;
//	int transit;
//	int areatype;
//	std::string notes;
//	double census_zone;
//	double x;
//	double y;
//	std::string land_use;
//};

/*
#pragma db object //table("ACCESS")
class Access
{
public:
	// Default Constructor
	Access () {}	
	//Constructor
	Access ( shared_ptr<Link> link_, int from_id_, int from_type_, int to_id_, int to_type_, int dir_, double time_, int cost_ )  
	: link (link_), from_id (from_id_), from_type (from_type_), to_id (to_id_), to_type (to_type_), dir (dir_), time (time_), cost (cost_)
	{
	}
	//Accessors
	const shared_ptr<Link>& getLink () const {return link;}
	void setLink (const shared_ptr<Link>& link_){link = link_;}
	void setLink (const int& link_, InputContainer& container){link = container.Links[link_];}
	const int& getFrom_Id () const {return from_id;}
	void setFrom_Id (const int& from_id_){from_id = from_id_;}
	const int& getFrom_Type () const {return from_type;}
	void setFrom_Type (const int& from_type_){from_type = from_type_;}
	const int& getTo_Id () const {return to_id;}
	void setTo_Id (const int& to_id_){to_id = to_id_;}
	const int& getTo_Type () const {return to_type;}
	void setTo_Type (const int& to_type_){to_type = to_type_;}
	const int& getDir () const {return dir;}
	void setDir (const int& dir_){dir = dir_;}
	const double& getTime () const {return time;}
	void setTime (const double& time_){time = time_;}
	const int& getCost () const {return cost;}
	void setCost (const int& cost_){cost = cost_;}
	const unsigned long& getPrimaryKey () const {return access;}
	const unsigned long& getAccess () const {return access;}

//Data Fields
private:
	friend class odb::access;
	#pragma db id auto
	unsigned long access;
	shared_ptr<Link> link;
	int from_id;
	int from_type;
	int to_id;
	int to_type;
	int dir;
	double time;
	int cost;

};
*/
#pragma db object //table("SIGN")
class Sign
{
public:
	// Default Constructor
	Sign () {}	
	//Constructor
	Sign ( shared_ptr<Link> link_, int dir_, std::string sign_ )  
	: link (link_), dir (dir_), sign (sign_)
	{
	}
	//Accessors
	const shared_ptr<Link>& getLink () const {return link;}
	void setLink (const shared_ptr<Link>& link_){link = link_;}
	void setLink (const int& link_, InputContainer& container){link = container.Links[link_];}
	const int& getDir () const {return dir;}
	void setDir (const int& dir_){dir = dir_;}
	const std::string& getSign () const {return sign;}
	void setSign (const std::string& sign_){sign = sign_;}
	const unsigned long& getPrimaryKey () const {return sign_id;}
	const unsigned long& getSign_Id () const {return sign_id;}

//Data Fields
private:
	friend class odb::access;
	#pragma db id auto
	unsigned long sign_id;
	shared_ptr<Link> link;
	int dir;
	std::string sign;

};

#pragma db object //table("SIGNAL")
class Signal
{
public:
	// Default Constructor
	Signal () {}	
	//Constructor
	Signal ( int signal_, int group_, int times_, node_ptr nodes_, std::string type_, int offset_ )  
	: signal (signal_), group (group_), times (times_), nodes (nodes_), type (type_), offset (offset_)
	{
	}
	//Accessors
	const int& getSignal () const {return signal;}
	void setSignal (const int& signal_){signal = signal_;}
	const int& getGroup () const {return group;}
	void setGroup (const int& group_){group = group_;}
	const int& getTimes () const {return times;}
	void setTimes (const int& times_){times = times_;}
	const node_ptr& getNodes () const {return nodes;}
	void setNodes (const node_ptr& nodes_){nodes = nodes_;}
	void setNodes (const int& nodes_, InputContainer& container){nodes = container.Nodes[nodes_];}
	const std::string& getType () const {return type;}
	void setType (const std::string& type_){type = type_;}
	const int& getOffset () const {return offset;}
	void setOffset (const int& offset_){offset = offset_;}
	const int& getPrimaryKey () const {return signal;}


	//Vector that contains the associated nested records
	 std::vector<signal_time> nested_records;

//Data Fields
private:
	friend class odb::access;
	#pragma db id
	int signal;
	int group;
	int times;
	node_ptr nodes;
	std::string type;
	int offset;
	#pragma db index member(signal)

};

#pragma db object //table("TIMING")
class Timing
{
public:
	// Default Constructor
	Timing () {}	
	//Constructor
	Timing ( shared_ptr<Signal> signal_, int timing_, int type_, int cycle_, int offset_, int phases_ )  
	: signal (signal_), timing (timing_), type (type_), cycle (cycle_), offset (offset_), phases (phases_)
	{
	}
	//Accessors
	const shared_ptr<Signal>& getSignal () const {return signal;}
	void setSignal (const shared_ptr<Signal>& signal_){signal = signal_;}
	void setSignal (const int& signal_, InputContainer& container){signal = container.Signals[signal_];}
	const int& getTiming () const {return timing;}
	void setTiming (const int& timing_){timing = timing_;}
	const int& getType () const {return type;}
	void setType (const int& type_){type = type_;}
	const int& getCycle () const {return cycle;}
	void setCycle (const int& cycle_){cycle = cycle_;}
	const int& getOffset () const {return offset;}
	void setOffset (const int& offset_){offset = offset_;}
	const int& getPhases () const {return phases;}
	void setPhases (const int& phases_){phases = phases_;}
	const unsigned long& getPrimaryKey () const {return timing_id;}
	const unsigned long& getTiming_Id() const {return timing_id;}


	//Vector that contains the associated nested records
	 std::vector<timing_phase> nested_records;

//Data Fields
private:
	friend class odb::access;
	#pragma db id auto
	unsigned long timing_id;
	shared_ptr<Signal> signal;
	int timing;
	int type;
	int cycle;
	int offset;
	int phases;

};

#pragma db object //table("PHASING")
class Phasing
{
public:
	// Default Constructor
	Phasing () {}	
	//Constructor
	Phasing ( shared_ptr<Signal> signal_, int phasing_, int phase_, std::string detectors_, int movements_ )  
	: signal (signal_), phasing (phasing_), phase (phase_), detectors (detectors_), movements (movements_)
	{
	}
	//Accessors
	const shared_ptr<Signal>& getSignal () const {return signal;}
	void setSignal (const shared_ptr<Signal>& signal_){signal = signal_;}
	void setSignal (const int& signal_, InputContainer& container){signal = container.Signals[signal_];}
	const int& getPhasing () const {return phasing;}
	void setPhasing (const int& phasing_){phasing = phasing_;}
	const int& getPhase () const {return phase;}
	void setPhase (const int& phase_){phase = phase_;}
	const std::string& getDetectors () const {return detectors;}
	void setDetectors (const std::string& detectors_){detectors = detectors_;}
	const int& getMovements () const {return movements;}
	void setMovements (const int& movements_){movements = movements_;}
	const unsigned long& getPrimaryKey () const {return phasing_id;}
	const unsigned long& getPhasing_Id () const {return phasing_id;}


	//Vector that contains the associated nested records
	 std::vector<phase_movement> nested_records;

//Data Fields
private:
	friend class odb::access;
	#pragma db id auto
	unsigned long phasing_id;
	shared_ptr<Signal> signal;
	int phasing;
	int phase;
	std::string detectors;
	int movements;

};

/*
#pragma db object //table("DETECTOR")
class Detector
{
public:
	// Default Constructor
	Detector () {}	
	//Constructor
	Detector ( int detector_, shared_ptr<Link> link_, int dir_, double offset_, double length_, int lanes_, int type_, int use_, int low_, int high_ )  
	: detector (detector_), link (link_), dir (dir_), offset (offset_), length (length_), lanes (lanes_), type (type_), use (use_), low (low_), high (high_)
	{
	}
	//Accessors
	const int& getDetector () const {return detector;}
	void setDetector (const int& detector_){detector = detector_;}
	const shared_ptr<Link>& getLink () const {return link;}
	void setLink (const shared_ptr<Link>& link_){link = link_;}
	void setLink (const int& link_, InputContainer& container){link = container.Links[link_];}
	const int& getDir () const {return dir;}
	void setDir (const int& dir_){dir = dir_;}
	const double& getOffset () const {return offset;}
	void setOffset (const double& offset_){offset = offset_;}
	const double& getLength () const {return length;}
	void setLength (const double& length_){length = length_;}
	const int& getLanes () const {return lanes;}
	void setLanes (const int& lanes_){lanes = lanes_;}
	const int& getType () const {return type;}
	void setType (const int& type_){type = type_;}
	const int& getUse () const {return use;}
	void setUse (const int& use_){use = use_;}
	const int& getLow () const {return low;}
	void setLow (const int& low_){low = low_;}
	const int& getHigh () const {return high;}
	void setHigh (const int& high_){high = high_;}
	const int& getPrimaryKey () const {return detector;}

//Data Fields
private:
	friend class odb::access;
	#pragma db id
	int detector;
	shared_ptr<Link> link;
	int dir;
	double offset;
	double length;
	int lanes;
	int type;
	int use;
	int low;
	int high;
	#pragma db index member(detector)

};*/

/*#pragma db object //table("STOP")
class Stop
{
public:
	// Default Constructor
	Stop () {}	
	//Constructor
	Stop ( int stop_, std::string name_, shared_ptr<Link> link_, int dir_, double offset_, int use_, int type_, int space_ )  
	: stop (stop_), name (name_), link (link_), dir (dir_), offset (offset_), use (use_), type (type_), space (space_)
	{
	}
	//Accessors
	const int& getStop () const {return stop;}
	void setStop (const int& stop_){stop = stop_;}
	const std::string& getName () const {return name;}
	void setName (const std::string& name_){name = name_;}
	const shared_ptr<Link>& getLink () const {return link;}
	void setLink (const shared_ptr<Link>& link_){link = link_;}
	void setLink (const int& link_, InputContainer& container){link = container.Links[link_];}
	const int& getDir () const {return dir;}
	void setDir (const int& dir_){dir = dir_;}
	const double& getOffset () const {return offset;}
	void setOffset (const double& offset_){offset = offset_;}
	const int& getUse () const {return use;}
	void setUse (const int& use_){use = use_;}
	const int& getType () const {return type;}
	void setType (const int& type_){type = type_;}
	const int& getSpace () const {return space;}
	void setSpace (const int& space_){space = space_;}
	const int& getPrimaryKey () const {return stop;}

//Data Fields
private:
	friend class odb::access;
	#pragma db id
	int stop;
	std::string name;
	shared_ptr<Link> link;
	int dir;
	double offset;
	int use;
	int type;
	int space;
	#pragma db index member(stop)

};*/

/*#pragma db object //table("FARE")
class Fare
{
public:
	// Default Constructor
	Fare () {}	
	//Constructor
	Fare ( shared_ptr<Zone> from_zone_, shared_ptr<Zone> to_zone_, int from_mode_, int to_mode_, int period_, int type_, int fare_ )  
	: from_zone (from_zone_), to_zone (to_zone_), from_mode (from_mode_), to_mode (to_mode_), period (period_), type (type_), fare (fare_)
	{
	}
	//Accessors
	const shared_ptr<Zone>& getFrom_Zone () const {return from_zone;}
	void setFrom_Zone (const shared_ptr<Zone>& from_zone_){from_zone = from_zone_;}
	void setFrom_Zone (const int& from_zone_, InputContainer& container){from_zone = container.Zones[from_zone_];}
	const shared_ptr<Zone>& getTo_Zone () const {return to_zone;}
	void setTo_Zone (const shared_ptr<Zone>& to_zone_){to_zone = to_zone_;}
	void setTo_Zone (const int& to_zone_, InputContainer& container){to_zone = container.Zones[to_zone_];}
	const int& getFrom_Mode () const {return from_mode;}
	void setFrom_Mode (const int& from_mode_){from_mode = from_mode_;}
	const int& getTo_Mode () const {return to_mode;}
	void setTo_Mode (const int& to_mode_){to_mode = to_mode_;}
	const int& getPeriod () const {return period;}
	void setPeriod (const int& period_){period = period_;}
	const int& getType () const {return type;}
	void setType (const int& type_){type = type_;}
	const int& getFare () const {return fare;}
	void setFare (const int& fare_){fare = fare_;}
	const int& getPrimaryKey () const {return fare;}

//Data Fields
private:
	friend class odb::access;
	shared_ptr<Zone> from_zone;
	shared_ptr<Zone> to_zone;
	int from_mode;
	int to_mode;
	int period;
	int type;
	#pragma db id
	int fare;
	#pragma db index member(fare)

};*/

/*#pragma db object //table("LINE")
class Line
{
public:
	// Default Constructor
	Line () {}	
	//Constructor
	Line ( int route_, shared_ptr<Stop> stops_, int mode_, int type_, std::string name_, shared_ptr<Stop> stop_, shared_ptr<Zone> zone_, int flag_ )  
	: route (route_), stops (stops_), mode (mode_), type (type_), name (name_), stop (stop_), zone (zone_), flag (flag_)
	{
	}
	//Accessors
	const int& getRoute () const {return route;}
	void setRoute (const int& route_){route = route_;}
	const shared_ptr<Stop>& getStops () const {return stops;}
	void setStops (const shared_ptr<Stop>& stops_){stops = stops_;}
	void setStops (const int& stops_, InputContainer& container){stops = container.Stops[stops_];}
	const int& getMode () const {return mode;}
	void setMode (const int& mode_){mode = mode_;}
	const int& getType () const {return type;}
	void setType (const int& type_){type = type_;}
	const std::string& getName () const {return name;}
	void setName (const std::string& name_){name = name_;}
	const shared_ptr<Stop>& getStop () const {return stop;}
	void setStop (const shared_ptr<Stop>& stop_){stop = stop_;}
	void setStop (const int& stop_, InputContainer& container){stop = container.Stops[stop_];}
	const shared_ptr<Zone>& getZone () const {return zone;}
	void setZone (const shared_ptr<Zone>& zone_){zone = zone_;}
	void setZone (const int& zone_, InputContainer& container){zone = container.Zones[zone_];}
	const int& getFlag () const {return flag;}
	void setFlag (const int& flag_){flag = flag_;}
	const unsigned long& getPrimaryKey () const {return line;}
	const unsigned long& getLine () const {return line;}

//Data Fields
private:
	friend class odb::access;
	#pragma db id auto
	unsigned long line;
	int route;
	shared_ptr<Stop> stops;
	int mode;
	int type;
	std::string name;
	shared_ptr<Stop> stop;
	shared_ptr<Zone> zone;
	int flag;

};*/

/*#pragma db object //table("SCHEDULE")
class Schedule
{
public:
	// Default Constructor
	Schedule () {}	
	//Constructor
	Schedule ( int route_, shared_ptr<Stop> stops_, shared_ptr<Stop> stop_ )  
	: route (route_), stops (stops_), stop (stop_)
	{
	}
	//Accessors
	const int& getRoute () const {return route;}
	void setRoute (const int& route_){route = route_;}
	const shared_ptr<Stop>& getStops () const {return stops;}
	void setStops (const shared_ptr<Stop>& stops_){stops = stops_;}
	void setStops (const int& stops_, InputContainer& container){stops = container.Stops[stops_];}
	const shared_ptr<Stop>& getStop () const {return stop;}
	void setStop (const shared_ptr<Stop>& stop_){stop = stop_;}
	void setStop (const int& stop_, InputContainer& container){stop = container.Stops[stop_];}
	const unsigned long& getPrimaryKey () const {return schedule;}
	const unsigned long& getSchedule () const {return schedule;}

//Data Fields
private:
	friend class odb::access;
	#pragma db id auto
	unsigned long schedule;
	int route;
	shared_ptr<Stop> stops;
	shared_ptr<Stop> stop;

};*/

/*#pragma db object //table("DRIVER")
class Driver
{
public:
	// Default Constructor
	Driver () {}	
	//Constructor
	Driver ( int route_, shared_ptr<Link> links_, int type_, int subtype_, shared_ptr<Link> link_, int dir_ )  
	: route (route_), links (links_), type (type_), subtype (subtype_), link (link_), dir (dir_)
	{
	}
	//Accessors
	const int& getRoute () const {return route;}
	void setRoute (const int& route_){route = route_;}
	const shared_ptr<Link>& getLinks () const {return links;}
	void setLinks (const shared_ptr<Link>& links_){links = links_;}
	void setLinks (const int& links_, InputContainer& container){links = container.Links[links_];}
	const int& getType () const {return type;}
	void setType (const int& type_){type = type_;}
	const int& getSubtype () const {return subtype;}
	void setSubtype (const int& subtype_){subtype = subtype_;}
	const shared_ptr<Link>& getLink () const {return link;}
	void setLink (const shared_ptr<Link>& link_){link = link_;}
	void setLink (const int& link_, InputContainer& container){link = container.Links[link_];}
	const int& getDir () const {return dir;}
	void setDir (const int& dir_){dir = dir_;}
	const unsigned long& getPrimaryKey () const {return driver;}
	const unsigned long& getDriver () const {return driver;}

//Data Fields
private:
	friend class odb::access;
	#pragma db id auto
	unsigned long driver;
	int route;
	shared_ptr<Link> links;
	int type;
	int subtype;
	shared_ptr<Link> link;
	int dir;

};*/

/*#pragma db object //table("ROUTE_NODES")
class Route_Nodes
{
public:
	// Default Constructor
	Route_Nodes () {}	
	//Constructor
	Route_Nodes ( int route_, int mode_, shared_ptr<Veh_Type> veh_type_, node_ptr nodes_, std::string name_, node_ptr node_, int type_, double dwell_, double time_, double speed_ )  
	: route (route_), mode (mode_), veh_type (veh_type_), nodes (nodes_), name (name_), node (node_), type (type_), dwell (dwell_), time (time_), speed (speed_)
	{
	}
	//Accessors
	const int& getRoute () const {return route;}
	void setRoute (const int& route_){route = route_;}
	const int& getMode () const {return mode;}
	void setMode (const int& mode_){mode = mode_;}
	const shared_ptr<Veh_Type>& getVeh_Type () const {return veh_type;}
	void setVeh_Type (const shared_ptr<Veh_Type>& veh_type_){veh_type = veh_type_;}
	void setVeh_Type (const int& veh_type_, InputContainer& container){veh_type = container.Veh_Types[veh_type_];}
	const node_ptr& getNodes () const {return nodes;}
	void setNodes (const node_ptr& nodes_){nodes = nodes_;}
	void setNodes (const int& nodes_, InputContainer& container){nodes = container.Nodes[nodes_];}
	const std::string& getName () const {return name;}
	void setName (const std::string& name_){name = name_;}
	const node_ptr& getNode () const {return node;}
	void setNode (const node_ptr& node_){node = node_;}
	void setNode (const int& node_, InputContainer& container){node = container.Nodes[node_];}
	const int& getType () const {return type;}
	void setType (const int& type_){type = type_;}
	const double& getDwell () const {return dwell;}
	void setDwell (const double& dwell_){dwell = dwell_;}
	const double& getTime () const {return time;}
	void setTime (const double& time_){time = time_;}
	const double& getSpeed () const {return speed;}
	void setSpeed (const double& speed_){speed = speed_;}
	const unsigned long& getPrimaryKey () const {return route_nodes;}
	const unsigned long& getRoute_Nodes () const {return route_nodes;}

//Data Fields
private:
	friend class odb::access;
	#pragma db id auto
	unsigned long route_nodes;
	int route;
	int mode;
	shared_ptr<Veh_Type> veh_type;
	node_ptr nodes;
	std::string name;
	node_ptr node;
	int type;
	double dwell;
	double time;
	double speed;

};*/

/*#pragma db object //table("VEH_TYPE")
class Veh_Type
{
public:
	// Default Constructor
	Veh_Type () {}	
	//Constructor
	Veh_Type ( int type_, double length_, double max_speed_, double max_accel_, double max_decel_, double op_cost_, int use_, int capacity_, double load_, double unload_, int method_, double min_dwell_, double max_dwell_, int subtype_ )  
	: type (type_), length (length_), max_speed (max_speed_), max_accel (max_accel_), max_decel (max_decel_), op_cost (op_cost_), use (use_), capacity (capacity_), load (load_), unload (unload_), method (method_), min_dwell (min_dwell_), max_dwell (max_dwell_), subtype (subtype_)
	{
	}
	//Accessors
	const int& getType () const {return type;}
	void setType (const int& type_){type = type_;}
	const double& getLength () const {return length;}
	void setLength (const double& length_){length = length_;}
	const double& getMax_Speed () const {return max_speed;}
	void setMax_Speed (const double& max_speed_){max_speed = max_speed_;}
	const double& getMax_Accel () const {return max_accel;}
	void setMax_Accel (const double& max_accel_){max_accel = max_accel_;}
	const double& getMax_Decel () const {return max_decel;}
	void setMax_Decel (const double& max_decel_){max_decel = max_decel_;}
	const double& getOp_Cost () const {return op_cost;}
	void setOp_Cost (const double& op_cost_){op_cost = op_cost_;}
	const int& getUse () const {return use;}
	void setUse (const int& use_){use = use_;}
	const int& getCapacity () const {return capacity;}
	void setCapacity (const int& capacity_){capacity = capacity_;}
	const double& getLoad () const {return load;}
	void setLoad (const double& load_){load = load_;}
	const double& getUnload () const {return unload;}
	void setUnload (const double& unload_){unload = unload_;}
	const int& getMethod () const {return method;}
	void setMethod (const int& method_){method = method_;}
	const double& getMin_Dwell () const {return min_dwell;}
	void setMin_Dwell (const double& min_dwell_){min_dwell = min_dwell_;}
	const double& getMax_Dwell () const {return max_dwell;}
	void setMax_Dwell (const double& max_dwell_){max_dwell = max_dwell_;}
	const int& getSubtype () const {return subtype;}
	void setSubtype (const int& subtype_){subtype = subtype_;}
	const int& getPrimaryKey () const {return type;}

//Data Fields
private:
	friend class odb::access;
	#pragma db id
	int type;
	double length;
	double max_speed;
	double max_accel;
	double max_decel;
	double op_cost;
	int use;
	int capacity;
	double load;
	double unload;
	int method;
	double min_dwell;
	double max_dwell;
	int subtype;
	#pragma db index member(type)

};*/


/*#pragma db object //table("RIDERSHIP")
class Ridership
{
public:
	// Default Constructor
	Ridership () {}	
	//Constructor
	Ridership ( int mode_, int route_, int run_, shared_ptr<Stop> stop_, double schedule_, double time_, int board_, int alight_, int load_, double factor_ )  
	: mode (mode_), route (route_), run (run_), stop (stop_), schedule (schedule_), time (time_), board (board_), alight (alight_), load (load_), factor (factor_)
	{
	}
	//Accessors
	const int& getMode () const {return mode;}
	void setMode (const int& mode_){mode = mode_;}
	const int& getRoute () const {return route;}
	void setRoute (const int& route_){route = route_;}
	const int& getRun () const {return run;}
	void setRun (const int& run_){run = run_;}
	const shared_ptr<Stop>& getStop () const {return stop;}
	void setStop (const shared_ptr<Stop>& stop_){stop = stop_;}
	void setStop (const int& stop_, InputContainer& container){stop = container.Stops[stop_];}
	const double& getSchedule () const {return schedule;}
	void setSchedule (const double& schedule_){schedule = schedule_;}
	const double& getTime () const {return time;}
	void setTime (const double& time_){time = time_;}
	const int& getBoard () const {return board;}
	void setBoard (const int& board_){board = board_;}
	const int& getAlight () const {return alight;}
	void setAlight (const int& alight_){alight = alight_;}
	const int& getLoad () const {return load;}
	void setLoad (const int& load_){load = load_;}
	const double& getFactor () const {return factor;}
	void setFactor (const double& factor_){factor = factor_;}
	const unsigned long& getPrimaryKey () const {return ridership;}
	const unsigned long& getRidership () const {return ridership;}

//Data Fields
private:
	friend class odb::access;
	#pragma db id auto
	unsigned long ridership;
	int mode;
	int route;
	int run;
	shared_ptr<Stop> stop;
	double schedule;
	double time;
	int board;
	int alight;
	int load;
	double factor;

};*/


#pragma db object
class Area_Type
{
public:
	// Default Constructor
	Area_Type () {}        
	Area_Type (int area_type_, std::string name_, std::string notes_)
	: area_type (area_type_), name (name_), notes (notes_)
	{
	}
	//Accessors
	const int& getArea_Type () const {return area_type;}
	void setArea_Type (const int& area_type_) {area_type = area_type_;}
	const std::string& getName () const {return name;}
	void setName (const std::string& name_) {name = name_;}
	const std::string& getNotes () const {return notes;}
	void setNotes (const std::string& notes_) {notes = notes_;}
	int getPrimaryKey() const {return area_type;}
//Data Fields
private:
	friend class odb::access;
	#pragma db id
	int area_type;
	std::string name;
	std::string notes;
};


/*#pragma db object
class Link_Type
{
public:
	// Default Constructor
	Link_Type () {}        
	Link_Type (std::string link_type_, int rank_, std::string use_codes_, std::string alternative_labels_, std::string notes_)
	: link_type (link_type_), rank (rank_), use_codes (use_codes_), alternative_labels (alternative_labels_), notes (notes_)
	{
	}
	//Accessors
	const std::string& getLink_Type () const {return link_type;}
	void setLink_Type (const std::string& link_type_) {link_type = link_type_;}
	const int& getRank () const {return rank;}
	void setRank (const int& rank_) {rank = rank_;}
	const std::string& getUse_Codes () const {return use_codes;}
	void setUse_Codes (const std::string& use_codes_) {use_codes = use_codes_;}
	const std::string& getAlternative_Labels () const {return alternative_labels;}
	void setAlternative_Labels (const std::string& alternative_labels_) {alternative_labels = alternative_labels_;}
	const std::string& getNotes () const {return notes;}
	void setNotes (const std::string& notes_) {notes = notes_;}
	const std::string& getPrimaryKey() const {return link_type;}
//Data Fields
private:
	friend class odb::access;
	#pragma db id
	std::string link_type;
	int rank;
	std::string use_codes;
	std::string alternative_labels;
	std::string notes;
};*/

/*#pragma db object
class Use_Code
{
public:
	// Default Constructor
	Use_Code () {}        
	Use_Code (std::string use_code_, int rank_, int routable_, std::string subset_of_, std::string superset_of_, std::string alternative_labels_, std::string notes_)
	: use_code (use_code_), rank (rank_), routable (routable_), subset_of (subset_of_), superset_of (superset_of_), alternative_labels (alternative_labels_), notes (notes_)
	{
	}
	//Accessors
	const std::string& getUse_Code () const {return use_code;}
	void setUse_Code (const std::string& use_code_) {use_code = use_code_;}
	const int& getRank () const {return rank;}
	void setRank (const int& rank_) {rank = rank_;}
	const int& getRoutable () const {return routable;}
	void setRoutable (const int& routable_) {routable = routable_;}
	const std::string& getSubset_Of () const {return subset_of;}
	void setSubset_Of (const std::string& subset_of_) {subset_of = subset_of_;}
	const std::string& getSuperset_Of () const {return superset_of;}
	void setSuperset_Of (const std::string& superset_of_) {superset_of = superset_of_;}
	const std::string& getAlternative_Labels () const {return alternative_labels;}
	void setAlternative_Labels (const std::string& alternative_labels_) {alternative_labels = alternative_labels_;}
	const std::string& getNotes () const {return notes;}
	void setNotes (const std::string& notes_) {notes = notes_;}
//Data Fields
private:
	friend class odb::access;
	#pragma db id
	std::string use_code;
	int rank;
	int routable;
	std::string subset_of;
	std::string superset_of;
	std::string alternative_labels;
	std::string notes;
};*/

/*#pragma db object
class LinkList
{
public:
	// Default Constructor
	LinkList () {}        
	LinkList (int id_, std::vector<int > links_)
	: id (id_), links (links_)
	{
	}
	//Accessors
	const int& getId () const {return id;}
	void setId (const int& id_) {id = id_;}
	const std::vector<int >& getLinks () const {return links;}
	void setLinks (const std::vector<int >& links_) {links = links_;}
	void setLink (const int  links_) {links.push_back(links_);}
	//Data Fields
private:
	friend class odb::access;
	#pragma db auto id
	int id;
	std::vector<int > links;
};


#pragma db object
class Component
{
public:
	// Default Constructor
	Component () {}        
	Component (int id_, std::string name_, std::string icon_, std::vector<std::weak_ptr<Action> > actions_)
	: id (id_), name (name_), icon (icon_), actions (actions_)
	{
	}
	Component (int id_, std::string name_, std::string icon_)
	: id (id_), name (name_), icon (icon_)
	{
	}
	//Accessors
	const int& getId () const {return id;}
	void setId (const int& id_) {id = id_;}
	const std::string& getName () const {return name;}
	void setName (const std::string& name_) {name = name_;}
	const std::string& getIcon () const {return icon;}
	void setIcon (const std::string& icon_) {icon = icon_;}
	const std::vector<std::weak_ptr<Action> >& getActions () const {return actions;}
	void setActions (const std::vector<std::weak_ptr<Action> >& actions_) {actions = actions_;}
	void setAction (const std::weak_ptr<Action>  actions_) {actions.push_back(actions_);}
	//Data Fields
private:
	friend class odb::access;
	#pragma db id
	int id;
	std::string name;
	std::string icon;
	#pragma db inverse(component)
	std::vector<std::weak_ptr<Action> > actions;
};*/


#pragma db object
class VMS
{
public:
	// Default Constructor
	VMS () {}        
	VMS (int id_, int link_, int dir_, float offset_, float setback_, int initial_event_)
	: id (id_), link (link_), dir (dir_), offset (offset_), setback (setback_), initial_event (initial_event_)
	{
	}
	//Accessors
	const int& getId () const {return id;}
	void setId (const int& id_) {id = id_;}
	const int& getLink () const {return link;}
	void setLink (const int& link_) {link = link_;}
	const int& getDir () const {return dir;}
	void setDir (const int& dir_) {dir = dir_;}
	const float& getOffset () const {return offset;}
	void setOffset (const float& offset_) {offset = offset_;}
	const float& getSetback () const {return setback;}
	void setSetback (const float& setback_) {setback = setback_;}
	const int& getInitial_Event () const {return initial_event;}
	void setInitial_Event (const int& initial_event_) {initial_event = initial_event_;}
	//Data Fields
private:
	friend class odb::access;
	#pragma db auto id
	int id;
	#pragma db not_null
	int link;
	int dir;
	float offset;
	float setback;
	#pragma db default(-1)
	int initial_event;
};


#pragma db object
class HAR
{
public:
	// Default Constructor
	HAR () {}        
	HAR (int id_, int link_, int dir_, float offset_, float setback_, int initial_event_)
	: id (id_), link (link_), dir (dir_), offset (offset_), setback (setback_), initial_event (initial_event_)
	{
	}
	//Accessors
	const int& getId () const {return id;}
	void setId (const int& id_) {id = id_;}
	const int& getLink () const {return link;}
	void setLink (const int& link_) {link = link_;}
	const int& getDir () const {return dir;}
	void setDir (const int& dir_) {dir = dir_;}
	const float& getOffset () const {return offset;}
	void setOffset (const float& offset_) {offset = offset_;}
	const float& getSetback () const {return setback;}
	void setSetback (const float& setback_) {setback = setback_;}
	const int& getInitial_Event () const {return initial_event;}
	void setInitial_Event (const int& initial_event_) {initial_event = initial_event_;}
	//Data Fields
private:
	friend class odb::access;
	#pragma db auto id
	int id;
	#pragma db not_null
	int link;
	int dir;
	float offset;
	float setback;
	#pragma db default(-1)
	int initial_event;
};


#pragma db object
class VSS
{
public:
	// Default Constructor
	VSS () {}        
	VSS (int id_, int link_, int dir_, float offset_, float setback_, int initial_speed_, std::string speed_)
	: id (id_), link (link_), dir (dir_), offset (offset_), setback (setback_), initial_speed (initial_speed_), speed (speed_)
	{
	}
	//Accessors
	const int& getId () const {return id;}
	void setId (const int& id_) {id = id_;}
	const int& getLink () const {return link;}
	void setLink (const int& link_) {link = link_;}
	const int& getDir () const {return dir;}
	void setDir (const int& dir_) {dir = dir_;}
	const float& getOffset () const {return offset;}
	void setOffset (const float& offset_) {offset = offset_;}
	const float& getSetback () const {return setback;}
	void setSetback (const float& setback_) {setback = setback_;}
	const int& getInitial_Speed () const {return initial_speed;}
	void setInitial_Speed (const int& initial_speed_) {initial_speed = initial_speed_;}
	const std::string& getSpeed () const {return speed;}
	void setSpeed (const std::string& speed_) {speed = speed_;}
	//Data Fields
private:
	friend class odb::access;
	#pragma db auto id
	int id;
	#pragma db not_null
	int link;
	int dir;
	float offset;
	float setback;
	#pragma db default(35)
	int initial_speed;
	#pragma db default("list(35,40,45,55,60)")
	std::string speed;
};


/*#pragma db object
class Depot
{
public:
	// Default Constructor
	Depot () {}        
	Depot (int id_, shared_ptr<Component> component_, int link_, int dir_, float offset_, float setback_, std::string policy_, shared_ptr<LinkList> links_, int fleet_size_, int available_)
	: id (id_), component (component_), link (link_), dir (dir_), offset (offset_), setback (setback_), policy (policy_), links (links_), fleet_size (fleet_size_), available (available_)
	{
	}
	Depot (int id_, int link_, int dir_, float offset_, float setback_, std::string policy_, int fleet_size_, int available_)
	: id (id_), link (link_), dir (dir_), offset (offset_), setback (setback_), policy (policy_), fleet_size (fleet_size_), available (available_)
	{
	}
	//Accessors
	const int& getId () const {return id;}
	void setId (const int& id_) {id = id_;}
	const shared_ptr<Component> getComponent () const {return component;}
	void setComponent (const shared_ptr<Component> component_) {component = component_;}
	const int& getLink () const {return link;}
	void setLink (const int& link_) {link = link_;}
	const int& getDir () const {return dir;}
	void setDir (const int& dir_) {dir = dir_;}
	const float& getOffset () const {return offset;}
	void setOffset (const float& offset_) {offset = offset_;}
	const float& getSetback () const {return setback;}
	void setSetback (const float& setback_) {setback = setback_;}
	const std::string& getPolicy () const {return policy;}
	void setPolicy (const std::string& policy_) {policy = policy_;}
	const shared_ptr<LinkList> getLinks () const {return links;}
	void setLinks (const shared_ptr<LinkList> links_) {links = links_;}
	const int& getFleet_Size () const {return fleet_size;}
	void setFleet_Size (const int& fleet_size_) {fleet_size = fleet_size_;}
	const int& getAvailable () const {return available;}
	void setAvailable (const int& available_) {available = available_;}
	//Data Fields
private:
	friend class odb::access;
	#pragma db auto id
	int id;
	#pragma db not_null
	shared_ptr<Component> component;
	int link;
	int dir;
	float offset;
	float setback;
	std::string policy;
	shared_ptr<LinkList> links;
	#pragma db default(99999)
	int fleet_size;
	#pragma db default(99999)
	int available;
};


#pragma db object
class OpenShoulder
{
public:
	// Default Constructor
	OpenShoulder () {}        
	OpenShoulder (int id_, bool open_)
	: id (id_), open (open_)
	{
	}
	//Accessors
	const int& getId () const {return id;}
	void setId (const int& id_) {id = id_;}
	const bool& getOpen () const {return open;}
	void setOpen (const bool& open_) {open = open_;}
	//Data Fields
private:
	friend class odb::access;
	#pragma db auto id
	int id;
	#pragma db default(false)
	bool open;
};


/*#pragma db object
class Action
{
public:
	// Default Constructor
	Action () {}        
	Action (int id_, shared_ptr<Component> component_, std::vector<shared_ptr<Action_Key> > keys_, std::string name_, std::string note_)
	: id (id_), component (component_), keys (keys_), name (name_), note (note_)
	{
	}
	Action (int id_, std::string name_, std::string note_)
	: id (id_), name (name_), note (note_)
	{
	}
	//Accessors
	const int& getId () const {return id;}
	void setId (const int& id_) {id = id_;}
	const shared_ptr<Component> getComponent () const {return component;}
	void setComponent (const shared_ptr<Component> component_) {component = component_;}
	const std::vector<shared_ptr<Action_Key> >& getKeys () const {return keys;}
	void setKeys (const std::vector<shared_ptr<Action_Key> >& keys_) {keys = keys_;}
	void setKey (const shared_ptr<Action_Key>  keys_) {keys.push_back(keys_);}
	const std::string& getName () const {return name;}
	void setName (const std::string& name_) {name = name_;}
	const std::string& getNote () const {return note;}
	void setNote (const std::string& note_) {note = note_;}
	//Data Fields
private:
	friend class odb::access;
	#pragma db auto id
	int id;
	#pragma db not_null
	shared_ptr<Component> component;
	std::vector<shared_ptr<Action_Key> > keys;
	std::string name;
	std::string note;
};

#pragma db object
class Fixed_Sensor
{
public:
	// Default Constructor
	Fixed_Sensor () {}        
	Fixed_Sensor (int id_, int link_, bool dir_, double offset_, double sigma_, int aggregation_period_sec_, std::string original_name_)
		: id (id_), link (link_), dir (dir_), offset (offset_), sigma (sigma_), aggregation_period_sec (aggregation_period_sec_), original_name (original_name_)
	{
	}
	//Accessors
	const int& getId () const {return id;}
	void setId (const int& id_) {id = id_;}
	const int& getLink () const {return link;}
	void setLink (const int& link_) {link = link_;}
	const bool& getDir () const {return dir;}
	void setDir (const bool& dir_) {dir = dir_;}
	const double& getOffset () const {return offset;}
	void setOffset (const double& offset_) {offset = offset_;}
	const double& getSigma () const {return sigma;}
	void setSigma (const double& sigma_) {sigma = sigma_;}
	const int& getAggregation_Period_Sec () const {return aggregation_period_sec;}
	void setAggregation_Period_Sec (const int& aggregation_period_sec_) {aggregation_period_sec = aggregation_period_sec_;}
	const std::string& getOriginal_Name () const {return original_name;}
	void setOriginal_Name (const std::string& original_name_) {original_name = original_name_;}
	//Data Fields
private:
	friend class odb::access;
#pragma db auto id
	int id;
	int link;
	bool dir;
	double offset;
	double sigma;
	int aggregation_period_sec;
	std::string original_name;
};
#pragma db object
class Action_Key
{
public:
	// Default Constructor
	Action_Key () {}        
	Action_Key (std::string key_, std::string value_type_, std::string value_constraint_, bool required_, std::string note_)
	: key (key_), value_type (value_type_), value_constraint (value_constraint_), required (required_), note (note_)
	{
	}
	//Accessors
	const std::string& getKey () const {return key;}
	void setKey (const std::string& key_) {key = key_;}
	const std::string& getValue_Type () const {return value_type;}
	void setValue_Type (const std::string& value_type_) {value_type = value_type_;}
	const std::string& getValue_Constraint () const {return value_constraint;}
	void setValue_Constraint (const std::string& value_constraint_) {value_constraint = value_constraint_;}
	const bool& getRequired () const {return required;}
	void setRequired (const bool& required_) {required = required_;}
	const std::string& getNote () const {return note;}
	void setNote (const std::string& note_) {note = note_;}
	//Data Fields
private:
	friend class odb::access;
	#pragma db id
	std::string key;
	std::string value_type;
	std::string value_constraint;
	bool required;
	std::string note;
};
#pragma db object
class Microsoft_Event
{
public:
	// Default Constructor
	Microsoft_Event () {}        
	Microsoft_Event (int incidentId_, int end_, int lastModified_, int start_, int type_, int severity_, std::string description_, std::string congestion_, std::string lane_, double lat_, double lng_, double x_, double y_, bool roadClosed_, bool verified_, int link_id_, double link_distance_)
		: incidentId (incidentId_), end (end_), lastModified (lastModified_), start (start_), type (type_), severity (severity_), description (description_), congestion (congestion_), lane (lane_), lat (lat_), lng (lng_), x (x_), y (y_), roadClosed (roadClosed_), verified (verified_), link_id (link_id_), link_distance (link_distance_)
	{
	}
	//Accessors
	const int& getIncidentid () const {return incidentId;}
	void setIncidentid (const int& incidentId_) {incidentId = incidentId_;}
	const int& getEnd () const {return end;}
	void setEnd (const int& end_) {end = end_;}
	const int& getLastmodified () const {return lastModified;}
	void setLastmodified (const int& lastModified_) {lastModified = lastModified_;}
	const int& getStart () const {return start;}
	void setStart (const int& start_) {start = start_;}
	const int& getType () const {return type;}
	void setType (const int& type_) {type = type_;}
	const int& getSeverity () const {return severity;}
	void setSeverity (const int& severity_) {severity = severity_;}
	const std::string& getDescription () const {return description;}
	void setDescription (const std::string& description_) {description = description_;}
	const std::string& getCongestion () const {return congestion;}
	void setCongestion (const std::string& congestion_) {congestion = congestion_;}
	const std::string& getLane () const {return lane;}
	void setLane (const std::string& lane_) {lane = lane_;}
	const double& getLat () const {return lat;}
	void setLat (const double& lat_) {lat = lat_;}
	const double& getLng () const {return lng;}
	void setLng (const double& lng_) {lng = lng_;}
	const double& getX () const {return x;}
	void setX (const double& x_) {x = x_;}
	const double& getY () const {return y;}
	void setY (const double& y_) {y = y_;}
	const bool& getRoadclosed () const {return roadClosed;}
	void setRoadclosed (const bool& roadClosed_) {roadClosed = roadClosed_;}
	const bool& getVerified () const {return verified;}
	void setVerified (const bool& verified_) {verified = verified_;}
	const int& getLink_Id () const {return link_id;}
	void setLink_Id (const int& link_id_) {link_id = link_id_;}
	const double& getLink_Distance () const {return link_distance;}
	void setLink_Distance (const double& link_distance_) {link_distance = link_distance_;}
	//Data Fields
private:
	friend class odb::access;
#pragma db id
	int incidentId;
	int end;
	int lastModified;
	int start;
	int type;
	int severity;
	std::string description;
	std::string congestion;
	std::string lane;
	double lat;
	double lng;
	double x;
	double y;
	bool roadClosed;
	bool verified;
	int link_id;
	double link_distance;
};*/
}//end of io namespace 
}//end of polaris namespace
#endif // IOSupply
