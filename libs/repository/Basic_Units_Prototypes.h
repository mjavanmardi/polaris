#pragma once
#include "Repository_Includes.h"

template<typename TargetType, typename InputType>
TargetType Round(InputType value, requires(TargetType,check(strip_modifiers(TargetType), is_integral) && check(InputType,is_arithmetic) && (check(InputType,!is_integral))))
{
	InputType remain = value - (InputType)((TargetType)value);
	if (remain >= 0.5) return (TargetType)(value - remain + 1);
	else return (TargetType)(value - remain);
}
template<typename TargetType, typename InputType>
TargetType Round(InputType value, requires(TargetType,check(strip_modifiers(TargetType), !is_integral) || check(InputType,!is_arithmetic) || (check(InputType,is_integral))))
{
	assert_check(strip_modifiers(TargetType),is_integral,"ReturnType must be integral");
	assert_check(InputType,is_arithmetic,"InputType must be arithmetic");
	assert_check(InputType,!is_integral,"InputType must be floating point type");
}

namespace polaris
{
namespace Basic_Units
{
	//==========================================================================
	// Define the arithmetic value type to use in basic unit calculations here
	typedef float Value_Type;
	typedef float Time_Value_Type;
	//==========================================================================

	namespace Types
	{
		namespace Length_tags
		{
			typedef true_type Length_tag;
			typedef true_type Feet_tag;
			typedef true_type Inches_tag;
			typedef true_type Miles_tag;
			typedef true_type Centimeters_tag;
			typedef true_type Meters_tag;
			typedef true_type Kilometers_tag;
			
		}

		typedef true_type Area_tag;
		typedef true_type Volume_tag;
		typedef true_type Rate_tag;
		typedef true_type Speed_tag;

		namespace Time_tags
		{
			typedef true_type Time_tag;
			typedef true_type Days_tag;
			typedef true_type Hours_tag;
			typedef true_type Minutes_tag;
			typedef true_type Seconds_tag;
			typedef true_type DRSeconds_tag; 
		}
	}

	namespace Concepts
	{
		#pragma region Length Concepts
		concept struct Is_Length_Value
		{
			check_typedef_type(Is_Length,Length_tag,true_type);
			check_typedef_type(Is_Inches,Inches_tag,true_type);
			check_typedef_type(Is_Feet,Feet_tag,true_type);
			check_typedef_type(Is_Miles,Miles_tag,true_type);
			check_typedef_type(Is_Centimeters,Centimeters_tag,true_type);
			check_typedef_type(Is_Meters,Meters_tag,true_type);
			check_typedef_type(Is_Kilometers,Kilometers_tag,true_type);
			define_default_check(Is_Length && (Is_Inches || Is_Feet || Is_Miles || Is_Centimeters || Is_Meters || Is_Kilometers));
		};

		/// Check if a type meets the requirements of being a basic 'Area' component
		concept struct Is_Length_Component
		{
			check_concept(Is_Component,Is_Polaris_Component, T, V);
			check_concept(Is_Length, Is_Length_Value, T, V);
			define_default_check(Is_Component && Is_Length);
		};
		concept struct Is_Length_Prototype
		{
			check_concept(Is_Prototype,Is_Prototype, T, V);
			check_typedef_type(Is_Length, Component_Type::Length_tag, true_type);
			define_default_check(Is_Prototype && Is_Length);
		};
		#pragma endregion

		#pragma region Area Concepts
		concept struct Is_Area_Value
		{
			check_typedef_type(Is_Area, Area_tag, true_type);
			check_typedef_type(Is_Length, Length_tag, true_type);
			define_default_check(Is_Area && Is_Length);
		};
		/// Check if a type meets the requirements of being a basic 'Area' component
		concept struct Is_Area_Component
		{
			check_concept(Is_Component,Is_Polaris_Component, T, V);
			check_concept(Is_Area, Is_Area_Value, T, V);
			define_default_check(Is_Component && Is_Area);
		};
		concept struct Is_Area_Prototype
		{
			check_concept(Is_Prototype,Is_Prototype, T, V);
			check_typedef_type(Is_Area, Component_Type::Area_tag, true_type);
			define_default_check(Is_Area && Is_Prototype);
		};
		#pragma endregion

		#pragma region Volume Concepts
		/// Check if a type meets the requirements of being a basic 'volume' value
		concept struct Is_Volume_Value
		{
			check_typedef_type(Is_Volume, Volume_tag, true_type);
			check_typedef_type(Is_Length, Length_tag, true_type);
			define_default_check(Is_Volume && Is_Length);
		};
		/// Check if a type meets the requirements of being a basic 'Time' component
		concept struct Is_Volume_Component
		{
			check_concept(Is_Component,Is_Polaris_Component, T, V);
			check_concept(Is_Volume,Is_Volume_Value, T, V);
			define_default_check(Is_Component && Is_Volume);
		};
		concept struct Is_Volume_Prototype
		{
			check_concept(Is_Prototype,Is_Prototype, T, V);
			check_typedef_type(Is_Volume, Component_Type::Volume_tag, true_type);
			define_default_check(Is_Volume && Is_Prototype);
		};
		#pragma endregion

		#pragma region Time Concepts
		/// Check if the class has an arithmetic 'Time' member defined
		concept struct Is_Time_Value
		{
			check_typedef_type(Has_Time,Time_tag,true_type);
			check_typedef_type(Is_DRSeconds,DRSeconds_tag,true_type);
			check_typedef_type(Is_Seconds,Seconds_tag,true_type);
			check_typedef_type(Is_Minutes,Minutes_tag,true_type);
			check_typedef_type(Is_Hours,Hours_tag,true_type);
			check_typedef_type(Is_Days,Days_tag,true_type);
			define_default_check(Has_Time && (Is_DRSeconds || Is_Seconds || Is_Minutes || Is_Hours || Is_Days));
		};

		/// Check if a type meets the requirements of being a basic 'Time' component
		concept struct Is_Time_Component
		{
			check_concept(Is_Component,Is_Polaris_Component, T, V);
			check_concept(Is_Time, Is_Time_Value, T, V);
			define_default_check(Is_Component && Is_Time);
		};
		concept struct Is_Time_Prototype
		{
			check_concept(Is_Prototype,Is_Prototype, T, V);
			check_typedef_type(Has_Time, Component_Type::Time_tag, true_type);
			define_default_check(Is_Prototype && Has_Time);
		};
		#pragma endregion

		#pragma region Rate Concepts
		/// Check if a type meets the requirements of being a basic 'volume' value
		concept struct Is_Rate_Value
		{
			check_typedef_type(Is_Rate, Rate_tag, true_type);
			check_concept(Is_Time, Concepts::Is_Time_Value, T, V);
			define_default_check(Is_Rate && Is_Time);
		};
		/// Check if a type meets the requirements of being a basic 'Time' component
		concept struct Is_Rate_Component
		{
			check_concept(Is_Component,Is_Polaris_Component, T, V);
			check_typedef_type(Is_Rate, Rate_tag, true_type);
			define_default_check(Is_Component && Is_Rate);
		};
		concept struct Is_Rate_Prototype
		{
			check_concept(Is_Prototype,Is_Prototype, T, V);
			check_typedef_type(Is_Rate, Component_Type::Rate_tag, true_type);
			define_default_check(Is_Rate && Is_Prototype);
		};
		concept struct Is_Speed_Value
		{
			check_typedef_type(Is_Speed, Speed_tag, true_type);
			check_concept(Is_Time, Concepts::Is_Time_Value, T, V);
			check_concept(Is_Length, Concepts::Is_Length_Value, T, V);
			define_default_check(Is_Speed && Is_Time && Is_Length);
		};
		/// Check if a type meets the requirements of being a basic 'Time' component
		concept struct Is_Speed_Component
		{
			check_concept(Is_Component,Is_Polaris_Component, T, V);
			check_typedef_type(Is_Speed, Speed_tag, true_type);
			define_default_check(Is_Component && Is_Speed);
		};
		concept struct Is_Speed_Prototype
		{
			check_concept(Is_Prototype,Is_Prototype, T, V);
			check_typedef_type(Is_Speed, Component_Type::Speed_tag, true_type);
			define_default_check(Is_Speed && Is_Prototype);
		};
		concept struct Is_Acceleration_Value
		{
			check_typedef_type(Is_Acceleration, Acceleration_tag, true_type);
			check_concept(Is_Time, Concepts::Is_Time_Value, T, V);
			check_concept(Is_Length, Concepts::Is_Length_Value, T, V);
			define_default_check(Is_Acceleration && Is_Time && Is_Length);
		};
		/// Check if a type meets the requirements of being a basic 'Time' component
		concept struct Is_Acceleration_Component
		{
			check_concept(Is_Component,Is_Polaris_Component, T, V);
			check_typedef_type(Is_Acceleration, Acceleration_tag, true_type);
			define_default_check(Is_Component && Is_Acceleration);
		};
		concept struct Is_Acceleration_Prototype
		{
			check_concept(Is_Prototype,Is_Prototype, T, V);
			check_typedef_type(Is_Acceleration, Component_Type::Acceleration_tag, true_type);
			define_default_check(Is_Acceleration && Is_Prototype);
		};
		#pragma endregion

