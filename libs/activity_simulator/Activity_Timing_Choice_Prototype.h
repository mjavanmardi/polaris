#pragma once
#include "User_Space_Includes.h"
#include "Person_Prototype.h"
#include "Activity_Prototype.h"

namespace Person_Components
{
	namespace Concepts
	{
	}
	
	namespace Prototypes
	{
		prototype struct Activity_Timing_Chooser ADD_DEBUG_INFO
		{
			accessor(Parent_Planner, NONE, NONE/*check(strip_modifiers(TargetType),Concepts::Is_Person_Planner_Prototype),check(strip_modifiers(TargetType),Concepts::Is_Person_Planner_Prototype)*/);


			//template<typename TargetType> void Plan_Activity_Start_Time(TargetType activity_ref, requires(TargetType,check(strip_modifiers(TargetType), Activity_Components::Concepts::Is_Activity_Plan_Prototype)))
			//{
//TODO
//			//	Time_Seconds start = this->Get_Start_Time<Target_Type<NT,Time_Seconds,TargetType>>(activity_ref);
			//	Activity_Components::Prototypes::Activity_Planner<typename TargetType::Component_Type>* act = (Activity_Components::Prototypes::Activity_Planner<typename TargetType::Component_Type>*)activity_ref;
			//	act->Start_Time<Time_Seconds>(start);
			//};
			//template<typename TargetType> void Plan_Activity_Duration(TargetType activity_ref, requires(TargetType,check(strip_modifiers(TargetType), Activity_Components::Concepts::Is_Activity_Plan_Prototype)))
			//{
//TODO
//			//	Time_Seconds dur = this->Get_Duration<Target_Type<NT,Time_Seconds,TargetType>>(activity_ref);
			//	Activity_Components::Prototypes::Activity_Planner<typename TargetType::Component_Type>* act = (Activity_Components::Prototypes::Activity_Planner<typename TargetType::Component_Type>*)activity_ref;
			//	act->Duration<Time_Seconds>(dur);
			//};
			//template<typename TargetType> void Plan_Activity_Start_Time_and_Duration(TargetType activity_ref, requires(TargetType,check(strip_modifiers(TargetType), Activity_Components::Concepts::Is_Activity_Plan_Prototype)))
			//{
//TODO
//			//	pair<Time_Seconds,Time_Seconds> start_dur = this->Get_Start_Time_and_Duration<Target_Type<NT,Time_Seconds,TargetType>>(activity_ref);
			//	Activity_Components::Prototypes::Activity_Planner<typename TargetType::Component_Type>* act = (Activity_Components::Prototypes::Activity_Planner<typename TargetType::Component_Type>*)activity_ref;
			//	act->Start_Time<Time_Seconds>(start_dur.first);
			//	act->Duration<Time_Seconds>(start_dur.second);
			//};


			template<typename TargetType> typename TargetType::ReturnType Get_Start_Time(typename TargetType::ParamType activity_ref, requires(TargetType,check(typename TargetType::ParamType, Activity_Components::Concepts::Is_Activity_Plan_Prototype) && check(typename TargetType::ReturnType,Is_Time_Value)))
			{
				this_component()->template Get_Start_Time<TargetType>();
			}
			template<typename TargetType> typename TargetType::ReturnType Get_Duration(typename TargetType::ParamType activity_ref, requires(TargetType,check(typename TargetType::ParamType, Activity_Components::Concepts::Is_Activity_Plan_Prototype) && check(typename TargetType::ReturnType,Is_Time_Value)))
			{
				this_component()->template Get_Duration<TargetType>();
			}
			template<typename TargetType> pair<typename TargetType::ReturnType,typename TargetType::ReturnType> Get_Start_Time_and_Duration(typename TargetType::ParamType activity_ref, requires(TargetType,check(typename TargetType::ParamType, Activity_Components::Concepts::Is_Activity_Plan_Prototype) && check(typename TargetType::ReturnType,Is_Time_Value)))
			{
				return this_component()->template Get_Start_Time_and_Duration<TargetType>(activity_ref);
			}
		};
	}
}
