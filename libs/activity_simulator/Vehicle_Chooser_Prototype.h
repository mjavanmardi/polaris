#pragma once
#include "Activity_Simulator_Includes.h"
//#include "Person_Prototype.h"
//#include "Activity_Prototype.h"


namespace Household_Components
{
namespace Prototypes
{
	prototype struct Vehicle_Chooser ADD_DEBUG_INFO
	{
		tag_as_prototype;

		// accessor to parent class
		accessor(Parent_Household, NONE, NONE);
		accessor(choice_set_size, NONE, NONE);
		accessor(choice_model, NONE, NONE);	

		template<typename ActivityItfType, typename ReturnType> ReturnType Choose_Mode(ActivityItfType activity, requires(ActivityItfType,
			check(ActivityItfType, is_pointer) &&
			check_2(ReturnType,Vehicle_Components::Types::Vehicle_Type_Keys,is_same)))
		{
			return this_component()->template Choose_Mode<ActivityItfType, ReturnType>(activity);
		}
		template<typename ActivityItfType, typename ReturnType>ReturnType Choose_Mode(ActivityItfType activity, requires(ActivityItfType,
			!check(ActivityItfType, is_pointer) ||
			!check_2(ReturnType,Vehicle_Components::Types::Vehicle_Type_Keys,is_same)))
		{
			assert_check(ActivityItfType, is_pointer, "Error, must pass activity for mode choice as a pointer.");
			assert_check_2(ReturnType,Vehicle_Components::Types::Vehicle_Type_Keys,is_same, "Must request return value as a Vehicle_Type_Keys type");
		}
	};


	prototype struct Vehicle_Choice_Option ADD_DEBUG_INFO
	{
		tag_as_prototype;

		// accessor to parent class
		accessor(Parent_Household, NONE, NONE);


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
