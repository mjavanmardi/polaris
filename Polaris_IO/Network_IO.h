
#ifndef InputContext
#define InputContext
#pragma warning(disable:4068)

#include <map>
#include <vector>
#include <string>

#include <odb/core.hxx>

// Include TR1 <memory> header in a compiler-specific fashion. Fall back
// on the Boost implementation if the compiler does not support TR1.
//
#include <odb/tr1/memory.hxx>
using std::tr1::shared_ptr;
namespace pio
{
//Forward declarations.
//
class Node;
class Zone;
class Shape;
class Link;
class Pocket;
class Lane_Use;
class Connect;
class Turn_Pen;
class Parking;
class Location;
class Access;
class Sign;
class Signal;
class Timing;
class Phasing;
class Detector;
class Stop;
class Fare;
class Line;
class Schedule;
class Driver;
class Route_Nodes;
class Selection;
class Household;
class Link_Delay;
class Performance;
class Ridership;
class Veh_Type;
class Vehicle;
class Trip;
class Problem;
class Plan;
class Skim;
class Event;
class Traveler;
class InputContainer;
//Input Container.
//
class InputContainer 
{
public:
	std::map<int,shared_ptr<Node>> Nodes;
	std::map<int,shared_ptr<Zone>> Zones;
	std::map<int,shared_ptr<Link>> Links;
	std::map<int,shared_ptr<Parking>> Parkings;
	std::map<int,shared_ptr<Location>> Locations;
	std::map<int,shared_ptr<Signal>> Signals;
	std::map<int,shared_ptr<Stop>> Stops;
	std::map<int,shared_ptr<Veh_Type>> Veh_Types;
};

#pragma db object
class Node
{
public:
	// Default Constructor
	Node () {}	
	//Contructor
	Node ( int node_, double x_, double y_, double z_, int subarea_, int part_ )  
	: node (node_), x (x_), y (y_), z (z_), subarea (subarea_), part (part_)
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

//Data Fields
private:
	friend class odb::access;
	#pragma db id
	int node;
	double x;
	double y;
	double z;
	int subarea;
	int part;

};

#pragma db object
class Zone
{
public:
	// Default Constructor
	Zone () {}	
	//Contructor
	Zone ( int zone_, double x_, double y_, double z_, int area_, double min_x_, double min_y_, double max_x_, double max_y_ )  
	: zone (zone_), x (x_), y (y_), z (z_), area (area_), min_x (min_x_), min_y (min_y_), max_x (max_x_), max_y (max_y_)
	{
	}
	//Accessors
	const int& getZone () const {return zone;}
	void setZone (const int& zone_){zone = zone_;}
	const double& getX () const {return x;}
	void setX (const double& x_){x = x_;}
	const double& getY () const {return y;}
	void setY (const double& y_){y = y_;}
	const double& getZ () const {return z;}
	void setZ (const double& z_){z = z_;}
	const int& getArea () const {return area;}
	void setArea (const int& area_){area = area_;}
	const double& getMin_X () const {return min_x;}
	void setMin_X (const double& min_x_){min_x = min_x_;}
	const double& getMin_Y () const {return min_y;}
	void setMin_Y (const double& min_y_){min_y = min_y_;}
	const double& getMax_X () const {return max_x;}
	void setMax_X (const double& max_x_){max_x = max_x_;}
	const double& getMax_Y () const {return max_y;}
	void setMax_Y (const double& max_y_){max_y = max_y_;}
	const int& getPrimaryKey () const {return zone;}

//Data Fields
private:
	friend class odb::access;
	#pragma db id
	int zone;
	double x;
	double y;
	double z;
	int area;
	double min_x;
	double min_y;
	double max_x;
	double max_y;

};

#pragma db object
class Shape
{
public:
	// Default Constructor
	Shape () {}	
	//Contructor
	Shape ( shared_ptr<Link> link_, int points_, double x_, double y_, double z_ )  
	: link (link_), points (points_), x (x_), y (y_), z (z_)
	{
	}
	//Accessors
	const shared_ptr<Link>& getLink () const {return link;}
	void setLink (const shared_ptr<Link>& link_){link = link_;}
	void setLink (const int& link_, InputContainer& container){link = container.Links[link_];}
	const int& getPoints () const {return points;}
	void setPoints (const int& points_){points = points_;}
	const double& getX () const {return x;}
	void setX (const double& x_){x = x_;}
	const double& getY () const {return y;}
	void setY (const double& y_){y = y_;}
	const double& getZ () const {return z;}
	void setZ (const double& z_){z = z_;}
	const unsigned long& getPrimaryKey () const {return auto_id;}
	const unsigned long& getAuto_id () const {return auto_id;}

//Data Fields
private:
	friend class odb::access;
	#pragma db id auto
	unsigned long auto_id;
	shared_ptr<Link> link;
	int points;
	double x;
	double y;
	double z;

};

#pragma db object
class Link
{
public:
	// Default Constructor
	Link () {}	
	//Contructor
	Link ( int link_, std::string name_, shared_ptr<Node> node_a_, shared_ptr<Node> node_b_, double length_, double setback_a_, double setback_b_, int bearing_a_, int bearing_b_, std::string type_, int divided_, int area_type_, int use_, double grade_, int lanes_ab_, double speed_ab_, double fspd_ab_, int cap_ab_, int lanes_ba_, double speed_ba_, double fspd_ba_, int cap_ba_, int left_ab_, int right_ab_, int left_ba_, int right_ba_ )  
	: link (link_), name (name_), node_a (node_a_), node_b (node_b_), length (length_), setback_a (setback_a_), setback_b (setback_b_), bearing_a (bearing_a_), bearing_b (bearing_b_), type (type_), divided (divided_), area_type (area_type_), use (use_), grade (grade_), lanes_ab (lanes_ab_), speed_ab (speed_ab_), fspd_ab (fspd_ab_), cap_ab (cap_ab_), lanes_ba (lanes_ba_), speed_ba (speed_ba_), fspd_ba (fspd_ba_), cap_ba (cap_ba_), left_ab (left_ab_), right_ab (right_ab_), left_ba (left_ba_), right_ba (right_ba_)
	{
	}
	//Accessors
	const int& getLink () const {return link;}
	void setLink (const int& link_){link = link_;}
	const std::string& getName () const {return name;}
	void setName (const std::string& name_){name = name_;}
	const shared_ptr<Node>& getNode_A () const {return node_a;}
	void setNode_A (const shared_ptr<Node>& node_a_){node_a = node_a_;}
	void setNode_A (const int& node_a_, InputContainer& container){node_a = container.Nodes[node_a_];}
	const shared_ptr<Node>& getNode_B () const {return node_b;}
	void setNode_B (const shared_ptr<Node>& node_b_){node_b = node_b_;}
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
	const std::string& getType () const {return type;}
	void setType (const std::string& type_){type = type_;}
	const int& getDivided () const {return divided;}
	void setDivided (const int& divided_){divided = divided_;}
	const int& getArea_Type () const {return area_type;}
	void setArea_Type (const int& area_type_){area_type = area_type_;}
	const int& getUse () const {return use;}
	void setUse (const int& use_){use = use_;}
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
	std::string name;
	shared_ptr<Node> node_a;
	shared_ptr<Node> node_b;
	double length;
	double setback_a;
	double setback_b;
	int bearing_a;
	int bearing_b;
	std::string type;
	int divided;
	int area_type;
	int use;
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

#pragma db object
class Pocket
{
public:
	// Default Constructor
	Pocket () {}	
	//Contructor
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
	const unsigned long& getPrimaryKey () const {return auto_id;}
	const unsigned long& getAuto_id () const {return auto_id;}

//Data Fields
private:
	friend class odb::access;
	#pragma db id auto
	unsigned long auto_id;
	shared_ptr<Link> link;
	int dir;
	std::string type;
	int lanes;
	double length;
	double offset;

};

#pragma db object
class Lane_Use
{
public:
	// Default Constructor
	Lane_Use () {}	
	//Contructor
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
	const unsigned long& getPrimaryKey () const {return auto_id;}
	const unsigned long& getAuto_id () const {return auto_id;}

//Data Fields
private:
	friend class odb::access;
	#pragma db id auto
	unsigned long auto_id;
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

#pragma db object
class Connect
{
public:
	// Default Constructor
	Connect () {}	
	//Contructor
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
	const unsigned long& getPrimaryKey () const {return auto_id;}
	const unsigned long& getAuto_id () const {return auto_id;}

//Data Fields
private:
	friend class odb::access;
	#pragma db id auto
	unsigned long auto_id;
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

#pragma db object
class Turn_Pen
{
public:
	// Default Constructor
	Turn_Pen () {}	
	//Contructor
	Turn_Pen ( shared_ptr<Link> link_, int dir_, shared_ptr<Link> to_link_, double start_, double end_, int use_, int min_type_, int max_type_, int penalty_, shared_ptr<Node> in_node_, shared_ptr<Node> out_node_ )  
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
	const shared_ptr<Node>& getIn_Node () const {return in_node;}
	void setIn_Node (const shared_ptr<Node>& in_node_){in_node = in_node_;}
	void setIn_Node (const int& in_node_, InputContainer& container){in_node = container.Nodes[in_node_];}
	const shared_ptr<Node>& getOut_Node () const {return out_node;}
	void setOut_Node (const shared_ptr<Node>& out_node_){out_node = out_node_;}
	void setOut_Node (const int& out_node_, InputContainer& container){out_node = container.Nodes[out_node_];}
	const unsigned long& getPrimaryKey () const {return auto_id;}
	const unsigned long& getAuto_id () const {return auto_id;}

//Data Fields
private:
	friend class odb::access;
	#pragma db id auto
	unsigned long auto_id;
	shared_ptr<Link> link;
	int dir;
	shared_ptr<Link> to_link;
	double start;
	double end;
	int use;
	int min_type;
	int max_type;
	int penalty;
	shared_ptr<Node> in_node;
	shared_ptr<Node> out_node;

};

#pragma db object
class Parking
{
public:
	// Default Constructor
	Parking () {}	
	//Contructor
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

};

#pragma db object
class Location
{
public:
	// Default Constructor
	Location () {}
	//Contructor
	Location ( int location_, shared_ptr<Link> link_, int dir_, double offset_, double setback_, shared_ptr<Zone> zone_ )  
	: location (location_), link (link_), dir (dir_), offset (offset_), setback (setback_), zone (zone_)
	{
	}
	//Accessors
	const int& getLocation () const {return location;}
	void setLocation (const int& location_){location = location_;}
	const shared_ptr<Link>& getLink () const {return link;}
	void setLink (const shared_ptr<Link>& link_){link = link_;}
	void setLink (const int& link_, InputContainer& container){link = container.Links[link_];}
	const int& getDir () const {return dir;}
	void setDir (const int& dir_){dir = dir_;}
	const double& getOffset () const {return offset;}
	void setOffset (const double& offset_){offset = offset_;}
	const double& getSetback () const {return setback;}
	void setSetback (const double& setback_){setback = setback_;}
	const shared_ptr<Zone>& getZone () const {return zone;}
	void setZone (const shared_ptr<Zone>& zone_){zone = zone_;}
	void setZone (const int& zone_, InputContainer& container){zone = container.Zones[zone_];}
	const int& getPrimaryKey () const {return location;}

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
};

#pragma db object
class Access
{
public:
	// Default Constructor
	Access () {}	
	//Contructor
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
	const unsigned long& getPrimaryKey () const {return auto_id;}
	const unsigned long& getAuto_id () const {return auto_id;}

//Data Fields
private:
	friend class odb::access;
	#pragma db id auto
	unsigned long auto_id;
	shared_ptr<Link> link;
	int from_id;
	int from_type;
	int to_id;
	int to_type;
	int dir;
	double time;
	int cost;

};

#pragma db object
class Sign
{
public:
	// Default Constructor
	Sign () {}	
	//Contructor
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
	const unsigned long& getPrimaryKey () const {return auto_id;}
	const unsigned long& getAuto_id () const {return auto_id;}

//Data Fields
private:
	friend class odb::access;
	#pragma db id auto
	unsigned long auto_id;
	shared_ptr<Link> link;
	int dir;
	std::string sign;

};

#pragma db object
class Signal
{
public:
	// Default Constructor
	Signal () {}	
	//Contructor
	Signal ( int signal_, int group_, int times_, double start_, double end_, int timing_, int phasing_, std::string type_, int offset_ )  
	: signal (signal_), group (group_), times (times_), start (start_), end (end_), timing (timing_), phasing (phasing_), type (type_), offset (offset_)
	{
	}
	//Accessors
	const int& getSignal () const {return signal;}
	void setSignal (const int& signal_){signal = signal_;}
	const int& getGroup () const {return group;}
	void setGroup (const int& group_){group = group_;}
	const int& getTimes () const {return times;}
	void setTimes (const int& times_){times = times_;}
	const double& getStart () const {return start;}
	void setStart (const double& start_){start = start_;}
	const double& getEnd () const {return end;}
	void setEnd (const double& end_){end = end_;}
	const int& getTiming () const {return timing;}
	void setTiming (const int& timing_){timing = timing_;}
	const int& getPhasing () const {return phasing;}
	void setPhasing (const int& phasing_){phasing = phasing_;}
	const std::string& getType () const {return type;}
	void setType (const std::string& type_){type = type_;}
	const int& getOffset () const {return offset;}
	void setOffset (const int& offset_){offset = offset_;}
	const int& getPrimaryKey () const {return signal;}

//Data Fields
private:
	friend class odb::access;
	#pragma db id
	int signal;
	int group;
	int times;
	double start;
	double end;
	int timing;
	int phasing;
	std::string type;
	int offset;

};

#pragma db object
class Timing
{
public:
	// Default Constructor
	Timing () {}	
	//Contructor
	Timing ( shared_ptr<Signal> signal_, int timing_, int type_, int cycle_, int offset_, int phases_, int phase_, int barrier_, int ring_, int position_, int minimum_, int maximum_, int extend_, int yellow_, int red_ )  
	: signal (signal_), timing (timing_), type (type_), cycle (cycle_), offset (offset_), phases (phases_), phase (phase_), barrier (barrier_), ring (ring_), position (position_), minimum (minimum_), maximum (maximum_), extend (extend_), yellow (yellow_), red (red_)
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
	const int& getPhase () const {return phase;}
	void setPhase (const int& phase_){phase = phase_;}
	const int& getBarrier () const {return barrier;}
	void setBarrier (const int& barrier_){barrier = barrier_;}
	const int& getRing () const {return ring;}
	void setRing (const int& ring_){ring = ring_;}
	const int& getPosition () const {return position;}
	void setPosition (const int& position_){position = position_;}
	const int& getMinimum () const {return minimum;}
	void setMinimum (const int& minimum_){minimum = minimum_;}
	const int& getMaximum () const {return maximum;}
	void setMaximum (const int& maximum_){maximum = maximum_;}
	const int& getExtend () const {return extend;}
	void setExtend (const int& extend_){extend = extend_;}
	const int& getYellow () const {return yellow;}
	void setYellow (const int& yellow_){yellow = yellow_;}
	const int& getRed () const {return red;}
	void setRed (const int& red_){red = red_;}
	const unsigned long& getPrimaryKey () const {return auto_id;}
	const unsigned long& getAuto_id () const {return auto_id;}

//Data Fields
private:
	friend class odb::access;
	#pragma db id auto
	unsigned long auto_id;
	shared_ptr<Signal> signal;
	int timing;
	int type;
	int cycle;
	int offset;
	int phases;
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

#pragma db object
class Phasing
{
public:
	// Default Constructor
	Phasing () {}	
	//Contructor
	Phasing ( shared_ptr<Signal> signal_, int phasing_, int phase_, std::string detectors_, int movements_, std::string movement_, shared_ptr<Link> link_, int dir_, shared_ptr<Link> to_link_, int protect_ )  
	: signal (signal_), phasing (phasing_), phase (phase_), detectors (detectors_), movements (movements_), movement (movement_), link (link_), dir (dir_), to_link (to_link_), protect (protect_)
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
	const std::string& getMovement () const {return movement;}
	void setMovement (const std::string& movement_){movement = movement_;}
	const shared_ptr<Link>& getLink () const {return link;}
	void setLink (const shared_ptr<Link>& link_){link = link_;}
	void setLink (const int& link_, InputContainer& container){link = container.Links[link_];}
	const int& getDir () const {return dir;}
	void setDir (const int& dir_){dir = dir_;}
	const shared_ptr<Link>& getTo_Link () const {return to_link;}
	void setTo_Link (const shared_ptr<Link>& to_link_){to_link = to_link_;}
	void setTo_Link (const int& to_link_, InputContainer& container){to_link = container.Links[to_link_];}
	const int& getProtect () const {return protect;}
	void setProtect (const int& protect_){protect = protect_;}
	const unsigned long& getPrimaryKey () const {return auto_id;}
	const unsigned long& getAuto_id () const {return auto_id;}

//Data Fields
private:
	friend class odb::access;
	#pragma db id auto
	unsigned long auto_id;
	shared_ptr<Signal> signal;
	int phasing;
	int phase;
	std::string detectors;
	int movements;
	std::string movement;
	shared_ptr<Link> link;
	int dir;
	shared_ptr<Link> to_link;
	int protect;

};

#pragma db object
class Detector
{
public:
	// Default Constructor
	Detector () {}	
	//Contructor
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

};

#pragma db object
class Stop
{
public:
	// Default Constructor
	Stop () {}	
	//Contructor
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

};

#pragma db object
class Fare
{
public:
	// Default Constructor
	Fare () {}	
	//Contructor
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

};

#pragma db object
class Line
{
public:
	// Default Constructor
	Line () {}	
	//Contructor
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
	const unsigned long& getPrimaryKey () const {return auto_id;}
	const unsigned long& getAuto_id () const {return auto_id;}

//Data Fields
private:
	friend class odb::access;
	#pragma db id auto
	unsigned long auto_id;
	int route;
	shared_ptr<Stop> stops;
	int mode;
	int type;
	std::string name;
	shared_ptr<Stop> stop;
	shared_ptr<Zone> zone;
	int flag;

};

#pragma db object
class Schedule
{
public:
	// Default Constructor
	Schedule () {}	
	//Contructor
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
	const unsigned long& getPrimaryKey () const {return auto_id;}
	const unsigned long& getAuto_id () const {return auto_id;}

//Data Fields
private:
	friend class odb::access;
	#pragma db id auto
	unsigned long auto_id;
	int route;
	shared_ptr<Stop> stops;
	shared_ptr<Stop> stop;

};

#pragma db object
class Driver
{
public:
	// Default Constructor
	Driver () {}	
	//Contructor
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
	const unsigned long& getPrimaryKey () const {return auto_id;}
	const unsigned long& getAuto_id () const {return auto_id;}

//Data Fields
private:
	friend class odb::access;
	#pragma db id auto
	unsigned long auto_id;
	int route;
	shared_ptr<Link> links;
	int type;
	int subtype;
	shared_ptr<Link> link;
	int dir;

};

#pragma db object
class Route_Nodes
{
public:
	// Default Constructor
	Route_Nodes () {}	
	//Contructor
	Route_Nodes ( int route_, int mode_, shared_ptr<Veh_Type> veh_type_, shared_ptr<Node> nodes_, std::string name_, shared_ptr<Node> node_, int type_, double dwell_, double time_, double speed_ )  
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
	const shared_ptr<Node>& getNodes () const {return nodes;}
	void setNodes (const shared_ptr<Node>& nodes_){nodes = nodes_;}
	void setNodes (const int& nodes_, InputContainer& container){nodes = container.Nodes[nodes_];}
	const std::string& getName () const {return name;}
	void setName (const std::string& name_){name = name_;}
	const shared_ptr<Node>& getNode () const {return node;}
	void setNode (const shared_ptr<Node>& node_){node = node_;}
	void setNode (const int& node_, InputContainer& container){node = container.Nodes[node_];}
	const int& getType () const {return type;}
	void setType (const int& type_){type = type_;}
	const double& getDwell () const {return dwell;}
	void setDwell (const double& dwell_){dwell = dwell_;}
	const double& getTime () const {return time;}
	void setTime (const double& time_){time = time_;}
	const double& getSpeed () const {return speed;}
	void setSpeed (const double& speed_){speed = speed_;}
	const unsigned long& getPrimaryKey () const {return auto_id;}
	const unsigned long& getAuto_id () const {return auto_id;}

//Data Fields
private:
	friend class odb::access;
	#pragma db id auto
	unsigned long auto_id;
	int route;
	int mode;
	shared_ptr<Veh_Type> veh_type;
	shared_ptr<Node> nodes;
	std::string name;
	shared_ptr<Node> node;
	int type;
	double dwell;
	double time;
	double speed;

};

#pragma db object
class Selection
{
public:
	// Default Constructor
	Selection () {}	
	//Contructor
	Selection ( int hhold_, int person_, int tour_, int trip_, int type_, int partition_ )  
	: hhold (hhold_), person (person_), tour (tour_), trip (trip_), type (type_), partition (partition_)
	{
	}
	//Accessors
	const int& getHhold () const {return hhold;}
	void setHhold (const int& hhold_){hhold = hhold_;}
	const int& getPerson () const {return person;}
	void setPerson (const int& person_){person = person_;}
	const int& getTour () const {return tour;}
	void setTour (const int& tour_){tour = tour_;}
	const int& getTrip () const {return trip;}
	void setTrip (const int& trip_){trip = trip_;}
	const int& getType () const {return type;}
	void setType (const int& type_){type = type_;}
	const int& getPartition () const {return partition;}
	void setPartition (const int& partition_){partition = partition_;}
	const unsigned long& getPrimaryKey () const {return auto_id;}
	const unsigned long& getAuto_id () const {return auto_id;}

//Data Fields
private:
	friend class odb::access;
	#pragma db id auto
	unsigned long auto_id;
	int hhold;
	int person;
	int tour;
	int trip;
	int type;
	int partition;

};

#pragma db object
class Household
{
public:
	// Default Constructor
	Household () {}	
	//Contructor
	Household ( int hhold_, shared_ptr<Location> location_, int persons_, int workers_, int vehicles_, int type_, int partition_, int person_, int age_, int relate_, int gender_, int work_, int drive_ )  
	: hhold (hhold_), location (location_), persons (persons_), workers (workers_), vehicles (vehicles_), type (type_), partition (partition_), person (person_), age (age_), relate (relate_), gender (gender_), work (work_), drive (drive_)
	{
	}
	//Accessors
	const int& getHhold () const {return hhold;}
	void setHhold (const int& hhold_){hhold = hhold_;}
	const shared_ptr<Location>& getLocation () const {return location;}
	void setLocation (const shared_ptr<Location>& location_){location = location_;}
	void setLocation (const int& location_, InputContainer& container){location = container.Locations[location_];}
	const int& getPersons () const {return persons;}
	void setPersons (const int& persons_){persons = persons_;}
	const int& getWorkers () const {return workers;}
	void setWorkers (const int& workers_){workers = workers_;}
	const int& getVehicles () const {return vehicles;}
	void setVehicles (const int& vehicles_){vehicles = vehicles_;}
	const int& getType () const {return type;}
	void setType (const int& type_){type = type_;}
	const int& getPartition () const {return partition;}
	void setPartition (const int& partition_){partition = partition_;}
	const int& getPerson () const {return person;}
	void setPerson (const int& person_){person = person_;}
	const int& getAge () const {return age;}
	void setAge (const int& age_){age = age_;}
	const int& getRelate () const {return relate;}
	void setRelate (const int& relate_){relate = relate_;}
	const int& getGender () const {return gender;}
	void setGender (const int& gender_){gender = gender_;}
	const int& getWork () const {return work;}
	void setWork (const int& work_){work = work_;}
	const int& getDrive () const {return drive;}
	void setDrive (const int& drive_){drive = drive_;}
	const unsigned long& getPrimaryKey () const {return auto_id;}
	const unsigned long& getAuto_id () const {return auto_id;}

//Data Fields
private:
	friend class odb::access;
	#pragma db id auto
	unsigned long auto_id;
	int hhold;
	shared_ptr<Location> location;
	int persons;
	int workers;
	int vehicles;
	int type;
	int partition;
	int person;
	int age;
	int relate;
	int gender;
	int work;
	int drive;

};

#pragma db object
class Link_Delay
{
public:
	// Default Constructor
	Link_Delay () {}	
	//Contructor
	Link_Delay ( shared_ptr<Link> link_, int dir_, int type_, double start_, double end_, double flow_, double time_, shared_ptr<Link> out_link_, double out_flow_, double out_time_ )  
	: link (link_), dir (dir_), type (type_), start (start_), end (end_), flow (flow_), time (time_), out_link (out_link_), out_flow (out_flow_), out_time (out_time_)
	{
	}
	//Accessors
	const shared_ptr<Link>& getLink () const {return link;}
	void setLink (const shared_ptr<Link>& link_){link = link_;}
	void setLink (const int& link_, InputContainer& container){link = container.Links[link_];}
	const int& getDir () const {return dir;}
	void setDir (const int& dir_){dir = dir_;}
	const int& getType () const {return type;}
	void setType (const int& type_){type = type_;}
	const double& getStart () const {return start;}
	void setStart (const double& start_){start = start_;}
	const double& getEnd () const {return end;}
	void setEnd (const double& end_){end = end_;}
	const double& getFlow () const {return flow;}
	void setFlow (const double& flow_){flow = flow_;}
	const double& getTime () const {return time;}
	void setTime (const double& time_){time = time_;}
	const shared_ptr<Link>& getOut_Link () const {return out_link;}
	void setOut_Link (const shared_ptr<Link>& out_link_){out_link = out_link_;}
	void setOut_Link (const int& out_link_, InputContainer& container){out_link = container.Links[out_link_];}
	const double& getOut_Flow () const {return out_flow;}
	void setOut_Flow (const double& out_flow_){out_flow = out_flow_;}
	const double& getOut_Time () const {return out_time;}
	void setOut_Time (const double& out_time_){out_time = out_time_;}
	const unsigned long& getPrimaryKey () const {return auto_id;}
	const unsigned long& getAuto_id () const {return auto_id;}

//Data Fields
private:
	friend class odb::access;
	#pragma db id auto
	unsigned long auto_id;
	shared_ptr<Link> link;
	int dir;
	int type;
	double start;
	double end;
	double flow;
	double time;
	shared_ptr<Link> out_link;
	double out_flow;
	double out_time;

};

#pragma db object
class Performance
{
public:
	// Default Constructor
	Performance () {}	
	//Contructor
	Performance ( double speed_, double delay_, double density_, double max_den_, double ratio_, double queue_, int max_que_, int fail_ )  
	: speed (speed_), delay (delay_), density (density_), max_den (max_den_), ratio (ratio_), queue (queue_), max_que (max_que_), fail (fail_)
	{
	}
	//Accessors
	const double& getSpeed () const {return speed;}
	void setSpeed (const double& speed_){speed = speed_;}
	const double& getDelay () const {return delay;}
	void setDelay (const double& delay_){delay = delay_;}
	const double& getDensity () const {return density;}
	void setDensity (const double& density_){density = density_;}
	const double& getMax_Den () const {return max_den;}
	void setMax_Den (const double& max_den_){max_den = max_den_;}
	const double& getRatio () const {return ratio;}
	void setRatio (const double& ratio_){ratio = ratio_;}
	const double& getQueue () const {return queue;}
	void setQueue (const double& queue_){queue = queue_;}
	const int& getMax_Que () const {return max_que;}
	void setMax_Que (const int& max_que_){max_que = max_que_;}
	const int& getFail () const {return fail;}
	void setFail (const int& fail_){fail = fail_;}
	const unsigned long& getPrimaryKey () const {return auto_id;}
	const unsigned long& getAuto_id () const {return auto_id;}

//Data Fields
private:
	friend class odb::access;
	#pragma db id auto
	unsigned long auto_id;
	double speed;
	double delay;
	double density;
	double max_den;
	double ratio;
	double queue;
	int max_que;
	int fail;

};

#pragma db object
class Ridership
{
public:
	// Default Constructor
	Ridership () {}	
	//Contructor
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
	const unsigned long& getPrimaryKey () const {return auto_id;}
	const unsigned long& getAuto_id () const {return auto_id;}

//Data Fields
private:
	friend class odb::access;
	#pragma db id auto
	unsigned long auto_id;
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

};

#pragma db object
class Veh_Type
{
public:
	// Default Constructor
	Veh_Type () {}	
	//Contructor
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

};

#pragma db object
class Vehicle
{
public:
	// Default Constructor
	Vehicle () {}	
	//Contructor
	Vehicle ( int hhold_, int vehicle_, shared_ptr<Parking> parking_, shared_ptr<Veh_Type> type_, int subtype_, int partition_ )  
	: hhold (hhold_), vehicle (vehicle_), parking (parking_), type (type_), subtype (subtype_), partition (partition_)
	{
	}
	//Accessors
	const int& getHhold () const {return hhold;}
	void setHhold (const int& hhold_){hhold = hhold_;}
	const int& getVehicle () const {return vehicle;}
	void setVehicle (const int& vehicle_){vehicle = vehicle_;}
	const shared_ptr<Parking>& getParking () const {return parking;}
	void setParking (const shared_ptr<Parking>& parking_){parking = parking_;}
	void setParking (const int& parking_, InputContainer& container){parking = container.Parkings[parking_];}
	const shared_ptr<Veh_Type>& getType () const {return type;}
	void setType (const shared_ptr<Veh_Type>& type_){type = type_;}
	void setType (const int& type_, InputContainer& container){type = container.Veh_Types[type_];}
	const int& getSubtype () const {return subtype;}
	void setSubtype (const int& subtype_){subtype = subtype_;}
	const int& getPartition () const {return partition;}
	void setPartition (const int& partition_){partition = partition_;}
	const unsigned long& getPrimaryKey () const {return auto_id;}
	const unsigned long& getAuto_id () const {return auto_id;}

//Data Fields
private:
	friend class odb::access;
	#pragma db id auto
	unsigned long auto_id;
	int hhold;
	int vehicle;
	shared_ptr<Parking> parking;
	shared_ptr<Veh_Type> type;
	int subtype;
	int partition;

};

#pragma db object
class Trip
{
public:
	// Default Constructor
	Trip () {}	
	//Contructor
	Trip ( int hhold_, int person_, int tour_, int trip_, double start_, double end_, double duration_, shared_ptr<Location> origin_, shared_ptr<Location> destination_, int purpose_, int mode_, int constraint_, int priority_, int vehicle_, int passengers_, int type_, int partition_ )  
	: hhold (hhold_), person (person_), tour (tour_), trip (trip_), start (start_), end (end_), duration (duration_), origin (origin_), destination (destination_), purpose (purpose_), mode (mode_), constraint (constraint_), priority (priority_), vehicle (vehicle_), passengers (passengers_), type (type_), partition (partition_)
	{
	}
	//Accessors
	const int& getHhold () const {return hhold;}
	void setHhold (const int& hhold_){hhold = hhold_;}
	const int& getPerson () const {return person;}
	void setPerson (const int& person_){person = person_;}
	const int& getTour () const {return tour;}
	void setTour (const int& tour_){tour = tour_;}
	const int& getTrip () const {return trip;}
	void setTrip (const int& trip_){trip = trip_;}
	const double& getStart () const {return start;}
	void setStart (const double& start_){start = start_;}
	const double& getEnd () const {return end;}
	void setEnd (const double& end_){end = end_;}
	const double& getDuration () const {return duration;}
	void setDuration (const double& duration_){duration = duration_;}
	const shared_ptr<Location>& getOrigin () const {return origin;}
	void setOrigin (const shared_ptr<Location>& origin_){origin = origin_;}
	void setOrigin (const int& origin_, InputContainer& container){origin = container.Locations[origin_];}
	const shared_ptr<Location>& getDestination () const {return destination;}
	void setDestination (const shared_ptr<Location>& destination_){destination = destination_;}
	void setDestination (const int& destination_, InputContainer& container){destination = container.Locations[destination_];}
	const int& getPurpose () const {return purpose;}
	void setPurpose (const int& purpose_){purpose = purpose_;}
	const int& getMode () const {return mode;}
	void setMode (const int& mode_){mode = mode_;}
	const int& getConstraint () const {return constraint;}
	void setConstraint (const int& constraint_){constraint = constraint_;}
	const int& getPriority () const {return priority;}
	void setPriority (const int& priority_){priority = priority_;}
	const int& getVehicle () const {return vehicle;}
	void setVehicle (const int& vehicle_){vehicle = vehicle_;}
	const int& getPassengers () const {return passengers;}
	void setPassengers (const int& passengers_){passengers = passengers_;}
	const int& getType () const {return type;}
	void setType (const int& type_){type = type_;}
	const int& getPartition () const {return partition;}
	void setPartition (const int& partition_){partition = partition_;}
	const unsigned long& getPrimaryKey () const {return auto_id;}
	const unsigned long& getAuto_id () const {return auto_id;}

//Data Fields
private:
	friend class odb::access;
	#pragma db id auto
	unsigned long auto_id;
	int hhold;
	int person;
	int tour;
	int trip;
	double start;
	double end;
	double duration;
	shared_ptr<Location> origin;
	shared_ptr<Location> destination;
	int purpose;
	int mode;
	int constraint;
	int priority;
	int vehicle;
	int passengers;
	int type;
	int partition;

};

#pragma db object
class Problem
{
public:
	// Default Constructor
	Problem () {}	
	//Contructor
	Problem ( int problem_, double time_, shared_ptr<Link> link_, int dir_, int lane_, double offset_, int route_, int survey_ )  
	: problem (problem_), time (time_), link (link_), dir (dir_), lane (lane_), offset (offset_), route (route_), survey (survey_)
	{
	}
	//Accessors
	const int& getProblem () const {return problem;}
	void setProblem (const int& problem_){problem = problem_;}
	const double& getTime () const {return time;}
	void setTime (const double& time_){time = time_;}
	const shared_ptr<Link>& getLink () const {return link;}
	void setLink (const shared_ptr<Link>& link_){link = link_;}
	void setLink (const int& link_, InputContainer& container){link = container.Links[link_];}
	const int& getDir () const {return dir;}
	void setDir (const int& dir_){dir = dir_;}
	const int& getLane () const {return lane;}
	void setLane (const int& lane_){lane = lane_;}
	const double& getOffset () const {return offset;}
	void setOffset (const double& offset_){offset = offset_;}
	const int& getRoute () const {return route;}
	void setRoute (const int& route_){route = route_;}
	const int& getSurvey () const {return survey;}
	void setSurvey (const int& survey_){survey = survey_;}
	const int& getPrimaryKey () const {return problem;}

//Data Fields
private:
	friend class odb::access;
	#pragma db id
	int problem;
	double time;
	shared_ptr<Link> link;
	int dir;
	int lane;
	double offset;
	int route;
	int survey;

};

#pragma db object
class Plan
{
public:
	// Default Constructor
	Plan () {}	
	//Contructor
	Plan ( double depart_, double arrive_, double activity_, double walk_, double drive_, double transit_, double wait_, double other_, double length_, double cost_, int impedance_, int leg_mode_, int leg_type_, int leg_id_, double leg_time_, double leg_length_, double leg_cost_, int leg_imp_ )  
	: depart (depart_), arrive (arrive_), activity (activity_), walk (walk_), drive (drive_), transit (transit_), wait (wait_), other (other_), length (length_), cost (cost_), impedance (impedance_), leg_mode (leg_mode_), leg_type (leg_type_), leg_id (leg_id_), leg_time (leg_time_), leg_length (leg_length_), leg_cost (leg_cost_), leg_imp (leg_imp_)
	{
	}
	//Accessors
	const double& getDepart () const {return depart;}
	void setDepart (const double& depart_){depart = depart_;}
	const double& getArrive () const {return arrive;}
	void setArrive (const double& arrive_){arrive = arrive_;}
	const double& getActivity () const {return activity;}
	void setActivity (const double& activity_){activity = activity_;}
	const double& getWalk () const {return walk;}
	void setWalk (const double& walk_){walk = walk_;}
	const double& getDrive () const {return drive;}
	void setDrive (const double& drive_){drive = drive_;}
	const double& getTransit () const {return transit;}
	void setTransit (const double& transit_){transit = transit_;}
	const double& getWait () const {return wait;}
	void setWait (const double& wait_){wait = wait_;}
	const double& getOther () const {return other;}
	void setOther (const double& other_){other = other_;}
	const double& getLength () const {return length;}
	void setLength (const double& length_){length = length_;}
	const double& getCost () const {return cost;}
	void setCost (const double& cost_){cost = cost_;}
	const int& getImpedance () const {return impedance;}
	void setImpedance (const int& impedance_){impedance = impedance_;}
	const int& getLeg_Mode () const {return leg_mode;}
	void setLeg_Mode (const int& leg_mode_){leg_mode = leg_mode_;}
	const int& getLeg_Type () const {return leg_type;}
	void setLeg_Type (const int& leg_type_){leg_type = leg_type_;}
	const int& getLeg_Id () const {return leg_id;}
	void setLeg_Id (const int& leg_id_){leg_id = leg_id_;}
	const double& getLeg_Time () const {return leg_time;}
	void setLeg_Time (const double& leg_time_){leg_time = leg_time_;}
	const double& getLeg_Length () const {return leg_length;}
	void setLeg_Length (const double& leg_length_){leg_length = leg_length_;}
	const double& getLeg_Cost () const {return leg_cost;}
	void setLeg_Cost (const double& leg_cost_){leg_cost = leg_cost_;}
	const int& getLeg_Imp () const {return leg_imp;}
	void setLeg_Imp (const int& leg_imp_){leg_imp = leg_imp_;}
	const unsigned long& getPrimaryKey () const {return auto_id;}
	const unsigned long& getAuto_id () const {return auto_id;}

//Data Fields
private:
	friend class odb::access;
	#pragma db id auto
	unsigned long auto_id;
	double depart;
	double arrive;
	double activity;
	double walk;
	double drive;
	double transit;
	double wait;
	double other;
	double length;
	double cost;
	int impedance;
	int leg_mode;
	int leg_type;
	int leg_id;
	double leg_time;
	double leg_length;
	double leg_cost;
	int leg_imp;

};

#pragma db object
class Skim
{
public:
	// Default Constructor
	Skim () {}	
	//Contructor
	Skim ( double time_, double walk_, double drive_, double transit_, double wait_, double other_, double length_, double cost_, int impedance_ )  
	: time (time_), walk (walk_), drive (drive_), transit (transit_), wait (wait_), other (other_), length (length_), cost (cost_), impedance (impedance_)
	{
	}
	//Accessors
	const double& getTime () const {return time;}
	void setTime (const double& time_){time = time_;}
	const double& getWalk () const {return walk;}
	void setWalk (const double& walk_){walk = walk_;}
	const double& getDrive () const {return drive;}
	void setDrive (const double& drive_){drive = drive_;}
	const double& getTransit () const {return transit;}
	void setTransit (const double& transit_){transit = transit_;}
	const double& getWait () const {return wait;}
	void setWait (const double& wait_){wait = wait_;}
	const double& getOther () const {return other;}
	void setOther (const double& other_){other = other_;}
	const double& getLength () const {return length;}
	void setLength (const double& length_){length = length_;}
	const double& getCost () const {return cost;}
	void setCost (const double& cost_){cost = cost_;}
	const int& getImpedance () const {return impedance;}
	void setImpedance (const int& impedance_){impedance = impedance_;}
	const unsigned long& getPrimaryKey () const {return auto_id;}
	const unsigned long& getAuto_id () const {return auto_id;}

//Data Fields
private:
	friend class odb::access;
	#pragma db id auto
	unsigned long auto_id;
	double time;
	double walk;
	double drive;
	double transit;
	double wait;
	double other;
	double length;
	double cost;
	int impedance;

};

#pragma db object
class Event
{
public:
	// Default Constructor
	Event () {}	
	//Contructor
	Event ( int hhold_, int person_, int tour_, int trip_, int mode_, int type_, double schedule_, double actual_, shared_ptr<Link> link_, int dir_, int lane_, double offset_, int route_ )  
	: hhold (hhold_), person (person_), tour (tour_), trip (trip_), mode (mode_), type (type_), schedule (schedule_), actual (actual_), link (link_), dir (dir_), lane (lane_), offset (offset_), route (route_)
	{
	}
	//Accessors
	const int& getHhold () const {return hhold;}
	void setHhold (const int& hhold_){hhold = hhold_;}
	const int& getPerson () const {return person;}
	void setPerson (const int& person_){person = person_;}
	const int& getTour () const {return tour;}
	void setTour (const int& tour_){tour = tour_;}
	const int& getTrip () const {return trip;}
	void setTrip (const int& trip_){trip = trip_;}
	const int& getMode () const {return mode;}
	void setMode (const int& mode_){mode = mode_;}
	const int& getType () const {return type;}
	void setType (const int& type_){type = type_;}
	const double& getSchedule () const {return schedule;}
	void setSchedule (const double& schedule_){schedule = schedule_;}
	const double& getActual () const {return actual;}
	void setActual (const double& actual_){actual = actual_;}
	const shared_ptr<Link>& getLink () const {return link;}
	void setLink (const shared_ptr<Link>& link_){link = link_;}
	void setLink (const int& link_, InputContainer& container){link = container.Links[link_];}
	const int& getDir () const {return dir;}
	void setDir (const int& dir_){dir = dir_;}
	const int& getLane () const {return lane;}
	void setLane (const int& lane_){lane = lane_;}
	const double& getOffset () const {return offset;}
	void setOffset (const double& offset_){offset = offset_;}
	const int& getRoute () const {return route;}
	void setRoute (const int& route_){route = route_;}
	const unsigned long& getPrimaryKey () const {return auto_id;}
	const unsigned long& getAuto_id () const {return auto_id;}

//Data Fields
private:
	friend class odb::access;
	#pragma db id auto
	unsigned long auto_id;
	int hhold;
	int person;
	int tour;
	int trip;
	int mode;
	int type;
	double schedule;
	double actual;
	shared_ptr<Link> link;
	int dir;
	int lane;
	double offset;
	int route;

};

#pragma db object
class Traveler
{
public:
	// Default Constructor
	Traveler () {}	
	//Contructor
	Traveler ( int hhold_, int person_, int tour_, int trip_, int mode_, double time_, double distance_, double speed_, shared_ptr<Link> link_, int dir_, int lane_, double offset_, int route_ )  
	: hhold (hhold_), person (person_), tour (tour_), trip (trip_), mode (mode_), time (time_), distance (distance_), speed (speed_), link (link_), dir (dir_), lane (lane_), offset (offset_), route (route_)
	{
	}
	//Accessors
	const int& getHhold () const {return hhold;}
	void setHhold (const int& hhold_){hhold = hhold_;}
	const int& getPerson () const {return person;}
	void setPerson (const int& person_){person = person_;}
	const int& getTour () const {return tour;}
	void setTour (const int& tour_){tour = tour_;}
	const int& getTrip () const {return trip;}
	void setTrip (const int& trip_){trip = trip_;}
	const int& getMode () const {return mode;}
	void setMode (const int& mode_){mode = mode_;}
	const double& getTime () const {return time;}
	void setTime (const double& time_){time = time_;}
	const double& getDistance () const {return distance;}
	void setDistance (const double& distance_){distance = distance_;}
	const double& getSpeed () const {return speed;}
	void setSpeed (const double& speed_){speed = speed_;}
	const shared_ptr<Link>& getLink () const {return link;}
	void setLink (const shared_ptr<Link>& link_){link = link_;}
	void setLink (const int& link_, InputContainer& container){link = container.Links[link_];}
	const int& getDir () const {return dir;}
	void setDir (const int& dir_){dir = dir_;}
	const int& getLane () const {return lane;}
	void setLane (const int& lane_){lane = lane_;}
	const double& getOffset () const {return offset;}
	void setOffset (const double& offset_){offset = offset_;}
	const int& getRoute () const {return route;}
	void setRoute (const int& route_){route = route_;}
	const unsigned long& getPrimaryKey () const {return auto_id;}
	const unsigned long& getAuto_id () const {return auto_id;}

//Data Fields
private:
	friend class odb::access;
	#pragma db id auto
	unsigned long auto_id;
	int hhold;
	int person;
	int tour;
	int trip;
	int mode;
	double time;
	double distance;
	double speed;
	shared_ptr<Link> link;
	int dir;
	int lane;
	double offset;
	int route;

};

}//end of namespace
#endif // InputContext