#pragma once
#include "Traffic_Simulator_Includes.h"

namespace Activity_Location_Components
{
	namespace Types
	{
		enum LAND_USE
		{
			LU_AGRICULTURE,
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
			LU_NON_RESIDENTIAL,
			LU_RESIDENTIAL,
			LU_RESIDENTIAL_MULTI,
			LU_RECREATION,
			LU_SPECIAL_GENERATOR,
			TRANSIT_STOP
		};

	}


	namespace Prototypes
	{
		prototype struct Activity_Location ADD_DEBUG_INFO
		{
			tag_as_prototype;

			accessor(origin_links, NONE, NONE);
			accessor(destination_links, NONE, NONE);
			accessor(origin_walk_links, NONE, NONE);
			accessor(destination_walk_links, NONE, NONE);
			accessor(origin_link_choice_cdfs, NONE, NONE);
			accessor(destination_link_choice_cdfs, NONE, NONE);
			accessor(zone, NONE, NONE);
			accessor(census_zone_id, NONE, NONE);
			accessor(uuid, NONE, NONE);
			accessor(internal_id, NONE, NONE);
			accessor(land_use_type, NONE, NONE);

			accessor(x_position, NONE, NONE);
			accessor(y_position, NONE, NONE);
			accessor(z_position, NONE, NONE);

			template<typename TargetType> bool Has_Valid_Origin_Link()
			{			
				typedef Random_Access_Sequence<typename get_type_of(origin_links)> links_itf;
				typedef Link_Components::Prototypes::Link<get_component_type(links_itf)> link_itf;
				
				typedef Random_Access_Sequence<typename link_itf::get_type_of(outbound_turn_movements)> turns_itf;
				typedef Turn_Movement_Components::Prototypes::Movement<get_component_type(turns_itf)> turn_itf;
				
				typename links_itf::iterator link_itr = this->origin_links<links_itf*>()->begin();
				if (link_itr != this->origin_links<links_itf*>()->end()	)
				{
					link_itf* link = (link_itf*)(*link_itr);
					if (link->template outbound_turn_movements<turns_itf*>()->size() > 0) return true;
					else return false;
				}
				else return false;

			}
			template<typename TargetType> bool Has_Valid_Destination_Link()
			{
				typedef Link_Components::Prototypes::Link<typename remove_pointer<typename get_type_of(destination_links)::value_type>::type> link_itf;
				typedef Random_Access_Sequence<typename get_type_of(destination_links),link_itf*> links_itf;

				typedef Turn_Movement_Components::Prototypes::Movement<typename remove_pointer<typename link_itf::get_type_of(outbound_turn_movements)::value_type>::type> turn_itf;
				typedef Random_Access_Sequence<typename link_itf::get_type_of(outbound_turn_movements),turn_itf*> turns_itf;

				
				typename links_itf::iterator link_itr = this->destination_links<links_itf*>()->begin();
				if (link_itr != this->destination_links<links_itf*>()->end()	)
				{
					link_itf* link = *link_itr;
					if (link->template outbound_turn_movements<turns_itf*>()->size() > 0) return true;
					else return false;
				}
				else return false;

			}
			template<typename TargetType> bool Is_Routable_Location()
			{
				// Location is routable only if it has a valid origin/destination link and is not a transit stop
				bool routable_location = Has_Valid_Origin_Link<NT>() /*&& Has_Valid_Destination_Link<NT>()*/ && land_use_type<Types::LAND_USE>() != Types::TRANSIT_STOP && land_use_type<Types::LAND_USE>() != Types::LU_NONE;
				//if (!routable_location) cout << "Warning: location id "<<this->template uuid<int>() <<", is not a routable activity location."<<endl;
				return routable_location;
			}
			template<typename TargetType> bool is_work_location()
			{
				if (this->land_use_type<Types::LAND_USE>() == Types::LAND_USE::LU_NONE || this->land_use_type<Types::LAND_USE>() == Types::LU_RESIDENTIAL) return false;
				else return true;
			}
			template<typename TargetType> bool is_school_location()
			{
				if (this->land_use_type<Types::LAND_USE>() == Types::LAND_USE::LU_ALL || this->land_use_type<Types::LAND_USE>() == Types::LU_EDUCATION || this->land_use_type<Types::LAND_USE>() == Types::LU_MIXED_USE) return true;
				else return false;
			}
			template<typename TargetType> bool is_residential_location()
			{
				if (this->land_use_type<Types::LAND_USE>() == Types::LAND_USE::LU_ALL || this->land_use_type<Types::LAND_USE>() == Types::LU_RESIDENTIAL || this->land_use_type<Types::LAND_USE>() == Types::LU_MIXED_USE || this->land_use_type<Types::LAND_USE>() == Types::LU_RESIDENTIAL_MULTI) return true;
				else return false;
			}

			template<typename LocationType, typename DistanceType> DistanceType distance(LocationType from_loc, requires(LocationType,check(LocationType, is_pointer)))
			{
				DistanceType x1 = from_loc->template x_position<DistanceType>();
				DistanceType y1 = from_loc->template y_position<DistanceType>();

				DistanceType x2 = x_position<DistanceType>();
				DistanceType y2 = y_position<DistanceType>();

				return sqrt(pow(x1-x2,2) + pow(y1-y2,2));
			}
		};
	}
}

using namespace Activity_Location_Components::Prototypes;
