#pragma once

#include "Repository_Base_Includes.h"

//=============================================================
/// POLARIS LOCATION INTERFACE
namespace Location_Components
{
	namespace Types
	{
		typedef true_type Transims_Location;
		typedef true_type Vadim_Location;
	}
	namespace Concepts
	{
		concept Is_Location
		{
			begin_requirements_list;

			requires_typename_state(none, LocationType, true_type,"This class has no LocationType = true_type defined");
			//requires_name(LocationType,location,"Has no 'location' member");
			end_requirements_list(LocationType);
		};
		concept Is_Vadim_Location
		{
			begin_requirements_list;
			requires_typename_state(none, Vadim_Location,true_type,"Type is not a Vadim Location");
			requires_typed_member(Vadim_Location,location,char*,"A Vadim_Location needs to have a string 'location' member.");
			end_requirements_list(location);
		};
		concept Is_Transims_Location
		{
			begin_requirements_list;
			requires_typename_state(none,Transims_Location,true_type,"Type is not a Transims Location");
			requires_typed_member(Transims_Location,location,int,"A Transims_Location needs to have an int 'location' member.");
			end_requirements_list(location);
		};
	}
	namespace Interfaces
	{
		/// Location Interface class
		struct Location_Interface
		{
			/// TypeTag for a location interface - checked by Is_Location concept
			typedef true_type LocationType;

			facet void Initialize(TargetType& location_struct, call_requires(ThisType,Is_Dispatchable))
			{
				return PTHIS(ThisType)->Initialize<Dispatch<ThisType>,CallerType,TargetType>(location_struct);
			}
			facet void Initialize(TargetType& location_struct, int(*guid_converter)(char*), call_requires(ThisType,Is_Dispatchable))
			{
				return PTHIS(ThisType)->Initialize<Dispatch<ThisType>,CallerType,TargetType>(location_struct, guid_converter);
			}

			facet void Write(call_requirements(requires(TargetType, is_arithmetic) || requires_2(TargetType,string,is_same)))
			{
				TargetType l = location<ThisType,CallerType,TargetType>();
				cout << l;
			}

			facet_accessor(location);
		};
	}
	namespace Data_Structures
	{
		struct Location_Struct
		{
			typedef Types::Transims_Location Transims_Location;
			int location;
		};
		
		struct VLocation_Struct
		{
			typedef Types::Vadim_Location Vadim_Location;
			char* location;
		};
	}
}
