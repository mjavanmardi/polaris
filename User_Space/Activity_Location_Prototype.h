#pragma once
#include "User_Space.h"

namespace Activity_Location_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
		concept struct Is_Activity_Location_Prototype
		{
			check_getter(has_origin_links, Component_Type::origin_links);
			check_getter(has_destination_links, Component_Type::destination_links);
			check_getter(has_zone, Component_Type::zone); 
			check_getter(has_uuid, Component_Type::uuid); 
			check_getter(has_internal_id, Component_Type::internal_id); 
			define_default_check(has_origin_links && has_destination_links && has_zone && has_uuid && has_internal_id);
		};
		concept struct Is_Activity_Location
		{
			check_getter(has_origin_links, origin_links);
			check_getter(has_destination_links, destination_links);
			check_getter(has_zone, zone); 
			check_getter(has_uuid, uuid); 
			check_getter(has_internal_id, internal_id); 

			check_concept(is_activity_location_prototype, Is_Activity_Location_Prototype);
			define_sub_check(is_activity_location, has_origin_links && has_destination_links && has_zone && has_uuid && has_internal_id);
			define_default_check(is_activity_location || is_activity_location_prototype);
		};
	}
	
	namespace Prototypes
	{
		prototype struct Activity_Location_Prototype
		{
			tag_as_prototype;

			feature_accessor(origin_links, none, none);
			feature_accessor(destination_links, none, none);
			feature_accessor(origin_link_choice_cdfs, none, none);
			feature_accessor(destination_link_choice_cdfs, none, none);
			feature_accessor(zone, none, none);
			feature_accessor(census_zone_id, none, none);
			feature_accessor(uuid, none, none);
			feature_accessor(internal_id, none, none);
		};
	}
}

using namespace Activity_Location_Components::Prototypes;