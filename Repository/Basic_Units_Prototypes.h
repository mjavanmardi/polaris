#pragma once
#include "Repository_Includes.h"



namespace Basic_Units
{
	//==========================================================================
	// Define the arithmetic value type to use in basic unit calculations here
	typedef double Value_Type;
	//==========================================================================

	namespace Types
	{
		namespace Length_Types
		{
			typedef true_type Length_Type;
			typedef true_type Feet_Type;
			typedef true_type Inches_Type;
			typedef true_type Miles_Type;
			typedef true_type Centimeters_Type;
			typedef true_type Meters_Type;
			typedef true_type Kilometers_Type;
			
		}

		typedef true_type Area_Type;
		typedef true_type Volume_Type;

		namespace Time_Types
		{
			typedef true_type Time_Type;
			typedef true_type Days_Type;
			typedef true_type Hours_Type;
			typedef true_type Minutes_Type;
			typedef true_type Seconds_Type;
			typedef true_type DRSeconds_Type; 
		}
	}

	namespace Concepts
	{
		#pragma region Length Concepts
		concept struct Is_Length_Value
		{
			check_typename_state(Is_Length,Length_Type,true_type);
			check_typename_state(Is_Inches,Inches_Type,true_type);
			check_typename_state(Is_Feet,Feet_Type,true_type);
			check_typename_state(Is_Miles,Miles_Type,true_type);
			check_typename_state(Is_Centimeters,Centimeters_Type,true_type);
			check_typename_state(Is_Meters,Meters_Type,true_type);
			check_typename_state(Is_Kilometers,Kilometers_Type,true_type);
			define_default_check(Is_Length && (Is_Inches || Is_Feet || Is_Miles || Is_Centimeters || Is_Meters || Is_Kilometers));
		};

		/// Check if a type meets the requirements of being a basic 'Area' component
		concept struct Is_Length_Component
		{
			check_concept(Is_Component,Is_Polaris_Component);
			check_typename_state(Is_Length, Length_Type, true_type);
			check_named_member(Has_Value,Value);
			define_default_check(Is_Component && Is_Length && Has_Value);
		};
		concept struct Is_Length_Prototype
		{
			check_concept(Is_Prototype,Is_Polaris_Prototype);
			check_typename_state(Is_Length, Component_Type::Length_Type, true_type);
		};
		#pragma endregion

		#pragma region Area Concepts
		concept struct Is_Area_Value
		{
			check_typename_state(Is_Area, Area_Type, true_type);
			check_typename_state(Is_Length, Length_Type, true_type);
			define_default_check(Is_Area && Is_Length);
		};
		/// Check if a type meets the requirements of being a basic 'Area' component
		concept struct Is_Area_Component
		{
			check_concept(Is_Component,Is_Polaris_Component);
			check_typename_state(Is_Area, Area_Type, true_type);
			check_named_member(Has_Value,Value);
			define_default_check(Is_Component && Is_Area && Has_Value);
		};
		concept struct Is_Area_Prototype
		{
			check_concept(Is_Prototype,Is_Polaris_Prototype);
			check_typename_state(Is_Area, Component_Type::Area_Type, true_type);
			define_default_check(Is_Area && Is_Prototype);
		};
		#pragma endregion

		#pragma region Volume Concepts
		/// Check if a type meets the requirements of being a basic 'volume' value
		concept struct Is_Volume_Value
		{
			check_typename_state(Is_Volume, Volume_Type, true_type);
			check_typename_state(Is_Length, Length_Type, true_type);
			define_default_check(Is_Volume && Is_Length);
		};
		/// Check if a type meets the requirements of being a basic 'Time' component
		concept struct Is_Volume_Component
		{
			check_concept(Is_Component,Is_Polaris_Component);
			check_typename_state(Is_Volume, Volume_Type, true_type);
			check_named_member(Has_Value,Value);
			define_default_check(Is_Component && Is_Volume && Has_Value);
		};
		concept struct Is_Volume_Prototype
		{
			check_concept(Is_Prototype,Is_Polaris_Prototype);
			check_typename_state(Is_Volume, Component_Type::Volume_Type, true_type);
			define_default_check(Is_Volume && Is_Prototype);
		};
		#pragma endregion

		#pragma region Time Concepts
		/// Check if the class has an arithmetic 'Time' member defined
		concept struct Is_Time_Value
		{
			check_typename_state(Has_Time,Time_Type,true_type);
			check_typename_state(Is_DRSeconds,DRSeconds_Type,true_type);
			check_typename_state(Is_Seconds,Seconds_Type,true_type);
			check_typename_state(Is_Minutes,Minutes_Type,true_type);
			check_typename_state(Is_Hours,Hours_Type,true_type);
			check_typename_state(Is_Days,Days_Type,true_type);
			define_default_check(Has_Time && (Is_DRSeconds || Is_Seconds || Is_Minutes || Is_Hours || Is_Days));
		};

