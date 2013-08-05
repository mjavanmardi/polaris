#pragma once
#include "User_Space_Includes.h"

namespace Analyze_Link_Group_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Prototypes
	{
		prototype struct Analyze_Link_Group:public ComponentType
		{
			tag_as_prototype;
			feature_accessor(links_container,none,none);
			feature_accessor(event,none,none);
			feature_accessor(link_moe_output_file,none,none);
			feature_accessor(analyze_link_group_id,none,none);
			feature_prototype void initialize(int group_id)
			{
				this_component()->template initialize<ComponentType,CallerType,TargetType>(group_id);
			}
			feature_prototype void output_moe(int time)
			{
				this_component()->template output_moe<ComponentType,CallerType,TargetType>(time);
			}
			feature_prototype void close_link_moe_output_file()
			{
				this_component()->template close_link_moe_output_file<ComponentType,CallerType,TargetType>();
			}
		};
	}
}

using namespace Analyze_Link_Group_Components::Prototypes;