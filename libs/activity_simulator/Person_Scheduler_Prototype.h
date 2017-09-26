#ifndef PERSON_SCHEDULER_PROTOTYPE_H
#define PERSON_SCHEDULER_PROTOTYPE_H

#include "Activity_Simulator_Includes.h"
//#include "Activity_Prototype.h"


//---------------------------------------------------------
//	Person Scheduler - Handles Activity Scheduling for the Planner Class
//---------------------------------------------------------

namespace Person_Components
{
	namespace Types
	{
		enum CONFLICT_TYPES
		{
			OVERLAP_START=1,
			OVERLAP_END,
			INSERT,
			OVERLAP_ALL
		};
		enum RESOLUTION_TYPES
		{
			MODIFY_ORIGINAL = 1,
			MODIFY_CONFLICTING = 2,
			MODIFY_BOTH = 3,
			DELETE_ORIGINAL = 4
		};
	}

	namespace Concepts
	{
	}
	
	namespace Prototypes
	{
		prototype struct Person_Scheduler ADD_DEBUG_INFO
		{
			tag_as_prototype;

			typedef Person_Scheduler<Component_Type> This_Type;
			
			// accessor to parent class
			accessor(Parent_Person, NONE, NONE);
			// accessors to parent class members
			accessor(Activity_Container, NONE, NONE);
			accessor(Movement_Plans_Container, NONE, NONE);
			accessor(Activity_Count, NONE, NONE);

			template<typename TargetType> void Initialize();

			accessor(current_movement_plan, NONE, NONE);
			accessor(current_activity_plan, NONE, NONE);
			accessor(Current_Activity, NONE, NONE);
			
			template<typename ParamType, typename ReturnType> ReturnType next_activity_plan(ParamType current_time, requires(ParamType, check(strip_modifiers(ParamType), Is_Time_Value)));
			template<typename ParamType, typename ReturnType> ReturnType next_activity_plan(ParamType current_act, requires(ParamType, check(strip_modifiers(ParamType), Activity_Components::Concepts::Is_Activity_Plan)));
			template<typename ParamType, typename ReturnType> ReturnType next_activity_plan(ParamType value, requires(ParamType, !check(strip_modifiers(ParamType), Is_Time_Value) && !check(strip_modifiers(ParamType), Activity_Components::Concepts::Is_Activity_Plan)));
			template<typename TimeType, typename ReturnType> ReturnType previous_activity_plan(TimeType current_time, requires(TimeType, check(TimeType, Is_Time_Value))) /*&& check(ReturnType,Activity_Components::Concepts::Is_Activity_Plan_Prototype)*/;
			template<typename TimeType, typename ReturnType> ReturnType previous_activity_plan(TimeType current_time, requires(TimeType, !check(TimeType, Is_Time_Value))) /*&& check(ReturnType,Activity_Components::Concepts::Is_Activity_Plan_Prototype)*/;
			template<typename TargetType, typename ReturnType> ReturnType next_location(TargetType current_activity, requires(TargetType, check(TargetType, is_pointer))) /*&& check(ReturnType,Activity_Components::Concepts::Is_Activity_Plan_Prototype)*/;
			template<typename TargetType, typename ReturnType> ReturnType previous_location(TargetType current_activity, requires(TargetType, check(TargetType, is_pointer))) /*&& check(ReturnType,Activity_Components::Concepts::Is_Activity_Plan_Prototype)*/;
			template<typename ActivityPtrType> void Update_Current_Activity(ActivityPtrType current_act);
			template<typename TargetType> bool Resolve_Timing_Conflict(TargetType current_activity, bool update_movement_plans = false);
			template<typename TargetType> void Add_Movement_Plan(TargetType movement_plan);
			template<typename TargetType> void Remove_Movement_Plan(TargetType movement_plan);
			template<typename TargetType> void Add_Activity_Plan(TargetType activity_plan);
			template<typename TargetType> void Remove_Activity_Plan(TargetType activity_plan, bool delete_activity=true);
			template<typename TargetType> void Sort_Activity_Schedule();
			template<typename TimeType> float Percent_Free_Time_In_Schedule(TimeType range_start, TimeType range_end);
		};

		template<typename ComponentType>
		template<typename TargetType>
		void Person_Scheduler<ComponentType>::Initialize()
		{
			this_component()->Initialize<TargetType>();
		}

		template<typename ComponentType>
		template<typename ParamType, typename ReturnType>
		ReturnType Person_Scheduler<ComponentType>::next_activity_plan(ParamType current_time, requires(ParamType, check(strip_modifiers(ParamType), Is_Time_Value)))
		{
			return this_component()->next_activity_plan<ParamType, ReturnType>(current_time);
		}

