#pragma once

#include "Person_Mover_Prototype.h"

namespace Person_Components
{
	namespace Implementations
	{
		//==================================================================================
		/// Person Agent classes
		//----------------------------------------------------------------------------------
		implementation struct Person_Mover_Implementation : public Polaris_Component<MasterType,INHERIT(Person_Mover_Implementation),Execution_Object>
		{
			// Tag as Implementation
			typedef typename Polaris_Component<MasterType,INHERIT(Person_Mover_Implementation),Execution_Object>::Component_Type ComponentType;
			typedef typename MasterType::base_network_event_type base_network_event_type;
			typedef typename MasterType::weather_network_event_type weather_network_event_type;
			typedef typename MasterType::accident_network_event_type accident_network_event_type;

			Person_Mover_Implementation()
			{
				_Movement_Scheduled=false;
				_Artificial_Movement_Scheduled=false;
				_Replanning_Needed=false;
				_Is_Moving = false;
			}

			m_prototype(Prototypes::Person,typename MasterType::person_type,Parent_Person, NONE, NONE);
			m_prototype(Movement_Plan_Components::Prototypes::Movement_Plan,typename MasterType::movement_plan_type,Movement, NONE, NONE);
			m_data(bool, Movement_Scheduled, NONE, NONE);
			m_data(bool, Is_Moving, NONE, NONE);
			m_data(bool, Artificial_Movement_Scheduled, NONE, NONE);
			m_data(Simulation_Timestep_Increment, Artificial_Arrival_Time, NONE, NONE);
			m_data(bool, Replanning_Needed, NONE, NONE);
		};

	}
}
