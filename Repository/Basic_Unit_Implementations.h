#pragma once
#include "Basic_Units_Prototypes.h"

namespace Basic_Units
{
	namespace Implementations
	{
		//=======================================================================
		// DESIGN NOTE:
		// Separate Base classess into inheritable and non-inheritable components
		//-----------------------------------------------------------------------

		//================================================================================================
		/// Basic Length base clase.
		implementation struct Length_Implementation : public Polaris_Component<APPEND_CHILD(Length_Implementation),MasterType>
		{
			typedef true_type Length_tag;
			typedef true_type Inches_tag;
			member_data(Value_Type,Value,none,none);
		};

		//================================================================================================
		/// Basic Area base clase.
		implementation struct Area_Implementation : public Length_Implementation<MasterType,ParentType,APPEND_CHILD(Area_Implementation)>{ typedef true_type Area_tag;};

		//================================================================================================
		/// Basic Volume base clase.
		implementation struct Volume_Implementation : public Length_Implementation<MasterType,ParentType,APPEND_CHILD(Volume_Implementation)>{ typedef true_type Volume_tag;};

		//================================================================================================
		/// Basic Time base clase and Time Horizon Classes.
		implementation struct Time_Implementation : public Polaris_Component<APPEND_CHILD(Time_Implementation),MasterType>
		{
			typedef true_type Time_tag;
			typedef true_type Seconds_tag;
			member_data(Value_Type,Value,none,none);
		};

		//================================================================================================
		/// Time Horizon base clase.
		implementation struct Horizon_Implementation : public Time_Implementation<MasterType,ParentType,APPEND_CHILD(Horizon_Implementation)>{ typedef true_type Horizon_tag; typedef true_type Impulsive_tag;};

		//================================================================================================
		/// Basic Rate and Speed base classes.
		implementation struct Rate_Implementation : public Time_Implementation<MasterType,ParentType,APPEND_CHILD(Rate_Implementation)>{ typedef true_type Rate_tag;};

		//================================================================================================
		/// Basic Rate and Speed base classes.
		implementation struct Speed_Implementation : public Rate_Implementation<MasterType,ParentType,APPEND_CHILD(Speed_Implementation)>
		{
			typedef true_type Speed_tag; 
			typedef true_type Length_tag;
			typedef true_type Inches_tag;
		};

		//================================================================================================
		/// Basic Currency base clase.
		implementation struct Currency_Implementation : public Polaris_Component<APPEND_CHILD(Currency_Implementation),MasterType>
		{
			typedef true_type Currency_tag;
			typedef true_type Cents_tag; 
			member_data(Value_Type,Value,none,none);
		};
		implementation struct _Currency {typedef true_type Currency_tag; typedef true_type Cents_tag; member_data(Value_Type,Value,none,none);};		
	}
}



//=================================================================================================================
// SIMULATION TIMER DEFINITION - Used by agents as a wrapper for the global _iteration variable
typedef Basic_Units::Implementations::Time_Implementation<NULLTYPE> Basic_Time;
template<typename Base_Time_Type>
struct _Simulation_Timer
{
	template<typename TargetType> TargetType Current_Time()
	{
		return Basic_Units::Prototypes::Time_Prototype<Basic_Time>::Convert_Value<Target_Type<NULLTYPE,TargetType,Base_Time_Type>>((typename Base_Time_Type::ValueType)_iteration);
	}
	template<typename InputType, typename TargetType> TargetType Future_Time(InputType Additional_Time_Increment)
	{
		Simulation_Timestep_Increment current_time;
		current_time = (Simulation_Timestep_Increment)_iteration;
		Simulation_Timestep_Increment additional_time = Basic_Units::Prototypes::Time_Prototype<Basic_Time>::Convert_Value<Target_Type<NULLTYPE,Simulation_Timestep_Increment,InputType>>(Additional_Time_Increment);
		return Basic_Units::Prototypes::Time_Prototype<Basic_Time>::Convert_Value<Target_Type<NULLTYPE,TargetType,Simulation_Timestep_Increment>>(current_time + additional_time);
	}
};

_Simulation_Timer<Simulation_Timestep_Increment> Simulation_Time;