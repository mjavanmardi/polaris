#pragma once


///============================================================================
/// polaris_variable / member_component_feature – PoDs with type tags
///============================================================================

#define polaris_variable(VARIABLE_NAME, VARIABLE_VALUE_TYPE, TYPE_TRAIT_TAGS,...) \
struct VARIABLE_NAME \
{													\
	typedef true_type TYPE_TRAIT_TAGS, __VA_ARGS__; \
	typedef VARIABLE_VALUE_TYPE ValueType; \
	typedef true_type polaris_variable_type;\
	VARIABLE_NAME(VARIABLE_VALUE_TYPE val): Value(val){} \
	VARIABLE_NAME(): Value(){} \
	VARIABLE_VALUE_TYPE Value; \
	operator VARIABLE_VALUE_TYPE(){return Value;}; \
	VARIABLE_NAME& operator=(VARIABLE_VALUE_TYPE& obj){Value = obj; return *this;}  \
	bool operator==(VARIABLE_VALUE_TYPE& obj){return Value == obj; }  \
	bool operator!=(VARIABLE_VALUE_TYPE& obj){return Value != obj; }  \
	bool operator>(VARIABLE_VALUE_TYPE& obj){return Value > obj; }  \
	bool operator>=(VARIABLE_VALUE_TYPE& obj){return Value >= obj; }  \
	bool operator<(VARIABLE_VALUE_TYPE& obj){return Value < obj; }  \
	bool operator<=(VARIABLE_VALUE_TYPE& obj){return Value <= obj; }  \
	VARIABLE_NAME& operator+(VARIABLE_VALUE_TYPE& obj){Value = Value + obj; return *this;}  \
	VARIABLE_NAME& operator-(VARIABLE_VALUE_TYPE& obj){Value = Value - obj; return *this;}  \
	VARIABLE_NAME& operator*(VARIABLE_VALUE_TYPE& obj){Value = Value * obj; return *this;}  \
	VARIABLE_NAME& operator/(VARIABLE_VALUE_TYPE& obj){Value = Value / obj; return *this;}  \
	VARIABLE_NAME& operator++(){Value = Value + 1; return *this;}  \
	VARIABLE_NAME& operator--(){Value = Value - 1; return *this;}  \
};

namespace polaris
{
namespace Basic_Units
{

namespace Time_Variables
{
	polaris_variable(Time_DRSeconds,double,Time_tag,DRSeconds_tag);
	polaris_variable(Time_Seconds,double,Time_tag,Seconds_tag);
	polaris_variable(Time_Minutes,double,Time_tag,Minutes_tag);
	polaris_variable(Time_Hours,double,Time_tag,Hours_tag);
	polaris_variable(Time_Days,double,Time_tag,Days_tag);

	polaris_variable(Time_DRSeconds_Float,float,Time_tag,DRSeconds_tag);
	polaris_variable(Time_Seconds_Float,float,Time_tag,Seconds_tag);
	polaris_variable(Time_Minutes_Float,float,Time_tag,Minutes_tag);
	polaris_variable(Time_Hours_Float,float,Time_tag,Hours_tag);
	polaris_variable(Time_Days_Float,float,Time_tag,Days_tag);

	polaris_variable(Time_DRSeconds_Short,short,Time_tag,DRSeconds_tag);
	polaris_variable(Time_Seconds_Short,short,Time_tag,Seconds_tag);
	polaris_variable(Time_Minutes_Short,short,Time_tag,Minutes_tag);
	polaris_variable(Time_Hours_Short,short,Time_tag,Hours_tag);
	polaris_variable(Time_Days_Short,short,Time_tag,Days_tag);
}

namespace Length_Variables
{
	typedef double length_data_type;
		
	polaris_variable(Inches, length_data_type, Length_tag, Inches_tag);
	polaris_variable(Feet, length_data_type, Length_tag, Feet_tag);
	polaris_variable(Miles, length_data_type, Length_tag, Miles_tag);
	polaris_variable(Centimeters, length_data_type, Length_tag, Centimeters_tag);
	polaris_variable(Meters, length_data_type, Length_tag, Meters_tag);
	polaris_variable(Kilometers, length_data_type, Length_tag, Kilometers_tag);
}

namespace Area_Variables
{
	typedef double length_data_type;
	polaris_variable(Square_Inches, length_data_type, Length_tag, Area_tag, Inches_tag);
	polaris_variable(Square_Feet, length_data_type, Length_tag, Area_tag, Feet_tag);
	polaris_variable(Square_Miles, length_data_type, Length_tag, Area_tag, Miles_tag);
	polaris_variable(Square_Centimeters, length_data_type, Length_tag, Area_tag, Centimeters_tag);
	polaris_variable(Square_Meters, length_data_type, Length_tag, Area_tag, Meters_tag);
	polaris_variable(Square_Kilometers, length_data_type, Length_tag, Area_tag, Kilometers_tag);
}

namespace Volume_Variables
{
	typedef double length_data_type;
	polaris_variable(Cubic_Inches, length_data_type, Length_tag, Volume_tag, Inches_tag);
	polaris_variable(Cubic_Feet, length_data_type, Length_tag, Volume_tag, Feet_tag);
	polaris_variable(Cubic_Miles, length_data_type, Length_tag, Volume_tag, Miles_tag);
	polaris_variable(Cubic_Centimeters, length_data_type, Length_tag, Volume_tag, Centimeters_tag);
	polaris_variable(Cubic_Meters, length_data_type, Length_tag, Volume_tag, Meters_tag);
	polaris_variable(Cubic_Kilometers, length_data_type, Length_tag, Volume_tag, Kilometers_tag);
}

namespace Unit_Rate_Variables
{
	typedef double rate_data_type;
	polaris_variable(Units_Per_DRSecond,rate_data_type,Time_tag,DRSeconds_tag);
	polaris_variable(Units_Per_Second,rate_data_type,Time_tag,Seconds_tag);
	polaris_variable(Units_Per_Minute,rate_data_type,Time_tag,Minutes_tag);
	polaris_variable(Units_Per_Hour,rate_data_type,Time_tag,Hours_tag);
	polaris_variable(Units_Per_Day,rate_data_type,Time_tag,Days_tag);
}

namespace Speed_Variables
{
	typedef double rate_data_type;
	polaris_variable(Feet_Per_Second,rate_data_type, Speed_tag, Time_tag, Length_tag, Seconds_tag, Feet_tag);
	polaris_variable(Feet_Per_Minute,rate_data_type, Speed_tag, Time_tag, Length_tag, Minutes_tag, Feet_tag);
	polaris_variable(Feet_Per_Hour,rate_data_type, Speed_tag, Time_tag, Length_tag, Hours_tag, Feet_tag);

