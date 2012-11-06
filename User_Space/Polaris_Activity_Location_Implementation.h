#pragma once
#include "User_Space_Includes.h"

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

			member_data(int, zone, none, none);

			member_container(vector<typename MasterType::link_type*>, origin_links, none, none);

			member_container(vector<typename MasterType::link_type*>, destination_links, none, none);

		};
	}

	namespace Components
	{
	}	

}