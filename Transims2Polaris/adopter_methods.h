
#pragma once
#include "io/Io.h"
#include "File_Service.hpp"
using namespace polaris::io;
//Converter for Node
shared_ptr<Node> Adapter( Node_File &file, InputContainer& container) 
{
	shared_ptr<Node> result (new Node ());
	result->setNode(file.Node ()); 
	result->setX(file.X ()); 
	result->setY(file.Y ()); 
	result->setZ(file.Z ()); 
	result->setSubarea(file.Subarea ()); 
	result->setPart(file.Partition ());
	return result;
}
//Converter for Zone
shared_ptr<Zone> Adapter( Zone_File &file, InputContainer& container) 
{
	shared_ptr<Zone> result (new Zone ());
	result->setZone(file.Zone ()); 
	result->setX(file.X ()); 
	result->setY(file.Y ()); 
	result->setZ(file.Z ()); 
	result->setArea(file.Area_Type (), container ); 
	result->setMin_X(file.Min_X ()); 
	result->setMin_Y(file.Min_Y ()); 
	result->setMax_X(file.Max_X ()); 
	result->setMax_Y(file.Max_Y ());
	return result;
}
//Converter for Shape
shared_ptr<Shape> Adapter( Shape_File &file, InputContainer& container) 
{
	shared_ptr<Shape> result (new Shape ());
	result->setLink(file.Link (), container); 
	result->setPoints(file.Points ());
	return result;
}
//Converter for Link
shared_ptr<Link> Adapter( Link_File &file, InputContainer& container) 
{
	shared_ptr<Link> result (new Link ());
	result->setLink(file.Link ()); 
	result->setName(file.Name ()); 
	result->setNode_A(file.Node_A (), container); 
	result->setNode_B(file.Node_B (), container); 
	result->setLength(file.Length ()); 
	result->setSetback_A(file.Setback_A ()); 
	result->setSetback_B(file.Setback_B ()); 
	result->setBearing_A(file.Bearing_A ()); 
	result->setBearing_B(file.Bearing_B ()); 
	result->setType(Static_Service::Facility_Code((Facility_Type)file.Type()), container); 
	result->setDivided(file.Divided ()); 
	result->setArea_Type(file.Area_Type (),container); 
	result->setUse(Static_Service::Use_Code((Use_Type)file.Use ())); 
	result->setGrade(file.Grade ()); 
	result->setLanes_Ab(file.Lanes_AB ()); 
	result->setSpeed_Ab(file.Speed_AB ()); 
	result->setFspd_Ab(file.Fspd_AB ()); 
	result->setCap_Ab(file.Cap_AB ()); 
	result->setLanes_Ba(file.Lanes_BA ()); 
	result->setSpeed_Ba(file.Speed_BA ()); 
	result->setFspd_Ba(file.Fspd_BA ()); 
	result->setCap_Ba(file.Cap_BA ()); 
	result->setLeft_Ab(file.Left_AB ()); 
	result->setRight_Ab(file.Right_AB ()); 
	result->setLeft_Ba(file.Left_BA ()); 
	result->setRight_Ba(file.Right_BA ());
	return result;
}
//Converter for Pocket
shared_ptr<Pocket> Adapter( Pocket_File &file, InputContainer& container) 
{
	shared_ptr<Pocket> result (new Pocket ());
	result->setLink(file.Link (), container); 
	result->setDir(file.Dir ()); 
	result->setType(Static_Service::Pocket_Code((Pocket_Type)file.Type())); 
	result->setLanes(file.Lanes ()); 
	result->setLength(file.Length ()); 
	result->setOffset(file.Offset ());
	return result;
}
//Converter for Lane_Use
shared_ptr<Lane_Use> Adapter( Lane_Use_File &file, InputContainer& container) 
{
	shared_ptr<Lane_Use> result (new Lane_Use ());
	result->setLink(file.Link (), container); 
	result->setDir(file.Dir ()); 
	result->setLanes(file.Lanes ()); 
	result->setUse(file.Use ()); 
	result->setType(file.Type ()); 
	result->setMin_Type(file.Min_Veh_Type ()); 
	result->setMax_Type(file.Max_Veh_Type ()); 
	result->setMin_Trav(file.Min_Traveler ()); 
	result->setMax_Trav(file.Max_Traveler ()); 
	result->setStart(file.Start ().Seconds()); 
	result->setEnd(file.End ().Seconds()); 
	result->setOffset(file.Offset ()); 
	result->setLength(file.Length ()); 
	result->setToll(file.Toll ()); 
	result->setRate(file.Toll_Rate ()); 
	result->setMin_Delay(file.Min_Delay ()); 
	result->setMax_Delay(file.Max_Delay ());
	return result;
}
//Converter for Connect
shared_ptr<Connect> Adapter( Connect_File &file, InputContainer& container) 
{
	shared_ptr<Connect> result (new Connect ());
	result->setLink(file.Link (), container); 
	result->setDir(file.Dir ()); 
	result->setTo_Link(file.To_Link (), container); 
	result->setLanes(Static_Service::Lane_Range_Code(file.Lanes())); 
	result->setTo_Lanes(Static_Service::Lane_Range_Code(file.To_Lanes())); 
	result->setType(Static_Service::Connect_Code((Connect_Type)file.Type())); 
	result->setPenalty(file.Penalty ()); 
	result->setSpeed(file.Speed ()); 
	result->setCapacity(file.Capacity ()); 
	result->setIn_High(file.In_High ()); 
	result->setOut_High(file.Out_High ());
	return result;
}
//Converter for Turn_Pen
shared_ptr<Turn_Pen> Adapter( Turn_Pen_File &file, InputContainer& container) 
{
	shared_ptr<Turn_Pen> result (new Turn_Pen ());
	result->setLink(file.Link (), container); 
	result->setDir(file.Dir ()); 
	result->setTo_Link(file.To_Link (), container); 
	result->setStart(file.Start ().Seconds()); 
	result->setEnd(file.End ().Seconds()); 
	result->setUse(file.Use ()); 
	result->setMin_Type(file.Min_Veh_Type ()); 
	result->setMax_Type(file.Max_Veh_Type ()); 
	result->setPenalty(file.Penalty ()); 
	result->setIn_Node(file.In_Node (), container); 
	result->setOut_Node(file.Out_Node (), container);
	return result;
}
//Converter for Parking
shared_ptr<Parking> Adapter( Parking_File &file, InputContainer& container) 
{
	shared_ptr<Parking> result (new Parking ());
	result->setParking(file.Parking ()); 
	result->setLink(file.Link (), container); 
	result->setDir(file.Dir ()); 
	result->setOffset(file.Offset ()); 
	result->setType(file.Type ()); 
	result->setUse(file.Use ()); 
	result->setStart(file.Start ().Seconds()); 
	result->setEnd(file.End ().Seconds()); 
	result->setSpace(file.Space ()); 
	result->setTime_In(file.Time_In ().Seconds()); 
	result->setTime_Out(file.Time_Out ().Seconds()); 
	result->setHourly(file.Hourly ()); 
	result->setDaily(file.Daily ());
	return result;
}
//Converter for Location
shared_ptr<Location> Adapter( Location_File &file, InputContainer& container) 
{
	shared_ptr<Location> result (new Location ());
	result->setLocation(file.Location ()); 
	result->setLink(file.Link (), container); 
	result->setDir(file.Dir ()); 
	result->setOffset(file.Offset ()); 
	result->setSetback(file.Setback ()); 
	result->setZone(file.Zone (), container);
	return result;
}
//Converter for Access
shared_ptr<Access> Adapter( Access_File &file, InputContainer& container) 
{
	shared_ptr<Access> result (new Access ());
	result->setLink(file.Link (), container); 
	result->setFrom_Id(file.From_ID ()); 
	result->setFrom_Type(file.From_Type ()); 
	result->setTo_Id(file.To_ID ()); 
	result->setTo_Type(file.To_Type ()); 
	result->setDir(file.Dir ()); 
	result->setTime(file.Time ().Seconds()); 
	result->setCost(file.Cost ());
	return result;
}
//Converter for Sign
shared_ptr<Sign> Adapter( Sign_File &file, InputContainer& container) 
{
	shared_ptr<Sign> result (new Sign ());
	result->setLink(file.Link (), container); 
	result->setDir(file.Dir ()); 
	result->setSign(Static_Service::Control_Code((Control_Type)file.Sign()));
	return result;
}
//Converter for Signal
shared_ptr<Signal> Adapter( Signal_File &file, InputContainer& container) 
{
	shared_ptr<Signal> result (new Signal ());
	result->setSignal(file.Signal ()); 
	result->setGroup(file.Group ()); 
	result->setTimes(file.Times ()); 
	result->setNodes(file.Nodes ().Integer(), container); 
	result->setType(Static_Service::Signal_Code((Signal_Type)file.Type())); 
	result->setOffset(file.Offset ());
	return result;
}
//Converter for Timing
shared_ptr<Timing> Adapter( Timing_File &file, InputContainer& container) 
{
	shared_ptr<Timing> result (new Timing ());
	result->setSignal(file.Signal (), container); 
	result->setTiming(file.Timing ()); 
	result->setType(file.Type ()); 
	result->setCycle(file.Cycle ()); 
	result->setOffset(file.Offset ()); 
	result->setPhases(file.Phases ());
	return result;
}
//Converter for Phasing
shared_ptr<Phasing> Adapter( Phasing_File &file, InputContainer& container) 
{
	shared_ptr<Phasing> result (new Phasing ());
	result->setSignal(file.Signal (), container); 
	result->setPhasing(file.Phasing ()); 
	result->setPhase(file.Phase ()); 
	result->setDetectors(file.Detectors ()); 
	result->setMovements(file.Movements ());
	return result;
}
//Converter for Detector
shared_ptr<Detector> Adapter( Detector_File &file, InputContainer& container) 
{
	shared_ptr<Detector> result (new Detector ());
	result->setDetector(file.Detector ()); 
	result->setLink(file.Link (), container); 
	result->setDir(file.Dir ()); 
	result->setOffset(file.Offset ()); 
	result->setLength(file.Length ()); 
	result->setLanes(file.Lanes ()); 
	result->setType(file.Type ()); 
	result->setUse(file.Use ()); 
	result->setLow(file.Low_Lane ()); 
	result->setHigh(file.High_Lane ());
	return result;
}
//Converter for Stop
shared_ptr<Stop> Adapter( Stop_File &file, InputContainer& container) 
{
	shared_ptr<Stop> result (new Stop ());
	result->setStop(file.Stop ()); 
	result->setName(file.Name ()); 
	result->setLink(file.Link (), container); 
	result->setDir(file.Dir ()); 
	result->setOffset(file.Offset ()); 
	result->setUse(file.Use ()); 
	result->setType(file.Type ()); 
	result->setSpace(file.Space ());
	return result;
}
//Converter for Fare
shared_ptr<Fare> Adapter( Fare_File &file, InputContainer& container) 
{
	shared_ptr<Fare> result (new Fare ());
	result->setFrom_Zone(file.From_Zone ().Integer(), container); 
	result->setTo_Zone(file.To_Zone ().Integer(), container); 
	result->setFrom_Mode(file.From_Mode ().Integer()); 
	result->setTo_Mode(file.To_Mode ().Integer()); 
	result->setPeriod(file.Period ().Integer()); 
	result->setType(file.Class ().Integer()); 
	result->setFare(file.Fare ());
	return result;
}
//Converter for Line
shared_ptr<Line> Adapter( Line_File &file, InputContainer& container) 
{
	shared_ptr<Line> result (new Line ());
	result->setRoute(file.Route ()); 
	result->setStops(file.Stops (), container); 
	result->setMode(file.Mode ()); 
	result->setType(file.Type ()); 
	result->setName(file.Name ()); 
	result->setStop(file.Stop (), container); 
	result->setZone(file.Zone (), container); 
	result->setFlag(file.Time_Flag ());
	return result;
}
//Converter for Schedule
shared_ptr<Schedule> Adapter( Schedule_File &file, InputContainer& container) 
{
	shared_ptr<Schedule> result (new Schedule ());
	result->setRoute(file.Route ()); 
	result->setStops(file.Stops (), container); 
	result->setStop(file.Stop (), container);
	return result;
}
//Converter for Driver
shared_ptr<Driver> Adapter( Driver_File &file, InputContainer& container) 
{
	shared_ptr<Driver> result (new Driver ());
	result->setRoute(file.Route ()); 
	result->setLinks(file.Links (), container); 
	result->setType(file.Type ()); 
	result->setSubtype(file.SubType ()); 
	result->setLink(file.Link (), container); 
	result->setDir(file.Dir ());
	return result;
}
//Converter for Route_Nodes
shared_ptr<Route_Nodes> Adapter( Route_Nodes_File &file, InputContainer& container) 
{
	shared_ptr<Route_Nodes> result (new Route_Nodes ());
	result->setRoute(file.Route ()); 
	result->setMode(file.Mode ()); 
	result->setVeh_Type(file.Veh_Type (), container); 
	result->setNodes(file.Nodes (), container); 
	result->setName(file.Name ()); 
	result->setNode(file.Node (), container); 
	result->setType(file.Type ()); 
	result->setDwell(file.Dwell ().Seconds()); 
	result->setTime(file.Time ().Seconds()); 
	result->setSpeed(file.Speed ());
	return result;
}
//Converter for Selection
shared_ptr<Selection> Adapter( Selection_File &file, InputContainer& container) 
{
	shared_ptr<Selection> result (new Selection ());
	result->setHhold(file.Household ()); 
	result->setPerson(file.Person ()); 
	result->setTour(file.Tour ()); 
	result->setTrip(file.Trip ()); 
	result->setType(file.Type ()); 
	result->setPartition(file.Partition ());
	return result;
}
//Converter for Household
shared_ptr<Household> Adapter( Household_File &file, InputContainer& container) 
{
	shared_ptr<Household> result (new Household ());
	result->setHhold(file.Household ()); 
	result->setLocation(file.Location ()); 
	result->setPersons(file.Persons ()); 
	result->setWorkers(file.Workers ()); 
	result->setVehicles(file.Vehicles ()); 
	result->setType(file.Type ()); 
	result->setPartition(file.Partition ()); 
	result->setPerson(file.Person ()); 
	result->setAge(file.Age ()); 
	result->setRelate(file.Relate ()); 
	result->setGender(file.Gender ()); 
	result->setWork(file.Work ()); 
	result->setDrive(file.Drive ());
	return result;
}
//Converter for Link_Delay
shared_ptr<Link_Delay> Adapter( Link_Delay_File &file, InputContainer& container) 
{
	shared_ptr<Link_Delay> result (new Link_Delay ());
	result->setLink(file.Link ()); 
	result->setDir(file.Dir ()); 
	result->setType(file.Type ()); 
	result->setStart(file.Start ().Seconds()); 
	result->setEnd(file.End ().Seconds()); 
	result->setFlow(file.Flow ()); 
	result->setTime(file.Time ().Seconds()); 
	result->setOut_Link(file.Out_Link ()); 
	result->setOut_Flow(file.Out_Flow ()); 
	result->setOut_Time(file.Out_Time ().Seconds());
	return result;
}
//Converter for Performance
shared_ptr<Performance> Adapter( Performance_File &file, InputContainer& container) 
{
	shared_ptr<Performance> result (new Performance ());
	result->setSpeed(file.Speed ()); 
	result->setDelay(file.Delay ().Seconds()); 
	result->setDensity(file.Density ()); 
	result->setMax_Den(file.Max_Density ()); 
	result->setRatio(file.Time_Ratio ()); 
	result->setQueue(file.Queue ()); 
	result->setMax_Que(file.Max_Queue ()); 
	result->setFail(file.Cycle_Failure ());
	return result;
}
//Converter for Ridership
shared_ptr<Ridership> Adapter( Ridership_File &file, InputContainer& container) 
{
	shared_ptr<Ridership> result (new Ridership ());
	result->setMode(file.Mode ()); 
	result->setRoute(file.Route ()); 
	result->setRun(file.Run ()); 
	result->setStop(file.Stop (), container); 
	result->setSchedule(file.Schedule ().Seconds()); 
	result->setTime(file.Time ().Seconds()); 
	result->setBoard(file.Board ()); 
	result->setAlight(file.Alight ()); 
	result->setLoad(file.Load ()); 
	result->setFactor(file.Factor ());
	return result;
}
//Converter for Veh_Type
shared_ptr<Veh_Type> Adapter( Veh_Type_File &file, InputContainer& container) 
{
	shared_ptr<Veh_Type> result (new Veh_Type ());
	result->setType(file.Type ()); 
	result->setLength(file.Length ()); 
	result->setMax_Speed(file.Max_Speed ()); 
	result->setMax_Accel(file.Max_Accel ()); 
	result->setMax_Decel(file.Max_Decel ()); 
	result->setOp_Cost(file.Op_Cost ()); 
	result->setUse(file.Use ()); 
	result->setCapacity(file.Capacity ()); 
	result->setLoad(file.Loading ()); 
	result->setUnload(file.Unloading ()); 
	result->setMethod(file.Method ()); 
	result->setMin_Dwell(file.Min_Dwell ().Seconds()); 
	result->setMax_Dwell(file.Max_Dwell ().Seconds()); 
	result->setSubtype(file.SubType ());
	return result;
}
//Converter for Vehicle
shared_ptr<Vehicle> Adapter( Vehicle_File &file, InputContainer& container) 
{
	shared_ptr<Vehicle> result (new Vehicle ());
	result->setHhold(file.Household ()); 
	result->setVehicle(file.Vehicle ()); 
	result->setParking(file.Parking ()); 
	result->setType(file.Type ()); 
	result->setSubtype(file.SubType ()); 
	result->setPartition(file.Partition ());
	return result;
}
//Converter for Trip
shared_ptr<Trip> Adapter( Trip_File &file, InputContainer& container) 
{
	shared_ptr<Trip> result (new Trip ());
	result->setHhold(file.Household ()); 
	result->setPerson(file.Person ()); 
	result->setTour(file.Tour ()); 
	result->setTrip(file.Trip ()); 
	result->setStart(file.Start ().Seconds()); 
	result->setEnd(file.End ().Seconds()); 
	result->setDuration(file.Duration ().Seconds()); 
	result->setOrigin(file.Origin ()); 
	result->setDestination(file.Destination ()); 
	result->setPurpose(file.Purpose ()); 
	result->setMode(file.Mode ()); 
	result->setConstraint(file.Constraint ()); 
	result->setPriority(file.Priority ()); 
	result->setVehicle(file.Vehicle ()); 
	result->setPassengers(file.Passengers ()); 
	result->setType(file.Type ()); 
	result->setPartition(file.Partition ());
	return result;
}
//Converter for Problem
shared_ptr<Problem> Adapter( Problem_File &file, InputContainer& container) 
{
	shared_ptr<Problem> result (new Problem ());
	result->setProblem(file.Problem ()); 
	result->setTime(file.Time ().Seconds()); 
	result->setLink(file.Link ()); 
	result->setDir(file.Dir ()); 
	result->setLane(file.Lane ()); 
	result->setOffset(file.Offset ()); 
	result->setRoute(file.Route ()); 
	result->setSurvey(file.Survey ());
	return result;
}
//Converter for Plan
shared_ptr<Plan> Adapter( Plan_File &file, InputContainer& container) 
{
	shared_ptr<Plan> result (new Plan ());
	result->setDepart(file.Depart ().Seconds()); 
	result->setArrive(file.Arrive ().Seconds()); 
	result->setActivity(file.Activity ().Seconds()); 
	result->setWalk(file.Walk ().Seconds()); 
	result->setDrive(file.Drive ().Seconds()); 
	result->setTransit(file.Transit ().Seconds()); 
	result->setWait(file.Wait ().Seconds()); 
	result->setOther(file.Other ().Seconds()); 
	result->setLength(file.Length ()); 
	result->setCost(file.Cost ()); 
	result->setImpedance(file.Impedance ()); 
	result->setLeg_Mode(file.Leg_Mode ()); 
	result->setLeg_Type(file.Leg_Type ()); 
	result->setLeg_Id(file.Leg_ID ()); 
	result->setLeg_Time(file.Leg_Time ().Seconds()); 
	result->setLeg_Length(file.Leg_Length ()); 
	result->setLeg_Cost(file.Leg_Cost ()); 
	result->setLeg_Imp(file.Leg_Impedance ());
	return result;
}
//Converter for Skim
shared_ptr<Skim> Adapter( Skim_File &file, InputContainer& container) 
{
	shared_ptr<Skim> result (new Skim ());
	result->setTime(file.Time ().Seconds()); 
	result->setWalk(file.Walk ().Seconds()); 
	result->setDrive(file.Drive ().Seconds()); 
	result->setTransit(file.Transit ().Seconds()); 
	result->setWait(file.Wait ().Seconds()); 
	result->setOther(file.Other ().Seconds()); 
	result->setLength(file.Length ()); 
	result->setCost(file.Cost ()); 
	result->setImpedance(file.Impedance ());
	return result;
}
//Converter for Event
shared_ptr<Event> Adapter( Event_File &file, InputContainer& container) 
{
	shared_ptr<Event> result (new Event ());
	result->setHhold(file.Household ()); 
	result->setPerson(file.Person ()); 
	result->setTour(file.Tour ()); 
	result->setTrip(file.Trip ()); 
	result->setMode(file.Mode ()); 
	result->setType(file.Event ()); 
	result->setSchedule(file.Schedule ().Seconds()); 
	result->setActual(file.Actual ().Seconds()); 
	result->setLink(file.Link ()); 
	result->setDir(file.Dir ()); 
	result->setLane(file.Lane ()); 
	result->setOffset(file.Offset ()); 
	result->setRoute(file.Route ());
	return result;
}
//Converter for Traveler
shared_ptr<Traveler> Adapter( Traveler_File &file, InputContainer& container) 
{
	shared_ptr<Traveler> result (new Traveler ());
	result->setHhold(file.Household ()); 
	result->setPerson(file.Person ()); 
	result->setTour(file.Tour ()); 
	result->setTrip(file.Trip ()); 
	result->setMode(file.Mode ()); 
	result->setTime(file.Time ().Seconds()); 
	result->setDistance(file.Distance ()); 
	result->setSpeed(file.Speed ()); 
	result->setLink(file.Link ()); 
	result->setDir(file.Dir ()); 
	result->setLane(file.Lane ()); 
	result->setOffset(file.Offset ()); 
	result->setRoute(file.Route ());
	return result;
}

