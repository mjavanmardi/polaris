#pragma once
#include "Basic_Units_Prototypes.h"

namespace Basic_Units
{
	namespace Implementations
	{
		//================================================================================================
		/// Basic Length base clase.
		#pragma region Lenth Implementations
		implementation struct Measure_1D_Implementation {typedef true_type Length_tag; member_data(Value_Type,Value,none,none);};
		implementation struct Inches_Implementation : Measure_1D_Implementation<MasterType>, public Polaris_Component_Class<Inches_Implementation,MasterType,Data_Object,ParentType>			{typedef true_type Inches_tag;};
		implementation struct Feet_Implementation : Measure_1D_Implementation<MasterType>, public Polaris_Component_Class<Feet_Implementation,MasterType,Data_Object,ParentType>				{typedef true_type Feet_tag;};
		implementation struct Miles_Implementation  : Measure_1D_Implementation<MasterType>, public Polaris_Component_Class<Miles_Implementation,MasterType,Data_Object,ParentType>				{typedef true_type Miles_tag;};
		implementation struct Centimeters_Implementation : Measure_1D_Implementation<MasterType>, public Polaris_Component_Class<Centimeters_Implementation,MasterType,Data_Object,ParentType>	{typedef true_type Centimeters_tag;};
		implementation struct Meters_Implementation : Measure_1D_Implementation<MasterType>, public Polaris_Component_Class<Meters_Implementation,MasterType,Data_Object,ParentType>			{typedef true_type Meters_tag;};
		implementation struct Kilometers_Implementation : Measure_1D_Implementation<MasterType>, public Polaris_Component_Class<Kilometers_Implementation,MasterType,Data_Object,ParentType>	{typedef true_type Kilometers_tag;};
		#pragma endregion

		//================================================================================================
		/// Basic Area base clase.
		#pragma region Area Implementations
		implementation struct Measure_2D_Implementation : Measure_1D_Implementation<MasterType>{typedef true_type Area_tag;};
		implementation struct Square_Inches_Implementation : Inches_Implementation<MasterType> {};
		implementation struct Square_Feet_Implementation : Feet_Implementation<MasterType> {};
		implementation struct Square_Miles_Implementation : Miles_Implementation<MasterType> {};
		implementation struct Square_Centimeters_Implementation : Centimeters_Implementation<MasterType> {};
		implementation struct Square_Meters_Implementation : Meters_Implementation<MasterType> {};
		implementation struct Square_Kilometers_Implementation : Kilometers_Implementation<MasterType> {};
		#pragma endregion

		//================================================================================================
		/// Basic Volume base clase.
		#pragma region Volume Implementations
		implementation struct Measure_3D_Implementation : Measure_1D_Implementation<MasterType>{typedef true_type Volume_tag;};
		implementation struct Cubic_Inches_Implementation : Inches_Implementation<MasterType> {};
		implementation struct Cubic_Feet_Implementation : Feet_Implementation<MasterType> {};
		implementation struct Cubic_Miles_Implementation : Miles_Implementation<MasterType> {};
		implementation struct Cubic_Centimeters_Implementation : Centimeters_Implementation<MasterType> {};
		implementation struct Cubic_Meters_Implementation : Meters_Implementation<MasterType> {};
		implementation struct Cubic_Kilometers_Implementation : Kilometers_Implementation<MasterType> {};
		#pragma endregion

		//================================================================================================
		/// Basic Time base clase.
		implementation struct Time_Implementation {typedef true_type Time_tag;member_data(Value_Type,Value,none,none);};
		implementation struct DRSeconds_Implementation : Time_Implementation<MasterType>{typedef true_type DRSeconds_tag;};
		implementation struct Seconds_Implementation : Time_Implementation<MasterType>	{typedef true_type Seconds_tag;};
		implementation struct Minutes_Implementation : Time_Implementation<MasterType>	{typedef true_type Minutes_tag;};
		implementation struct Hours_Implementation : Time_Implementation<MasterType>	{typedef true_type Hours_tag;};
		implementation struct Days_Implementation : Time_Implementation<MasterType>		{typedef true_type Days_tag;};

		//================================================================================================
		/// Basic Rate base clase.
		implementation struct Rate_Implementation : Time_Implementation<MasterType>{typedef true_type Rate_tag;};
		implementation struct Units_Per_DRSecond_Implementation : DRSeconds_Implementation<MasterType>{};
		implementation struct Units_Per_Second_Implementation : Seconds_Implementation<MasterType>{};
		implementation struct Units_Per_Minute_Implementation : Minutes_Implementation<MasterType>{};
		implementation struct Units_Per_Hour_Implementation : Hours_Implementation<MasterType>{};
		implementation struct Units_Per_Day_Implementation : Days_Implementation<MasterType>{};


