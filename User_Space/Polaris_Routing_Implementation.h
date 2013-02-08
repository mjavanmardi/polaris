#pragma once
#include "Routing_Prototype.h"
#include "Routable_Link_Implementation.h"
#include "Routable_Intersection_Implementation.h"
#ifndef FOR_LINUX_PORTING
#include "Person_Implementations.h"
#endif

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

		implementation	struct Routable_Network_Implementation:public Polaris_Component_Class<Routable_Network_Implementation,MasterType,Data_Object,ParentType>
		{
			typedef set<pair<float, void*>> scan_list_typedef;
			member_data(scan_list_typedef, scan_list, none, none);
			member_data(float, max_free_flow_speed, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));

			member_container(vector<typename MasterType::routable_intersection_type*>, intersections_container, none, none);

			member_container(vector<typename MasterType::routable_link_type*>, links_container, none, none);

			member_container(vector<typename MasterType::link_type*>, reversed_path_container, none, none);
			typedef typename MasterType::network_type regular_network_type;

			feature_implementation void reset_routable_network()
			{
				define_container_and_value_interface_unqualified_container(_Routable_Links_Container_Interface, _Routable_Link_Interface, type_of(links_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);

				typedef scan_list_type ScanListType;
				_scan_list.clear();
	
				_reversed_path_container.clear();
				typename _Routable_Links_Container_Interface::iterator link_itr;

				for(link_itr=_links_container.begin();link_itr!=_links_container.end();link_itr++)
				{
					_Routable_Link_Interface* link_ptr = (_Routable_Link_Interface*)(*link_itr);
					link_ptr->template reset_routable_link<NULLTYPE>();
				}
			}

			feature_implementation void read_network_data(typename TargetType::ParamType regular_network, requires(check_2(typename TargetType::NetIOType,Network_Components::Types::Regular_Network,is_same)))
			{

				define_container_and_value_interface_unqualified_container(_Routable_Links_Container_Interface, _Routable_Link_Interface, type_of(links_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_container_and_value_interface_unqualified_container(_Routable_Intersections_Container_Interface, _Routable_Intersection_Interface, type_of(intersections_container), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Intersection_Prototype, ComponentType);
				define_container_and_value_interface(_Routable_Outbound_Inbound_Movements_Container_Interface, _Routable_Outbound_Inbound_Movements_Interface, typename _Routable_Intersection_Interface::get_type_of(outbound_inbound_movements), Random_Access_Sequence_Prototype,Outbound_Inbound_Movements_Prototype, ComponentType);
				define_container_and_value_interface(_Routable_Inbound_Outbound_Movements_Container_Interface, _Routable_Inbound_Outbound_Movements_Interface, typename _Routable_Intersection_Interface::get_type_of(inbound_outbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Inbound_Outbound_Movements_Prototype, ComponentType);
				define_container_and_value_interface(_Routable_Movements_Container_Interface, _Routable_Movement_Interface, typename _Routable_Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements), Random_Access_Sequence_Prototype, Turn_Movement_Components::Prototypes::Movement_Prototype, ComponentType);

				typedef Network_Components::Prototypes::Network_Prototype<regular_network_type> _Regular_Network_Interface;
				define_container_and_value_interface(_Regular_Links_Container_Interface, _Regular_Link_Interface, typename regular_network_type::links_container_type, Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_container_and_value_interface(_Regular_Intersections_Container_Interface, _Regular_Intersection_Interface, typename regular_network_type::intersections_container_type, Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Intersection_Prototype, ComponentType);
				define_container_and_value_interface(_Regular_Outbound_Inbound_Movements_Container_Interface, _Regular_Outbound_Inbound_Movements_Interface, typename _Regular_Intersection_Interface::get_type_of(outbound_inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Outbound_Inbound_Movements_Prototype, ComponentType);
				define_container_and_value_interface(_Regular_Inbound_Outbound_Movements_Container_Interface, _Regular_Inbound_Outbound_Movements_Interface, typename _Regular_Intersection_Interface::get_type_of(inbound_outbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Inbound_Outbound_Movements_Prototype, ComponentType);
				define_container_and_value_interface(_Regular_Movements_Container_Interface, _Regular_Movement_Interface, typename _Regular_Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements), Random_Access_Sequence_Prototype, Turn_Movement_Components::Prototypes::Movement_Prototype, ComponentType);
				

				_max_free_flow_speed = ((_Regular_Network_Interface*)regular_network)->template max_free_flow_speed<float>();

				
				_Regular_Links_Container_Interface& regular_links_container = ((_Regular_Network_Interface*)regular_network)->template links_container<_Regular_Links_Container_Interface&>();
				
				// copy all links
				cout << "copying links" << endl;
				dense_hash_map<int, _Routable_Link_Interface*> linksMap;
				linksMap.set_empty_key(-1);
				linksMap.set_deleted_key(-2);
				typename _Regular_Links_Container_Interface::iterator regular_link_itr;
				for(regular_link_itr=regular_links_container.begin(); regular_link_itr!=regular_links_container.end(); regular_link_itr++)
				{
					_Regular_Link_Interface* regular_link = (_Regular_Link_Interface*)(*regular_link_itr);
					_Routable_Link_Interface* routable_link =  (_Routable_Link_Interface*)Allocate<typename _Routable_Link_Interface::Component_Type>();
					routable_link->template construct_routable_from_regular<_Regular_Link_Interface*>(regular_link);
					links_container<ComponentType,CallerType,_Routable_Links_Container_Interface&>().push_back(routable_link);
					linksMap.insert(pair<int, _Routable_Link_Interface*>(regular_link->template internal_id<int>(), routable_link));
					regular_link->template replicas_container<_Routable_Links_Container_Interface&>().push_back(routable_link);
				}
				cout << "copying intersections" << endl;
				// copy all intersections
				int counter = -1;
				_Regular_Intersections_Container_Interface& regular_intersections_container = ((_Regular_Network_Interface*)regular_network)->template intersections_container<_Regular_Intersections_Container_Interface&>();
				dense_hash_map<int, _Routable_Intersection_Interface*> intersectionsMap;
				intersectionsMap.set_empty_key(-1);
				intersectionsMap.set_deleted_key(-2);
				typename _Regular_Intersections_Container_Interface::iterator regular_intersection_itr;
				for(regular_intersection_itr=regular_intersections_container.begin(); regular_intersection_itr!=regular_intersections_container.end(); regular_intersection_itr++)
				{
					if (++counter % 10000 == 0)
					{
						cout << counter << " intersections copied" << endl;
					}
					_Regular_Intersection_Interface* regular_intersection = (_Regular_Intersection_Interface*)(*regular_intersection_itr);
					_Routable_Intersection_Interface* routable_intersection =  (_Routable_Intersection_Interface*)Allocate<typename _Routable_Intersection_Interface::Component_Type>();
					typedef dense_hash_map<int,_Routable_Link_Interface*>& linksMapType;
					routable_intersection->template construct_routable_from_regular<Target_Type<void,_Regular_Intersection_Interface*,linksMapType>>(regular_intersection, linksMap);

					intersections_container<ComponentType,CallerType,_Routable_Intersections_Container_Interface&>().push_back(routable_intersection);
					intersectionsMap.insert(pair<int, _Routable_Intersection_Interface*>(regular_intersection->template internal_id<int>(), routable_intersection));
				}
				cout << " adding up/down streams to each link" << endl;
				// add upstream and downstream to each link
				typename _Routable_Links_Container_Interface::iterator routable_link_itr;
				for(routable_link_itr=_links_container.begin(); routable_link_itr!=_links_container.end(); routable_link_itr++)
				{

					_Routable_Link_Interface* routable_link = (_Routable_Link_Interface*)(*routable_link_itr);
					_Regular_Link_Interface* regular_link = routable_link->template network_link_reference<_Regular_Link_Interface*>();
		
					_Regular_Intersection_Interface* regular_upstream_intersection = regular_link->template upstream_intersection<_Regular_Intersection_Interface*>();
					_Routable_Intersection_Interface* routable_upstream_intersection = intersectionsMap.find(regular_upstream_intersection->template internal_id<int>())->second;
					routable_link->template upstream_intersection<_Routable_Intersection_Interface*>(routable_upstream_intersection);
					_Regular_Intersection_Interface* regular_downstream_intersection = regular_link->template downstream_intersection<_Regular_Intersection_Interface*>();
					_Routable_Intersection_Interface* routable_downstream_intersection = intersectionsMap.find(regular_downstream_intersection->template internal_id<int>())->second;
					routable_link->template downstream_intersection<_Routable_Intersection_Interface*>(routable_downstream_intersection);
				}
				cout << " streams added" << endl;
			}
		};

		implementation struct Polaris_Routing_Implementation:public Polaris_Component_Class<Polaris_Routing_Implementation,MasterType,Execution_Object,ParentType>
		{

#ifndef FOR_LINUX_PORTING
			member_component(typename MasterType::person_type, traveler, none, none);
			define_component_interface(_Traveler_Interface, typename MasterType::person_type, Person_Components::Prototypes::Person_Prototype, NULLTYPE); 
#else
			member_component(typename MasterType::traveler_type, traveler, none, none);
			define_component_interface(_Traveler_Interface, typename MasterType::traveler_type, Traveler_Components::Prototypes::Traveler_Prototype, NULLTYPE); 
#endif
			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType vehicle()
			{
				return ((_Traveler_Interface*)_traveler)->template vehicle<TargetType>();
			}
			
			tag_getter_as_available(vehicle);

			member_component(typename MasterType::network_type, network, none, none);

			define_component_interface(_Network_Interface, typename MasterType::network_type, Network_Components::Prototypes::Network_Prototype, NULLTYPE);
			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType routable_network()
			{
				return ((_Network_Interface*)_network)->template routable_network<TargetType>();
			}
			
			tag_getter_as_available(routable_network);			
			
#ifndef FOR_LINUX_PORTING			

			// time increment at which skim tables are updated - set in the initializer
			member_data_component(Basic_Units::Implementations::Time_Implementation<MasterType>,_update_increment,none,none);
			member_component_feature(update_increment,_update_increment,Value,Basic_Units::Prototypes::Time_Prototype);

			// time period during which routing takes place
			member_data_component(Basic_Units::Implementations::Time_Implementation<MasterType>,_start_time,none,none);
			member_component_feature(start_time,_start_time,Value,Basic_Units::Prototypes::Time_Prototype);
			member_data_component(Basic_Units::Implementations::Time_Implementation<MasterType>,_end_time,none,none);
			member_component_feature(end_time,_end_time,Value,Basic_Units::Prototypes::Time_Prototype);
#endif			
			template<typename ComponentType, typename CallerType, typename TargetType>
			void routable_origin(TargetType set_value)
			{
				_routable_origin = set_value->template internal_id<int>();
			}
			tag_setter_as_available(routable_origin);

			typedef vector<typename MasterType::routable_link_type*> routable_links_container_type;
			typedef Polaris_Container<routable_links_container_type> _Links_Container_Type;

			typedef Random_Access_Sequence_Prototype<_Links_Container_Type, NULLTYPE, void*> _Links_Container_Interface;

			typedef Network_Components::Prototypes::Network_Prototype<typename MasterType::routable_network_type, NULLTYPE> _Routable_Network_Interface;
			template<typename ComponentType, typename CallerType, typename TargetType>
			TargetType routable_origin()
			{
				return (TargetType)(((_Network_Interface*)_network)->template routable_network<_Routable_Network_Interface*>()->template links_container<_Links_Container_Interface&>()[_routable_origin]);
			}
			tag_getter_as_available(routable_origin);

			int _routable_origin;

			template<typename ComponentType, typename CallerType, typename TargetType>
			void routable_destination(TargetType set_value)
			{
				_routable_destination = set_value->template internal_id<int>();
			}
			tag_setter_as_available(routable_destination);

			template<typename ComponentType, typename CallerType, typename TargetType>
			TargetType routable_destination()
			{
				return (TargetType)(((_Network_Interface*)_network)->template routable_network<_Routable_Network_Interface*>()->template links_container<_Links_Container_Interface&>()[_routable_destination]);
			}
			tag_getter_as_available(routable_destination);

			int _routable_destination;	
			
			typedef typename MasterType::link_type regular_link_type;
			typedef typename MasterType::routable_link_type routable_link_type;
			typedef typename MasterType::vehicle_type vehicle_type;
			typedef typename MasterType::routable_network_type routable_network_type;
		};

		implementation struct Polaris_Integrated_Routing_Implementation:public Polaris_Component_Class<Polaris_Integrated_Routing_Implementation,MasterType,Execution_Object,ParentType>
		{
			member_container(vector<float>,travel_times_to_link_container,none,none);

#ifndef FOR_LINUX_PORTING
			member_component(typename MasterType::person_type, traveler, none, none);
			define_component_interface(_Traveler_Interface, typename MasterType::person_type, Person_Components::Prototypes::Person_Prototype, NULLTYPE); 
#else
			member_component(typename MasterType::traveler_type, traveler, none, none);
			define_component_interface(_Traveler_Interface, typename MasterType::traveler_type, Traveler_Components::Prototypes::Traveler_Prototype, NULLTYPE); 
#endif
			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType vehicle()
			{
				return ((_Traveler_Interface*)_traveler)->template vehicle<TargetType>();
			}
			
			tag_getter_as_available(vehicle);

			member_component(typename MasterType::network_type, network, none, none);

			define_component_interface(_Network_Interface, typename MasterType::network_type, Network_Components::Prototypes::Network_Prototype, NULLTYPE);
			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType routable_network()
			{
				return ((_Network_Interface*)_network)->template routable_network<TargetType>();
			}
			
			tag_getter_as_available(routable_network);			
			
#ifndef FOR_LINUX_PORTING			

			// time increment at which skim tables are updated - set in the initializer
			member_data_component(Basic_Units::Implementations::Time_Implementation<MasterType>,_update_increment,none,none);
			member_component_feature(update_increment,_update_increment,Value,Basic_Units::Prototypes::Time_Prototype);

			// time period during which routing takes place
			member_data_component(Basic_Units::Implementations::Time_Implementation<MasterType>,_start_time,none,none);
			member_component_feature(start_time,_start_time,Value,Basic_Units::Prototypes::Time_Prototype);
			member_data_component(Basic_Units::Implementations::Time_Implementation<MasterType>,_end_time,none,none);
			member_component_feature(end_time,_end_time,Value,Basic_Units::Prototypes::Time_Prototype);
#endif			
			template<typename ComponentType, typename CallerType, typename TargetType>
			void routable_origin(TargetType set_value)
			{
				_routable_origin = set_value->template internal_id<int>();
			}
			tag_setter_as_available(routable_origin);

			typedef vector<typename MasterType::routable_link_type*> routable_links_container_type;
			typedef Polaris_Container<routable_links_container_type> _Links_Container_Type;

			typedef Random_Access_Sequence_Prototype<_Links_Container_Type, NULLTYPE, void*> _Links_Container_Interface;

			typedef Network_Components::Prototypes::Network_Prototype<typename MasterType::routable_network_type, NULLTYPE> _Routable_Network_Interface;
			template<typename ComponentType, typename CallerType, typename TargetType>
			TargetType routable_origin()
			{
				return (TargetType)(((_Network_Interface*)_network)->template routable_network<_Routable_Network_Interface*>()->template links_container<_Links_Container_Interface&>()[_routable_origin]);
			}
			tag_getter_as_available(routable_origin);

			int _routable_origin;

			template<typename ComponentType, typename CallerType, typename TargetType>
			void routable_destination(TargetType set_value)
			{
				_routable_destination = set_value->template internal_id<int>();
			}
			tag_setter_as_available(routable_destination);

			template<typename ComponentType, typename CallerType, typename TargetType>
			TargetType routable_destination()
			{
				return (TargetType)(((_Network_Interface*)_network)->template routable_network<_Routable_Network_Interface*>()->template links_container<_Links_Container_Interface&>()[_routable_destination]);
			}
			tag_getter_as_available(routable_destination);

			int _routable_destination;	
			
			typedef typename MasterType::link_type regular_link_type;
			typedef typename MasterType::routable_link_type routable_link_type;
			typedef typename MasterType::vehicle_type vehicle_type;
			typedef typename MasterType::routable_network_type routable_network_type;
		};
	}

}