#pragma once
#include "Repository_User_Includes.h"
#include "Link_Interface.h"
#include "Vehicle_Interface.h"
#include "Repository_User\Polaris_Link_Base.h"
#include "Intersection_Interface.h"

namespace Intersection_Components
{
	namespace Types
	{
	
	}

	namespace Concepts
	{
	}
	
	namespace Bases
	{

		struct Polaris_Movement_Base
		{

			//========================================================================================================
			//start implementation of memeber_data(Link_Interface*, movement_reference)
			//--------------------------------------------------------------------------------------------------------
		public:
			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType movement_reference(call_requirements(requires(ThisType,Is_Dispatched)))
			{
				return (TargetType)_movement_reference;
			}
 
			tag_getter(movement_reference);
	
		protected:
			Link_Components::Interfaces::Link_Interface<Link_Components::Components::Polaris_Link_Component, NULLTYPE>* _movement_reference;
			//end
			//--------------------------------------------------------------------------------------------------------

			member_data_basic(float, movement_capacity);
			member_data_basic(float, movement_supply);
			member_data_basic(float, movement_demand);
			member_data_basic(float, movement_flow);
			member_data_basic(int, movement_transferred);

			member_data_basic(Intersection_Components::Types::Turn_Movement_Type_Keys, movement_type);
			member_data_basic(Intersection_Components::Types::Turn_Movement_Rule_Keys, movement_rule);

			//========================================================================================================
			//start implementation of memeber_data(vector<float>, cached_outbound_link_arrival_time_based_experienced_link_turn_travel_delay_array)
			//--------------------------------------------------------------------------------------------------------
		public:
			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType cached_outbound_link_arrival_time_based_experienced_link_turn_travel_delay_array(call_requirements(requires(ThisType,Is_Dispatched)))
			{
				return (TargetType)&(_cached_outbound_link_arrival_time_based_experienced_link_turn_travel_delay_array.front());
			}

			tag_getter(cached_outbound_link_arrival_time_based_experienced_link_turn_travel_delay_array);
	
		protected:
			vector<float> _cached_outbound_link_arrival_time_based_experienced_link_turn_travel_delay_array;
			//--------------------------------------------------------------------------------------------------------

			//========================================================================================================
			//start implementation of memeber_data(vector<float>, cached_inbound_link_departure_time_based_experienced_link_turn_travel_delay_array)
			//--------------------------------------------------------------------------------------------------------
		public:
			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType cached_inbound_link_departure_time_based_experienced_link_turn_travel_delay_array(call_requirements(requires(ThisType,Is_Dispatched)))
			{
				return (TargetType)&(_cached_inbound_link_departure_time_based_experienced_link_turn_travel_delay_array.front());
			}

			tag_getter(cached_inbound_link_departure_time_based_experienced_link_turn_travel_delay_array);
	
		protected:
			vector<float> _cached_inbound_link_departure_time_based_experienced_link_turn_travel_delay_array;
			//--------------------------------------------------------------------------------------------------------

			//========================================================================================================
			//start implementation of memeber_data(vector<float>, cached_outbound_link_departure_time_based_experienced_link_turn_travel_delay_array)
			//--------------------------------------------------------------------------------------------------------
		public:
			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType cached_outbound_link_departure_time_based_experienced_link_turn_travel_delay_array(call_requirements(requires(ThisType,Is_Dispatched)))
			{
				return (TargetType)&(_cached_outbound_link_departure_time_based_experienced_link_turn_travel_delay_array.front());
			}

			tag_getter(cached_outbound_link_departure_time_based_experienced_link_turn_travel_delay_array);
	
		protected:
			vector<float> _cached_outbound_link_departure_time_based_experienced_link_turn_travel_delay_array;
			//--------------------------------------------------------------------------------------------------------

			member_data_basic(float, inbound_link_departure_time_based_experienced_link_turn_travel_delay);
			member_data_basic(float, outbound_link_arrival_time_based_experienced_link_turn_travel_delay);
			
			member_data_basic(int, turn_movement_cumulative_arrival_vehicles);
			member_data_basic(int, turn_movement_cumulative_vehicles);

			//========================================================================================================
			//start implementation of memeber_data(vector<float>, cached_turn_movement_cumulative_shifted_arrival_vehicles_array)
			//--------------------------------------------------------------------------------------------------------
		public:
			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType cached_turn_movement_cumulative_shifted_arrival_vehicles_array(call_requirements(requires(ThisType,Is_Dispatched)))
			{
				return (TargetType)&(_cached_turn_movement_cumulative_shifted_arrival_vehicles_array.front());
			}

			tag_getter(cached_turn_movement_cumulative_shifted_arrival_vehicles_array);
	
		protected:
			vector<int> _cached_turn_movement_cumulative_shifted_arrival_vehicles_array;
			//--------------------------------------------------------------------------------------------------------
			
			member_data_basic(int, turn_movement_cumulative_shifted_arrival_vehicles);

			//========================================================================================================
			//start implementation of memeber_data(vector<Vehicle_Interface>, vehicles_container)
			//--------------------------------------------------------------------------------------------------------			
		public:
			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType vehicles_container(call_requirements(requires(ThisType,Is_Dispatched)))
			{
				return (TargetType)(_vehicles_container);
			}

			tag_getter(vehicles_container);
			typedef vector<Vehicle_Components::Interfaces::Vehicle_Interface<Vehicle_Components::Components::Polaris_Vehicle_Component,NULLTYPE>*> vehicles_container_type;
		protected:
			vehicles_container_type _vehicles_container;
			//--------------------------------------------------------------------------------------------------------

