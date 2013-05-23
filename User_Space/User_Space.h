#pragma once

//#include "User_Space_Includes.h"

void* _global_network;
void* _global_scenario;

#include "Polaris_Intersection_Implementation.h"
#include "Polaris_Link_Implementation.h"
#include "Polaris_Network_Implementation.h"
#include "Polaris_Vehicle_Implementation.h"
#include "Polaris_Routing_Implementation.h"
#include "Polaris_Demand_Implementation.h"
#include "Polaris_Activity_Location_Implementation.h"
#include "Polaris_Traveler_Implementation.h"
#include "Polaris_Intersection_Control_Implementation.h"


//#include "Polaris_Operation_Implementation.h"
#include "Polaris_Zone_Implementation.h"
#include "Polaris_Plan_Implementation.h"
#include "Polaris_Movement_Plan_Implementation.h"
#include "Polaris_Turn_Movement_Implementation.h"


#include "Polaris_Link_Event_Handler_Implementation.h"

#include "Polaris_Network_Implementation_Input_File.h"
#include "Polaris_Network_Implementation_Output_File.h"
#include "Polaris_Network_Implementation_Results.h"
#ifndef EXCLUDE_DEMAND
#include "Polaris_Network_Skimming_Implementation.h"
#include "Activity_Implementations.h"


// POPSYN COMPONENTS
//#include "Population_Unit_Implementations.h"
//#include "Synthesis_Zone_Implementations.h"
#include "Synthesis_Region_Implementation.h"
#include "PopSyn_Implementations.h"
#endif


// ITS COMPONENTS

#include "Geometry_Implementation.h"
#include "Advisory_Radio_Implementation.h"
#include "Variable_Message_Sign_Implementation.h"
#include "Depot_Implementation.h"
#include "Link_Control_Implementation.h"
#include "Traffic_Management_Center_Implementation.h"
#include "Polaris_Sensor_Implementation.h"

// NETWORK_EVENT COMPONENTS

#include "Network_Event_Implementations.h"

#ifndef EXCLUDE_DEMAND
// PERSON AGENT COMPONENTS

#include "Person_Planner_Implementations.h"
#include "Activity_Generator_Implementations.h"
#include "Destination_Choice_Implementation.h"
#include "Activity_Timing_Choice_Implementation.h"
#include "Person_Properties_Implementations.h"
#include "Person_Scheduler_Implementations.h"
#include "Person_Perception_Implementation.h"
#include "Person_Implementations.h"
#endif