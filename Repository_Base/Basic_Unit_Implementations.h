#pragma once
#include "Basic_Units_Prototypes.h"

namespace Basic_Units
{
	namespace Implementations
	{
		implementation struct Measure_1D_Implementation
		{
			// LENGTH - stored in feet
			typedef true_type Length_Type;
			member_data_basic(float,length);
		};
		implementation struct Inches_Implementation : Measure_1D_Implementation<MasterType>
		{
			typedef true_type Inches_Type;
		};
		implementation struct Feet_Implementation : Measure_1D_Implementation<MasterType>
		{
			typedef true_type Feet_Type;
		};
		implementation struct Miles_Implementation  : Measure_1D_Implementation<MasterType>
		{
			typedef true_type Miles_Type;
		};
		implementation struct Centimeters_Implementation  : Measure_1D_Implementation<MasterType>
		{
			typedef true_type Centimeters_Type;
		};
		implementation struct Meters_Implementation : Measure_1D_Implementation<MasterType>
		{
			typedef true_type Meters_Type;
		};
		implementation struct Kilometers_Implementation : Measure_1D_Implementation<MasterType>
		{
			typedef true_type Kilometers_Type;
		};

		//================================================================================================
		/// Basic time_component base clase.
		/// The basic time component base clase has a float _local_time member which stores the total seconds
		/// represented by the time_class.  The initializers, addition operation, and accessors all use this 
		/// local member.  The functions below handle requests from the Time_Interface
		implementation struct Time_Implementation
		{
			typedef true_type TimeType;

			//================================================================================================
			// Initializers for different time types:
			//------------------------------------------------------------------------------------------------
			/// Initialize Time using a DRSeconds struct
			feature_implementation void Initialize(TargetType t, call_requirements(requires(TargetType,Concepts::Time_In_DRSeconds)))
			{
				this->_local_time=((float)t/ (float)10.0);
			}
			/// Initialize Time using a Seconds struct
			feature_implementation void Initialize(TargetType t, call_requirements(requires(TargetType,Concepts::Time_In_Seconds)))
			{
				this->_local_time=((float)t);
			}
			/// Initialize Time using a Minutes struct
			feature_implementation void Initialize(TargetType t, call_requirements(requires(TargetType,Concepts::Time_In_Minutes)))
			{
				this->_local_time=((float)t * (float)60.0);
			}
			/// Initialize Time using an Hours struct
			feature_implementation void Initialize(TargetType t, call_requirements(requires(TargetType,Concepts::Time_In_Hours)))
			{
				this->_local_time=((float)t * (float)3600.0);
			}
			/// Initialize Time using a Days struct
			feature_implementation void Initialize(TargetType t, call_requirements(requires(TargetType,Concepts::Time_In_Days)))
			{
				this->_local_time=((float)t * (float)86400.0);
			}
			/// Initialization Error Handler - initializing without a valid time_struct, i.e. either has not 'Time' value member or is not tagged with an appropriate time_type
			feature_implementation void Initialize(TargetType t, call_requirements(requires(TargetType,!Concepts::Time_In_DRSeconds) && requires(TargetType,!Concepts::Time_In_Seconds) && 
				requires(TargetType,!Concepts::Time_In_Minutes) && requires(TargetType,!Concepts::Time_In_Hours) && requires(TargetType,!Concepts::Time_In_Days)))
			{
				assert_requirements(ComponentType, Is_Dispatched, "ComponentType is not dispatched.");
				assert_requirements(TargetType, Concepts::Time_In_DRSeconds,"Your TargetType does not contain a valid Time_Type tag (i.e Days, Hours, Minutes, Seconds, DRSeconds).");
			}


			//================================================================================================
			// Functions for adding time for different time types
			//------------------------------------------------------------------------------------------------
			/// Add a DRSeconds time value to the current time
			feature_implementation void Add_Time(TargetType t, call_requirements(requires(ComponentType, Is_Dispatched) && requires(TargetType,Concepts::Time_In_DRSeconds)))
			{
				this->_local_time = this->_local_time + (float)t/ (float)10.0;
			}
			/// Add a Seconds time value to the current time
			feature_implementation void Add_Time(TargetType& t, call_requirements(requires(ComponentType, Is_Dispatched) && requires(TargetType,Concepts::Time_In_Seconds)))
			{
				this->_local_time = this->_local_time + (float)t;
			}
			/// Add a Minutes time value to the current time
			feature_implementation void Add_Time(TargetType& t, call_requirements(requires(ComponentType, Is_Dispatched) && requires(TargetType,Concepts::Time_In_Minutes)))
			{
				this->local_time=this->_local_time + (float)t * (float)60.0;
			}
			/// Add a Hours time value to the current time
			feature_implementation void Add_Time(TargetType& t, call_requirements(requires(ComponentType, Is_Dispatched) && requires(TargetType,Concepts::Time_In_Hours)))
			{
				this->_local_time=this->_local_time + (float)t.Time * (float)3600.0;
			}
			/// Add a Days time value to the current time
			feature_implementation void Add_Time(TargetType& t, call_requirements(requires(ComponentType, Is_Dispatched) && requires(TargetType,Concepts::Time_In_Days)))
			{
				this->local_time=this->_local_time + (float)t  / (float)86400.0;
			}
			/// Adding time Error Handler
			feature_implementation void Add_Time(TargetType& t, call_requirements(
				requires(ComponentType, !Is_Dispatched) || (requires(TargetType,!Concepts::Time_In_DRSeconds) &&	requires(TargetType,!Concepts::Time_In_Seconds) && 
				requires(TargetType,!Concepts::Time_In_Minutes) && requires(TargetType,!Concepts::Time_In_Hours) && requires(TargetType,!Concepts::Time_In_Days))))
			{
				assert_requirements(ComponentType, Is_Dispatched, "ComponentType is not dispatched.");
				assert_requirements(TargetType, Concepts::Time_In_DRSeconds,"Your TargetType does not contain a valid Time_Type tag (i.e Days, Hours, Minutes, Seconds, DRSeconds).");
			}


			//================================================================================================
			/// Accessor Functions
			//------------------------------------------------------------------------------------------------
			// returns the requested component of the current time, i.e. 12:30:45 => 12 hours, 30 minutes, 45 seconds, etc.
			tag_getter(Time_Component);
			feature_implementation TargetType Time_Component(call_requirements(requires(TargetType,Concepts::Time_In_Days)))
			{
				return (TargetType)((int)(_local_time/86400.0));
			}
			feature_implementation TargetType Time_Component(call_requirements(requires(TargetType,Concepts::Time_In_DRSeconds)))
			{
				return (TargetType)((int)(_local_time*10)%10);
			}
			feature_implementation TargetType Time_Component(call_requirements(requires(TargetType,Concepts::Time_In_Hours)))
			{
				return (TargetType)((int)(_local_time/3600.0) % 24);
			}
			feature_implementation TargetType Time_Component(call_requirements(requires(TargetType,Concepts::Time_In_Minutes)))
			{
				return (TargetType)((int)(_local_time/60.0)%60);
			}
			feature_implementation TargetType Time_Component(call_requirements(requires(TargetType,Concepts::Time_In_Seconds)))
			{
				return (TargetType) ((int)(_local_time)%60);
			}
			feature_implementation void Time_Component(call_requirements(requires(TargetType,!Concepts::Time_In_DRSeconds) &&	requires(TargetType,!Concepts::Time_In_Seconds) && 
				requires(TargetType,!Concepts::Time_In_Minutes) && requires(TargetType,!Concepts::Time_In_Hours) && requires(TargetType,!Concepts::Time_In_Days)))
			{
				assert_requirements(ComponentType, Is_Dispatched, "ComponentType is not dispatched.");
				assert_requirements(TargetType, Concepts::Time_In_DRSeconds,"Your TargetType does not contain a valid Time_Type tag (i.e Days, Hours, Minutes, Seconds, DRSeconds).");
			}

			// returns the current time in the requested time unit
			tag_getter(Total_Time);
			feature_implementation TargetType Total_Time(call_requirements(requires(ComponentType, Is_Dispatched) && requires(TargetType,Concepts::Time_In_Days)))
			{
				return (TargetType)_local_time/86400.0f;
			}
			feature_implementation TargetType Total_Time(call_requirements(requires(ComponentType, Is_Dispatched) && requires(TargetType,Concepts::Time_In_DRSeconds)))
			{
				return (TargetType)_local_time*10.0f;
			}
			feature_implementation TargetType Total_Time(call_requirements(requires(ComponentType, Is_Dispatched) && requires(TargetType,Concepts::Time_In_Hours)))
			{
				return (TargetType)_local_time/3600.0f;
			}
			feature_implementation TargetType Total_Time(call_requirements(requires(ComponentType, Is_Dispatched) && requires(TargetType,Concepts::Time_In_Minutes)))
			{
				return (TargetType)_local_time/60.0f;
			}
			feature_implementation TargetType Total_Time(call_requirements(requires(ComponentType, Is_Dispatched) && requires(TargetType,Concepts::Time_In_Seconds)))
			{
				return (TargetType)_local_time;
			}
			feature_implementation void Total_Time(call_requirements(requires(TargetType,!Concepts::Time_In_DRSeconds) && requires(TargetType,!Concepts::Time_In_Seconds) && 
				requires(TargetType,!Concepts::Time_In_Minutes) && requires(TargetType,!Concepts::Time_In_Hours) && requires(TargetType,!Concepts::Time_In_Days)))
			{
				assert_requirements(ComponentType, Is_Dispatched, "ComponentType is not dispatched.");
				assert_requirements(TargetType, Concepts::Time_In_DRSeconds,"Your TargetType does not contain a valid Time_Type tag (i.e Days, Hours, Minutes, Seconds, DRSeconds).");
			}


			//================================================================================================
			/// Storage for local_time value
			//------------------------------------------------------------------------------------------------
			member_data(float, local_time, requires(TargetType,is_arithmetic),requires(TargetType, is_arithmetic));		
		};
	}

	namespace Components
	{
		implementation struct Inches
		{
			typedef Polaris_Component<Implementations::Inches_Implementation, MasterType> type;
		};
		implementation struct Feet
		{
			typedef Polaris_Component<Implementations::Feet_Implementation,MasterType> type;
		};
		implementation struct Miles
		{
			typedef Polaris_Component<Implementations::Miles_Implementation,MasterType> type;
		};
		implementation struct Centimeters
		{
			typedef Polaris_Component<Implementations::Centimeters_Implementation,MasterType> type;
		};
		implementation struct Meters
		{
			typedef Polaris_Component<Implementations::Meters_Implementation,MasterType> type;
		};
		implementation struct Kilometers
		{
			typedef Polaris_Component<Implementations::Kilometers_Implementation,MasterType> type;
		};

		implementation struct Time
		{
			typedef Polaris_Component<Implementations::Time_Implementation,MasterType> type;
		};
	}
}