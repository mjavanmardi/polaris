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
class Vehicle_Type;
class Trip;
class Plan;
class Taveler;
class Person;
class Activity;
class Powertrain_Type;
class Vehicle_Class;
class Fuel_Type;
class Automation_Type;
class Connectivity_Type;
class link_travel;
class Path;


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

//#pragma db object //table("HOUSEHOLD")
//class Household
//{
//public:
//	// Default Constructor
//	Household () {}	
//	//Constructor
//	Household ( int hhold_, int location_, int persons_, int workers_, int vehicles_, int type_, int partition_, int person_, int age_, int relate_, int gender_, int work_, int drive_ )  
//	: hhold (hhold_), location (location_), persons (persons_), workers (workers_), vehicles (vehicles_), type (type_), partition (partition_), person (person_), age (age_), relate (relate_), gender (gender_), work (work_), drive (drive_)
//	{
//	}
//	//Accessors
//	const int& getHhold () const {return hhold;}
//	void setHhold (const int& hhold_){hhold = hhold_;}
//	const int& getLocation () const {return location;}
//	void setLocation (const int& location_){location = location_;}
//	const int& getPersons () const {return persons;}
//	void setPersons (const int& persons_){persons = persons_;}
//	const int& getWorkers () const {return workers;}
//	void setWorkers (const int& workers_){workers = workers_;}
//	const int& getVehicles () const {return vehicles;}
//	void setVehicles (const int& vehicles_){vehicles = vehicles_;}
//	const int& getType () const {return type;}
//	void setType (const int& type_){type = type_;}
//	const int& getPartition () const {return partition;}
//	void setPartition (const int& partition_){partition = partition_;}
//	const int& getPerson () const {return person;}
//	void setPerson (const int& person_){person = person_;}
//	const int& getAge () const {return age;}
//	void setAge (const int& age_){age = age_;}
//	const int& getRelate () const {return relate;}
//	void setRelate (const int& relate_){relate = relate_;}
//	const int& getGender () const {return gender;}
//	void setGender (const int& gender_){gender = gender_;}
//	const int& getWork () const {return work;}
//	void setWork (const int& work_){work = work_;}
//	const int& getDrive () const {return drive;}
//	void setDrive (const int& drive_){drive = drive_;}
//	const unsigned long& getPrimaryKey () const {return household;}
//	const unsigned long& gehousehold () const {return household;}
//
////Data Fields
//private:
//	friend class odb::access;
//	#pragma db id auto
//	unsigned long household;
//	int hhold;
//	int location;
//	int persons;
//	int workers;
//	int vehicles;
//	int type;
//	int partition;
//	int person;
//	int age;
//	int relate;
//	int gender;
//	int work;
//	int drive;
//
//};
//
//#pragma db object //table("PERSON")
//class Person
//{
//public:
//    // Default Constructor
//    Person () {}        
//	Person (int id_, int school_location_id_, int work_location_id_, shared_ptr<Household> household_)
//	: id (id_), school_location_id (school_location_id_), work_location_id (work_location_id_), household (household_)
//	{
//	}
//	Person (int id_, int school_location_id_, int work_location_id_)
//	: id (id_), school_location_id (school_location_id_), work_location_id (work_location_id_)
//	{
//	}
//	//Accessors
//	const int& getId () const {return id;}
//	void setId (const int& id_) {id = id_;}
//	const int& getSchool_Location_Id () const {return school_location_id;}
//	void setSchool_Location_Id (const int& school_location_id_) {school_location_id = school_location_id_;}
//	const int& getWork_Location_Id () const {return work_location_id;}
//	void setWork_Location_Id (const int& work_location_id_) {work_location_id = work_location_id_;}
//	const int& getAge () const {return age;}
//	void setAge (const int& age_) {age = age_;}
//	const shared_ptr<Household> getHousehold () const {return household;}
//	void setHousehold (const shared_ptr<Household> household_) {household = household_;}
//	const unsigned int& getPrimaryKey () const {return person;}
//	const unsigned int& getPerson () const {return person;}
//	//Data Fields
//private:
//	friend class odb::access;
//	#pragma db id auto
//	unsigned int person;
//	int id;
//	int school_location_id;
//	int work_location_id;
//	int age;
//	#pragma db not_null
//	shared_ptr<Household> household;
//};
#pragma db object //table("HOUSEHOLD")
class Household
{
public:
	// Default Constructor
	Household () {}	
	//Constructor
	Household ( long long hhold_, long long location_, int persons_, int workers_, int vehicles_, int type_, int income_)  
	: hhold (hhold_), location (location_), persons (persons_), workers (workers_), vehicles (vehicles_), type (type_), income (income_)
	{
	}
	//Accessors
	const long long& getHhold () const {return hhold;}
	void setHhold (const long long& hhold_){hhold = hhold_;}
	const long long& getLocation () const {return location;}
	void setLocation (const long long& location_){location = location_;}
	const int& getPersons () const {return persons;}
	void setPersons (const int& persons_){persons = persons_;}
	const int& getWorkers () const {return workers;}
	void setWorkers (const int& workers_){workers = workers_;}
	const int& getVehicles () const {return vehicles;}
	void setVehicles (const int& vehicles_){vehicles = vehicles_;}
	const int& getType () const {return type;}
	void setType (const int& type_){type = type_;}
	const int& getIncome () const {return income;}
	void setIncome (const int& income_){income = income_;}
	const unsigned long& getPrimaryKey () const {return household;}
	const unsigned long& getHousehold () const {return household;}

//Data Fields
private:
	friend class odb::access;
	#pragma db id auto
	unsigned long household;
	long long hhold;
	long long location;
	int persons;
	int workers;
	int vehicles;
	int type;
	int income;

};

