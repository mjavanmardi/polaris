#pragma once

//#include "../io/Io.h"
#include "Traffic_Simulator_Includes.h"

//#include "../File_IO/utilities.h"
//#include "../File_IO/network_data.h"
#include "Network_Prototype.h"
//#include "Scenario_Prototype.h"

namespace Network_Components
{
	
	namespace Prototypes
	{
		prototype struct Network_DB_Reader ADD_DEBUG_INFO
		{
			tag_as_prototype;

			accessor(network_reference, NONE, NONE);
	
			template<typename TargetType> void read_network_data(TargetType data_source)
			{
				this_component()->template read_network_data<TargetType>(data_source);
			}

			//template<typename TargetType> void read_network_data(requires(TargetType,!check_2(NetIOType,Types::ODB_Network,is_same) && !check_2(NetIOType,Types::File_Network,is_same) && !check_2(NetIOType,Types::Regular_Network,is_same)))
			//{
			//	//assert_check(false,"NetIOType is not supported");
			//	assert_check_2(NetIOType,Types::ODB_Network,is_same,"TargetType should indicate ODB_Network if you want to read it in with ODB");
			//	assert_check_2(NetIOType,Types::File_Network,is_same,"TargetType should indicate File_Network if you want to read from file");
			//	assert_check_2(NetIOType,Types::Regular_Network,is_same,"TargetType should indicate Regular_Network if you want to create a routable network from a regular network");
			//}

			template<typename TargetType> void write_network_data(typename TargetType::ParamType data_destination)
			{
				this_component()->template write_network_data<TargetType>(data_destination);
			}
		};
	}
}

using namespace Network_Components::Prototypes;
