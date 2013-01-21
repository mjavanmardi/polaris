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
		implementation struct Length_In_Inches : Measure_1D_Implementation<MasterType,ParentType>, public Polaris_Component_Class<Length_In_Inches,MasterType,Data_Object,ParentType>			{typedef true_type Inches_tag;};
		implementation struct Length_In_Feet : Measure_1D_Implementation<MasterType,ParentType>, public Polaris_Component_Class<Length_In_Feet,MasterType,Data_Object,ParentType>				{typedef true_type Feet_tag;};
		implementation struct Length_In_Miles : Measure_1D_Implementation<MasterType,ParentType>, public Polaris_Component_Class<Length_In_Miles,MasterType,Data_Object,ParentType>			{typedef true_type Miles_tag;};
		implementation struct Length_In_Centimeters : Measure_1D_Implementation<MasterType,ParentType>, public Polaris_Component_Class<Length_In_Centimeters,MasterType,Data_Object,ParentType>{typedef true_type Centimeters_tag;};
		implementation struct Length_In_Meters : Measure_1D_Implementation<MasterType,ParentType>, public Polaris_Component_Class<Length_In_Meters,MasterType,Data_Object,ParentType>			{typedef true_type Meters_tag;};
		implementation struct Length_In_Kilometers : Measure_1D_Implementation<MasterType,ParentType>, public Polaris_Component_Class<Length_In_Kilometers,MasterType,Data_Object,ParentType>	{typedef true_type Kilometers_tag;};
		#pragma endregion

		//================================================================================================
		/// Basic Area base clase.
		#pragma region Area Implementations
		implementation struct Measure_2D_Implementation : Measure_1D_Implementation<MasterType,ParentType>{typedef true_type Area_tag;};
		implementation struct Area_In_Square_Inches : Length_In_Inches<MasterType,ParentType>, public Polaris_Component_Class<Area_In_Square_Inches,MasterType,Data_Object,ParentType>	 {};
		implementation struct Area_In_Square_Feet : Length_In_Feet<MasterType,ParentType>, public Polaris_Component_Class<Area_In_Square_Feet,MasterType,Data_Object,ParentType>	 {};
		implementation struct Area_In_Square_Miles : Length_In_Miles<MasterType,ParentType>, public Polaris_Component_Class<Area_In_Square_Miles,MasterType,Data_Object,ParentType>	 {};
		implementation struct Area_In_Square_Centimeters : Length_In_Centimeters<MasterType,ParentType>, public Polaris_Component_Class<Area_In_Square_Centimeters,MasterType,Data_Object,ParentType>	 {};
		implementation struct Area_In_Square_Meters : Length_In_Meters<MasterType,ParentType>, public Polaris_Component_Class<Area_In_Square_Meters,MasterType,Data_Object,ParentType>	 {};
		implementation struct Area_In_Square_Kilometers : Length_In_Kilometers<MasterType,ParentType>, public Polaris_Component_Class<Area_In_Square_Kilometers,MasterType,Data_Object,ParentType>	 {};
		#pragma endregion

		//================================================================================================
		/// Basic Volume base clase.
		#pragma region Volume Implementations
		implementation struct Measure_3D_Implementation : Measure_1D_Implementation<MasterType,ParentType>{typedef true_type Volume_tag;};
		implementation struct Volume_In_Cubic_Inches : Length_In_Inches<MasterType,ParentType>, public Polaris_Component_Class<Volume_In_Cubic_Inches,MasterType,Data_Object,ParentType> {};
		implementation struct Volume_In_Cubic_Feet : Length_In_Feet<MasterType,ParentType>, public Polaris_Component_Class<Volume_In_Cubic_Feet,MasterType,Data_Object,ParentType> {};
		implementation struct Volume_In_Cubic_Miles : Length_In_Miles<MasterType,ParentType>, public Polaris_Component_Class<Volume_In_Cubic_Miles,MasterType,Data_Object,ParentType> {};
		implementation struct Volume_In_Cubic_Centimeters : Length_In_Centimeters<MasterType,ParentType>, public Polaris_Component_Class<Volume_In_Cubic_Centimeters,MasterType,Data_Object,ParentType> {};
		implementation struct Volume_In_Cubic_Meters : Length_In_Meters<MasterType,ParentType>, public Polaris_Component_Class<Volume_In_Cubic_Meters,MasterType,Data_Object,ParentType> {};
		implementation struct Volume_In_Cubic_Kilometers : Length_In_Kilometers<MasterType,ParentType>, public Polaris_Component_Class<Volume_In_Cubic_Kilometers,MasterType,Data_Object,ParentType> {};
		#pragma endregion

		//================================================================================================
		/// Basic Time base clase.
		implementation struct Time_Implementation {typedef true_type Time_tag;member_data(Value_Type,Value,none,none);};
		implementation struct Time_In_DRSeconds : Time_Implementation<MasterType,ParentType>, public Polaris_Component_Class<Time_In_DRSeconds,MasterType,Data_Object,ParentType>{typedef true_type DRSeconds_tag;};
		implementation struct Time_In_Seconds : Time_Implementation<MasterType,ParentType>, public Polaris_Component_Class<Time_In_Seconds,MasterType,Data_Object,ParentType>	{typedef true_type Seconds_tag;};
		implementation struct Time_In_Minutes : Time_Implementation<MasterType,ParentType>, public Polaris_Component_Class<Time_In_Minutes,MasterType,Data_Object,ParentType>	{typedef true_type Minutes_tag;};
		implementation struct Time_In_Hours : Time_Implementation<MasterType,ParentType>, public Polaris_Component_Class<Time_In_Hours,MasterType,Data_Object,ParentType>		{typedef true_type Hours_tag;};
		implementation struct Time_In_Days : Time_Implementation<MasterType,ParentType>, public Polaris_Component_Class<Time_In_Days,MasterType,Data_Object,ParentType>		{typedef true_type Days_tag;};

		//================================================================================================
		/// Time Horizon base clase.
		implementation struct Horizon_Impulsive : Time_In_Minutes<MasterType,ParentType>, public Polaris_Component_Class<Horizon_Impulsive,MasterType,Data_Object,ParentType>			{typedef true_type Horizon_tag; typedef true_type Impulsive_tag;};
		implementation struct Horizon_Within_1_Hour : Time_In_Minutes<MasterType,ParentType>, public Polaris_Component_Class<Horizon_Within_1_Hour,MasterType,Data_Object,ParentType>	{typedef true_type Horizon_tag; typedef true_type Within_1_Hour_tag;};
		implementation struct Horizon_Within_Hours : Time_In_Minutes<MasterType,ParentType>, public Polaris_Component_Class<Horizon_Within_Hours,MasterType,Data_Object,ParentType>	{typedef true_type Horizon_tag; typedef true_type Within_Hours_tag;};
		implementation struct Horizon_Within_Days : Time_In_Minutes<MasterType,ParentType>, public Polaris_Component_Class<Horizon_Within_Days,MasterType,Data_Object,ParentType>		{typedef true_type Horizon_tag; typedef true_type Within_Days_tag;};
		implementation struct Horizon_Same_Day : Time_In_Minutes<MasterType,ParentType>, public Polaris_Component_Class<Horizon_Same_Day,MasterType,Data_Object,ParentType>			{typedef true_type Horizon_tag; typedef true_type Same_Day_tag;};
		implementation struct Horizon_Previous_Day : Time_In_Minutes<MasterType,ParentType>, public Polaris_Component_Class<Horizon_Previous_Day,MasterType,Data_Object,ParentType>	{typedef true_type Horizon_tag; typedef true_type Previous_Day_tag;};
		implementation struct Horizon_Same_Week : Time_In_Minutes<MasterType,ParentType>, public Polaris_Component_Class<Horizon_Same_Week,MasterType,Data_Object,ParentType>			{typedef true_type Horizon_tag; typedef true_type Same_Week_tag;};
		implementation struct Horizon_Same_Month : Time_In_Minutes<MasterType,ParentType>, public Polaris_Component_Class<Horizon_Same_Month,MasterType,Data_Object,ParentType>		{typedef true_type Horizon_tag; typedef true_type Same_Month_tag;};
		implementation struct Horizon_Preplanned : Time_In_Minutes<MasterType,ParentType>, public Polaris_Component_Class<Horizon_Preplanned,MasterType,Data_Object,ParentType>		{typedef true_type Horizon_tag; typedef true_type Preplanned_tag;};
		implementation struct Horizon_Routine : Time_In_Minutes<MasterType,ParentType>, public Polaris_Component_Class<Horizon_Routine,MasterType,Data_Object,ParentType>				{typedef true_type Horizon_tag; typedef true_type Routine_tag;};

		//================================================================================================
		/// Basic Rate base clase.
		implementation struct Rate_Implementation : Time_Implementation<MasterType,ParentType>{typedef true_type Rate_tag;};
		implementation struct Units_Per_DRSecond : Time_In_DRSeconds<MasterType,ParentType>, public Polaris_Component_Class<Units_Per_DRSecond,MasterType,Data_Object,ParentType>{};
		implementation struct Units_Per_Second : Time_In_Seconds<MasterType,ParentType>, public Polaris_Component_Class<Units_Per_Second,MasterType,Data_Object,ParentType>{};
		implementation struct Units_Per_Minute : Time_In_Minutes<MasterType,ParentType>, public Polaris_Component_Class<Units_Per_Minute,MasterType,Data_Object,ParentType>{};
		implementation struct Units_Per_Hour : Time_In_Hours<MasterType,ParentType>, public Polaris_Component_Class<Units_Per_Hour,MasterType,Data_Object,ParentType>{};
		implementation struct Units_Per_Day : Time_In_Days<MasterType,ParentType>, public Polaris_Component_Class<Units_Per_Day,MasterType,Data_Object,ParentType>{};


		//================================================================================================
		/// Basic Speed_component base clase.
		implementation struct Speed_Implementation : Time_Implementation<MasterType,ParentType>{typedef true_type Speed_tag; typedef true_type Length_tag;};
		implementation struct Speed_In_Feet_Per_Second : Time_In_Seconds<MasterType,ParentType>, Speed_Implementation<MasterType,ParentType>, public Polaris_Component_Class<Speed_In_Feet_Per_Second,MasterType,Data_Object,ParentType>			{typedef true_type Feet_tag;};
		implementation struct Speed_In_Feet_Per_Minute : Time_In_Minutes<MasterType,ParentType>, Speed_Implementation<MasterType,ParentType>, public Polaris_Component_Class<Speed_In_Feet_Per_Minute,MasterType,Data_Object,ParentType>			{typedef true_type Feet_tag;};
		implementation struct Speed_In_Feet_Per_Hour : Time_In_Hours<MasterType,ParentType>, Speed_Implementation<MasterType,ParentType>, public Polaris_Component_Class<Speed_In_Feet_Per_Hour,MasterType,Data_Object,ParentType>					{typedef true_type Feet_tag;};
		implementation struct Speed_In_Meters_Per_Second : Time_In_Seconds<MasterType,ParentType>, Speed_Implementation<MasterType,ParentType>, public Polaris_Component_Class<Speed_In_Meters_Per_Second,MasterType,Data_Object,ParentType>		{typedef true_type Meters_tag;};
		implementation struct Speed_In_Meters_Per_Minute : Time_In_Minutes<MasterType,ParentType>, Speed_Implementation<MasterType,ParentType>, public Polaris_Component_Class<Speed_In_Meters_Per_Minute,MasterType,Data_Object,ParentType>		{typedef true_type Meters_tag;};
		implementation struct Speed_In_Meters_Per_Hour : Time_In_Hours<MasterType,ParentType>, Speed_Implementation<MasterType,ParentType>, public Polaris_Component_Class<Speed_In_Meters_Per_Hour,MasterType,Data_Object,ParentType>				{typedef true_type Meters_tag;};
		implementation struct Speed_In_Miles_Per_Second : Time_In_Seconds<MasterType,ParentType>, Speed_Implementation<MasterType,ParentType>, public Polaris_Component_Class<Speed_In_Miles_Per_Second,MasterType,Data_Object,ParentType>			{typedef true_type Miles_tag;};
		implementation struct Speed_In_Miles_Per_Minute : Time_In_Minutes<MasterType,ParentType>, Speed_Implementation<MasterType,ParentType>, public Polaris_Component_Class<Speed_In_Miles_Per_Minute,MasterType,Data_Object,ParentType>			{typedef true_type Miles_tag;};
		implementation struct Speed_In_Miles_Per_Hour : Time_In_Hours<MasterType,ParentType>, Speed_Implementation<MasterType,ParentType>, public Polaris_Component_Class<Speed_In_Miles_Per_Hour,MasterType,Data_Object,ParentType>				{typedef true_type Miles_tag;};
		implementation struct Speed_In_Kilometers_Per_Second : Time_In_Seconds<MasterType,ParentType>, Speed_Implementation<MasterType,ParentType>, public Polaris_Component_Class<Speed_In_Kilometers_Per_Second,MasterType,Data_Object,ParentType>{typedef true_type Kilometers_tag;};
		implementation struct Speed_In_Kilometers_Per_Minute : Time_In_Minutes<MasterType,ParentType>, Speed_Implementation<MasterType,ParentType>, public Polaris_Component_Class<Speed_In_Kilometers_Per_Minute,MasterType,Data_Object,ParentType>{typedef true_type Kilometers_tag;};
		implementation struct Speed_In_Kilometers_Per_Hour : Time_In_Hours<MasterType,ParentType>, Speed_Implementation<MasterType,ParentType>, public Polaris_Component_Class<Speed_In_Kilometers_Per_Hour,MasterType,Data_Object,ParentType>		{typedef true_type Kilometers_tag;};

		//================================================================================================
		/// Basic Currency base clase.
		#pragma region Currency Implementations
		implementation struct Currency_Implementation {typedef true_type Currency_tag; member_data(Value_Type,Value,none,none);};
		implementation struct Currency_In_Cents : Currency_Implementation<MasterType,ParentType>, public Polaris_Component_Class<Currency_In_Cents,MasterType,Data_Object,ParentType>						{typedef true_type Cents_tag;};
		implementation struct Currency_In_Dollars : Currency_Implementation<MasterType,ParentType>, public Polaris_Component_Class<Currency_In_Dollars,MasterType,Data_Object,ParentType>					{typedef true_type Dollars_tag;};
		implementation struct Currency_In_Thousand_Dollars  : Currency_Implementation<MasterType,ParentType>, public Polaris_Component_Class<Currency_In_Thousand_Dollars,MasterType,Data_Object,ParentType>{typedef true_type Thousand_Dollars_tag;};
		implementation struct Currency_In_Million_Dollars : Currency_Implementation<MasterType,ParentType>, public Polaris_Component_Class<Currency_In_Million_Dollars,MasterType,Data_Object,ParentType>	{typedef true_type Million_Dollars_tag;};

		#pragma endregion
	}


	//template<typename ParentType> struct Length_In_Meters {typedef Polaris_Component<Basic_Units::Implementations::Meters_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	//template<typename ParentType> struct Length_In_Feet {typedef Polaris_Component<Basic_Units::Implementations::Feet_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	//template<typename ParentType> struct Length_In_Inches {typedef Polaris_Component<Basic_Units::Implementations::Inches_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	//template<typename ParentType> struct Length_In_Centimeters {typedef Polaris_Component<Basic_Units::Implementations::Centimeters_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	//template<typename ParentType> struct Length_In_Kilometers {typedef Polaris_Component<Basic_Units::Implementations::Kilometers_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	//template<typename ParentType> struct Length_In_Miles {typedef Polaris_Component<Basic_Units::Implementations::Miles_Implementation, NULLTYPE, Data_Object, ParentType> type;};

	//template<typename ParentType> struct Area_In_Square_Meters {typedef Polaris_Component<Basic_Units::Implementations::Square_Meters_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	//template<typename ParentType> struct Area_In_Square_Feet {typedef Polaris_Component<Basic_Units::Implementations::Square_Feet_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	//template<typename ParentType> struct Area_In_Square_Inches {typedef Polaris_Component<Basic_Units::Implementations::Square_Inches_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	//template<typename ParentType> struct Area_In_Square_Centimeters {typedef Polaris_Component<Basic_Units::Implementations::Square_Centimeters_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	//template<typename ParentType> struct Area_In_Square_Kilometers {typedef Polaris_Component<Basic_Units::Implementations::Square_Kilometers_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	//template<typename ParentType> struct Area_In_Square_Miles {typedef Polaris_Component<Basic_Units::Implementations::Square_Miles_Implementation, NULLTYPE, Data_Object, ParentType> type;};

	//template<typename ParentType> struct Volume_In_Cubic_Meters {typedef Polaris_Component<Basic_Units::Implementations::Cubic_Meters_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	//template<typename ParentType> struct Volume_In_Cubic_Feet {typedef Polaris_Component<Basic_Units::Implementations::Cubic_Feet_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	//template<typename ParentType> struct Volume_In_Cubic_Inches {typedef Polaris_Component<Basic_Units::Implementations::Cubic_Inches_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	//template<typename ParentType> struct Volume_In_Cubic_Centimeters {typedef Polaris_Component<Basic_Units::Implementations::Cubic_Centimeters_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	//template<typename ParentType> struct Volume_In_Cubic_Kilometers {typedef Polaris_Component<Basic_Units::Implementations::Cubic_Kilometers_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	//template<typename ParentType> struct Volume_In_Cubic_Miles {typedef Polaris_Component<Basic_Units::Implementations::Cubic_Miles_Implementation, NULLTYPE, Data_Object, ParentType> type;};

	//template<typename ParentType> struct Time_In_DRSeconds {typedef Polaris_Component<Basic_Units::Implementations::DRSeconds_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	//template<typename ParentType> struct Time_In_Seconds {typedef Polaris_Component<Basic_Units::Implementations::Seconds_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	//template<typename ParentType> struct Time_In_Minutes {typedef Polaris_Component<Basic_Units::Implementations::Minutes_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	//template<typename ParentType> struct Time_In_Hours {typedef Polaris_Component<Basic_Units::Implementations::Hours_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	//template<typename ParentType> struct Time_In_Days {typedef Polaris_Component<Basic_Units::Implementations::Days_Implementation, NULLTYPE, Data_Object, ParentType> type;};

	//template<typename ParentType> struct Speed_In_Meters_Per_Second {typedef Polaris_Component<Basic_Units::Implementations::Meters_Per_Second_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	//template<typename ParentType> struct Speed_In_Feet_Per_Second {typedef Polaris_Component<Basic_Units::Implementations::Feet_Per_Second_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	//template<typename ParentType> struct Speed_In_Kilometers_Per_Second {typedef Polaris_Component<Basic_Units::Implementations::Kilometers_Per_Second_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	//template<typename ParentType> struct Speed_In_Miles_Per_Second {typedef Polaris_Component<Basic_Units::Implementations::Miles_Per_Second_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	//template<typename ParentType> struct Speed_In_Meters_Per_Minute {typedef Polaris_Component<Basic_Units::Implementations::Meters_Per_Minute_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	//template<typename ParentType> struct Speed_In_Feet_Per_Minute {typedef Polaris_Component<Basic_Units::Implementations::Feet_Per_Minute_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	//template<typename ParentType> struct Speed_In_Kilometers_Per_Minute {typedef Polaris_Component<Basic_Units::Implementations::Kilometers_Per_Minute_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	//template<typename ParentType> struct Speed_In_Miles_Per_Minute {typedef Polaris_Component<Basic_Units::Implementations::Miles_Per_Minute_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	//template<typename ParentType> struct Speed_In_Meters_Per_Hour {typedef Polaris_Component<Basic_Units::Implementations::Meters_Per_Hour_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	//template<typename ParentType> struct Speed_In_Feet_Per_Hour {typedef Polaris_Component<Basic_Units::Implementations::Feet_Per_Hour_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	//template<typename ParentType> struct Speed_In_Kilometers_Per_Hour {typedef Polaris_Component<Basic_Units::Implementations::Kilometers_Per_Hour_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	//template<typename ParentType> struct Speed_In_Miles_Per_Hour {typedef Polaris_Component<Basic_Units::Implementations::Miles_Per_Hour_Implementation, NULLTYPE, Data_Object, ParentType> type;};

	//template<typename ParentType> struct Currency_In_Cents {typedef Polaris_Component<Basic_Units::Implementations::Cents_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	//template<typename ParentType> struct Currency_In_Dollars {typedef Polaris_Component<Basic_Units::Implementations::Dollars_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	//template<typename ParentType> struct Currency_In_Thousand_Dollars {typedef Polaris_Component<Basic_Units::Implementations::Thousand_Dollars_Implementation, NULLTYPE, Data_Object, ParentType> type;};
	//template<typename ParentType> struct Currency_In_Million_Dollars {typedef Polaris_Component<Basic_Units::Implementations::Million_Dollars_Implementation, NULLTYPE, Data_Object, ParentType> type;};

}


//=================================================================================================================
// SIMULATION TIMER DEFINITION - Used by agents as a wrapper for the global _iteration variable
typedef Basic_Units::Implementations::Time_In_Seconds<NULLTYPE,NULLTYPE> Basic_Time;
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