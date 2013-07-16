#pragma once
#include "User_Space_Includes.h"
#include "../File_IO/utilities.h"
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

		prototype struct Outbound_Inbound_Movements_Prototype
		{
			tag_as_prototype;

			feature_accessor(outbound_link_reference, none, none);
			feature_accessor(inbound_movements, none, none);

			feature_prototype void update_state()
			{
				this_component()->template update_state<ComponentType,CallerType,TargetType>();
			}

			feature_prototype void supply_allocation_based_on_driving_rule()
			{
				this_component()->template supply_allocation_based_on_driving_rule<ComponentType,CallerType,TargetType>();
			}

			feature_prototype void supply_allocation_proportion_to_demand()
			{
				this_component()->template supply_allocation_proportion_to_demand<ComponentType,CallerType,TargetType>();
			}

			feature_prototype void supply_allocation_proportion_to_link()
			{
				this_component()->template supply_allocation_proportion_to_link<ComponentType,CallerType,TargetType>();
			}
			
			feature_prototype void supply_allocation_proportion_to_lane()
			{
				this_component()->template supply_allocation_proportion_to_lane<ComponentType,CallerType,TargetType>();
			}
		};

		prototype struct Inbound_Outbound_Movements_Prototype
		{
			tag_as_prototype;

			feature_accessor(inbound_link_reference, none, none);
			feature_accessor(lane_groups, none, none);
			feature_accessor(outbound_movements, none, none);

			feature_prototype void update_state()
			{
				define_component_interface(_Link_Interface, typename get_type_of(inbound_link_reference), Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_container_and_value_interface(_Outbound_Movements_Container_Interface, _Movement_Interface, typename get_type_of(outbound_movements), Random_Access_Sequence_Prototype, Turn_Movement_Components::Prototypes::Movement_Prototype, ComponentType);
				define_container_and_value_interface(_Vehicles_Container_Interface, _Vehicle_Interface, typename _Movement_Interface::get_type_of(vehicles_container), Back_Insertion_Sequence_Prototype, Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);

				_Link_Interface* inbound_link = inbound_link_reference<_Link_Interface*>();

				int num_vehicles_on_link = 0;
				_Outbound_Movements_Container_Interface& outbound_movements_container = outbound_movements<_Outbound_Movements_Container_Interface&>();
				typename _Outbound_Movements_Container_Interface::iterator outbound_movement_itr;
				for (outbound_movement_itr=outbound_movements_container.begin();outbound_movement_itr!=outbound_movements_container.end();outbound_movement_itr++)
				{
					_Movement_Interface* outbound_movement = (_Movement_Interface*)(*outbound_movement_itr);
					num_vehicles_on_link += int(outbound_movement->template vehicles_container<_Vehicles_Container_Interface&>().size());
				}
				inbound_link->template num_vehicles_on_link<int>(num_vehicles_on_link);
			}
		};

		prototype struct Intersection_Prototype
		{
			tag_as_prototype;

			// basic intersection
			feature_accessor(uuid, none, none);
			feature_accessor(internal_id, none, none);
			feature_accessor(x_position, none, none);
			feature_accessor(y_position, none, none);
			feature_accessor(z_position, none, none);
			feature_accessor(inbound_links, none, none);
			feature_accessor(outbound_links, none, none);
			feature_accessor(intersection_type, none, none);
			feature_accessor(network_reference, none, none);

			// transportation intersection
			feature_accessor(inbound_outbound_movements, none, none);
			feature_accessor(outbound_inbound_movements, none, none);

			// controlled intersection
			feature_accessor(intersection_control, none, none);
			feature_accessor(signal, none, none);

			// simulation intersection
			feature_accessor(rng_stream, none, none);

			feature_prototype void push_vehicle(void* vehicle)
			{
				accept_vehicle<TargetType>(vehicle);
			}

			feature_prototype void accept_vehicle(void* vehicle)
			{
				this_component()->template accept_vehicle<ComponentType,CallerType,TargetType>(vehicle);
			}

			feature_prototype void turn_movement_capacity_update()
			{
				define_container_and_value_interface(_Outbound_Inbound_Movements_Container_Interface, _Outbound_Inbound_Movements_Interface, typename get_type_of(outbound_inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Outbound_Inbound_Movements_Prototype, ComponentType);
				define_component_interface(_Link_Interface, typename _Outbound_Inbound_Movements_Interface::get_type_of(outbound_link_reference), Link_Components::Prototypes::Link_Prototype,  ComponentType);
				define_container_and_value_interface(_Movements_Container_Interface, _Movement_Interface, typename _Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements), Random_Access_Sequence_Prototype, Turn_Movement_Components::Prototypes::Movement_Prototype, ComponentType);

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

			feature_prototype void turn_movement_flow_calculation()
			{
				define_container_and_value_interface(_Outbound_Inbound_Movements_Container_Interface, _Outbound_Inbound_Movements_Interface, typename get_type_of(outbound_inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Outbound_Inbound_Movements_Prototype, ComponentType);
				define_component_interface(_Link_Interface, typename _Outbound_Inbound_Movements_Interface::get_type_of(outbound_link_reference), Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_container_and_value_interface(_Movements_Container_Interface, _Movement_Interface, typename _Outbound_Inbound_Movements_Interface::get_type_of(inbound_movements), Random_Access_Sequence_Prototype, Turn_Movement_Components::Prototypes::Movement_Prototype, ComponentType);

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

			feature_prototype void turn_movement_supply_allocation()
			{
				this_component()->template turn_movement_supply_allocation<ComponentType,CallerType,TargetType>();
			}

			feature_prototype void node_transfer()
			{
				this_component()->template node_transfer<ComponentType,CallerType,TargetType>();
			}

			feature_prototype void network_state_update()
			{
				define_container_and_value_interface(_Outbound_Inbound_Movements_Container_Interface, _Outbound_Inbound_Movements_Interface, typename get_type_of(outbound_inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Outbound_Inbound_Movements_Prototype, ComponentType);
				define_container_and_value_interface(_Inbound_Outbound_Movements_Container_Interface, _Inbound_Outbound_Movements_Interface, typename get_type_of(inbound_outbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Inbound_Outbound_Movements_Prototype, ComponentType);

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
			
			feature_prototype void origin_link_loading()
			{
				this_component()->template origin_link_loading<ComponentType,CallerType,TargetType>();
			}

			feature_prototype void initialize_features(TargetType param)
			{
				this_component()->template initialize_features<ComponentType,CallerType,TargetType>(param);
			}

			feature_prototype void Initialize()
			{
				this_component()->template Initialize<ComponentType,CallerType,TargetType>();
			}

			feature_prototype void construct_routable_from_regular(typename TargetType::ParamType regular_intersection_param, typename TargetType::Param2Type linksMap_param)
			{
				this_component()->template construct_routable_from_regular<ComponentType,CallerType,TargetType>(regular_intersection_param, linksMap_param);
			}
			
			feature_prototype void construct_realtime_routable_from_regular(typename TargetType::ParamType regular_intersection_param, typename TargetType::Param2Type linksMap_param)
			{
				this_component()->template construct_realtime_routable_from_regular<ComponentType,CallerType,TargetType>(regular_intersection_param, linksMap_param);
			}
			
			feature_prototype void set_forward_link_turn_travel_time(typename TargetType::ParamType movement_travel_time_map)
			{
				this_component()->template set_forward_link_turn_travel_time<ComponentType,CallerType,TargetType>(movement_travel_time_map);
			}

			feature_prototype void calculate_moe_for_simulation_interval()
			{
				this_component()->template calculate_moe_for_simulation_interval<ComponentType,CallerType,TargetType>();
			}

			feature_prototype void calculate_moe_for_assignment_interval()
			{
				this_component()->template calculate_moe_for_assignment_interval<ComponentType,CallerType,TargetType>();
			}
			
			feature_prototype void update_vehicle_locations()
			{
				this_component()->template update_vehicle_locations<ComponentType,CallerType,TargetType>();
			}

			feature_prototype void update_in_network_vehicle_vht()
			{
				this_component()->template update_in_network_vehicle_vht<ComponentType,CallerType,TargetType>();
			}
		};
	}
}

using namespace Intersection_Components::Prototypes;