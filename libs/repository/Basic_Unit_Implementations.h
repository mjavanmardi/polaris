#pragma once
#include "Basic_Units_Prototypes.h"

namespace polaris
{
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
		implementation struct Length_Implementation : public Polaris_Component<MasterType,INHERIT(Length_Implementation),NT>
		{
			typedef true_type Length_tag;
			typedef true_type Inches_tag;
			m_data(Value_Type,Value, NONE, NONE);
            //tag_getter_as_available(Value);
            //tag_setter_as_available(Value);
		};

		//================================================================================================
		/// Basic Area base clase.
		implementation struct Area_Implementation : public Length_Implementation<MasterType,INHERIT(Area_Implementation)>{ typedef true_type Area_tag;};

		//================================================================================================
		/// Basic Volume base clase.
		implementation struct Volume_Implementation : public Length_Implementation<MasterType,INHERIT(Volume_Implementation)>{ typedef true_type Volume_tag;};

		//================================================================================================
		/// Basic Time base clase and Time Horizon Classes.
		implementation struct Time_Implementation : public Polaris_Component<MasterType,INHERIT(Time_Implementation),NT>
		{
			Time_Implementation()
			{
				this->_Value = 0;
			}
			typedef true_type Time_tag;
			typedef true_type Seconds_tag;
			m_data(Time_Value_Type,Value, NONE, NONE);
            //tag_getter_as_available(Value);
            //tag_setter_as_available(Value);
		};

		//================================================================================================
		/// Time Horizon base clase.
		implementation struct Horizon_Implementation : public Time_Implementation<MasterType,INHERIT(Horizon_Implementation)>{ typedef true_type Horizon_tag; typedef true_type Impulsive_tag;};

		//================================================================================================
		/// Basic Rate and Speed base classes.
		implementation struct Rate_Implementation : public Time_Implementation<MasterType,INHERIT(Rate_Implementation)>{ typedef true_type Rate_tag;};

		//================================================================================================
		/// Basic Rate and Speed base classes.
		implementation struct Speed_Implementation : public Rate_Implementation<MasterType,INHERIT(Speed_Implementation)>
		{
			typedef true_type Speed_tag; 
			typedef true_type Length_tag;
			typedef true_type Inches_tag;
		};

		implementation struct Acceleration_Implementation : public Speed_Implementation<MasterType,INHERIT(Acceleration_Implementation)>
		{
			typedef true_type Acceleration_tag; 
		};

