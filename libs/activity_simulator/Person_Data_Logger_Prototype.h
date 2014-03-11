#pragma once

#include "Activity_Simulator_Includes.h"
//#include "Person_Prototype.h"
//#include "Activity_Prototype.h"


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
		template<typename TargetType> void Add_Record(TargetType act_record, bool is_executed, requires(TargetType,check(TargetType,is_pointer) /*&& check(strip_modifiers(TargetType),Activity_Components::Concepts::Is_Activity_Plan_Prototype)*/))
		{
			this_component()->Add_Record<TargetType>(act_record,is_executed);
		}
		template<typename TargetType> void Add_Record(TargetType act_record, bool is_executed, requires(TargetType,!check(TargetType,is_pointer)/* || !check(strip_modifiers(TargetType),Activity_Components::Concepts::Is_Activity_Plan_Prototype)*/))
		{
			assert_check(TargetType,is_pointer,"Target type is not a pointer");
			//assert_check(strip_modifiers(TargetType),Activity_Components::Concepts::Is_Activity_Plan_Prototype,"Target type is not an activity plan prototype");
		}
		template<typename TargetType> void Write_Data_To_File()
		{
			this_component()->Write_Data_To_File<TargetType>();
		}
		template<typename TargetType> void Initialize()
		{
			this_component()->Initialize<TargetType>();
		}

		template<typename TargetType> void Increment_Cancelled_Activities()
		{
			this_component()->Increment_Cancelled_Activities<TargetType>();
		}
		template<typename TargetType> void Increment_Replanned_Activities()
		{
			this_component()->Increment_Replanned_Activities<TargetType>();
		}

		accessor(Next_Logging_Time, NONE, NONE);
		accessor(Logging_Interval, NONE, NONE);
	};
}

}