#pragma db object //table("PERSON")
class Person
{
public:
    // Default Constructor
    Person () {}        
	//Synthetic_Person (int id_, int school_location_id_, int work_location_id_, shared_ptr<Synthetic_Household> household_)
	//: id (id_), school_location_id (school_location_id_), work_location_id (work_location_id_), household (household_)
	//{
	//}
	//Synthetic_Person (int id_, int school_location_id_, int work_location_id_)
	//: id (id_), school_location_id (school_location_id_), work_location_id (work_location_id_)
	//{
	//}

	//Accessors
	const int& getId () const {return id;}
	void setId (const int& id_) {id = id_;}

	const int& getSchool_Location_Id () const {return school_location_id;}
	void setSchool_Location_Id (const int& school_location_id_) {school_location_id = school_location_id_;}

	const int& getWork_Location_Id () const {return work_location_id;}
	void setWork_Location_Id (const int& work_location_id_) {work_location_id = work_location_id_;}

	const int& getAge () const {return age;}
	void setAge (const int& age_) {age = age_;}

	const int& getWorker_class () const {return worker_class;}
	void setWorker_class (const int& worker_class_) {worker_class = worker_class_;}

	const int& getEducation () const {return education;}
	void setEducation (const int& education_) {education = education_;}

	const int& getIndustry () const {return industry;}
	void setIndustry (const int& industry_) {industry = industry_;}

	const int& getEmployment () const {return employment;}
	void setEmployment (const int& employment_) {employment = employment_;}

	const int& getGender () const {return gender;}
	void setGender (const int& gender_) {gender = gender_;}

	const int& getIncome () const {return income;}
	void setIncome (const int& income_) {income = income_;}

	const int& getJourney_to_work_arrival_time () const {return journey_to_work_arrival_time;}
	void setJourney_to_work_arrival_time (const int& journey_to_work_arrival_time_) {journey_to_work_arrival_time = journey_to_work_arrival_time_;}

