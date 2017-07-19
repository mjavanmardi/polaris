#pragma once
#include "Traffic_Simulator_Includes.h"
//#include "../File_IO/utilities.h"
//#include "Signal_Prototypes.h"

namespace Intersection_Components
{
	namespace Types
	{
		enum Intersection_Type_Keys
		{
			NO_CONTROL=0,
			YIELD_SIGN,
			ALL_WAY_STOP_SIGN,
			TWO_WAY_STOP_SIGN,
			PRE_TIMED_SIGNAL_CONTROL, 
			ACTUATED_SIGNAL_CONTROL, 
			ADAPTIVE_SIGNAL_CONTROL,
		};
	}

	namespace Concepts
	{
	}
	
	namespace Prototypes
	{

		prototype struct Outbound_Inbound_Movements ADD_DEBUG_INFO
		{
			tag_as_prototype;

			accessor(outbound_link_reference, NONE, NONE);
			accessor(inbound_movements, NONE, NONE);

			template<typename TargetType> void update_state()
			{
				this_component()->template update_state<TargetType>();
			}

			template<typename TargetType> void supply_allocation_based_on_driving_rule()
			{
				this_component()->template supply_allocation_based_on_driving_rule<TargetType>();
			}

			template<typename TargetType> void supply_allocation_proportion_to_demand()
			{
				this_component()->template supply_allocation_proportion_to_demand<TargetType>();
			}

			template<typename TargetType> void supply_allocation_proportion_to_link()
			{
				this_component()->template supply_allocation_proportion_to_link<TargetType>();
			}
			
			template<typename TargetType> void supply_allocation_proportion_to_lane()
			{
				this_component()->template supply_allocation_proportion_to_lane<TargetType>();
			}
		};

		prototype struct Inbound_Outbound_Movements ADD_DEBUG_INFO
		{
			tag_as_prototype;

			accessor(inbound_link_reference, NONE, NONE);
			accessor(lane_groups, NONE, NONE);
			accessor(outbound_movements, NONE, NONE);

			template<typename TargetType> void link_capacity_allocation()
			{
				this_component()->template link_capacity_allocation<TargetType>();
			}

			template<typename TargetType> void update_state()
			{
				//typedef  Link_Components::Prototypes::Link< typename get_type_of(inbound_link_reference)> _Link_Interface;
				//typedef  Turn_Movement_Components::Prototypes::Movement<typename remove_pointer< typename get_type_of(outbound_movements)::value_type>::type>  _Movement_Interface;
				//typedef  Random_Access_Sequence< typename get_type_of(outbound_movements), _Movement_Interface*> _Outbound_Movements_Container_Interface;

				//typedef  Vehicle_Components::Prototypes::Vehicle<typename remove_pointer< typename _Movement_Interface::get_type_of(vehicles_container)::value_type>::type>  _Vehicle_Interface;
				//typedef  Back_Insertion_Sequence< typename _Movement_Interface::get_type_of(vehicles_container), _Vehicle_Interface*> _Vehicles_Container_Interface;


				//_Link_Interface* inbound_link = inbound_link_reference<_Link_Interface*>();

				//int num_vehicles_on_link = 0;
				//_Outbound_Movements_Container_Interface& outbound_movements_container = outbound_movements<_Outbound_Movements_Container_Interface&>();
				//typename _Outbound_Movements_Container_Interface::iterator outbound_movement_itr;
				//for (outbound_movement_itr=outbound_movements_container.begin();outbound_movement_itr!=outbound_movements_container.end();outbound_movement_itr++)
				//{
				//	_Movement_Interface* outbound_movement = (_Movement_Interface*)(*outbound_movement_itr);
				//	num_vehicles_on_link += int(outbound_movement->template vehicles_container<_Vehicles_Container_Interface&>().size());
				//}
				//inbound_link->template num_vehicles_on_link<int>(num_vehicles_on_link);
			}
		};

