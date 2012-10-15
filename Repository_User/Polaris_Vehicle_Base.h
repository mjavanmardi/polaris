#pragma once
#include "Repository_User_Includes.h"
#include "Repository_User\Vehicle_Interface.h"
#include "Polaris_Link_Base.h"

namespace Vehicle_Components
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
		struct Polaris_Trajectory_Unit_Base
		{
			member_data_basic(int, delayed_time);
			member_data_basic(int, enter_time);
			member_data_basic(int, enter_interval_index);
			
			//========================================================================================================
			//start implementation of member_data(LinkInterface*, link)
			//--------------------------------------------------------------------------------------------------------
		
			template<typename ThisType, typename CallerType, typename TargetType>
			void link(TargetType set_value, call_requirements(requires(ThisType,Is_Dispatched))){_link = (void*)set_value;}
			tag_setter(link);

			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType link(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType)(_link);}
			tag_getter(link);

			typedef typename MasterType::link_type link_type;


			void* _link;
			//end
			//--------------------------------------------------------------------------------------------------------


		};

		template<typename MasterType>
		struct Polaris_Vehicle_Base
		{
			member_data_basic(int, current_trajectory_unit_index);
			member_data_basic(int, departed_simulation_interval_index);
			member_data_basic(int, departed_assignment_interval_index);
			member_data_basic(void*, origin_link);
			member_data_basic(void*, destination_link);
			member_data_basic(void*, next_link);
			member_data_basic(Vehicle_Components::Types::Vehicle_Status_Keys, simulation_status);
			member_data_basic(int, arrived_time);
			member_data_basic(int, arrived_simulation_interval_index);
			member_data_basic(int, current_link_enter_time);
			member_data_basic(int, current_link_enter_interval_index);
			member_data_basic(int, uuid);

			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType trajectory_container(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType)(_trajectory_container);} tag_getter(trajectory_container);

			typedef Polaris_Component<Interfaces::Trajectory_Unit_Interface, Bases::Polaris_Trajectory_Unit_Base<MasterType>,NULLTYPE,MasterType> trajectory_unit_type;

			typedef vector<void*> trajectory_container_type;

			trajectory_container_type _trajectory_container;

			typedef trajectory_container_type trajectory_type;
			typedef trajectory_unit_type trajectory_element_type; 

			template<typename ThisType, typename CallerType, typename TargetType>
			void origin_activity_location(TargetType set_value, call_requirements(requires(ThisType,Is_Dispatched))){_origin_activity_location = (void *)(set_value);}
			tag_setter(origin_activity_location);
			
			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType origin_activity_location(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType)(_origin_activity_location);}
			tag_getter(origin_activity_location);
			
			void* _origin_activity_location;

			template<typename ThisType, typename CallerType, typename TargetType>
			void destination_activity_location(TargetType set_value, call_requirements(requires(ThisType,Is_Dispatched))){_destination_activity_location = (void *)(set_value);}
			tag_setter(destination_activity_location);
			
			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType destination_activity_location(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType)(_destination_activity_location);}
			tag_getter(destination_activity_location);
			
			void* _destination_activity_location;

		};
	}

	namespace Components
	{
		template<typename MasterType>
		struct Polaris_Vehicle_Component
		{
			typedef Polaris_Component<Interfaces::Vehicle_Interface, Bases::Polaris_Vehicle_Base<MasterType>,NULLTYPE,MasterType> type;
		};

		template<typename MasterType>
		struct Polaris_Trajectory_Unit_Component
		{
			typedef Polaris_Component<Interfaces::Trajectory_Unit_Interface, Bases::Polaris_Trajectory_Unit_Base<MasterType>,NULLTYPE,MasterType> type;
		};
	}	
}