#pragma once

#include "Polaris_Link_Base.h"
#include "Routing_Interface.h"

namespace Link_Components
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
		struct Routable_Link_Base {

			typedef typename MasterType::link_type network_link_type;

			member_data_basic(int, uuid);
			member_data_basic(float, travel_time);
			member_data_basic(float, f_cost);
			member_data_basic(float, h_cost);
			member_data_basic(void*, label_pointer);
			member_data_basic(float, label_cost);
			member_data_basic(Scan_List_Status_Keys, scan_list_status);

			//--------------------------------------------------------
			// member_data<void*, network_link_reference>
			//--------------------------------------------------------
			template<typename ThisType, typename CallerType, typename TargetType>
			void network_link_reference(TargetType set_value, call_requirements(requires(ThisType,Is_Dispatched))){_network_link_reference = (void *)(set_value);}
			tag_setter(network_link_reference);
			
			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType network_link_reference(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType)(_network_link_reference);} 
			tag_getter(network_link_reference);
			
			void* _network_link_reference;

			//==================================================================================================================
			/// Upstream and Downstream Intersections Reference
			//------------------------------------------------------------------------------------------------------------------	
			typedef typename MasterType::routable_intersection_type intersection_type;
			
			template<typename ThisType, typename CallerType, typename TargetType>
			void upstream_intersection(TargetType value,call_requirements(requires(ThisType,Is_Dispatched))){_upstream_intersection=(TargetType)value;}
			tag_setter(upstream_intersection);
			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType upstream_intersection(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType)_upstream_intersection;}
			tag_getter(upstream_intersection);
			typedef intersection_type upstream_intersection_type;
			void* _upstream_intersection;

			template<typename ThisType, typename CallerType, typename TargetType>
			void downstream_intersection(TargetType value,call_requirements(requires(ThisType,Is_Dispatched))){_downstream_intersection=(TargetType)value;}
			tag_setter(downstream_intersection);
			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType downstream_intersection(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType)_downstream_intersection;}
			tag_getter(downstream_intersection);
			typedef intersection_type upstream_intersection_type;
			void* _downstream_intersection;
		};
		
	}

	namespace Components
	{
		template<typename MasterType>
		struct Routable_Link_Component
		{
			typedef Polaris_Component<Link_Interface, Link_Components::Bases::Routable_Link_Base<MasterType>, NULLTYPE, MasterType> type;
		};
	}	

}