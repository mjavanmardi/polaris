#pragma once
#include "Zone_Prototype.h"

namespace Zone_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{
		implementation struct Polaris_Zone_Implementation:public Polaris_Component_Class<Polaris_Zone_Implementation,MasterType,Data_Object,ParentType>
		{

			member_data(int, uuid, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, internal_id, none, none);

			member_container(vector<typename MasterType::activity_location_type*>, origin_activity_locations, none, none);

			member_container(vector<typename MasterType::link_type*>, destination_activity_locations, none, none);

			member_container(vector<float>, origin_activity_location_choice_cdfs, none, none);

			member_container(vector<float>, destination_activity_location_choice_cdfs, none, none);

		};
	}

}