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
		implementation struct Polaris_Activity_Location_Implementation:public Polaris_Component<APPEND_CHILD(Polaris_Activity_Location_Implementation),MasterType,Data_Object,ParentType>
		{

			member_component(typename MasterType::zone_type, zone, none, none);

			member_container(vector<typename MasterType::link_type*>, origin_links, none, none);

			member_container(vector<typename MasterType::link_type*>, destination_links, none, none);

			member_container(vector<float>, origin_link_choice_cdfs, none, none);

			member_container(vector<float>, destination_link_choice_cdfs, none, none);

			member_data(int, uuid, check(ReturnValueType,is_integral), check(SetValueType,is_integral));

			member_data(int, internal_id, check(ReturnValueType,is_integral), check(SetValueType,is_integral));

			member_data(long long, census_zone_id, check(ReturnValueType,is_integral), check(SetValueType,is_integral));

			member_data(Types::LAND_USE, land_use_type, none, none);

		};
	}

}