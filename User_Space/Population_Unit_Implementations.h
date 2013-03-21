#pragma once

#include "Population_Unit_Prototype.h"

namespace PopSyn
{
	namespace Implementations
	{
		implementation struct ADAPTS_Population_Unit_Implementation : public Polaris_Component<APPEND_CHILD(ADAPTS_Population_Unit_Implementation), MasterType, Data_Object, ParentType>
		{
			member_data(double,ID,none,none);
			member_data(double,Weight,none,none);
			member_data(uint,Index,none,none);				 //index into the joint-distribution matrix of the region (convert using region.get_index())
			member_container(vector<double>, Characteristics,none,none);

			member_data(Types::GENDER, Gender,check(ReturnValueType, is_integral),check(SetValueType,is_integral));
			member_data(Types::RACE, Race, check(ReturnValueType, is_integral),check(SetValueType,is_integral));
			member_data(int, Age, check(ReturnValueType, is_integral),check(SetValueType,is_integral));
			member_data(Types::CLASS_OF_WORKER, Class_of_worker,check(ReturnValueType, is_integral),check(SetValueType,is_integral));

			//member_data_component(Basic_Units::Implementations::Time_Implementation<NT>, _Journey_To_Work_Travel_Time,none,none);
			//member_component_feature(Journey_To_Work_Travel_Time,_Journey_To_Work_Travel_Time,Value,
			member_data(int, Journey_To_Work_Vehicle_Occupancy,none,none);
			member_data(int, Journey_To_Work_Mode,none,none);
			member_data(int, Marital_Status,none,none);
			member_data(int, School_Enrollment,none,none);
			member_data(int, School_Grade_Level,none,none);
			member_data(int, Educational_Attainment,none,none);
			member_data(int, Work_Hours,none,none);
			member_data(int, Employment_Status,none,none);
			member_data(int, Employment_Industry,none,none);	
			member_data(int, Journey_To_Work_Arrival_Time,none,none);
			member_data(int, Journey_To_Work_Departure_Time,none,none);
			member_data(int, Income,none,none);

			feature_implementation void Test()
			{

			}
		};
	}
}