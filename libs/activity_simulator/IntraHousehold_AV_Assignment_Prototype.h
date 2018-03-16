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

			template<typename MovementPlanType> vector<MovementPlanType*>* Assign_Shared_Vehicles()
			{
				return this_component()->Assign_Shared_Vehicles<MovementPlanType>();
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