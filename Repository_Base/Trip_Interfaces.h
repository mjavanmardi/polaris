#pragma once

#include "Repository_Base_Includes.h"
#include "Time_Interfaces.h"

//=============================================================
/// POLARIS TRIP INTERFACE
namespace Trip_Components
{
	namespace Types
	{
		typedef true_type TripType;
	}
	namespace Concepts
	{
		concept Is_Trip
		{
			begin_requirements_list;
			requires_typename_state(none,TripType,true_type,"Type is not a TripType");
			//requires_named_member(TripType, Start_Time,"Type does not have a 'Start Time' member");
			//requires_named_member(Start_Time, End_Time,"Type does not have a 'filename' member");
			//requires_named_member(End_Time, Origin,"Type does not have a 'filename' member");
			//requires_named_member(Origin, Destination,"Type does not have a 'filename' member");
			//requires_named_member(Destination, Write,"Type does not have a 'filename' member");
			end_requirements_list(TripType);
		};
	}
	namespace Interfaces
	{
		template<typename ThisType, typename CallerType>
		struct Trip_Interface
		{
			/// Tells everyone that this interface is a trip type
			typedef Types::TripType TripType ;
			typedef ThisType This_Type;

			/// Trip Initializers requires valid Time and Location Types
			facet void Initialize(typename TargetType::ParamType ID, typename TargetType::Param2Type start_time_val, typename TargetType::Param2Type end_time_val, typename TargetType::Param3Type origin, typename TargetType::Param3Type destination, call_requirements(
				(requires(ThisType, Is_Dispatchable) && requires(TargetType, Is_Target_Type_Struct))))
			{
				return PTHIS(ThisType)->Initialize<Dispatch<ThisType>,CallerType,TargetType>(ID,start_time_val, end_time_val, origin, destination);
			}
		

			facet void Write()
			{
				PTHIS(ThisType)->Write<Dispatch<ThisType>,CallerType,TargetType>();
			}

			facet_accessor(ID);
			facet_accessor_interface(Start_Time);
			facet_accessor_interface(End_Time);
			facet_accessor_interface(Origin);
			facet_accessor_interface(Destination);
		};
	}
	namespace Data_Structures
	{
	}
}
