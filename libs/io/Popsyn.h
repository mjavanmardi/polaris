#include "Shared.h"

namespace polaris
{
namespace io
{
//Forward declarations.
//

class Synthetic_Household;
class Synthetic_Person;


#pragma db object //table("HOUSEHOLD")
class Synthetic_Household
{
public:
	// Default Constructor
	Synthetic_Household () {}	
	//Constructor
	Synthetic_Household ( int hhold_, long long location_, int persons_, int workers_, int vehicles_, int type_, int income_)  
	: hhold (hhold_), location (location_), persons (persons_), workers (workers_), vehicles (vehicles_), type (type_), income (income_)
	{
	}
	//Accessors
	const int& getHhold () const {return hhold;}
	void setHhold (const int& hhold_){hhold = hhold_;}
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
	const unsigned long& gehousehold () const {return household;}

//Data Fields
private:
	friend class odb::access;
	#pragma db id auto
	unsigned long household;
	int hhold;
	long long location;
	int persons;
	int workers;
	int vehicles;
	int type;
	int income;

};

#pragma db object //table("PERSON")
class Synthetic_Person
{
public:
    // Default Constructor
    Synthetic_Person () {}        
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

	const shared_ptr<Synthetic_Household> getHousehold () const {return household;}
	void setHousehold (const shared_ptr<Synthetic_Household> household_) {household = household_;}
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
	shared_ptr<Synthetic_Household> household;
};


}//end of io namespace
}//end of polaris namespace
