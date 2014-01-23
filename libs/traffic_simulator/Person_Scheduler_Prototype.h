#pragma once

#include "User_Space_Includes.h"
//#include "Person_Prototype.h"


//---------------------------------------------------------
//	Person Scheduler - Handles Activity Scheduling for the Planner Class
//---------------------------------------------------------

namespace Person_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Prototypes
	{
		prototype struct Person_Scheduler ADD_DEBUG_INFO
		{
			tag_as_prototype;

			typedef Person_Scheduler<ComponentType> This_Type;
			
			// accessor to parent class
			accessor(Parent_Person, NONE, NONE);
			// accessors to parent class members
			accessor(Activity_Container, NONE, NONE);
			accessor(Movement_Plans_Container, NONE, NONE);
			accessor(Activity_Count, NONE, NONE);

			template<typename TargetType>_method_void(Initialize, NONE);

			accessor(current_movement_plan, NONE, NONE);
			accessor(current_activity_plan, NONE, NONE);
			
			template<typename TargetType> typename TargetType::ReturnType next_activity_plan(typename TargetType::ParamType current_time, requires(TargetType,check(typename TargetType::ParamType,Is_Time_Value)))
			{
				return this_component()->next_activity_plan<TargetType>(current_time);
			}
			template<typename TargetType> typename TargetType::ReturnType next_activity_plan(typename TargetType::ParamType current_act, requires(TargetType,check(typename TargetType::ParamType,Activity_Components::Concepts::Is_Activity_Plan_Prototype)))
			{
				return this_component()->next_activity_plan_by_act<typename TargetType::ParamType>(current_act);
			}
			template<typename TargetType> typename TargetType::ReturnType next_activity_plan(typename TargetType::ParamType value, requires(TargetType,!check(typename TargetType::ParamType,Is_Time_Value) && !check(typename TargetType::ParamType,Activity_Components::Concepts::Is_Activity_Plan_Prototype)))
			{
				assert_check(typename TargetType::ParamType,Is_Time_Value, "target type must be either Time_Value type or");
				assert_check(typename TargetType::ParamType,Activity_Components::Concepts::Is_Activity_Plan_Prototype, "must be an Activity_Plan prototype");
			}
			//feature_method_1_arg(next_activity_plan, current_time, check(typename TargetType::ParamType,Is_Time_Value) /*&& check(typename TargetType::ReturnType,Activity_Components::Concepts::Is_Activity_Plan_Prototype)*/);
			template<typename TargetType>_method_1_arg(previous_activity_plan, current_time, check(typename TargetType::ParamType,Is_Time_Value) /*&& check(typename TargetType::ReturnType,Activity_Components::Concepts::Is_Activity_Plan_Prototype)*/);
			template<typename TargetType>_method_1_arg(next_location, current_activity, check(typename TargetType::ParamType,is_pointer) /*&& check(typename TargetType::ReturnType,Activity_Components::Concepts::Is_Activity_Plan_Prototype)*/);
			template<typename TargetType>_method_1_arg(previous_location, current_activity, check(typename TargetType::ParamType,is_pointer) /*&& check(typename TargetType::ReturnType,Activity_Components::Concepts::Is_Activity_Plan_Prototype)*/);
			
			//feature_method_1_arg(Resolve_Timing_Conflict, current_activity, check_2(typename TargetType::ReturnType,bool,is_same));

			template<typename TargetType> bool Resolve_Timing_Conflict(TargetType current_activity, bool update_movement_plans=false)
			{
				return this_component()->Resolve_Timing_Conflict<TargetType>(current_activity, update_movement_plans);
			}
			template<typename TargetType> void Add_Movement_Plan(TargetType movement_plan)
			{
				this_component()->template Add_Movement_Plan<TargetType>(movement_plan);
			}
			template<typename TargetType> void Remove_Movement_Plan(TargetType movement_plan)
			{
				this_component()->template Remove_Movement_Plan<TargetType>(movement_plan);
			}
			template<typename TargetType> void Add_Activity_Plan(TargetType activity_plan)
			{
				this_component()->template Add_Activity_Plan<TargetType>(activity_plan);
			}
			template<typename TargetType> void Remove_Activity_Plan(TargetType activity_plan)
			{
				this_component()->template Remove_Activity_Plan<TargetType>(activity_plan);
			}

			template<typename TargetType>_method_void(Sort_Activity_Schedule, NONE);
		};
	}

}
