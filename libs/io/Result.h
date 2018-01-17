#ifndef IOResult
#define IOResult

#include "Shared.h"

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
class Link_Probability;
class LinkMOE;
class RealtimeLinkMOE;
class TurnMOE;
class RealtimeTurnMOE;
class multimodalSPLabels;

#pragma db object //table("LINK_DELAY")
class Link_Delay
{
public:
	// Default Constructor
	Link_Delay () {}	
	//Constructor
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
	//Constructor
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
	//Constructor
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
	//Constructor
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
	//Constructor
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
#pragma db object
class Link_Probability
{
public:
	// Default Constructor
	Link_Probability () {}        
	Link_Probability (int id_, int dir_link_from_, int dir_link_to_, int count_, float prob_)
		: id (id_), dir_link_from (dir_link_from_), dir_link_to (dir_link_to_), count (count_), prob (prob_)
	{
	}
	//Accessors
	const int& getId () const {return id;}
	void setId (const int& id_) {id = id_;}
	const int& getDir_Link_From () const {return dir_link_from;}
	void setDir_Link_From (const int& dir_link_from_) {dir_link_from = dir_link_from_;}
	const int& getDir_Link_To () const {return dir_link_to;}
	void setDir_Link_To (const int& dir_link_to_) {dir_link_to = dir_link_to_;}
	const int& getCount () const {return count;}
	void setCount (const int& count_) {count = count_;}
	const float& getProb () const {return prob;}
	void setProb (const float& prob_) {prob = prob_;}
	//Data Fields
private:
	friend class odb::access;
#pragma db auto id
	int id;
	int dir_link_from;
	int dir_link_to;
	int count;
	float prob;
};
#pragma db object
class LinkMOE
{
public:
	// Default Constructor
	LinkMOE () {}        
	LinkMOE (int id_, int link_uid_, int link_type, float link_length, int start_time_, int end_time_, float link_travel_time_, float link_travel_time_standard_deviation_, float link_queue_length_, float link_travel_delay_, float link_travel_delay_standard_deviation_, float link_speed_, float link_density_, float link_in_flow_rate_, float link_out_flow_rate_, float link_in_volume_, float link_out_volume_, float link_speed_ratio_, float link_in_flow_ratio_, float link_out_flow_ratio_, float link_density_ratio_, float link_travel_time_ratio_, float num_vehicles_in_link_)
		: id (id_), link_uid (link_uid_), link_type (link_type), link_length (link_length), start_time (start_time_), end_time (end_time_), link_travel_time (link_travel_time_), link_travel_time_standard_deviation (link_travel_time_standard_deviation_), link_queue_length (link_queue_length_), link_travel_delay (link_travel_delay_), link_travel_delay_standard_deviation (link_travel_delay_standard_deviation_), link_speed (link_speed_), link_density (link_density_), link_in_flow_rate (link_in_flow_rate_), link_out_flow_rate (link_out_flow_rate_), link_in_volume (link_in_volume_), link_out_volume (link_out_volume_), link_speed_ratio (link_speed_ratio_), link_in_flow_ratio (link_in_flow_ratio_), link_out_flow_ratio (link_out_flow_ratio_), link_density_ratio (link_density_ratio_), link_travel_time_ratio (link_travel_time_ratio_), num_vehicles_in_link (num_vehicles_in_link_)
	{
	}
	//Accessors
	const int& getId () const {return id;}
	void setId (const int& id_) {id = id_;}
	const int& getLink_Uid () const {return link_uid;}
	void setLink_Uid (const int& link_uid_) {link_uid = link_uid_;}
	const int& getLink_Type () const {return link_type;}
	void setLink_Type (const int& link_type_) {link_type = link_type_;}
	const float& getLink_Length () const {return link_length;}
	void setLink_Length (const float& link_length_) {link_length = link_length_;}
	const int& getStart_Time () const {return start_time;}
	void setStart_Time (const int& start_time_) {start_time = start_time_;}
	const int& getEnd_Time () const {return end_time;}
	void setEnd_Time (const int& end_time_) {end_time = end_time_;}
	const float& getLink_Travel_Time () const {return link_travel_time;}
	void setLink_Travel_Time (const float& link_travel_time_) {link_travel_time = link_travel_time_;}
	const float& getLink_Travel_Time_Standard_Deviation () const {return link_travel_time_standard_deviation;}
	void setLink_Travel_Time_Standard_Deviation (const float& link_travel_time_standard_deviation_) {link_travel_time_standard_deviation = link_travel_time_standard_deviation_;}
	const float& getLink_Queue_Length () const {return link_queue_length;}
	void setLink_Queue_Length (const float& link_queue_length_) {link_queue_length = link_queue_length_;}
	const float& getLink_Travel_Delay () const {return link_travel_delay;}
	void setLink_Travel_Delay (const float& link_travel_delay_) {link_travel_delay = link_travel_delay_;}
	const float& getLink_Travel_Delay_Standard_Deviation () const {return link_travel_delay_standard_deviation;}
	void setLink_Travel_Delay_Standard_Deviation (const float& link_travel_delay_standard_deviation_) {link_travel_delay_standard_deviation = link_travel_delay_standard_deviation_;}
	const float& getLink_Speed () const {return link_speed;}
	void setLink_Speed (const float& link_speed_) {link_speed = link_speed_;}
	const float& getLink_Density () const {return link_density;}
	void setLink_Density (const float& link_density_) {link_density = link_density_;}
	const float& getLink_In_Flow_Rate () const {return link_in_flow_rate;}
	void setLink_In_Flow_Rate (const float& link_in_flow_rate_) {link_in_flow_rate = link_in_flow_rate_;}
	const float& getLink_Out_Flow_Rate () const {return link_out_flow_rate;}
	void setLink_Out_Flow_Rate (const float& link_out_flow_rate_) {link_out_flow_rate = link_out_flow_rate_;}
	const float& getLink_In_Volume () const {return link_in_volume;}
	void setLink_In_Volume (const float& link_in_volume_) {link_in_volume = link_in_volume_;}
	const float& getLink_Out_Volume () const {return link_out_volume;}
	void setLink_Out_Volume (const float& link_out_volume_) {link_out_volume = link_out_volume_;}
	const float& getLink_Speed_Ratio () const {return link_speed_ratio;}
	void setLink_Speed_Ratio (const float& link_speed_ratio_) {link_speed_ratio = link_speed_ratio_;}
	const float& getLink_In_Flow_Ratio () const {return link_in_flow_ratio;}
	void setLink_In_Flow_Ratio (const float& link_in_flow_ratio_) {link_in_flow_ratio = link_in_flow_ratio_;}
	const float& getLink_Out_Flow_Ratio () const {return link_out_flow_ratio;}
	void setLink_Out_Flow_Ratio (const float& link_out_flow_ratio_) {link_out_flow_ratio = link_out_flow_ratio_;}
	const float& getLink_Density_Ratio () const {return link_density_ratio;}
	void setLink_Density_Ratio (const float& link_density_ratio_) {link_density_ratio = link_density_ratio_;}
	const float& getLink_Travel_Time_Ratio () const {return link_travel_time_ratio;}
	void setLink_Travel_Time_Ratio (const float& link_travel_time_ratio_) {link_travel_time_ratio = link_travel_time_ratio_;}
	const float& getNum_Vehicles_In_Link () const {return num_vehicles_in_link;}
	void setNum_Vehicles_In_Link (const float& num_vehicles_in_link_) {num_vehicles_in_link = num_vehicles_in_link_;}
	//Data Fields
private:
	friend class odb::access;
#pragma db auto id
	int id;
#pragma db not_null
	int link_uid;
	int link_type;
	float link_length;
	int start_time;
	int end_time;
	float link_travel_time;
	float link_travel_time_standard_deviation;
	float link_queue_length;
	float link_travel_delay;
	float link_travel_delay_standard_deviation;
	float link_speed;
	float link_density;
	float link_in_flow_rate;
	float link_out_flow_rate;
	float link_in_volume;
	float link_out_volume;
	float link_speed_ratio;
	float link_in_flow_ratio;
	float link_out_flow_ratio;
	float link_density_ratio;
	float link_travel_time_ratio;
	float num_vehicles_in_link;
};
#pragma db object
class RealtimeLinkMOE : public LinkMOE
{};
#pragma db object
class TurnMOE
{
public:
	// Default Constructor
	TurnMOE() {}
	TurnMOE(int id_, int turn_uid_, int start_time_, int end_time_, int inbound_link_uid_, int outbound_link_uid_, int node_uid_, float turn_penalty_, float turn_penalty_sd_, float inbound_turn_travel_time_, float outbound_turn_travel_time_, float turn_flow_rate_)
		: id(id_), turn_uid(turn_uid_),start_time(start_time_), end_time(end_time_), inbound_link_uid(inbound_link_uid_), outbound_link_uid(outbound_link_uid_), node_uid(node_uid_), turn_penalty(turn_penalty_), turn_penalty_sd(turn_penalty_sd_), inbound_turn_travel_time(inbound_turn_travel_time_), outbound_turn_travel_time(outbound_turn_travel_time_), turn_flow_rate(turn_flow_rate)
	{
	}
	//Accessors
	const int& getId() const { return id; }
	void setId(const int& id_) { id = id_; }
	const int& getTurn_Uid() const { return turn_uid; }
	void setTurn_Uid(const int& turn_uid_) { turn_uid = turn_uid_; }
	const int& getStart_Time() const { return start_time; }
	void setStart_Time(const int& start_time_) { start_time = start_time_; }
	const int& getEnd_Time() const { return end_time; }
	void setEnd_Time(const int& end_time_) { end_time = end_time_; }
	const int& getInbound_Link_Uid() const { return inbound_link_uid; }
	void setInbound_Link_Uid(const int& inbound_link_uid_) { inbound_link_uid = inbound_link_uid_; }
	const int& getOutbound_Link_Uid() const { return outbound_link_uid; }
	void setOutbound_Link_Uid(const int& outbound_link_uid_) { outbound_link_uid = outbound_link_uid_; }
	const int& getNode_Uid() const { return node_uid; }
	void setNode_Uid(const int& node_uid_) { node_uid = node_uid_; }
	const float& getTurn_Penalty() const { return turn_penalty; }
	void setTurn_Penalty(const float& turn_penalty_) { turn_penalty = turn_penalty_; }
	const float& getTurn_Penalty_SD() const { return turn_penalty_sd; }
	void setTurn_Penalty_SD(const float& turn_penalty_sd_) { turn_penalty_sd = turn_penalty_sd_; }

