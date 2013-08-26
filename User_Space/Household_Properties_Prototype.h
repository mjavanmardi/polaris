#pragma once

#include "User_Space_Includes.h"
//#include "Person_Prototype.h"
//#include "PopSyn_Prototype.h"

//---------------------------------------------------------
//	POP_PROPERTIES OBJECT CLASS (i.e. HH, PERSON, VEHICLE, FIRM, etc.)
//---------------------------------------------------------

namespace Household_Components
{
	namespace Types
	{
		// VARIOUS CENSUS ACS FILE CODES (NAME IN FILE given in comments)

		// HOUSEHOLD TYPE
		enum HHTYPE
		{
			OWN_FAMILY_MARRIED=1,
			OWN_FAMILY_OTHER,
			OWN_MALE_SINGLE,
			OWN_MALE_OTHER,
			OWN_FEMALE_SINGLE,
			OWN_FEMALE_OTHER,
			RENT_FAMILY_MARRIED,
			RENT_FAMILY_OTHER,
			RENT_MALE_SINGLE,
			RENT_MALE_OTHER,
			RENT_FEMALE_SINGLE,
			RENT_FEMALE_OTHER
		};

	}

	namespace Concepts
	{
		concept struct Is_Census_ACS_Compliant
		{
			// component versions
			check_typed_member(has_gender,Gender, Types::HHTYPE);
			check_getter(has_race,Race);
			check_typed_member(has_employment_status,Employment_Status, Person_Components::Types::EMPLOYMENT_STATUS);

			check_getter(has_work_travel_time,Journey_To_Work_Travel_Time);
			check_typed_member(has_work_travel_mode,Journey_To_Work_Mode, Person_Components::Types::JOURNEY_TO_WORK_MODE);
			check_getter(has_work_arrival_time,Journey_To_Work_Arrival_Time);

			// prototype versions
			check_typed_member(has_gender_prototype,Component_Type::Gender, Person_Components::Types::GENDER);
			check_getter(has_race_prototype,Component_Type::Race);
			check_typed_member(has_employment_status_prototype,Component_Type::Employment_Status, Person_Components::Types::EMPLOYMENT_STATUS);

			check_getter(has_work_travel_time_prototype,Component_Type::Journey_To_Work_Travel_Time);
			check_typed_member(has_work_travel_mode_prototype,Component_Type::Journey_To_Work_Mode, Person_Components::Types::JOURNEY_TO_WORK_MODE);
			check_getter(has_work_arrival_time_prototype,Component_Type::Journey_To_Work_Arrival_Time);

			// concept checks
			define_sub_check(has_demographics, (has_gender && has_race && has_employment_status) || (has_gender_prototype && has_race_prototype && has_employment_status_prototype));
			define_sub_check(has_journey_to_work_info, (has_work_travel_time && has_work_travel_mode && has_work_arrival_time) || (has_work_travel_time_prototype && has_work_travel_mode_prototype && has_work_arrival_time_prototype));
			define_default_check(has_demographics && has_journey_to_work_info);
		};
	}
	
	namespace Prototypes
	{
		prototype struct Household_Properties ADD_DEBUG_INFO
		{
			tag_as_prototype;

			typedef Household_Properties<ComponentType, CallerType> This_Type;
			
			// accessor to parent class
			feature_accessor(Parent_Household,none,none);

			// basic properties
			feature_accessor(ID,check(ReturnValueType,is_arithmetic),check(SetValueType,is_arithmetic));
			feature_accessor(Weight,check(ReturnValueType,is_arithmetic),check(SetValueType,is_arithmetic));
			feature_accessor(Index,check(ReturnValueType,is_arithmetic),check(SetValueType,is_arithmetic));
			//feature_accessor(Characteristics,none,none);

			// Static Census ACS type properties
			feature_accessor(Household_type,none,none);
			feature_accessor(Household_size,none,none);
			feature_accessor(Number_of_workers,none,none);
			feature_accessor(Number_of_vehicles,none,none);
			feature_accessor(Income,none,none);

			feature_accessor(Persons_Container,none,none);

			// Non-static properties
			feature_accessor(home_location_id,none,none);
			
			// Pass through methods
			feature_method_void(Initialize,	check_2(ReturnValueType,void,is_same));
			feature_method_1_arg(Initialize, home_zone, check_2(typename TargetType::ReturnType,void,is_same));
			feature_method_1_arg(Characteristics, data, check_2(typename TargetType::ParamType, vector<double>*, is_same) && check_2(typename TargetType::ReturnType, void, is_same) );

			feature_prototype void Normalize_Weight(TargetType normalization_factor, requires(check(TargetType,is_arithmetic)))
			{
				this->Weight<TargetType>(this->Weight<TargetType>()/normalization_factor);
			}
			feature_prototype void Normalize_Weight(TargetType normalization_factor, requires(!check(TargetType,is_arithmetic)))
			{
				assert_check(TargetType,is_arithmetic,"The specified TargetType must be arithmetic to use this function");
			}
		};
	}
}

using namespace Household_Components::Types;
using namespace Household_Components::Prototypes;