#pragma once
#include "Transit_Pattern_Prototype.h"
#include "Network_DB_Reader_Implementations.h"
#include "Intersection_Implementation.h"
#include "../repository/RNG_Implementations.h"

namespace Transit_Pattern_Components
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

		implementation struct Transit_Pattern_Implementation :public Polaris_Component<MasterType, INHERIT(Transit_Pattern_Implementation), Data_Object>
		{
			typedef typename Polaris_Component<MasterType, INHERIT(Transit_Pattern_Implementation), Data_Object>::Component_Type ComponentType;

			m_data(int, internal_id, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(std::string, dbid, NONE, NONE);
			m_data(int, uuid, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_prototype(Transit_Route_Components::Prototypes::Transit_Route, typename MasterType::transit_route_type, route, NONE, NONE);
			m_container(std::vector<typename MasterType::intersection_type*>, pattern_stops, NONE, NONE);
			m_container(std::vector<typename MasterType::link_type*>, pattern_links, NONE, NONE);
			m_container(std::vector<int>, pattern_edge_ids, NONE, NONE);
			m_container(std::vector<typename MasterType::transit_vehicle_trip_type*>, pattern_trips, NONE, NONE);


			/*typedef Transit_Route_Components::Prototypes::Transit_Route<type_of(route)> _Transit_Route_Interface;
			typedef Intersection_Components::Prototypes::Intersection<type_of(pattern_stops)> _Intersection_Interface; 
			typedef Link_Components::Prototypes::Link<type_of(pattern_link)> _Link_Interface;*/
			//typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;
			//typedef  Vehicle_Components::Prototypes::Vehicle<typename remove_pointer<typename  type_of(vehicles_container)::value_type>::type>  _Vehicle_Interface;
			//typedef  Back_Insertion_Sequence<type_of(vehicles_container), _Vehicle_Interface*> _Vehicles_Container_Interface;

			//typedef Network_Components::Prototypes::Network<typename MasterType::network_type> _Network_Interface;
			//typedef Turn_Movement_Components::Prototypes::Movement<typename MasterType::movement_type> _Turn_Movement_Interface;
			////typedef  Movement_Plan_Components::Prototypes::Movement_Plan< typename _Vehicle_Interface::get_type_of(movement_plan)> _Movement_Plan_Interface;
			//typedef Movement_Plan_Components::Prototypes::Movement_Plan<typename MasterType::movement_plan_type> _Movement_Plan_Interface;
			//typedef typename MasterType::link_type _link_component_type;

			/*Transit_Pattern_Implementation()
			{
				UNLOCK(_mvmt_lock);
			}*/

		};

	}
}