	const int& getJourney_to_work_mode () const {return journey_to_work_mode;}
	void setJourney_to_work_mode (const int& journey_to_work_mode_) {journey_to_work_mode = journey_to_work_mode_;}

	const int& getJourney_to_work_travel_time () const {return journey_to_work_travel_time;}
	void setJourney_to_work_travel_time (const int& journey_to_work_travel_time_) {journey_to_work_travel_time = journey_to_work_travel_time_;}

	const int& getJourney_to_work_vehicle_occupancy () const {return journey_to_work_vehicle_occupancy;}
	void setJourney_to_work_vehicle_occupancy (const int& journey_to_work_vehicle_occupancy_) {journey_to_work_vehicle_occupancy = journey_to_work_vehicle_occupancy_;}

	const int& getMarital_status () const {return marital_status;}
	void setMarital_status (const int& marital_status_) {marital_status = marital_status_;}

	const int& getRace () const {return race;}
	void setRace (const int& race_) {race = race_;}

	const int& getSchool_enrollment () const {return school_enrollment;}
	void setSchool_enrollment (const int& school_enrollment_) {school_enrollment = school_enrollment_;}

	const int& getSchool_grade_level () const {return school_grade_level;}
	void setSchool_grade_level (const int& school_grade_level_) {school_grade_level = school_grade_level_;}

	const int& getWork_hours () const {return work_hours;}
	void setWork_hours (const int& work_hours_) {work_hours = work_hours_;}

	const shared_ptr<Household> getHousehold () const {return household;}
	void setHousehold (const shared_ptr<Household> household_) {household = household_;}
	const unsigned int& getPrimaryKey () const {return person;}
	const unsigned int& getPerson () const {return person;}
	//Data Fields
private:
	friend class odb::access;
	#pragma db id auto
	unsigned int person;
	int id;
	int school_location_id;
	int work_location_id;
	int age;
	int worker_class;
	int education;
	int industry;
	int employment;
	int gender;
	int income;
	int journey_to_work_arrival_time;
	int journey_to_work_mode;
	int journey_to_work_travel_time;
	int journey_to_work_vehicle_occupancy;
	int marital_status;
	int race;
	int school_enrollment;
	int school_grade_level;
	int work_hours;
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
	Vehicle ( int hhold_, int parking_, shared_ptr<Vehicle_Type> type_, int subtype_)  
	: hhold (hhold_), parking (parking_), type (type_), subtype (subtype_)
	{
	}
	//Accessors
	const int& getHhold () const {return hhold;}
	void setHhold (const int& hhold_){hhold = hhold_;}
	const int& getParking () const {return parking;}
	void setParking (const int& parking_){parking = parking_;}
	const int& getL3_wtp() const { return L3_wtp; }
	void setL3_wtp(const int& L3_wtp_) { L3_wtp = L3_wtp_; }
	const int& getL4_wtp() const { return L4_wtp; }
	void setL4_wtp(const int& L4_wtp_) { L4_wtp = L4_wtp_; }
	const shared_ptr<Vehicle_Type> getType () const {return type;}
	void setType (const shared_ptr<Vehicle_Type> type_){type = type_;}
	const int& getSubtype () const {return subtype;}
	void setSubtype (const int& subtype_){subtype = subtype_;}
	const unsigned long& getPrimaryKey () const {return vehicle_id;}
	const unsigned long& getVehicle_Id () const {return vehicle_id;}

//Data Fields
private:
	friend class odb::access;
	#pragma db id auto
	unsigned long vehicle_id;
	int hhold;
	int parking;
	int L3_wtp;
	int L4_wtp;
	#pragma db not_null
	shared_ptr<Vehicle_Type> type;
	int subtype;
};