		#pragma region Currency Concepts
		/// Check if the class has an arithmetic 'Time' member defined
		concept struct Is_Currency_Value
		{
			check_typedef_type(Has_Currency,Currency_tag,true_type);
			check_typedef_type(Is_Cents,Cents_tag,true_type);
			check_typedef_type(Is_Dollars,Dollars_tag,true_type);
			check_typedef_type(Is_Thousand_Dollars,Thousand_Dollars_tag,true_type);
			check_typedef_type(Is_Million_Dollars,Million_Dollars_tag,true_type);	
			define_default_check(Has_Currency && (Is_Cents || Is_Dollars || Is_Thousand_Dollars || Is_Million_Dollars));
		};

		/// Check if a type meets the requirements of being a basic 'Time' component
		concept struct Is_Currency_Component
		{
			check_concept(Is_Component,Is_Polaris_Component, T, V);
			check_concept(Is_Currency, Is_Currency_Value, T, V);
			define_default_check(Is_Component && Is_Currency);
		};
		concept struct Is_Currency_Prototype
		{
			check_concept(Is_Prototype,Is_Prototype, T, V);
			check_typedef_type(Is_Currency, Component_Type::Currency_tag, true_type);
			define_default_check(Is_Prototype && Is_Currency);
		};
		#pragma endregion
	}