	polaris_variable(Meters_Per_Second,rate_data_type, Speed_tag, Time_tag, Length_tag, Seconds_tag, Meters_tag);
	polaris_variable(Meters_Per_Minute,rate_data_type, Speed_tag, Time_tag, Length_tag, Minutes_tag, Meters_tag);
	polaris_variable(Meters_Per_Hour,rate_data_type, Speed_tag, Time_tag, Length_tag, Hours_tag, Meters_tag);

	polaris_variable(Miles_Per_Second,rate_data_type, Speed_tag, Time_tag, Length_tag, Seconds_tag, Miles_tag);
	polaris_variable(Miles_Per_Minute,rate_data_type, Speed_tag, Time_tag, Length_tag, Minutes_tag, Miles_tag);
	polaris_variable(Miles_Per_Hour,rate_data_type, Speed_tag, Time_tag, Length_tag, Hours_tag, Miles_tag);

	polaris_variable(Kilometers_Per_Second,rate_data_type, Speed_tag, Time_tag, Length_tag, Seconds_tag, Kilometers_tag);
	polaris_variable(Kilometers_Per_Minute,rate_data_type, Speed_tag, Time_tag, Length_tag, Minutes_tag, Kilometers_tag);
	polaris_variable(Kilometers_Per_Hour,rate_data_type, Speed_tag, Time_tag, Length_tag, Hours_tag, Kilometers_tag);
}

namespace Acceleration_Variables
{
	typedef double rate_data_type;
	polaris_variable(Feet_Per_Second_Squared,rate_data_type, Acceleration_tag, Speed_tag, Time_tag, Length_tag, Seconds_tag, Feet_tag);
	polaris_variable(Feet_Per_Minute_Squared,rate_data_type, Acceleration_tag, Speed_tag, Time_tag, Length_tag, Minutes_tag, Feet_tag);
	polaris_variable(Feet_Per_Hour_Squared,rate_data_type, Acceleration_tag, Speed_tag, Time_tag, Length_tag, Hours_tag, Feet_tag);

	polaris_variable(Meters_Per_Second_Squared,rate_data_type, Acceleration_tag, Speed_tag, Time_tag, Length_tag, Seconds_tag, Meters_tag);
	polaris_variable(Meters_Per_Minute_Squared,rate_data_type, Acceleration_tag, Speed_tag, Time_tag, Length_tag, Minutes_tag, Meters_tag);
	polaris_variable(Meters_Per_Hour_Squared,rate_data_type, Acceleration_tag, Speed_tag, Time_tag, Length_tag, Hours_tag, Meters_tag);

	polaris_variable(Miles_Per_Second_Squared,rate_data_type, Acceleration_tag, Speed_tag, Time_tag, Length_tag, Seconds_tag, Miles_tag);
	polaris_variable(Miles_Per_Minute_Squared,rate_data_type, Acceleration_tag, Speed_tag, Time_tag, Length_tag, Minutes_tag, Miles_tag);
	polaris_variable(Miles_Per_Hour_Squared,rate_data_type, Acceleration_tag, Speed_tag, Time_tag, Length_tag, Hours_tag, Miles_tag);

	polaris_variable(Kilometers_Per_Second_Squared,rate_data_type, Acceleration_tag, Speed_tag, Time_tag, Length_tag, Seconds_tag, Kilometers_tag);
	polaris_variable(Kilometers_Per_Minute_Squared,rate_data_type, Acceleration_tag, Speed_tag, Time_tag, Length_tag, Minutes_tag, Kilometers_tag);
	polaris_variable(Kilometers_Per_Hour_Squared,rate_data_type, Acceleration_tag, Speed_tag, Time_tag, Length_tag, Hours_tag, Kilometers_tag);
}

namespace Currency_Variables
{
	typedef double data_type;
	polaris_variable(Cents,data_type, Currency_tag, Cents_tag);
	polaris_variable(Dollars,data_type, Currency_tag, Dollars_tag);
	polaris_variable(Thousand_Dollars,data_type, Currency_tag, Thousand_Dollars_tag);
	polaris_variable(Million_Dollars,data_type,Currency_tag, Million_Dollars_tag);
}

}
using namespace Basic_Units::Time_Variables;
using namespace Basic_Units::Length_Variables;
using namespace Basic_Units::Area_Variables;
using namespace Basic_Units::Volume_Variables;
using namespace Basic_Units::Unit_Rate_Variables;
using namespace Basic_Units::Speed_Variables;
using namespace Basic_Units::Currency_Variables;
using namespace Basic_Units::Acceleration_Variables;
}

