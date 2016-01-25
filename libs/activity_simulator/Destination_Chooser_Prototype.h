#pragma once
#include "Activity_Simulator_Includes.h"
//#include "Person_Prototype.h"
//#include "Activity_Prototype.h"


namespace Person_Components
{
namespace Prototypes
{
	prototype struct Destination_Chooser ADD_DEBUG_INFO
	{
		tag_as_prototype;

		// accessor to parent class
		accessor(Parent_Planner, NONE, NONE);
		accessor(choice_set_size, NONE, NONE);
		accessor(choice_model, NONE, NONE);	
		accessor(Current_Activity, NONE, NONE);

		template<typename ActivityRefType, typename ReturnType> ReturnType Choose_Destination(ActivityRefType current_activity, std::vector<ReturnType>* destinations_to_use=nullptr, requires(ActivityRefType,check(ActivityRefType,is_pointer) && check(strip_modifiers(ReturnType),Activity_Location_Components::Concepts::Is_Activity_Location)))
		{
			return this_component()->template Choose_Destination<ActivityRefType, ReturnType>(current_activity, destinations_to_use);
		}
		template<typename ActivityRefType, typename ReturnType> ReturnType Evaluate_Destination(ActivityRefType current_activity, std::vector<ReturnType>* destinations_to_use=nullptr, requires(ActivityRefType,check(ActivityRefType,is_pointer) && check(strip_modifiers(ReturnType),Activity_Location_Components::Concepts::Is_Activity_Location)))
		{
			return this_component()->template Evaluate_Destination<ActivityRefType, ReturnType>(current_activity, destinations_to_use);
		}
		template<typename ReturnType> ReturnType Choose_Routine_Destination(Activity_Components::Types::ACTIVITY_TYPES act_type, std::vector<ReturnType>* destinations_to_use=nullptr, requires(ReturnType,check(ReturnType,is_pointer) && check(strip_modifiers(ReturnType),Activity_Location_Components::Concepts::Is_Activity_Location)))
		{
			return this_component()->template Choose_Routine_Destination<ReturnType>(act_type, destinations_to_use);
		}
	};


	prototype struct Destination_Choice_Option ADD_DEBUG_INFO
	{
		tag_as_prototype;

		// accessor to parent class
		accessor(Parent_Planner, NONE, NONE);
		accessor(previous, NONE, NONE);
		accessor(destination, NONE, NONE);
		accessor(next, NONE, NONE);
		accessor(activity_type, NONE, NONE);
		accessor(bias_correction, NONE, NONE);
		accessor(start_time,NONE,NONE);

		template<typename TargetType> TargetType Calculate_Utility()
		{
			return this_component()->template Calculate_Utility<TargetType>();
		}
		template<typename TargetType> TargetType Print_Utility()
		{
			return this_component()->template Print_Utility<TargetType>();
		}
	};
	
}
}
