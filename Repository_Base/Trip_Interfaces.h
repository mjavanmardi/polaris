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
		template<typename ComponentType, typename CallerType>
		struct Trip_Interface
		{
			/// Tells everyone that this interface is a trip type
			typedef Types::TripType TripType ;
			typedef ComponentType This_Type;

			/// Trip Initializers requires valid Time and Location Types
			facet void Initialize(typename TargetType::ParamType ID, typename TargetType::Param2Type start_time_val, typename TargetType::Param2Type end_time_val, typename TargetType::Param3Type origin, typename TargetType::Param3Type destination, call_requirements(
				(requires(ComponentType, Is_Dispatchable) && requires(TargetType, Is_Target_Type_Struct))))
			{
				return PTHIS(ComponentType)->Initialize<Dispatch<ComponentType>,CallerType,TargetType>(ID,start_time_val, end_time_val, origin, destination);
			}
		

			facet void Write()
			{
				PTHIS(ComponentType)->Write<Dispatch<ComponentType>,CallerType,TargetType>();
			}

			feature_accessor(ID);
			feature_accessor(Start_Time);
			feature_accessor(End_Time);
			feature_accessor(Origin);
			feature_accessor(Destination);
		};
	}
	namespace Data_Structures
	{
	}
}