#pragma db object //table("VEHICLE_TYPE")
class Vehicle_Type
{
public:
	// Default Constructor
	Vehicle_Type () {}	
	//Constructor
	Vehicle_Type ( int type_id_)  
	: type_id (type_id_)
	{
	}
	//Accessors
	const shared_ptr<Powertrain_Type> getPowertrain_type () const {return powertrain_type;}
	void setPowertrain_type (const shared_ptr<Powertrain_Type> powertrain_type_){powertrain_type = powertrain_type_;}
	const shared_ptr<Vehicle_Class> getVehicle_class () const {return vehicle_class;}
	void setVehicle_class (const shared_ptr<Vehicle_Class> vehicle_class_){vehicle_class = vehicle_class_;}
	const shared_ptr<Fuel_Type> getFuel_type () const {return fuel_type;}
	void setFuel_type (const shared_ptr<Fuel_Type> fuel_type_){fuel_type = fuel_type_;}
	const shared_ptr<Automation_Type> getAutomation_type () const {return automation_type;}
	void setAutomation_type (const shared_ptr<Automation_Type> automation_type_){automation_type = automation_type_;}
	const shared_ptr<Connectivity_Type> getConnectivity_type () const {return connectivity_type;}
	void setConnectivity_type (const shared_ptr<Connectivity_Type> connectivity_type_){connectivity_type = connectivity_type_;}
	const unsigned long& getPrimaryKey () const {return type_id;}
	const unsigned long& getVehicle_Id () const {return type_id;}


//Data Fields
private:
	friend class odb::access;
	#pragma db id //auto
	unsigned long type_id;

	#pragma db not_null
	shared_ptr<Powertrain_Type> powertrain_type;
	#pragma db not_null
	shared_ptr<Vehicle_Class> vehicle_class;
	#pragma db not_null
	shared_ptr<Fuel_Type> fuel_type;
	#pragma db not_null
	shared_ptr<Automation_Type> automation_type;
	#pragma db not_null
	shared_ptr<Connectivity_Type> connectivity_type;
};

#pragma db object //table("Powertrain_Type")
class Powertrain_Type
{
public:
	// Default Constructor
	Powertrain_Type () {}	
	//Constructor
	Powertrain_Type ( int type_id_, std::string type_)  
	: type_id (type_id_), type (type_)
	{
	}
	//Accessors
	const std::string& getType () const {return type;}
	void setType (const std::string& type_) { type =  type_;}

	const unsigned long& getPrimaryKey () const {return type_id;}
	const unsigned long& getPowertrain_type () const {return type_id;}


//Data Fields
private:
	friend class odb::access;
	#pragma db id //auto
	unsigned long type_id;
	std::string type;
};

#pragma db object //table("Vehicle_Class")
class Vehicle_Class
{
public:
	// Default Constructor
	Vehicle_Class () {}	
	//Constructor
	Vehicle_Class ( int class_id_, std::string type_)  
	: class_id (class_id_), class_type (type_)
	{
	}
	//Accessors
	const std::string& getType () const {return class_type;}
	void setType (const std::string& type_) { class_type =  type_;}
	const unsigned long& getPrimaryKey () const {return class_id;}
	const unsigned long& getVehicle_class () const {return class_id;}
	const int& getCapacity () const {return capacity;}
	void setCapacity (const int& capacity_){capacity = capacity_;}
	const double& getLength () const {return length;}
	void setLength (const double& length_){length = length_;}
	const double& getMax_Speed () const {return max_speed;}
	void setMax_Speed (const double& max_speed_){max_speed = max_speed_;}
	const double& getMax_Accel () const {return max_accel;}
	void setMax_Accel (const double& max_accel_){max_accel = max_accel_;}
	const double& getMax_Decel () const {return max_decel;}
	void setMax_Decel (const double& max_decel_){max_decel = max_decel_;}


//Data Fields
private:
	friend class odb::access;
	#pragma db id //auto
	unsigned long class_id;
	std::string class_type;
	int capacity;
	double length;
	double max_speed;
	double max_accel;
	double max_decel;
};

