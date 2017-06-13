#pragma once
#include "Traffic_Simulator_Includes.h"

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
		prototype struct Analyze_Link_Group ADD_DEBUG_INFO
		{
			tag_as_prototype;
			accessor(links_container, NONE, NONE);
			accessor(event, NONE, NONE);
			accessor(link_moe_output_file, NONE, NONE);
			accessor(analyze_link_group_id, NONE, NONE);
			template<typename TargetType> void initialize(int group_id)
			{
				this_component()->template initialize<TargetType>(group_id);
			}
			template<typename TargetType> void output_moe(int time)
			{
				this_component()->template output_moe<TargetType>(time);
			}
			template<typename TargetType> void close_link_moe_output_file()
			{
				this_component()->template close_link_moe_output_file<TargetType>();
			}
		};
	}
}

using namespace Analyze_Link_Group_Components::Prototypes;
