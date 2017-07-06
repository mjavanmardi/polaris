#pragma once
#include "../core/Forms.h"
namespace Activity_Location_Components {
	namespace Prototypes
	{
		template<typename ComponentType> struct Activity_Location;
	}
};
namespace Demand_Components {
	namespace Prototypes
	{
		template<typename ComponentType> struct Demand;
	}
};
namespace Intersection_Control_Components {
	namespace Prototypes
	{
		template<typename ComponentType> struct Phase_Movement;
	}
};
namespace Intersection_Control_Components {
	namespace Prototypes
	{
		template<typename ComponentType> struct Phase;
	}
};
namespace Intersection_Control_Components {
	namespace Prototypes
	{
		template<typename ComponentType> struct Approach;
	}
};
namespace Intersection_Control_Components {
	namespace Prototypes
	{
		template<typename ComponentType> struct Control_Plan;
	}
};
namespace Intersection_Control_Components {
	namespace Prototypes
	{
		template<typename ComponentType> struct Intersection_Control;
	}
};
namespace Ramp_Metering_Components {
	namespace Prototypes
	{
		template<typename ComponentType> struct Ramp_Metering;
	}
};
namespace Intersection_Components {
	namespace Prototypes
	{
		template<typename ComponentType> struct Outbound_Inbound_Movements;
	}
};
namespace Intersection_Components {
	namespace Prototypes
	{
		template<typename ComponentType> struct Inbound_Outbound_Movements;
	}
};
namespace Intersection_Components {
	namespace Prototypes
	{
		template<typename ComponentType> struct Intersection;
	}
};
namespace Link_Components {
	namespace Prototypes
	{
		template<typename ComponentType> struct Link;
	}
};
namespace Location_Components {
	namespace Prototypes
	{
		template<typename ComponentType> struct Location;
	}
};
namespace Movement_Plan_Components {
	namespace Prototypes
	{
		template<typename ComponentType> struct Trajectory_Unit;
	}
};
namespace Movement_Plan_Components {
	namespace Prototypes
	{
		template<typename ComponentType> struct Movement_Plan;
	}
};
namespace Network_Components {
	namespace Prototypes
	{
		template<typename ComponentType> struct Network;
	}
};
namespace Network_Components {
	namespace Prototypes
	{
		template<typename ComponentType> struct Network_DB_Reader;
	}
};
namespace Operation_Components {
	namespace Prototypes
	{
		template<typename ComponentType> struct Operation;
	}
};
namespace Routing_Components {
	namespace Prototypes
	{
		template<typename ComponentType> struct Routing;
	}
};
namespace Traveler_Components {
	namespace Prototypes
	{
		template<typename ComponentType> struct Traveler;
	}
};
namespace Turn_Movement_Components {
	namespace Prototypes
	{
		template<typename ComponentType> struct Movement;
	}
};
namespace Scenario_Components {
	namespace Prototypes
	{
		template<typename ComponentType> struct Scenario;
	}
};
namespace Vehicle_Components {
	namespace Types
	{
		
	}

	namespace Prototypes
	{
		template<typename ComponentType> struct Vehicle;
	}
};
namespace Zone_Components {
	namespace Prototypes
	{
		template<typename ComponentType> struct Zone;
	}
};

namespace PopSyn {
	namespace Prototypes
	{
		template<typename ComponentType> struct Population_Synthesizer;
		template<typename ComponentType> struct Synthesis_Zone;
		template<typename ComponentType> struct Synthesis_Region;
		template<typename ComponentType> struct Solver_Settings;
	}
};

namespace Person_Components {
	namespace Prototypes
	{
		template<typename ComponentType> struct Activity_Generator;
		template<typename ComponentType> struct Activity_Timing_Chooser;
		template<typename ComponentType> struct Destination_Chooser;
		template<typename ComponentType> struct Destination_Choice_Option;
		template<typename ComponentType> struct Mode_Chooser;
		template<typename ComponentType> struct Mode_Choice_Option;
		template<typename ComponentType> struct Person_Planner;
		template<typename ComponentType> struct Person_Properties;
		template<typename ComponentType> struct Person_Scheduler;
		template<typename ComponentType> struct Person_Mover;
		template<typename ComponentType> struct Person_Data_Logger;
		template<typename ComponentType> struct Person;
	}
};

namespace Household_Components {
	namespace Prototypes
	{
		template<typename ComponentType> struct Household;
		template<typename ComponentType> struct Household_Properties;
	}
};

namespace Plan_Components {
	namespace Prototypes
	{
		template<typename ComponentType> struct Plan;
	}
};
namespace Activity_Components {
	namespace Prototypes
	{
		template<typename ComponentType> struct Activity_Planner;
	}
};
namespace Network_Skimming_Components {
	namespace Prototypes
	{
		template<typename ComponentType> struct Network_Skimming;
	}
};
namespace Network_Skimming_Components {
	namespace Prototypes
	{
		template<typename ComponentType> struct Network_DB_Reader;
	}
};
namespace Advisory_ITS_Components
{
	namespace Prototypes
	{
		template<typename ComponentType> struct Advisory_ITS;
	}
};
namespace Depot_Components
{
	namespace Prototypes
	{
		template<typename ComponentType> struct Depot;
	}
};
namespace Network_Components
{
	namespace Prototypes
	{
		template<typename ComponentType> struct Network_DB_Reader;
	}
};
namespace Network_Event_Components
{
	namespace Prototypes
	{
		template<typename ComponentType> struct Network_Event;
		template<typename ComponentType> struct Network_Event_Manager;
	}
};
namespace Sensor_Components
{
	namespace Prototypes
	{
		template<typename ComponentType> struct Sensor;
	}
};
namespace Traffic_Management_Center_Components
{
	namespace Prototypes
	{
		template<typename ComponentType> struct Traffic_Management_Center;
	}
};
namespace Network_Components
{
	namespace Prototypes
	{
		template<typename ComponentType> struct Network_DB_Reader;
	}
};
namespace Analyze_Link_Group_Components
{
	namespace Prototypes
	{
		template<typename ComponentType> struct Analyze_Link_Group;
	}
};
