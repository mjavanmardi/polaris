#pragma once

#include "Activity_Simulator_Includes.h"


//---------------------------------------------------------
//	POP_PROPERTIES OBJECT CLASS (i.e. HH, PERSON, VEHICLE, FIRM, etc.)
//---------------------------------------------------------

namespace Person_Components
{
	
	namespace Prototypes
	{
		prototype struct Person_Properties ADD_DEBUG_INFO
		{
			tag_as_prototype;

			typedef Person_Properties<ComponentType> This_Type;
			
			// accessor to parent class
			accessor(Parent_Person, NONE, NONE);

			// basic properties
			accessor(ID, NONE, NONE);
			accessor(Weight, NONE, NONE);
			accessor(Index, NONE, NONE);
			accessor(Test_Index, NONE, NONE);
			//accessor(Characteristics, NONE, NONE);

			// Static Census ACS type properties
			accessor(Gender, NONE, NONE);
			accessor(Race, NONE, NONE);
			accessor(Age, NONE, NONE);
			accessor(Class_of_worker, NONE, NONE);
			accessor(Journey_To_Work_Travel_Time, NONE, NONE);
			accessor(Journey_To_Work_Vehicle_Occupancy, NONE, NONE);
			accessor(Journey_To_Work_Mode, NONE, NONE);
			accessor(Marital_Status, NONE, NONE);
			accessor(School_Enrollment, NONE, NONE);
			accessor(School_Grade_Level, NONE, NONE);
			accessor(Educational_Attainment, NONE, NONE);
			accessor(Work_Hours, NONE, NONE);
			accessor(Employment_Status, NONE, NONE);
			accessor(Employment_Industry, NONE, NONE);	
			accessor(Journey_To_Work_Arrival_Time, check(strip_modifiers(TargetType),Basic_Units::Concepts::Is_Time_Value), NONE);
			accessor(Income, NONE, NONE);

			accessor(Telecommute_Frequency, NONE, NONE);

			// Non-static properties
			accessor(work_location_id, NONE, NONE);
			accessor(school_location_id, NONE, NONE);

			local_check_template_method_name(Average_Activity_Frequency_exists,Average_Activity_Frequency);
			template<typename TargetType, typename ReturnType, typename CType> ReturnType Average_Activity_Frequency(TargetType act_type)
			{
				assert_check(ComponentType,Average_Activity_Frequency_exists, "Component has no Average_Activity_Frequency.");

				return this_component()->Average_Activity_Frequency<TargetType, ReturnType>(act_type);
			}

			template<typename TargetType, typename TimeType> void Average_Activity_Frequency(TargetType act_type, TimeType set_value)
			{
				assert_check(ComponentType,Average_Activity_Frequency_exists, "Component has no Average_Activity_Frequency.");

				this_component()->Average_Activity_Frequency<TargetType, TimeType>(act_type, set_value);
			}


			local_check_template_method_name(Average_Activity_Duration_exists,Average_Activity_Duration);
			template<typename TargetType, typename ReturnType> ReturnType Average_Activity_Duration(TargetType act_type)
			{
				assert_check(ComponentType,Average_Activity_Duration_exists, "Component has no Average_Activity_Duration.");

				return this_component()->Average_Activity_Duration<TargetType, ReturnType>(act_type);
			}

			template<typename TargetType, typename TimeType> void Average_Activity_Duration(TargetType act_type, TimeType set_value)
			{
				assert_check(ComponentType,Average_Activity_Duration_exists, "Component has no Average_Activity_Duration.");

				this_component()->Average_Activity_Duration<TargetType, TimeType>(act_type, set_value);
			}

			
			// Pass through methods
			local_check_template_method_name(Initialize_exists,Initialize);
			template<typename TargetType> void Initialize()
			{
				assert_check(ComponentType,Initialize_exists,"ComponentType does not have 'Initialize' method defined.");

				this_component()->Initialize<TargetType>();
			}
			template<typename TargetType> void Initialize(TargetType home_zone)
			{
				assert_check(ComponentType,Initialize_exists,"ComponentType does not have 'Initialize' method defined.");

				this_component()->Initialize<TargetType>(home_zone);
			}

			local_check_template_method_name(Set_Locations_exists, Set_Locations);
			template<typename TargetType> void Set_Locations()
			{
				assert_check(ComponentType, Set_Locations_exists, "ComponentType does not have 'Set_Locations' method defined.");

				this_component()->Set_Locations<TargetType>();
			}


			local_check_template_method_name(Is_Employed_exists,Is_Employed);
			template<typename TargetType> bool Is_Employed()
			{
				assert_check(ComponentType,Is_Employed_exists,"ComponentType does not have 'Is_Employed' method defined.");

				return this_component()->Is_Employed<TargetType>();
			}
			bool Is_Fulltime_Worker()
			{
				return this_component()->Is_Employed<NT>() && this_component()->Work_Hours<Time_Minutes>() >= 30;

			}
			bool Is_Parttime_Worker()
			{
				return this_component()->Is_Employed<NT>() && this_component()->Work_Hours<Time_Minutes>() < 30;
			}

			local_check_template_method_name(Is_Student_exists,Is_Student);
			template<typename TargetType> bool Is_Student()
			{
				assert_check(ComponentType,Is_Student_exists,"ComponentType does not have 'Is_Student' method defined.");

				return this_component()->Is_Student<TargetType>();
			}

			template<typename TargetType> TargetType Value_of_Travel_Time_Adjustment()
			{
				return this_component()->Value_of_Travel_Time_Adjustment<TargetType>();
			}
			
			local_check_template_method_name(Characteristics_exists,Characteristics);
			template<typename TargetType> void Characteristics(TargetType data, requires(TargetType, check_2(TargetType, std::vector<double>*, is_same)))
			{
				assert_check(ComponentType,Characteristics_exists,"ComponentType does not have 'Characteristics' method defined.");

				this_component()->Characteristics<TargetType>(data);
			}
			template<typename TargetType> void Characteristics(TargetType data, requires(TargetType, !check_2(TargetType, std::vector<double>*, is_same)))
			{
				assert_check_2(TargetType, std::vector<double>*, is_same, "TargetType must be specified as a pointer to a vector of doubles.");
			}

			template<typename TargetType> Types::EMPLOYMENT_INDUSTRY_BASE Employment_Industry_Base()
			{
				int i = this->Employment_Industry<Types::EMPLOYMENT_INDUSTRY>();

				if (i <= 290) return Types::EMPLOYMENT_INDUSTRY_BASE::IND_AGRICULTURE;
				else if (i <= 490) return Types::EMPLOYMENT_INDUSTRY_BASE::IND_EXTRACTION;
				else if (i <= 690) return Types::EMPLOYMENT_INDUSTRY_BASE::IND_UTILITIES;
				else if (i <= 770) return Types::EMPLOYMENT_INDUSTRY_BASE::IND_CONSTRUCTION;
				else if (i <= 3990) return Types::EMPLOYMENT_INDUSTRY_BASE::IND_MANUFACTURING;
				else if (i <= 4590) return Types::EMPLOYMENT_INDUSTRY_BASE::IND_WHOLESALE;
				else if (i <= 5790) return Types::EMPLOYMENT_INDUSTRY_BASE::IND_RETAIL;
				else if (i <= 6390) return Types::EMPLOYMENT_INDUSTRY_BASE::IND_TRANSPORTATION;
				else if (i <= 6780) return Types::EMPLOYMENT_INDUSTRY_BASE::IND_INFORMATION;
				else if (i <= 6990) return Types::EMPLOYMENT_INDUSTRY_BASE::IND_FINANCE;
				else if (i <= 7190) return Types::EMPLOYMENT_INDUSTRY_BASE::IND_REALESTATE;
				else if (i <= 7790) return Types::EMPLOYMENT_INDUSTRY_BASE::IND_PROFESSIONAL;
				else if (i <= 7890) return Types::EMPLOYMENT_INDUSTRY_BASE::IND_EDUCATION;
				else if (i <= 8290) return Types::EMPLOYMENT_INDUSTRY_BASE::IND_MEDICAL;
				else if (i <= 8470) return Types::EMPLOYMENT_INDUSTRY_BASE::IND_SOCIALWORK;
				else if (i <= 8690) return Types::EMPLOYMENT_INDUSTRY_BASE::IND_ENTERTAINMENT;
				else if (i <= 9290) return Types::EMPLOYMENT_INDUSTRY_BASE::IND_SERVICES;
				else if (i <= 9590) return Types::EMPLOYMENT_INDUSTRY_BASE::IND_ADMINISTRATION;
				else if (i <= 9870) return Types::EMPLOYMENT_INDUSTRY_BASE::IND_MILITARY;
				else if (i <= 9920) return Types::EMPLOYMENT_INDUSTRY_BASE::IND_UNEMPLOYED;
				else return Types::EMPLOYMENT_INDUSTRY_BASE::IND_NA;
			}
			template<typename TargetType> Types::EMPLOYMENT_INDUSTRY_SIMPLE Employment_Industry_Simple()
			{
				int i = this->Employment_Industry<Types::EMPLOYMENT_INDUSTRY>();

				if (i <= 770 || (i >= 6070 && i <= 6390)) return Types::EMPLOYMENT_INDUSTRY_SIMPLE::INDUSTRIAL;
				else if (i <= 3990) return Types::EMPLOYMENT_INDUSTRY_SIMPLE::MANUFACTURING;
				else if (i <= 5790) return Types::EMPLOYMENT_INDUSTRY_SIMPLE::RETAIL;
				else if (i <= 7790 || (i >= 8660 && i <= 9290)) return Types::EMPLOYMENT_INDUSTRY_SIMPLE::SERVICE;
				else if ((i >= 7860 && i <= 7890) || (i >= 9370 && i <= 9870)) return Types::EMPLOYMENT_INDUSTRY_SIMPLE::GOVERNMENT;
				else return Types::EMPLOYMENT_INDUSTRY_SIMPLE::OTHER;
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

using namespace Person_Components::Types;
using namespace Person_Components::Prototypes;
