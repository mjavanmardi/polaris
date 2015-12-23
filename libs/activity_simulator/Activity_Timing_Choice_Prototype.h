#pragma once
#include "Activity_Simulator_Includes.h"
//#include "Person_Prototype.h"
//#include "Activity_Prototype.h"

namespace Person_Components
{
	namespace Concepts
	{
	}
	
	namespace Prototypes
	{
		prototype struct Activity_Timing_Chooser ADD_DEBUG_INFO
		{
			tag_as_prototype;

			accessor(Parent_Planner, NONE, NONE/*check(strip_modifiers(TargetType),Concepts::Is_Person_Planner_Prototype),check(strip_modifiers(TargetType),Concepts::Is_Person_Planner_Prototype)*/);


			//template<typename TargetType> void Plan_Activity_Start_Time(TargetType activity_ref, requires(TargetType,check(strip_modifiers(TargetType), Activity_Components::Concepts::Is_Activity_Plan_Prototype)))
			//{
			//	Time_Seconds start = this->Get_Start_Time<TargetType,Time_Seconds>(activity_ref);
			//	Activity_Components::Prototypes::Activity_Planner<typename TargetType::Component_Type>* act = (Activity_Components::Prototypes::Activity_Planner<typename TargetType::Component_Type>*)activity_ref;
			//	act->Start_Time<Time_Seconds>(start);
			//};
			//template<typename TargetType> void Plan_Activity_Duration(TargetType activity_ref, requires(TargetType,check(strip_modifiers(TargetType), Activity_Components::Concepts::Is_Activity_Plan_Prototype)))
			//{
			//	Time_Seconds dur = this->Get_Duration<TargetType,Time_Seconds>(activity_ref);
			//	Activity_Components::Prototypes::Activity_Planner<typename TargetType::Component_Type>* act = (Activity_Components::Prototypes::Activity_Planner<typename TargetType::Component_Type>*)activity_ref;
			//	act->Duration<Time_Seconds>(dur);
			//};
			//template<typename TargetType> void Plan_Activity_Start_Time_and_Duration(TargetType activity_ref, requires(TargetType,check(strip_modifiers(TargetType), Activity_Components::Concepts::Is_Activity_Plan_Prototype)))
			//{
			//	pair<Time_Seconds,Time_Seconds> start_dur = this->Get_Start_Time_and_Duration<TargetType,Time_Seconds>(activity_ref);
			//	Activity_Components::Prototypes::Activity_Planner<typename TargetType::Component_Type>* act = (Activity_Components::Prototypes::Activity_Planner<typename TargetType::Component_Type>*)activity_ref;
			//	act->Start_Time<Time_Seconds>(start_dur.first);
			//	act->Duration<Time_Seconds>(start_dur.second);
			//};


			template<typename ActivityRefType, typename ReturnType> ReturnType Get_Start_Time(ActivityRefType activity_ref, requires(ActivityRefType,check(strip_modifiers(ActivityRefType), Activity_Components::Concepts::Is_Activity_Plan_Prototype) && check(ReturnType,Is_Time_Value)))
			{
				return this_component()->template Get_Start_Time<ActivityRefType, ReturnType>();
			}
			template<typename ActivityRefType, typename ReturnType> ReturnType Get_Duration(ActivityRefType activity_ref, requires(ActivityRefType,check(strip_modifiers(ActivityRefType), Activity_Components::Concepts::Is_Activity_Plan_Prototype) && check(ReturnType,Is_Time_Value)))
			{
				return this_component()->template Get_Duration<ActivityRefType, ReturnType>();
			}
			template<typename ActivityRefType, typename ReturnType> pair<ReturnType,ReturnType> Get_Start_Time_and_Duration(ActivityRefType activity_ref, requires(ActivityRefType,check(strip_modifiers(ActivityRefType), Activity_Components::Concepts::Is_Activity_Plan_Prototype) && check(ReturnType,Is_Time_Value)))
			{
				return this_component()->template Get_Start_Time_and_Duration<ActivityRefType, ReturnType>(activity_ref);
			}
			template<typename ActivityRefType, typename ReturnType> pair<ReturnType,ReturnType> Get_Start_Time_and_Duration(ActivityRefType activity_ref, requires(ActivityRefType,!check(strip_modifiers(ActivityRefType), Activity_Components::Concepts::Is_Activity_Plan_Prototype) || !check(ReturnType,Is_Time_Value)))
			{
				assert_check(strip_modifiers(ActivityRefType), Activity_Components::Concepts::Is_Activity_Plan_Prototype,"ActivityRefType is not a valid activity type");
			}
		};
	}
}
