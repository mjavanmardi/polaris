#pragma once
#include "Traveler_Simulator_Includes.h"

namespace Traveler_Components
{

	namespace Prototypes
	{
		prototype struct Traveler ADD_DEBUG_INFO
		{
			tag_as_prototype;

			accessor(router, NONE, NONE);
			accessor(vehicle, NONE, NONE);
			accessor(uuid, NONE, NONE);
			accessor(internal_id, NONE, NONE);
			accessor(plan, NONE, NONE);

			template<typename TargetType> void Initialize();
			//accessor(Platooning_Faculty, NONE, NONE);

			void Schedule_New_Departure(int departed_time){this_component()->Schedule_New_Departure(departed_time);}
		};

		template<typename ComponentType>
		template<typename TargetType>
		void Traveler<ComponentType>::Initialize()
		{
			this_component()->template Initialize<TargetType>();
		}
	}

	

}

using namespace Traveler_Components::Prototypes;
