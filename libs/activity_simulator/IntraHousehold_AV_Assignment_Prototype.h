#pragma once
#include "Household_Prototype.h"

namespace Household_Components
{
	namespace Prototypes
	{
		prototype struct IntraHousehold_AV_Assignment ADD_DEBUG_INFO
		{
			tag_as_prototype;

			// accessor to parent class
			accessor(Parent_Household, NONE, NONE);


			template<typename T> void Initialize(T Household)
			{
				this_component()->template Initialize<T>(Household);
			}

			void Assign_Shared_Vehicles()
			{
				this_component()->Assign_Shared_Vehicles();
			}

			int Get_Max_Number_of_AVs()
			{
				this_component()->Get_Max_Number_of_AVs();
			}

			void Run_Gurobi_Model()
			{
				this_component()->Run_Gurobi_Model();
			}
		};
	}
}