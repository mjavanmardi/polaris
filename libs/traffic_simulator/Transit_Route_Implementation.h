#pragma once
#include "Transit_Route_Prototype.h"
#include "../repository/RNG_Implementations.h"

namespace Transit_Route_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}

	namespace Implementations
	{
		/*struct Movement_MOE_Data
		{
		float movement_flow_rate;
		float turn_penalty;
		float turn_penalty_standard_deviation;
		float inbound_link_turn_time;
		float outbound_link_turn_time;
		};*/

		implementation struct Transit_Route_Implementation :public Polaris_Component<MasterType, INHERIT(Transit_Route_Implementation), Data_Object>
		{
			typedef typename Polaris_Component<MasterType, INHERIT(Transit_Route_Implementation), Data_Object>::Component_Type ComponentType;
			
			m_data(int, internal_id, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(std::string, dbid, NONE, NONE);
			m_data(int, uuid, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));			
			m_data(std::string, agency, NONE, NONE);
			m_data(std::string, shortname, NONE, NONE);
			m_data(std::string, longname, NONE, NONE);
			m_data(std::string, description, NONE, NONE);
			m_data(int, type, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));


			//m_prototype(Null_Prototype, typename MasterType::link_type, inbound_link, NONE, NONE);

			//typedef  Link_Components::Prototypes::Link<type_of(inbound_link)> _Link_Interface;
			//typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;
			//typedef  Vehicle_Components::Prototypes::Vehicle<typename remove_pointer<typename  type_of(vehicles_container)::value_type>::type>  _Vehicle_Interface;
			//typedef  Back_Insertion_Sequence<type_of(vehicles_container), _Vehicle_Interface*> _Vehicles_Container_Interface;

			//typedef Network_Components::Prototypes::Network<typename MasterType::network_type> _Network_Interface;
			//typedef Turn_Movement_Components::Prototypes::Movement<typename MasterType::movement_type> _Turn_Movement_Interface;
			////typedef  Movement_Plan_Components::Prototypes::Movement_Plan< typename _Vehicle_Interface::get_type_of(movement_plan)> _Movement_Plan_Interface;
			//typedef Movement_Plan_Components::Prototypes::Movement_Plan<typename MasterType::movement_plan_type> _Movement_Plan_Interface;
			//typedef typename MasterType::link_type _link_component_type;

			/*Transit_Route_Implementation()
			{
				UNLOCK(_mvmt_lock);
			}*/

		};
	}
}