#pragma db object //table("Fuel_Type")
class Fuel_Type
{
public:
	// Default Constructor
	Fuel_Type () {}	
	//Constructor
	Fuel_Type ( int type_id_, std::string type_)  
	: type_id (type_id_), type (type_)
	{
	}
	//Accessors
	const std::string& getType () const {return type;}
	void setType (const std::string& type_) { type =  type_;}

	const unsigned long& getPrimaryKey () const {return type_id;}
	const unsigned long& getFuel_type () const {return type_id;}


//Data Fields
private:
	friend class odb::access;
	#pragma db id //auto
	unsigned long type_id;
	std::string type;
};

#pragma db object //table("Automation_Type")
class Automation_Type
{
public:
	// Default Constructor
	Automation_Type () {}	
	//Constructor
	Automation_Type ( int type_id_, std::string type_)  
	: type_id (type_id_)
	{
	}
	//Accessors
	const std::string& getType () const {return type;}
	void setType (const std::string& type_) { type =  type_;}
	const bool& getAcc () const {return acc;}
	void setAcc (const bool& acc_) { acc =  acc_;}
	const bool& getCacc () const {return cacc;}
	void setCacc (const bool& cacc_) { cacc =  cacc_;}
	const bool& getConnected_signal () const {return connected_signal;}
	void setConnected_signal (const bool& connected_signal_) { connected_signal =  connected_signal_;}
	const bool& getFully_autonomous () const {return fully_autonomous;}
	void setFully_autonomous (const bool& fully_autonomous_) { fully_autonomous =  fully_autonomous_;}
	const unsigned long& getPrimaryKey () const {return type_id;}
	const unsigned long& getAutomation_type () const {return type_id;}


//Data Fields
private:
	friend class odb::access;
	#pragma db id //auto
	unsigned long type_id;
	std::string type;
	bool acc;
	bool cacc;
	bool connected_signal;
	bool fully_autonomous;
};

#pragma db object //table("Connectivity_Type")
class Connectivity_Type
{
public:
	// Default Constructor
	Connectivity_Type () {}	
	//Constructor
	Connectivity_Type ( int type_id_, std::string type_)  
	: type_id (type_id_), type (type_)
	{
	}
	//Accessors
	const std::string& getType () const {return type;}
	void setType (const std::string& type_) { type =  type_;}

	const unsigned long& getPrimaryKey () const {return type_id;}
	const unsigned long& getConnectivity_type () const {return type_id;}


//Data Fields
private:
	friend class odb::access;
	#pragma db id //auto
	unsigned long type_id;
	std::string type;
};


#pragma db object
class Activity
{
public:
    // Default Constructor
    Activity () {}        
	Activity (int id_, int seq_num_, int location_id_, double start_time_, double duration_, std::string mode_, std::string type_, shared_ptr<Person> person_, /*int person_, /*shared_ptr<Trip>*/unsigned long trip_, int origin_id_)
	: id (id_), seq_num(seq_num_), location_id (location_id_), start_time (start_time_), duration (duration_), mode (mode_), type (type_), person (person_), trip (trip_), origin_id(origin_id_)
	{
	}
	Activity (int id_, int location_id_, double start_time_, double duration_, std::string mode_, std::string type_)
	: id (id_), location_id (location_id_), start_time (start_time_), duration (duration_), mode (mode_), type (type_)
	{
	}
	//Accessors
	const int& getId () const {return id;}
	void setId (const int& id_) {id = id_;}
	const int& getSeq_num() const { return seq_num; }
	void setSeq_num(const int& seq_num_) { seq_num = seq_num_; }
	const int& getLocation_Id () const {return location_id;}
	void setLocation_Id (const int& location_id_) {location_id = location_id_;}
	const double& getStart_Time () const {return start_time;}
	void setStart_Time (const double& start_time_) {start_time = start_time_;}
	const double& getDuration () const {return duration;}
	void setDuration (const double& duration_) {duration = duration_;}
	const std::string& getMode () const {return mode;}
	void setMode (const std::string& mode_) {mode = mode_;}
	const std::string& getType () const {return type;}
	void setType (const std::string& type_) { type =  type_;}
	const shared_ptr<Person> getPerson () const {return person;}
	void setPerson (const shared_ptr<Person> person_) {person = person_;}
	/*const shared_ptr<Trip> getTrip () const {return trip;}
	void setTrip (const shared_ptr<Trip> trip_) {trip = trip_;}*/
	/*const int& getPerson () const {return person;}
	void setPerson (const int& person_){person = person_;}*/
	const unsigned long& getTrip () const {return trip;}
	void setTrip (const unsigned long& trip_){trip = trip_;}
	const int& getOrigin_Id() const { return origin_id; }
	void setOrigin_Id(const int& origin_id_) { origin_id = origin_id_; }
	//Data Fields
private:
	friend class odb::access;
	#pragma db auto id
	int id;
	int seq_num;
	int location_id;
	double start_time;
	double duration;
	std::string mode;
	std::string type;
	#pragma db not_null
	shared_ptr<Person> person;
	//shared_ptr<Trip> trip;
	//int person;
	unsigned long trip;
	int origin_id;
};


