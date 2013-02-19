#pragma once
#include "Repository_Includes.h"

template<typename TargetType, typename InputType>
TargetType Round(InputType value, requires(check(TargetType, is_integral) && check(InputType,is_arithmetic) && (check(InputType,!is_integral))))
{
	InputType remain = value - (InputType)((TargetType)value);
	if (remain >= 0.5) return (TargetType)(value - remain + 1);
	else return (TargetType)(value - remain);
}
template<typename TargetType, typename InputType>
TargetType Round(InputType value, requires(check(TargetType, !is_integral) || check(InputType,!is_arithmetic) || (check(InputType,is_integral))))
{
	assert_check(TargetType,is_integral,"ReturnType must be integral");
	assert_check(InputType,is_arithmetic,"InputType must be arithmetic");
	assert_check(InputType,!is_integral,"InputType must be floating point type");
}


namespace Basic_Units
{
	//==========================================================================
	// Define the arithmetic value type to use in basic unit calculations here
	typedef double Value_Type;
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
			check_typename_state(Is_Length,Length_tag,true_type);
			check_typename_state(Is_Inches,Inches_tag,true_type);
			check_typename_state(Is_Feet,Feet_tag,true_type);
			check_typename_state(Is_Miles,Miles_tag,true_type);
			check_typename_state(Is_Centimeters,Centimeters_tag,true_type);
			check_typename_state(Is_Meters,Meters_tag,true_type);
			check_typename_state(Is_Kilometers,Kilometers_tag,true_type);
			define_default_check(Is_Length && (Is_Inches || Is_Feet || Is_Miles || Is_Centimeters || Is_Meters || Is_Kilometers));
		};

		/// Check if a type meets the requirements of being a basic 'Area' component
		concept struct Is_Length_Component
		{
			check_concept(Is_Component,Is_Polaris_Component);
			check_concept(Is_Length, Is_Length_Value);
			define_default_check(Is_Component && Is_Length);
		};
		concept struct Is_Length_Prototype
		{
			check_concept(Is_Prototype,Is_Polaris_Prototype);
			check_typename_state(Is_Length, Component_Type::Length_tag, true_type);
			define_default_check(Is_Prototype && Is_Length);
		};
		#pragma endregion

		#pragma region Area Concepts
		concept struct Is_Area_Value
		{
			check_typename_state(Is_Area, Area_tag, true_type);
			check_typename_state(Is_Length, Length_tag, true_type);
			define_default_check(Is_Area && Is_Length);
		};
		/// Check if a type meets the requirements of being a basic 'Area' component
		concept struct Is_Area_Component
		{
			check_concept(Is_Component,Is_Polaris_Component);
			check_concept(Is_Area, Is_Area_Value);
			define_default_check(Is_Component && Is_Area);
		};
		concept struct Is_Area_Prototype
		{
			check_concept(Is_Prototype,Is_Polaris_Prototype);
			check_typename_state(Is_Area, Component_Type::Area_tag, true_type);
			define_default_check(Is_Area && Is_Prototype);
		};
		#pragma endregion

		#pragma region Volume Concepts
		/// Check if a type meets the requirements of being a basic 'volume' value
		concept struct Is_Volume_Value
		{
			check_typename_state(Is_Volume, Volume_tag, true_type);
			check_typename_state(Is_Length, Length_tag, true_type);
			define_default_check(Is_Volume && Is_Length);
		};
		/// Check if a type meets the requirements of being a basic 'Time' component
		concept struct Is_Volume_Component
		{
			check_concept(Is_Component,Is_Polaris_Component);
			check_concept(Is_Volume,Is_Volume_Value);
			define_default_check(Is_Component && Is_Volume);
		};
		concept struct Is_Volume_Prototype
		{
			check_concept(Is_Prototype,Is_Polaris_Prototype);
			check_typename_state(Is_Volume, Component_Type::Volume_tag, true_type);
			define_default_check(Is_Volume && Is_Prototype);
		};
		#pragma endregion

		#pragma region Time Concepts
		/// Check if the class has an arithmetic 'Time' member defined
		concept struct Is_Time_Value
		{
			check_typename_state(Has_Time,Time_tag,true_type);
			check_typename_state(Is_DRSeconds,DRSeconds_tag,true_type);
			check_typename_state(Is_Seconds,Seconds_tag,true_type);
			check_typename_state(Is_Minutes,Minutes_tag,true_type);
			check_typename_state(Is_Hours,Hours_tag,true_type);
			check_typename_state(Is_Days,Days_tag,true_type);
			define_default_check(Has_Time && (Is_DRSeconds || Is_Seconds || Is_Minutes || Is_Hours || Is_Days));
		};

		/// Check if a type meets the requirements of being a basic 'Time' component
		concept struct Is_Time_Component
		{
			check_concept(Is_Component,Is_Polaris_Component);
			check_concept(Is_Time, Is_Time_Value);
			define_default_check(Is_Component && Is_Time);
		};
		concept struct Is_Time_Prototype
		{
			check_concept(Is_Prototype,Is_Polaris_Prototype);
			check_typename_state(Has_Time, Component_Type::Time_tag, true_type);
			define_default_check(Is_Prototype && Has_Time);
		};
		#pragma endregion

		#pragma region Rate Concepts
		/// Check if a type meets the requirements of being a basic 'volume' value
		concept struct Is_Rate_Value
		{
			check_typename_state(Is_Rate, Rate_tag, true_type);
			check_concept(Is_Time, Concepts::Is_Time_Value);
			define_default_check(Is_Rate && Is_Time);
		};
		/// Check if a type meets the requirements of being a basic 'Time' component
		concept struct Is_Rate_Component
		{
			check_concept(Is_Component,Is_Polaris_Component);
			check_typename_state(Is_Rate, Rate_tag, true_type);
			define_default_check(Is_Component && Is_Rate);
		};
		concept struct Is_Rate_Prototype
		{
			check_concept(Is_Prototype,Is_Polaris_Prototype);
			check_typename_state(Is_Rate, Component_Type::Rate_tag, true_type);
			define_default_check(Is_Rate && Is_Prototype);
		};
		concept struct Is_Speed_Value
		{
			check_typename_state(Is_Speed, Speed_tag, true_type);
			check_concept(Is_Time, Concepts::Is_Time_Value);
			check_concept(Is_Length, Concepts::Is_Length_Value);
			define_default_check(Is_Speed && Is_Time && Is_Length);
		};
		/// Check if a type meets the requirements of being a basic 'Time' component
		concept struct Is_Speed_Component
		{
			check_concept(Is_Component,Is_Polaris_Component);
			check_typename_state(Is_Speed, Speed_tag, true_type);
			define_default_check(Is_Component && Is_Speed);
		};
		concept struct Is_Speed_Prototype
		{
			check_concept(Is_Prototype,Is_Polaris_Prototype);
			check_typename_state(Is_Speed, Component_Type::Speed_tag, true_type);
			define_default_check(Is_Speed && Is_Prototype);
		};
		#pragma endregion

		#pragma region Currency Concepts
		/// Check if the class has an arithmetic 'Time' member defined
		concept struct Is_Currency_Value
		{
			check_typename_state(Has_Currency,Currency_tag,true_type);
			check_typename_state(Is_Cents,Cents_tag,true_type);
			check_typename_state(Is_Dollars,Dollars_tag,true_type);
			check_typename_state(Is_Thousand_Dollars,Thousand_Dollars_tag,true_type);
			check_typename_state(Is_Million_Dollars,Million_Dollars_tag,true_type);	
			define_default_check(Has_Currency && (Is_Cents || Is_Dollars || Is_Thousand_Dollars || Is_Million_Dollars));
		};

		/// Check if a type meets the requirements of being a basic 'Time' component
		concept struct Is_Currency_Component
		{
			check_concept(Is_Component,Is_Polaris_Component);
			check_concept(Is_Currency, Is_Currency_Value);
			define_default_check(Is_Component && Is_Currency);
		};
		concept struct Is_Currency_Prototype
		{
			check_concept(Is_Prototype,Is_Polaris_Prototype);
			check_typename_state(Is_Currency, Component_Type::Currency_tag, true_type);
			define_default_check(Is_Prototype && Is_Currency);
		};
		#pragma endregion
	}

	namespace Prototypes
	{
		//------------------------------------------------------------------------------------------------------------------
		/// RENAME THE Inteface struct below.  This is the inteface to a POLARIS component
		//------------------------------------------------------------------------------------------------------------------
		prototype struct Length_Prototype
		{
			tag_as_prototype;

			define_get_set_exists_check(Value, Get_Value_exists, Set_Value_exists);
			template<typename ReturnValueType> ReturnValueType Value(requires_getter(check(ComponentType,Get_Value_exists) && check(ReturnValueType,Concepts::Is_Length_Value)))
			{
				return ReturnValueType(this_component()->template Value<ComponentType,CallerType,Value_Type>() * Conversion_Factor<ReturnValueType>());
			}
			template<typename ReturnValueType> ReturnValueType Value(requires_getter(check(ComponentType,!Get_Value_exists) || check(ReturnValueType,!Concepts::Is_Length_Value)))
			{
				assert_check(ComponentType,Get_Value_exists, "Getter does not exists for this accessor.");
				assert_check(ReturnValueType,Concepts::Is_Length_Value, "The specified TargetType is not a valid Length data structure.");
			}
			template<typename SetValueType> void Value(SetValueType value, requires_setter(check(ComponentType,Set_Value_exists) && check(SetValueType,Concepts::Is_Length_Value)))
			{
				this_component()->template Value<ComponentType,CallerType,Value_Type>(value / Conversion_Factor<SetValueType>());
			}
			template<typename SetValueType> void Value(SetValueType value, requires_setter(check(ComponentType,!Set_Value_exists) || check(SetValueType,!Concepts::Is_Length_Value)))
			{
				assert_check(ComponentType,Set_Value_exists, "Setter does not exists for this accessor.");
				assert_check(SetValueType,Concepts::Is_Length_Value, "The specified TargetType is not a valid Length data structure.");
			}

			feature_prototype static typename TargetType::ReturnType Convert_Value(typename TargetType::ParamType input_value, requires(check(TargetType,Is_Target_Type_Struct) && check(typename TargetType::ParamType,Concepts::Is_Length_Value) && check(typename TargetType::ReturnType,Concepts::Is_Length_Value)))
			{
				Value_Type convert_component_value_to_param = Conversion_Factor<typename TargetType::ParamType>();
				Value_Type convert_component_value_to_return = Conversion_Factor<typename TargetType::ReturnType>();
				return typename TargetType::ReturnType((Value_Type)(input_value.Value) * convert_component_value_to_return / convert_component_value_to_param);
			}
			feature_prototype static typename TargetType::ReturnType Convert_Value(typename TargetType::ParamType input_value, requires(check(TargetType,!Is_Target_Type_Struct) || check(typename TargetType::ParamType,!Concepts::Is_Length_Value) || check(typename TargetType::ReturnType,!Concepts::Is_Length_Value)))
			{
				assert_check(TargetType,Is_Target_Type_Struct, "TargetType is not a valid Target_Type_Struct structure.");
				assert_check(typename TargetType::ReturnType,Concepts::Is_Length_Value, "TargetTyp::ReturnType is not a valid length value structure.");
				assert_check(typename TargetType::ParamType,Concepts::Is_Length_Value, "TargetType::ParamType is not a valid length value structure.");
			}

			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Length_Value, Is_Centimeters) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Centimeters)))
			{
				return (Value_Type)1.0;
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Length_Value, Is_Feet) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Centimeters)))
			{		
				return (Value_Type)0.0328084;
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Length_Value, Is_Inches) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Centimeters)))
			{
				return (Value_Type)0.393701;				
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Length_Value, Is_Kilometers) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Centimeters)))
			{
				return (Value_Type)1.0 / (Value_Type)100000.0;
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Length_Value, Is_Meters) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Centimeters)))
			{
				return (Value_Type)1.0 / (Value_Type)100.0;
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Length_Value, Is_Miles) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Centimeters)))
			{
				return (Value_Type)(1.0 / 160934.0);
			};

			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Length_Value, Is_Centimeters) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Feet)))
			{
				return (Value_Type)(30.48);
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Length_Value, Is_Feet) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Feet)))
			{		
				return (Value_Type)(1.0);
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Length_Value, Is_Inches) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Feet)))
			{
				return (Value_Type)(12.0);				
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Length_Value, Is_Kilometers) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Feet)))
			{
				return (Value_Type)(1.0 / 3280.84);
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Length_Value, Is_Meters) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Feet)))
			{
				return (Value_Type)(0.3048);
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Length_Value, Is_Miles) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Feet)))
			{
				return (Value_Type)(1.0 / 5280.0);
			};

			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Length_Value, Is_Centimeters) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Inches)))
			{
				return (Value_Type)(2.54);
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Length_Value, Is_Feet) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Inches)))
			{		
				return (Value_Type)(1.0/12.0);
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Length_Value, Is_Inches) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Inches)))
			{
				return (Value_Type)(1.0);				
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Length_Value, Is_Kilometers) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Inches)))
			{
				return (Value_Type)(2.54/100000.0);
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Length_Value, Is_Meters) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Inches)))
			{
				return (Value_Type)(0.0254);
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Length_Value, Is_Miles) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Inches)))
			{
				return (Value_Type)(0.0833333 / 5280.0);
			};

			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Length_Value, Is_Centimeters) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Kilometers)))
			{
				return (Value_Type)(100000.0);
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Length_Value, Is_Feet) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Kilometers)))
			{		
				return (Value_Type)(3280.84);
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Length_Value, Is_Inches) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Kilometers)))
			{
				return (Value_Type)(39370.1);				
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Length_Value, Is_Kilometers) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Kilometers)))
			{
				return (Value_Type)(1.0);
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Length_Value, Is_Meters) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Kilometers)))
			{
				return (Value_Type)(1000.0);
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Length_Value, Is_Miles) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Kilometers)))
			{
				return (Value_Type)(0.621371);
			};
				
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Length_Value, Is_Centimeters) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Meters)))
			{
				return (Value_Type)(100.0);
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Length_Value, Is_Feet) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Meters)))
			{		
				return (Value_Type)(3.28084);
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Length_Value, Is_Inches) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Meters)))
			{
				return (Value_Type)(39.3701);				
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Length_Value, Is_Kilometers) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Meters)))
			{
				return (Value_Type)(0.001);
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Length_Value, Is_Meters) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Meters)))
			{
				return (Value_Type)(1.0);
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Length_Value, Is_Miles) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Meters)))
			{
				return (Value_Type)(0.000621371);
			};
					
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Length_Value, Is_Centimeters) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Miles)))
			{
				return (Value_Type)(160934.0);
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Length_Value, Is_Feet) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Miles)))
			{		
				return (Value_Type)(5280.0);
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Length_Value, Is_Inches) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Miles)))
			{
				return (Value_Type)(63360.0);				
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Length_Value, Is_Kilometers) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Miles)))
			{
				return (Value_Type)(1.60934);
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Length_Value, Is_Meters) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Miles)))
			{
				return (Value_Type)(1609.34);
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Length_Value, Is_Miles) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Miles)))
			{
				return (Value_Type)(1.0);
			};

			feature_prototype static Value_Type Conversion_Factor(requires(!(
				(sub_check(TargetType,Concepts::Is_Length_Value, Is_Centimeters) || sub_check(TargetType,Concepts::Is_Length_Value, Is_Feet) || sub_check(TargetType,Concepts::Is_Length_Value, Is_Inches) || 
				sub_check(TargetType,Concepts::Is_Length_Value, Is_Kilometers) || sub_check(TargetType,Concepts::Is_Length_Value, Is_Meters) || sub_check(TargetType,Concepts::Is_Length_Value, Is_Miles)) &&
				(sub_check(ComponentType,Concepts::Is_Length_Value, Is_Centimeters) || sub_check(ComponentType,Concepts::Is_Length_Value, Is_Feet) || sub_check(ComponentType,Concepts::Is_Length_Value, Is_Inches) || 
				sub_check(ComponentType,Concepts::Is_Length_Value, Is_Kilometers) || sub_check(ComponentType,Concepts::Is_Length_Value, Is_Meters) || sub_check(ComponentType,Concepts::Is_Length_Value, Is_Miles))) ))
			{
				assert_check(TargetType,Concepts::Is_Length_Value,"The specified TargetType is not a valid spatial measurement Data Structure.");
			}
		};

		prototype struct Width_Prototype : public Length_Prototype<ComponentType,CallerType>
		{
			feature_prototype TargetType Value()
			{
				return ((Length_Prototype<ComponentType,CallerType>*)this)->template Value<TargetType>();
			}

			feature_prototype void Value(TargetType value)
			{
				((Length_Prototype<ComponentType,CallerType>*)this)->template Value<TargetType>(value);
			}
		};

		prototype struct Height_Prototype : public Length_Prototype<ComponentType,CallerType>
		{
			feature_prototype TargetType Value()
			{
				return ((Length_Prototype<ComponentType,CallerType>*)this)->template Value<TargetType>();
			}

			feature_prototype void Value(TargetType value)
			{
				((Length_Prototype<ComponentType,CallerType>*)this)->template Value<TargetType>(value);
			}
		};

		prototype struct Area_Prototype : public Length_Prototype<ComponentType, CallerType>
		{
			typedef Length_Prototype<ComponentType, CallerType> base_type;
			template<typename ReturnValueType> ReturnValueType Value(requires_getter(check(ReturnValueType,Concepts::Is_Area_Value)))
			{		
				return ((Length_Prototype<ComponentType,CallerType>*)this)->template Value<ReturnValueType>()*((Length_Prototype<ComponentType,CallerType>*)this)->template Conversion_Factor<ReturnValueType>();
			}
			template<typename ReturnValueType> ReturnValueType Value(requires_getter(check(ReturnValueType,!Concepts::Is_Area_Value)))
			{
				assert_check(ReturnValueType,Concepts::Is_Area_Value,"Your target type is not identified as an area measure.");
			}

			template<typename SetValueType> void Value(SetValueType value, requires_setter(check(SetValueType,Concepts::Is_Area_Value)))
			{
				((Length_Prototype<ComponentType,CallerType>*)this)->template Value<SetValueType>(value / ((Length_Prototype<ComponentType,CallerType>*)this)->template Conversion_Factor<SetValueType>());
			}
			template<typename SetValueType> void Value(SetValueType value, requires_setter(check(SetValueType,!Concepts::Is_Area_Value)))
			{
				assert_check(SetValueType,Concepts::Is_Area_Value,"Your target type is not identified as an area measure.");
			}

			feature_prototype static typename TargetType::ReturnType Convert_Value(typename TargetType::ParamType input_value, requires(check(TargetType,Is_Target_Type_Struct) && check(typename TargetType::ParamType,Concepts::Is_Area_Value) && check(typename TargetType::ReturnType,Concepts::Is_Area_Value)))
			{
				Value_Type convert_component_value_to_param = base_type::template Conversion_Factor<typename TargetType::ParamType>();
				Value_Type convert_component_value_to_return = base_type::template Conversion_Factor<typename TargetType::ReturnType>();
				Value_Type conversion = convert_component_value_to_return / convert_component_value_to_param;
				return typename TargetType::ReturnType((Value_Type)(input_value.Value) * pow(conversion,2.0));
			}
			feature_prototype static typename TargetType::ReturnType Convert_Value(typename TargetType::ParamType input_value, requires(check(TargetType,!Is_Target_Type_Struct) || check(typename TargetType::ParamType,!Concepts::Is_Area_Value) || check(typename TargetType::ReturnType,!Concepts::Is_Area_Value)))
			{
				assert_check(TargetType,Is_Target_Type_Struct, "TargetType is not a valid Target_Type_Struct structure.");
				assert_check(typename TargetType::ReturnType,Concepts::Is_Area_Value, "TargetType::ReturnType is not a valid area value structure.");
				assert_check(typename TargetType::ParamType,Concepts::Is_Area_Value, "TargetType::ParamType is not a valid area value structure.");
			}
		};

		prototype struct Volume_Prototype : protected  Length_Prototype<ComponentType, CallerType>
		{
			typedef Length_Prototype<ComponentType, CallerType> base_type;

			template<typename ReturnValueType>  ReturnValueType Value(requires_getter(check(ReturnValueType,Concepts::Is_Volume_Value)))
			{		
				return ((Length_Prototype<ComponentType,CallerType>*)this)->template Value<ReturnValueType>()*((Length_Prototype<ComponentType,CallerType>*)this)->template Conversion_Factor<ReturnValueType>()*((Length_Prototype<ComponentType,CallerType>*)this)->template Conversion_Factor<ReturnValueType>();
			}
			template<typename ReturnValueType>  ReturnValueType Value(requires_getter(check(ReturnValueType,!Concepts::Is_Volume_Value)))
			{
				assert_check(ReturnValueType,Concepts::Is_Volume_Value,"Your target type is not identified as a volume measure.");
			}

			template<typename SetValueType>  void Value(SetValueType value,requires_setter(check(SetValueType,Concepts::Is_Volume_Value)))
			{
				((Length_Prototype<ComponentType,CallerType>*)this)->template Value<SetValueType>(value / ((Length_Prototype<ComponentType,CallerType>*)this)->template Conversion_Factor<SetValueType>() / ((Length_Prototype<ComponentType,CallerType>*)this)->template Conversion_Factor<SetValueType>());
			}
			template<typename SetValueType>  void Value(SetValueType value, requires_setter(check(SetValueType,!Concepts::Is_Volume_Value)))
			{
				assert_check(SetValueType,Concepts::Is_Volume_Value,"Your target type is not identified as a volume measure.");
			}

			feature_prototype static typename TargetType::ReturnType Convert_Value(typename TargetType::ParamType input_value, requires(check(TargetType,Is_Target_Type_Struct) && check(typename TargetType::ParamType,Concepts::Is_Volume_Value) && check(typename TargetType::ReturnType,Concepts::Is_Volume_Value)))
			{
				Value_Type convert_component_value_to_param = base_type::template Conversion_Factor<typename TargetType::ParamType>();
				Value_Type convert_component_value_to_return = base_type::template Conversion_Factor<typename TargetType::ReturnType>();
				Value_Type conversion = convert_component_value_to_return / convert_component_value_to_param;
				return typename TargetType::ReturnType((Value_Type)(input_value.Value) * pow(conversion,3.0));
			}
			feature_prototype static typename TargetType::ReturnType Convert_Value(typename TargetType::ParamType input_value, requires(check(TargetType,!Is_Target_Type_Struct) || check(typename TargetType::ParamType,!Concepts::Is_Volume_Value) || check(typename TargetType::ParamType,!Concepts::Is_Volume_Value)))
			{
				assert_check(TargetType,Is_Target_Type_Struct, "TargetType is not a valid Target_Type_Struct structure.");
				assert_check(typename TargetType::ReturnType,Concepts::Is_Volume_Value, "TargetType::ReturnType is not a valid volume value structure.");
				assert_check(typename TargetType::ParamType,Concepts::Is_Volume_Value, "TargetType::ParamType is not a valid volume value structure.");
			}
		};

		prototype struct Time_Prototype
		{
			tag_as_prototype;

			define_get_set_exists_check(Value,Get_Value_exists, Set_Value_exists);
			template<typename ReturnValueType> ReturnValueType Value(requires_getter(check(ComponentType,Get_Value_exists) && check(ReturnValueType,Concepts::Is_Time_Value)))
			{
				return ReturnValueType(this_component()->template Value<ComponentType,CallerType,Value_Type>() * Conversion_Factor<ReturnValueType>());
			}
			template<typename ReturnValueType> ReturnValueType Value(requires_getter(check(ComponentType,!Get_Value_exists) || check(ReturnValueType,!Concepts::Is_Time_Value)))
			{
				assert_check(ComponentType,Get_Value_exists, "Getter does not exists for this accessor.");
				assert_check(ReturnValueType,Concepts::Is_Time_Value, "The specified TargetType is not a valid Time data structure.");
			}
			template<typename SetValueType> void Value(SetValueType value, requires_setter(check(ComponentType,Set_Value_exists) && check(SetValueType,Concepts::Is_Time_Value)))
			{
				this_component()->template Value<ComponentType,CallerType,Value_Type>(value / Conversion_Factor<SetValueType>());
			}
			template<typename SetValueType> void Value(SetValueType value, requires_setter(check(ComponentType,!Set_Value_exists) || check(SetValueType,!Concepts::Is_Time_Value)))
			{
				assert_check(ComponentType,Set_Value_exists, "Setter does not exists for this accessor.");
				assert_check(SetValueType,Concepts::Is_Time_Value, "The specified TargetType is not a valid Time data structure.");
			}
			
			feature_prototype static typename TargetType::ReturnType Convert_Value(typename TargetType::ParamType input_value, requires(check(TargetType,Is_Target_Type_Struct) && check(typename TargetType::ParamType,Concepts::Is_Time_Value) && check(typename TargetType::ReturnType,Concepts::Is_Time_Value)))
			{
				Value_Type convert_component_value_to_param = Time_Prototype::Conversion_Factor<typename TargetType::ParamType>();
				Value_Type convert_component_value_to_return = Time_Prototype::Conversion_Factor<typename TargetType::ReturnType>();
				return typename TargetType::ReturnType((Value_Type)(input_value.Value) * convert_component_value_to_return / convert_component_value_to_param);
			}
			feature_prototype static typename TargetType::ReturnType Convert_Value(typename TargetType::ParamType input_value, requires(check(TargetType,!Is_Target_Type_Struct) || check(typename TargetType::ParamType,!Concepts::Is_Time_Value) || check(typename TargetType::ReturnType,!Concepts::Is_Time_Value)))
			{
				assert_check(TargetType,Is_Target_Type_Struct,"TargetType is not a valid target type struct.");
				assert_check(typename TargetType::ParamType,Concepts::Is_Time_Value,"TargetType::ParamType is not a valid Time type.");
				assert_check(typename TargetType::ReturnType,Concepts::Is_Time_Value,"TargetType::ReturnType is not a valid Time type.");
			}

			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Time_Value, Is_Days) && sub_check(ComponentType,Concepts::Is_Time_Value, Is_Days)))
			{
				return (Value_Type)(1.0);
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Time_Value, Is_DRSeconds) && sub_check(ComponentType,Concepts::Is_Time_Value, Is_Days)))
			{
				return (Value_Type)(864000.0);
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Time_Value, Is_Hours) && sub_check(ComponentType,Concepts::Is_Time_Value, Is_Days)))
			{
				return (Value_Type)(24.0);
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Time_Value, Is_Minutes) && sub_check(ComponentType,Concepts::Is_Time_Value, Is_Days)))
			{
				return (Value_Type)(1440.0);
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Time_Value, Is_Seconds) && sub_check(ComponentType,Concepts::Is_Time_Value, Is_Days)))
			{
				return (Value_Type)(86400.0);
			};

			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Time_Value, Is_Days) && sub_check(ComponentType,Concepts::Is_Time_Value, Is_DRSeconds)))
			{
				return (Value_Type)(1.0 / 864000.0);
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Time_Value, Is_DRSeconds) && sub_check(ComponentType,Concepts::Is_Time_Value, Is_DRSeconds)))
			{
				return (Value_Type)(1.0);
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Time_Value, Is_Hours) && sub_check(ComponentType,Concepts::Is_Time_Value, Is_DRSeconds)))
			{
				return (Value_Type)(1.0/36000.0);
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Time_Value, Is_Minutes) && sub_check(ComponentType,Concepts::Is_Time_Value, Is_DRSeconds)))
			{
				return (Value_Type)(1.0/600.0);
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Time_Value, Is_Seconds) && sub_check(ComponentType,Concepts::Is_Time_Value, Is_DRSeconds)))
			{
				return (Value_Type)(0.1);
			};

			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Time_Value, Is_Days) && sub_check(ComponentType,Concepts::Is_Time_Value, Is_Hours)))
			{
				return (Value_Type)(1.0/24.0);
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Time_Value, Is_DRSeconds) && sub_check(ComponentType,Concepts::Is_Time_Value, Is_Hours)))
			{
				return (Value_Type)(36000.0);
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Time_Value, Is_Hours) && sub_check(ComponentType,Concepts::Is_Time_Value, Is_Hours)))
			{
				return (Value_Type)(1.0);
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Time_Value, Is_Minutes) && sub_check(ComponentType,Concepts::Is_Time_Value, Is_Hours)))
			{
				return (Value_Type)(60.0);
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Time_Value, Is_Seconds) && sub_check(ComponentType,Concepts::Is_Time_Value, Is_Hours)))
			{
				return (Value_Type)(3600.0);
			};

			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Time_Value, Is_Days) && sub_check(ComponentType,Concepts::Is_Time_Value, Is_Minutes)))
			{
				return (Value_Type)(1.0/1440.0);
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Time_Value, Is_DRSeconds) && sub_check(ComponentType,Concepts::Is_Time_Value, Is_Minutes)))
			{
				return (Value_Type)(600.0);
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Time_Value, Is_Hours) && sub_check(ComponentType,Concepts::Is_Time_Value, Is_Minutes)))
			{
				return (Value_Type)(1.0/60.0);
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Time_Value, Is_Minutes) && sub_check(ComponentType,Concepts::Is_Time_Value, Is_Minutes)))
			{
				return (Value_Type)(1.0);
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Time_Value, Is_Seconds) && sub_check(ComponentType,Concepts::Is_Time_Value, Is_Minutes)))
			{
				return (Value_Type)(60.0);
			};

			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Time_Value, Is_Days) && sub_check(ComponentType,Concepts::Is_Time_Value, Is_Seconds)))
			{
				return (Value_Type)(1.0/ 86400.0);
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Time_Value, Is_DRSeconds) && sub_check(ComponentType,Concepts::Is_Time_Value, Is_Seconds)))
			{
				return (Value_Type)(10.0);
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Time_Value, Is_Hours) && sub_check(ComponentType,Concepts::Is_Time_Value, Is_Seconds)))
			{
				return (Value_Type)(1.0/3600.0);
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Time_Value, Is_Minutes) && sub_check(ComponentType,Concepts::Is_Time_Value, Is_Seconds)))
			{
				return (Value_Type)(1.0/60.0);
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Time_Value, Is_Seconds) && sub_check(ComponentType,Concepts::Is_Time_Value, Is_Seconds)))
			{
				return (Value_Type)(1.0);
			};
			//================================================================================================
			// Time value accessors for:
			//------------------------------------------------------------------------------------------------
			/// Time component accessor - returns the requested portion of the current time
			feature_accessor(Time_Component,none,none);
			/// Display the time
			feature_prototype void Write()
			{
				//cout <<"Total Seconds: "<<pthis->Total_Seconds<ComponentType, CallerType, TargetType>()<<endl;
				cout << "Day "<< this->Time_Component<Time_Days>()<<":  ";
				cout << this->Time_Component<Time_Hours>()<<":"<<this->Time_Component<Time_Minutes>()<<":"<<this->Time_Component<Time_Seconds>()<<"."<< this->Time_Component<Time_DRSeconds>();
			}
		};

		prototype struct Rate_Prototype : protected Time_Prototype<ComponentType,CallerType>
		{
			tag_as_prototype;
			typedef Time_Prototype<ComponentType, CallerType> base_type;

			define_get_set_exists_check(Value,Get_Value_exists, Set_Value_exists);
			template<typename ReturnValueType> ReturnValueType Value(requires_getter(check(ComponentType,Get_Value_exists) && check(ReturnValueType,Concepts::Is_Time_Value)))
			{
				ReturnValueType val = base_type::template Conversion_Factor<ReturnValueType>();
				return ReturnValueType(this_component()->template Value<ComponentType,CallerType,Value_Type>() / val);
			}
			template<typename ReturnValueType> ReturnValueType Value(requires_getter(check(ComponentType,!Get_Value_exists) || check(ReturnValueType,!Concepts::Is_Time_Value)))
			{
				assert_check(ComponentType,Get_Value_exists, "Getter does not exists for this accessor.");
				assert_check(ReturnValueType,Concepts::Is_Length_Value, "The specified TargetType is not a valid Time data structure.");
			}
			template<typename SetValueType> void Value(SetValueType value, requires_setter(check(ComponentType,Set_Value_exists) && check(SetValueType,Concepts::Is_Time_Value)))
			{
				this_component()->template Value<ComponentType,CallerType,Value_Type>(value * base_type::template Conversion_Factor<SetValueType>());
			}
			template<typename SetValueType> void Value(SetValueType value, requires_setter(check(ComponentType,!Set_Value_exists) || check(SetValueType,!Concepts::Is_Time_Value)))
			{
				assert_check(ComponentType,Set_Value_exists, "Setter does not exists for this accessor.");
				assert_check(SetValueType,Concepts::Is_Length_Value, "The specified TargetType is not a valid Time data structure.");
			}
			
			feature_prototype static typename TargetType::ReturnType Convert_Value(typename TargetType::ParamType input_value, requires(check(TargetType,Is_Target_Type_Struct) && check(typename TargetType::ParamType,Concepts::Is_Time_Value) && check(typename TargetType::ReturnType,Concepts::Is_Time_Value)))
			{
				Value_Type convert_component_value_to_param = base_type::template Conversion_Factor<typename TargetType::ParamType>();
				Value_Type convert_component_value_to_return = base_type::template Conversion_Factor<typename TargetType::ReturnType>();
				return typename TargetType::ReturnType((Value_Type)(input_value.Value) / convert_component_value_to_return * convert_component_value_to_param);
			}
			feature_prototype static typename TargetType::ReturnType Convert_Value(typename TargetType::ParamType input_value, requires(check(TargetType,!Is_Target_Type_Struct) || check(typename TargetType::ParamType,!Concepts::Is_Time_Value) || check(typename TargetType::ReturnType,!Concepts::Is_Time_Value)))
			{
				assert_check(TargetType,Is_Target_Type_Struct,"TargetType is not a valid target type struct.");
				assert_check(typename TargetType::ParamType,Concepts::Is_Time_Value,"TargetType::ParamType is not a valid Time type.");
				assert_check(typename TargetType::ReturnType,Concepts::Is_Time_Value,"TargetType::ReturnType is not a valid Time type.");
			}
		};

		prototype struct Speed_Prototype : protected Time_Prototype<ComponentType,CallerType>, protected Length_Prototype<ComponentType,CallerType>
		{
			tag_as_prototype;
			typedef Length_Prototype<ComponentType, CallerType> length_base;
			typedef Time_Prototype<ComponentType, CallerType> time_base;

			define_get_set_exists_check(Value,Get_Value_exists, Set_Value_exists);
			template<typename ReturnValueType> ReturnValueType Value(requires_getter(check(ComponentType,Get_Value_exists) && check(ComponentType, Concepts::Is_Speed_Component) && check(ReturnValueType,Concepts::Is_Speed_Value)))
			{
				ReturnValueType len_val = length_base::template Conversion_Factor<ReturnValueType>();
				ReturnValueType time_val = time_base::template Conversion_Factor<ReturnValueType>();
				return ReturnValueType(this_component()->template Value<ComponentType,CallerType,Value_Type>() * len_val / time_val );
			}
			template<typename ReturnValueType> ReturnValueType Value(requires_getter(!check(ComponentType,Get_Value_exists) || !check(ComponentType, Concepts::Is_Speed_Component) || !check(ReturnValueType,Concepts::Is_Speed_Value)))
			{
				assert_check(ComponentType,Get_Value_exists, "Getter does not exists for this accessor.");
				assert_check(ReturnValueType,Concepts::Is_Speed_Value, "The specified TargetType is not a valid Speed data structure, ensure that TargetType has tags: {Speed_Type, Length_Type and Time_Type}");
				assert_check(ComponentType,Concepts::Is_Speed_Component, "The specified ComponentType is not a valid Speed component, ensure that ComponentType is tagged as a Speed_Type and has Value member}");
			}
			template<typename SetValueType> void Value(SetValueType value, requires_setter(check(ComponentType,Set_Value_exists) && check(ComponentType, Concepts::Is_Speed_Component) && check(SetValueType,Concepts::Is_Speed_Value)))
			{
				SetValueType len_val = length_base::template Conversion_Factor<SetValueType>();
				SetValueType time_val = time_base::template Conversion_Factor<SetValueType>();
				this_component()->template Value<ComponentType,CallerType,Value_Type>(Value_Type(value * time_val / len_val));
			}
			template<typename SetValueType> void Value(SetValueType value, requires_setter(!check(ComponentType,Set_Value_exists) || !check(ComponentType, Concepts::Is_Speed_Component) || !check(SetValueType,Concepts::Is_Speed_Value)))
			{
				assert_check(ComponentType,Set_Value_exists, "Setter does not exists for this accessor.");
				assert_check(SetValueType,Concepts::Is_Speed_Value, "The specified TargetType is not a valid Speed data structure, ensure that TargetType has tags: {Speed_Type, Length_Type and Time_Type}");
				assert_sub_check(ComponentType,Is_Polaris_Component,has_this_type, "Doesn't have This_Type");
				assert_sub_check(ComponentType,Is_Polaris_Component,has_parent_type,"Doesn't have Parent_Type");
				assert_sub_check(ComponentType,Is_Polaris_Component,has_group_list,"Doesn't have Group_List");
				assert_sub_check(ComponentType,Is_Polaris_Component,has_object_type,"Doesn't have Object_Type");
				assert_sub_check(ComponentType,Is_Polaris_Component,has_master_type,"Doesn't have Master_Type");
				assert_sub_check(ComponentType,Concepts::Is_Speed_Component,Is_Speed,"The CompentType is not tagged with Speed_tag.");
				assert_check(ComponentType,Concepts::Is_Speed_Component, "The specified ComponentType is not a valid Speed component, ensure that ComponentType is tagged as a Speed_Type and has Value member}");


			}
			
			feature_prototype static typename TargetType::ReturnType Convert_Value(typename TargetType::ParamType input_value, requires(check(TargetType,Is_Target_Type_Struct) && check(typename TargetType::ParamType,Concepts::Is_Time_Value) && check(typename TargetType::ReturnType,Concepts::Is_Time_Value)))
			{
				Value_Type convert_component_value_to_param = time_base::template Conversion_Factor<typename TargetType::ParamType>();
				Value_Type convert_component_value_to_return = time_base::template Conversion_Factor<typename TargetType::ReturnType>();
				return typename TargetType::ReturnType((Value_Type)(input_value.Value) / convert_component_value_to_return * convert_component_value_to_param);
			}
			feature_prototype static typename TargetType::ReturnType Convert_Value(typename TargetType::ParamType input_value, requires(check(TargetType,!Is_Target_Type_Struct) || check(typename TargetType::ParamType,!Concepts::Is_Time_Value) || check(typename TargetType::ReturnType,!Concepts::Is_Time_Value)))
			{
				assert_check(TargetType,Is_Target_Type_Struct,"TargetType is not a valid target type struct.");
				assert_check(typename TargetType::ParamType,Concepts::Is_Time_Value,"TargetType::ParamType is not a valid Time type.");
				assert_check(typename TargetType::ReturnType,Concepts::Is_Time_Value,"TargetType::ReturnType is not a valid Time type.");
			}
		};
		
		prototype struct Currency_Prototype
		{
			tag_as_prototype;

			define_get_set_exists_check(Value, Get_Value_exists, Set_Value_exists);
			template<typename ReturnValueType> ReturnValueType Value(requires_getter(check(ComponentType,Get_Value_exists) && check(ReturnValueType,Concepts::Is_Currency_Value)))
			{
				return ReturnValueType(this_component()->template Value<ComponentType,CallerType,Value_Type>() * Conversion_Factor<ReturnValueType>());
			}
			template<typename ReturnValueType> ReturnValueType Value(requires_getter(check(ComponentType,!Get_Value_exists) || check(ReturnValueType,!Concepts::Is_Currency_Value)))
			{
				assert_check(ComponentType,Get_Value_exists, "Getter does not exists for this accessor.");
				assert_check(ReturnValueType,Concepts::Is_Length_Value, "The specified TargetType is not a valid Length data structure.");
			}
			template<typename SetValueType> void Value(SetValueType value, requires_setter(check(ComponentType,Set_Value_exists) && check(SetValueType,Concepts::Is_Currency_Value)))
			{
				this_component()->template Value<ComponentType,CallerType,Value_Type>(value / Conversion_Factor<SetValueType>());
			}
			template<typename SetValueType> void Value(SetValueType value, requires_setter(check(ComponentType,!Set_Value_exists) || check(SetValueType,!Concepts::Is_Currency_Value)))
			{
				assert_check(ComponentType,Set_Value_exists, "Setter does not exists for this accessor.");
				assert_check(SetValueType,Concepts::Is_Length_Value, "The specified TargetType is not a valid Length data structure.");
			}

			feature_prototype static typename TargetType::ReturnType Convert_Value(typename TargetType::ParamType input_value, requires(check(TargetType,Is_Target_Type_Struct) && check(typename TargetType::ParamType,Concepts::Is_Currency_Value) && check(typename TargetType::ReturnType,Concepts::Is_Currency_Value)))
			{
				Value_Type convert_component_value_to_param = Conversion_Factor<typename TargetType::ParamType>();
				Value_Type convert_component_value_to_return = Conversion_Factor<typename TargetType::ReturnType>();
				return typename TargetType::ReturnType((Value_Type)(input_value.Value) * convert_component_value_to_return / convert_component_value_to_param);
			}
			feature_prototype static typename TargetType::ReturnType Convert_Value(typename TargetType::ParamType input_value, requires(check(TargetType,!Is_Target_Type_Struct) || check(typename TargetType::ParamType,!Concepts::Is_Currency_Value) || check(typename TargetType::ReturnType,!Concepts::Is_Currency_Value)))
			{
				assert_check(TargetType,Is_Target_Type_Struct, "TargetType is not a valid Target_Type_Struct structure.");
				assert_check(typename TargetType::ReturnType,Concepts::Is_Length_Value, "TargetTyp::ReturnType is not a valid length value structure.");
				assert_check(typename TargetType::ParamType,Concepts::Is_Length_Value, "TargetType::ParamType is not a valid length value structure.");
			}

			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Currency_Value, Is_Cents) && sub_check(ComponentType,Concepts::Is_Currency_Value, Is_Cents)))
			{
				return (Value_Type)1.0;
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Currency_Value, Is_Dollars) && sub_check(ComponentType,Concepts::Is_Currency_Value, Is_Cents)))
			{		
				return (Value_Type)0.01;
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Currency_Value, Is_Thousand_Dollars) && sub_check(ComponentType,Concepts::Is_Currency_Value, Is_Cents)))
			{
				return (Value_Type)0.00001;				
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Currency_Value, Is_Million_Dollars) && sub_check(ComponentType,Concepts::Is_Currency_Value, Is_Cents)))
			{
				return (Value_Type)0.00000001;
			};

			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Currency_Value, Is_Cents) && sub_check(ComponentType,Concepts::Is_Currency_Value, Is_Dollars)))
			{
				return (Value_Type)(100.0);
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Currency_Value, Is_Dollars) && sub_check(ComponentType,Concepts::Is_Currency_Value, Is_Dollars)))
			{		
				return (Value_Type)(1.0);
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Currency_Value, Is_Thousand_Dollars) && sub_check(ComponentType,Concepts::Is_Currency_Value, Is_Dollars)))
			{
				return (Value_Type)(0.001);				
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Currency_Value, Is_Million_Dollars) && sub_check(ComponentType,Concepts::Is_Currency_Value, Is_Dollars)))
			{
				return (Value_Type)(0.000001);
			};

			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Currency_Value, Is_Cents) && sub_check(ComponentType,Concepts::Is_Currency_Value, Is_Thousand_Dollars)))
			{
				return (Value_Type)(100000.0);
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Currency_Value, Is_Dollars) && sub_check(ComponentType,Concepts::Is_Currency_Value, Is_Thousand_Dollars)))
			{		
				return (Value_Type)(1000.0);
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Currency_Value, Is_Thousand_Dollars) && sub_check(ComponentType,Concepts::Is_Currency_Value, Is_Thousand_Dollars)))
			{
				return (Value_Type)(1.0);				
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Currency_Value, Is_Million_Dollars) && sub_check(ComponentType,Concepts::Is_Currency_Value, Is_Thousand_Dollars)))
			{
				return (Value_Type)(0.001);
			};

			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Currency_Value, Is_Cents) && sub_check(ComponentType,Concepts::Is_Currency_Value, Is_Million_Dollars)))
			{
				return (Value_Type)(100000000.0);
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Currency_Value, Is_Dollars) && sub_check(ComponentType,Concepts::Is_Currency_Value, Is_Million_Dollars)))
			{		
				return (Value_Type)(1000000.0);
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Currency_Value, Is_Thousand_Dollars) && sub_check(ComponentType,Concepts::Is_Currency_Value, Is_Million_Dollars)))
			{
				return (Value_Type)(1000.0);				
			};
			feature_prototype static Value_Type Conversion_Factor(requires(sub_check(TargetType,Concepts::Is_Currency_Value, Is_Million_Dollars) && sub_check(ComponentType,Concepts::Is_Currency_Value, Is_Million_Dollars)))
			{
				return (Value_Type)(1.0);
			};


			feature_prototype static Value_Type Conversion_Factor(requires(!(
				(sub_check(TargetType,Concepts::Is_Currency_Value, Is_Cents) || sub_check(TargetType,Concepts::Is_Currency_Value, Is_Dollars) || sub_check(TargetType,Concepts::Is_Currency_Value, Is_Thousand_Dollars) || 
				sub_check(TargetType,Concepts::Is_Currency_Value, Is_Million_Dollars)) &&
				(sub_check(ComponentType,Concepts::Is_Currency_Value, Is_Cents) || sub_check(ComponentType,Concepts::Is_Currency_Value, Is_Dollars) || sub_check(ComponentType,Concepts::Is_Currency_Value, Is_Thousand_Dollars) || 
				sub_check(ComponentType,Concepts::Is_Currency_Value, Is_Million_Dollars))) ))
			{
				assert_check(TargetType,Concepts::Is_Currency_Value,"The specified TargetType is not a valid spatial measurement Data Structure.");
			}
		};
	}

}

using namespace Basic_Units::Prototypes;