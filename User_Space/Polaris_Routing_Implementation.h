#pragma once
#include "User_Space_Includes.h"
#include "Routable_Intersection_Implementation.h"
#include "Routable_Link_Implementation.h"
#include "Polaris_Vehicle_Implementation.h"
#include "Traveler_Prototype.h"
#include <iostream>
namespace Routing_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{

		implementation	struct Routable_Network_Implementation
		{
			typedef set<pair<float, void*>> scan_list_typedef;
			member_data(scan_list_typedef, scan_list, none, none);
			member_data(float, max_free_flow_speed, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));

			member_container(vector<typename MasterType::routable_intersection_type*>, intersections, none, none);

			member_container(vector<typename MasterType::routable_link_type*>, links, none, none);

			member_container(vector<typename MasterType::link_type*>, reversed_path_container, none, none);
			typedef typename MasterType::network_type regular_network_type;
		};

		implementation struct Polaris_Routing_Implementation
		{


			member_component(typename MasterType::traveler_type, traveler, none, none);
			define_component_interface(_Traveler_Interface, MasterType::traveler_type, Traveler_Components::Prototypes::Traveler_Prototype, NULLTYPE); 
			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType vehicle()
			{
				return ((_Traveler_Interface*)_traveler)->template vehicle<TargetType>();
			}
			
			tag_getter_as_available(vehicle);

			member_component(typename MasterType::network_type, network, none, none);

			define_component_interface(_Network_Interface, MasterType::network_type, Network_Components::Prototypes::Network_Prototype, NULLTYPE);
			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType routable_network()
			{
				return ((_Network_Interface*)_network)->template routable_network<TargetType>();
			}
			
			tag_getter_as_available(routable_network);			
			
			

			
			template<typename ComponentType, typename CallerType, typename TargetType>
			void origin_link(TargetType set_value)
			{
				_origin_link = set_value->template internal_id<int>();
			}
			tag_setter_as_available(origin_link);

			typedef vector<typename MasterType::routable_link_type*> routable_links_container_type;
			typedef Polaris_Container<routable_links_container_type> _Links_Container_Type;

			typedef Random_Access_Sequence_Prototype<_Links_Container_Type, NULLTYPE, void*> _Links_Container_Interface;

			typedef Routing_Components::Prototypes::Routable_Network_Prototype<typename MasterType::routable_network_type, NULLTYPE> _Routable_Network_Interface;
			template<typename ComponentType, typename CallerType, typename TargetType>
			TargetType origin_link()
			{
				return (TargetType)(((_Network_Interface*)_network)->template routable_network<_Routable_Network_Interface*>()->template links<_Links_Container_Interface&>()[_origin_link]);
			}
			tag_getter_as_available(origin_link);

			int _origin_link;

			template<typename ComponentType, typename CallerType, typename TargetType>
			void destination_link(TargetType set_value)
			{
				_destination_link = set_value->template internal_id<int>();
			}
			tag_setter_as_available(destination_link);

			template<typename ComponentType, typename CallerType, typename TargetType>
			TargetType destination_link()
			{
				return (TargetType)(((_Network_Interface*)_network)->template routable_network<_Routable_Network_Interface*>()->template links<_Links_Container_Interface&>()[_destination_link]);
			}
			tag_getter_as_available(destination_link);

			int _destination_link;	
			
			typedef typename MasterType::link_type regular_link_type;
			typedef typename MasterType::routable_link_type routable_link_type;
			typedef typename MasterType::vehicle_type vehicle_type;
			typedef typename MasterType::routable_network_type routable_network_type;
		};
	}

}