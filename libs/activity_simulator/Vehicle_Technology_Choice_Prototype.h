#pragma once
#include "Vehicle_Chooser_Prototype.h"



namespace Household_Components
{
	namespace Prototypes
	{
		prototype struct Vehicle_Technology_Chooser ADD_DEBUG_INFO
		{
			tag_as_prototype;

			// accessor to parent class
			accessor(Parent_Household, NONE, NONE);

			template<typename T> void Initialize(T household)
			{
				this_component()->Initialize<T>(household);
			}

			template<typename VehicleItfType, typename PersonItfType> void Select_Vehicle_Technology(VehicleItfType* veh, PersonItfType* person)
			{
				this_component()->Select_Vehicle_Technology(veh, person);
			}
		};
	}
}
