#pragma once
#include "Plan_Prototype.h"

namespace Plan_Components
{
	namespace Types
	{

	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{

		implementation struct Plan_Implementation:public Polaris_Component<MasterType,INHERIT(Plan_Implementation),Data_Object>
		{
			m_prototype(Null_Prototype,typename MasterType::movement_plan_type, movement_plan, NONE, NONE);
			m_prototype(Null_Prototype,typename MasterType::traveler_type, traveler, NONE, NONE);
		};
	}
}
