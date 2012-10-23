#pragma once
#include "Repository_User_Includes.h"

namespace Activity_Location_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Prototypes
	{
		template<typename ComponentType,typename CallerType>
		struct Activity_Location_Prototype
		{
			feature_accessor(origin_links);
			feature_accessor(destination_links);
			feature_accessor(zone);
		};
	}
}

using namespace Activity_Location_Components::Prototypes;