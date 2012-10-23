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
	
	namespace Implementations
	{
		implementation struct Polaris_Activity_Location_Implementation
		{

			member_data_basic(int, zone);

			member_container_basic(vector<typename MasterType::link_type*>, origin_links);

			member_container_basic(vector<typename MasterType::link_type*>, destination_links);

		};
	}

	namespace Components
	{
		template<typename MasterType>
		struct Polaris_Activity_Location_Component{typedef Polaris_Component<Activity_Location_Components::Implementations::Polaris_Activity_Location_Implementation, MasterType> type; };
	}	

}