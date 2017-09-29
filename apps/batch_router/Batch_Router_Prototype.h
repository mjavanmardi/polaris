#pragma once
#include "Traffic_Simulator_Includes.h"
#include "Activity_Location_Prototype.h"
#include "Traffic_Simulator_Concepts.h"

static vector<stringstream> results_by_thread;
static vector<stringstream> summary_by_thread;
static vector<stringstream> details_by_thread;
static vector<int> object_count_by_thread;
static File_IO::File_Writer fw_output;
static File_IO::File_Writer fw_mm_sp_summary;
static File_IO::File_Writer fw_mm_sp_details;

namespace Batch_Router_Components
{
	namespace Types
	{
		enum CALCULATION_STATUS
		{
			PROCESSING,
			COMPLETE
		};
		enum SUB_ITERATIONS
		{
			INITIALIZE = 19,
			PROCESS = 20,
			PATH_BUILDING = 21,
			UPDATE = 22,
			GET_RESULTS=23
		};
	}

	namespace Prototypes
	{

		prototype struct Routed_Trip ADD_DEBUG_INFO
		{
			tag_as_prototype;

			//=============================================
			// Primary data accessors
			//---------------------------------------------
			// links back to the network to be skimmed
			accessor(network_reference,NONE,NONE);
			accessor(router,NONE,NONE);
			accessor(origin,NONE,NONE);
			accessor(destination,NONE,NONE);
			accessor(mode,NONE,NONE);
			accessor(departure_time,NONE,NONE);
			accessor(result_travel_time,NONE,NONE);
			accessor(result_distance,NONE,NONE);
			accessor(result_trajectory,NONE,NONE);
			accessor(link_failure,NONE,NONE);
			accessor(turn_failure,NONE,NONE);

			//=============================================
			// Primary function accessors - used to pass through to the specific skimm table based on time-key
			//---------------------------------------------
			template<typename NetworkType, typename LocationType, typename TimeType> void Initialize(string mode, int Trip_Id, NetworkType network_ref, LocationType orig, LocationType dest, TimeType depart, requires(LocationType,check(LocationType, is_pointer) && check(strip_modifiers(LocationType), Activity_Location_Components::Concepts::Is_Activity_Location_Prototype)))
			{
				this_component()->Initialize(mode, Trip_Id, network_ref, orig, dest, depart);
			}			
			template<typename NetworkType, typename LocationType, typename TimeType> void Initialize(string mode, int Trip_Id, NetworkType network, LocationType orig, LocationType dest, TimeType depart, requires(LocationType,!check(LocationType, is_pointer) || !check(strip_modifiers(LocationType), Activity_Location_Components::Concepts::Is_Activity_Location_Prototype)))
			{
				assert_check(TargetType, is_pointer,"TargetType is not a pointer" );
				assert_check(strip_modifiers(TargetType), Activity_Location_Components::Concepts::Is_Activity_Location_Prototype, "TargetType is not a valid location interface");
			}			
			
			template<typename TargetType> void Get_Results()
			{
				this_component()->Get_Results<TargetType>();
			}
		};

	}
}