		template<typename ComponentType>
		template<typename ParamType, typename ReturnType>
		ReturnType Person_Scheduler<ComponentType>::next_activity_plan(ParamType current_act, requires(ParamType, check(strip_modifiers(ParamType), Activity_Components::Concepts::Is_Activity_Plan)))
		{
			return this_component()->next_activity_plan<ParamType, ReturnType>(current_act);
		}

		template<typename ComponentType>
		template<typename ParamType, typename ReturnType>
		ReturnType Person_Scheduler<ComponentType>::next_activity_plan(ParamType value, requires(ParamType, !check(strip_modifiers(ParamType), Is_Time_Value) && !check(strip_modifiers(ParamType), Activity_Components::Concepts::Is_Activity_Plan)))
		{
			assert_check(strip_modifiers(ParamType), Is_Time_Value, "target type must be either Time_Value type or");
			assert_check(strip_modifiers(ParamType), Activity_Components::Concepts::Is_Activity_Plan_Prototype, "must be an Activity_Plan prototype");
		}

		template<typename ComponentType>
		template<typename TimeType, typename ReturnType>
		ReturnType Person_Scheduler<ComponentType>::previous_activity_plan(TimeType current_time, requires(TimeType, check(TimeType, Is_Time_Value))) /*&& check(ReturnType,Activity_Components::Concepts::Is_Activity_Plan_Prototype)*/
		{
			return this_component()->previous_activity_plan<TimeType, ReturnType>(current_time);
		}

		template<typename ComponentType>
		template<typename TimeType, typename ReturnType>
		ReturnType Person_Scheduler<ComponentType>::previous_activity_plan(TimeType current_time, requires(TimeType, !check(TimeType, Is_Time_Value))) /*&& check(ReturnType,Activity_Components::Concepts::Is_Activity_Plan_Prototype)*/
		{
			assert_check(TimeType, Is_Time_Value, "TimeType is not a valid Time_Value type.");
		}

		template<typename ComponentType>
		template<typename TargetType, typename ReturnType>
		ReturnType Person_Scheduler<ComponentType>::next_location(TargetType current_activity, requires(TargetType, check(TargetType, is_pointer))) /*&& check(ReturnType,Activity_Components::Concepts::Is_Activity_Plan_Prototype)*/
		{
			return this_component()->next_location<TargetType, ReturnType>(current_activity);
		}

		template<typename ComponentType>
		template<typename TargetType, typename ReturnType>
		ReturnType Person_Scheduler<ComponentType>::previous_location(TargetType current_activity, requires(TargetType, check(TargetType, is_pointer))) /*&& check(ReturnType,Activity_Components::Concepts::Is_Activity_Plan_Prototype)*/
		{
			return this_component()->previous_location<TargetType, ReturnType>(current_activity);
		}

		template<typename ComponentType>
		template<typename ActivityPtrType>
		void Person_Scheduler<ComponentType>::Update_Current_Activity(ActivityPtrType current_act)
		{
			this_component()->Update_Current_Activity(current_act);
		}

		template<typename ComponentType>
		template<typename TargetType>
		bool Person_Scheduler<ComponentType>::Resolve_Timing_Conflict(TargetType current_activity, bool update_movement_plans = false)
		{
			return this_component()->Resolve_Timing_Conflict<TargetType>(current_activity, update_movement_plans);
		}

		template<typename ComponentType>
		template<typename TargetType>
		void Person_Scheduler<ComponentType>::Add_Movement_Plan(TargetType movement_plan)
		{
			this_component()->template Add_Movement_Plan<TargetType>(movement_plan);
		}

		template<typename ComponentType>
		template<typename TargetType>
		void Person_Scheduler<ComponentType>::Remove_Movement_Plan(TargetType movement_plan)
		{
			this_component()->template Remove_Movement_Plan<TargetType>(movement_plan);
		}

		template<typename ComponentType>
		template<typename TargetType>
		void Person_Scheduler<ComponentType>::Add_Activity_Plan(TargetType activity_plan)
		{
			this_component()->template Add_Activity_Plan<TargetType>(activity_plan);
		}

		template<typename ComponentType>
		template<typename TargetType>
		void Person_Scheduler<ComponentType>::Remove_Activity_Plan(TargetType activity_plan, bool delete_activity = true)
		{
			this_component()->template Remove_Activity_Plan<TargetType>(activity_plan, delete_activity);
		}

		template<typename ComponentType>
		template<typename TargetType>
		void Person_Scheduler<ComponentType>::Sort_Activity_Schedule()
		{
			this_component()->Sort_Activity_Schedule<TargetType>();
		}

		template<typename ComponentType>
		template<typename TimeType>
		float Person_Scheduler<ComponentType>::Percent_Free_Time_In_Schedule(TimeType range_start, TimeType range_end)
		{
			return this_component()->Percent_Free_Time_In_Schedule(range_start, range_end);
		}
	}

}

#endif PERSON_SCHEDULER_PROTOTYPE_H