		/// Check if a type meets the requirements of being a basic 'Time' component
		concept struct Is_Time
		{
			check_concept(Is_Component,Is_Polaris_Component);
			check_typename_state(Has_Time, Time_Type, true_type);
			check_named_member(Has_Value,Value);
			define_default_check(Is_Component && Has_Time && Has_Value);
		};
		concept struct Is_Time_Prototype
		{
			check_concept(Is_Prototype,Is_Polaris_Prototype);
			check_typename_state(Has_Time, Component_Type::Time_Type, true_type);
			check_named_member(Has_Value,Value);
			define_default_check(Is_Prototype && Has_Time && Has_Value);
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
				return ReturnValueType(this_component()->Value<ComponentType,CallerType,Value_Type>() * Conversion_Factor<ReturnValueType>());
			}
			template<typename ReturnValueType> ReturnValueType Value(requires_getter(check(ComponentType,!Get_Value_exists) || check(ReturnValueType,!Concepts::Is_Length_Value)))
			{
				assert_check(ComponentType,Get_Value_exists, "Getter does not exists for this accessor.");
				assert_check(ReturnValueType,Concepts::Is_Length_Value, "The specified TargetType is not a valid Length data structure.");
			}
			template<typename SetValueType> void Value(SetValueType value, requires_setter(check(ComponentType,Set_Value_exists) && check(SetValueType,Concepts::Is_Length_Value)))
			{
				this_component()->Value<ComponentType,CallerType,Value_Type>(value / Conversion_Factor<SetValueType>());
			}
			template<typename SetValueType> void Value(SetValueType value, requires_setter(check(ComponentType,!Set_Value_exists) || check(SetValueType,!Concepts::Is_Length_Value)))
			{
				assert_requirements(ComponentType,set_exists_check(Value), "Setter does not exists for this accessor.");
				assert_requirements(SetValueType,Concepts::Is_Length_Value, "The specified TargetType is not a valid Length data structure.");
			}

			feature_prototype static typename TargetType::ReturnType Convert_Value(typename TargetType::ParamType input_value, requires(check(typename TargetType,Is_Target_Type_Struct) && check(typename TargetType::ParamType,Concepts::Is_Length_Value) && check(typename TargetType::ReturnType,Concepts::Is_Length_Value)))
			{
				Value_Type convert_component_value_to_param = Conversion_Factor<TargetType::ParamType>();
				Value_Type convert_component_value_to_return = Conversion_Factor<TargetType::ReturnType>();
				return TargetType::ReturnType((Value_Type)(input_value.Value) * convert_component_value_to_return / convert_component_value_to_param);
			}
			feature_prototype static typename TargetType::ReturnType Convert_Value(typename TargetType::ParamType input_value, requires(check(typename TargetType,!Is_Target_Type_Struct) || check(typename TargetType::ParamType,!Concepts::Is_Length_Value) || check(typename TargetType::ReturnType,!Concepts::Is_Length_Value)))
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
				return ((Length_Prototype<ComponentType,CallerType>*)this)->Value<TargetType>();
			}