	const float& getInbound_Turn_Travel_Time() const { return inbound_turn_travel_time; }
	void setInbound_Turn_Travel_Time(const float& inbound_turn_travel_time_) { inbound_turn_travel_time = inbound_turn_travel_time_; }
	const float& getOutbound_Turn_Travel_Time() const { return outbound_turn_travel_time; }
	void setOutbound_Turn_Travel_Time(const float& outbound_turn_travel_time_) {outbound_turn_travel_time = outbound_turn_travel_time_; }
	const float& getTurn_Flow_Rate() const { return turn_flow_rate; }
	void setTurn_Flow_Rate(const float& turn_flow_rate_) { turn_flow_rate = turn_flow_rate_; }

	//Data Fields
private:
	friend class odb::access;
#pragma db auto id
	int id;
#pragma db not_null
	int turn_uid;
	int start_time;
	int end_time;
	int inbound_link_uid;
	int outbound_link_uid;
	int node_uid;
	float turn_penalty;
	float turn_penalty_sd;
	float inbound_turn_travel_time;
	float outbound_turn_travel_time;
	float turn_flow_rate;
};

#pragma db object
class multimodalSPLabels
{
public:
	// Default Constructor
	multimodalSPLabels() {}
	multimodalSPLabels(int Origin_ID_, int Destination_ID_, int Departure_Time_, int Sub_Mode_, float Arrival_Time_, float Gen_Cost_, float Duration_, int Wait_Count_, float Wait_Time_, float Walk_Time_, float Bike_Time_, float IVTT_, float Car_Time_, float Transfer_Pen_, float Est_Cost_, int Scan_Count_, float aStar_Time_, float visit_Time_, std::string Success_Status_, float Euc_Dist_km_)
		: Origin_ID(Origin_ID_), Destination_ID(Destination_ID_), Departure_Time(Departure_Time_), Sub_Mode(Sub_Mode_), Arrival_Time(Arrival_Time_), Gen_Cost(Gen_Cost_), Duration(Duration_), Wait_Count(Wait_Count_), Wait_Time(Wait_Time_), Walk_Time(Walk_Time_), Bike_Time(Bike_Time_), IVTT(IVTT_), Car_Time(Car_Time_), Transfer_Pen(Transfer_Pen_), Est_Cost(Est_Cost_), Scan_Count(Scan_Count_), aStar_Time(aStar_Time_), visit_Time(visit_Time_), Success_Status(Success_Status_), Euc_Dist_km(Euc_Dist_km_)
	{
	}
	//Accessors
	const int& getOrigin_ID() const { return Origin_ID; }
	void setOrigin_ID(const int& Origin_ID_) { Origin_ID = Origin_ID_; }
	const int& getDestination_ID() const { return Destination_ID; }
	void setDestination_ID(const int& Destination_ID_) { Destination_ID = Destination_ID_; }
	const int& getDeparture_Time() const { return Departure_Time; }
	void setDeparture_Time(const int& Departure_Time_) { Departure_Time = Departure_Time_; }
	const int& getSub_Mode() const { return Sub_Mode; }
	void setSub_Mode(const int& Sub_Mode_) { Sub_Mode = Sub_Mode_; }
	const float& getArrival_Time() const { return Arrival_Time; }
	void setArrival_Time(const float& Arrival_Time_) { Arrival_Time = Arrival_Time_; }
	const float& getGen_Cost() const { return Gen_Cost; }
	void setGen_Cost(const float& Gen_Cost_) { Gen_Cost = Gen_Cost_; }
	const float& getDuration() const { return Duration; }
	void setDuration(const float& Duration_) { Duration = Duration_; }
	const int& getWait_Count() const { return Wait_Count; }
	void setWait_Count(const int& Wait_Count_) { Wait_Count = Wait_Count_; }
	const float& getWait_Timet() const { return Wait_Time; }
	void setWait_Time(const float& Wait_Time_) { Wait_Time = Wait_Time_; }
	const float& getWalk_Time() const { return Walk_Time; }
	void setWalk_Time(const float& Walk_Time_) { Walk_Time = Walk_Time_; }
	const float& getBike_Time() const { return Bike_Time; }
	void setBike_Time(const float& Bike_Time_) { Bike_Time = Bike_Time_; }
	const float& getIVTT() const { return IVTT; }
	void setIVTT(const float& IVTT_) { IVTT = IVTT_; }
	const float& getCar_Time() const { return Car_Time; }
	void setCar_Time(const float& Car_Time_) { Car_Time = Car_Time_; }
	const float& getTransfer_Pen() const { return Transfer_Pen; }
	void setTransfer_Pen(const float& Transfer_Pen_) { Transfer_Pen = Transfer_Pen; }
	const float& getEst_Cost() const { return Est_Cost; }
	void setEst_Cost(const float& Est_Cost_) { Est_Cost = Est_Cost_; }
	const int& getScan_Count() const { return Scan_Count; }
	void setScan_Count(const int& Scan_Count_) { Scan_Count = Scan_Count_; }
	const float& getaStar_Time() const { return aStar_Time; }
	void setaStar_Time(const float& aStar_Time_) { aStar_Time = aStar_Time_; }
	const float& getvisit_Time() const { return visit_Time; }
	void setvisit_Time(const float& visit_Time_) { visit_Time = visit_Time_; }
	const std::string& getSuccess_Status() const { return Success_Status; }
	void setSuccess_Status(const std::string& Success_Status_) { Success_Status = Success_Status_; }
	const float& getEuc_Dist_km() const { return Euc_Dist_km; }
	void setEuc_Dist_km(const float& Euc_Dist_km_) { Euc_Dist_km = Euc_Dist_km_; }

	//Data Fields
private:
	friend class odb::access;
#pragma db auto id
	int id;
#pragma db not_null
	int Origin_ID;
	int Destination_ID;
	int Departure_Time;
	int Sub_Mode;
	float Arrival_Time;
	float Gen_Cost;
	float Duration;
	int Wait_Count;
	float Wait_Time;
	float Walk_Time;
	float Bike_Time;
	float IVTT;
	float Car_Time;
	float Transfer_Pen;
	float Est_Cost;
	int Scan_Count;
	float aStar_Time;
	float visit_Time;
	std::string Success_Status;
	float Euc_Dist_km;
};

#pragma db object
class RealtimeTurnMOE : public TurnMOE
{};
}//end of io namespace
}//end of polaris namespace
#endif // IOResult