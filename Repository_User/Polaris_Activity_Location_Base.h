#pragma once
#include "Repository_User_Includes.h"

namespace Activity_Location_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Bases
	{
		template<typename MasterType>
		struct Polaris_Activity_Location_Base
		{
			typedef typename MasterType::link_type link_type;
			typedef link_type links_container_element_type;
			typedef vector<void *> links_container_type;
		
			member_data_basic(int, zone);

			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType origin_links(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType)_origin_links;} tag_getter(origin_links);
			links_container_type _origin_links;

			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType destination_links(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType)_destination_links;} tag_getter(destination_links);
			links_container_type _destination_links;

		};
	}

	namespace Components
	{
		template<typename MasterType>
		struct Polaris_Activity_Location_Component
		{
			typedef Polaris_Component<Activity_Location_Components::Interfaces::Activity_Location_Interface, Activity_Location_Components::Bases::Polaris_Activity_Location_Base<MasterType>,NULLTYPE,MasterType> type;
		};
	}	

}