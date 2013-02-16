#pragma once
#include "Activity_Prototype.h"


namespace Activity_Components
{
	namespace Types
	{

	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{
		implementation struct Activity_Plan_Implementation : public Polaris_Component<APPEND_CHILD(Activity_Plan_Implementation),MasterType,Data_Object,ParentType>
		{
			member_data(long,Activity_Plan_ID,none,none);
		};
	}
}
