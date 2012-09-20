#pragma once

#include "Repository_Base_Includes.h"

//=============================================================
/// POLARIS TIME CLASS INTERFACE
///
namespace Time_Components
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
			requires_typename_state(none,TimeType,true_type,"Type is not a Time data structure");
			//requires_concept_checked_member(TimeType, value, is_arithmetic,"An arithmetic 'Time' data member has not been defined.");
			end_requirements_list(TimeType);
		};
		/// Check if initializing struct is tagged as DRSeconds (i.e. 1/10th of a second)
		concept Time_In_DRSeconds
		{
			begin_requirements_list;
			requires_concept(none,Has_Time);
			requires_typename_state(Has_Time,DRSeconds,true_type,"Type is not in DRSeconds");
			end_requirements_list(DRSeconds);
		};
		/// Check if initializing struct is tagged as Seconds
		concept Time_In_Seconds
		{
			begin_requirements_list;
			requires_concept(none,Has_Time);
			requires_typename_state(Has_Time,Seconds,true_type,"Type is not in Seconds");
			end_requirements_list(Seconds);
		};
		/// Check if initializing struct is tagged as Minutes
		concept Time_In_Minutes
		{
			begin_requirements_list;
			requires_concept(none,Has_Time);
			requires_typename_state(Has_Time,Minutes,true_type,"Type is not in Minutes");
			end_requirements_list(Minutes);
		};
		/// Check if initializing struct is tagged as Hours
		concept Time_In_Hours
		{
			begin_requirements_list;
			requires_concept(none,Has_Time);
			requires_typename_state(Has_Time,Hours,true_type,"Type is not in Hours");
			end_requirements_list(Hours);
		};
		/// Check if initializing struct is tagged as Days
		concept Time_In_Days
		{
			begin_requirements_list;
			requires_concept(none,Has_Time);
			requires_typename_state(Has_Time,Days,true_type,"Type is not in Hours");
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
		template<typename ThisType, typename CallerType>
		struct Time_Interface
		{
			/// Communicates that this time interface is a time type
			typedef ThisType This_Type;
			typedef true_type TimeType;

			/// Initializer
			facet void Initialize(TargetType t, call_requires(ThisType,Is_Dispatchable))
			{
				return PTHIS(ThisType)->Initialize<Dispatch<ThisType>,CallerType,TargetType>(t);
			}

			/// Adding time
			facet void Add_Time(TargetType t, call_requires(ThisType,Is_Dispatchable))
			{
				return PTHIS(ThisType)->Add_Time<Dispatch<ThisType>,CallerType,TargetType>(t);
			}

			/// Display the time
			facet void Write()
			{
				//cout <<"Total Seconds: "<<pthis->Total_Seconds<ThisType, CallerType, TargetType>()<<endl;
				cout << "Day "<< this->Time_Component<Data_Structures::Time_Days>()<<":  ";
				cout << this->Time_Component<Data_Structures::Time_Hours>()<<":"<<this->Time_Component<Data_Structures::Time_Minutes>()<<":"<<this->Time_Component<Data_Structures::Time_Seconds>()<<"."<< this->Time_Component<Data_Structures::Time_DRSeconds>();
			}

			//================================================================================================
			// Time value accessors for:
			//------------------------------------------------------------------------------------------------
			/// Time component accessor - returns the requested portion of the current time
			facet_accessor(Time_Component);
			/// Returns the value of the current time converted to the requested time units
			facet_accessor(Total_Time);
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
		Basic_Data_Struct(Time_DRSeconds,float,TimeType,DRSeconds);
		Basic_Data_Struct(Time_Seconds,float,TimeType,Seconds);
		Basic_Data_Struct(Time_Minutes,float,TimeType,Minutes);
		Basic_Data_Struct(Time_Hours,float,TimeType,Hours);
		Basic_Data_Struct(Time_Days,float,TimeType,Days);
	}
}
