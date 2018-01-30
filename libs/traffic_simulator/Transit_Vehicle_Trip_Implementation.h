#pragma once
#include "Transit_Vehicle_Trip_Prototype.h"
#include "../repository/RNG_Implementations.h"

namespace Transit_Vehicle_Trip_Components
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

		implementation struct Transit_Vehicle_Trip_Implementation :public Polaris_Component<MasterType, INHERIT(Transit_Vehicle_Trip_Implementation), Execution_Object>
		{
			typedef typename Polaris_Component<MasterType, INHERIT(Transit_Vehicle_Trip_Implementation), Data_Object>::Component_Type ComponentType;

			m_data(int, internal_id, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(std::string, dbid, NONE, NONE);
			m_data(int, uuid, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(int, direction, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_prototype(Transit_Pattern_Components::Prototypes::Transit_Pattern, typename MasterType::transit_pattern_type, pattern, NONE, NONE);
			m_container(std::vector<int>, arrival_seconds, NONE, NONE);
			m_container(std::vector<int>, departure_seconds, NONE, NONE);

			template<typename TargetType> void initialize();

		};

		template<typename MasterType, typename InheritanceList>
		template<typename TargetType>
		void Transit_Vehicle_Trip_Implementation<MasterType, InheritanceList>::initialize()
		{
		}
	}
}

