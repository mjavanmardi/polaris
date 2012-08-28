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
		class Time_Base
		{
		public: 
					
			//================================================================================================
			// Initializers for different time types:
			/// Initialize Time using a DRSeconds struct
			facet void Initialize(TargetType& t, call_requirements(requires(ThisType, Is_Dispatched) && requires(TargetType, Concepts::Has_Time) && requires(TargetType,Concepts::Time_In_DRSeconds)))
			{
				this->_local_time=((float)t.Time/ (float)10.0);
			}
			/// Initialize Time using a Seconds struct
			facet void Initialize(TargetType& t, call_requirements(requires(ThisType, Is_Dispatched) && requires(TargetType, Concepts::Has_Time) && requires(TargetType,Concepts::Time_In_Seconds)))
			{
				this->_local_time=((float)t.Time);
			}
			/// Initialize Time using a Minutes struct
			facet void Initialize(TargetType& t, call_requirements(requires(ThisType, Is_Dispatched) && requires(TargetType, Concepts::Has_Time) && requires(TargetType,Concepts::Time_In_Minutes)))
			{
				this->_local_time=((float)t.Time * (float)60.0);
			}
			/// Initialize Time using an Hours struct
			facet void Initialize(TargetType& t, call_requirements(requires(ThisType, Is_Dispatched) && requires(TargetType, Concepts::Has_Time) && requires(TargetType,Concepts::Time_In_Hours)))
			{
				this->_local_time=((float)t.Time * (float)3600.0);
			}
			/// Initialize Time using a Days struct
			facet void Initialize(TargetType& t, call_requirements(requires(ThisType, Is_Dispatched) && requires(TargetType, Concepts::Has_Time) && requires(TargetType,Concepts::Time_In_Days)))
			{
				this->_local_time=((float)t.Time * (float)86400.0);
			}
			/// Initialization Error Handler - initializing without a valid time_struct, i.e. either has not 'Time' value member or is not tagged with an appropriate time_type
			facet void Initialize(TargetType& t, call_requirements(
				requires(ThisType, !Is_Dispatched) || 
				requires(TargetType,!Concepts::Has_Time) || (
				requires(TargetType,!Concepts::Time_In_DRSeconds) &&
				requires(TargetType,!Concepts::Time_In_Seconds) && 
				requires(TargetType,!Concepts::Time_In_Minutes) && 
				requires(TargetType,!Concepts::Time_In_Hours) && 
				requires(TargetType,!Concepts::Time_In_Days))))
			{
				assert_requirements(ThisType, Is_Dispatched, "ThisType is not dispatched.");
				assert_requirements(TargetType, Concepts::Has_Time, "ERROR: your time structure does not have 'Time' member.");
				assert_requirements(TargetType, Concepts::Time_In_DRSeconds,"Your TargetType does not contain a valid Time_Type tag (i.e Days, Hours, Minutes, Seconds, DRSeconds).");
			}

			//================================================================================================
			// Adding time for different time types
			/// Add a DRSeconds time value to the current time
			facet void Add_Time(TargetType& t, call_requirements(requires(ThisType, Is_Dispatched) && requires(TargetType, Concepts::Has_Time) && requires(TargetType,Concepts::Time_In_DRSeconds)))
			{
				this->_local_time += (float)t.Time / (float)10.0;
			}
			/// Add a Seconds time value to the current time
			facet void Add_Time(TargetType& t, call_requirements(requires(ThisType, Is_Dispatched) && requires(TargetType, Concepts::Has_Time) && requires(TargetType,Concepts::Time_In_Seconds)))
			{
				this->_local_time+=(float)t.Time;
			}
			/// Add a Minutes time value to the current time
			facet void Add_Time(TargetType& t, call_requirements(requires(ThisType, Is_Dispatched) && requires(TargetType, Concepts::Has_Time) && requires(TargetType,Concepts::Time_In_Minutes)))
			{
				this->local_time+=(float)t.Time * (float)60.0;
			}
			/// Add a Hours time value to the current time
			facet void Add_Time(TargetType& t, call_requirements(requires(ThisType, Is_Dispatched) && requires(TargetType, Concepts::Has_Time) && requires(TargetType,Concepts::Time_In_Hours)))
			{
				this->_local_time+=(float)t.Time * (float)3600.0;
			}
			/// Add a Days time value to the current time
			facet void Add_Time(TargetType& t, call_requirements(requires(ThisType, Is_Dispatched) && requires(TargetType, Concepts::Has_Time) && requires(TargetType,Concepts::Time_In_Days)))
			{
				this->_local_time+=(float)t.Time / (float)86400.0;
			}
			/// Adding time Error Handler
			facet void Add_Time(TargetType& t, call_requirements(
				requires(ThisType, !Is_Dispatched) || 
				requires(TargetType,!Concepts::Has_Time) || (
				requires(TargetType,!Concepts::Time_In_DRSeconds) &&
				requires(TargetType,!Concepts::Time_In_Seconds) && 
				requires(TargetType,!Concepts::Time_In_Minutes) && 
				requires(TargetType,!Concepts::Time_In_Hours) && 
				requires(TargetType,!Concepts::Time_In_Days))))
			{
				assert_requirements(ThisType, Is_Dispatched, "ThisType is not dispatched.");
				assert_requirements(TargetType, Concepts::Has_Time, "ERROR: your time structure does not have 'Time' member.");
				assert_requirements(TargetType, Concepts::Time_In_DRSeconds,"Your TargetType does not contain a valid Time_Type tag (i.e Days, Hours, Minutes, Seconds, DRSeconds).");
			}

			//================================================================================================
			/// Accessor Functions
			tag_getter(Days);
			template <typename ThisType, typename CallerType, typename TargetType>
			TargetType Days(call_requirements(requires(ThisType,Is_Dispatched) && requires(TargetType,is_arithmetic)))
			{
				return (TargetType)((int)(_local_time/86400.0));
			}
			template <typename ThisType, typename CallerType, typename TargetType>
			TargetType Days(call_requirements(requires(ThisType,Is_Dispatched) && requires(TargetType,!is_arithmetic)))
			{
				assert_requirements_std(TargetType,is_arithmetic,"Days Accessor TargetType is not arithmetic!");
			}

			tag_getter(Hours);
			template <typename ThisType, typename CallerType, typename TargetType>
			TargetType Hours(call_requirements(requires(ThisType,Is_Dispatched) && requires(TargetType,is_arithmetic)))
			{
				return (TargetType)((int)(_local_time/3600.0) % 24);
			}
			template <typename ThisType, typename CallerType, typename TargetType>
			TargetType Hours(call_requirements(requires(ThisType,Is_Dispatched) && requires(TargetType,!is_arithmetic)))
			{
				assert_requirements_std(TargetType,is_arithmetic,"Hours Accessor TargetType is not arithmetic!");
			}

			tag_getter(Minutes);
			template <typename ThisType, typename CallerType, typename TargetType>
			TargetType Minutes(call_requirements(requires(ThisType,Is_Dispatched) && requires(TargetType,is_arithmetic)))
			{
				return (TargetType)((int)(_local_time/60.0)%60);
			}
			template <typename ThisType, typename CallerType, typename TargetType>
			TargetType Minutes(call_requirements(requires(ThisType,Is_Dispatched) && requires(TargetType,!is_arithmetic)))
			{
				assert_requirements_std(TargetType,is_arithmetic,"Minutes Accessor TargetType is not arithmetic!");
			}

			tag_getter(Seconds);
			template <typename ThisType, typename CallerType, typename TargetType>
			TargetType Seconds(call_requirements(requires(ThisType,Is_Dispatched) && requires(TargetType,is_arithmetic)))
			{
				return (TargetType) ((int)(_local_time)%60);
			}
			template <typename ThisType, typename CallerType, typename TargetType>
			TargetType Seconds(call_requirements(requires(ThisType,Is_Dispatched) && requires(TargetType,!is_arithmetic)))
			{
				assert_requirements_std(TargetType,is_arithmetic,"Seconds Accessor TargetType is not arithmetic!");
			}

			tag_getter(DRseconds);
			template <typename ThisType, typename CallerType, typename TargetType>
			TargetType DRseconds(call_requirements(requires(ThisType,Is_Dispatched) && requires(TargetType,is_arithmetic)))
			{
				return (TargetType)((int)(_local_time*10)%10);
			}
			template <typename ThisType, typename CallerType, typename TargetType>
			TargetType DRseconds(call_requirements(requires(ThisType,Is_Dispatched) && requires(TargetType,!is_arithmetic)))
			{
				assert_requirements_std(TargetType,is_arithmetic,"DRseconds Accessor TargetType is not arithmetic!");
			}

			tag_getter(Total_Seconds);
			facet TargetType Total_Seconds(call_requirements(requires(ThisType, Is_Dispatched) && requires(TargetType,is_arithmetic)))
			{
				return (TargetType)_local_time;
			}
			facet TargetType Total_Seconds(call_requirements(requires(ThisType, !Is_Dispatched) || requires(TargetType,!is_arithmetic)))
			{
				assert_requirements_std(ThisType,Is_Dispatched,"ThisType has not been dispatched!");
				assert_requirements_std(TargetType,is_arithmetic,"DRseconds Accessor TargetType is not arithmetic!");
			}

		protected:
			//================================================================================================
			/// Storage for local_time value
			float _local_time;
			tag_getter(local_time);
			tag_setter(local_time);
			facet TargetType local_time(call_requirements(requires(ThisType, Is_Dispatched) && requires(TargetType,is_arithmetic)))
			{
				return (TargetType)_local_time;
			}
			facet TargetType local_time(call_requirements(requires(ThisType, !Is_Dispatched) || requires(TargetType,!is_arithmetic)))
			{
				assert_requirements(TargetType,is_arithmetic,"TargetType is not arithmetic.");
				assert_requirements(ThisType,Is_Dispatched,"ThisType has not been dispatched by an appropriate interface.");
			}
			facet void local_time(TargetType set_value, call_requirements(requires_2(ThisType,CallerType,Is_Same_Component) && requires(ThisType, Is_Dispatched) && requires(TargetType, is_arithmetic)))
			{
				_local_time = (float)set_value;
			}
			facet void local_time(TargetType set_value, call_requirements(requires_2(ThisType,CallerType,Is_Same_Component) && requires(ThisType, !Is_Dispatched) || requires(TargetType, !is_arithmetic)))
			{
				assert_requirements(TargetType,is_arithmetic,"TargetType is not arithmetic.");
				assert_requirements(ThisType,Is_Dispatched,"ThisType has not been dispatched by an appropriate interface.");
			}
		
		};
	}

	namespace Components
	{
		///================================================================================================
		/// Create 'Time' Polaris component
		typedef Polaris_Component<Interfaces::Time_Interface,Bases::Time_Base,NULLTYPE,NULLTYPE,Data_Object> Time;
	}

}