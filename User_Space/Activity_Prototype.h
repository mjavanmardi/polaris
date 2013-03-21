#pragma once
#include "User_Space_Includes.h"
#include "Activity_Location_Prototype.h"
#include "Vehicle_Prototype.h"

namespace Activity_Components
{
	namespace Types
	{
		enum FLEXIBILITY_VALUES
		{
			LOW_FLEXIBILITY,
			MODERATE_FLEXIBILITY,
			HIGH_FLEXIBILITY
		};
		enum PLAN_HORIZON_VALUES
		{
			ROUTINE,
			PREPLANNED_OVER_7_DAYS,
			PREPLANNED_2_TO_7_DAYS,
			PREPLANNED_PREVIOUS_DAY,
			PREPLANNED_SAME_DAY,
			IMPULSIVE
		};
		enum ACTIVITY_TYPES
		{
			WORK_AT_HOME_ACTIVITY,
			AT_HOME_ACTIVITY,
			PRIMARY_WORK_ACTIVITY,
			OTHER_WORK_ACTIVITY,
			SCHOOL_ACTIVITY,
			MAJOR_SHOPPING_ACTIVITY,
			OTHER_SHOPPING_ACTIVITY,
			SERVICE_VEHICLE_ACTIVITY,
			HEALTHCARE_ACTIVITY,
			PERSONAL_BUSINESS_ACTIVITY,
			ERRANDS_ACTIVITY,
			PICK_UP_OR_DROP_OFF_ACTIVITY,
			EAT_OUT_ACTIVITY,
			RELIGIOUS_OR_CIVIC_ACTIVITY,
			LEISURE_ACTIVITY,
			RECREATION_ACTIVITY,
			SOCIAL_ACTIVITY,
			CHANGE_TRANSPORTATION_ACTIVITY,
			OTHER_ACTIVITY
		};

	}

	namespace Concepts
	{
		concept struct Is_Activity_Plan
		{
			check_getter(has_id,Activity_Plan_ID);
			define_default_check(has_id);
		};
		concept struct Is_Activity_Plan_Prototype
		{
			check_getter(has_id,Component_Type::Activity_Plan_ID);
			define_default_check(has_id);
		};
	}
	
	namespace Prototypes
	{
		prototype struct Activity_Plan
		{
			feature_prototype void Initialize()
			{
				this_component()->Initialize<ComponentType, ComponentType, TargetType>();
			}
			feature_prototype void Do_Activity_Planning()
			{
				cout <<endl<<"Doing Activity Planning for Activity "<<this->Activity_Plan_ID<int>();
			}

			// Pointer back to planner
			feature_accessor(Parent_Planner, none, none);

			// Fundamental activity properties
			feature_accessor(Activity_Plan_ID, check(ReturnValueType,is_arithmetic), check(SetValueType,is_arithmetic));
			feature_accessor(Activity_Type,none,none);
			feature_accessor(Activity_Plan_Horizon,none,none);
			feature_accessor(Activity_Planning_Time, check(ReturnValueType,Basic_Units::Concepts::Is_Time_Value), check(SetValueType,Basic_Units::Concepts::Is_Time_Value))

			// Activity attribute planning properties		
			feature_accessor(Location_Plan_Horizon, none, none);
			feature_accessor(Location_Flexibility, none, none);
			feature_accessor(Mode_Plan_Horizon, none, none);
			feature_accessor(Mode_Flexibility, none, none);
			feature_accessor(Start_Time_Plan_Horizon,none,none);
			feature_accessor(Start_Time_Flexibility,none,none);
			feature_accessor(Duration_Plan_Horizon,none,none);
			feature_accessor(Duration_Flexibility,none,none);
			feature_accessor(Involved_Persons_Plan_Horizon,none,none);
			feature_accessor(Involved_Persons_Flexibility,none,none);

			// Activity attributes
			feature_accessor(Location, check(ReturnValueType,is_arithmetic), check(SetValueType,is_arithmetic));
			feature_accessor(Mode, none, none);
			feature_accessor(Start_Time, check(ReturnValueType,Basic_Units::Concepts::Is_Time_Value), check(SetValueType,Basic_Units::Concepts::Is_Time_Value)); 
			feature_accessor(Duration, check(ReturnValueType,Basic_Units::Concepts::Is_Time_Value), check(SetValueType,Basic_Units::Concepts::Is_Time_Value)); 
			feature_accessor(Involved_Persons_Container,none,none);
			feature_accessor(Expected_Travel_Time, check(ReturnValueType,Basic_Units::Concepts::Is_Time_Value), check(SetValueType,Basic_Units::Concepts::Is_Time_Value)); 
		};
	}
}

using namespace Activity_Components::Prototypes;