		prototype struct Intersection ADD_DEBUG_INFO
		{
			tag_as_prototype;

			// basic intersection
			accessor(uuid, NONE, NONE);
			accessor(dbid, NONE, NONE);
			accessor(internal_id, NONE, NONE);
			accessor(x_position, NONE, NONE);
			accessor(y_position, NONE, NONE);
			accessor(z_position, NONE, NONE);
			accessor(agency, NONE, NONE);
			accessor(street, NONE, NONE);
			accessor(name, NONE, NONE);
			accessor(description, NONE, NONE);
			accessor(inbound_links, NONE, NONE);
			accessor(outbound_links, NONE, NONE);
			accessor(zone, NONE, NONE);
			/*accessor(inbound_walk_links, NONE, NONE);
			accessor(outbound_walk_links, NONE, NONE);
			accessor(inbound_transit_links, NONE, NONE);
			accessor(outbound_transit_links, NONE, NONE);*/
			accessor(intersection_type, NONE, NONE);
			accessor(network_reference, NONE, NONE);

			// transportation intersection
			accessor(inbound_outbound_movements, NONE, NONE);
			accessor(outbound_inbound_movements, NONE, NONE);

			// controlled intersection
			accessor(intersection_control, NONE, NONE);
			accessor(signal, NONE, NONE);

			// simulation intersection
			accessor(rng_stream, NONE, NONE);

			template<typename TargetType> void push_vehicle(void* vehicle)
			{
				accept_vehicle<TargetType>(vehicle);
			}

			template<typename TargetType> void accept_vehicle(void* vehicle)
			{
				this_component()->template accept_vehicle<TargetType>(vehicle);
			}

			template<typename TargetType> void turn_movement_capacity_update()
			{
				typedef  Intersection_Components::Prototypes::Outbound_Inbound_Movements<typename remove_pointer< typename get_type_of(outbound_inbound_movements)::value_type>::type>  _Outbound_Inbound_Movements_Interface;
				typedef  Random_Access_Sequence< typename get_type_of(outbound_inbound_movements), _Outbound_Inbound_Movements_Interface*> _Outbound_Inbound_Movements_Container_Interface;

				typedef  Link_Components::Prototypes::Link< typename _Outbound_Inbound_Movements_Interface::get_type_of(outbound_link_reference)> _Link_Interface;
				typedef  Turn_Movement_Components::Prototypes::Movement<typename remove_pointer< typename _Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements)::value_type>::type>  _Movement_Interface;
				typedef  Random_Access_Sequence< typename _Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements), _Movement_Interface*> _Movements_Container_Interface;


