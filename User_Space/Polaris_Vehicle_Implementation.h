#pragma once
#include "Vehicle_Prototype.h"

namespace Vehicle_Components
{
	namespace Types
	{

	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{

		implementation struct Polaris_Vehicle_Implementation:public Polaris_Component_Class<Polaris_Vehicle_Implementation,MasterType,Data_Object,ParentType>
		{
			member_data(Vehicle_Components::Types::Vehicle_Status_Keys, simulation_status, none, none);
			member_data(int, uuid, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, internal_id, none, none);
			member_component(typename MasterType::movement_plan_type, movement_plan, none, none);
			member_component(typename MasterType::person_type, traveler, none, none);
			
			feature_implementation void load(requires(check_2(TargetType,Types::Load_To_Entry_Queue,is_same)))
			{
				_simulation_status = Types::Vehicle_Status_Keys::IN_ENTRY_QUEUE;
			}

			feature_implementation void load(requires(check_2(TargetType,Types::Load_To_Origin_Link,is_same)))
			{
				typedef Movement_Plan_Components::Prototypes::Movement_Plan_Prototype<typename MasterType::movement_plan_type,ComponentType> _Movement_Plan_Interface;
				_simulation_status =  Types::Vehicle_Status_Keys::IN_NETWORK;
				((_Movement_Plan_Interface*)_movement_plan)->template initialize_trajectory<NULLTYPE>();
			}

			feature_implementation void load(requires(!check_2(TargetType,Types::Load_To_Origin_Link,is_same) && !check_2(TargetType,Types::Load_To_Entry_Queue,is_same)))
			{
				static_assert(false,"Unrecognized load type!");
			}

			feature_implementation void unload()
			{
				_simulation_status = Types::Vehicle_Status_Keys::OUT_NETWORK;
			}

		};
	}

}