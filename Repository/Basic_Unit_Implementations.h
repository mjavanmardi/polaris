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
		#pragma region Lenth Implementations
		implementation struct _Length {typedef true_type Length_tag; typedef true_type Inches_tag; member_data(Value_Type,Value,none,none);};
		implementation struct Length_Implementation : public _Length<MasterType, ParentType>, public Polaris_Component_Class<Length_Implementation,MasterType,Data_Object,ParentType>{};
		/*struct _Inches		{typedef true_type Inches_tag;};
		struct _Feet		{typedef true_type Feet_tag;};
		struct _Miles		{typedef true_type Miles_tag;};
		struct _Centimeters	{typedef true_type Centimeters_tag;};
		struct _Meters		{typedef true_type Meters_tag;};
		struct _Kilometers	{typedef true_type Kilometers_tag;};
		implementation struct Length_In_Inches		: _Measure_1D<MasterType,ParentType>, _Inches, public Polaris_Component_Class<Length_In_Inches,MasterType,Data_Object,ParentType>{};
		implementation struct Length_In_Feet		: _Measure_1D<MasterType,ParentType>, _Feet, public Polaris_Component_Class<Length_In_Feet,MasterType,Data_Object,ParentType>{};
		implementation struct Length_In_Miles		: _Measure_1D<MasterType,ParentType>, _Miles, public Polaris_Component_Class<Length_In_Miles,MasterType,Data_Object,ParentType>{};
		implementation struct Length_In_Centimeters : _Measure_1D<MasterType,ParentType>, _Centimeters, public Polaris_Component_Class<Length_In_Centimeters,MasterType,Data_Object,ParentType>{};
		implementation struct Length_In_Meters		: _Measure_1D<MasterType,ParentType>, _Meters, public Polaris_Component_Class<Length_In_Meters,MasterType,Data_Object,ParentType>{};
		implementation struct Length_In_Kilometers	: _Measure_1D<MasterType,ParentType>, _Kilometers, public Polaris_Component_Class<Length_In_Kilometers,MasterType,Data_Object,ParentType>{};*/
		#pragma endregion

		//================================================================================================
		/// Basic Area base clase.
		#pragma region Area Implementations
		implementation struct _Area : _Length<MasterType,ParentType>{typedef true_type Area_tag;};
		implementation struct Area_Implementation : _Area<MasterType,ParentType>, public Polaris_Component_Class<Area_Implementation,MasterType,Data_Object,ParentType>{};
		/*implementation struct Area_In_Square_Inches		: _Inches,		_Measure_2D<MasterType,ParentType>, public Polaris_Component_Class<Area_In_Square_Inches,MasterType,Data_Object,ParentType>{};
		implementation struct Area_In_Square_Feet		: _Feet,		_Measure_2D<MasterType,ParentType>, public Polaris_Component_Class<Area_In_Square_Feet,MasterType,Data_Object,ParentType>{};
		implementation struct Area_In_Square_Miles		: _Miles,		_Measure_2D<MasterType,ParentType>, public Polaris_Component_Class<Area_In_Square_Miles,MasterType,Data_Object,ParentType>{};
		implementation struct Area_In_Square_Centimeters: _Centimeters, _Measure_2D<MasterType,ParentType>, public Polaris_Component_Class<Area_In_Square_Centimeters,MasterType,Data_Object,ParentType>{};
		implementation struct Area_In_Square_Meters		: _Meters,		_Measure_2D<MasterType,ParentType>, public Polaris_Component_Class<Area_In_Square_Meters,MasterType,Data_Object,ParentType>{};
		implementation struct Area_In_Square_Kilometers : _Kilometers,	_Measure_2D<MasterType,ParentType>, public Polaris_Component_Class<Area_In_Square_Kilometers,MasterType,Data_Object,ParentType>{};*/
		#pragma endregion

		//================================================================================================
		/// Basic Volume base clase.
		#pragma region Volume Implementations
		implementation struct _Volume : _Length<MasterType,ParentType>{typedef true_type Volume_tag;};
		implementation struct Volume_Implementation	: _Volume<MasterType,ParentType>, public Polaris_Component_Class<Volume_Implementation,MasterType,Data_Object,ParentType> {};
		/*implementation struct Volume_In_Cubic_Inches		: _Inches,		_Measure_3D<MasterType,ParentType>,  public Polaris_Component_Class<Volume_In_Cubic_Inches,MasterType,Data_Object,ParentType> {};
		implementation struct Volume_In_Cubic_Feet			: _Feet,		_Measure_3D<MasterType,ParentType>, public Polaris_Component_Class<Volume_In_Cubic_Feet,MasterType,Data_Object,ParentType> {};
		implementation struct Volume_In_Cubic_Miles			: _Miles,		_Measure_3D<MasterType,ParentType>, public Polaris_Component_Class<Volume_In_Cubic_Miles,MasterType,Data_Object,ParentType> {};
		implementation struct Volume_In_Cubic_Centimeters	: _Centimeters, _Measure_3D<MasterType,ParentType>, public Polaris_Component_Class<Volume_In_Cubic_Centimeters,MasterType,Data_Object,ParentType> {};
		implementation struct Volume_In_Cubic_Meters		: _Meters,		_Measure_3D<MasterType,ParentType>, public Polaris_Component_Class<Volume_In_Cubic_Meters,MasterType,Data_Object,ParentType> {};
		implementation struct Volume_In_Cubic_Kilometers	: _Kilometers,	_Measure_3D<MasterType,ParentType>, public Polaris_Component_Class<Volume_In_Cubic_Kilometers,MasterType,Data_Object,ParentType> {};*/
		#pragma endregion

		//================================================================================================
		/// Basic Time base clase and Time Horizon Classes.
		#pragma region Time Implementations
		implementation struct _Time {typedef true_type Time_tag; typedef true_type Seconds_tag; member_data(Value_Type,Value,none,none);};
		implementation struct Time_Implementation : _Time<MasterType,ParentType>, public Polaris_Component_Class<Time_Implementation,MasterType,Data_Object,ParentType>{};
		/*struct _DRSeconds	{typedef true_type DRSeconds_tag;};
		struct _Seconds		{typedef true_type Seconds_tag;};
		struct _Minutes		{typedef true_type Minutes_tag;};
		struct _Hours		{typedef true_type Hours_tag;};
		struct _Days		{typedef true_type Days_tag;};
		implementation struct Time_In_DRSeconds : _DRSeconds, _Time<MasterType,ParentType>, public Polaris_Component_Class<Time_In_DRSeconds,MasterType,Data_Object,ParentType>{};
		implementation struct Time_In_Seconds	: _Seconds, _Time<MasterType,ParentType>,  public Polaris_Component_Class<Time_In_Seconds,MasterType,Data_Object,ParentType>{};
		implementation struct Time_In_Minutes	: _Minutes, _Time<MasterType,ParentType>,  public Polaris_Component_Class<Time_In_Minutes,MasterType,Data_Object,ParentType>{};
		implementation struct Time_In_Hours		: _Hours, _Time<MasterType,ParentType>,  public Polaris_Component_Class<Time_In_Hours,MasterType,Data_Object,ParentType>{};
		implementation struct Time_In_Days		: _Days, _Time<MasterType,ParentType>,  public Polaris_Component_Class<Time_In_Days,MasterType,Data_Object,ParentType>{};*/

		//================================================================================================
		/// Time Horizon base clase.
		/*implementation struct _Horizon : _Time<MasterType,ParentType> {typedef true_type Horizon_tag;};
		implementation struct Horizon_Impulsive		: _Minutes, _Horizon<MasterType,ParentType>, public Polaris_Component_Class<Horizon_Impulsive,MasterType,Data_Object,ParentType>	{typedef true_type Impulsive_tag;};
		implementation struct Horizon_Within_1_Hour : _Minutes, _Horizon<MasterType,ParentType>, public Polaris_Component_Class<Horizon_Within_1_Hour,MasterType,Data_Object,ParentType>{typedef true_type Within_1_Hour_tag;};
		implementation struct Horizon_Within_Hours	: _Minutes, _Horizon<MasterType,ParentType>, public Polaris_Component_Class<Horizon_Within_Hours,MasterType,Data_Object,ParentType>	{typedef true_type Within_Hours_tag;};
		implementation struct Horizon_Within_Days	: _Minutes, _Horizon<MasterType,ParentType>, public Polaris_Component_Class<Horizon_Within_Days,MasterType,Data_Object,ParentType>	{typedef true_type Within_Days_tag;};
		implementation struct Horizon_Same_Day		: _Minutes, _Horizon<MasterType,ParentType>, public Polaris_Component_Class<Horizon_Same_Day,MasterType,Data_Object,ParentType>		{typedef true_type Same_Day_tag;};
		implementation struct Horizon_Previous_Day	: _Minutes, _Horizon<MasterType,ParentType>, public Polaris_Component_Class<Horizon_Previous_Day,MasterType,Data_Object,ParentType>	{typedef true_type Previous_Day_tag;};
		implementation struct Horizon_Same_Week		: _Minutes, _Horizon<MasterType,ParentType>, public Polaris_Component_Class<Horizon_Same_Week,MasterType,Data_Object,ParentType>	{typedef true_type Same_Week_tag;};
		implementation struct Horizon_Same_Month	: _Minutes, _Horizon<MasterType,ParentType>, public Polaris_Component_Class<Horizon_Same_Month,MasterType,Data_Object,ParentType>	{typedef true_type Same_Month_tag;};
		implementation struct Horizon_Preplanned	: _Minutes, _Horizon<MasterType,ParentType>, public Polaris_Component_Class<Horizon_Preplanned,MasterType,Data_Object,ParentType>	{typedef true_type Preplanned_tag;};
		implementation struct Horizon_Routine		: _Minutes, _Horizon<MasterType,ParentType>, public Polaris_Component_Class<Horizon_Routine,MasterType,Data_Object,ParentType>		{typedef true_type Routine_tag;};*/
		#pragma endregion

		//================================================================================================
		/// Basic Rate and Speed base classes.
		#pragma region Rate Implementations
		implementation struct _Rate : _Time<MasterType,ParentType>{typedef true_type Rate_tag;};
		implementation struct Rate_Implementation : _Rate<MasterType,ParentType>, public Polaris_Component_Class<Rate_Implementation,MasterType,Data_Object,ParentType>{};
		/*implementation struct Units_Per_DRSecond: _DRSeconds,	_Rate<MasterType,ParentType>, public Polaris_Component_Class<Units_Per_DRSecond,MasterType,Data_Object,ParentType>{};
		implementation struct Units_Per_Second	: _Seconds,		_Rate<MasterType,ParentType>, public Polaris_Component_Class<Units_Per_Second,MasterType,Data_Object,ParentType>{};
		implementation struct Units_Per_Minute	: _Minutes,		_Rate<MasterType,ParentType>, public Polaris_Component_Class<Units_Per_Minute,MasterType,Data_Object,ParentType>{};
		implementation struct Units_Per_Hour	: _Hours,		_Rate<MasterType,ParentType>, public Polaris_Component_Class<Units_Per_Hour,MasterType,Data_Object,ParentType>{};
		implementation struct Units_Per_Day		: _Days,		_Rate<MasterType,ParentType>, public Polaris_Component_Class<Units_Per_Day,MasterType,Data_Object,ParentType>{};*/

		//================================================================================================
		/// Basic Rate and Speed base classes.
		implementation struct _Speed : _Time<MasterType,ParentType>{typedef true_type Speed_tag; typedef true_type Feet_tag;};
		implementation struct Speed_Implementation : _Speed<MasterType,ParentType>, public Polaris_Component_Class<Speed_Implementation,MasterType,Data_Object,ParentType>{};
		/*implementation struct Speed_In_Feet_Per_Second		: _Feet, _Seconds,		_Speed<MasterType,ParentType>, public Polaris_Component_Class<Speed_In_Feet_Per_Second,MasterType,Data_Object,ParentType>{};
		implementation struct Speed_In_Feet_Per_Minute		: _Feet, _Minutes,		_Speed<MasterType,ParentType>, public Polaris_Component_Class<Speed_In_Feet_Per_Minute,MasterType,Data_Object,ParentType>{};
		implementation struct Speed_In_Feet_Per_Hour		: _Feet, _Hours,		_Speed<MasterType,ParentType>, public Polaris_Component_Class<Speed_In_Feet_Per_Hour,MasterType,Data_Object,ParentType>{};
		implementation struct Speed_In_Meters_Per_Second	: _Meters, _Seconds,	_Speed<MasterType,ParentType>, public Polaris_Component_Class<Speed_In_Meters_Per_Second,MasterType,Data_Object,ParentType>{};
		implementation struct Speed_In_Meters_Per_Minute	: _Meters, _Minutes,	_Speed<MasterType,ParentType>, public Polaris_Component_Class<Speed_In_Meters_Per_Minute,MasterType,Data_Object,ParentType>{};
		implementation struct Speed_In_Meters_Per_Hour		: _Meters, _Hours,		_Speed<MasterType,ParentType>, public Polaris_Component_Class<Speed_In_Meters_Per_Hour,MasterType,Data_Object,ParentType>{};
		implementation struct Speed_In_Miles_Per_Second		: _Miles, _Seconds,		_Speed<MasterType,ParentType>, public Polaris_Component_Class<Speed_In_Miles_Per_Second,MasterType,Data_Object,ParentType>{};
		implementation struct Speed_In_Miles_Per_Minute		: _Miles, _Minutes,		_Speed<MasterType,ParentType>, public Polaris_Component_Class<Speed_In_Miles_Per_Minute,MasterType,Data_Object,ParentType>{};
		implementation struct Speed_In_Miles_Per_Hour		: _Miles, _Hours,		_Speed<MasterType,ParentType>, public Polaris_Component_Class<Speed_In_Miles_Per_Hour,MasterType,Data_Object,ParentType>{};
		implementation struct Speed_In_Kilometers_Per_Second: _Kilometers, _Seconds,_Speed<MasterType,ParentType>, public Polaris_Component_Class<Speed_In_Kilometers_Per_Second,MasterType,Data_Object,ParentType>{};
		implementation struct Speed_In_Kilometers_Per_Minute: _Kilometers, _Minutes,_Speed<MasterType,ParentType>, public Polaris_Component_Class<Speed_In_Kilometers_Per_Minute,MasterType,Data_Object,ParentType>{};
		implementation struct Speed_In_Kilometers_Per_Hour	: _Kilometers, _Hours,	_Speed<MasterType,ParentType>, public Polaris_Component_Class<Speed_In_Kilometers_Per_Hour,MasterType,Data_Object,ParentType>{};*/
		#pragma endregion


		//================================================================================================
		/// Basic Currency base clase.
		#pragma region Currency Implementations
		implementation struct _Currency {typedef true_type Currency_tag; typedef true_type Cents_tag; member_data(Value_Type,Value,none,none);};
		implementation struct Currency_Implementation : _Currency<MasterType,ParentType>, public Polaris_Component_Class<Currency_Implementation,MasterType,Data_Object,ParentType>{};
		/*struct _Cents			{typedef true_type Cents_tag;};
		struct _Dollars			{typedef true_type Dollars_tag;};
		struct _Thousand_Dollars{typedef true_type Thousand_Dollars_tag;};
		struct _Million_Dollars	{typedef true_type Million_Dollars_tag;};
		implementation struct Currency_In_Cents				: _Cents,			_Currency<MasterType,ParentType>, public Polaris_Component_Class<Currency_In_Cents,MasterType,Data_Object,ParentType>{};
		implementation struct Currency_In_Dollars			: _Dollars,			_Currency<MasterType,ParentType>, public Polaris_Component_Class<Currency_In_Dollars,MasterType,Data_Object,ParentType>{};
		implementation struct Currency_In_Thousand_Dollars  : _Thousand_Dollars,_Currency<MasterType,ParentType>, public Polaris_Component_Class<Currency_In_Thousand_Dollars,MasterType,Data_Object,ParentType>{};
		implementation struct Currency_In_Million_Dollars	: _Million_Dollars,	_Currency<MasterType,ParentType>, public Polaris_Component_Class<Currency_In_Million_Dollars,MasterType,Data_Object,ParentType>	{};
	*/	#pragma endregion
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
		return Basic_Units::Prototypes::Time_Prototype<Basic_Time>::Convert_Value<Target_Type<TargetType,Base_Time_Type>>((typename Base_Time_Type::ValueType)_iteration);
	}
	template<typename InputType, typename TargetType> TargetType Future_Time(InputType Additional_Time_Increment)
	{
		Simulation_Timestep_Increment current_time;
		current_time = (Simulation_Timestep_Increment)_iteration;
		Simulation_Timestep_Increment additional_time = Basic_Units::Prototypes::Time_Prototype<Basic_Time>::Convert_Value<Target_Type<Simulation_Timestep_Increment,InputType>>(Additional_Time_Increment);
		return Basic_Units::Prototypes::Time_Prototype<Basic_Time>::Convert_Value<Target_Type<TargetType,Simulation_Timestep_Increment>>(current_time + additional_time);
	}
};

_Simulation_Timer<Simulation_Timestep_Increment> Simulation_Time;