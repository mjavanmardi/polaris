#pragma once
#include "Repository_Base_Includes.h"

namespace Basic_Units
{
	namespace Length_Namespace
	{
		namespace Types
		{
			typedef true_type Length_Type;
			typedef true_type Feet;
			typedef true_type Inches;
			typedef true_type Miles;
			typedef true_type Centimeters;
			typedef true_type Meters;
			typedef true_type Kilometers;
		}

		namespace Concepts
		{
			#pragma region //Length Concepts
			concept Length_in_Inches
			{
				begin_requirements_list(none);
				requires_typename_state(none, Inches, true_type,"Length is not in Inches");
				requires_typename_state(Inches, Length_Type, true_type,"Type is not a length type");
				end_requirements_list(Length_Type);
			};
			concept Length_in_Feet
			{
				begin_requirements_list(none);
				requires_typename_state(none, Feet, true_type,"Length is not in feet");
				requires_typename_state(Feet, Length_Type, true_type,"Type is not a length type");
				end_requirements_list(Length_Type);
			};
			concept Length_in_Miles
			{
				begin_requirements_list(none);
				requires_typename_state(none, Miles, true_type,"Length is not in Miles");
				requires_typename_state(Miles, Length_Type, true_type,"Type is not a length type");
				end_requirements_list(Length_Type);
			};
			concept Length_in_Centimeters
			{
				begin_requirements_list(none);
				requires_typename_state(none, Centimeters, true_type,"Length is not in Centimeters");
				requires_typename_state(Centimeters, Length_Type, true_type,"Type is not a length type");
				end_requirements_list(Length_Type);
			};
			concept Length_in_Meters
			{
				begin_requirements_list(none);
				requires_typename_state(none, Meters, true_type,"Length is not in Meters");
				requires_typename_state(Meters, Length_Type, true_type,"Type is not a length type");
				end_requirements_list(Length_Type);
			};
			concept Length_in_Kilometers
			{
				begin_requirements_list(none);
				requires_typename_state(none, Kilometers, true_type,"Length is not in Kilometers");
				requires_typename_state(Kilometers, Length_Type, true_type,"Type is not a length type");
				end_requirements_list(Length_Type);
			};
			#pragma endregion
		}

		namespace Interfaces
		{
			//------------------------------------------------------------------------------------------------------------------
			/// RENAME THE Inteface struct below.  This is the inteface to a POLARIS component
			//------------------------------------------------------------------------------------------------------------------
			template <typename ComponentType, typename CallerType=NULLTYPE>
			struct Length_Interface
			{
				facet TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Centimeters) && requires(ComponentType,Concepts::Length_in_Centimeters)))
				{
					return Data_Structures::Length_Centimeters(this->Length<Data_Structures::Length_Centimeters::ValueType>());
				};
				facet TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Centimeters) && requires(ComponentType,Concepts::Length_in_Centimeters)))
				{
					Data_Structures::Length_Centimeters val;
					val.Value = this->Length<Data_Structures::Length_Centimeters::ValueType>();
					return Data_Structures::Length_Centimeters(this->Length<Data_Structures::Length_Centimeters::ValueType>());
				};

				facet TargetType Length(call_requirements(!(
					(requires(TargetType,Concepts::Length_in_Centimeters) || requires(TargetType,Concepts::Length_in_Feet) || requires(TargetType,Concepts::Length_in_Inches) || 
					requires(TargetType,Concepts::Length_in_Kilometers) || requires(TargetType,Concepts::Length_in_Meters) || requires(TargetType,Concepts::Length_in_Miles)) &&
					(requires(ComponentType,Concepts::Length_in_Centimeters) || requires(ComponentType,Concepts::Length_in_Feet) || requires(ComponentType,Concepts::Length_in_Inches) || 
					requires(ComponentType,Concepts::Length_in_Kilometers) || requires(ComponentType,Concepts::Length_in_Meters) || requires(ComponentType,Concepts::Length_in_Miles))) ))
				{
					assert_requirements(ComponentType,Concepts::Length_in_Centimeters,"The specified ComponentType is not a valid length component.");
					assert_requirements(TargetType,Concepts::Length_in_Centimeters,"The specified TargetType is not a valid Length Data Structure.");
				}
			};
		}

		namespace Bases
		{
			template<typename MasterType>
			struct Length
			{
				// LENGTH - stored in feet
				typedef true_type Length_Type;
				member_data_basic(float,length);
			};
			template<typename MasterType>
			struct Length_in_inches : Length<MasterType>
			{
				typedef true_type Length_Inches;
			};
			template<typename MasterType>
			struct Length_in_feet : Length<MasterType>
			{
				typedef true_type Length_Feet;
			};
			template<typename MasterType>
			struct Length_in_miles : Length<MasterType>
			{
				typedef true_type Length_Miles;
			};
			template<typename MasterType>
			struct Length_in_centimeters : Length<MasterType>
			{
				typedef true_type Length_Centimeters;
			};
			template<typename MasterType>
			struct Length_in_meters : Length<MasterType>
			{
				typedef true_type Length_Meters;
			};
			template<typename MasterType>
			struct Length_in_kilometers : Length<MasterType>
			{
				typedef true_type Length_Kilometers;
			};
		}

		namespace Components
		{
			template<typename MasterType>
			struct Length_in_inches
			{
				typedef Polaris_Component<Bases::Length_in_inches, MasterType> type;
			};
			template<typename MasterType>
			struct Length_in_feet
			{
				typedef Polaris_Component<Bases::Length_in_feet,MasterType> type;
			};
			template<typename MasterType>
			struct Length_in_miles
			{
				typedef Polaris_Component<Bases::Length_in_miles,MasterType> type;
			};
			template<typename MasterType>
			struct Length_in_centimeters
			{
				typedef Polaris_Component<Bases::Length_in_centimeters,MasterType> type;
			};
			template<typename MasterType>
			struct Length_in_meters
			{
				typedef Polaris_Component<Bases::Length_in_meters,MasterType> type;
			};
			template<typename MasterType>
			struct Length_in_kilometers
			{
				typedef Polaris_Component<Bases::Length_in_kilometers,MasterType> type;
			};
		}

		namespace Data_Structures
		{
			typedef float length_data_type;
			Basic_Data_Struct(Length_Inches, length_data_type, Length_Type, Inches);
			Basic_Data_Struct(Length_Feet, length_data_type, Length_Type, Feet);
			Basic_Data_Struct(Length_Miles, length_data_type, Length_Type, Miles);
			Basic_Data_Struct(Length_Centimeters, length_data_type, Length_Type, Centimeters);
			Basic_Data_Struct(Length_Meters, length_data_type, Length_Type, Meters);
			Basic_Data_Struct(Length_Kilometers, length_data_type, Length_Type, Kilometers);
		}

	}
}