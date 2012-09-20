#pragma once

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
		struct Routable_Movement_Base
		{
			member_data_basic(float, forward_link_turn_travel_time);
			
			template<typename ThisType, typename CallerType, typename TargetType>
			void movement_reference(TargetType set_value, call_requirements(requires(ThisType,Is_Dispatched))){_movement_reference = (void *)(set_value);} 
			tag_setter(movement_reference);
			
			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType movement_reference(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType)(_movement_reference);} 
			tag_getter(movement_reference);
			
			void* _movement_reference;
		};
			
		//template<typename MasterType>
		//struct Routable_Outbound_Inbound_Movements_Base
		//{

		//	typedef typename MasterType::link_type link_type;

		//	typedef typename MasterType::movement_type movements_element_type;

		//	typedef vector<void*> movements_container_type;

		//	//--------------------------------------------------------
		//	// member_data<void*, outbound_movement_reference>
		//	//--------------------------------------------------------
		//	template<typename ThisType, typename CallerType, typename TargetType>
		//	void outbound_movement_reference(TargetType set_value, call_requirements(requires(ThisType,Is_Dispatched))){_outbound_movement_reference = (void *)(set_value);} 
		//	tag_setter(outbound_movement_reference);
		//	
		//	template<typename ThisType, typename CallerType, typename TargetType>
		//	TargetType outbound_movement_reference(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType)(_outbound_movement_reference);} 
		//	tag_getter(outbound_movement_reference);
		//	
		//	void* _outbound_movement_reference;
		//	//--------------------------------------------------------
		//};

		template<typename MasterType>
		struct Routable_Inbound_Outbound_Movements_Base
		{

			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType inbound_link_reference(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType)(_inbound_link_reference);}
			tag_setter(inbound_link_reference);
			template<typename ThisType, typename CallerType, typename TargetType>
			void inbound_link_reference(TargetType set_value, call_requirements(requires(ThisType,Is_Dispatched))){_inbound_link_reference = (void*)set_value;}
			tag_getter(inbound_link_reference);
			
			void* _inbound_link_reference;

			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType outbound_movements(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType)(_outbound_movements);}
			tag_getter(outbound_movements);

			vector<void*> _outbound_movements;
		};

		template<typename MasterType>
		struct Routable_Intersection_Base
		{
			member_data_basic(float, x_position);
			member_data_basic(float, y_position);
			member_data_basic(float, z_position);

			//typedef Polaris_Component<Interfaces::Outbound_Inbound_Movements_Interface,Routable_Outbound_Inbound_Movements_Base<MasterType>,NULLTYPE,MasterType> outbound_inbound_movements_container_element_type;
			//typedef vector<void*> outbound_inbound_movements_container_type;
			//
			////--------------------------------------------------------
			//// member_data<vector<void*>, outbound_inbound_movements>
			////--------------------------------------------------------

			//template<typename ThisType, typename CallerType, typename TargetType>
			//TargetType outbound_inbound_movements(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType)(_outbound_inbound_movements);} 
			//tag_getter(outbound_inbound_movements);

			//outbound_inbound_movements_container_type _outbound_inbound_movements;
			////--------------------------------------------------------

			typedef Polaris_Component<Interfaces::Inbound_Outbound_Movements_Interface,Routable_Inbound_Outbound_Movements_Base<MasterType>,NULLTYPE,MasterType> inbound_outbound_movements_container_element_type;
			typedef vector<void*> inbound_outbound_movements_container_type;
			
			//--------------------------------------------------------
			// member_data<vector<void*>, inbound_outbound_movements>
			//--------------------------------------------------------

			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType inbound_outbound_movements(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType)(_inbound_outbound_movements);} 
			tag_getter(inbound_outbound_movements);

			inbound_outbound_movements_container_type _inbound_outbound_movements;
			//--------------------------------------------------------

			typedef typename vector<void*> movements_container_type;
			typedef typename MasterType::routable_movement_type movements_container_element_type;
		};
	}

	namespace Components
	{
		template<typename MasterType>
		struct Routable_Intersection_Component
		{
			typedef Polaris_Component<Intersection_Components::Interfaces::Intersection_Interface, Intersection_Components::Bases::Routable_Intersection_Base<MasterType>, NULLTYPE, MasterType> type;
		};

		//template<typename MasterType>
		//struct Routable_Outbound_Inbound_Movements_Component
		//{
		//	typedef Polaris_Component<Intersection_Components::Interfaces::Outbound_Inbound_Movements_Interface, Intersection_Components::Bases::Routable_Outbound_Inbound_Movements_Base<MasterType>, NULLTYPE, MasterType> type;
		//};
		template<typename MasterType>
		struct Routable_Movement_Component
		{
			typedef Polaris_Component<Intersection_Components::Interfaces::Movement_Interface, Intersection_Components::Bases::Routable_Movement_Base<MasterType>, NULLTYPE, MasterType> type;
		};
		template<typename MasterType>
		struct Routable_Inbound_Outbound_Movements_Component
		{
			typedef Polaris_Component<Intersection_Components::Interfaces::Inbound_Outbound_Movements_Interface, Intersection_Components::Bases::Routable_Inbound_Outbound_Movements_Base<MasterType>, NULLTYPE, MasterType> type;
		};
	}	

}
