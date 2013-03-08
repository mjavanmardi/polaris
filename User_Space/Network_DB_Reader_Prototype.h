#pragma once

#include "../io/Io.h"
#include "User_Space_Includes.h"

#include "../File_IO/utilities.h"
#include "../File_IO/network_data.h"


namespace Network_Components
{
	
	namespace Prototypes
	{
		prototype struct Network_DB_Reader_Prototype
		{
			tag_as_prototype;

			feature_accessor(network_reference,none,none);
	
			feature_prototype void read_network_data(TargetType data_source)
			{
				this_component()->template read_network_data<ComponentType,CallerType,TargetType>(data_source);
			}

			//feature_prototype void read_network_data(requires(!check_2(typename TargetType::NetIOType,Types::ODB_Network,is_same) && !check_2(typename TargetType::NetIOType,Types::File_Network,is_same) && !check_2(typename TargetType::NetIOType,Types::Regular_Network,is_same)))
			//{
			//	//assert_check(false,"TargetType::NetIOType is not supported");
			//	assert_check_2(typename TargetType::NetIOType,Types::ODB_Network,is_same,"TargetType should indicate ODB_Network if you want to read it in with ODB");
			//	assert_check_2(typename TargetType::NetIOType,Types::File_Network,is_same,"TargetType should indicate File_Network if you want to read from file");
			//	assert_check_2(typename TargetType::NetIOType,Types::Regular_Network,is_same,"TargetType should indicate Regular_Network if you want to create a routable network from a regular network");
			//}

			feature_prototype void write_network_data(typename TargetType::ParamType data_destination)
			{
				this_component()->template write_network_data<ComponentType,CallerType,TargetType>(data_destination);
			}
		};
	}
}

using namespace Network_Components::Prototypes;