				_Link_Interface* outbound_link;
				_Outbound_Inbound_Movements_Container_Interface& outbound_links_container=outbound_inbound_movements<_Outbound_Inbound_Movements_Container_Interface&>();
				typename _Outbound_Inbound_Movements_Container_Interface::iterator outbound_itr;
				for(outbound_itr=outbound_links_container.begin(); outbound_itr!=outbound_links_container.end(); outbound_itr++)
				{
					outbound_link=((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->template outbound_link_reference<_Link_Interface*>();
					_Movements_Container_Interface& inbound_links_container=((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->template inbound_movements<_Movements_Container_Interface&>();
					typename _Movements_Container_Interface::iterator inbound_itr;
					_Movement_Interface* inbound_movement;
					for(inbound_itr=inbound_links_container.begin();inbound_itr!=inbound_links_container.end();inbound_itr++)
					{
						inbound_movement=(_Movement_Interface*)(*inbound_itr);
						inbound_movement->template update_capacity<Types::Intersection_Type_Keys>(intersection_type<Types::Intersection_Type_Keys>());
					}
				}
			}
			template<typename TargetType> void turn_movement_demand_calculation()
			{
				typedef  Intersection_Components::Prototypes::Outbound_Inbound_Movements<typename remove_pointer< typename get_type_of(outbound_inbound_movements)::value_type>::type>  _Outbound_Inbound_Movements_Interface;
				typedef  Random_Access_Sequence< typename get_type_of(outbound_inbound_movements), _Outbound_Inbound_Movements_Interface*> _Outbound_Inbound_Movements_Container_Interface;

				typedef  Link_Components::Prototypes::Link< typename _Outbound_Inbound_Movements_Interface::get_type_of(outbound_link_reference)> _Link_Interface;
				typedef  Turn_Movement_Components::Prototypes::Movement<typename remove_pointer< typename _Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements)::value_type>::type>  _Movement_Interface;
				typedef  Random_Access_Sequence< typename _Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements), _Movement_Interface*> _Movements_Container_Interface;


				_Link_Interface* outbound_link;
				_Outbound_Inbound_Movements_Container_Interface& outbound_links_container=outbound_inbound_movements<_Outbound_Inbound_Movements_Container_Interface&>();
				typename _Outbound_Inbound_Movements_Container_Interface::iterator outbound_itr;
				for (outbound_itr=outbound_links_container.begin(); outbound_itr!=outbound_links_container.end(); outbound_itr++)
				{
					outbound_link=((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->template outbound_link_reference<_Link_Interface*>();
					_Movement_Interface* inbound_movement;
					_Movements_Container_Interface& inbound_links_container = ((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->template inbound_movements<_Movements_Container_Interface&>();
					typename _Movements_Container_Interface::iterator inbound_itr;
					for(inbound_itr=inbound_links_container.begin();inbound_itr!=inbound_links_container.end();inbound_itr++)
					{
						inbound_movement=(_Movement_Interface*)(*inbound_itr);
						inbound_movement->template update_demand<NT>();

					}
				}
			}

			template<typename TargetType> void turn_movement_flow_calculation()
			{
				typedef  Intersection_Components::Prototypes::Outbound_Inbound_Movements<typename remove_pointer< typename get_type_of(outbound_inbound_movements)::value_type>::type>  _Outbound_Inbound_Movements_Interface;
				typedef  Random_Access_Sequence< typename get_type_of(outbound_inbound_movements), _Outbound_Inbound_Movements_Interface*> _Outbound_Inbound_Movements_Container_Interface;

				typedef  Link_Components::Prototypes::Link< typename _Outbound_Inbound_Movements_Interface::get_type_of(outbound_link_reference)> _Link_Interface;
				typedef  Turn_Movement_Components::Prototypes::Movement<typename remove_pointer< typename _Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements)::value_type>::type>  _Movement_Interface;
				typedef  Random_Access_Sequence< typename _Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements), _Movement_Interface*> _Movements_Container_Interface;


				_Link_Interface* outbound_link;
				_Outbound_Inbound_Movements_Container_Interface& outbound_links_container=outbound_inbound_movements<_Outbound_Inbound_Movements_Container_Interface&>();
				typename _Outbound_Inbound_Movements_Container_Interface::iterator outbound_itr;
				for (outbound_itr=outbound_links_container.begin(); outbound_itr!=outbound_links_container.end(); outbound_itr++)
				{
					outbound_link=((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->template outbound_link_reference<_Link_Interface*>();
					_Movement_Interface* inbound_movement;
					_Movements_Container_Interface& inbound_links_container = ((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->template inbound_movements<_Movements_Container_Interface&>();
					typename _Movements_Container_Interface::iterator inbound_itr;
					for(inbound_itr=inbound_links_container.begin();inbound_itr!=inbound_links_container.end();inbound_itr++)
					{
						inbound_movement=(_Movement_Interface*)(*inbound_itr);

						inbound_movement->template update_flow<int>();
					}
				}
			}

			template<typename TargetType> void turn_movement_supply_allocation()
			{
				this_component()->template turn_movement_supply_allocation<TargetType>();
			}
			
			template<typename TargetType> void link_capacity_allocation()
			{
				this_component()->template link_capacity_allocation<TargetType>();
			}

			template<typename TargetType> void node_transfer()
			{
				this_component()->template node_transfer<TargetType>();
			}

			template<typename TargetType> void network_state_update()
			{
				typedef  Intersection_Components::Prototypes::Outbound_Inbound_Movements<typename remove_pointer< typename get_type_of(outbound_inbound_movements)::value_type>::type>  _Outbound_Inbound_Movements_Interface;
				typedef  Random_Access_Sequence< typename get_type_of(outbound_inbound_movements), _Outbound_Inbound_Movements_Interface*> _Outbound_Inbound_Movements_Container_Interface;

				typedef  Intersection_Components::Prototypes::Inbound_Outbound_Movements<typename remove_pointer< typename get_type_of(inbound_outbound_movements)::value_type>::type>  _Inbound_Outbound_Movements_Interface;
				typedef  Random_Access_Sequence< typename get_type_of(inbound_outbound_movements), _Inbound_Outbound_Movements_Interface*> _Inbound_Outbound_Movements_Container_Interface;


				_Outbound_Inbound_Movements_Container_Interface& outbound_links_container=outbound_inbound_movements<_Outbound_Inbound_Movements_Container_Interface&>();
				typename _Outbound_Inbound_Movements_Container_Interface::iterator outbound_itr;
				for (outbound_itr=outbound_links_container.begin(); outbound_itr!=outbound_links_container.end(); outbound_itr++)
				{
					((_Outbound_Inbound_Movements_Interface*)(*outbound_itr))->template update_state<NULLTYPE>();
				}
				_Inbound_Outbound_Movements_Container_Interface& inbound_links_container=inbound_outbound_movements<_Inbound_Outbound_Movements_Container_Interface&>();
				typename _Inbound_Outbound_Movements_Container_Interface::iterator inbound_itr;
				for (inbound_itr=inbound_links_container.begin(); inbound_itr!=inbound_links_container.end(); inbound_itr++)
				{
					((_Inbound_Outbound_Movements_Interface*)(*inbound_itr))->template update_state<NULLTYPE>();
				}
			}
			
			template<typename TargetType> void origin_link_loading()
			{
				this_component()->template origin_link_loading<TargetType>();
			}

			template<typename TargetType> void initialize_features(TargetType param)
			{
				this_component()->template initialize_features<TargetType>(param);
			}

			template<typename TargetType> void Initialize()
			{
				this_component()->template Initialize<TargetType>();
			}

			//template<typename TargetType> void construct_routable_from_regular(typename TargetType::ParamType regular_intersection_param, typename TargetType::Param2Type linksMap_param)
			//{
			//	this_component()->template construct_routable_from_regular<TargetType>(regular_intersection_param, linksMap_param);
			//}
			
			template<typename RegularIntersectionParamType,typename LinksMapParamType> void construct_routable_from_regular(RegularIntersectionParamType regular_intersection_param, LinksMapParamType linksMap_param)
			{
				this_component()->template construct_routable_from_regular<RegularIntersectionParamType,LinksMapParamType>(regular_intersection_param, linksMap_param);
			}

			template<typename TargetType> void construct_realtime_routable_from_regular(typename TargetType::ParamType regular_intersection_param, typename TargetType::Param2Type linksMap_param)
			{
				this_component()->template construct_realtime_routable_from_regular<TargetType>(regular_intersection_param, linksMap_param);
			}
			
			template<typename TargetType> void set_forward_link_turn_travel_time(TargetType movement_travel_time_map)
			{
				this_component()->template set_forward_link_turn_travel_time<TargetType>(movement_travel_time_map);
			}

			template<typename TargetType> void calculate_moe_for_simulation_interval()
			{
				this_component()->template calculate_moe_for_simulation_interval<TargetType>();
			}

			template<typename TargetType> void calculate_moe_for_assignment_interval()
			{
				this_component()->template calculate_moe_for_assignment_interval<TargetType>();
			}
			
			template<typename TargetType> void update_vehicle_locations()
			{
				this_component()->template update_vehicle_locations<TargetType>();
			}

			template<typename TargetType> void update_in_network_vehicle_vht()
			{
				this_component()->template update_in_network_vehicle_vht<TargetType>();
			}
		};
	}
}

using namespace Intersection_Components::Prototypes;
