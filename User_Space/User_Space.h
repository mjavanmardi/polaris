#pragma once
#ifndef FOR_LINUX_PORTING
#include "../io/Io.h"
#endif
#include "User_Space_Includes.h"
//#include "User_Space_Forward_Declaration.h"

void* _global_network;
void* _global_scenario;

#ifndef EXAMPLE

#include "Polaris_Intersection_Implementation.h"
#include "Polaris_Link_Implementation.h"
#include "Polaris_Network_Implementation.h"
#include "Polaris_Vehicle_Implementation.h"
#include "Polaris_Routing_Implementation.h"
#include "Polaris_Demand_Implementation.h"
#include "Polaris_Activity_Location_Implementation.h"
#include "Polaris_Traveler_Implementation.h"
#include "Polaris_Intersection_Control_Implementation.h"
#include "Polaris_Operation_Implementation.h"
#include "Polaris_Zone_Implementation.h"
#include "Polaris_Plan_Implementation.h"
#include "Polaris_Movement_Plan_Implementation.h"
#include "Polaris_Turn_Movement_Implementation.h"
#include "Polaris_Network_Implementation_Input_File.h"
#include "Polaris_Network_Implementation_Output_File.h"
#include "Polaris_Network_Implementation_Results.h"
#include "Polaris_Network_Skimming_Implementation.h"

#endif

#ifndef FOR_LINUX_PORTING
#include "Polaris_Network_Implementation_Input_DB.h"
#endif

#include "Activity_Implementations.h"
#include "Population_Unit_Implementations.h"
#include "Synthesis_Zone_Implementations.h"
#include "Synthesis_Region_Implementation.h"
#include "PopSyn_Implementations.h"
#include "Person_Implementations.h"


