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


#include "Polaris_Network_Implementation_Input_File.h"
#include "Polaris_Network_Implementation_Output_File.h"
#include "Polaris_Network_Implementation_Results.h"
#include "Polaris_Network_Skimming_Implementation.h"

#include "Activity_Implementations.h"



// POPSYN COMPONENTS
#include "Population_Unit_Implementations.h"
#include "Synthesis_Zone_Implementations.h"
#include "Synthesis_Region_Implementation.h"
#include "PopSyn_Implementations.h"




// PERSON AGENT COMPONENTS
#include "Person_Planner_Implementations.h"
#include "Activity_Generator_Implementations.h"
#include "Person_Properties_Implementations.h"
#include "Person_Implementations.h"


