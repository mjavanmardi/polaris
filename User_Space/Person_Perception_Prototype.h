#pragma once

#include "User_Space_Includes.h"


//---------------------------------------------------------
//	Person Perception class - Handles interactions with physical elements in simulation
//---------------------------------------------------------

namespace Person_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Prototypes
	{
		prototype struct Person_Perception ADD_DEBUG_INFO
		{
			tag_as_prototype;

			typedef Person_Scheduler<ComponentType, CallerType> This_Type;
			
			// accessor to parent class
			feature_accessor(Parent_Person,none,none);

			// accessors to physical world descriptors
			feature_accessor(Network,none,none);
			feature_accessor(Scenario,none,none);

			feature_method_void(Check_Network_State,none);
			feature_method_void(Initialize,none);
		};
	}

}