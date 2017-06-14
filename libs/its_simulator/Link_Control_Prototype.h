#pragma once
#include "ITS_Simulator_Includes.h"

namespace Link_Control_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Prototypes
	{
		prototype struct Link_Control ADD_DEBUG_INFO
		{
			tag_as_prototype;
			
			template<typename TargetType> static void Initialize_Type(TargetType configuration)
			{
				ComponentType::template Initialize_Type<TargetType>(configuration);
			}

			template<typename TargetType> static void Initialize_Type()
			{
				ComponentType::template Initialize_Type<TargetType>();
			}	

			template<typename TargetType> void Initialize(TargetType configuration)
			{
				this_component()->template Initialize<TargetType>(configuration);
			}

			accessor(x_position, NONE, NONE);
			accessor(y_position, NONE, NONE);

			accessor(active_properties, NONE, NONE);
		};
	}
}

using namespace Link_Control_Components::Prototypes;
