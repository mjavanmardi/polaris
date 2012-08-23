#pragma once

#include "Repository_Base_Includes.h"
#include "Time_API.h"

//=============================================================
/// POLARIS TRIP INTERFACE
namespace Trip_API
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
		struct Trip_Interface
		{
			/// Tells everyone that this interface is a trip type
			typedef Types::TripType TripType ;

			/// Trip Initializers requires valid Time and Location Types
			template<typename ThisType, typename CallerType, typename TimeType, typename LocationType>
			void Initialize(int ID, typename TimeType::Interface_Type* start_time_val, typename TimeType::Interface_Type* end_time_val, typename LocationType::Interface_Type* origin, typename LocationType::Interface_Type* destination, call_requirements(
				(requires(ThisType, Is_Dispatchable))))
			{
				return PTHIS(ThisType)->Initialize<Dispatch<ThisType>,CallerType,TimeType,LocationType>(ID,start_time_val, end_time_val, origin, destination);
			}
			template<typename ThisType, typename CallerType, typename IDType, typename TimeStructType, typename LocationStructType>
			void Initialize(IDType ID, TimeStructType start_time_struct, TimeStructType end_time_struct, LocationStructType origin_struct, LocationStructType destination_struct, call_requirements(
				(requires(ThisType, Is_Dispatchable))))
			{
				return PTHIS(ThisType)->Initialize<Dispatch<ThisType>,CallerType,IDType,TimeStructType,LocationStructType>(ID,start_time_struct, end_time_struct, origin_struct, destination_struct);
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
}