		//================================================================================================
		/// Basic Speed_component base clase.
		implementation struct Speed_Implementation : Time_Implementation<MasterType>{typedef true_type Speed_tag; typedef true_type Length_tag;};
		implementation struct Feet_Per_Second_Implementation : Seconds_Implementation<MasterType>, Speed_Implementation<MasterType>			{typedef true_type Feet_tag;};
		implementation struct Feet_Per_Minute_Implementation : Minutes_Implementation<MasterType>, Speed_Implementation<MasterType>			{typedef true_type Feet_tag;};
		implementation struct Feet_Per_Hour_Implementation : Hours_Implementation<MasterType>, Speed_Implementation<MasterType>				{typedef true_type Feet_tag;};
		implementation struct Meters_Per_Second_Implementation : Seconds_Implementation<MasterType>, Speed_Implementation<MasterType>		{typedef true_type Meters_tag;};
		implementation struct Meters_Per_Minute_Implementation : Minutes_Implementation<MasterType>, Speed_Implementation<MasterType>		{typedef true_type Meters_tag;};
		implementation struct Meters_Per_Hour_Implementation : Hours_Implementation<MasterType>, Speed_Implementation<MasterType>			{typedef true_type Meters_tag;};
		implementation struct Miles_Per_Second_Implementation : Seconds_Implementation<MasterType>, Speed_Implementation<MasterType>		{typedef true_type Miles_tag;};
		implementation struct Miles_Per_Minute_Implementation : Minutes_Implementation<MasterType>, Speed_Implementation<MasterType>		{typedef true_type Miles_tag;};
		implementation struct Miles_Per_Hour_Implementation : Hours_Implementation<MasterType>, Speed_Implementation<MasterType>			{typedef true_type Miles_tag;};
		implementation struct Kilometers_Per_Second_Implementation : Seconds_Implementation<MasterType>, Speed_Implementation<MasterType>	{typedef true_type Kilometers_tag;};
		implementation struct Kilometers_Per_Minute_Implementation : Minutes_Implementation<MasterType>, Speed_Implementation<MasterType>	{typedef true_type Kilometers_tag;};
		implementation struct Kilometers_Per_Hour_Implementation : Hours_Implementation<MasterType>, Speed_Implementation<MasterType>		{typedef true_type Kilometers_tag;};

		//================================================================================================
		/// Basic Currency base clase.
		#pragma region Currency Implementations
		implementation struct Currency_Implementation {typedef true_type Currency_tag; member_data(Value_Type,Value,none,none);};
		implementation struct Cents_Implementation : Currency_Implementation<MasterType>			{typedef true_type Cents_tag;};
		implementation struct Dollars_Implementation : Currency_Implementation<MasterType>			{typedef true_type Dollars_tag;};
		implementation struct Thousand_Dollars_Implementation  : Currency_Implementation<MasterType>{typedef true_type Thousand_Dollars_tag;};
		implementation struct Million_Dollars_Implementation : Currency_Implementation<MasterType>	{typedef true_type Million_Dollars_tag;};

		#pragma endregion
	}


	//template<typename ParentType> struct Length_In_Meters {typedef Polaris_Component<Basic_Units::Implementations::Meters_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	//template<typename ParentType> struct Length_In_Feet {typedef Polaris_Component<Basic_Units::Implementations::Feet_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	//template<typename ParentType> struct Length_In_Inches {typedef Polaris_Component<Basic_Units::Implementations::Inches_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	//template<typename ParentType> struct Length_In_Centimeters {typedef Polaris_Component<Basic_Units::Implementations::Centimeters_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	//template<typename ParentType> struct Length_In_Kilometers {typedef Polaris_Component<Basic_Units::Implementations::Kilometers_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	//template<typename ParentType> struct Length_In_Miles {typedef Polaris_Component<Basic_Units::Implementations::Miles_Implementation, NULLTYPE, Data_Object, ParentType> type;};

