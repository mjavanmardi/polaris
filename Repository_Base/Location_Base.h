#pragma once

#include "Location_Interfaces.h"


namespace Location_Components
{
	namespace Bases
	{
		struct Transims_Location_Base
		{
			/// TypeTag for a location interface - checked by Is_Location concept
			typedef true_type LocationType;


			facet_base void Initialize(TargetType location_struct, call_requirements(requires(ThisType,Is_Dispatched) && requires(TargetType,Concepts::Is_Transims_Location)))
			{
				_location = location_struct;
			}
			facet_base void Initialize(TargetType location_struct, call_requirements(requires(ThisType,!Is_Dispatched) || requires(TargetType,!Concepts::Is_Transims_Location)))
			{
				assert_requirements(TargetType,Is_Transims_Location,"TargetType is not a transims location - required to use this version of initialize");
				assert_requirements(TargetType,Is_Dispatched,"TargetType is not dispatched");
			}

			member_data(int, location, requires(TargetType,is_arithmetic),false);
		};
	}

	namespace Components
	{
		typedef Polaris_Component<Interfaces::Location_Interface,Bases::Transims_Location_Base,NULLTYPE,NULLTYPE,Data_Object> Location;
	}	
}