			member_data_basic(float, minimum_merge_rate);
		};

		struct Polaris_Outbound_Inbound_Movements_Base
		{

			//========================================================================================================
			//start implementation of memeber_data(Link_Interface*, outbound_movement_reference)
			//--------------------------------------------------------------------------------------------------------
		public:
			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType outbound_movement_reference(call_requirements(requires(ThisType,Is_Dispatched)))
			{
				return (TargetType)(_outbound_movement_reference);
			}
			typedef Link_Components::Interfaces::Link_Interface<Link_Components::Components::Polaris_Link_Component, NULLTYPE>* outbound_movement_reference_type;
			tag_getter(outbound_movement_reference);
	
		protected:
			outbound_movement_reference_type _outbound_movement_reference;
			//--------------------------------------------------------------------------------------------------------

			//========================================================================================================
			//start implementation of memeber_data(Vector<Movement_Interface*>, inbound_movements)
			//--------------------------------------------------------------------------------------------------------
		public:
			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType inbound_movements(call_requirements(requires(ThisType,Is_Dispatched)))
			{
				return (TargetType)(_inbound_movements);
			}
			typedef vector<Intersection_Components::Interfaces::Intersection_Interface<Intersection_Components::Components::Polaris_Movement_Component, NULLTYPE>*> inbound_movements_type;
			tag_getter(outbound_movement_reference);
	
		protected:
			inbound_movements_type _inbound_movements;
			//--------------------------------------------------------------------------------------------------------
		};
		
		struct Polaris_Intersection_Base
		{
			typedef Polaris_Outbound_Inbound_Movements_Base::inbound_movements_type inbound_movements_type;
			typedef Polaris_Movement_Base inbound_movement_type;
			typedef Vehicle_Components::Components::Polaris_Vehicle_Component vehicle_type; 
			typedef Polaris_Movement_Base::vehicles_container_type vehicles_container_type;

			member_data_basic(int, uuid);

			typedef vector<Link_Components::Interfaces::Link_Interface<Link_Components::Components::Polaris_Link_Component, NULLTYPE>*> LinkContainerType;
			//========================================================================================================
			//start implementation of memeber_data(vector<Link_Interface>, inbound_links)
			//--------------------------------------------------------------------------------------------------------
		public:
			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType inbound_links(call_requirements(requires(ThisType,Is_Dispatched)))
			{
				return (TargetType)_inbound_links;
			}

			tag_getter(inbound_links);
			
		protected:
			LinkContainerType _inbound_links;
			//end
			//--------------------------------------------------------------------------------------------------------

			//========================================================================================================
			//start implementation of memeber_data(vector<Link_Interface>, outbound_links)
			//--------------------------------------------------------------------------------------------------------
		public:
			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType outbound_links(call_requirements(requires(ThisType,Is_Dispatched)))
			{
				return (TargetType)_outbound_links;
			}

			tag_getter(outbound_links);
			
		protected:
			LinkContainerType _outbound_links;
			//end
			//--------------------------------------------------------------------------------------------------------

			//========================================================================================================
			//start implementation of memeber_data(Vector<Outbound_Inbound_Movements_Interface*>, outbound_inbound_movements)
			//--------------------------------------------------------------------------------------------------------
		public:
			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType outbound_inbound_movements(call_requirements(requires(ThisType,Is_Dispatched)))
			{
				return (TargetType)(_outbound_inbound_movements);
			}
			typedef vector<Intersection_Components::Interfaces::Outbound_Inbound_Movements_Interface<Intersection_Components::Components::Polaris_Outbound_Inbound_Movements_Component, NULLTYPE>*> outbound_inbound_movements_type;
			tag_getter(outbound_inbound_movements);
	
		protected:
			outbound_inbound_movements_type _outbound_inbound_movements;
			//--------------------------------------------------------------------------------------------------------

			member_data_basic(float, x_position);
			member_data_basic(float, y_position);
			member_data_basic(float, z_position);

			member_data_basic(Intersection_Components::Types::Node_Type_Keys, intersection_type);

			//========================================================================================================
			//start implementation of memeber_data(Scenario_Interface*, scenario_reference)
			//--------------------------------------------------------------------------------------------------------
		public:
			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType scenario_reference(call_requirements(requires(ThisType,Is_Dispatched)))
			{
				return (TargetType)(_scenario_reference);
			}

			tag_getter(scenario_reference);
			typedef Scenario_Components::Interfaces::Scenario_Interface<Scenario_Components::Components::Polaris_Scenario_Component, NULLTYPE>* scenario_reference_type;
		protected:
			scenario_reference_type _scenario_reference;
			//--------------------------------------------------------------------------------------------------------
		};
	}

	namespace Components
	{
		typedef Polaris_Component<Intersection_Components::Interfaces::Intersection_Interface, Intersection_Components::Bases::Polaris_Intersection_Base> Polaris_Intersection_Component;
		typedef Polaris_Component<Intersection_Components::Interfaces::Movement_Interface, Intersection_Components::Bases::Polaris_Movement_Base> Polaris_Movement_Component;
		typedef Polaris_Component<Intersection_Components::Interfaces::Outbound_Inbound_Movements_Interface, Intersection_Components::Bases::Polaris_Outbound_Inbound_Movements_Base> Polaris_Outbound_Inbound_Movements_Component;
	}	

}
