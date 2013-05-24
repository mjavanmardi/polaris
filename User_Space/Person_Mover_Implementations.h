#pragma once

#include "Person_Mover_Prototype.h"

namespace Person_Components
{
	namespace Implementations
	{
		//==================================================================================
		/// Person Agent classes
		//----------------------------------------------------------------------------------
		implementation struct Person_Mover_Implementation : public Polaris_Component<APPEND_CHILD(Person_Mover_Implementation),MasterType,Execution_Object,ParentType>
		{
			// Tag as Implementation
			typedef typename Polaris_Component<APPEND_CHILD(Person_Mover_Implementation),MasterType,Execution_Object>::Component_Type ComponentType;
			typedef typename MasterType::base_network_event_type base_network_event_type;
			typedef typename MasterType::weather_network_event_type weather_network_event_type;
			typedef typename MasterType::accident_network_event_type accident_network_event_type;

			member_prototype(Prototypes::Person,Parent_Person,typename MasterType::person_type,none,none);
			member_prototype(Movement_Plan_Components::Prototypes::Movement_Plan_Prototype,Movement,typename MasterType::movement_plan_type,none,none);
			member_data(bool, Movement_Scheduled, check(ReturnValueType,is_integral), check(SetValueType,is_integral));
			member_data(bool, Replanning_Needed, check(ReturnValueType,is_integral), check(SetValueType,is_integral));
		};

	}
}