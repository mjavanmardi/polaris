#pragma once


void* _global_network;
void* _global_demand;
void* _global_person_logger;
void* _global_vehicle_logger;

#include "Intersection_Implementation.h"
#include "Link_Implementation.h"
#include "Network_Implementation.h"
#include "Vehicle_Implementation.h"
#include "Routing_Implementation.h"
#include "Demand_Implementation.h"
#include "Activity_Location_Implementation.h"
#include "Traveler_Implementation.h"
#include "Intersection_Control_Implementation.h"
#include "Operation_Implementation.h"
#include "Ramp_Metering_Implementation.h"
#include "Routable_Network_Implementation.h"


//#include "Polaris_Operation_Implementation.h"
#include "Zone_Implementation.h"
#include "Plan_Implementation.h"
#include "Movement_Plan_Implementation.h"
#include "Turn_Movement_Implementation.h"
#include "Transit_Route_Implementation.h"
#include "Transit_Pattern_Implementation.h"
#include "Transit_Vehicle_Trip_Implementation.h"

#include "Link_Event_Handler_Implementation.h"

//#include "Polaris_Network_Implementation_Input_File.h"
//#include "Polaris_Network_Implementation_Output_File.h"
#include "Network_Implementation_Results.h"

#include "Network_Skimming_Implementation.h"

// ITS COMPONENTS

//#include "Geometry_Implementation.h"
#include "Advisory_Radio_Implementation.h"
#include "Variable_Message_Sign_Implementation.h"
#include "Depot_Implementation.h"
#include "Link_Control_Implementation.h"
#include "Traffic_Management_Center_Implementation.h"
#include "Sensor_Implementation.h"

// NETWORK_EVENT COMPONENTS

#include "Network_Event_Implementations.h"
#include "Analyze_Link_Group_Implementation.h"

#ifdef ANTARES
#include "Vehicle_Data_Logger_Implementation.h"
#endif