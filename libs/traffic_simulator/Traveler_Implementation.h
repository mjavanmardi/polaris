#pragma once
#include "Traveler_Prototype.h"

namespace Traveler_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{
		implementation struct Traveler_Implementation:public Polaris_Component<MasterType,INHERIT(Traveler_Implementation),Execution_Object>
		{
			m_data(int, uuid, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(int, internal_id, NONE, NONE);

			m_prototype(Null_Prototype<typename MasterType::routing_type>, router, NONE, NONE);
			m_prototype(Null_Prototype<typename MasterType::vehicle_type>, vehicle, NONE, NONE);
			m_prototype(Null_Prototype<typename MasterType::plan_type>, plan, NONE, NONE);
		};
	}

}
