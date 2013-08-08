#pragma once
#include "User_Space_Includes.h"

namespace Activity_Location_Components
{
	namespace Types
	{
		enum LAND_USE
		{
			LU_ALL,
			LU_BUSINESS,
			LU_CIVIC_RELIGIOUS,
			LU_CULTURAL,
			LU_EDUCATION,
			LU_INDUSTRIAL,
			LU_SHOPPING,
			LU_MEDICAL,
			LU_MIXED_USE,
			LU_NONE,
			LU_RESIDENTIAL
		};

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
		prototype struct Activity_Location_Prototype ADD_DEBUG_INFO
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
			feature_accessor(land_use_type, none, none);

			feature_accessor(x_position, none, none);
			feature_accessor(y_position, none, none);
			feature_accessor(z_position, none, none);

			feature bool Has_Valid_Origin_Link()
			{
				define_container_and_value_interface(links_itf,link_itf,typename get_type_of(origin_links),Random_Access_Sequence_Prototype,Link_Components::Prototypes::Link_Prototype,ComponentType);			
				define_container_and_value_interface(turns_itf,turn_itf,typename link_itf::get_type_of(outbound_turn_movements),Random_Access_Sequence_Prototype,Turn_Movement_Components::Prototypes::Movement_Prototype,ComponentType);
				
				typename links_itf::iterator link_itr = this->origin_links<links_itf*>()->begin();
				if (link_itr != this->origin_links<links_itf*>()->end()	)
				{
					link_itf* link = *link_itr;
					if (link->template outbound_turn_movements<turns_itf*>()->size() > 0) return true;
					else return false;
				}
				else return false;

			}
			feature bool Has_Valid_Destination_Link()
			{
				define_container_and_value_interface(links_itf,link_itf,typename get_type_of(destination_links),Random_Access_Sequence_Prototype,Link_Components::Prototypes::Link_Prototype,ComponentType);			
				define_container_and_value_interface(turns_itf,turn_itf,typename link_itf::get_type_of(outbound_turn_movements),Random_Access_Sequence_Prototype,Turn_Movement_Components::Prototypes::Movement_Prototype,ComponentType);
				
				typename links_itf::iterator link_itr = this->destination_links<links_itf*>()->begin();
				if (link_itr != this->destination_links<links_itf*>()->end()	)
				{
					link_itf* link = *link_itr;
					if (link->template outbound_turn_movements<turns_itf*>()->size() > 0) return true;
					else return false;
				}
				else return false;

			}
		};
	}
}

using namespace Activity_Location_Components::Prototypes;