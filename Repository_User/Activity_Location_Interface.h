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
	
	namespace Interfaces
	{
		template<typename ThisType,typename CallerType>
		struct Activity_Location_Interface
		{
			facet_accessor(origin_links);
			facet_accessor(destination_links);
			facet_accessor(zone);
		};
	}
}