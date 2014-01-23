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

			typedef Person_Scheduler<ComponentType> This_Type;
			
			// accessor to parent class
			accessor(Parent_Person, NONE, NONE);

			// accessors to physical world descriptors
			accessor(Network, NONE, NONE);
			accessor(Scenario, NONE, NONE);

			template<typename TargetType>_method_void(Check_Network_State, NONE);
			template<typename TargetType>_method_void(Initialize, NONE);
		};
	}

}
