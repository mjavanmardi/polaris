#pragma once

#include "Location_API.h"


namespace Location_API
{
	class Transims_Location_Base
	{
		facet void Initialize(TargetType& location_struct, call_requirements(requires(ThisType,Is_Dispatched) && requires(TargetType,Concepts::Is_Transims_Location)))
		{
			_location_id = location_struct.location;
		}
		facet void Initialize(TargetType& location_struct, call_requirements(requires(ThisType,!Is_Dispatched) || requires(TargetType,!Concepts::Is_Transims_Location)))
		{
			assert_requirements(TargetType,Is_Transims_Location,"TargetType is not a transims location - required to use this version of initialize");
			assert_requirements(TargetType,Is_Dispatched,"TargetType is not dispatched");
		}

		tag_getter(location);
		declare_facet_getter(location,true)
		{
			return _location_id;
		}
		

	protected:
		int _location_id;
	};
	typedef Polaris_Component<Interfaces::Location_Interface,Transims_Location_Base,NULLTYPE,NULLTYPE,Data_Object> Location;
	


	class Vadim_Location_Base
	{
		facet void Initialize(TargetType& location_struct, int(*Guid_Converter)(char*), call_requirements(requires(ThisType,Is_Dispatched) && requires(TargetType,Concepts::Is_Vadim_Location)))
		{
			_location_id = Guid_Converter(location_struct.location);
		}
		facet void Initialize(TargetType& location_struct, int(*Guid_Converter)(char*), call_requirements(requires(ThisType,!Is_Dispatched) || requires(TargetType,!Concepts::Is_Vadim_Location)))
		{
			assert_requirements(TargetType,Concepts::Is_Vadim_Location,"Your TargetType is not a Vadim location -");
			assert_requirements(TargetType,Is_Dispatched,"TargetType is not dispatched");
		}
		declare_facet_getter(location,true)
		{
			return _location_id;
		}

	protected:
		int _location_id;
	};
	typedef Polaris_Component<Interfaces::Location_Interface,Vadim_Location_Base,NULLTYPE,NULLTYPE,Data_Object> Vadim_Location;
}
