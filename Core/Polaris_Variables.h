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

namespace Basic_Units
{

namespace Time_Variables
{
	typedef double time_data_type;
	polaris_variable(Time_DRSeconds,time_data_type,Time_Type,DRSeconds_Type);
	polaris_variable(Time_Seconds,time_data_type,Time_Type,Seconds_Type);
	polaris_variable(Time_Minutes,time_data_type,Time_Type,Minutes_Type);
	polaris_variable(Time_Hours,time_data_type,Time_Type,Hours_Type);
	polaris_variable(Time_Days,time_data_type,Time_Type,Days_Type);
}

namespace Length_Variables
{
	typedef double length_data_type;
		
	polaris_variable(Inches, length_data_type, Length_Type, Inches_Type);
	polaris_variable(Feet, length_data_type, Length_Type, Feet_Type);
	polaris_variable(Miles, length_data_type, Length_Type, Miles_Type);
	polaris_variable(Centimeters, length_data_type, Length_Type, Centimeters_Type);
	polaris_variable(Meters, length_data_type, Length_Type, Meters_Type);
	polaris_variable(Kilometers, length_data_type, Length_Type, Kilometers_Type);
}

namespace Area_Variables
{
	typedef double length_data_type;
	polaris_variable(Square_Inches, length_data_type, Length_Type, Area_Type, Inches_Type);
	polaris_variable(Square_Feet, length_data_type, Length_Type, Area_Type, Feet_Type);
	polaris_variable(Square_Miles, length_data_type, Length_Type, Area_Type, Miles_Type);
	polaris_variable(Square_Centimeters, length_data_type, Length_Type, Area_Type, Centimeters_Type);
	polaris_variable(Square_Meters, length_data_type, Length_Type, Area_Type,  Meters_Type);
	polaris_variable(Square_Kilometers, length_data_type, Length_Type, Area_Type, Kilometers_Type);
}

namespace Volume_Variables
{
	typedef double length_data_type;
	polaris_variable(Cubic_Inches, length_data_type, Length_Type, Volume_Type, Inches_Type);
	polaris_variable(Cubic_Feet, length_data_type, Length_Type, Volume_Type, Feet_Type);
	polaris_variable(Cubic_Miles, length_data_type, Length_Type, Volume_Type, Miles_Type);
	polaris_variable(Cubic_Centimeters, length_data_type, Length_Type, Volume_Type, Centimeters_Type);
	polaris_variable(Cubic_Meters, length_data_type, Length_Type, Volume_Type, Meters_Type);
	polaris_variable(Cubic_Kilometers, length_data_type, Length_Type, Volume_Type, Kilometers_Type);
}

namespace Unit_Rate_Variables
{
	typedef double rate_data_type;
	polaris_variable(Unit_Per_DRSeconds,rate_data_type,Time_Type,DRSeconds_Type);
	polaris_variable(Unit_Per_Seconds,rate_data_type,Time_Type,Seconds_Type);
	polaris_variable(Unit_Per_Minutes,rate_data_type,Time_Type,Minutes_Type);
	polaris_variable(Unit_Per_Hours,rate_data_type,Time_Type,Hours_Type);
	polaris_variable(Unit_Per_Days,rate_data_type,Time_Type,Days_Type);
}

namespace Speed_Variables
{
	typedef double rate_data_type;
	polaris_variable(Feet_Per_Second,rate_data_type,Time_Type, Length_Type, Seconds_Type, Feet_Type);
	polaris_variable(Feet_Per_Minute,rate_data_type,Time_Type, Length_Type, Minutes_Type, Feet_Type);
	polaris_variable(Feet_Per_Hour,rate_data_type,Time_Type, Length_Type, Hours_Type, Feet_Type);

	polaris_variable(Meters_Per_Second,rate_data_type,Time_Type, Length_Type, Seconds_Type, Meters_Type);
	polaris_variable(Meters_Per_Minute,rate_data_type,Time_Type, Length_Type, Minutes_Type, Meters_Type);
	polaris_variable(Meters_Per_Hour,rate_data_type,Time_Type, Length_Type, Hours_Type, Meters_Type);

	polaris_variable(Miles_Per_Second,rate_data_type,Time_Type, Length_Type, Seconds_Type, Miles_Type);
	polaris_variable(Miles_Per_Minute,rate_data_type,Time_Type, Length_Type, Minutes_Type, Miles_Type);
	polaris_variable(Miles_Per_Hour,rate_data_type,Time_Type, Length_Type, Hours_Type, Miles_Type);

	polaris_variable(Kilometers_Per_Second,rate_data_type,Time_Type, Length_Type, Seconds_Type, Kilometers_Type);
	polaris_variable(Kilometers_Per_Minute,rate_data_type,Time_Type, Length_Type, Minutes_Type, Kilometers_Type);
	polaris_variable(Kilometers_Per_Hour,rate_data_type,Time_Type, Length_Type, Hours_Type, Kilometers_Type);
}

}

using namespace Basic_Units::Time_Variables;
using namespace Basic_Units::Length_Variables;
using namespace Basic_Units::Area_Variables;
using namespace Basic_Units::Volume_Variables;
using namespace Basic_Units::Unit_Rate_Variables;