	template<typename ParentType> struct Area_In_Square_Meters {typedef Polaris_Component<Basic_Units::Implementations::Square_Meters_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	template<typename ParentType> struct Area_In_Square_Feet {typedef Polaris_Component<Basic_Units::Implementations::Square_Feet_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	template<typename ParentType> struct Area_In_Square_Inches {typedef Polaris_Component<Basic_Units::Implementations::Square_Inches_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	template<typename ParentType> struct Area_In_Square_Centimeters {typedef Polaris_Component<Basic_Units::Implementations::Square_Centimeters_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	template<typename ParentType> struct Area_In_Square_Kilometers {typedef Polaris_Component<Basic_Units::Implementations::Square_Kilometers_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	template<typename ParentType> struct Area_In_Square_Miles {typedef Polaris_Component<Basic_Units::Implementations::Square_Miles_Implementation, NULLTYPE, Data_Object, ParentType> type;};

	template<typename ParentType> struct Volume_In_Cubic_Meters {typedef Polaris_Component<Basic_Units::Implementations::Cubic_Meters_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	template<typename ParentType> struct Volume_In_Cubic_Feet {typedef Polaris_Component<Basic_Units::Implementations::Cubic_Feet_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	template<typename ParentType> struct Volume_In_Cubic_Inches {typedef Polaris_Component<Basic_Units::Implementations::Cubic_Inches_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	template<typename ParentType> struct Volume_In_Cubic_Centimeters {typedef Polaris_Component<Basic_Units::Implementations::Cubic_Centimeters_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	template<typename ParentType> struct Volume_In_Cubic_Kilometers {typedef Polaris_Component<Basic_Units::Implementations::Cubic_Kilometers_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	template<typename ParentType> struct Volume_In_Cubic_Miles {typedef Polaris_Component<Basic_Units::Implementations::Cubic_Miles_Implementation, NULLTYPE, Data_Object, ParentType> type;};

	template<typename ParentType> struct Time_In_DRSeconds {typedef Polaris_Component<Basic_Units::Implementations::DRSeconds_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	template<typename ParentType> struct Time_In_Seconds {typedef Polaris_Component<Basic_Units::Implementations::Seconds_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	template<typename ParentType> struct Time_In_Minutes {typedef Polaris_Component<Basic_Units::Implementations::Minutes_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	template<typename ParentType> struct Time_In_Hours {typedef Polaris_Component<Basic_Units::Implementations::Hours_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	template<typename ParentType> struct Time_In_Days {typedef Polaris_Component<Basic_Units::Implementations::Days_Implementation, NULLTYPE, Data_Object, ParentType> type;};

	template<typename ParentType> struct Speed_In_Meters_Per_Second {typedef Polaris_Component<Basic_Units::Implementations::Meters_Per_Second_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	template<typename ParentType> struct Speed_In_Feet_Per_Second {typedef Polaris_Component<Basic_Units::Implementations::Feet_Per_Second_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	template<typename ParentType> struct Speed_In_Kilometers_Per_Second {typedef Polaris_Component<Basic_Units::Implementations::Kilometers_Per_Second_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	template<typename ParentType> struct Speed_In_Miles_Per_Second {typedef Polaris_Component<Basic_Units::Implementations::Miles_Per_Second_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	template<typename ParentType> struct Speed_In_Meters_Per_Minute {typedef Polaris_Component<Basic_Units::Implementations::Meters_Per_Minute_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	template<typename ParentType> struct Speed_In_Feet_Per_Minute {typedef Polaris_Component<Basic_Units::Implementations::Feet_Per_Minute_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	template<typename ParentType> struct Speed_In_Kilometers_Per_Minute {typedef Polaris_Component<Basic_Units::Implementations::Kilometers_Per_Minute_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	template<typename ParentType> struct Speed_In_Miles_Per_Minute {typedef Polaris_Component<Basic_Units::Implementations::Miles_Per_Minute_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	template<typename ParentType> struct Speed_In_Meters_Per_Hour {typedef Polaris_Component<Basic_Units::Implementations::Meters_Per_Hour_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	template<typename ParentType> struct Speed_In_Feet_Per_Hour {typedef Polaris_Component<Basic_Units::Implementations::Feet_Per_Hour_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	template<typename ParentType> struct Speed_In_Kilometers_Per_Hour {typedef Polaris_Component<Basic_Units::Implementations::Kilometers_Per_Hour_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	template<typename ParentType> struct Speed_In_Miles_Per_Hour {typedef Polaris_Component<Basic_Units::Implementations::Miles_Per_Hour_Implementation, NULLTYPE, Data_Object, ParentType> type;};

	template<typename ParentType> struct Currency_In_Cents {typedef Polaris_Component<Basic_Units::Implementations::Cents_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	template<typename ParentType> struct Currency_In_Dollars {typedef Polaris_Component<Basic_Units::Implementations::Dollars_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	template<typename ParentType> struct Currency_In_Thousand_Dollars {typedef Polaris_Component<Basic_Units::Implementations::Thousand_Dollars_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	template<typename ParentType> struct Currency_In_Million_Dollars {typedef Polaris_Component<Basic_Units::Implementations::Million_Dollars_Implementation, NULLTYPE, Data_Object, ParentType> type;};

}


//=================================================================================================================
// SIMULATION TIMER DEFINITION - Used by agents as a wrapper for the global _iteration variable
typedef Polaris_Component<Basic_Units::Implementations::Seconds_Implementation, NULLTYPE, Data_Object> Basic_Time;
template<typename Base_Time_Type>
struct Simulation_Timer
{
	template<typename TargetType> TargetType Current_Time(requires(check(Base_Time_Type,Basic_Units::Concepts::Is_Time_Value) && check(TargetType,Basic_Units::Concepts::Is_Time_Value)))
	{
		return Basic_Units::Prototypes::Time_Prototype<Basic_Time>::Convert_Value<Target_Type<TargetType,Base_Time_Type>>((Base_Time_Type::ValueType)_iteration);
	}
	template<typename TargetType> Base_Time_Type Future_Time(TargetType Additional_Time_Increment, requires(check(Base_Time_Type,Basic_Units::Concepts::Is_Time_Value) && check(TargetType,Basic_Units::Concepts::Is_Time_Value)))
	{
		Base_Time_Type current_time;
		current_time = (Base_Time_Type::ValueType)_iteration;
		TargetType additional_time = Basic_Units::Prototypes::Time_Prototype<Basic_Time>::Convert_Value<Target_Type<Base_Time_Type,TargetType>>(Additional_Time_Increment);
		return  current_time + additional_time;
	}
};

Simulation_Timer<Simulation_Timestep_Increment> Simulation_Time;