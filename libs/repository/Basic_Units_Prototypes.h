#pragma once
#include "Dependencies.h"

//template<typename TargetType, typename InputType>
//TargetType Round(InputType value, requires(check(TargetType, is_integral) && check(InputType,is_arithmetic) && (check(InputType,!is_integral))))
//{
//	InputType remain = value - (InputType)((TargetType)value);
//	if (remain >= 0.5) return (TargetType)(value - remain + 1);
//	else return (TargetType)(value - remain);
//}
//template<typename TargetType, typename InputType>
//TargetType Round(InputType value, requires(check(TargetType, !is_integral) || check(InputType,!is_arithmetic) || (check(InputType,is_integral))))
//{
//	assert_check(TargetType,is_integral,"ReturnType must be integral");
//	assert_check(InputType,is_arithmetic,"InputType must be arithmetic");
//	assert_check(InputType,!is_integral,"InputType must be floating point type");
//}

namespace polaris{

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
		concept struct Is_Polaris_Component
		{
			// Basic check to satisfy polaris component requirements
			check_typedef_name(has_parent_type,Parent_Type);
			check_typedef_name(has_group_list,Group_List);
			check_typedef_name(has_object_type,Object_Type);
			check_typedef_name(has_master_type,Master_Type);

			// subchecks which can be used to identify object type
			check_typedef_type(is_data_object,Object_Type, Data_Object);
			check_typedef_type(is_execution_object,Object_Type, Execution_Object);
	
			// check to make sure it is not a debug version of prototype (i.e. debug prototypes will satisfy all of the above checks due to inheriting from COmponent_Type
			check_typedef_type(has_prototype_tag,Is_Prototype, true_type);

			// default check when concept is used
			define_default_check(!has_prototype_tag && has_parent_type && has_group_list && has_object_type && has_master_type && (is_data_object || is_execution_object));
		};


		concept struct Is_Polaris_Prototype
		{
			check_typedef_name(has_component_type,Component_Type);
			check_typedef_name(has_caller_type,Caller_Type);
			check_typedef_type(has_prototype_tag,Is_Prototype, true_type);
			define_default_check(has_component_type && has_caller_type && has_prototype_tag);
		};

