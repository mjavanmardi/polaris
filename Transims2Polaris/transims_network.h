#pragma once

#include "Data_Service.hpp"


class TransimsNetwork : public Data_Service
{
public:
	//default constructor
	TransimsNetwork (void) : Data_Service() {
		Program ("Transims2Polaris");
		Version (1);
		Title ("Reading the Network Files");

		System_File_Type required_files [] = {
			END_FILE
		};
		System_File_Type optional_files [] = {
			 NODE, LINK,LOCATION, CONNECTION, PARKING, ZONE, SHAPE, POCKET, SIGN, SIGNAL, TIMING_PLAN, PHASING_PLAN, DETECTOR, TRIP, VEHICLE, VEHICLE_TYPE, LANE_USE, END_FILE
		};
		Required_System_Files (required_files);
		Optional_System_Files (optional_files);

		Control_Key transims2polaris_keys [] = { //--- code, key, level, status, type, default, range, help ----
			{NEW_SQLITE_DATABASE, "NEW_SQLITE_DATABASE", LEVEL0, REQ_KEY, OUT_KEY, "", FILE_RANGE, NO_HELP},
			{ADD_GEOMETRY, "ADD_GEOMETRY", LEVEL0, OPT_KEY, BOOL_KEY, "", BOOL_RANGE, NO_HELP},
			{GENERATE_TRIP_WITH_REF, "GENERATE_TRIP_WITH_REF", LEVEL0, OPT_KEY, BOOL_KEY, "", BOOL_RANGE, NO_HELP},
			{SRID, "SRID", LEVEL0, OPT_KEY, INT_KEY, "32616", "4000..40000", NO_HELP},
			{MESSAGE, "MESSAGE", LEVEL0, OPT_KEY, TEXT_KEY, "", FORMAT_RANGE, NO_HELP},
			END_CONTROL
		};
		Key_List(transims2polaris_keys);
	}
	~TransimsNetwork (void) {}

	void Init(int argc, char* argv[])
	{
		Start_Execution(argc,argv);
	}

	void Init(Strings control_keys)
	{
		Quiet(false);
		Start_Execution(control_keys);
	}
	void Init()
	{
		Quiet(false);
		Strings control_keys;
		//control_keys.push_back("NODE_FILE C:/Users/vsokolov/usr/polaris/TransimsData/Case1/network/Node.txt");
		//control_keys.push_back("LINK_FILE C:/Users/vsokolov/usr/polaris/TransimsData/Case1/network/Link.txt");
		//control_keys.push_back("LOCATION_FILE C:/Users/vsokolov/usr/polaris/TransimsData/Case1/network/Location.txt");
		//control_keys.push_back("CONNECTION_FILE C:/Users/vsokolov/usr/polaris/TransimsData/Case1/network/Connection.txt");
		//control_keys.push_back("PARKING_FILE C:/Users/vsokolov/usr/polaris/TransimsData/Case1/network/Parking.txt");
		//control_keys.push_back("ZONE_FILE C:/Users/vsokolov/usr/polaris/TransimsData/Case1/network/Zone.txt");
		//control_keys.push_back("SIGN_FILE C:/Users/vsokolov/usr/polaris/TransimsData/Case1/network/Sign.txt");

		control_keys.push_back("NODE_FILE C:/Users/vsokolov/usr/polaris/TransimsData/TestNet50/network/Node.txt");
		control_keys.push_back("LINK_FILE C:/Users/vsokolov/usr/polaris/TransimsData/TestNet50/network/link.txt");
		control_keys.push_back("LOCATION_FILE C:/Users/vsokolov/usr/polaris/TransimsData/TestNet50/network/Location.txt");
		control_keys.push_back("CONNECTION_FILE C:/Users/vsokolov/usr/polaris/TransimsData/TestNet50/network/Connection.txt");
		control_keys.push_back("PARKING_FILE C:/Users/vsokolov/usr/polaris/TransimsData/TestNet50/network/Parking.txt");
		control_keys.push_back("ZONE_FILE C:/Users/vsokolov/usr/polaris/TransimsData/TestNet50/network/Zone.txt");
		control_keys.push_back("SIGN_FILE C:/Users/vsokolov/usr/polaris/TransimsData/TestNet50/network/Sign.txt");
		control_keys.push_back("SIGNAL_FILE C:/Users/vsokolov/usr/polaris/TransimsData/TestNet50/network/Signal.txt");
		control_keys.push_back("TIMING_PLAN_FILE C:/Users/vsokolov/usr/polaris/TransimsData/TestNet50/network/timing_plan.txt");
		control_keys.push_back("PHASING_PLAN_FILE C:/Users/vsokolov/usr/polaris/TransimsData/TestNet50/network/phasing_plan.txt");
		control_keys.push_back("DETECTOR_FILE C:/Users/vsokolov/usr/polaris/TransimsData/TestNet50/network/detector.txt");
		control_keys.push_back("TRIP_FILE C:/Users/vsokolov/usr/polaris/TransimsData/TestNet50/demand/trip.txt");
		control_keys.push_back("VEHICLE_TYPE_FILE C:/Users/vsokolov/usr/polaris/TransimsData/TestNet50/demand/Vehicle_Type.txt");
		control_keys.push_back("VEHICLE_FILE C:/Users/vsokolov/usr/polaris/TransimsData/TestNet50/demand/vehicle.txt");
		control_keys.push_back("NEW_SQLITE_DATABASE C:/Users/vsokolov/usr/polaris/source/branches/Polaris_Shared/Transims2Polaris/out/test");

		Start_Execution(control_keys);
	}
	void Execute (void)
	{
		//read the network files
		//Data_Service::Execute();
	}
	std::string path_to_database;
	bool add_geo_columns, generate_trip_with_ref;
	int srid;
protected:
	void Program_Control (void)
	{
		//open the network files
		Data_Service::Program_Control();
		String key = Get_Control_String(NEW_SQLITE_DATABASE);
		if (key.empty()) 
		{
			cout << "Error: The required NEW_SQLITE_DATABASE key was not specified\n";
			exit(0);
		}
		this->path_to_database = Project_Filename(key);
		key = Get_Control_String(ADD_GEOMETRY);
		if (key.empty())
			this->add_geo_columns = false;
		else
			this->add_geo_columns = Get_Control_Flag(ADD_GEOMETRY);
		key = Get_Control_String(GENERATE_TRIP_WITH_REF);
		if (key.empty())
			this->generate_trip_with_ref = false;
		else
			this->generate_trip_with_ref = Get_Control_Flag(GENERATE_TRIP_WITH_REF);
		key = Get_Control_String(SRID);
		if (key.empty())
			this->srid = 32616;
		else
			this->srid = Get_Control_Integer(SRID);
	}
	enum Transims2Polaris_Keys {
		NEW_SQLITE_DATABASE = 1, SRID, ADD_GEOMETRY, GENERATE_TRIP_WITH_REF, MESSAGE
	};


}; //end class TransimsNetwork



