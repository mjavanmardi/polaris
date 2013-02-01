#pragma once
#include "User_Space_Includes.h"

namespace Network_Skimming_Components
{
	namespace Types
	{
		enum CALCULATION_STATUS
		{
			PROCESSING,
			COMPLETE
		};
	}

	namespace Concepts
	{
		concept struct Is_Movement_Plan
		{
			check_getter(has_trajectory, trajectory_container);
			check_getter(has_origin, origin);
			check_getter(has_destination, destination);
			check_getter(has_departed_time, departed_time);
			check_getter(has_arrived_time, arrived_time);

			define_default_check(has_trajectory && has_origin && has_destination && has_departed_time && has_arrived_time);
		};
		concept struct Is_Movement_Plan_Prototype
		{
			check_getter(has_trajectory, Component_Type::trajectory_container);
			check_getter(has_origin, Component_Type::origin);
			check_getter(has_destination, Component_Type::destination);
			check_getter(has_departed_time, Component_Type::departed_time);
			check_getter(has_arrived_time, Component_Type::arrived_time);

			define_default_check(has_trajectory && has_origin && has_destination && has_departed_time && has_arrived_time);
		};
	}
	
	namespace Prototypes
	{
		prototype struct Network_Skimming_Prototype
		{
			tag_as_prototype;

			//=============================================
			// Primary data accessors
			//---------------------------------------------
			// links back to the network to be skimmed
			feature_accessor(network_reference, none, none);
			// contains a map of skim_tables, one for each specified time slice
			feature_accessor(skim_tables_container, none, none);
			// reference to the current, completed skim table from skim_tables_container, based on simulation time, where skim_table
			feature_accessor(current_skim_table, none, none);
			// time increment at which skim tables are updated - set in the initializer
			feature_accessor(update_increment,check(ReturnValueType,Basic_Units::Concepts::Is_Time_Value),check(SetValueType,Basic_Units::Concepts::Is_Time_Value));


			//=============================================
			// Primary function accessors - used to pass through to the specific skimm table based on time-key
			//---------------------------------------------
			feature_prototype void Initialize()
			{
			}
			feature_prototype void Initialize(TargetType input_file)
			{
			}
			feature_prototype bool Update_Skims();

			//=============================================
			// Primary function accessors - used to pass through to the specific skimm table based on time-key
			//---------------------------------------------
			// This returns the travel time based on free-flow speed
			feature_prototype typename TargetType::ReturnType Get_LOS(typename TargetType::ParamType Origin_ID, typename TargetType::ParamType Destination_ID, typename TargetType::Param2Type Mode_Indicator, requires(check(typename TargetType::ReturnType, Basic_Units::Concepts::Is_Time_Value)))
			{
			}
			// This returns the travel time based on the current simulation time
			feature_prototype typename TargetType::ReturnType Get_Current_LOS(typename TargetType::ParamType Origin_ID, typename TargetType::ParamType Destination_ID, typename TargetType::Param2Type Mode_Indicator, requires(check(typename TargetType::ReturnType, Basic_Units::Concepts::Is_Time_Value)))
			{
			}
			// This returns the travel time during a specific time interval
			feature_prototype typename TargetType::ReturnType Get_LOS(typename TargetType::ParamType Origin_ID, typename TargetType::ParamType Destination_ID, typename TargetType::Param2Type Mode_Indicator, typename TargetType::Param3Type Start_Time, requires(check(typename TargetType::ReturnType, Basic_Units::Concepts::Is_Time_Value)))
			{
				// example fo a possible function to direct to appropriate skim_table (needs to be updated with interfaces, etc.
				//this->skim_tables_container<skim_tables_container::iterator&>().find(Intervalized_Start_Time)->second->Get_LOS<TargetType>(Origin_ID, Destination_ID,Mode_Indicator);
			}
			//=============================================
			// Primary events
			//---------------------------------------------
			declare_feature_conditional(Skim_Table_Update_Conditional)
			{
			}
			declare_feature_event(Update_Skims_Event);

		};

		prototype struct Skim_Table_Prototype
		{
			tag_as_prototype;

			// Associative Container of skim matrices, keyed on Mode Indicator values
			feature_accessor(skim_tables_container,none,none);
			// links back to the network to be skimmed
			feature_accessor(network_reference, none, none);

			//=============================================
			// Primary function accessors - used to calculate/return LOS values for OD pairs
			//---------------------------------------------
			feature_prototype void Initialize();
			feature_prototype bool Update_LOS()
			{
				// For each Mode :
					// For each Origin :
						// For each Destination :
							// Do routing for Origin to Destination
							// Get travel time for route
							// Store travel time in Skim_Table [Origin.Index , Destination.Index]
			}
			feature_prototype typename TargetType::ReturnType Get_LOS(typename TargetType::ParamType Origin_ID, typename TargetType::ParamType Destination_ID, typename TargetType::Param2Type Mode_Indicator)
			{

			}
		};
	}
}