timing_phase AdapterNested(Timing_File &file, InputContainer& container)
{
	timing_phase nested_record;
	nested_record.phase = file.Phase ();
	nested_record.barrier = file.Barrier ();
	nested_record.ring = file.Ring ();
	nested_record.position = file.Position ();
	nested_record.minimum = file.Min_Green ();
	nested_record.maximum = file.Max_Green ();
	nested_record.extend = file.Extension ();
	nested_record.yellow = file.Yellow ();
	nested_record.red = file.All_Red ();
	return nested_record;
};
shape_geometry AdapterNested(Shape_File &file, InputContainer& container)
{
	shape_geometry nested_record;
	nested_record.x = file.X ();
	nested_record.y = file.Y ();
	nested_record.z = file.Z ();
	return nested_record;
};
signal_time AdapterNested(Signal_File &file, InputContainer& container)
{
	signal_time nested_record;
	nested_record.start = file.Start().Seconds();
	nested_record.end = file.End().Seconds();
	nested_record.timing = file.Timing ();
	nested_record.phasing = file.Phasing ();
	return nested_record;
};
phase_movement AdapterNested(Phasing_File &file, InputContainer& container)
{
	phase_movement nested_record;
	nested_record.movement = Static_Service::Movement_Code((Movement_Type)file.Movement());
	nested_record.link = container.Links[file.Link()];
	nested_record.dir = file.Dir ();
	nested_record.to_link = container.Links[file.To_Link()];
	nested_record.protect = Static_Service::Protection_Code((Protection_Type)file.Protection());
	return nested_record;
};
