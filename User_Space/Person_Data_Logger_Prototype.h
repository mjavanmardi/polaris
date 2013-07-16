#pragma once

#include "User_Space_Includes.h"
#include "Person_Prototype.h"
#include "Activity_Prototype.h"


namespace Person_Components
{

namespace Concepts
{
}

namespace Types
{
}

namespace Prototypes
{
	prototype struct Person_Data_Logger ADD_DEBUG_INFO
	{
		tag_as_prototype;
		feature_prototype void Add_Record(TargetType act_record, bool is_executed, requires(check_as_given(TargetType,is_pointer) /*&& check(TargetType,Activity_Components::Concepts::Is_Activity_Plan_Prototype)*/))
		{
			this_component()->Add_Record<ComponentType,CallerType,TargetType>(act_record,is_executed);
		}
		feature_prototype void Add_Record(TargetType act_record, bool is_executed, requires(!check_as_given(TargetType,is_pointer)/* || !check(TargetType,Activity_Components::Concepts::Is_Activity_Plan_Prototype)*/))
		{
			assert_check(TargetType,is_pointer,"Target type is not a pointer");
			//assert_check(TargetType,Activity_Components::Concepts::Is_Activity_Plan_Prototype,"Target type is not an activity plan prototype");
		}
		feature_prototype void Write_Data_To_File()
		{
			this_component()->Write_Data_To_File<ComponentType,CallerType,TargetType>();
		}
		feature_prototype void Initialize()
		{
			this_component()->Initialize<ComponentType,CallerType,TargetType>();
		}

		feature_prototype void Increment_Cancelled_Activities()
		{
			this_component()->Increment_Cancelled_Activities<ComponentType,CallerType,TargetType>();
		}

		feature_accessor(Next_Logging_Time,none,none);
		feature_accessor(Logging_Interval,none,none);
	};
}

}