#pragma once

#include "Time_Interfaces.h"



namespace Time_Components
{
	namespace Bases
	{
		//================================================================================================
		/// Basic time_component base clase.
		/// The basic time component base clase has a float _local_time member which stores the total seconds
		/// represented by the time_class.  The initializers, addition operation, and accessors all use this 
		/// local member.  The functions below handle requests from the Time_Interface
		struct Time_Base
		{
			typedef true_type TimeType;

			//================================================================================================
			// Initializers for different time types:
			//------------------------------------------------------------------------------------------------
			/// Initialize Time using a DRSeconds struct
			facet_base void Initialize(TargetType t, call_requirements(requires(ThisType, Is_Dispatched) && requires(TargetType,Concepts::Time_In_DRSeconds)))
			{
				this->_local_time=((float)t/ (float)10.0);
			}
			/// Initialize Time using a Seconds struct
			facet_base void Initialize(TargetType t, call_requirements(requires(ThisType, Is_Dispatched) && requires(TargetType,Concepts::Time_In_Seconds)))
			{
				this->_local_time=((float)t);
			}
			/// Initialize Time using a Minutes struct
			facet_base void Initialize(TargetType t, call_requirements(requires(ThisType, Is_Dispatched) && requires(TargetType,Concepts::Time_In_Minutes)))
			{
				this->_local_time=((float)t * (float)60.0);
			}
			/// Initialize Time using an Hours struct
			facet_base void Initialize(TargetType t, call_requirements(requires(ThisType, Is_Dispatched) && requires(TargetType,Concepts::Time_In_Hours)))
			{
				this->_local_time=((float)t * (float)3600.0);
			}
			/// Initialize Time using a Days struct
			facet_base void Initialize(TargetType t, call_requirements(requires(ThisType, Is_Dispatched) && requires(TargetType,Concepts::Time_In_Days)))
			{
				this->_local_time=((float)t * (float)86400.0);
			}
			/// Initialization Error Handler - initializing without a valid time_struct, i.e. either has not 'Time' value member or is not tagged with an appropriate time_type
			facet_base void Initialize(TargetType t, call_requirements(
				requires(ThisType, !Is_Dispatched) || (requires(TargetType,!Concepts::Time_In_DRSeconds) && requires(TargetType,!Concepts::Time_In_Seconds) && 
				requires(TargetType,!Concepts::Time_In_Minutes) && requires(TargetType,!Concepts::Time_In_Hours) && requires(TargetType,!Concepts::Time_In_Days))))
			{
				assert_requirements(ThisType, Is_Dispatched, "ThisType is not dispatched.");
				assert_requirements(TargetType, Concepts::Time_In_DRSeconds,"Your TargetType does not contain a valid Time_Type tag (i.e Days, Hours, Minutes, Seconds, DRSeconds).");
			}


			//================================================================================================
			// Functions for adding time for different time types
			//------------------------------------------------------------------------------------------------
			/// Add a DRSeconds time value to the current time
			facet_base void Add_Time(TargetType t, call_requirements(requires(ThisType, Is_Dispatched) && requires(TargetType,Concepts::Time_In_DRSeconds)))
			{
				this->_local_time += (float)t.Time / (float)10.0;
			}
			/// Add a Seconds time value to the current time
			facet_base void Add_Time(TargetType& t, call_requirements(requires(ThisType, Is_Dispatched) && requires(TargetType,Concepts::Time_In_Seconds)))
			{
				this->_local_time+=(float)t.Time;
			}
			/// Add a Minutes time value to the current time
			facet_base void Add_Time(TargetType& t, call_requirements(requires(ThisType, Is_Dispatched) && requires(TargetType,Concepts::Time_In_Minutes)))
			{
				this->local_time+=(float)t.Time * (float)60.0;
			}
			/// Add a Hours time value to the current time
			facet_base void Add_Time(TargetType& t, call_requirements(requires(ThisType, Is_Dispatched) && requires(TargetType,Concepts::Time_In_Hours)))
			{
				this->_local_time+=(float)t.Time * (float)3600.0;
			}
			/// Add a Days time value to the current time
			facet_base void Add_Time(TargetType& t, call_requirements(requires(ThisType, Is_Dispatched) && requires(TargetType,Concepts::Time_In_Days)))
			{
				this->_local_time+=(float)t.Time / (float)86400.0;
			}
			/// Adding time Error Handler
			facet_base void Add_Time(TargetType& t, call_requirements(
				requires(ThisType, !Is_Dispatched) || (requires(TargetType,!Concepts::Time_In_DRSeconds) &&	requires(TargetType,!Concepts::Time_In_Seconds) && 
				requires(TargetType,!Concepts::Time_In_Minutes) && requires(TargetType,!Concepts::Time_In_Hours) && requires(TargetType,!Concepts::Time_In_Days))))
			{
				assert_requirements(ThisType, Is_Dispatched, "ThisType is not dispatched.");
				assert_requirements(TargetType, Concepts::Time_In_DRSeconds,"Your TargetType does not contain a valid Time_Type tag (i.e Days, Hours, Minutes, Seconds, DRSeconds).");
			}


