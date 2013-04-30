#ifndef IOResult
#define IOResult
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
namespace polaris
{
namespace io
{
//Forward declarations.
//
class Link_Delay;
class Performance;
class Problem;
class Skim;
class Transims_Event;

#pragma db object //table("LINK_DELAY")
class Link_Delay
{
public:
	// Default Constructor
	Link_Delay () {}	
	//Contructor
	Link_Delay ( int link_, int dir_, int type_, double start_, double end_, double flow_, double time_, int out_link_, double out_flow_, double out_time_ )  
	: link (link_), dir (dir_), type (type_), start (start_), end (end_), flow (flow_), time (time_), out_link (out_link_), out_flow (out_flow_), out_time (out_time_)
	{
	}
	//Accessors
	const int& getLink () const {return link;}
	void setLink (const int& link_){link = link_;}
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
	const int& getOut_Link () const {return out_link;}
	void setOut_Link (const int& out_link_){out_link = out_link_;}
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
	int link;
	int dir;
	int type;
	double start;
	double end;
	double flow;
	double time;
	int out_link;
	double out_flow;
	double out_time;

};

#pragma db object //table("PERFORMANCE")
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


#pragma db object //table("PROBLEM")
class Problem
{
public:
	// Default Constructor
	Problem () {}	
	//Contructor
	Problem ( int problem_, double time_, int link_, int dir_, int lane_, double offset_, int route_, int survey_ )  
	: problem (problem_), time (time_), link (link_), dir (dir_), lane (lane_), offset (offset_), route (route_), survey (survey_)
	{
	}
	//Accessors
	const int& getProblem () const {return problem;}
	void setProblem (const int& problem_){problem = problem_;}
	const double& getTime () const {return time;}
	void setTime (const double& time_){time = time_;}
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
	const int& getSurvey () const {return survey;}
	void setSurvey (const int& survey_){survey = survey_;}
	const int& getPrimaryKey () const {return problem;}

//Data Fields
private:
	friend class odb::access;
	#pragma db id
	int problem;
	double time;
	int link;
	int dir;
	int lane;
	double offset;
	int route;
	int survey;
	#pragma db index member(problem)

};


#pragma db object //table("SKIM")
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

#pragma db object //table("EVENT")
class Transims_Event
{
public:
	// Default Constructor
	Transims_Event () {}	
	//Contructor
	Transims_Event ( int hhold_, int person_, int tour_, int trip_, int mode_, int type_, double schedule_, double actual_, int link_, int dir_, int lane_, double offset_, int route_ )  
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
	int link;
	int dir;
	int lane;
	double offset;
	int route;

};
}//end of io namespace
}//end of polaris namespace
#endif // IOResult