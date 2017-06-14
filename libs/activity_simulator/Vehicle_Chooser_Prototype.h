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
			accessor(Vehicle_Technology_Chooser, NONE, NONE);

			template<typename T> void Initialize(T Household)
			{
				this_component()->template Initialize<T>(Household);
			}

			template<typename T> void Select_Vehicles(T census_zone)
			{
				this_component()->template Select_Vehicles<T>(census_zone);
			}
		};

	}
}