			//================================================================================================
			/// Accessor Functions
			//------------------------------------------------------------------------------------------------
			// returns the requested component of the current time, i.e. 12:30:45 => 12 hours, 30 minutes, 45 seconds, etc.
			tag_getter(Time_Component);
			facet_base TargetType Time_Component(call_requirements(requires(ThisType, Is_Dispatched) && requires(TargetType,Concepts::Time_In_Days)))
			{
				return (TargetType)((int)(_local_time/86400.0));
			}
			facet_base TargetType Time_Component(call_requirements(requires(ThisType, Is_Dispatched) && requires(TargetType,Concepts::Time_In_DRSeconds)))
			{
				return (TargetType)((int)(_local_time*10)%10);
			}
			facet_base TargetType Time_Component(call_requirements(requires(ThisType, Is_Dispatched) && requires(TargetType,Concepts::Time_In_Hours)))
			{
				return (TargetType)((int)(_local_time/3600.0) % 24);
			}
			facet_base TargetType Time_Component(call_requirements(requires(ThisType, Is_Dispatched) && requires(TargetType,Concepts::Time_In_Minutes)))
			{
				return (TargetType)((int)(_local_time/60.0)%60);
			}
			facet_base TargetType Time_Component(call_requirements(requires(ThisType, Is_Dispatched) && requires(TargetType,Concepts::Time_In_Seconds)))
			{
				return (TargetType) ((int)(_local_time)%60);
			}
			facet_base void Time_Component(call_requirements(
				requires(ThisType, !Is_Dispatched) || (requires(TargetType,!Concepts::Time_In_DRSeconds) &&	requires(TargetType,!Concepts::Time_In_Seconds) && 
				requires(TargetType,!Concepts::Time_In_Minutes) && requires(TargetType,!Concepts::Time_In_Hours) && requires(TargetType,!Concepts::Time_In_Days))))
			{
				assert_requirements(ThisType, Is_Dispatched, "ThisType is not dispatched.");
				assert_requirements(TargetType, Concepts::Time_In_DRSeconds,"Your TargetType does not contain a valid Time_Type tag (i.e Days, Hours, Minutes, Seconds, DRSeconds).");
			}

			// returns the current time in the requested time unit
			tag_getter(Total_Time);
			facet_base TargetType Total_Time(call_requirements(requires(ThisType, Is_Dispatched) && requires(TargetType,Concepts::Time_In_Days)))
			{
				return (TargetType)_local_time/86400.0f;
			}
			facet_base TargetType Total_Time(call_requirements(requires(ThisType, Is_Dispatched) && requires(TargetType,Concepts::Time_In_DRSeconds)))
			{
				return (TargetType)_local_time*10.0f;
			}
			facet_base TargetType Total_Time(call_requirements(requires(ThisType, Is_Dispatched) && requires(TargetType,Concepts::Time_In_Hours)))
			{
				return (TargetType)_local_time/3600.0f;
			}
			facet_base TargetType Total_Time(call_requirements(requires(ThisType, Is_Dispatched) && requires(TargetType,Concepts::Time_In_Minutes)))
			{
				return (TargetType)_local_time/60.0f;
			}
			facet_base TargetType Total_Time(call_requirements(requires(ThisType, Is_Dispatched) && requires(TargetType,Concepts::Time_In_Seconds)))
			{
				return (TargetType)_local_time;
			}
			facet_base void Total_Time(call_requirements(
				requires(ThisType, !Is_Dispatched) || (requires(TargetType,!Concepts::Time_In_DRSeconds) &&	requires(TargetType,!Concepts::Time_In_Seconds) && 
				requires(TargetType,!Concepts::Time_In_Minutes) && requires(TargetType,!Concepts::Time_In_Hours) && requires(TargetType,!Concepts::Time_In_Days))))
			{
				assert_requirements(ThisType, Is_Dispatched, "ThisType is not dispatched.");
				assert_requirements(TargetType, Concepts::Time_In_DRSeconds,"Your TargetType does not contain a valid Time_Type tag (i.e Days, Hours, Minutes, Seconds, DRSeconds).");
			}


			//================================================================================================
			/// Storage for local_time value
			//------------------------------------------------------------------------------------------------
			member_data(float, local_time, requires(TargetType,is_arithmetic),requires_2(ThisType,CallerType,Is_Same_Component) && requires(TargetType, is_arithmetic));		
		};
	}

	namespace Components
	{
		///================================================================================================
		/// Create 'Time' Polaris component
		typedef Polaris_Component<Interfaces::Time_Interface,Bases::Time_Base,NULLTYPE,NULLTYPE,Data_Object> Time;
	}

}