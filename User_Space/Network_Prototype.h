#pragma once
#include <hash_map>
#include "User_Space.h"
#include "../File_IO/utilities.h"

#include "../File_IO/network_data.h"
//using namespace Basic_Units::Data_Structures;
//using namespace Signal_Components::Data_Structures;

/*
 * Needed for Linux compatability. Do not hurt Windows compilation.
 */
namespace Scenario_Components {
	namespace Prototypes
	{
		forward_declare_prototype struct Scenario_Prototype;
	}
};

namespace Activity_Location_Components {
	namespace Prototypes
	{
		forward_declare_prototype struct Activity_Location_Prototype;
	}
};
namespace Link_Components {
	namespace Prototypes
	{
		forward_declare_prototype struct Link_Prototype;
	}
};
namespace Vehicle_Components {
	namespace Prototypes
	{
		forward_declare_prototype struct Vehicle_Prototype;
	}
};
namespace Intersection_Components {
	namespace Prototypes
	{
		forward_declare_prototype struct Intersection_Prototype;
		forward_declare_prototype struct Movement_Prototype;
		forward_declare_prototype struct Outbound_Inbound_Movements_Prototype;
		forward_declare_prototype struct Inbound_Outbound_Movements_Prototype;
	}
};
namespace Zone_Components {
	namespace Prototypes
	{
		forward_declare_prototype struct Zone_Prototype;
	}
};
namespace Intersection_Control_Components {
	namespace Prototypes
	{
		forward_declare_prototype struct Intersection_Control_Prototype;
	}
};
/*------------------------------------------------*/

namespace Network_Components
{
	namespace Types
	{
		struct ODB_Network{};
		struct File_Network{};
		struct Regular_Network{};
			
		struct Network_IO_Maps
		{
			dense_hash_map<int,void*> intersection_id_to_ptr;
			dense_hash_map<long long,void*> link_id_dir_to_ptr;
		};

		template<typename _NetIOType,typename _ParamType>
		struct Network_Initialization_Type
		{
			typedef _NetIOType NetIOType;
			typedef _ParamType ParamType;
		};

		enum Scan_List_Status_Keys
		{
				UNSCANNED=0,
				SCANNED,
				INSELIST,
		};
	}

	namespace Concepts
	{
	}
	
	namespace Prototypes
	{
		prototype struct Network_Prototype
		{
			tag_as_prototype;

			//==================================================================================================================
			/// basic network
			//------------------------------------------------------------------------------------------------------------------
			feature_accessor(intersections_container, none, none);
			feature_accessor(links_container, none, none);
			//------------------------------------------------------------------------------------------------------------------
			
			//==================================================================================================================
			/// transportation network
			//------------------------------------------------------------------------------------------------------------------
			feature_accessor(turn_movements_container, none, none);
			feature_accessor(activity_locations_container, none, none);
			feature_accessor(zones_container, none, none);
			//------------------------------------------------------------------------------------------------------------------

			//==================================================================================================================
			/// routable network
			//------------------------------------------------------------------------------------------------------------------
			feature_accessor(routable_network, none, none);
			feature_accessor(routable_networks_container, none, none);
			feature_accessor(scan_list, none, none);
			feature_accessor(reversed_path_container, none, none);
			//------------------------------------------------------------------------------------------------------------------

			//==================================================================================================================
			/// simulation network
			//------------------------------------------------------------------------------------------------------------------
			feature_accessor(scenario_reference, none, none);
			feature_accessor(max_free_flow_speed, none, none);
			
			//------------------------------------------------------------------------------------------------------------------

			feature_prototype void read_network_data(typename TargetType::ParamType data_source, requires(check_2(TargetType::NetIOType,Types::ODB_Network,is_same) || check_2(TargetType::NetIOType,Types::File_Network,is_same) || check_2(TargetType::NetIOType,Types::Regular_Network,is_same)))
			{
				this_component()->template read_network_data<ComponentType,CallerType,TargetType>(data_source);
			}

			feature_prototype void read_network_data(requires(!check_2(TargetType::NetIOType,Types::ODB_Network,is_same) && !check_2(TargetType::NetIOType,Types::File_Network,is_same) && !check_2(TargetType::NetIOType,Types::Regular_Network,is_same)))
			{
				//assert_check(false,"TargetType::NetIOType is not supported");
				assert_check_2(TargetType::NetIOType,Types::ODB_Network,is_same,"TargetType should indicate ODB_Network if you want to read it in with ODB");
				assert_check_2(TargetType::NetIOType,Types::File_Network,is_same,"TargetType should indicate File_Network if you want to read from file");
				assert_check_2(TargetType::NetIOType,Types::Regular_Network,is_same,"TargetType should indicate Regular_Network if you want to create a routable network from a regular network");
			}

			feature_prototype void write_network_data(typename TargetType::ParamType data_destination)
			{
				this_component()->write_network_data<ComponentType,CallerType,TargetType>(data_destination);
			}

			feature_prototype void simulation_initialize()
			{
				this_component()->simulation_initialize<ComponentType,CallerType,TargetType>();
			}

			feature_prototype void reset_routable_network()
			{
				this_component()->reset_routable_network<ComponentType,CallerType,TargetType>();
			}

			feature_prototype TargetType current_simulation_interval_index()
			{
				define_component_interface(_Scenario_Interface, get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
				return (TargetType)(start_of_current_simulation_interval_relative<TargetType>() / scenario_reference<_Scenario_Interface*>()->template simulation_interval_length<int>());
			}

			feature_prototype TargetType start_of_current_simulation_interval_relative()
			{
				define_component_interface(_Scenario_Interface, get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
				int current_time = int(floor(Simulation_Time.Current_Time<Basic_Units::Time_Variables::Time_Seconds>() + 0.5));
				if (current_time < scenario_reference<_Scenario_Interface*>()->template simulation_interval_length<int>() - 1) 
				{
					cout << "_iteration must start from (simulation_interval_length - 1)" << endl;
					exit(1);
				}
				return (TargetType)(_iteration - (scenario_reference<_Scenario_Interface*>()->template simulation_interval_length<int>() - 1));
			}

			feature_prototype TargetType start_of_current_simulation_interval_absolute()
			{
				define_component_interface(_Scenario_Interface, get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
				return (TargetType)(start_of_current_simulation_interval_relative<TargetType>() + scenario_reference<_Scenario_Interface*>()->template simulation_start_time<int>());
			}
		};
	}
}

using namespace Network_Components::Prototypes;
void* _global_network;