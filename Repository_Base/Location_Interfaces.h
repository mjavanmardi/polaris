#pragma once

#include "Repository_Base_Includes.h"

//=============================================================
/// POLARIS LOCATION INTERFACE
namespace Location_Components
{
	namespace Types
	{
		typedef true_type Transims_Location;
	}
	namespace Concepts
	{
		concept Is_Location
		{
			begin_requirements_list;
			requires_typename_state(none, LocationType, true_type,"This class has no LocationType = true_type defined");
			end_requirements_list(LocationType);
		};

		concept Is_Transims_Location
		{
			begin_requirements_list;
			requires_typename_state(none,Location,true_type,"Type is not a Location");
			requires_typename_state(Location,Transims,true_type,"Type is not a Transims Location");
			//requires_typed_member(Transims,value,int,"A Transims_Location needs to have an int 'location' member.");
			end_requirements_list(Transims);
		};
	}
	namespace Interfaces
	{
		/// Location Interface class
		template<typename ComponentType, typename CallerType>
		struct Location_Interface
		{
			typedef ComponentType This_Type;
			typedef true_type LocationType;

			facet void Initialize(TargetType location_struct, call_requires(ComponentType,Is_Dispatchable))
			{
				return PTHIS(ComponentType)->Initialize<Dispatch<ComponentType>,CallerType,TargetType>(location_struct);
			}
			facet void Write(call_requirements(requires(TargetType, is_arithmetic)))
			{
				TargetType l = location<TargetType>();
				cout << l;
			}
			facet_accessor(location);
		};
	}
	namespace Data_Structures
	{
		Basic_Data_Struct(Location_Struct,int,Location,Transims);
	}
}