#pragma db object //table("TRIP")
class Trip
{
public:
	// Default Constructor
	Trip () {}	
	//Constructor
	Trip ( int hhold_, /*int person_*/shared_ptr<Person> person_, int tour_, int trip_, double start_, double end_, double duration_, int origin_, int destination_, int purpose_, int mode_, int constraint_, int priority_, shared_ptr<Vehicle> vehicle_, int passengers_, int type_, int partition_ )  
	: hhold (hhold_), person (person_), tour (tour_), trip (trip_), start (start_), end (end_), duration (duration_), origin (origin_), destination (destination_), purpose (purpose_), mode (mode_), constraint (constraint_), priority (priority_), vehicle (vehicle_), passengers (passengers_), type (type_), partition (partition_)
	{
	}
	//Accessors
	const int& getHhold () const {return hhold;}
	void setHhold (const int& hhold_){hhold = hhold_;}
	const shared_ptr<Person> getPerson () const {return person;}
	void setPerson (const shared_ptr<Person> person_) {person = person_;}
	/*const int& getPerson () const {return person;}
	void setPerson (const int& person_){person = person_;}*/
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
	const shared_ptr<Vehicle> getVehicle () const {return vehicle;}
	void setVehicle (shared_ptr<Vehicle> vehicle_){vehicle = vehicle_;}
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
	//int person;
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
	shared_ptr<Vehicle> vehicle;
	int passengers;
	int type;
	int partition;
	#pragma db not_null
	shared_ptr<Person> person;

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

#pragma db value
class link_travel
{
public:
	// Default Constructor
	link_travel() {}
	link_travel(int link_, bool dir_, int entering_time_, int travel_time_, int delayed_time_, float exit_position_)
		: link(link_), dir(dir_), entering_time(entering_time_), travel_time(travel_time_), delayed_time(delayed_time_), exit_position(exit_position_)
	{
	}
	//Accessors
	const int& getLink() const { return link; }
	void setLink(const int& link_) { link = link_; }
	const bool& getDir() const { return dir; }
	void setDir(const bool& dir_) { dir = dir_; }
	const int& getEntering_Time() const { return entering_time; }
	void setEntering_Time(const int& entering_time_) { entering_time = entering_time_; }
	const int& getTravel_Time() const { return travel_time; }
	void setTravel_Time(const int& travel_time_) { travel_time = travel_time_; }
	const int& getDelayed_Time() const { return delayed_time; }
	void setDelayed_Time(const int& delayed_time_) { delayed_time = delayed_time_; }
	const float& getExit_Position() const { return exit_position; }
	void setExit_Position(const float& exit_position_) { exit_position = exit_position_; }
	//Data Fields
private:
	friend class odb::access;
	int link;
	bool dir;
	int entering_time;
	int travel_time;
	int delayed_time;
	float exit_position;
};


#pragma db object
class Path
{
public:
	// Default Constructor
	Path() {}
	Path(int id_, int traveler_id_, shared_ptr<Vehicle> vehicle_, /*int origin_zone_, int destination_zone_,*/ int origin_activity_location_, int destination_activity_location_, int origin_link_, int destination_link_, int num_links_, int departure_time_, int routed_time_, int travel_time_, std::vector<link_travel > links_)
		: id(id_), traveler_id(traveler_id_), vehicle(vehicle_), /*origin_zone (origin_zone_), destination_zone (destination_zone_),*/ origin_activity_location(origin_activity_location_), destination_activity_location(destination_activity_location_), origin_link(origin_link_), destination_link(destination_link_), num_links(num_links_), departure_time(departure_time_), routed_time(routed_time_), travel_time(travel_time_), links(links_)
	{
	}
	//Accessors
	const int& getId() const { return id; }
	void setId(const int& id_) { id = id_; }
	const shared_ptr<Vehicle> getVehicle() const { return vehicle; }
	void setVehicle(const shared_ptr<Vehicle> vehicle_) { vehicle = vehicle_; }
	//const int& getOrigin_Zone () const {return origin_zone;}
	//void setOrigin_Zone (const int& origin_zone_) {origin_zone = origin_zone_;}
	//const int& getDestination_Zone () const {return destination_zone;}
	//void setDestination_Zone (const int& destination_zone_) {destination_zone = destination_zone_;}
	const int& getTraveler_ID() const { return traveler_id; }
	void setTraveler_ID(const int& traveler_id_) { traveler_id = traveler_id_; }
	const int& getOrigin_Activity_Location() const { return origin_activity_location; }
	void setOrigin_Activity_Location(const int& origin_activity_location_) { origin_activity_location = origin_activity_location_; }
	const int& getDestination_Activity_Location() const { return destination_activity_location; }
	void setDestination_Activity_Location(const int& destination_activity_location_) { destination_activity_location = destination_activity_location_; }
	const int& getOrigin_Link() const { return origin_link; }
	void setOrigin_Link(const int& origin_link_) { origin_link = origin_link_; }
	const int& getDestination_Link() const { return destination_link; }
	void setDestination_Link(const int& destination_link_) { destination_link = destination_link_; }
	const int& getNum_Links() const { return num_links; }
	void setNum_Links(const int& num_links_) { num_links = num_links_; }
	const int& getDeparture_Time() const { return departure_time; }
	void setDeparture_Time(const int& departure_time_) { departure_time = departure_time_; }
	const int& getRouted_Time() const { return routed_time; }
	void setRouted_Time(const int& routed_time_) { routed_time = routed_time_; }
	const int& getTravel_Time() const { return travel_time; }
	void setTravel_Time(const int& travel_time_) { travel_time = travel_time_; }
	const std::vector<link_travel >& getLinks() const { return links; }
	void setLinks(const std::vector<link_travel >& links_) { links = links_; }
	void setLink(const link_travel  links_) { links.push_back(links_); }
	//Data Fields
private:
	friend class odb::access;
#pragma db auto id
	int id;
#pragma db not_null
	//#pragma db not_null
	//int origin_zone;
	//#pragma db not_null
	//int destination_zone;
	int traveler_id;
#pragma db not_null
	int origin_activity_location;
#pragma db not_null
	int destination_activity_location;
#pragma db not_null
	int origin_link;
#pragma db not_null
	int destination_link;
#pragma db not_null
	int num_links;
#pragma db not_null
	int departure_time;
#pragma db not_null
	int routed_time;
#pragma db not_null
	int travel_time;
	std::vector<link_travel > links;
	shared_ptr<Vehicle> vehicle;
};

}//end of io namespace
}//end of polaris namespace
#endif // IODemand