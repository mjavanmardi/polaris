#pragma once
#include "User_Space_Includes.h"

namespace Activity_Components
{
	namespace Types
	{

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
		prototype struct Activity_Plan_Prototype
		{
			feature_prototype void Initialize()
			{
				this_component()->Initialize<ComponentType, ComponentType, TargetType>();
			}
			feature_prototype void Do_Activity_Planning()
			{
				cout <<endl<<"Doing Activity Planning for Activity "<<this->Activity_Plan_ID<int>();
			}

			feature_accessor(Activity_Plan_ID, check(ReturnValueType,is_arithmetic), check(SetValueType,is_arithmetic));
			feature_accessor(Activity_Type,none,none);
			feature_accessor(Activity_Plan_Horizon,none,none);
			feature_accessor(planning_time,none,none);

			feature_accessor(Location_Type, none, none);
			feature_accessor(Location_ID, check(ReturnValueType,is_arithmetic), check(SetValueType,is_arithmetic));
			feature_accessor(Location_Plan_Horizon, none, none);
			feature_accessor(Location_Flexibility, none, none);

			feature_accessor(Mode_Type, none, none);
			feature_accessor(Mode_ID, none, none);
			feature_accessor(Mode_Plan_Horizon, none, none);
			feature_accessor(Mode_Flexibility, none, none);
		};
	}
}

using namespace Activity_Components::Prototypes;