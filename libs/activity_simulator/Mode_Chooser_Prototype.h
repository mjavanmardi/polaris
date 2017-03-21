#pragma once
#include "Activity_Simulator_Includes.h"
//#include "Person_Prototype.h"
//#include "Activity_Prototype.h"


namespace Person_Components
{
namespace Prototypes
{
	prototype struct Mode_Chooser ADD_DEBUG_INFO
	{
		tag_as_prototype;

		// accessor to parent class
		accessor(Parent_Planner, NONE, NONE);
		accessor(choice_set_size, NONE, NONE);
		accessor(choice_model, NONE, NONE);	
		accessor(previous_activity, NONE, NONE);
		accessor(current_activity, NONE, NONE);
		accessor(next_activity, NONE, NONE);
		accessor(previous_location, NONE, NONE);
		accessor(destination, NONE, NONE);
		accessor(next_location, NONE, NONE);
		accessor(los, NONE, NONE);

		accessor(auto_available, NONE, NONE);
		accessor(to_work_school, NONE, NONE);
		accessor(from_work_school, NONE, NONE);
		accessor(to_CBD, NONE, NONE);
		accessor(home_based, NONE, NONE);

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


	prototype struct Mode_Choice_Option : public Choice_Model_Components::Prototypes::Choice_Option<ComponentType>
	{
		tag_as_prototype;

		// accessor to parent class
		accessor(Parent_Planner, NONE, NONE);
		accessor(Mode_Chooser, NONE, NONE);
		accessor(mode_type, NONE, NONE);

		void copy(Mode_Choice_Option<ComponentType>* obj)
		{
			this_component()->copy((ComponentType*)obj);
		}
	};
	
}
}
