#pragma once
#include "Sensor_Prototype.h"
#include "Repository\Basic_Unit_Implementations.h"

namespace Sensor_Components
{
	namespace Types{}

	namespace Concepts{}
	
	namespace Implementations
	{
		implementation struct Fixed_Sensor:public Polaris_Component_Class<Fixed_Sensor,MasterType,Execution_Object>
		{
			member_data(int,storage_start,none,none);
			member_data(int,storage_interval,none,none);
			member_data(int,storage_memory,none,none);

			feature_implementation void Initialize()
			{
				_quality=true;
				_malfunction=false;
				_operation=true;

				_storage_start=0;
				_storage_interval=1;
				_storage_memory=60*5;

				Load_Register<Fixed_Sensor>(&Sensor<Fixed_Sensor>::Maintenance_Condition<NULLTYPE>,&Sensor<Fixed_Sensor>::Maintenance<NULLTYPE>,0,0);
			}

			member_container(deque<float>,speeds,none,none);
			member_container(deque<int>,volumes,none,none);
			member_container(deque<float>,occupancies,none,none);
			member_container(deque<int>,counts,none,none);

			member_data(bool,quality,none,none);
			member_data(bool,malfunction,none,none);
			member_data(bool,operation,none,none);
		};
	}
}

using namespace Sensor_Components::Implementations;