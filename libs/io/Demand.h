#ifndef IODemand
#define IODemand

#include "Shared.h"

namespace polaris
{
namespace io
{
//Forward declarations.
//
class Selection;
class Household;
class Vehicle;
class Trip;
class Plan;
class Taveler;
class Person;
class Activity;


#pragma db object //table("SELECTION")
class Selection
{
public:
	// Default Constructor
	Selection () {}	
	//Constructor
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
	const unsigned long& getPrimaryKey () const {return selection;}
	const unsigned long& getSelection () const {return selection;}

//Data Fields
private:
	friend class odb::access;
	#pragma db id auto
	unsigned long selection;
	int hhold;
	int person;
	int tour;
	int trip;
	int type;
	int partition;

};

#pragma db object //table("HOUSEHOLD")
class Household
{
public:
	// Default Constructor
	Household () {}	
	//Constructor
	Household ( int hhold_, int location_, int persons_, int workers_, int vehicles_, int type_, int partition_, int person_, int age_, int relate_, int gender_, int work_, int drive_ )  
	: hhold (hhold_), location (location_), persons (persons_), workers (workers_), vehicles (vehicles_), type (type_), partition (partition_), person (person_), age (age_), relate (relate_), gender (gender_), work (work_), drive (drive_)
	{
	}
	//Accessors
	const int& getHhold () const {return hhold;}
	void setHhold (const int& hhold_){hhold = hhold_;}
	const int& getLocation () const {return location;}
	void setLocation (const int& location_){location = location_;}
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
	const unsigned long& getPrimaryKey () const {return household;}
	const unsigned long& gehousehold () const {return household;}

//Data Fields
private:
	friend class odb::access;
	#pragma db id auto
	unsigned long household;
	int hhold;
	int location;
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

#pragma db object //table("PERSON")
class Person
{
public:
    // Default Constructor
    Person () {}        
	Person (int id_, int school_location_id_, int work_location_id_, shared_ptr<Household> household_)
	: id (id_), school_location_id (school_location_id_), work_location_id (work_location_id_), household (household_)
	{
	}
	Person (int id_, int school_location_id_, int work_location_id_)
	: id (id_), school_location_id (school_location_id_), work_location_id (work_location_id_)
	{
	}
	//Accessors
	const int& getId () const {return id;}
	void setId (const int& id_) {id = id_;}
	const int& getSchool_Location_Id () const {return school_location_id;}
	void setSchool_Location_Id (const int& school_location_id_) {school_location_id = school_location_id_;}
	const int& getWork_Location_Id () const {return work_location_id;}
	void setWork_Location_Id (const int& work_location_id_) {work_location_id = work_location_id_;}
	const int& getAge () const {return age;}
	void setAge (const int& age_) {age = age_;}
	const shared_ptr<Household> getHousehold () const {return household;}
	void setHousehold (const shared_ptr<Household> household_) {household = household_;}
	const unsigned int& getPrimaryKey () const {return person;}
	const unsigned int& getperson () const {return person;}
	//Data Fields
private:
	friend class odb::access;
	#pragma db id auto
	unsigned int person;
	int id;
	int school_location_id;
	int work_location_id;
	int age;
	#pragma db not_null
	shared_ptr<Household> household;
};


#pragma db object //table("VEHICLE")
class Vehicle
{
public:
	// Default Constructor
	Vehicle () {}	
	//Constructor
	Vehicle ( int hhold_, int vehicle_, int parking_, int type_, int subtype_, int partition_ )  
	: hhold (hhold_), vehicle (vehicle_), parking (parking_), type (type_), subtype (subtype_), partition (partition_)
	{
	}
	//Accessors
	const int& getHhold () const {return hhold;}
	void setHhold (const int& hhold_){hhold = hhold_;}
	const int& getVehicle () const {return vehicle;}
	void setVehicle (const int& vehicle_){vehicle = vehicle_;}
	const int& getParking () const {return parking;}
	void setParking (const int& parking_){parking = parking_;}
	const int& getType () const {return type;}
	void setType (const int& type_){type = type_;}
	const int& getSubtype () const {return subtype;}
	void setSubtype (const int& subtype_){subtype = subtype_;}
	const int& getPartition () const {return partition;}
	void setPartition (const int& partition_){partition = partition_;}
	const unsigned long& getPrimaryKey () const {return vehicle_id;}
	const unsigned long& getVehicle_Id () const {return vehicle_id;}

//Data Fields
private:
	friend class odb::access;
	#pragma db id auto
	unsigned long vehicle_id;
	int hhold;
	int vehicle;
	int parking;
	int type;
	int subtype;
	int partition;

};


#pragma db object
class Activity
{
public:
    // Default Constructor
    Activity () {}        
	Activity (int id_, int location_id_, double start_time_, double duration_, std::string mode_, std::string type_, shared_ptr<Person> person_, shared_ptr<Trip> trip_)
	: id (id_), location_id (location_id_), start_time (start_time_), duration (duration_), mode (mode_), type (type_), person (person_), trip (trip_)
	{
	}
	Activity (int id_, int location_id_, double start_time_, double duration_, std::string mode_, std::string type_)
	: id (id_), location_id (location_id_), start_time (start_time_), duration (duration_), mode (mode_), type (type_)
	{
	}
	//Accessors
	const int& getId () const {return id;}
	void setId (const int& id_) {id = id_;}
	const int& getLocation_Id () const {return location_id;}
	void setLocation_Id (const int& location_id_) {location_id = location_id_;}
	const double& getStart_Time () const {return start_time;}
	void setStart_Time (const double& start_time_) {start_time = start_time_;}
	const double& getDuration () const {return duration;}
	void setDuration (const double& duration_) {duration = duration_;}
	const std::string& getMode () const {return mode;}
	void setMode (const std::string& mode_) {mode = mode_;}
	const std::string& getType () const {return type;}
	void setType (const std::string& type_) {type = type_;}
	const shared_ptr<Person> getPerson () const {return person;}
	void setPerson (const shared_ptr<Person> person_) {person = person_;}
	const shared_ptr<Trip> getTrip () const {return trip;}
	void setTrip (const shared_ptr<Trip> trip_) {trip = trip_;}
	//Data Fields
private:
	friend class odb::access;
	#pragma db auto id
	int id;
	int location_id;
	double start_time;
	double duration;
	std::string mode;
	std::string type;
	#pragma db not_null
	shared_ptr<Person> person;
	shared_ptr<Trip> trip;
};


#pragma db object //table("TRIP")
class Trip
{
public:
	// Default Constructor
	Trip () {}	
	//Constructor
	Trip ( int hhold_, int person_, int tour_, int trip_, double start_, double end_, double duration_, int origin_, int destination_, int purpose_, int mode_, int constraint_, int priority_, int vehicle_, int passengers_, int type_, int partition_ )  
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
	const int& getOrigin () const {return origin;}
	void setOrigin (const int& origin_){origin = origin_;}
	const int& getDestination () const {return destination;}
	void setDestination (const int& destination_){destination = destination_;}
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
	const unsigned long& getPrimaryKey () const {return trip_id;}
	const unsigned long& getTrip_Id () const {return trip_id;}

//Data Fields
private:
	friend class odb::access;
	#pragma db id auto
	unsigned long trip_id;
	int hhold;
	int person;
	int tour;
	int trip;
	double start;
	double end;
	double duration;
	int origin;
	int destination;
	int purpose;
	int mode;
	int constraint;
	int priority;
	int vehicle;
	int passengers;
	int type;
	int partition;

};


#pragma db object //table("PLAN")
class Plan
{
public:
	// Default Constructor
	Plan () {}	
	//Constructor
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
	const unsigned long& getPrimaryKey () const {return plan;}
	const unsigned long& getPlan () const {return plan;}

//Data Fields
private:
	friend class odb::access;
	#pragma db id auto
	unsigned long plan;
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
#pragma db object //table("TRAVELER")
class Traveler
{
public:
	// Default Constructor
	Traveler () {}	
	//Constructor
	Traveler ( int hhold_, int person_, int tour_, int trip_, int mode_, double time_, double distance_, double speed_, int link_, int dir_, int lane_, double offset_, int route_ )  
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
	const int& getLink () const {return link;}
	void setLink (const int& link_){link = link_;}
	const int& getDir () const {return dir;}
	void setDir (const int& dir_){dir = dir_;}
	const int& getLane () const {return lane;}
	void setLane (const int& lane_){lane = lane_;}
	const double& getOffset () const {return offset;}
	void setOffset (const double& offset_){offset = offset_;}
	const int& getRoute () const {return route;}
	void setRoute (const int& route_){route = route_;}
	const unsigned long& getPrimaryKey () const {return traveler;}
	const unsigned long& getTraveler () const {return traveler;}

//Data Fields
private:
	friend class odb::access;
	#pragma db id auto
	unsigned long traveler;
	int hhold;
	int person;
	int tour;
	int trip;
	int mode;
	double time;
	double distance;
	double speed;
	int link;
	int dir;
	int lane;
	double offset;
	int route;

};
}//end of io namespace
}//end of polaris namespace
#endif // IODemand