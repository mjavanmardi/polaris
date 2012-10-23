#pragma once
#include "Repository_User.h"
#include "Routable_Intersection_Implementation.h"
#include "Routable_Link_Implementation.h"
#include "Polaris_Vehicle_Implementation.h"

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
			typedef set<pair<float, void*>> scan_list_type;
			member_data_basic(scan_list_type, scan_list);
			member_data_basic(float, max_free_flow_speed);

			member_container_basic(vector<typename MasterType::routable_intersection_type*>, intersections);

			member_container_basic(vector<typename MasterType::routable_link_type*>, links);

			member_container_basic(vector<typename MasterType::link_type*>, reversed_path_container);
			typedef typename MasterType::network_type regular_network_type;
		};

		implementation struct Polaris_Routing_Implementation
		{


			member_component_basic(typename MasterType::traveler_type, traveler);
			define_component_interface(_Traveler_Interface, Traveler_Prototype, traveler, NULLTYPE); 
			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType vehicle()
			{
				return ((_Traveler_Interface*)_traveler)->vehicle<TargetType>();
			}
			
			tag_getter(vehicle);

			member_component_basic(typename MasterType::network_type, network);

			define_component_interface(_Network_Interface, Network_Prototype, network, NULLTYPE);
			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType routable_network()
			{
				return ((_Network_Interface*)_network)->routable_network<TargetType>();
			}
			
			tag_getter(routable_network);			
			
			

			
			template<typename ComponentType, typename CallerType, typename TargetType>
			void origin_link(TargetType set_value)
			{
				_origin_link = set_value->uuid<int>();
			}
			tag_setter(origin_link);

			typedef vector<typename MasterType::routable_link_type*> routable_links_container_type;
			typedef Polaris_Container<routable_links_container_type> _Links_Container_Type;

			typedef Polaris_Random_Access_Sequence_Prototype<_Links_Container_Type, NULLTYPE, void*> _Links_Container_Interface;

			typedef Network_Prototype<typename MasterType::network_type, NULLTYPE> _Network_Interface;
			typedef Routable_Network_Prototype<typename MasterType::routable_network_type, NULLTYPE> _Routable_Network_Interface;
			template<typename ComponentType, typename CallerType, typename TargetType>
			TargetType origin_link()
			{
				return (TargetType)(((_Network_Interface*)_network)->routable_network<_Routable_Network_Interface*>()->links<_Links_Container_Interface&>()[_origin_link]);
			}
			tag_getter(origin_link);

			int _origin_link;

			template<typename ComponentType, typename CallerType, typename TargetType>
			void destination_link(TargetType set_value)
			{
				_destination_link = set_value->uuid<int>();
			}
			tag_setter(destination_link);

			template<typename ComponentType, typename CallerType, typename TargetType>
			TargetType destination_link()
			{
				return (TargetType)(((_Network_Interface*)_network)->routable_network<_Routable_Network_Interface*>()->links<_Links_Container_Interface&>()[_destination_link]);
			}
			tag_getter(destination_link);

			int _destination_link;	
			
			typedef typename MasterType::link_type regular_link_type;
			typedef typename MasterType::routable_link_type routable_link_type;
			typedef typename MasterType::vehicle_type vehicle_type;
			typedef typename MasterType::routable_network_type routable_network_type;
		};
	}

	namespace Components
	{
		template<typename MasterType>
		struct Routable_Network_Component{typedef Polaris_Component<Routing_Components::Implementations::Routable_Network_Implementation, MasterType> type; };

		template<typename MasterType>
		struct Polaris_Routing_Component{typedef Polaris_Component_Execution<Routing_Components::Implementations::Polaris_Routing_Implementation, MasterType> type; };	
	}	

}