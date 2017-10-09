#pragma once
#include "Activity_Location_Prototype.h"
namespace Activity_Location_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{
		implementation struct Activity_Location_Implementation:public Polaris_Component<MasterType,INHERIT(Activity_Location_Implementation),Data_Object>
		{
			typedef Polaris_Component<MasterType,INHERIT(Activity_Location_Implementation),Data_Object> Base_t;
			typedef typename Base_t::ComponentType ComponentType;

			m_prototype(Null_Prototype,typename MasterType::zone_type, zone, NONE, NONE);

			m_container(std::vector<Link_Components::Prototypes::Link<typename MasterType::link_type>*>, origin_links, NONE, NONE);

			m_container(std::vector<Link_Components::Prototypes::Link<typename MasterType::link_type>*>, destination_links, NONE, NONE);

			m_container(std::vector<Link_Components::Prototypes::Link<typename MasterType::link_type>*>, origin_walk_links, NONE, NONE);

			m_container(std::vector<Link_Components::Prototypes::Link<typename MasterType::link_type>*>, destination_walk_links, NONE, NONE);

			m_container(std::vector<float>, origin_link_choice_cdfs, NONE, NONE);

			m_container(std::vector<float>, destination_link_choice_cdfs, NONE, NONE);

			m_data(int, uuid, NONE, NONE);

			m_data(int, internal_id, NONE, NONE);

			m_data(long long, census_zone_id, NONE, NONE);

			m_data(Types::LAND_USE, land_use_type, NONE, NONE);

			//m_data(float, x_position, NONE, NONE);
			//m_data(float, y_position, NONE, NONE);

			member_component_and_feature_accessor(x_position, Value, Basic_Units::Prototypes::Length, Basic_Units::Implementations::template Length_Implementation<NT>);
			member_component_and_feature_accessor(y_position, Value, Basic_Units::Prototypes::Length, Basic_Units::Implementations::template Length_Implementation<NT>);
		};
	}

}
