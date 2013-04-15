#pragma once
#include "User_Space_Includes.h"

namespace Variable_Message_Sign_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Prototypes
	{
		prototype struct Variable_Message_Sign:public ComponentType
		{
			tag_as_prototype;
			
			feature static void Initialize_Type(TargetType configuration)
			{
				ComponentType::Initialize_Type<ComponentType,CallerType,TargetType>(configuration);
			}

			feature void Initialize(TargetType configuration)
			{
				ComponentType::Initialize<ComponentType,CallerType,TargetType>(configuration);
			}

			feature_accessor(x_position, none, none);
			feature_accessor(y_position, none, none);

			feature void Get_Messages(vector<Network_Event<TargetType,ComponentType>*>& bucket)
			{
				return this_component()->Get_Messages<ComponentType,CallerType,TargetType>(bucket);
			}

			feature TargetType Get_Single_Message()
			{
				return this_component()->Get_Single_Message<ComponentType,CallerType,TargetType>();
			}

			feature_accessor(active_properties, none, none);
		};
	}
}

using namespace Variable_Message_Sign_Components::Prototypes;