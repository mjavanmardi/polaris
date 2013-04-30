#pragma once

#include "User_Space_Includes.h"
//#include "Person_Prototype.h"


//---------------------------------------------------------
//	Person Scheduler - Handles Activity Scheduling for the Planner Class
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
		prototype struct Person_Scheduler ADD_DEBUG_INFO
		{
			tag_as_prototype;

			typedef Person_Properties<ComponentType, CallerType> This_Type;
			
			// accessor to parent class
			feature_accessor(Parent_Planner,none,none);
			// accessors to parent class members
			feature_accessor(Activity_Schedule,none,none);
			feature_accessor(Movement_Schedule,none,none);
		};
	}

}