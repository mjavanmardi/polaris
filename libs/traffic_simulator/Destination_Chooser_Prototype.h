#pragma once
#include "User_Space_Includes.h"
#include "Person_Prototype.h"
#include "Activity_Prototype.h"


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

		template<typename TargetType> typename TargetType::ReturnType Choose_Destination(typename TargetType::ParamType current_activity, boost::container::vector<typename TargetType::ReturnType>* destinations_to_use=nullptr, requires(TargetType,check(typename TargetType::ReturnType,is_pointer) && check(typename TargetType::ReturnType,Activity_Location_Components::Concepts::Is_Activity_Location)))
		{
			return this_component()->template Choose_Destination<TargetType>(current_activity, destinations_to_use);
		}
		template<typename TargetType> TargetType Choose_Routine_Destination(Activity_Components::Types::ACTIVITY_TYPES act_type, boost::container::vector<TargetType>* destinations_to_use=nullptr, requires(TargetType,check(strip_modifiers(TargetType),is_pointer) && check(strip_modifiers(TargetType),Activity_Location_Components::Concepts::Is_Activity_Location)))
		{
			return this_component()->template Choose_Routine_Destination<TargetType>(act_type, destinations_to_use);
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
