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

			template<typename T> void Select_Vehicles(T census_zone)
			{
				this_component()->template Select_Vehicles<T>(census_zone);
			}
		};


		//prototype struct Vehicle_Choice_Option ADD_DEBUG_INFO
		//{
		//	tag_as_prototype;

		//	// accessor to parent class
		//	accessor(Parent_Household, NONE, NONE);


		//	template<typename TargetType> TargetType Calculate_Utility()
		//	{
		//		return this_component()->template Calculate_Utility<TargetType>();
		//	}
		//	template<typename TargetType> TargetType Print_Utility()
		//	{
		//		return this_component()->template Print_Utility<TargetType>();
		//	}
		//};

	}
}
