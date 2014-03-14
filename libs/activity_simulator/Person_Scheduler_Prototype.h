#pragma once

#include "Activity_Simulator_Includes.h"
//#include "Activity_Prototype.h"


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

			template<typename TargetType> void Initialize()
			{
				this_component()->Initialize<TargetType>();
			}

			accessor(current_movement_plan, NONE, NONE);
			accessor(current_activity_plan, NONE, NONE);
			
			template<typename ParamType, typename ReturnType> ReturnType next_activity_plan(ParamType current_time, requires(ParamType,check(strip_modifiers(ParamType),Is_Time_Value)))
			{
				return this_component()->next_activity_plan<ParamType, ReturnType>(current_time);
			}
			template<typename ParamType, typename ReturnType> ReturnType next_activity_plan(ParamType current_act, requires(ParamType,check(strip_modifiers(ParamType),Activity_Components::Concepts::Is_Activity_Plan)))
			{
				return this_component()->next_activity_plan<ParamType, ReturnType>(current_act);
			}
			template<typename ParamType, typename ReturnType> ReturnType next_activity_plan(ParamType value, requires(ParamType,!check(strip_modifiers(ParamType),Is_Time_Value) && !check(strip_modifiers(ParamType),Activity_Components::Concepts::Is_Activity_Plan)))
			{
				assert_check(strip_modifiers(ParamType),Is_Time_Value, "target type must be either Time_Value type or");
				assert_check(strip_modifiers(ParamType),Activity_Components::Concepts::Is_Activity_Plan_Prototype, "must be an Activity_Plan prototype");
			}
			
			template<typename TimeType, typename ReturnType> ReturnType previous_activity_plan(TimeType current_time, requires(TimeType, check(TimeType,Is_Time_Value))) /*&& check(ReturnType,Activity_Components::Concepts::Is_Activity_Plan_Prototype)*/
			{
				return this_component()->previous_activity_plan<TimeType,ReturnType>(current_time);
			}
			template<typename TimeType, typename ReturnType> ReturnType previous_activity_plan(TimeType current_time, requires(TimeType, !check(TimeType,Is_Time_Value))) /*&& check(ReturnType,Activity_Components::Concepts::Is_Activity_Plan_Prototype)*/
			{
				assert_check(TimeType,Is_Time_Value,"TimeType is not a valid Time_Value type.");
			}
			
			template<typename TargetType, typename ReturnType> ReturnType next_location(TargetType current_activity, requires(TargetType,check(TargetType,is_pointer))) /*&& check(ReturnType,Activity_Components::Concepts::Is_Activity_Plan_Prototype)*/
			{
				return this_component()->next_location<TargetType,ReturnType>(current_activity);
			}
			template<typename TargetType, typename ReturnType> ReturnType previous_location(TargetType current_activity, requires(TargetType,check(TargetType,is_pointer))) /*&& check(ReturnType,Activity_Components::Concepts::Is_Activity_Plan_Prototype)*/
			{
				return this_component()->previous_location<TargetType,ReturnType>(current_activity);
			}
			
			
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

			template<typename TargetType> void Sort_Activity_Schedule()
			{
				this_component()->Sort_Activity_Schedule<TargetType>();
			}
		};
	}

}
