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
		public:
			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType routing(TargetType set_value, call_requirements(requires(ThisType,Is_Dispatched)))
			{
				_routing = (void *)(set_value);
			}
			
			tag_setter(routing);
			
			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType routing(call_requirements(requires(ThisType,Is_Dispatched)))
			{
				return (TargetType)(_routing);
			}

			tag_getter(routing);
			
			typedef typename MasterType::routing_type routing_type;

		protected:
			void* _routing;
			//end
			//--------------------------------------------------------------------------------------------------------

			//========================================================================================================
			//start implementation of member_data(Vehicle_Interface*, vehicle)
			//--------------------------------------------------------------------------------------------------------
		public:
			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType vehicle(TargetType set_value, call_requirements(requires(ThisType,Is_Dispatched)))
			{
				_vehicle = (void *)(set_value);
			}
			
			tag_setter(routing);
			
			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType routing(call_requirements(requires(ThisType,Is_Dispatched)))
			{
				return (TargetType)(_vehicle);
			}

			tag_getter(vehicle);
			
			typedef typename MasterType::vehicle_type vehicle_type;

		protected:
			void* _vehicle;
			//end
			//--------------------------------------------------------------------------------------------------------
		};
	}

	namespace Components
	{
		typedef Polaris_Component<Traveler_Components::Interfaces::Traveler_Interface, Traveler_Components::Bases::Polaris_Traveler_Base> Polaris_Traveler_Component;
	}
}