#pragma once

#include "Repository_Base_Includes.h"

//=============================================================
/// POLARIS TIME CLASS INTERFACE
///
namespace Time_API
{
	//=============================================================
	/// Useful types related to Time.
	/// These types are used to tag the Data_Structures to communicate information and in concept checking
	namespace Types
	{
		typedef true_type TimeType;
		namespace Time_Types
		{
			typedef true_type Days;
			typedef true_type Hours;
			typedef true_type Minutes;
			typedef true_type Seconds;
			typedef true_type DRSeconds; 
		}

	}
	//=============================================================
	/// Concepts related to Time.
	/// These concepts check if a data structure is an appropriate time initializer, if a class is an acceptable Time class, etc.
	/// The time type checking concepts are used to overload initializers for different types of input time
	namespace Concepts
	{
		/// Check if the class has an arithmetic 'Time' member defined
		concept Has_Time
		{
			begin_requirements_list;
			//requires_named_member(none,Time,"An arithmetic 'Time' data member has not been defined.");
			requires_concept_checked_member(none, Time, is_arithmetic,"An arithmetic 'Time' data member has not been defined.");

			end_requirements_list(Time);
		};
		/// Check if initializing struct is tagged as DRSeconds (i.e. 1/10th of a second)
		concept Time_In_DRSeconds
		{
			begin_requirements_list;

			requires_typename_state(none,DRSeconds,true_type,"Type is not in DRSeconds");
	
			end_requirements_list(DRSeconds);
		};
		/// Check if initializing struct is tagged as Seconds
		concept Time_In_Seconds
		{
			begin_requirements_list;

			requires_typename_state(none,Seconds,true_type,"Type is not in Seconds");
	
			end_requirements_list(Seconds);
		};
		/// Check if initializing struct is tagged as Minutes
		concept Time_In_Minutes
		{
			begin_requirements_list;

			requires_typename_state(none,Minutes,true_type,"Type is not in Minutes");
	
			end_requirements_list(Minutes);
		};
		/// Check if initializing struct is tagged as Hours
		concept Time_In_Hours
		{
			begin_requirements_list;

			requires_typename_state(none,Hours,true_type,"Type is not in Hours");
	
			end_requirements_list(Hours);
		};
		/// Check if initializing struct is tagged as Days
		concept Time_In_Days
		{
			begin_requirements_list;

			requires_typename_state(none,Days,true_type,"Type is not in Hours");
	
			end_requirements_list(Days);
		};
		/// Check if a type meets the requirements of being a basic 'Time' class
		concept Is_Time
		{
			begin_requirements_list;
			requires_typename_state(none, TimeType, true_type,"This class has no TimeType = true_type defined");
			end_requirements_list(TimeType);
		};
	}
	//=============================================================
	/// Time Interface(s).
	/// Various Time interfaces which can be used to create Time components
	namespace Interfaces
	{
		//=============================================================
		/// Basic Time Interface
		/// This is the core time object in polaris.  It implements, initialization, addition, display and conversion routines
		struct Time_Interface
		{
			/// Communicates that this time interface is a time type
			typedef true_type TimeType;

			/// Initializer
			facet void Initialize(TargetType& t, call_requires(ThisType,Is_Dispatchable))
			{
				return PTHIS(ThisType)->Initialize<Dispatch<ThisType>,CallerType,TargetType>(t);
			}

			/// Adding time
			facet void Add_Time(TargetType& t, call_requires(ThisType,Is_Dispatchable))
			{
				return PTHIS(ThisType)->Add_Time<Dispatch<ThisType>,CallerType,TargetType>(t);
			}

			/// Display the time
			facet void Write()
			{
				Time_Interface* pthis = PTHIS(ThisType);
				//cout <<"Total Seconds: "<<pthis->Total_Seconds<ThisType, CallerType, TargetType>()<<endl;
				cout << "Day "<< pthis->Days<ThisType, CallerType, TargetType>()<<":  ";
				cout << pthis->Hours<ThisType, CallerType, TargetType>()<<":"<<pthis->Minutes<ThisType, CallerType, TargetType>()<<":"<<pthis->Seconds<ThisType, CallerType, TargetType>()<<"."<< pthis->DRseconds<ThisType, CallerType, TargetType>();
			}

