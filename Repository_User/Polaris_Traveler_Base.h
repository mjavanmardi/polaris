#pragma once
#include "Repository_User_Includes.h"
#include "Traveler_Interface.h"

namespace Traveler_Components
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
		struct Polaris_Traveler_Base
		{
			//========================================================================================================
			//start implementation of member_data(Routing_Interface*, routing)
			//--------------------------------------------------------------------------------------------------------
			template<typename ThisType, typename CallerType, typename TargetType>
			void router(TargetType set_value, call_requirements(requires(ThisType,Is_Dispatched))){_router = (void *)(set_value);}
			tag_setter(router);
			
			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType router(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType)(_router);}
			tag_getter(router);
			
			void* _router;
			typedef typename MasterType::routing_type router_type;


			//end
			//--------------------------------------------------------------------------------------------------------

			//========================================================================================================
			//start implementation of member_data(Vehicle_Interface*, vehicle)
			//--------------------------------------------------------------------------------------------------------

			template<typename ThisType, typename CallerType, typename TargetType>
			void vehicle(TargetType set_value, call_requirements(requires(ThisType,Is_Dispatched))){_vehicle = (void *)(set_value);}
			tag_setter(vehicle);
			
			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType vehicle(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType)(_vehicle);}
			tag_getter(vehicle);
			
			typedef typename MasterType::vehicle_type vehicle_type;

			void* _vehicle;
			//end
			//--------------------------------------------------------------------------------------------------------
		};
	}

	namespace Components
	{
		template<typename MasterType>
		struct Polaris_Traveler_Component
		{
			typedef Polaris_Component<Traveler_Components::Interfaces::Traveler_Interface, Traveler_Components::Bases::Polaris_Traveler_Base<MasterType>,NULLTYPE,MasterType> type;
		};
	}
}