		//================================================================================================
		/// Basic Currency base clase.
		implementation struct Currency_Implementation : public Polaris_Component<MasterType,INHERIT(Currency_Implementation),NT>
		{
			typedef true_type Currency_tag;
			typedef true_type Cents_tag; 
			m_data(Value_Type,Value, NONE, NONE);
            //tag_getter_as_available(Value);
            //tag_setter_as_available(Value);
		};
		//implementation struct _Currency {typedef true_type Currency_tag; typedef true_type Cents_tag; m_data(Value_Type,Value, NONE, NONE);};		
	}
}

}
namespace GLOBALS
{
//=================================================================================================================
// SIMULATION TIMER DEFINITION - Used by agents as a wrapper for the global _iteration variable
typedef Basic_Units::Implementations::Time_Implementation<NULLTYPE> Basic_Time;
typedef Basic_Units::Implementations::Currency_Implementation<NT> Basic_Currency;
typedef Basic_Units::Implementations::Length_Implementation<NT> Basic_Length;
typedef Basic_Units::Implementations::Area_Implementation<NT> Basic_Area;
typedef Basic_Units::Implementations::Speed_Implementation<NT> Basic_Speed;
typedef Basic_Units::Implementations::Acceleration_Implementation<NT> Basic_Acceleration;

template<typename Base_Time_Type>
struct _Simulation_Timer
{
	template<typename TargetType> TargetType Current_Time();
	template<typename InputType, typename ReturnType> ReturnType Future_Time(InputType Additional_Time_Increment);
	template<typename InputType> Simulation_Timestep_Increment Convert_Time_To_Simulation_Timestep(InputType Time);
};

template<typename Base_Time_Type>
template<typename TargetType>
TargetType _Simulation_Timer<Base_Time_Type>::Current_Time()
{
	return Basic_Units::Prototypes::Time<Basic_Time>::Convert_Value<Base_Time_Type, TargetType>((typename Base_Time_Type::ValueType)iteration());
}

template<typename Base_Time_Type>
template<typename InputType, typename ReturnType>
ReturnType _Simulation_Timer<Base_Time_Type>::Future_Time(InputType Additional_Time_Increment)
{
	Simulation_Timestep_Increment current_time;
	current_time = (Simulation_Timestep_Increment)iteration();
	Simulation_Timestep_Increment additional_time = Basic_Units::Prototypes::Time<Basic_Time>::Convert_Value<InputType, Simulation_Timestep_Increment>(Additional_Time_Increment);
	ReturnType return_value = Basic_Units::Prototypes::Time<Basic_Time>::Convert_Value<Simulation_Timestep_Increment, ReturnType>(Round<int, Basic_Units::Time_Value_Type>(current_time) + Round<int, Basic_Units::Time_Value_Type>(additional_time));
	return (ReturnType)Round<int, Basic_Units::Time_Value_Type>(return_value);
}

template<typename Base_Time_Type>
template<typename InputType>
Simulation_Timestep_Increment _Simulation_Timer<Base_Time_Type>::Convert_Time_To_Simulation_Timestep(InputType Time)
{
	return Basic_Units::Prototypes::Time<Basic_Time>::Convert_Value<InputType, Simulation_Timestep_Increment>(Time);
}

static _Simulation_Timer<Simulation_Timestep_Increment> Simulation_Time;

static Basic_Units::Prototypes::Time<Basic_Time> Time_Converter;
static Basic_Units::Prototypes::Currency<Basic_Currency> Currency_Converter;
static Basic_Units::Prototypes::Length<Basic_Length> Length_Converter;
static Basic_Units::Prototypes::Area<Basic_Area> Area_Converter;
static Basic_Units::Prototypes::Speed<Basic_Speed> Speed_Converter;
static Basic_Units::Prototypes::Acceleration<Basic_Acceleration> Acceleration_Converter;

template<typename Input_Unit_Type, typename Output_Unit_Type> Output_Unit_Type Convert_Units(Input_Unit_Type value, requires(Output_Unit_Type, check(Output_Unit_Type, Basic_Units::Concepts::Is_Time_Value) && !check(Output_Unit_Type,Basic_Units::Concepts::Is_Speed_Value) && !check(Output_Unit_Type, Basic_Units::Concepts::Is_Acceleration_Value)))
{
	return Time_Converter.Convert_Value<Input_Unit_Type, Output_Unit_Type>(value);
}
template<typename Input_Unit_Type, typename Output_Unit_Type> Output_Unit_Type Convert_Units(Input_Unit_Type value, requires(Output_Unit_Type, check(Output_Unit_Type, Basic_Units::Concepts::Is_Currency_Value)))
{
	return Currency_Converter.Convert_Value<Input_Unit_Type, Output_Unit_Type>(value);
}
template<typename Input_Unit_Type, typename Output_Unit_Type> Output_Unit_Type Convert_Units(Input_Unit_Type value, requires(Output_Unit_Type, check(Output_Unit_Type, Basic_Units::Concepts::Is_Length_Value) && !check(Output_Unit_Type, Basic_Units::Concepts::Is_Area_Value) && !check(Output_Unit_Type, Basic_Units::Concepts::Is_Speed_Value) && !check(Output_Unit_Type, Basic_Units::Concepts::Is_Acceleration_Value)))
{
	return Length_Converter.Convert_Value<Input_Unit_Type, Output_Unit_Type>(value);
}
template<typename Input_Unit_Type, typename Output_Unit_Type> Output_Unit_Type Convert_Units(Input_Unit_Type value, requires(Output_Unit_Type, check(Output_Unit_Type, Basic_Units::Concepts::Is_Area_Value)))
{
	return Area_Converter.Convert_Value<Input_Unit_Type, Output_Unit_Type>(value);
}
template<typename Input_Unit_Type, typename Output_Unit_Type> Output_Unit_Type Convert_Units(Input_Unit_Type value, requires(Output_Unit_Type, check(Output_Unit_Type, Basic_Units::Concepts::Is_Speed_Value) && !check(Output_Unit_Type, Basic_Units::Concepts::Is_Acceleration_Value)))
{
	return Speed_Converter.Convert_Value<Input_Unit_Type, Output_Unit_Type>(value);
}
template<typename Input_Unit_Type, typename Output_Unit_Type> Output_Unit_Type Convert_Units(Input_Unit_Type value, requires(Output_Unit_Type, check(Output_Unit_Type, Basic_Units::Concepts::Is_Acceleration_Value)))
{
	return Acceleration_Converter.Convert_Value<Input_Unit_Type, Output_Unit_Type>(value);
}

}

using namespace GLOBALS;
