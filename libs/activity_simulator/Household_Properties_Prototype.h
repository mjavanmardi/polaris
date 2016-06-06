#pragma once

#include "Activity_Simulator_Includes.h"
#include "Person_Properties_Prototype.h"

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
			check_data_member_type(has_gender,Gender, Types::HHTYPE);
			check_accessor_name(has_race,Race);
			check_data_member_type(has_employment_status,Employment_Status, Person_Components::Types::EMPLOYMENT_STATUS);

			check_accessor_name(has_work_travel_time,Journey_To_Work_Travel_Time);
			check_data_member_type(has_work_travel_mode,Journey_To_Work_Mode, Person_Components::Types::JOURNEY_TO_WORK_MODE);
			check_accessor_name(has_work_arrival_time,Journey_To_Work_Arrival_Time);

			// prototype versions
			check_data_member_type(has_gender_prototype,Component_Type::Gender, Person_Components::Types::GENDER);
			check_component_accessor_name(has_race_prototype,Race);
			check_data_member_type(has_employment_status_prototype,Component_Type::Employment_Status, Person_Components::Types::EMPLOYMENT_STATUS);

			check_component_accessor_name(has_work_travel_time_prototype,Journey_To_Work_Travel_Time);
			check_data_member_type(has_work_travel_mode_prototype,Component_Type::Journey_To_Work_Mode, Person_Components::Types::JOURNEY_TO_WORK_MODE);
			check_component_accessor_name(has_work_arrival_time_prototype,Journey_To_Work_Arrival_Time);

			// concept checks
			define_sub_check(has_demographics, (has_gender && has_race && has_employment_status) || (has_gender_prototype && has_race_prototype && has_employment_status_prototype));
			define_sub_check(has_journey_to_work_info, (has_work_travel_time && has_work_travel_mode && has_work_arrival_time) || (has_work_travel_time_prototype && has_work_travel_mode_prototype && has_work_arrival_time_prototype));
			define_default_check(has_demographics && has_journey_to_work_info);
		};

		concept struct Initialize_exists
		{
			check_method_name(has_Initialize, Initialize);
			define_default_check(has_Initialize);
		};
		concept struct Characteristics_exists
		{
			check_method_name(has_Characteristics, Characteristics);
			define_default_check(has_Characteristics);
		};
	}
	
	namespace Prototypes
	{
		prototype struct Household_Properties ADD_DEBUG_INFO
		{
			tag_as_prototype;

			typedef Household_Properties<ComponentType> This_Type;
			
			// accessor to parent class
			accessor(Parent_Household, NONE, NONE);

			// basic properties
			accessor(ID, NONE, NONE);
			accessor(Weight, NONE, NONE);
			accessor(Index, NONE, NONE);
			accessor(Test_Index, NONE, NONE);
			//accessor(Characteristics, NONE, NONE);

			// Static Census ACS type properties
			accessor(Household_type, NONE, NONE);
			accessor(Household_size, NONE, NONE);
			accessor(Number_of_workers, NONE, NONE);
			accessor(Number_of_vehicles, NONE, NONE);
			accessor(Income, NONE, NONE);

			accessor(Persons_Container, NONE, NONE);

			// Non-static properties
			accessor(home_location_id, NONE, NONE);
			
			// Pass through methods
			local_check_template_method_name(Initialize_exists,Initialize);
			template<typename TargetType> void Initialize(/*requires(TargetType, check(ComponentType,Initialize_exists))*/)
			{
				assert_check(ComponentType,Initialize_exists,"No initializer defined for this component.");

				this_component()->Initialize<TargetType>();
			}
			//template<typename TargetType> void Initialize(requires(TargetType, !check(ComponentType,Initialize_exists)))
			//{
			//	assert_check(ComponentType,Initialize_exists,"No initializer defined for this component.");
			//}

			template<typename TargetType> void Initialize(TargetType home_zone/*, requires(TargetType, check(ComponentType,Initialize_exists))*/)
			{
				assert_check(ComponentType,Initialize_exists,"No initializer defined for this component.");

				this_component()->Initialize<TargetType>(home_zone);
			}
			//template<typename TargetType> void Initialize(TargetType home_zone, requires(TargetType, !check(ComponentType,Initialize_exists)))
			//{
			//	assert_check(ComponentType,Initialize_exists,"No initializer defined for this component.");
			//}

			local_check_template_method_name(Characteristics_exists,Characteristics);
			template<typename TargetType> void Characteristics(TargetType data, requires(TargetType, /*check(ComponentType, Characteristics_exists) && */check_2(TargetType, std::vector<double>*, is_same)))
			{
				assert_check(ComponentType, Characteristics_exists, "No 'Characteristics' data member defined in component.");

				this_component()->Characteristics<TargetType>(data);
			}
			template<typename TargetType> void Characteristics(TargetType data, requires(TargetType, /*!check(ComponentType, Characteristics_exists) || */!check_2(TargetType, std::vector<double>*, is_same)))
			{
				/*assert_check(ComponentType, Characteristics_exists, "No 'Characteristics' data member defined in component.");*/
				assert_check(TargetType, Characteristics_exists, "'data' parameter must be passed as a pointer to a vector of doubles.");
			}

			template<typename TargetType> void Normalize_Weight(TargetType normalization_factor, requires(TargetType,check(strip_modifiers(TargetType),is_arithmetic)))
			{
				this->Weight<TargetType>(this->Weight<TargetType>()/normalization_factor);
			}
			template<typename TargetType> void Normalize_Weight(TargetType normalization_factor, requires(TargetType,!check(strip_modifiers(TargetType),is_arithmetic)))
			{
				assert_check(strip_modifiers(TargetType),is_arithmetic,"The specified TargetType must be arithmetic to use this function");
			}
		};
	}
}

using namespace Household_Components::Types;
using namespace Household_Components::Prototypes;