			feature_prototype void Value(TargetType value)
			{
				((Length_Prototype<ComponentType,CallerType>*)this)->Value<TargetType>(value);
			}
		};

		prototype struct Height_Prototype : public Length_Prototype<ComponentType,CallerType>
		{
			feature_prototype TargetType Value()
			{
				return ((Length_Prototype<ComponentType,CallerType>*)this)->Value<TargetType>();
			}

			feature_prototype void Value(TargetType value)
			{
				((Length_Prototype<ComponentType,CallerType>*)this)->Value<TargetType>(value);
			}
		};

		prototype struct Area_Prototype : public Length_Prototype<ComponentType, CallerType>
		{
			template<typename ReturnValueType> ReturnValueType Value(requires_getter(check(ReturnValueType,Concepts::Is_Area_Value)))
			{		
				return ((Length_Prototype<ComponentType,CallerType>*)this)->Value<ReturnValueType>()*((Length_Prototype<ComponentType,CallerType>*)this)->Conversion_Factor<ReturnValueType>();
			}
			template<typename ReturnValueType> ReturnValueType Value(requires_getter(check(ReturnValueType,!Concepts::Is_Area_Value)))
			{
				assert_check(ReturnValueType,Concepts::Is_Area_Value,"Your target type is not identified as an area measure.")
			}

			template<typename SetValueType> void Value(SetValueType value, requires_setter(check(SetValueType,Concepts::Is_Area_Value)))
			{
				((Length_Prototype<ComponentType,CallerType>*)this)->Value<SetValueType>(value / ((Length_Prototype<ComponentType,CallerType>*)this)->Conversion_Factor<SetValueType>());
			}
			template<typename SetValueType> void Value(SetValueType value, requires_setter(check(SetValueType,!Concepts::Is_Area_Value)))
			{
				assert_check(SetValueType,Concepts::Is_Area_Value,"Your target type is not identified as an area measure.")
			}

			feature_prototype static typename TargetType::ReturnType Convert_Value(typename TargetType::ParamType input_value, requires(check(TargetType,Is_Target_Type_Struct) && check(typename TargetType::ParamType,Concepts::Is_Area_Value) && check(typename TargetType::ReturnType,Concepts::Is_Area_Value)))
			{
				Value_Type convert_component_value_to_param = Conversion_Factor<TargetType::ParamType>();
				Value_Type convert_component_value_to_return = Conversion_Factor<TargetType::ReturnType>();
				Value_Type conversion = convert_component_value_to_return / convert_component_value_to_param;
				return TargetType::ReturnType((Value_Type)(input_value.Value) * pow(conversion,2.0));
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
			template<typename ReturnValueType>  ReturnValueType Value(requires_getter(check(ReturnValueType,Concepts::Is_Volume_Value)))
			{		
				return ((Length_Prototype<ComponentType,CallerType>*)this)->Value<ReturnValueType>()*((Length_Prototype<ComponentType,CallerType>*)this)->Conversion_Factor<ReturnValueType>()*((Length_Prototype<ComponentType,CallerType>*)this)->Conversion_Factor<ReturnValueType>();
			}
			template<typename ReturnValueType>  ReturnValueType Value(requires_getter(check(ReturnValueType,!Concepts::Is_Volume_Value)))
			{
				assert_check(ReturnValueType,Concepts::Is_Volume_Value,"Your target type is not identified as a volume measure.")
			}

			template<typename SetValueType>  void Value(SetValueType value,requires_setter(check(SetValueType,Concepts::Is_Volume_Value)))
			{
				((Length_Prototype<ComponentType,CallerType>*)this)->Value<SetValueType>(value / ((Length_Prototype<ComponentType,CallerType>*)this)->Conversion_Factor<SetValueType>() / ((Length_Prototype<ComponentType,CallerType>*)this)->Conversion_Factor<SetValueType>());
			}
			template<typename SetValueType>  void Value(SetValueType value, requires_setter(check(SetValueType,!Concepts::Is_Volume_Value)))
			{
				assert_check(SetValueType,Concepts::Is_Volume_Value,"Your target type is not identified as a volume measure.")
			}

			feature_prototype static typename TargetType::ReturnType Convert_Value(typename TargetType::ParamType input_value, requires(check(TargetType,Is_Target_Type_Struct) && check(typename TargetType::ParamType,Concepts::Is_Volume_Value) && check(typename TargetType::ReturnType,Concepts::Is_Volume_Value)))
			{
				Value_Type convert_component_value_to_param = Conversion_Factor<TargetType::ParamType>();
				Value_Type convert_component_value_to_return = Conversion_Factor<TargetType::ReturnType>();
				Value_Type conversion = convert_component_value_to_return / convert_component_value_to_param;
				return TargetType::ReturnType((Value_Type)(input_value.Value) * pow(conversion,3.0));
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
				return ReturnValueType(this_component()->Value<ComponentType,CallerType,Value_Type>() * Conversion_Factor<ReturnValueType>());
			}
			template<typename ReturnValueType> ReturnValueType Value(requires_getter(check(ComponentType,!Get_Value_exists) || check(ReturnValueType,!Concepts::Is_Time_Value)))
			{
				assert_requirements(ComponentType,get_exists_check(Value), "Getter does not exists for this accessor.");
				assert_requirements(TargetType,Concepts::Is_Length_Value, "The specified TargetType is not a valid Time data structure.");
			}
			template<typename SetValueType> void Value(SetValueType value, requires_setter(check(ComponentType,Set_Value_exists) && check(SetValueType,Concepts::Is_Time_Value)))
			{
				this_component()->Value<ComponentType,CallerType,Value_Type>(value / Conversion_Factor<SetValueType>());
			}
			template<typename SetValueType> void Value(SetValueType value, requires_setter(check(ComponentType,!Set_Value_exists) || check(SetValueType,!Concepts::Is_Time_Value)))
			{
				assert_requirements(ComponentType,set_exists_check(Value), "Setter does not exists for this accessor.");
				assert_requirements(SetValueType,Concepts::Is_Length_Value, "The specified TargetType is not a valid Time data structure.");
			}
			
			feature_prototype static typename TargetType::ReturnType Convert_Value(typename TargetType::ParamType input_value, requires(check(TargetType,Is_Target_Type_Struct) && check(typename TargetType::ParamType,Concepts::Is_Time_Value) && check(typename TargetType::ReturnType,Concepts::Is_Time_Value)))
			{
				Value_Type convert_component_value_to_param = Time_Prototype::Conversion_Factor<TargetType::ParamType>();
				Value_Type convert_component_value_to_return = Time_Prototype::Conversion_Factor<TargetType::ReturnType>();
				return TargetType::ReturnType((Value_Type)(input_value.Value) * convert_component_value_to_return / convert_component_value_to_param);
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
				cout << "Day "<< this->Time_Component<Data_Structures::Time_Days>()<<":  ";
				cout << this->Time_Component<Data_Structures::Time_Hours>()<<":"<<this->Time_Component<Data_Structures::Time_Minutes>()<<":"<<this->Time_Component<Data_Structures::Time_Seconds>()<<"."<< this->Time_Component<Data_Structures::Time_DRSeconds>();
			}
		};

		prototype struct Rate_Prototype : protected Time_Prototype<ComponentType,CallerType>
		{
			tag_as_prototype;

			feature_prototype TargetType Length()
			{
				return TargetType(cast_self_to_component().length<ComponentType,CallerType,Value_Type>() * Conversion_Factor<TargetType>());
			}
			feature_prototype void Length(TargetType value)
			{
				cast_self_to_component().length<ComponentType,CallerType,Value_Type>(value / Conversion_Factor<TargetType>());
			}

		};
	}

	namespace Length_Variables
	{
		typedef Value_Type length_data_type;
		
		polaris_variable(Inches, length_data_type, Length_Type, Inches_Type);
		polaris_variable(Feet, length_data_type, Length_Type, Feet_Type);
		polaris_variable(Miles, length_data_type, Length_Type, Miles_Type);
		polaris_variable(Centimeters, length_data_type, Length_Type, Centimeters_Type);
		polaris_variable(Meters, length_data_type, Length_Type, Meters_Type);
		polaris_variable(Kilometers, length_data_type, Length_Type, Kilometers_Type);
	}

	namespace Area_Variables
	{
		typedef Value_Type length_data_type;
		polaris_variable(Square_Inches, length_data_type, Length_Type, Area_Type, Inches_Type);
		polaris_variable(Square_Feet, length_data_type, Length_Type, Area_Type, Feet_Type);
		polaris_variable(Square_Miles, length_data_type, Length_Type, Area_Type, Miles_Type);
		polaris_variable(Square_Centimeters, length_data_type, Length_Type, Area_Type, Centimeters_Type);
		polaris_variable(Square_Meters, length_data_type, Length_Type, Area_Type,  Meters_Type);
		polaris_variable(Square_Kilometers, length_data_type, Length_Type, Area_Type, Kilometers_Type);
	}

	namespace Volume_Variables
	{
		typedef Value_Type length_data_type;
		polaris_variable(Cubic_Inches, length_data_type, Length_Type, Volume_Type, Inches_Type);
		polaris_variable(Cubic_Feet, length_data_type, Length_Type, Volume_Type, Feet_Type);
		polaris_variable(Cubic_Miles, length_data_type, Length_Type, Volume_Type, Miles_Type);
		polaris_variable(Cubic_Centimeters, length_data_type, Length_Type, Volume_Type, Centimeters_Type);
		polaris_variable(Cubic_Meters, length_data_type, Length_Type, Volume_Type, Meters_Type);
		polaris_variable(Cubic_Kilometers, length_data_type, Length_Type, Volume_Type, Kilometers_Type);
	}

	namespace Time_Variables
	{
		typedef Value_Type time_data_type;
		polaris_variable(Time_DRSeconds,time_data_type,Time_Type,DRSeconds_Type);
		polaris_variable(Time_Seconds,time_data_type,Time_Type,Seconds_Type);
		polaris_variable(Time_Minutes,time_data_type,Time_Type,Minutes_Type);
		polaris_variable(Time_Hours,time_data_type,Time_Type,Hours_Type);
		polaris_variable(Time_Days,time_data_type,Time_Type,Days_Type);
	}

	namespace Rate_Variables
	{
		typedef Value_Type rate_data_type;
		polaris_variable(Units_Per_Second,rate_data_type,Time_Type,Seconds_Type);
		polaris_variable(Units_Per_Minute,rate_data_type,Time_Type,Minutes_Type);
		polaris_variable(Units_Per_Hour,rate_data_type,Time_Type,Hours_Type);
		polaris_variable(Units_Per_Day,rate_data_type,Time_Type,Days_Type);
	}
}