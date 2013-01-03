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
			member_data(Value_Type,Value,none,none);
		};
		implementation struct Measure_2D_Implementation : Measure_1D_Implementation<MasterType>
		{
			typedef true_type Area_Type;
		};
		implementation struct Measure_3D_Implementation : Measure_2D_Implementation<MasterType>
		{
			typedef true_type Volume_Type;
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

		implementation struct Square_Inches_Implementation : Inches_Implementation<MasterType> {};
		implementation struct Square_Feet_Implementation : Feet_Implementation<MasterType> {};
		implementation struct Square_Miles_Implementation : Miles_Implementation<MasterType> {};
		implementation struct Square_Centimeters_Implementation : Centimeters_Implementation<MasterType> {};
		implementation struct Square_Meters_Implementation : Meters_Implementation<MasterType> {};
		implementation struct Square_Kilometers_Implementation : Kilometers_Implementation<MasterType> {};



		//================================================================================================
		/// Basic time_component base clase.
		/// The basic time component base clase has a Value_Type _local_time member which stores the total seconds
		/// represented by the time_class.  The initializers, addition operation, and accessors all use this 
		/// local member.  The functions below handle requests from the Time_Interface
		implementation struct Time_Implementation
		{
			typedef true_type Time_Type;
			member_data(Value_Type,Value,none,none);
		};
		implementation struct DRSeconds_Implementation : Time_Implementation<MasterType>
		{
			typedef true_type DRSeconds_Type;
		};
		implementation struct Seconds_Implementation : Time_Implementation<MasterType>
		{
			typedef true_type Seconds_Type;
		};
		implementation struct Minutes_Implementation : Time_Implementation<MasterType>
		{
			typedef true_type Minutes_Type;
		};
		implementation struct Hours_Implementation : Time_Implementation<MasterType>
		{
			typedef true_type Hours_Type;
		};
		implementation struct Days_Implementation : Time_Implementation<MasterType>
		{
			typedef true_type Days_Type;
		};

		//implementation struct Time_Implementation_old
		//{
		//	typedef true_type Time;

		//	//================================================================================================
		//	// Initializers for different time types:
		//	//------------------------------------------------------------------------------------------------
		//	/// Initialize Time using a DRSeconds struct
		//	feature_implementation void Initialize(TargetType t, call_requirements(requires(TargetType,Concepts::Time_In_DRSeconds)))
		//	{
		//		this->_local_time=((Value_Type)t/ (Value_Type)10.0);
		//	}
		//	/// Initialize Time using a Seconds struct
		//	feature_implementation void Initialize(TargetType t, call_requirements(requires(TargetType,Concepts::Time_In_Seconds)))
		//	{
		//		this->_local_time=((Value_Type)t);
		//	}
		//	/// Initialize Time using a Minutes struct
		//	feature_implementation void Initialize(TargetType t, call_requirements(requires(TargetType,Concepts::Time_In_Minutes)))
		//	{
		//		this->_local_time=((Value_Type)t * (Value_Type)60.0);
		//	}
		//	/// Initialize Time using an Hours struct
		//	feature_implementation void Initialize(TargetType t, call_requirements(requires(TargetType,Concepts::Time_In_Hours)))
		//	{
		//		this->_local_time=((Value_Type)t * (Value_Type)3600.0);
		//	}
		//	/// Initialize Time using a Days struct
		//	feature_implementation void Initialize(TargetType t, call_requirements(requires(TargetType,Concepts::Time_In_Days)))
		//	{
		//		this->_local_time=((Value_Type)t * (Value_Type)86400.0);
		//	}
		//	/// Initialization Error Handler - initializing without a valid time_struct, i.e. either has not 'Time' value member or is not tagged with an appropriate time_type
		//	feature_implementation void Initialize(TargetType t, call_requirements(requires(TargetType,!Concepts::Time_In_DRSeconds) && requires(TargetType,!Concepts::Time_In_Seconds) && 
		//		requires(TargetType,!Concepts::Time_In_Minutes) && requires(TargetType,!Concepts::Time_In_Hours) && requires(TargetType,!Concepts::Time_In_Days)))
		//	{
		//		assert_requirements(ComponentType, Is_Dispatched, "ComponentType is not dispatched.");
		//		assert_requirements(TargetType, Concepts::Time_In_DRSeconds,"Your TargetType does not contain a valid Time_Type tag (i.e Days, Hours, Minutes, Seconds, DRSeconds).");
		//	}


		//	//================================================================================================
		//	// Functions for adding time for different time types
		//	//------------------------------------------------------------------------------------------------
		//	/// Add a DRSeconds time value to the current time
		//	feature_implementation void Add_Time(TargetType t, call_requirements(requires(ComponentType, Is_Dispatched) && requires(TargetType,Concepts::Time_In_DRSeconds)))
		//	{
		//		this->_local_time = this->_local_time + (Value_Type)t/ (Value_Type)10.0;
		//	}
		//	/// Add a Seconds time value to the current time
		//	feature_implementation void Add_Time(TargetType& t, call_requirements(requires(ComponentType, Is_Dispatched) && requires(TargetType,Concepts::Time_In_Seconds)))
		//	{
		//		this->_local_time = this->_local_time + (Value_Type)t;
		//	}
		//	/// Add a Minutes time value to the current time
		//	feature_implementation void Add_Time(TargetType& t, call_requirements(requires(ComponentType, Is_Dispatched) && requires(TargetType,Concepts::Time_In_Minutes)))
		//	{
		//		this->local_time=this->_local_time + (Value_Type)t * (Value_Type)60.0;
		//	}
		//	/// Add a Hours time value to the current time
		//	feature_implementation void Add_Time(TargetType& t, call_requirements(requires(ComponentType, Is_Dispatched) && requires(TargetType,Concepts::Time_In_Hours)))
		//	{
		//		this->_local_time=this->_local_time + (Value_Type)t.Time * (Value_Type)3600.0;
		//	}
		//	/// Add a Days time value to the current time
		//	feature_implementation void Add_Time(TargetType& t, call_requirements(requires(ComponentType, Is_Dispatched) && requires(TargetType,Concepts::Time_In_Days)))
		//	{
		//		this->local_time=this->_local_time + (Value_Type)t  / (Value_Type)86400.0;
		//	}
		//	/// Adding time Error Handler
		//	feature_implementation void Add_Time(TargetType& t, call_requirements(
		//		requires(ComponentType, !Is_Dispatched) || (requires(TargetType,!Concepts::Time_In_DRSeconds) &&	requires(TargetType,!Concepts::Time_In_Seconds) && 
		//		requires(TargetType,!Concepts::Time_In_Minutes) && requires(TargetType,!Concepts::Time_In_Hours) && requires(TargetType,!Concepts::Time_In_Days))))
		//	{
		//		assert_requirements(ComponentType, Is_Dispatched, "ComponentType is not dispatched.");
		//		assert_requirements(TargetType, Concepts::Time_In_DRSeconds,"Your TargetType does not contain a valid Time_Type tag (i.e Days, Hours, Minutes, Seconds, DRSeconds).");
		//	}


		//	//================================================================================================
		//	/// Accessor Functions
		//	//------------------------------------------------------------------------------------------------
		//	// returns the requested component of the current time, i.e. 12:30:45 => 12 hours, 30 minutes, 45 seconds, etc.
		//	tag_getter(Time_Component);
		//	feature_implementation TargetType Time_Component(call_requirements(requires(TargetType,Concepts::Time_In_Days)))
		//	{
		//		return (TargetType)((int)(_local_time/86400.0));
		//	}
		//	feature_implementation TargetType Time_Component(call_requirements(requires(TargetType,Concepts::Time_In_DRSeconds)))
		//	{
		//		return (TargetType)((int)(_local_time*10)%10);
		//	}
		//	feature_implementation TargetType Time_Component(call_requirements(requires(TargetType,Concepts::Time_In_Hours)))
		//	{
		//		return (TargetType)((int)(_local_time/3600.0) % 24);
		//	}
		//	feature_implementation TargetType Time_Component(call_requirements(requires(TargetType,Concepts::Time_In_Minutes)))
		//	{
		//		return (TargetType)((int)(_local_time/60.0)%60);
		//	}
		//	feature_implementation TargetType Time_Component(call_requirements(requires(TargetType,Concepts::Time_In_Seconds)))
		//	{
		//		return (TargetType) ((int)(_local_time)%60);
		//	}
		//	feature_implementation void Time_Component(call_requirements(requires(TargetType,!Concepts::Time_In_DRSeconds) &&	requires(TargetType,!Concepts::Time_In_Seconds) && 
		//		requires(TargetType,!Concepts::Time_In_Minutes) && requires(TargetType,!Concepts::Time_In_Hours) && requires(TargetType,!Concepts::Time_In_Days)))
		//	{
		//		assert_requirements(ComponentType, Is_Dispatched, "ComponentType is not dispatched.");
		//		assert_requirements(TargetType, Concepts::Time_In_DRSeconds,"Your TargetType does not contain a valid Time_Type tag (i.e Days, Hours, Minutes, Seconds, DRSeconds).");
		//	}

		//	// returns the current time in the requested time unit
		//	tag_getter(Total_Time);
		//	feature_implementation TargetType Total_Time(call_requirements(requires(ComponentType, Is_Dispatched) && requires(TargetType,Concepts::Time_In_Days)))
		//	{
		//		return (TargetType)_local_time/86400.0f;
		//	}
		//	feature_implementation TargetType Total_Time(call_requirements(requires(ComponentType, Is_Dispatched) && requires(TargetType,Concepts::Time_In_DRSeconds)))
		//	{
		//		return (TargetType)_local_time*10.0f;
		//	}
		//	feature_implementation TargetType Total_Time(call_requirements(requires(ComponentType, Is_Dispatched) && requires(TargetType,Concepts::Time_In_Hours)))
		//	{
		//		return (TargetType)_local_time/3600.0f;
		//	}
		//	feature_implementation TargetType Total_Time(call_requirements(requires(ComponentType, Is_Dispatched) && requires(TargetType,Concepts::Time_In_Minutes)))
		//	{
		//		return (TargetType)_local_time/60.0f;
		//	}
		//	feature_implementation TargetType Total_Time(call_requirements(requires(ComponentType, Is_Dispatched) && requires(TargetType,Concepts::Time_In_Seconds)))
		//	{
		//		return (TargetType)_local_time;
		//	}
		//	feature_implementation void Total_Time(call_requirements(requires(TargetType,!Concepts::Time_In_DRSeconds) && requires(TargetType,!Concepts::Time_In_Seconds) && 
		//		requires(TargetType,!Concepts::Time_In_Minutes) && requires(TargetType,!Concepts::Time_In_Hours) && requires(TargetType,!Concepts::Time_In_Days)))
		//	{
		//		assert_requirements(ComponentType, Is_Dispatched, "ComponentType is not dispatched.");
		//		assert_requirements(TargetType, Concepts::Time_In_DRSeconds,"Your TargetType does not contain a valid Time_Type tag (i.e Days, Hours, Minutes, Seconds, DRSeconds).");
		//	}


		//	//================================================================================================
		//	/// Storage for local_time value
		//	//------------------------------------------------------------------------------------------------
		//	member_data(Value_Type, local_time, requires(TargetType,is_arithmetic),requires(TargetType, is_arithmetic));		
		//};
	}

	//namespace Components
	//{
	//	implementation struct Inches
	//	{
	//		typedef Polaris_Component<Implementations::Inches_Implementation, MasterType> type;
	//	};
	//	implementation struct Feet
	//	{
	//		typedef Polaris_Component<Implementations::Feet_Implementation,MasterType> type;
	//	};
	//	implementation struct Miles
	//	{
	//		typedef Polaris_Component<Implementations::Miles_Implementation,MasterType> type;
	//	};
	//	implementation struct Centimeters
	//	{
	//		typedef Polaris_Component<Implementations::Centimeters_Implementation,MasterType> type;
	//	};
	//	implementation struct Meters
	//	{
	//		typedef Polaris_Component<Implementations::Meters_Implementation,MasterType> type;
	//	};
	//	implementation struct Kilometers
	//	{
	//		typedef Polaris_Component<Implementations::Kilometers_Implementation,MasterType> type;
	//	};

	//	implementation struct Square_Inches
	//	{
	//		typedef Polaris_Component<Implementations::Inches_Implementation, MasterType> type;
	//	};
	//	implementation struct Square_Feet
	//	{
	//		typedef Polaris_Component<Implementations::Feet_Implementation,MasterType> type;
	//	};
	//	implementation struct Square_Miles
	//	{
	//		typedef Polaris_Component<Implementations::Miles_Implementation,MasterType> type;
	//	};
	//	implementation struct Square_Centimeters
	//	{
	//		typedef Polaris_Component<Implementations::Centimeters_Implementation,MasterType> type;
	//	};
	//	implementation struct Square_Meters
	//	{
	//		typedef Polaris_Component<Implementations::Meters_Implementation,MasterType> type;
	//	};
	//	implementation struct Square_Kilometers
	//	{
	//		typedef Polaris_Component<Implementations::Kilometers_Implementation,MasterType> type;
	//	};

	//	implementation struct Cubic_Inches
	//	{
	//		typedef Polaris_Component<Implementations::Inches_Implementation, MasterType> type;
	//	};
	//	implementation struct Cubic_Feet
	//	{
	//		typedef Polaris_Component<Implementations::Feet_Implementation,MasterType> type;
	//	};
	//	implementation struct Cubic_Miles
	//	{
	//		typedef Polaris_Component<Implementations::Miles_Implementation,MasterType> type;
	//	};
	//	implementation struct Cubic_Centimeters
	//	{
	//		typedef Polaris_Component<Implementations::Centimeters_Implementation,MasterType> type;
	//	};
	//	implementation struct Cubic_Meters
	//	{
	//		typedef Polaris_Component<Implementations::Meters_Implementation,MasterType> type;
	//	};
	//	implementation struct Cubic_Kilometers
	//	{
	//		typedef Polaris_Component<Implementations::Kilometers_Implementation,MasterType> type;
	//	};

	//	implementation struct DRSeconds
	//	{
	//		typedef Polaris_Component<Implementations::DRSeconds_Implementation,MasterType> type;
	//	};
	//	implementation struct Seconds
	//	{
	//		typedef Polaris_Component<Implementations::Seconds_Implementation,MasterType> type;
	//	};
	//	implementation struct Minutes
	//	{
	//		typedef Polaris_Component<Implementations::Minutes_Implementation,MasterType> type;
	//	};
	//	implementation struct Hours
	//	{
	//		typedef Polaris_Component<Implementations::Hours_Implementation,MasterType> type;
	//	};
	//	implementation struct Days
	//	{
	//		typedef Polaris_Component<Implementations::Days_Implementation,MasterType> type;
	//	};
	//}
}