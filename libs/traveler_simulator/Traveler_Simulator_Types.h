#pragma once

#include "core\core.h"
using namespace std;

//---------------------------------------------------------
//	Types for movement plan
//---------------------------------------------------------
namespace Movement_Plan_Components
{
	namespace Types
	{
		struct Load_To_Entry_Queue {};
		struct Load_To_Origin_Link {};
	}
}


namespace Demand_Components
{
	//==================================================================================================================
	/// TYPE Of interest to the Demand Prototype.
	/// Used in concept checking, component functions, etc.
	//------------------------------------------------------------------------------------------------------------------
	namespace Types
	{
		namespace ModelTypes
		{
			typedef true_type TransimsType;
		}
		namespace FileTypes
		{
			typedef true_type TripFile;
			typedef true_type ControlFile;
		}
		namespace FileFormatTypes
		{
			typedef true_type Tab_Delimited;
			typedef true_type CSV_Delimited;
			typedef true_type Binary;
		}

		struct Hard_Coded_Demand {};
		struct ODB_Demand {};
		struct File_Demand {};

	}
}

namespace Vehicle_Components
{
	namespace Types
	{
		enum Powertrain_Type_Keys
		{
			CONVENTIONAL = 0,
			HEV,
			PHEV,
			BEV,
			FCEV
		};
		enum Fuel_Type_Keys
		{
			GASOLINE = 0,
			DIESEL,
			CNG,
			H2,
			ELECTRIC
		};
		enum EPA_Vehicle_Class_Keys
		{
			CAR_MINI_COMPACT = 0,
			CAR_SUB_COMPACT,
			CAR_COMPACT,
			CAR_MID_SIZE,
			CAR_FULL_SIZE,
			WAGON_COMPACT,
			WAGON_MID_SIZE,
			WAGON_FULL_SIZE,
			SUV_MID_SIZE,
			SUV_FULL_SIZE,
			TRUCK_MID_SIZE,
			TRUCK_FULL_SIZE,
			VAN_MID_SIZE,
			VAN_FULL_SIZE
		};

		enum Vehicle_Type_Keys
		{
			SOV = 0,
			AUTO,
			HOV,
			TRUCK,
			BUS,
			RAIL,
			NONMOTORIZED,
			BICYCLE,
			WALK,
			TAXI,
			SCHOOLBUS,
			PARK_AND_RIDE,
			KISS_AND_RIDE
		};

		enum Route_Choice_Use_Class_Keys
		{
			UNRESPONSIVE = 0,
			ENROUTE_SWITCHING,
			USER_EQUILIBRIUM,
			SYSTEM_OPTIMAL,
		};

		enum Enroute_Information_Keys
		{
			NO_REALTIME_INFORMATION = 0,
			WITH_REALTIME_INFORMATION,
		};

		enum Vehicle_Status_Keys
		{
			UNLOADED = 0,
			IN_ENTRY_QUEUE,
			IN_NETWORK,
			OUT_NETWORK,
		};
		enum Type_Vehicle_Action_keys
		{
			MOVE_TO_NEXT_LINK = 0,
			MOVE_TO_ORIGIN_LINK,
			DO_NOTHING,
		};

		struct Load_To_Entry_Queue {};
		struct Load_To_Origin_Link {};
	}
}