			//===========================================================================
			/// Convert base time to Days.  Requires that Time_Base has a 'Total_Seconds' accessor to convert base time to seconds
			template <typename ThisType, typename CallerType, typename OutputTimeStructType>
			OutputTimeStructType Convert_Time(call_requirements(requires(typename OutputTimeStructType::ValueType, is_arithmetic) && requires(OutputTimeStructType,Time_API::Concepts::Time_In_Days)))
			{
				OutputTimeStructType t;
				t.Time = this->Total_Seconds<ThisType, ThisType, OutputTimeStructType::Value_Type>() / (OutputTimeStructType::Value_Type)86400.0;
				return t ;
			}
			/// Convert base time to Hours.  Requires that Time_Base has a 'Total_Seconds' accessor to convert base time to seconds
			template <typename ThisType, typename CallerType, typename OutputTimeStructType>
			OutputTimeStructType Convert_Time(call_requirements(requires(typename OutputTimeStructType::ValueType, is_arithmetic) && requires(OutputTimeStructType,Time_API::Concepts::Time_In_Hours)))
			{
				OutputTimeStructType t;
				t.Time = this->Total_Seconds<ThisType, ThisType, OutputTimeStructType::ValueType>() / (OutputTimeStructType::ValueType)1440.0;
				return t ;
			}
			/// Convert base time to Minutes.  Requires that Time_Base has a 'Total_Seconds' accessor to convert base time to seconds
			template <typename ThisType, typename CallerType, typename OutputTimeStructType>
			OutputTimeStructType Convert_Time(call_requirements(requires(typename OutputTimeStructType::ValueType, is_arithmetic) && requires(OutputTimeStructType,Time_API::Concepts::Time_In_Minutes)))
			{
				OutputTimeStructType t;
				t.Time = this->Total_Seconds<ThisType, ThisType, OutputTimeStructType::Value_Type>() / (OutputTimeStructType::Value_Type)60.0;
				return t ;
			}
			/// Convert base time to Seconds.  Requires that Time_Base has a 'Total_Seconds' accessor to convert base time to seconds
			template <typename ThisType, typename CallerType, typename OutputTimeStructType>
			OutputTimeStructType Convert_Time(call_requirements(requires(typename OutputTimeStructType::ValueType, is_arithmetic) && requires(OutputTimeStructType,Time_API::Concepts::Time_In_Seconds)))
			{
				OutputTimeStructType t;
				t.Time = this->Total_Seconds<ThisType, ThisType, OutputTimeStructType::Value_Type>();
				return t ;
			}
			/// Convert base time to DRSeconds.  Requires that Time_Base has a 'Total_Seconds' accessor to convert base time to seconds
			template <typename ThisType, typename CallerType, typename OutputTimeStructType>
			OutputTimeStructType Convert_Time(call_requirements(requires(typename OutputTimeStructType::ValueType, is_arithmetic) && requires(OutputTimeStructType,Time_API::Concepts::Time_In_DRSeconds)))
			{
				OutputTimeStructType t;
				t.Time = this->Total_Seconds<ThisType, ThisType, OutputTimeStructType::ValueType>() * (OutputTimeStructType::ValueType)10.0;
				return t ;
			}
			//===========================================================================
			/// Time conversion error handler.
			/// Either the requested OutputTimeStructType was not valid, or the requested TargetType was non-arithmetic
			template <typename ThisType, typename CallerType, typename OutputTimeStructType>
			OutputTimeStructType Convert_Time(call_requirements(requires(typename OutputTimeStructType::ValueType, !is_arithmetic) || (
				requires(OutputTimeStructType,!Time_API::Concepts::Time_In_Days) &&
				requires(OutputTimeStructType,!Time_API::Concepts::Time_In_Hours) &&
				requires(OutputTimeStructType,!Time_API::Concepts::Time_In_Minutes) &&
				requires(OutputTimeStructType,!Time_API::Concepts::Time_In_Seconds) &&
				requires(OutputTimeStructType,!Time_API::Concepts::Time_In_DRSeconds)
				)))
			{
				TargetType t;
				t = (TargetType)this->Total_Seconds<ThisType, ThisType, TargetType>();
				return  t * (TargetType)10.0;
			}

			//================================================================================================
			// Time value accessors for:
			/// Days accessor - returns the Days portion of the time
			facet_accessor(Days);
			/// Returns the hours portion of the time i.e. if time was 14:23:10, would return (TargetType)14
			facet_accessor(Hours);
			/// Returns the hours portion of the time i.e. if time was 14:23:10, would return (TargetType)23
			facet_accessor(Minutes);
			/// Returns the hours portion of the time i.e. if time was 14:23:10, would return (TargetType)10
			facet_accessor(Seconds);
			/// Returns the hours portion of the time i.e. if time was 14:23:10.5, would return (TargetType)5
			facet_accessor(DRseconds);
			/// Returns the absolute time converted to seconds i.e. if time was 'Day 2, 14:23:10.5', would return (TargetType)(2*86400 + 14*3600 + 23*60 + 10.5)
			facet_accessor(Total_Seconds);
		};
	}		
	//=============================================================
	/// Time-related Data structures.
	/// Various Time-related data structures used for initializing, adding, etc. a basic Time component.
	/// The data structures include semantic information regarding the data structure as well as a 'Time' member
	/// The idea here is that POLARIS components operate on value types which are put into temporary structs and 
	/// tagged with additional semantic information that can be concept-checked and handled appropriately, rather
	/// than using only basic value types.
	namespace Data_Structures
	{
		/// Time in DRSeconds
		struct Time_DRSeconds
		{
			typedef Types::Time_Types::DRSeconds DRSeconds;
			typedef float ValueType;
			ValueType Time;
		};
		/// Time in Seconds
		struct Time_Seconds
		{
			typedef Types::Time_Types::Seconds Seconds;
			typedef float ValueType;
			ValueType Time;
		};
		/// Time in Minutes
		struct Time_Minutes
		{
			typedef Types::Time_Types::Minutes Minutes;
			typedef float ValueType;
			ValueType Time;
		};
		/// Time in Hours
		struct Time_Hours
		{
			typedef Types::Time_Types::Hours Hours;
			typedef float ValueType;
			ValueType Time;
		};
		/// Time in Days
		struct Time_Days
		{
			typedef Types::Time_Types::Days Days;
			typedef float ValueType;
			ValueType Time;
		};

	}
}
