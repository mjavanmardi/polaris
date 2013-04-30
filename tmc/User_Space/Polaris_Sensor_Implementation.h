#pragma once
#include "Sensor_Prototype.h"
#ifndef FOR_LINUX_PORTING
#include "../Repository/Basic_Unit_Implementations.h"
#endif
namespace Sensor_Components
{
	namespace Types{}

	namespace Concepts{}
	
	namespace Implementations
	{
		implementation struct FixedSensor:public Polaris_Component<APPEND_CHILD(FixedSensor),MasterType,Execution_Object,ParentType>
		{
			member_data(int, delta_t, none, none);
			member_data(double, sigma_speed, none, none);
			member_data(double, sigma_occupancy, none, none);
			member_data(double, sigma_volume, none, none);
			member_data(vector<Types::Reading>, reading_container, none, none);
			member_prototype(Link_Prototype, link, MasterType::link_type, none, none);

			feature_prototye void Initialize(typename MasterType::link_type* link,TargetType::Param2Type tmc)
			{
				_link = link;
			}
		}
	}
}

using namespace Sensor_Components::Implementations;