		concept struct Has_Value
		{
			check_template_method_name(has_value,Value);
			define_default_check(has_value);
		};
		/*concept struct Has_Getter
		{
			check_template_method_type(has_getter,Value,concat(Member_Function_Signature<T,NT>),NT);
			define_default_check(has_getter);
		};
		concept struct Has_Setter
		{
			check_template_method_type(has_setter,Value,concat(Member_Function_Signature<T,void,NT>),NT);
			define_default_check(has_setter);
		};*/
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
			check_concept(Is_Component,Is_Polaris_Component,T,V);
			check_concept(Is_Length, Is_Length_Value,T,V);
			define_default_check(Is_Component && Is_Length);
		};
		concept struct Is_Length_Prototype
		{
			check_concept(Is_Prototype,Is_Polaris_Prototype,T,V);
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
			check_concept(Is_Component,Is_Polaris_Component,T,V);
			check_concept(Is_Area, Is_Area_Value,T,V);
			define_default_check(Is_Component && Is_Area);
		};
		concept struct Is_Area_Prototype
		{
			check_concept(Is_Prototype,Is_Polaris_Prototype,T,V);
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
			check_concept(Is_Component,Is_Polaris_Component,T,V);
			check_concept(Is_Volume,Is_Volume_Value,T,V);
			define_default_check(Is_Component && Is_Volume);
		};
		concept struct Is_Volume_Prototype
		{
			check_concept(Is_Prototype,Is_Polaris_Prototype,T,V);
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
			check_concept(Is_Component,Is_Polaris_Component,T,V);
			check_concept(Is_Time, Is_Time_Value,T,V);
			define_default_check(Is_Component && Is_Time);
		};
		concept struct Is_Time_Prototype
		{
			check_concept(Is_Prototype,Is_Polaris_Prototype,T,V);
			check_typedef_type(Has_Time, Component_Type::Time_tag, true_type);
			define_default_check(Is_Prototype && Has_Time);
		};
		#pragma endregion

		#pragma region Rate Concepts
		/// Check if a type meets the requirements of being a basic 'volume' value
		concept struct Is_Rate_Value
		{
			check_typedef_type(Is_Rate, Rate_tag, true_type);
			check_concept(Is_Time, Concepts::Is_Time_Value,T,V);
			define_default_check(Is_Rate && Is_Time);
		};
		/// Check if a type meets the requirements of being a basic 'Time' component
		concept struct Is_Rate_Component
		{
			check_concept(Is_Component,Is_Polaris_Component,T,V);
			check_typedef_type(Is_Rate, Rate_tag, true_type);
			define_default_check(Is_Component && Is_Rate);
		};
		concept struct Is_Rate_Prototype
		{
			check_concept(Is_Prototype,Is_Polaris_Prototype,T,V);
			check_typedef_type(Is_Rate, Component_Type::Rate_tag, true_type);
			define_default_check(Is_Rate && Is_Prototype);
		};
		concept struct Is_Speed_Value
		{
			check_typedef_type(Is_Speed, Speed_tag, true_type);
			check_concept(Is_Time, Concepts::Is_Time_Value,T,V);
			check_concept(Is_Length, Concepts::Is_Length_Value,T,V);
			define_default_check(Is_Speed && Is_Time && Is_Length);
		};
		/// Check if a type meets the requirements of being a basic 'Time' component
		concept struct Is_Speed_Component
		{
			check_concept(Is_Component,Is_Polaris_Component,T,V);
			check_typedef_type(Is_Speed, Speed_tag, true_type);
			define_default_check(Is_Component && Is_Speed);
		};
		concept struct Is_Speed_Prototype
		{
			check_concept(Is_Prototype,Is_Polaris_Prototype,T,V);
			check_typedef_type(Is_Speed, Component_Type::Speed_tag, true_type);
			define_default_check(Is_Speed && Is_Prototype);
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
			check_concept(Is_Component,Is_Polaris_Component,T,V);
			check_concept(Is_Currency, Is_Currency_Value,T,V);
			define_default_check(Is_Component && Is_Currency);
		};
		concept struct Is_Currency_Prototype
		{
			check_concept(Is_Prototype,Is_Polaris_Prototype,T,V);
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
		prototype struct Length_Prototype
		{
			tag_as_prototype;

			//define_get_set_exists_check(Value, Get_Value_exists, Set_Value_exists);
			template<typename ReturnValueType> ReturnValueType Value(requires(ReturnValueType,check(ComponentType,Concepts::Has_Value) && check(ReturnValueType,Concepts::Is_Length_Value)))
			{
				return ReturnValueType(this_component()->template Value<ComponentType,CallerType,Value_Type>() * Conversion_Factor<ReturnValueType>());
			}
			template<typename ReturnValueType> ReturnValueType Value(requires(ReturnValueType,check(ComponentType,!Concepts::Has_Value) || check(ReturnValueType,!Concepts::Is_Length_Value)))
			{
				assert_default_check(ComponentType,Concepts::Has_Value, "Getter does not exists for this accessor.");
				assert_default_check(ReturnValueType,Concepts::Is_Length_Value, "The specified TargetType is not a valid Length data structure.");
			}
			template<typename SetValueType> void Value(SetValueType value, requires(SetValueType,check(ComponentType,Concepts::Has_Value) && check(SetValueType,Concepts::Is_Length_Value)))
			{
				this_component()->template Value<ComponentType,CallerType,Value_Type>(value / Conversion_Factor<SetValueType>());
			}
			template<typename SetValueType> void Value(SetValueType value, requires(SetValueType,check(ComponentType,!Concepts::Has_Value) || check(SetValueType,!Concepts::Is_Length_Value)))
			{
				assert_default_check(ComponentType,Concepts::Has_Value, "Setter does not exists for this accessor.");
				assert_default_check(SetValueType,Concepts::Is_Length_Value, "The specified TargetType is not a valid Length data structure.");
			}

			template<typename ReturnType, typename ParamType> static ReturnType Convert_Value(ParamType input_value, requires(ReturnType, check(ParamType,Concepts::Is_Length_Value) && check(ReturnType,Concepts::Is_Length_Value)))
			{
				Value_Type convert_component_value_to_param = Conversion_Factor<ParamType>();
				Value_Type convert_component_value_to_return = Conversion_Factor<ReturnType>();
				return ReturnType((Value_Type)(input_value.Value) * convert_component_value_to_return / convert_component_value_to_param);
			}
			template<typename ReturnType, typename ParamType> static ReturnType Convert_Value(ParamType input_value, requires(ReturnType, check(ParamType,!Concepts::Is_Length_Value) || check(ReturnType,!Concepts::Is_Length_Value)))
			{
				assert_default_check(ReturnType,Concepts::Is_Length_Value, "TargetTyp::ReturnType is not a valid length value structure.");
				assert_default_check(ParamType,Concepts::Is_Length_Value, "TargetType::ParamType is not a valid length value structure.");
			}

			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Length_Value, Is_Centimeters) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Centimeters)))
			{
				return (Value_Type)1.0;
			};
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Length_Value, Is_Feet) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Centimeters)))
			{		
				return (Value_Type)0.0328084;
			};
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Length_Value, Is_Inches) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Centimeters)))
			{
				return (Value_Type)0.393701;				
			};
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Length_Value, Is_Kilometers) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Centimeters)))
			{
				return (Value_Type)1.0 / (Value_Type)100000.0;
			};
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Length_Value, Is_Meters) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Centimeters)))
			{
				return (Value_Type)1.0 / (Value_Type)100.0;
			};
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Length_Value, Is_Miles) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Centimeters)))
			{
				return (Value_Type)(1.0 / 160934.0);
			};

			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Length_Value, Is_Centimeters) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Feet)))
			{
				return (Value_Type)(30.48);
			};
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Length_Value, Is_Feet) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Feet)))
			{		
				return (Value_Type)(1.0);
			};
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Length_Value, Is_Inches) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Feet)))
			{
				return (Value_Type)(12.0);				
			};
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Length_Value, Is_Kilometers) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Feet)))
			{
				return (Value_Type)(1.0 / 3280.84);
			};
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Length_Value, Is_Meters) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Feet)))
			{
				return (Value_Type)(0.3048);
			};
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Length_Value, Is_Miles) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Feet)))
			{
				return (Value_Type)(1.0 / 5280.0);
			};

			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Length_Value, Is_Centimeters) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Inches)))
			{
				return (Value_Type)(2.54);
			};
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Length_Value, Is_Feet) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Inches)))
			{		
				return (Value_Type)(1.0/12.0);
			};
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Length_Value, Is_Inches) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Inches)))
			{
				return (Value_Type)(1.0);				
			};
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Length_Value, Is_Kilometers) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Inches)))
			{
				return (Value_Type)(2.54/100000.0);
			};
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Length_Value, Is_Meters) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Inches)))
			{
				return (Value_Type)(0.0254);
			};
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Length_Value, Is_Miles) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Inches)))
			{
				return (Value_Type)(0.0833333 / 5280.0);
			};

			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Length_Value, Is_Centimeters) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Kilometers)))
			{
				return (Value_Type)(100000.0);
			};
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Length_Value, Is_Feet) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Kilometers)))
			{		
				return (Value_Type)(3280.84);
			};
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Length_Value, Is_Inches) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Kilometers)))
			{
				return (Value_Type)(39370.1);				
			};
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Length_Value, Is_Kilometers) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Kilometers)))
			{
				return (Value_Type)(1.0);
			};
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Length_Value, Is_Meters) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Kilometers)))
			{
				return (Value_Type)(1000.0);
			};
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Length_Value, Is_Miles) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Kilometers)))
			{
				return (Value_Type)(0.621371);
			};
				
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Length_Value, Is_Centimeters) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Meters)))
			{
				return (Value_Type)(100.0);
			};
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Length_Value, Is_Feet) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Meters)))
			{		
				return (Value_Type)(3.28084);
			};
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Length_Value, Is_Inches) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Meters)))
			{
				return (Value_Type)(39.3701);				
			};
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Length_Value, Is_Kilometers) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Meters)))
			{
				return (Value_Type)(0.001);
			};
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Length_Value, Is_Meters) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Meters)))
			{
				return (Value_Type)(1.0);
			};
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Length_Value, Is_Miles) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Meters)))
			{
				return (Value_Type)(0.000621371);
			};
					
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Length_Value, Is_Centimeters) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Miles)))
			{
				return (Value_Type)(160934.0);
			};
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Length_Value, Is_Feet) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Miles)))
			{		
				return (Value_Type)(5280.0);
			};
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Length_Value, Is_Inches) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Miles)))
			{
				return (Value_Type)(63360.0);				
			};
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Length_Value, Is_Kilometers) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Miles)))
			{
				return (Value_Type)(1.60934);
			};
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Length_Value, Is_Meters) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Miles)))
			{
				return (Value_Type)(1609.34);
			};
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Length_Value, Is_Miles) && sub_check(ComponentType,Concepts::Is_Length_Value, Is_Miles)))
			{
				return (Value_Type)(1.0);
			};

			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, !(
				(sub_check(TargetType,Concepts::Is_Length_Value, Is_Centimeters) || sub_check(TargetType,Concepts::Is_Length_Value, Is_Feet) || sub_check(TargetType,Concepts::Is_Length_Value, Is_Inches) || 
				sub_check(TargetType,Concepts::Is_Length_Value, Is_Kilometers) || sub_check(TargetType,Concepts::Is_Length_Value, Is_Meters) || sub_check(TargetType,Concepts::Is_Length_Value, Is_Miles)) &&
				(sub_check(ComponentType,Concepts::Is_Length_Value, Is_Centimeters) || sub_check(ComponentType,Concepts::Is_Length_Value, Is_Feet) || sub_check(ComponentType,Concepts::Is_Length_Value, Is_Inches) || 
				sub_check(ComponentType,Concepts::Is_Length_Value, Is_Kilometers) || sub_check(ComponentType,Concepts::Is_Length_Value, Is_Meters) || sub_check(ComponentType,Concepts::Is_Length_Value, Is_Miles))) ))
			{
				assert_check(TargetType,Concepts::Is_Length_Value,"The specified TargetType is not a valid spatial measurement Data Structure.");
			}
		};


		prototype struct Time_Prototype
		{
			tag_as_prototype;

			template<typename ReturnValueType> ReturnValueType Value(requires(ReturnValueType, check(ComponentType,Concepts::Has_Value) && check(ReturnValueType,Concepts::Is_Time_Value)))
			{
				return ReturnValueType(this_component()->template Value<ComponentType,CallerType,Value_Type>() * Conversion_Factor<ReturnValueType>());
			}
			template<typename ReturnValueType> ReturnValueType Value(requires(ReturnValueType, check(ComponentType,!Concepts::Has_Value) || check(ReturnValueType,!Concepts::Is_Time_Value)))
			{
				assert_default_check(ComponentType,Concepts::Has_Value, "Getter does not exists for this accessor.");
				assert_default_check(ReturnValueType,Concepts::Is_Time_Value, "The specified TargetType is not a valid Time data structure.");
			}
			template<typename SetValueType> void Value(SetValueType value, requires(SetValueType, check(ComponentType,Concepts::Has_Value) && check(SetValueType,Concepts::Is_Time_Value)))
			{
				this_component()->template Value<ComponentType,CallerType,Value_Type>(value / Conversion_Factor<SetValueType>());
			}
			template<typename SetValueType> void Value(SetValueType value, requires(SetValueType, check(ComponentType,!Concepts::Has_Value) || check(SetValueType,!Concepts::Is_Time_Value)))
			{
				assert_default_check(ComponentType,Concepts::Has_Value, "Setter does not exists for this accessor.");
				assert_default_check(SetValueType,Concepts::Is_Time_Value, "The specified TargetType is not a valid Time data structure.");
			}
			
			template<typename ReturnType, typename ParamType> static ReturnType Convert_Value(ParamType input_value, requires(ReturnType, check(ParamType,Concepts::Is_Time_Value) && check(ReturnType,Concepts::Is_Time_Value)))
			{
				Value_Type convert_component_value_to_param = Time_Prototype::Conversion_Factor<ParamType>();
				Value_Type convert_component_value_to_return = Time_Prototype::Conversion_Factor<ReturnType>();
				return ReturnType((Value_Type)(input_value.Value) * convert_component_value_to_return / convert_component_value_to_param);
			}
			template<typename ReturnType, typename ParamType> static ReturnType Convert_Value(ParamType input_value, requires(ReturnType, check(ParamType,!Concepts::Is_Time_Value) || check(ReturnType,!Concepts::Is_Time_Value)))
			{
				assert_default_check(ParamType,Concepts::Is_Time_Value,"TargetType::ParamType is not a valid Time type.");
				assert_default_check(ReturnType,Concepts::Is_Time_Value,"TargetType::ReturnType is not a valid Time type.");
			}

			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Time_Value, Is_Days) && sub_check(ComponentType,Concepts::Is_Time_Value, Is_Days)))
			{
				return (Value_Type)(1.0);
			};
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Time_Value, Is_DRSeconds) && sub_check(ComponentType,Concepts::Is_Time_Value, Is_Days)))
			{
				return (Value_Type)(864000.0);
			};
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Time_Value, Is_Hours) && sub_check(ComponentType,Concepts::Is_Time_Value, Is_Days)))
			{
				return (Value_Type)(24.0);
			};
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Time_Value, Is_Minutes) && sub_check(ComponentType,Concepts::Is_Time_Value, Is_Days)))
			{
				return (Value_Type)(1440.0);
			};
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Time_Value, Is_Seconds) && sub_check(ComponentType,Concepts::Is_Time_Value, Is_Days)))
			{
				return (Value_Type)(86400.0);
			};

			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Time_Value, Is_Days) && sub_check(ComponentType,Concepts::Is_Time_Value, Is_DRSeconds)))
			{
				return (Value_Type)(1.0 / 864000.0);
			};
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Time_Value, Is_DRSeconds) && sub_check(ComponentType,Concepts::Is_Time_Value, Is_DRSeconds)))
			{
				return (Value_Type)(1.0);
			};
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Time_Value, Is_Hours) && sub_check(ComponentType,Concepts::Is_Time_Value, Is_DRSeconds)))
			{
				return (Value_Type)(1.0/36000.0);
			};
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Time_Value, Is_Minutes) && sub_check(ComponentType,Concepts::Is_Time_Value, Is_DRSeconds)))
			{
				return (Value_Type)(1.0/600.0);
			};
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Time_Value, Is_Seconds) && sub_check(ComponentType,Concepts::Is_Time_Value, Is_DRSeconds)))
			{
				return (Value_Type)(0.1);
			};

			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Time_Value, Is_Days) && sub_check(ComponentType,Concepts::Is_Time_Value, Is_Hours)))
			{
				return (Value_Type)(1.0/24.0);
			};
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Time_Value, Is_DRSeconds) && sub_check(ComponentType,Concepts::Is_Time_Value, Is_Hours)))
			{
				return (Value_Type)(36000.0);
			};
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Time_Value, Is_Hours) && sub_check(ComponentType,Concepts::Is_Time_Value, Is_Hours)))
			{
				return (Value_Type)(1.0);
			};
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Time_Value, Is_Minutes) && sub_check(ComponentType,Concepts::Is_Time_Value, Is_Hours)))
			{
				return (Value_Type)(60.0);
			};
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Time_Value, Is_Seconds) && sub_check(ComponentType,Concepts::Is_Time_Value, Is_Hours)))
			{
				return (Value_Type)(3600.0);
			};

			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Time_Value, Is_Days) && sub_check(ComponentType,Concepts::Is_Time_Value, Is_Minutes)))
			{
				return (Value_Type)(1.0/1440.0);
			};
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Time_Value, Is_DRSeconds) && sub_check(ComponentType,Concepts::Is_Time_Value, Is_Minutes)))
			{
				return (Value_Type)(600.0);
			};
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Time_Value, Is_Hours) && sub_check(ComponentType,Concepts::Is_Time_Value, Is_Minutes)))
			{
				return (Value_Type)(1.0/60.0);
			};
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Time_Value, Is_Minutes) && sub_check(ComponentType,Concepts::Is_Time_Value, Is_Minutes)))
			{
				return (Value_Type)(1.0);
			};
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Time_Value, Is_Seconds) && sub_check(ComponentType,Concepts::Is_Time_Value, Is_Minutes)))
			{
				return (Value_Type)(60.0);
			};

			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Time_Value, Is_Days) && sub_check(ComponentType,Concepts::Is_Time_Value, Is_Seconds)))
			{
				return (Value_Type)(1.0/ 86400.0);
			};
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Time_Value, Is_DRSeconds) && sub_check(ComponentType,Concepts::Is_Time_Value, Is_Seconds)))
			{
				return (Value_Type)(10.0);
			};
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Time_Value, Is_Hours) && sub_check(ComponentType,Concepts::Is_Time_Value, Is_Seconds)))
			{
				return (Value_Type)(1.0/3600.0);
			};
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Time_Value, Is_Minutes) && sub_check(ComponentType,Concepts::Is_Time_Value, Is_Seconds)))
			{
				return (Value_Type)(1.0/60.0);
			};
			template<typename TargetType> static Value_Type Conversion_Factor(requires(TargetType, sub_check(TargetType,Concepts::Is_Time_Value, Is_Seconds) && sub_check(ComponentType,Concepts::Is_Time_Value, Is_Seconds)))
			{
				return (Value_Type)(1.0);
			};
			//================================================================================================
			// Time value accessors for:
			//------------------------------------------------------------------------------------------------
			/// Time component accessor - returns the requested portion of the current time
			accessor(Time_Component,NONE,NONE);
			/// Display the time
			template<typename TargetType> void Write()
			{
				//cout <<"Total Seconds: "<<pthis->Total_Seconds<ComponentType, CallerType, TargetType>()<<endl;
				cout << "Day "<< this->Time_Component<Time_Days>()<<":  ";
				cout << this->Time_Component<Time_Hours>()<<":"<<this->Time_Component<Time_Minutes>()<<":"<<this->Time_Component<Time_Seconds>()<<"."<< this->Time_Component<Time_DRSeconds>();
			}
		};


		prototype struct Speed_Prototype : protected Time_Prototype<ComponentType>, protected Length_Prototype<ComponentType>
		{
			tag_as_prototype;
			typedef Length_Prototype<ComponentType> length_base;
			typedef Time_Prototype<ComponentType> time_base;

			template<typename ReturnValueType> ReturnValueType Value(requires(ReturnValueType, check(ComponentType,Concepts::Has_Value) && check(ComponentType, Concepts::Is_Speed_Component) && check(ReturnValueType,Concepts::Is_Speed_Value)))
			{
				ReturnValueType len_val = length_base::template Conversion_Factor<ReturnValueType>();
				ReturnValueType time_val = time_base::template Conversion_Factor<ReturnValueType>();
				return ReturnValueType(this_component()->template Value<ComponentType,CallerType,Value_Type>() * len_val / time_val );
			}
			template<typename ReturnValueType> ReturnValueType Value(requires(ReturnValueType, !check(ComponentType,Concepts::Has_Value) || !check(ComponentType, Concepts::Is_Speed_Component) || !check(ReturnValueType,Concepts::Is_Speed_Value)))
			{
				assert_default_check(ComponentType,Concepts::Has_Value, "Getter does not exists for this accessor.");
				assert_default_check(ReturnValueType,Concepts::Is_Speed_Value, "The specified TargetType is not a valid Speed data structure, ensure that TargetType has tags: {Speed_Type, Length_Type and Time_Type}");
				assert_default_check(ComponentType,Concepts::Is_Speed_Component, "The specified ComponentType is not a valid Speed component, ensure that ComponentType is tagged as a Speed_Type and has Value member}");
			}
			template<typename SetValueType> void Value(SetValueType value, requires(SetValueType, check(ComponentType,Concepts::Has_Value) && check(ComponentType, Concepts::Is_Speed_Component) && check(SetValueType,Concepts::Is_Speed_Value)))
			{
				SetValueType len_val = length_base::template Conversion_Factor<SetValueType>();
				SetValueType time_val = time_base::template Conversion_Factor<SetValueType>();
				this_component()->template Value<ComponentType,CallerType,Value_Type>(Value_Type(value * time_val / len_val));
			}
			template<typename SetValueType> void Value(SetValueType value, requires(SetValueType, !check(ComponentType,Concepts::Has_Value) || !check(ComponentType, Concepts::Is_Speed_Component) || !check(SetValueType,Concepts::Is_Speed_Value)))
			{
				assert_default_check(ComponentType,Concepts::Has_Value, "Setter does not exists for this accessor.");
				assert_default_check(SetValueType,Concepts::Is_Speed_Value, "The specified TargetType is not a valid Speed data structure, ensure that TargetType has tags: {Speed_Type, Length_Type and Time_Type}");
				assert_default_check(ComponentType,Concepts::Is_Speed_Component, "The specified ComponentType is not a valid Speed component, ensure that ComponentType is tagged as a Speed_Type and has Value member}");


			}
			
			template<typename ReturnType, typename ParamType> static ReturnType Convert_Value(ParamType input_value, requires(ReturnType, check(ParamType,Concepts::Is_Time_Value) && check(ReturnType,Concepts::Is_Time_Value)))
			{
				Value_Type convert_component_value_to_param = time_base::template Conversion_Factor<ParamType>();
				Value_Type convert_component_value_to_return = time_base::template Conversion_Factor<ReturnType>();
				return ReturnType((Value_Type)(input_value.Value) / convert_component_value_to_return * convert_component_value_to_param);
			}
			template<typename ReturnType, typename ParamType> static ReturnType Convert_Value(ParamType input_value, requires(ReturnType, check(ParamType,!Concepts::Is_Time_Value) || check(ReturnType,!Concepts::Is_Time_Value)))
			{
				assert_default_check(ParamType,Concepts::Is_Time_Value,"TargetType::ParamType is not a valid Time type.");
				assert_default_check(ReturnType,Concepts::Is_Time_Value,"TargetType::ReturnType is not a valid Time type.");
			}
		};
		
	}

}

using namespace Basic_Units::Concepts;
using namespace Basic_Units::Prototypes;

}