	namespace Prototypes
	{
		//------------------------------------------------------------------------------------------------------------------
		/// RENAME THE Inteface struct below.  This is the inteface to a POLARIS component
		//------------------------------------------------------------------------------------------------------------------
		prototype struct Length ADD_DEBUG_INFO
		{
			tag_as_prototype;

			define_get_set_exists_check(Value, Get_Value_exists, Set_Value_exists);

			template<typename TargetType> TargetType Value(requires(TargetType, check(ComponentType, Get_Value_exists) && check(strip_modifiers(TargetType), Concepts::Is_Length_Value)));
			template<typename TargetType> TargetType Value(requires(TargetType, check(ComponentType, !Get_Value_exists) || check(strip_modifiers(TargetType), !Concepts::Is_Length_Value)));
			template<typename TargetType> void Value(TargetType value, requires(TargetType, check(ComponentType, Set_Value_exists) && check(strip_modifiers(TargetType), Concepts::Is_Length_Value)));
			template<typename TargetType> void Value(TargetType value, requires(TargetType, check(ComponentType, !Set_Value_exists) || check(strip_modifiers(TargetType), !Concepts::Is_Length_Value)));

			template<typename InputType, typename ReturnType> static ReturnType Convert_Value(InputType input_value, requires(InputType, check(InputType, Concepts::Is_Length_Value) && check(ReturnType, Concepts::Is_Length_Value)));
			template<typename InputType, typename ReturnType> static ReturnType Convert_Value(InputType input_value, requires(InputType, check(InputType, !Concepts::Is_Length_Value) || check(ReturnType, !Concepts::Is_Length_Value)));

			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Centimeters) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Centimeters)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Feet) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Centimeters)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Inches) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Centimeters)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Kilometers) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Centimeters)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Meters) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Centimeters)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Miles) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Centimeters)));

			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Centimeters) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Feet)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Feet) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Feet)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Inches) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Feet)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Kilometers) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Feet)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Meters) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Feet)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Miles) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Feet)));

			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Centimeters) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Inches)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Feet) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Inches)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Inches) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Inches)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Kilometers) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Inches)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Meters) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Inches)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Miles) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Inches)));

			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Centimeters) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Kilometers)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Feet) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Kilometers)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Inches) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Kilometers)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Kilometers) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Kilometers)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Meters) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Kilometers)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Miles) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Kilometers)));
				
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Centimeters) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Meters)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Feet) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Meters)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Inches) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Meters)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Kilometers) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Meters)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Meters) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Meters)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Miles) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Meters)));
					
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Centimeters) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Miles)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Feet) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Miles)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Inches) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Miles)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Kilometers) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Miles)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Meters) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Miles)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Miles) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Miles)));

			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, !(
				(sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Centimeters) || sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Feet) || sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Inches) ||
					sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Kilometers) || sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Meters) || sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Miles)) &&
					(sub_check(ComponentType, Concepts::Is_Length_Value, Is_Centimeters) || sub_check(ComponentType, Concepts::Is_Length_Value, Is_Feet) || sub_check(ComponentType, Concepts::Is_Length_Value, Is_Inches) ||
						sub_check(ComponentType, Concepts::Is_Length_Value, Is_Kilometers) || sub_check(ComponentType, Concepts::Is_Length_Value, Is_Meters) || sub_check(ComponentType, Concepts::Is_Length_Value, Is_Miles)))));
		};

		template<typename ComponentType>
		template<typename TargetType>
		TargetType Length<ComponentType>::Value(requires(TargetType, check(ComponentType, Get_Value_exists) && check(strip_modifiers(TargetType), Concepts::Is_Length_Value)))
		{
			return TargetType(this_component()->template Value<Value_Type>() * Conversion_Factor<TargetType>());
		}

		template<typename ComponentType>
		template<typename TargetType>
		TargetType Length<ComponentType>::Value(requires(TargetType, check(ComponentType, !Get_Value_exists) || check(strip_modifiers(TargetType), !Concepts::Is_Length_Value)))
		{
			assert_check(ComponentType, Get_Value_exists, "Getter does not exists for this accessor.");
			assert_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, "The specified TargetType is not a valid Length data structure.");
		}

		template<typename ComponentType>
		template<typename TargetType>
		void Length<ComponentType>::Value(TargetType value, requires(TargetType, check(ComponentType, Set_Value_exists) && check(strip_modifiers(TargetType), Concepts::Is_Length_Value)))
		{
			this_component()->template Value<Value_Type>(value / Conversion_Factor<TargetType>());
		}

		template<typename ComponentType>
		template<typename TargetType>
		void Length<ComponentType>::Value(TargetType value, requires(TargetType, check(ComponentType, !Set_Value_exists) || check(strip_modifiers(TargetType), !Concepts::Is_Length_Value)))
		{
			assert_check(ComponentType, Set_Value_exists, "Setter does not exists for this accessor.");
			// %%%RLW assert_check(strip_modifiers(TargetType),Concepts::Is_Length_Value, "The specified TargetType is not a valid Length data structure.");
		}

		template<typename ComponentType>
		template<typename InputType, typename ReturnType>
		ReturnType Length<ComponentType>::Convert_Value(InputType input_value, requires(InputType, check(InputType, Concepts::Is_Length_Value) && check(ReturnType, Concepts::Is_Length_Value)))
		{
			Value_Type convert_component_value_to_param = Conversion_Factor<InputType>();
			Value_Type convert_component_value_to_return = Conversion_Factor<ReturnType>();
			return ReturnType((Value_Type)(input_value.Value) * convert_component_value_to_return / convert_component_value_to_param);
		}

		template<typename ComponentType>
		template<typename InputType, typename ReturnType>
		ReturnType Length<ComponentType>::Convert_Value(InputType input_value, requires(InputType, check(InputType, !Concepts::Is_Length_Value) || check(ReturnType, !Concepts::Is_Length_Value)))
		{
			assert_check(ReturnType, Concepts::Is_Length_Value, "ReturnType is not a valid length value structure.");
			assert_check(InputType, Concepts::Is_Length_Value, "InputTypee is not a valid length value structure.");
		}

		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Length<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Centimeters) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Centimeters)))
		{
			return (Value_Type)1.0;
		};

		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Length<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Feet) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Centimeters)))
		{
			return (Value_Type)0.0328084;
		};

		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Length<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Inches) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Centimeters)))
		{
			return (Value_Type)0.393701;
		};

		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Length<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Kilometers) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Centimeters)))
		{
			return (Value_Type)1.0 / (Value_Type)100000.0;
		};

		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Length<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Meters) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Centimeters)))
		{
			return (Value_Type)1.0 / (Value_Type)100.0;
		};
		
		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Length<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Miles) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Centimeters)))
		{
			return (Value_Type)(1.0 / 160934.0);
		};

		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Length<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Centimeters) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Feet)))
		{
			return (Value_Type)(30.48);
		};
		
		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Length<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Feet) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Feet)))
		{
			return (Value_Type)(1.0);
		};
		
		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Length<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Inches) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Feet)))
		{
			return (Value_Type)(12.0);
		};
		
		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Length<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Kilometers) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Feet)))
		{
			return (Value_Type)(1.0 / 3280.84);
		};
		
		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Length<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Meters) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Feet)))
		{
			return (Value_Type)(0.3048);
		};
		
		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Length<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Miles) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Feet)))
		{
			return (Value_Type)(1.0 / 5280.0);
		};

		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Length<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Centimeters) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Inches)))
		{
			return (Value_Type)(2.54);
		};
		
		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Length<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Feet) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Inches)))
		{
			return (Value_Type)(1.0 / 12.0);
		};
		
		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Length<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Inches) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Inches)))
		{
			return (Value_Type)(1.0);
		};
		
		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Length<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Kilometers) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Inches)))
		{
			return (Value_Type)(2.54 / 100000.0);
		};
		
		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Length<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Meters) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Inches)))
		{
			return (Value_Type)(0.0254);
		};
		
		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Length<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Miles) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Inches)))
		{
			return (Value_Type)(0.0833333 / 5280.0);
		};

		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Length<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Centimeters) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Kilometers)))
		{
			return (Value_Type)(100000.0);
		};

		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Length<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Feet) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Kilometers)))
		{
			return (Value_Type)(3280.84);
		};

		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Length<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Inches) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Kilometers)))
		{
			return (Value_Type)(39370.1);
		};

		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Length<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Kilometers) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Kilometers)))
		{
			return (Value_Type)(1.0);
		};

		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Length<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Meters) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Kilometers)))
		{
			return (Value_Type)(1000.0);
		};

		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Length<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Miles) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Kilometers)))
		{
			return (Value_Type)(0.621371);
		};

		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Length<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Centimeters) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Meters)))
		{
			return (Value_Type)(100.0);
		};

		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Length<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Feet) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Meters)))
		{
			return (Value_Type)(3.28084);
		};

		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Length<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Inches) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Meters)))
		{
			return (Value_Type)(39.3701);
		};

		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Length<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Kilometers) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Meters)))
		{
			return (Value_Type)(0.001);
		};

		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Length<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Meters) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Meters)))
		{
			return (Value_Type)(1.0);
		};

		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Length<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Miles) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Meters)))
		{
			return (Value_Type)(0.000621371);
		};

		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Length<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Centimeters) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Miles)))
		{
			return (Value_Type)(160934.0);
		};

		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Length<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Feet) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Miles)))
		{
			return (Value_Type)(5280.0);
		};

		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Length<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Inches) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Miles)))
		{
			return (Value_Type)(63360.0);
		};

		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Length<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Kilometers) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Miles)))
		{
			return (Value_Type)(1.60934);
		};

		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Length<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Meters) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Miles)))
		{
			return (Value_Type)(1609.34);
		};

		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Length<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Miles) && sub_check(ComponentType, Concepts::Is_Length_Value, Is_Miles)))
		{
			return (Value_Type)(1.0);
		};

		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Length<ComponentType>::Conversion_Factor(requires(TargetType, !(
			(sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Centimeters) || sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Feet) || sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Inches) ||
				sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Kilometers) || sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Meters) || sub_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, Is_Miles)) &&
				(sub_check(ComponentType, Concepts::Is_Length_Value, Is_Centimeters) || sub_check(ComponentType, Concepts::Is_Length_Value, Is_Feet) || sub_check(ComponentType, Concepts::Is_Length_Value, Is_Inches) ||
					sub_check(ComponentType, Concepts::Is_Length_Value, Is_Kilometers) || sub_check(ComponentType, Concepts::Is_Length_Value, Is_Meters) || sub_check(ComponentType, Concepts::Is_Length_Value, Is_Miles)))))
		{
			assert_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, "The specified TargetType is not a valid spatial measurement Data Structure.");
		}


		prototype struct Width : public Length<ComponentType>
		{
			tag_as_prototype;

			template<typename TargetType> TargetType Value();
			template<typename TargetType> void Value(TargetType value);
		};

		template<typename ComponentType>
		template<typename TargetType>
		TargetType Width<ComponentType>::Value()
		{
			return ((Length<ComponentType>*)this)->template Value<TargetType>();
		}

		template<typename ComponentType>
		template<typename TargetType>
		void Width<ComponentType>::Value(TargetType value)
		{
			((Length<ComponentType>*)this)->template Value<TargetType>(value);
		}

		prototype struct Height : public Length<ComponentType>
		{
			tag_as_prototype;

			template<typename TargetType> TargetType Value();
			template<typename TargetType> void Value(TargetType value);
		};

		template<typename ComponentType>
		template<typename TargetType>
		TargetType Height<ComponentType>::Value()
		{
			return ((Length<ComponentType>*)this)->template Value<TargetType>();
		}

		template<typename ComponentType>
		template<typename TargetType>
		void Height<ComponentType>::Value(TargetType value)
		{
			((Length<ComponentType>*)this)->template Value<TargetType>(value);
		}

		prototype struct Area : public Length<ComponentType>
		{
			tag_as_prototype;

			typedef Length<ComponentType> base_type;
			template<typename TargetType> TargetType Value(requires(TargetType, check(strip_modifiers(TargetType), Concepts::Is_Area_Value)));
			template<typename TargetType> TargetType Value(requires(TargetType, check(strip_modifiers(TargetType), !Concepts::Is_Area_Value)));

			template<typename TargetType> void Value(TargetType value, requires(TargetType, check(strip_modifiers(TargetType), Concepts::Is_Area_Value)));
			template<typename TargetType> void Value(TargetType value, requires(TargetType, check(strip_modifiers(TargetType), !Concepts::Is_Area_Value)));

			template<typename InputType, typename ReturnType> static ReturnType Convert_Value(InputType input_value, requires(InputType, check(InputType, Concepts::Is_Area_Value) && check(ReturnType, Concepts::Is_Area_Value)));
			template<typename InputType, typename ReturnType>  static ReturnType Convert_Value(InputType input_value, requires(InputType, check(InputType, !Concepts::Is_Area_Value) || check(ReturnType, !Concepts::Is_Area_Value)));
		};

		template<typename ComponentType>
		template<typename TargetType>
		TargetType Area<ComponentType>::Value(requires(TargetType, check(strip_modifiers(TargetType), Concepts::Is_Area_Value)))
		{
			return ((Length<ComponentType>*)this)->template Value<TargetType>()*((Length<ComponentType>*)this)->template Conversion_Factor<TargetType>();
		}

		template<typename ComponentType>
		template<typename TargetType>
		TargetType Area<ComponentType>::Value(requires(TargetType, check(strip_modifiers(TargetType), !Concepts::Is_Area_Value)))
		{
			assert_check(strip_modifiers(TargetType), Concepts::Is_Area_Value, "Your target type is not identified as an area measure.");
		}

		template<typename ComponentType>
		template<typename TargetType>
		void Area<ComponentType>::Value(TargetType value, requires(TargetType, check(strip_modifiers(TargetType), Concepts::Is_Area_Value)))
		{
			((Length<ComponentType>*)this)->template Value<TargetType>(value / ((Length<ComponentType>*)this)->template Conversion_Factor<TargetType>());
		}

		template<typename ComponentType>
		template<typename TargetType>
		void Area<ComponentType>::Value(TargetType value, requires(TargetType, check(strip_modifiers(TargetType), !Concepts::Is_Area_Value)))
		{
			assert_check(strip_modifiers(TargetType), Concepts::Is_Area_Value, "Your target type is not identified as an area measure.");
		}

		template<typename ComponentType>
		template<typename InputType, typename ReturnType>
		ReturnType Area<ComponentType>::Convert_Value(InputType input_value, requires(InputType, check(InputType, Concepts::Is_Area_Value) && check(ReturnType, Concepts::Is_Area_Value)))
		{
			Value_Type convert_component_value_to_param = base_type::template Conversion_Factor<InputType>();
			Value_Type convert_component_value_to_return = base_type::template Conversion_Factor<ReturnType>();
			Value_Type conversion = convert_component_value_to_return / convert_component_value_to_param;
			return ReturnType((Value_Type)(input_value.Value) * pow(conversion, 2.0));
		}

		template<typename ComponentType>
		template<typename InputType, typename ReturnType>
		ReturnType Area<ComponentType>::Convert_Value(InputType input_value, requires(InputType, check(InputType, !Concepts::Is_Area_Value) || check(ReturnType, !Concepts::Is_Area_Value)))
		{
			assert_check(ReturnType, Concepts::Is_Area_Value, "ReturnType is not a valid area value structure.");
			assert_check(InputType, Concepts::Is_Area_Value, "InputType is not a valid area value structure.");
		}

		prototype struct Volume : protected  Length<ComponentType>
		{
			typedef Length<ComponentType> base_type;

			template<typename TargetType>  TargetType Value(requires(TargetType, check(strip_modifiers(TargetType), Concepts::Is_Volume_Value)));
			template<typename TargetType>  TargetType Value(requires(TargetType, check(strip_modifiers(TargetType), !Concepts::Is_Volume_Value)));

			template<typename TargetType>  void Value(TargetType value, requires(TargetType, check(strip_modifiers(TargetType), Concepts::Is_Volume_Value)));
			template<typename TargetType>  void Value(TargetType value, requires(TargetType, check(strip_modifiers(TargetType), !Concepts::Is_Volume_Value)));

			template<typename InputType, typename ReturnType> static ReturnType Convert_Value(InputType input_value, requires(InputType, check(InputType, Concepts::Is_Volume_Value) && check(ReturnType, Concepts::Is_Volume_Value)));
			template<typename InputType, typename ReturnType> static ReturnType Convert_Value(InputType input_value, requires(InputType, check(InputType, !Concepts::Is_Volume_Value) || check(ReturnType, !Concepts::Is_Volume_Value)));
		};

		template<typename ComponentType>
		template<typename TargetType>
		TargetType Volume<ComponentType>::Value(requires(TargetType, check(strip_modifiers(TargetType), Concepts::Is_Volume_Value)))
		{
			return ((Length<ComponentType>*)this)->template Value<TargetType>()*((Length<ComponentType>*)this)->template Conversion_Factor<TargetType>()*((Length<ComponentType>*)this)->template Conversion_Factor<TargetType>();
		}

		template<typename ComponentType>
		template<typename TargetType>
		TargetType Volume<ComponentType>::Value(requires(TargetType, check(strip_modifiers(TargetType), !Concepts::Is_Volume_Value)))
		{
			assert_check(strip_modifiers(TargetType), Concepts::Is_Volume_Value, "Your target type is not identified as a volume measure.");
		}

		template<typename ComponentType>
		template<typename TargetType>
		void Volume<ComponentType>::Value(TargetType value, requires(TargetType, check(strip_modifiers(TargetType), Concepts::Is_Volume_Value)))
		{
			((Length<ComponentType>*)this)->template Value<TargetType>(value / ((Length<ComponentType>*)this)->template Conversion_Factor<TargetType>() / ((Length<ComponentType>*)this)->template Conversion_Factor<TargetType>());
		}

		template<typename ComponentType>
		template<typename TargetType>
		void Volume<ComponentType>::Value(TargetType value, requires(TargetType, check(strip_modifiers(TargetType), !Concepts::Is_Volume_Value)))
		{
			assert_check(strip_modifiers(TargetType), Concepts::Is_Volume_Value, "Your target type is not identified as a volume measure.");
		}

		template<typename ComponentType>
		template<typename InputType, typename ReturnType>
		ReturnType Volume<ComponentType>::Convert_Value(InputType input_value, requires(InputType, check(InputType, Concepts::Is_Volume_Value) && check(ReturnType, Concepts::Is_Volume_Value)))
		{
			Value_Type convert_component_value_to_param = base_type::template Conversion_Factor<InputType>();
			Value_Type convert_component_value_to_return = base_type::template Conversion_Factor<ReturnType>();
			Value_Type conversion = convert_component_value_to_return / convert_component_value_to_param;
			return ReturnType((Value_Type)(input_value.Value) * pow(conversion, 3.0));
		}

		template<typename ComponentType>
		template<typename InputType, typename ReturnType>
		ReturnType Volume<ComponentType>::Convert_Value(InputType input_value, requires(InputType, check(InputType, !Concepts::Is_Volume_Value) || check(ReturnType, !Concepts::Is_Volume_Value)))
		{
			assert_check(ReturnType, Concepts::Is_Volume_Value, "ReturnType is not a valid volume value structure.");
			assert_check(InputType, Concepts::Is_Volume_Value, "InputType is not a valid volume value structure.");
		}

		prototype struct Time ADD_DEBUG_INFO
		{
			tag_as_prototype;

			define_get_set_exists_check(Value,Get_Value_exists, Set_Value_exists);
			template<typename TargetType> TargetType Value(requires(TargetType, check(ComponentType, Get_Value_exists) && check(strip_modifiers(TargetType), Concepts::Is_Time_Value) && !check_2(strip_modifiers(TargetType), Simulation_Timestep_Increment, is_same)));
			template<typename TargetType> TargetType Value(requires(TargetType, check(ComponentType, Get_Value_exists) && check(strip_modifiers(TargetType), Concepts::Is_Time_Value) && check_2(strip_modifiers(TargetType), Simulation_Timestep_Increment, is_same)));
			template<typename TargetType> TargetType Value(requires(TargetType, check(ComponentType, !Get_Value_exists) || check(strip_modifiers(TargetType), !Concepts::Is_Time_Value)));
			template<typename TargetType> void Value(TargetType value, requires(TargetType, check(ComponentType, Set_Value_exists) && check(strip_modifiers(TargetType), Concepts::Is_Time_Value)));
			template<typename TargetType> void Value(TargetType value, requires(TargetType, check(ComponentType, !Set_Value_exists) || check(strip_modifiers(TargetType), !Concepts::Is_Time_Value)));
			
			template<typename InputType, typename ReturnType> static ReturnType Convert_Value(InputType input_value, requires(InputType, check(InputType, Concepts::Is_Time_Value) && check(ReturnType, Concepts::Is_Time_Value) && !check_2(ReturnType, Simulation_Timestep_Increment, is_same)));
			template<typename InputType, typename ReturnType> static ReturnType Convert_Value(InputType input_value, requires(InputType, check(InputType, Concepts::Is_Time_Value) && check(ReturnType, Concepts::Is_Time_Value) && check_2(ReturnType, Simulation_Timestep_Increment, is_same)));
			template<typename InputType, typename ReturnType> static ReturnType Convert_Value(InputType input_value, requires(InputType, check(InputType, !Concepts::Is_Time_Value) || check(ReturnType, !Concepts::Is_Time_Value)));

			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Time_Value, Is_Days) && sub_check(ComponentType, Concepts::Is_Time_Value, Is_Days)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Time_Value, Is_DRSeconds) && sub_check(ComponentType, Concepts::Is_Time_Value, Is_Days)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Time_Value, Is_Hours) && sub_check(ComponentType, Concepts::Is_Time_Value, Is_Days)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Time_Value, Is_Minutes) && sub_check(ComponentType, Concepts::Is_Time_Value, Is_Days)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Time_Value, Is_Seconds) && sub_check(ComponentType, Concepts::Is_Time_Value, Is_Days)));

			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Time_Value, Is_Days) && sub_check(ComponentType, Concepts::Is_Time_Value, Is_DRSeconds)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Time_Value, Is_DRSeconds) && sub_check(ComponentType, Concepts::Is_Time_Value, Is_DRSeconds)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Time_Value, Is_Hours) && sub_check(ComponentType, Concepts::Is_Time_Value, Is_DRSeconds)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Time_Value, Is_Minutes) && sub_check(ComponentType, Concepts::Is_Time_Value, Is_DRSeconds)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Time_Value, Is_Seconds) && sub_check(ComponentType, Concepts::Is_Time_Value, Is_DRSeconds)));

			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Time_Value, Is_Days) && sub_check(ComponentType, Concepts::Is_Time_Value, Is_Hours)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Time_Value, Is_DRSeconds) && sub_check(ComponentType, Concepts::Is_Time_Value, Is_Hours)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Time_Value, Is_Hours) && sub_check(ComponentType, Concepts::Is_Time_Value, Is_Hours)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Time_Value, Is_Minutes) && sub_check(ComponentType, Concepts::Is_Time_Value, Is_Hours)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Time_Value, Is_Seconds) && sub_check(ComponentType, Concepts::Is_Time_Value, Is_Hours)));

			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Time_Value, Is_Days) && sub_check(ComponentType, Concepts::Is_Time_Value, Is_Minutes)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Time_Value, Is_DRSeconds) && sub_check(ComponentType, Concepts::Is_Time_Value, Is_Minutes)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Time_Value, Is_Hours) && sub_check(ComponentType, Concepts::Is_Time_Value, Is_Minutes)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Time_Value, Is_Minutes) && sub_check(ComponentType, Concepts::Is_Time_Value, Is_Minutes)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Time_Value, Is_Seconds) && sub_check(ComponentType, Concepts::Is_Time_Value, Is_Minutes)));

			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Time_Value, Is_Days) && sub_check(ComponentType, Concepts::Is_Time_Value, Is_Seconds)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Time_Value, Is_DRSeconds) && sub_check(ComponentType, Concepts::Is_Time_Value, Is_Seconds)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Time_Value, Is_Hours) && sub_check(ComponentType, Concepts::Is_Time_Value, Is_Seconds)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Time_Value, Is_Minutes) && sub_check(ComponentType, Concepts::Is_Time_Value, Is_Seconds)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Time_Value, Is_Seconds) && sub_check(ComponentType, Concepts::Is_Time_Value, Is_Seconds)));
			//================================================================================================
			// Time value accessors for:
			//------------------------------------------------------------------------------------------------
			/// Time component accessor - returns the requested portion of the current time
			//accessor(Time_Component, NONE, NONE);
			///// Display the time
			//template<typename TargetType> void Write()
			//{
			//	//cout <<"Total Seconds: "<<pthis->Total_Seconds<  TargetType>()<<endl;
			//	cout << "Day "<< this->Time_Component<Time_Days>()<<":  ";
			//	cout << this->Time_Component<Time_Hours>()<<":"<<this->Time_Component<Time_Minutes>()<<":"<<this->Time_Component<Time_Seconds>()<<"."<< this->Time_Component<Time_DRSeconds>();
			//}
		};

		template<typename ComponentType>
		template<typename TargetType>
		TargetType Time<ComponentType>::Value(requires(TargetType, check(ComponentType, Get_Value_exists) && check(strip_modifiers(TargetType), Concepts::Is_Time_Value) && !check_2(strip_modifiers(TargetType), Simulation_Timestep_Increment, is_same)))
		{
			return TargetType(this_component()->template Value<Value_Type>() * Conversion_Factor<TargetType>());
		}

		template<typename ComponentType>
		template<typename TargetType>
		TargetType Time<ComponentType>::Value(requires(TargetType, check(ComponentType, Get_Value_exists) && check(strip_modifiers(TargetType), Concepts::Is_Time_Value) && check_2(strip_modifiers(TargetType), Simulation_Timestep_Increment, is_same)))
		{
			return TargetType(Round<int, Value_Type>(this_component()->template Value<Value_Type>() * Conversion_Factor<TargetType>()));
		}

		template<typename ComponentType>
		template<typename TargetType>
		TargetType Time<ComponentType>::Value(requires(TargetType, check(ComponentType, !Get_Value_exists) || check(strip_modifiers(TargetType), !Concepts::Is_Time_Value)))
		{
			assert_check(ComponentType, Get_Value_exists, "Getter does not exists for this accessor.");
			assert_check(strip_modifiers(TargetType), Concepts::Is_Time_Value, "The specified TargetType is not a valid Time data structure.");
		}

		template<typename ComponentType>
		template<typename TargetType>
		void Time<ComponentType>::Value(TargetType value, requires(TargetType, check(ComponentType, Set_Value_exists) && check(strip_modifiers(TargetType), Concepts::Is_Time_Value)))
		{
			this_component()->template Value<Value_Type>(value / Conversion_Factor<TargetType>());
		}

		template<typename ComponentType>
		template<typename TargetType>
		void Time<ComponentType>::Value(TargetType value, requires(TargetType, check(ComponentType, !Set_Value_exists) || check(strip_modifiers(TargetType), !Concepts::Is_Time_Value)))
		{
			assert_check(ComponentType, Set_Value_exists, "Setter does not exists for this accessor.");
			// %%%RLW assert_check(strip_modifiers(TargetType),Concepts::Is_Time_Value, "The specified TargetType is not a valid Time data structure.");
		}

		template<typename ComponentType>
		template<typename InputType, typename ReturnType>
		ReturnType Time<ComponentType>::Convert_Value(InputType input_value, requires(InputType, check(InputType, Concepts::Is_Time_Value) && check(ReturnType, Concepts::Is_Time_Value) && !check_2(ReturnType, Simulation_Timestep_Increment, is_same)))
		{
			Value_Type convert_component_value_to_param = Time::Conversion_Factor<InputType>();
			Value_Type convert_component_value_to_return = Time::Conversion_Factor<ReturnType>();
			return ReturnType((Value_Type)(input_value.Value) * convert_component_value_to_return / convert_component_value_to_param);
		}

		template<typename ComponentType>
		template<typename InputType, typename ReturnType>
		ReturnType Time<ComponentType>::Convert_Value(InputType input_value, requires(InputType, check(InputType, Concepts::Is_Time_Value) && check(ReturnType, Concepts::Is_Time_Value) && check_2(ReturnType, Simulation_Timestep_Increment, is_same)))
		{
			Value_Type convert_component_value_to_param = Time::Conversion_Factor<InputType>();
			Value_Type convert_component_value_to_return = Time::Conversion_Factor<ReturnType>();
			return ReturnType(Round<int, Value_Type>((Value_Type)(input_value.Value) * convert_component_value_to_return / convert_component_value_to_param));
		}

		template<typename ComponentType>
		template<typename InputType, typename ReturnType>
		ReturnType Time<ComponentType>::Convert_Value(InputType input_value, requires(InputType, check(InputType, !Concepts::Is_Time_Value) || check(ReturnType, !Concepts::Is_Time_Value)))
		{
			assert_check(InputType, Concepts::Is_Time_Value, "InputType is not a valid Time type.");
			assert_check(ReturnType, Concepts::Is_Time_Value, "ReturnType is not a valid Time type.");
		}

		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Time<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Time_Value, Is_Days) && sub_check(ComponentType, Concepts::Is_Time_Value, Is_Days)))
		{
			return (Value_Type)(1.0);
		};

		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Time<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Time_Value, Is_DRSeconds) && sub_check(ComponentType, Concepts::Is_Time_Value, Is_Days)))
		{
			return (Value_Type)(864000.0);
		};

		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Time<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Time_Value, Is_Hours) && sub_check(ComponentType, Concepts::Is_Time_Value, Is_Days)))
		{
			return (Value_Type)(24.0);
		};

		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Time<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Time_Value, Is_Minutes) && sub_check(ComponentType, Concepts::Is_Time_Value, Is_Days)))
		{
			return (Value_Type)(1440.0);
		};

		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Time<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Time_Value, Is_Seconds) && sub_check(ComponentType, Concepts::Is_Time_Value, Is_Days)))
		{
			return (Value_Type)(86400.0);
		};

		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Time<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Time_Value, Is_Days) && sub_check(ComponentType, Concepts::Is_Time_Value, Is_DRSeconds)))
		{
			return (Value_Type)(1.0 / 864000.0);
		};

		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Time<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Time_Value, Is_DRSeconds) && sub_check(ComponentType, Concepts::Is_Time_Value, Is_DRSeconds)))
		{
			return (Value_Type)(1.0);
		};

		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Time<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Time_Value, Is_Hours) && sub_check(ComponentType, Concepts::Is_Time_Value, Is_DRSeconds)))
		{
			return (Value_Type)(1.0 / 36000.0);
		};

		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Time<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Time_Value, Is_Minutes) && sub_check(ComponentType, Concepts::Is_Time_Value, Is_DRSeconds)))
		{
			return (Value_Type)(1.0 / 600.0);
		};

		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Time<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Time_Value, Is_Seconds) && sub_check(ComponentType, Concepts::Is_Time_Value, Is_DRSeconds)))
		{
			return (Value_Type)(0.1);
		};

		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Time<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Time_Value, Is_Days) && sub_check(ComponentType, Concepts::Is_Time_Value, Is_Hours)))
		{
			return (Value_Type)(1.0 / 24.0);
		};

		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Time<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Time_Value, Is_DRSeconds) && sub_check(ComponentType, Concepts::Is_Time_Value, Is_Hours)))
		{
			return (Value_Type)(36000.0);
		};

		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Time<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Time_Value, Is_Hours) && sub_check(ComponentType, Concepts::Is_Time_Value, Is_Hours)))
		{
			return (Value_Type)(1.0);
		};

		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Time<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Time_Value, Is_Minutes) && sub_check(ComponentType, Concepts::Is_Time_Value, Is_Hours)))
		{
			return (Value_Type)(60.0);
		};

		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Time<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Time_Value, Is_Seconds) && sub_check(ComponentType, Concepts::Is_Time_Value, Is_Hours)))
		{
			return (Value_Type)(3600.0);
		};

		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Time<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Time_Value, Is_Days) && sub_check(ComponentType, Concepts::Is_Time_Value, Is_Minutes)))
		{
			return (Value_Type)(1.0 / 1440.0);
		};

		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Time<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Time_Value, Is_DRSeconds) && sub_check(ComponentType, Concepts::Is_Time_Value, Is_Minutes)))
		{
			return (Value_Type)(600.0);
		};

		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Time<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Time_Value, Is_Hours) && sub_check(ComponentType, Concepts::Is_Time_Value, Is_Minutes)))
		{
			return (Value_Type)(1.0 / 60.0);
		};

		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Time<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Time_Value, Is_Minutes) && sub_check(ComponentType, Concepts::Is_Time_Value, Is_Minutes)))
		{
			return (Value_Type)(1.0);
		};

		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Time<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Time_Value, Is_Seconds) && sub_check(ComponentType, Concepts::Is_Time_Value, Is_Minutes)))
		{
			return (Value_Type)(60.0);
		};

		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Time<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Time_Value, Is_Days) && sub_check(ComponentType, Concepts::Is_Time_Value, Is_Seconds)))
		{
			return (Value_Type)(1.0 / 86400.0);
		};

		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Time<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Time_Value, Is_DRSeconds) && sub_check(ComponentType, Concepts::Is_Time_Value, Is_Seconds)))
		{
			return (Value_Type)(10.0);
		};

		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Time<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Time_Value, Is_Hours) && sub_check(ComponentType, Concepts::Is_Time_Value, Is_Seconds)))
		{
			return (Value_Type)(1.0 / 3600.0);
		};

		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Time<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Time_Value, Is_Minutes) && sub_check(ComponentType, Concepts::Is_Time_Value, Is_Seconds)))
		{
			return (Value_Type)(1.0 / 60.0);
		};

		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Time<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Time_Value, Is_Seconds) && sub_check(ComponentType, Concepts::Is_Time_Value, Is_Seconds)))
		{
			return (Value_Type)(1.0);
		};

		prototype struct Rate : protected Time<ComponentType>
		{
			tag_as_prototype;
			typedef Time<ComponentType> base_type;

			define_get_set_exists_check(Value,Get_Value_exists, Set_Value_exists);
			template<typename TargetType> TargetType Value(requires(TargetType, check(ComponentType, Get_Value_exists) && check(strip_modifiers(TargetType), Concepts::Is_Time_Value)));
			template<typename TargetType> TargetType Value(requires(TargetType, check(ComponentType, !Get_Value_exists) || check(strip_modifiers(TargetType), !Concepts::Is_Time_Value)));
			template<typename TargetType> void Value(TargetType value, requires(TargetType, check(ComponentType, Set_Value_exists) && check(strip_modifiers(TargetType), Concepts::Is_Time_Value)));
			template<typename TargetType> void Value(TargetType value, requires(TargetType, check(ComponentType, !Set_Value_exists) || check(strip_modifiers(TargetType), !Concepts::Is_Time_Value)));
			
			template<typename InputType, typename ReturnType> static ReturnType Convert_Value(InputType input_value, requires(InputType, check(InputType, Concepts::Is_Time_Value) && check(ReturnType, Concepts::Is_Time_Value)));
			template<typename InputType, typename ReturnType> static ReturnType Convert_Value(InputType input_value, requires(InputType, check(InputType, !Concepts::Is_Time_Value) || check(ReturnType, !Concepts::Is_Time_Value)));
		};

		template<typename ComponentType>
		template<typename TargetType>
		TargetType Rate<ComponentType>::Value(requires(TargetType, check(ComponentType, Get_Value_exists) && check(strip_modifiers(TargetType), Concepts::Is_Time_Value)))
		{
			TargetType val = base_type::template Conversion_Factor<TargetType>();
			return TargetType(this_component()->template Value<Value_Type>() / val);
		}

		template<typename ComponentType>
		template<typename TargetType>
		TargetType Rate<ComponentType>::Value(requires(TargetType, check(ComponentType, !Get_Value_exists) || check(strip_modifiers(TargetType), !Concepts::Is_Time_Value)))
		{
			assert_check(ComponentType, Get_Value_exists, "Getter does not exists for this accessor.");
			assert_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, "The specified TargetType is not a valid Time data structure.");
		}

		template<typename ComponentType>
		template<typename TargetType>
		void Rate<ComponentType>::Value(TargetType value, requires(TargetType, check(ComponentType, Set_Value_exists) && check(strip_modifiers(TargetType), Concepts::Is_Time_Value)))
		{
			this_component()->template Value<Value_Type>(value * base_type::template Conversion_Factor<TargetType>());
		}

		template<typename ComponentType>
		template<typename TargetType>
		void Rate<ComponentType>::Value(TargetType value, requires(TargetType, check(ComponentType, !Set_Value_exists) || check(strip_modifiers(TargetType), !Concepts::Is_Time_Value)))
		{
			assert_check(ComponentType, Set_Value_exists, "Setter does not exists for this accessor.");
			assert_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, "The specified TargetType is not a valid Time data structure.");
		}

		template<typename ComponentType>
		template<typename InputType, typename ReturnType>
		ReturnType Rate<ComponentType>::Convert_Value(InputType input_value, requires(InputType, check(InputType, Concepts::Is_Time_Value) && check(ReturnType, Concepts::Is_Time_Value)))
		{
			Value_Type convert_component_value_to_param = base_type::template Conversion_Factor<InputType>();
			Value_Type convert_component_value_to_return = base_type::template Conversion_Factor<ReturnType>();
			return ReturnType((Value_Type)(input_value.Value) / convert_component_value_to_return * convert_component_value_to_param);
		}

		template<typename ComponentType>
		template<typename InputType, typename ReturnType>
		ReturnType Rate<ComponentType>::Convert_Value(InputType input_value, requires(InputType, check(InputType, !Concepts::Is_Time_Value) || check(ReturnType, !Concepts::Is_Time_Value)))
		{
			assert_check(InputType, Concepts::Is_Time_Value, "InputType is not a valid Time type.");
			assert_check(ReturnType, Concepts::Is_Time_Value, "ReturnType is not a valid Time type.");
		}

		prototype struct Speed : public ComponentType, protected Time<ComponentType>, protected Length<ComponentType>
		{
			tag_as_prototype;
			typedef Length<ComponentType> length_base;
			typedef Time<ComponentType> time_base;

			define_get_set_exists_check(Value,Get_Value_exists, Set_Value_exists);
			template<typename TargetType> TargetType Value(requires(TargetType, check(ComponentType, Get_Value_exists) && check(strip_modifiers(TargetType), Concepts::Is_Speed_Value)));
			template<typename TargetType> TargetType Value(requires(TargetType, !check(ComponentType, Get_Value_exists) || !check(strip_modifiers(TargetType), Concepts::Is_Speed_Value)));
			template<typename TargetType> void Value(TargetType value, requires(TargetType, check(ComponentType, Set_Value_exists) && check(strip_modifiers(TargetType), Concepts::Is_Speed_Value)));
			template<typename TargetType> void Value(TargetType value, requires(TargetType, !check(ComponentType, Set_Value_exists) || !check(strip_modifiers(TargetType), Concepts::Is_Speed_Value)));
			
			template<typename InputType, typename ReturnType> static ReturnType Convert_Value(InputType input_value, requires(InputType,check(InputType,Concepts::Is_Time_Value) && check(ReturnType,Concepts::Is_Time_Value)))
			{
				Value_Type convert_component_value_to_param = time_base::template Conversion_Factor<InputType>() / length_base::template Conversion_Factor<InputType>();
				Value_Type convert_component_value_to_return = time_base::template Conversion_Factor<ReturnType>() / length_base::template Conversion_Factor<ReturnType>();
				return ReturnType((Value_Type)(input_value.Value) / convert_component_value_to_return * convert_component_value_to_param);
			}
			template<typename InputType, typename ReturnType> static ReturnType Convert_Value(InputType input_value, requires(InputType,check(InputType,!Concepts::Is_Time_Value) || check(ReturnType,!Concepts::Is_Time_Value)))
			{
				assert_check(InputType,Concepts::Is_Time_Value,"InputType is not a valid Time type.");
				assert_check(ReturnType,Concepts::Is_Time_Value,"ReturnType is not a valid Time type.");
			}
		};

		template<typename ComponentType>
		template<typename TargetType>
		TargetType Speed<ComponentType>::Value(requires(TargetType, check(ComponentType, Get_Value_exists) && check(strip_modifiers(TargetType), Concepts::Is_Speed_Value)))
		{
			TargetType len_val = length_base::template Conversion_Factor<TargetType>();
			TargetType time_val = time_base::template Conversion_Factor<TargetType>();
			return TargetType(this_component()->template Value<Value_Type>() * len_val / time_val);
		}

		template<typename ComponentType>
		template<typename TargetType>
		TargetType Speed<ComponentType>::Value(requires(TargetType, !check(ComponentType, Get_Value_exists) || !check(strip_modifiers(TargetType), Concepts::Is_Speed_Value)))
		{
			assert_check(ComponentType, Get_Value_exists, "Getter does not exists for this accessor.");
			assert_check(strip_modifiers(TargetType), Concepts::Is_Speed_Value, "The specified TargetType is not a valid Speed data structure, ensure that TargetType has tags: {Speed_Type, Length_Type and Time_Type}");
			assert_check(ComponentType, Concepts::Is_Speed_Component, "The specified ComponentType is not a valid Speed component, ensure that ComponentType is tagged as a Speed_Type and has Value member}");
		}

		template<typename ComponentType>
		template<typename TargetType>
		void Speed<ComponentType>::Value(TargetType value, requires(TargetType, check(ComponentType, Set_Value_exists) && check(strip_modifiers(TargetType), Concepts::Is_Speed_Value)))
		{
			TargetType len_val = length_base::template Conversion_Factor<TargetType>();
			TargetType time_val = time_base::template Conversion_Factor<TargetType>();
			this_component()->template Value<Value_Type>(Value_Type(value * time_val / len_val));
		}

		template<typename ComponentType>
		template<typename TargetType>
		void Speed<ComponentType>::Value(TargetType value, requires(TargetType, !check(ComponentType, Set_Value_exists) || !check(strip_modifiers(TargetType), Concepts::Is_Speed_Value)))
		{
			assert_check(ComponentType, Set_Value_exists, "Setter does not exists for this accessor.");
			assert_check(strip_modifiers(TargetType), Concepts::Is_Speed_Value, "The specified TargetType is not a valid Speed data structure, ensure that TargetType has tags: {Speed_Type, Length_Type and Time_Type}");
			assert_sub_check(ComponentType, Is_Polaris_Component, has_this_type, "Doesn't have This_Type");
			assert_sub_check(ComponentType, Is_Polaris_Component, has_parent_type, "Doesn't have Parent_Type");
			assert_sub_check(ComponentType, Is_Polaris_Component, has_group_list, "Doesn't have Group_List");
			assert_sub_check(ComponentType, Is_Polaris_Component, has_object_type, "Doesn't have Object_Type");
			assert_sub_check(ComponentType, Is_Polaris_Component, has_master_type, "Doesn't have Master_Type");
			assert_sub_check(ComponentType, Concepts::Is_Speed_Component, Is_Speed, "The CompentType is not tagged with Speed_tag.");
			assert_check(ComponentType, Concepts::Is_Speed_Component, "The specified ComponentType is not a valid Speed component, ensure that ComponentType is tagged as a Speed_Type and has Value member}");


		}

		prototype struct Acceleration : public ComponentType, protected Speed<ComponentType>
		{
			tag_as_prototype;

			define_get_set_exists_check(Value,Get_Value_exists, Set_Value_exists);
			
			template<typename TargetType, requires(TargetType, check(ComponentType, Get_Value_exists) && check(strip_modifiers(TargetType), Concepts::Is_Acceleration_Value))>
			TargetType Value()
			{
				TargetType len_val = Speed<ComponentType>::length_base::template Conversion_Factor<TargetType>();
				TargetType time_val = Speed<ComponentType>::time_base::template Conversion_Factor<TargetType>();
				return TargetType(this_component()->template Value<Value_Type>() * len_val / time_val / time_val);
			}

			template<typename TargetType, requires(TargetType, !check(ComponentType, Get_Value_exists) || !check(strip_modifiers(TargetType), Concepts::Is_Acceleration_Value))>
			TargetType Value()
			{
				assert_check(ComponentType, Get_Value_exists, "Getter does not exists for this accessor.");
				assert_check(strip_modifiers(TargetType), Concepts::Is_Acceleration_Value, "The specified TargetType is not a valid Acceleration data structure, ensure that TargetType has tags: {Acceleration_Type, Length_Type and Time_Type}");
				assert_check(ComponentType, Concepts::Is_Acceleration_Component, "The specified ComponentType is not a valid Acceleration component, ensure that ComponentType is tagged as a Acceleration_Type and has Value member}");
			}

			template<typename TargetType, requires(TargetType, check(ComponentType, Set_Value_exists) && check(strip_modifiers(TargetType), Concepts::Is_Acceleration_Value))>
			void Value(TargetType value)
			{
				TargetType len_val = Speed<ComponentType>::length_base::template Conversion_Factor<TargetType>();
				TargetType time_val = Speed<ComponentType>::time_base::template Conversion_Factor<TargetType>();
				this_component()->template Value<Value_Type>(Value_Type(value * time_val * time_val / len_val));
			}

			template<typename TargetType, requires(TargetType, !check(ComponentType, Set_Value_exists) || !check(strip_modifiers(TargetType), Concepts::Is_Acceleration_Value))>
			void Value(TargetType value)
			{
				assert_check(ComponentType, Set_Value_exists, "Setter does not exists for this accessor.");
				assert_check(strip_modifiers(TargetType), Concepts::Is_Speed_Value, "The specified TargetType is not a valid Acceleration data structure, ensure that TargetType has tags: {Acceleration_Type, Length_Type and Time_Type}");
			}
			
			template<typename InputType, typename ReturnType> static ReturnType Convert_Value(InputType input_value, requires(InputType,check(InputType,Concepts::Is_Time_Value) && check(ReturnType,Concepts::Is_Time_Value)))
			{
				Value_Type convert_component_value_to_param = time_base::template Conversion_Factor<InputType>()*time_base::template Conversion_Factor<InputType>() / length_base::template Conversion_Factor<InputType>();
				Value_Type convert_component_value_to_return = time_base::template Conversion_Factor<ReturnType>()*time_base::template Conversion_Factor<ReturnType>() / length_base::template Conversion_Factor<ReturnType>();
				return ReturnType((Value_Type)(input_value.Value) / convert_component_value_to_return * convert_component_value_to_param);
			}
			template<typename InputType, typename ReturnType> static ReturnType Convert_Value(InputType input_value, requires(InputType,check(InputType,!Concepts::Is_Time_Value) || check(ReturnType,!Concepts::Is_Time_Value)))
			{
				assert_check(InputType,Concepts::Is_Time_Value,"InputType is not a valid Time type.");
				assert_check(ReturnType,Concepts::Is_Time_Value,"ReturnType is not a valid Time type.");
			}
		};


		prototype struct Currency ADD_DEBUG_INFO
		{
			tag_as_prototype;

			define_get_set_exists_check(Value, Get_Value_exists, Set_Value_exists);
			template<typename TargetType> TargetType Value(requires(TargetType, check(ComponentType, Get_Value_exists) && check(strip_modifiers(TargetType), Concepts::Is_Currency_Value)));
			template<typename TargetType> TargetType Value(requires(TargetType, check(ComponentType, !Get_Value_exists) || check(strip_modifiers(TargetType), !Concepts::Is_Currency_Value)));
			template<typename TargetType> void Value(TargetType value, requires(TargetType, check(ComponentType, Set_Value_exists) && check(strip_modifiers(TargetType), Concepts::Is_Currency_Value)));
			template<typename TargetType> void Value(TargetType value, requires(TargetType, check(ComponentType, !Set_Value_exists) || check(strip_modifiers(TargetType), !Concepts::Is_Currency_Value)));

			template<typename InputType, typename ReturnType> static ReturnType Convert_Value(InputType input_value, requires(InputType, check(InputType, Concepts::Is_Currency_Value) && check(ReturnType, Concepts::Is_Currency_Value)));
			template<typename InputType, typename ReturnType> static ReturnType Convert_Value(InputType input_value, requires(InputType, check(InputType, !Concepts::Is_Currency_Value) || check(ReturnType, !Concepts::Is_Currency_Value)));

			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Currency_Value, Is_Cents) && sub_check(ComponentType, Concepts::Is_Currency_Value, Is_Cents)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Currency_Value, Is_Dollars) && sub_check(ComponentType, Concepts::Is_Currency_Value, Is_Cents)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Currency_Value, Is_Thousand_Dollars) && sub_check(ComponentType, Concepts::Is_Currency_Value, Is_Cents)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Currency_Value, Is_Million_Dollars) && sub_check(ComponentType, Concepts::Is_Currency_Value, Is_Cents)));

			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Currency_Value, Is_Cents) && sub_check(ComponentType, Concepts::Is_Currency_Value, Is_Dollars)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Currency_Value, Is_Dollars) && sub_check(ComponentType, Concepts::Is_Currency_Value, Is_Dollars)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Currency_Value, Is_Thousand_Dollars) && sub_check(ComponentType, Concepts::Is_Currency_Value, Is_Dollars)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Currency_Value, Is_Million_Dollars) && sub_check(ComponentType, Concepts::Is_Currency_Value, Is_Dollars)));

			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Currency_Value, Is_Cents) && sub_check(ComponentType, Concepts::Is_Currency_Value, Is_Thousand_Dollars)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType,sub_check(strip_modifiers(TargetType),Concepts::Is_Currency_Value, Is_Dollars) && sub_check(ComponentType,Concepts::Is_Currency_Value, Is_Thousand_Dollars)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Currency_Value, Is_Thousand_Dollars) && sub_check(ComponentType, Concepts::Is_Currency_Value, Is_Thousand_Dollars)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Currency_Value, Is_Million_Dollars) && sub_check(ComponentType, Concepts::Is_Currency_Value, Is_Thousand_Dollars)));

			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Currency_Value, Is_Cents) && sub_check(ComponentType, Concepts::Is_Currency_Value, Is_Million_Dollars)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Currency_Value, Is_Dollars) && sub_check(ComponentType, Concepts::Is_Currency_Value, Is_Million_Dollars)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Currency_Value, Is_Thousand_Dollars) && sub_check(ComponentType, Concepts::Is_Currency_Value, Is_Million_Dollars)));
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Currency_Value, Is_Million_Dollars) && sub_check(ComponentType, Concepts::Is_Currency_Value, Is_Million_Dollars)));


			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, !(
				(sub_check(strip_modifiers(TargetType), Concepts::Is_Currency_Value, Is_Cents) || sub_check(strip_modifiers(TargetType), Concepts::Is_Currency_Value, Is_Dollars) || sub_check(strip_modifiers(TargetType), Concepts::Is_Currency_Value, Is_Thousand_Dollars) ||
					sub_check(strip_modifiers(TargetType), Concepts::Is_Currency_Value, Is_Million_Dollars)) &&
					(sub_check(ComponentType, Concepts::Is_Currency_Value, Is_Cents) || sub_check(ComponentType, Concepts::Is_Currency_Value, Is_Dollars) || sub_check(ComponentType, Concepts::Is_Currency_Value, Is_Thousand_Dollars) ||
						sub_check(ComponentType, Concepts::Is_Currency_Value, Is_Million_Dollars)))));
		};

		template<typename ComponentType>
		template<typename TargetType>
		TargetType Currency<ComponentType>::Value(requires(TargetType, check(ComponentType, Get_Value_exists) && check(strip_modifiers(TargetType), Concepts::Is_Currency_Value)))
		{
			return TargetType(this_component()->template Value<Value_Type>() * Conversion_Factor<TargetType>());
		}
		
		template<typename ComponentType>
		template<typename TargetType>
		TargetType Currency<ComponentType>::Value(requires(TargetType, check(ComponentType, !Get_Value_exists) || check(strip_modifiers(TargetType), !Concepts::Is_Currency_Value)))
		{
			assert_check(ComponentType, Get_Value_exists, "Getter does not exists for this accessor.");
			assert_check(strip_modifiers(TargetType), Concepts::Is_Length_Value, "The specified TargetType is not a valid Currency data structure.");
		}
		
		template<typename ComponentType>
		template<typename TargetType>
		void Currency<ComponentType>::Value(TargetType value, requires(TargetType, check(ComponentType, Set_Value_exists) && check(strip_modifiers(TargetType), Concepts::Is_Currency_Value)))
		{
			this_component()->template Value<Value_Type>(value / Conversion_Factor<TargetType>());
		}
		
		template<typename ComponentType>
		template<typename TargetType>
		void Currency<ComponentType>::Value(TargetType value, requires(TargetType, check(ComponentType, !Set_Value_exists) || check(strip_modifiers(TargetType), !Concepts::Is_Currency_Value)))
		{
			assert_check(ComponentType, Set_Value_exists, "Setter does not exists for this accessor.");
			// %%%RLW assert_check(strip_modifiers(TargetType),Concepts::Is_Currency_Value, "The specified TargetType is not a valid Currency data structure.");
		}

		template<typename ComponentType>
		template<typename InputType, typename ReturnType>
		ReturnType Currency<ComponentType>::Convert_Value(InputType input_value, requires(InputType, check(InputType, Concepts::Is_Currency_Value) && check(ReturnType, Concepts::Is_Currency_Value)))
		{
			Value_Type convert_component_value_to_param = Conversion_Factor<InputType>();
			Value_Type convert_component_value_to_return = Conversion_Factor<ReturnType>();
			return ReturnType((Value_Type)(input_value.Value) * convert_component_value_to_return / convert_component_value_to_param);
		}
		
		template<typename ComponentType>
		template<typename InputType, typename ReturnType>
		ReturnType Currency<ComponentType>::Convert_Value(InputType input_value, requires(InputType, check(InputType, !Concepts::Is_Currency_Value) || check(ReturnType, !Concepts::Is_Currency_Value)))
		{
			assert_check(ReturnType, Concepts::Is_Length_Value, "ReturnType is not a valid length value structure.");
			assert_check(InputType, Concepts::Is_Length_Value, "InputType is not a valid length value structure.");
		}

		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Currency<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Currency_Value, Is_Cents) && sub_check(ComponentType, Concepts::Is_Currency_Value, Is_Cents)))
		{
			return (Value_Type)1.0;
		};
		
		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Currency<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Currency_Value, Is_Dollars) && sub_check(ComponentType, Concepts::Is_Currency_Value, Is_Cents)))
		{
			return (Value_Type)0.01;
		};
		
		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Currency<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Currency_Value, Is_Thousand_Dollars) && sub_check(ComponentType, Concepts::Is_Currency_Value, Is_Cents)))
		{
			return (Value_Type)0.00001;
		};
		
		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Currency<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Currency_Value, Is_Million_Dollars) && sub_check(ComponentType, Concepts::Is_Currency_Value, Is_Cents)))
		{
			return (Value_Type)0.00000001;
		};

		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Currency<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Currency_Value, Is_Cents) && sub_check(ComponentType, Concepts::Is_Currency_Value, Is_Dollars)))
		{
			return (Value_Type)(100.0);
		};
		
		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Currency<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Currency_Value, Is_Dollars) && sub_check(ComponentType, Concepts::Is_Currency_Value, Is_Dollars)))
		{
			return (Value_Type)(1.0);
		};
		
		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Currency<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Currency_Value, Is_Thousand_Dollars) && sub_check(ComponentType, Concepts::Is_Currency_Value, Is_Dollars)))
		{
			return (Value_Type)(0.001);
		};
		
		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Currency<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Currency_Value, Is_Million_Dollars) && sub_check(ComponentType, Concepts::Is_Currency_Value, Is_Dollars)))
		{
			return (Value_Type)(0.000001);
		};

		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Currency<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Currency_Value, Is_Cents) && sub_check(ComponentType, Concepts::Is_Currency_Value, Is_Thousand_Dollars)))
		{
			return (Value_Type)(100000.0);
		};
		
		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Currency<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Currency_Value, Is_Dollars) && sub_check(ComponentType, Concepts::Is_Currency_Value, Is_Thousand_Dollars)))
		{
			return (Value_Type)(1000.0);
		};

		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Currency<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Currency_Value, Is_Thousand_Dollars) && sub_check(ComponentType, Concepts::Is_Currency_Value, Is_Thousand_Dollars)))
		{
			return (Value_Type)(1.0);
		};
		
		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Currency<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Currency_Value, Is_Million_Dollars) && sub_check(ComponentType, Concepts::Is_Currency_Value, Is_Thousand_Dollars)))
		{
			return (Value_Type)(0.001);
		};

		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Currency<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Currency_Value, Is_Cents) && sub_check(ComponentType, Concepts::Is_Currency_Value, Is_Million_Dollars)))
		{
			return (Value_Type)(100000000.0);
		};
		
		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Currency<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Currency_Value, Is_Dollars) && sub_check(ComponentType, Concepts::Is_Currency_Value, Is_Million_Dollars)))
		{
			return (Value_Type)(1000000.0);
		};
		
		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Currency<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Currency_Value, Is_Thousand_Dollars) && sub_check(ComponentType, Concepts::Is_Currency_Value, Is_Million_Dollars)))
		{
			return (Value_Type)(1000.0);
		};
		
		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Currency<ComponentType>::Conversion_Factor(requires(TargetType, sub_check(strip_modifiers(TargetType), Concepts::Is_Currency_Value, Is_Million_Dollars) && sub_check(ComponentType, Concepts::Is_Currency_Value, Is_Million_Dollars)))
		{
			return (Value_Type)(1.0);
		};


		template<typename ComponentType>
		template<typename TargetType>
		Value_Type Currency<ComponentType>::Conversion_Factor(requires(TargetType, !(
			(sub_check(strip_modifiers(TargetType), Concepts::Is_Currency_Value, Is_Cents) || sub_check(strip_modifiers(TargetType), Concepts::Is_Currency_Value, Is_Dollars) || sub_check(strip_modifiers(TargetType), Concepts::Is_Currency_Value, Is_Thousand_Dollars) ||
				sub_check(strip_modifiers(TargetType), Concepts::Is_Currency_Value, Is_Million_Dollars)) &&
				(sub_check(ComponentType, Concepts::Is_Currency_Value, Is_Cents) || sub_check(ComponentType, Concepts::Is_Currency_Value, Is_Dollars) || sub_check(ComponentType, Concepts::Is_Currency_Value, Is_Thousand_Dollars) ||
					sub_check(ComponentType, Concepts::Is_Currency_Value, Is_Million_Dollars)))))
		{
			assert_check(strip_modifiers(TargetType), Concepts::Is_Currency_Value, "The specified TargetType is not a valid spatial measurement Data Structure.");
		}
	}

}
}
using namespace Basic_Units::Concepts;
using namespace Basic_Units::Prototypes;
