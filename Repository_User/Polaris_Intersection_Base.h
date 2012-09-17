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

		template<typename MasterType>
		struct Polaris_Movement_Base
		{
		public:
			typedef typename MasterType::link_type link_type;
			typedef typename MasterType::vehicle_type vehicle_type;


			member_data_basic(float, movement_capacity);
			member_data_basic(float, movement_supply);
			member_data_basic(float, movement_demand);
			member_data_basic(float, movement_flow);
			member_data_basic(int, movement_transferred);

			member_data_basic(Intersection_Components::Types::Turn_Movement_Type_Keys, movement_type);
			member_data_basic(Intersection_Components::Types::Turn_Movement_Rule_Keys, movement_rule);

			member_data_basic(float, inbound_link_departure_time_based_experienced_link_turn_travel_delay);
			member_data_basic(float, outbound_link_arrival_time_based_experienced_link_turn_travel_delay);
			
			member_data_basic(int, turn_movement_cumulative_arrival_vehicles);
			member_data_basic(int, turn_movement_cumulative_vehicles);

			member_data_basic(int, turn_movement_cumulative_shifted_arrival_vehicles);
			
			member_data_basic(float, minimum_merge_rate);



			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType movement_reference(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType)_movement_reference;}
			tag_getter(movement_reference);

			void* _movement_reference;



			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType cached_outbound_link_arrival_time_based_experienced_link_turn_travel_delay_array(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType)&(_cached_outbound_link_arrival_time_based_experienced_link_turn_travel_delay_array.front());}
			tag_getter(cached_outbound_link_arrival_time_based_experienced_link_turn_travel_delay_array);

			vector<float> _cached_outbound_link_arrival_time_based_experienced_link_turn_travel_delay_array;



			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType cached_inbound_link_departure_time_based_experienced_link_turn_travel_delay_array(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType)&(_cached_inbound_link_departure_time_based_experienced_link_turn_travel_delay_array.front());}
			tag_getter(cached_inbound_link_departure_time_based_experienced_link_turn_travel_delay_array);

			vector<float> _cached_inbound_link_departure_time_based_experienced_link_turn_travel_delay_array;



			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType cached_outbound_link_departure_time_based_experienced_link_turn_travel_delay_array(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType)&(_cached_outbound_link_departure_time_based_experienced_link_turn_travel_delay_array.front());}
			tag_getter(cached_outbound_link_departure_time_based_experienced_link_turn_travel_delay_array);

			vector<float> _cached_outbound_link_departure_time_based_experienced_link_turn_travel_delay_array;



			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType cached_turn_movement_cumulative_shifted_arrival_vehicles_array(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType)&(_cached_turn_movement_cumulative_shifted_arrival_vehicles_array.front());}
			tag_getter(cached_turn_movement_cumulative_shifted_arrival_vehicles_array);

			vector<int> _cached_turn_movement_cumulative_shifted_arrival_vehicles_array;



			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType vehicles_container(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType)(_vehicles_container);}
			tag_getter(vehicles_container);

			typedef deque<void*> vehicles_container_type;
			typedef vehicle_type vehicles_container_element_type;

			vehicles_container_type _vehicles_container;
		};
		


		template<typename MasterType>
		struct Polaris_Outbound_Inbound_Movements_Base
		{
		public:
			typedef typename MasterType::link_type link_type;

			typedef typename MasterType::movement_type movements_element_type;

			typedef vector<void*> movements_container_type;



			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType outbound_movement_reference(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType)(_outbound_movement_reference);} tag_getter(outbound_movement_reference);
			
			void* _outbound_movement_reference;

			typedef link_type outbound_movement_reference_type;


			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType inbound_movements(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType)(_inbound_movements);} tag_getter(inbound_movements);

			movements_container_type _inbound_movements;
		};
		
		template<typename MasterType>
		struct Polaris_Inbound_Outbound_Movements_Base
		{
		public:
			typedef typename MasterType::link_type link_type;

			typedef Polaris_Component<Interfaces::Movement_Interface,Polaris_Movement_Base<MasterType>,NULLTYPE,MasterType> movements_element_type;

			typedef vector<Intersection_Components::Interfaces::Movement_Interface<movements_element_type, NULLTYPE>*> movements_container_type;


			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType inbound_movement_reference(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType)(_inbound_movement_reference);}
			tag_getter(inbound_movement_reference);

			movements_container_type _inbound_movement_reference;



			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType outbound_movements(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType)(_outbound_movements);}
			tag_getter(outbound_movement_reference);

			movements_container_type _outbound_movements;
		};
		

		template<typename MasterType>
		struct Polaris_Intersection_Base
		{
			typedef typename MasterType::link_type link_type;
			typedef typename MasterType::vehicle_type vehicle_type;

			typedef Polaris_Component<Interfaces::Outbound_Inbound_Movements_Interface,Polaris_Outbound_Inbound_Movements_Base<MasterType>,NULLTYPE,MasterType> outbound_inbound_movements_container_element_type;
			typedef vector<void*> outbound_inbound_movements_container_type;

			typedef Polaris_Component<Interfaces::Inbound_Outbound_Movements_Interface,Polaris_Inbound_Outbound_Movements_Base<MasterType>,NULLTYPE,MasterType> inbound_outbound_movements_container_element_type;
			typedef vector<void*> inbound_outbound_movements_container_type;

			typedef vector<void*> inbound_movements_container_type;
			typedef vector<void*> outbound_movements_container_type;
			
			
			typedef Polaris_Component<Interfaces::Movement_Interface,Polaris_Movement_Base<MasterType>,NULLTYPE,MasterType> inbound_movement_type;
			
			//typedef Vehicle_Components::Components::Polaris_Vehicle_Component vehicle_type; 
			//typedef Polaris_Movement_Base::vehicles_container_type vehicles_container_type;

			member_data_basic(int, uuid);
			member_data_basic(float, x_position);
			member_data_basic(float, y_position);
			member_data_basic(float, z_position);

			member_data_basic(Intersection_Components::Types::Intersection_Type_Keys, intersection_type);



			typedef vector<Link_Components::Interfaces::Link_Interface<link_type, NULLTYPE>*> LinkContainerType;

			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType inbound_links(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType)_inbound_links;} tag_getter(inbound_links);

			LinkContainerType _inbound_links;



			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType outbound_links(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType)_outbound_links;} tag_getter(outbound_links);

			LinkContainerType _outbound_links;



			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType outbound_inbound_movements(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType)(_outbound_inbound_movements);} tag_getter(outbound_inbound_movements);

			outbound_inbound_movements_container_type _outbound_inbound_movements;



			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType inbound_outbound_movements(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType)(_inbound_outbound_movements);} tag_getter(inbound_outbound_movements);

			inbound_outbound_movements_container_type _inbound_outbound_movements;


			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType scenario_reference(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType)(_scenario_reference);} tag_getter(scenario_reference);

			typedef typename MasterType::scenario_type scenario_type;
			void* _scenario_reference;
		};
	}

	namespace Components
	{
		template<typename MasterType>
		struct Polaris_Intersection_Component
		{
			typedef Polaris_Component_Execution<Interfaces::Intersection_Interface,Bases::Polaris_Intersection_Base<MasterType>,NULLTYPE,MasterType> type;
		};

		template<typename MasterType>
		struct Polaris_Movement_Component
		{
			typedef Polaris_Component<Interfaces::Movement_Interface, Bases::Polaris_Movement_Base<MasterType>,NULLTYPE,MasterType> type;
		};
		
		template<typename MasterType>
		struct Polaris_Outbound_Inbound_Movements_Component
		{
			typedef Polaris_Component<Interfaces::Outbound_Inbound_Movements_Interface, Bases::Polaris_Outbound_Inbound_Movements_Base<MasterType>,NULLTYPE,MasterType> type;
		};
		
		template<typename MasterType>
		struct Polaris_Inbound_Outbound_Movements_Component
		{
			typedef Polaris_Component<Interfaces::Inbound_Outbound_Movements_Interface, Bases::Polaris_Inbound_Outbound_Movements_Base<MasterType>,NULLTYPE,MasterType> type;
		};

		//typedef Polaris_Component<Intersection_Components::Interfaces::Intersection_Interface, Intersection_Components::Bases::Polaris_Intersection_Base> Polaris_Intersection_Component;
		//typedef Polaris_Component<Intersection_Components::Interfaces::Movement_Interface, Intersection_Components::Bases::Polaris_Movement_Base> Polaris_Movement_Component;
		//typedef Polaris_Component<Intersection_Components::Interfaces::Outbound_Inbound_Movements_Interface, Intersection_Components::Bases::Polaris_Outbound_Inbound_Movements_Base> Polaris_Outbound_Inbound_Movements_Component;
		//typedef Polaris_Component<Intersection_Components::Interfaces::Inbound_Outbound_Movements_Interface, Intersection_Components::Bases::Polaris_Inbound_Outbound_Movements_Base> Polaris_Inbound_Outbound_Movements_Component;
	}	

}
