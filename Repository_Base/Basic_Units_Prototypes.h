#pragma once
#include "Repository_Base_Includes.h"



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
		#pragma region // Length Concepts
		concept Measure_in_Inches
		{
			begin_requirements_list(none);
			requires_typename_state(none, Inches_Type, true_type,"Measure is not in Inches");
			requires_typename_state(Inches_Type, Length_Type, true_type,"Type is not a length type");
			end_requirements_list(Length_Type);
		};
		concept Measure_in_Feet
		{
			begin_requirements_list(none);
			requires_typename_state(none, Feet_Type, true_type,"Measure is not in feet");
			requires_typename_state(Feet_Type, Length_Type, true_type,"Type is not a length type");
			end_requirements_list(Length_Type);
		};
		concept Measure_in_Miles
		{
			begin_requirements_list(none);
			requires_typename_state(none, Miles_Type, true_type,"Measure is not in Miles");
			requires_typename_state(Miles_Type, Length_Type, true_type,"Type is not a length type");
			end_requirements_list(Length_Type);
		};
		concept Measure_in_Centimeters
		{
			begin_requirements_list(none);
			requires_typename_state(none, Centimeters_Type, true_type,"Measure is not in Centimeters");
			requires_typename_state(Centimeters_Type, Length_Type, true_type,"Type is not a length type");
			end_requirements_list(Length_Type);
		};
		concept Measure_in_Meters
		{
			begin_requirements_list(none);
			requires_typename_state(none, Meters_Type, true_type,"Measure is not in Meters");
			requires_typename_state(Meters_Type, Length_Type, true_type,"Type is not a length type");
			end_requirements_list(Length_Type);
		};
		concept Measure_in_Kilometers
		{
			begin_requirements_list(none);
			requires_typename_state(none, Kilometers_Type, true_type,"Measure is not in Kilometers");
			requires_typename_state(Kilometers_Type, Length_Type, true_type,"Type is not a length type");
			end_requirements_list(Length_Type);
		};
		concept Is_Length_Value
		{
			begin_requirements_list(none);
			requires_typename_state(none, Length_Type, true_type,"Type is not tagged as a valid area type measure");
			end_requirements_list(Length_Type);
		};
		/// Check if a type meets the requirements of being a basic 'Area' component
		concept Is_Length
		{
			begin_requirements_list(none);
			requires_concept(none,Is_Polaris_Component);
			requires_typename_state(Is_Polaris_Component, Length_Type, true_type,"This class has no Area_Type = true_type defined");
			end_requirements_list(Length_Type);
		};
		concept Is_Length_Prototype
		{
			begin_requirements_list(none);
			requires_concept(none,Is_Polaris_Prototype);
			requires_typename_state_interface(Is_Polaris_Prototype, Length_Type, true_type,"This class has no Area_Type = true_type defined");
			end_requirements_list(Length_Type);
		};
		#pragma endregion

		#pragma region // Area Concepts
		concept Is_Area_Value
		{
			begin_requirements_list(none);
			requires_typename_state(none, Area_Type, true_type,"Type is not tagged as a valid area type measure");
			requires_typename_state(Area_Type, Length_Type, true_type,"Type does not derive from a valid length type");
			end_requirements_list(Length_Type);
		};
		/// Check if a type meets the requirements of being a basic 'Area' component
		concept Is_Area
		{
			begin_requirements_list(none);
			requires_concept(none,Is_Polaris_Component);
			requires_typename_state(Is_Polaris_Component, Area_Type, true_type,"This class has no Area_Type = true_type defined");
			end_requirements_list(Area_Type);
		};
		concept Is_Area_Prototype
		{
			begin_requirements_list(none);
			requires_concept(none,Is_Polaris_Prototype);
			requires_typename_state_interface(Is_Polaris_Prototype, Area_Type, true_type,"This class has no Area_Type = true_type defined");
			end_requirements_list(Area_Type);
		};
		#pragma endregion

		#pragma region // Volume Concepts
		/// Check if a type meets the requirements of being a basic 'Time' value
		concept Is_Volume_Value
		{
			begin_requirements_list(none);
			requires_typename_state(none, Volume_Type, true_type,"Type is not a valid volume type measure");
			requires_typename_state(Volume_Type, Length_Type, true_type,"Type does not derive from a valid length type");
			end_requirements_list(Length_Type);
		};
		/// Check if a type meets the requirements of being a basic 'Time' component
		concept Is_Volume
		{
			begin_requirements_list(none);
			requires_concept(none,Is_Polaris_Component);
			requires_typename_state(Is_Polaris_Component, Volume_Type, true_type,"This class has no TimeType = true_type defined");
			end_requirements_list(Volume_Type);
		};
		concept Is_Volume_Prototype
		{
			begin_requirements_list(none);
			requires_concept(none,Is_Polaris_Prototype);
			requires_typename_state_interface(Is_Polaris_Prototype, Volume_Type, true_type,"This class has no TimeType = true_type defined");
			end_requirements_list(Volume_Type);
		};
		#pragma endregion

		#pragma region // Time Concepts
		/// Check if the class has an arithmetic 'Time' member defined
		concept Has_Time
		{
			begin_requirements_list(none);
			requires_typename_state(none,Time_Type,true_type,"Type is not a Time data structure");
			//requires_concept_checked_member(TimeType, value, is_arithmetic,"An arithmetic 'Time' data member has not been defined.");
			end_requirements_list(Time_Type);
		};
		/// Check if initializing struct is tagged as DRSeconds (i.e. 1/10th of a second)
		concept Time_In_DRSeconds
		{
			begin_requirements_list(none);
			requires_concept(none,Has_Time);
			requires_typename_state(Has_Time,DRSeconds_Type,true_type,"Type is not in DRSeconds");
			end_requirements_list(DRSeconds_Type);
		};
		/// Check if initializing struct is tagged as Seconds
		concept Time_In_Seconds
		{
			begin_requirements_list(none);
			requires_concept(none,Has_Time);
			requires_typename_state(Has_Time,Seconds_Type,true_type,"Type is not in Seconds");
			end_requirements_list(Seconds_Type);
		};
		/// Check if initializing struct is tagged as Minutes
		concept Time_In_Minutes
		{
			begin_requirements_list(none);
			requires_concept(none,Has_Time);
			requires_typename_state(Has_Time,Minutes_Type,true_type,"Type is not in Minutes");
			end_requirements_list(Minutes_Type);
		};
		/// Check if initializing struct is tagged as Hours
		concept Time_In_Hours
		{
			begin_requirements_list(none);
			requires_concept(none,Has_Time);
			requires_typename_state(Has_Time,Hours_Type,true_type,"Type is not in Hours");
			end_requirements_list(Hours_Type);
		};
		/// Check if initializing struct is tagged as Days
		concept Time_In_Days
		{
			begin_requirements_list(none);
			requires_concept(none,Has_Time);
			requires_typename_state(Has_Time,Days_Type,true_type,"Type is not in Hours");
			end_requirements_list(Days_Type);
		};
		/// Check if a type meets the requirements of being a basic 'Time' value
		concept Is_Time_Value
		{
			begin_requirements_list(none);
			requires_typename_state(none, Time_Type, true_type,"This class has no TimeType = true_type defined");
			end_requirements_list(Time_Type);
		};
		/// Check if a type meets the requirements of being a basic 'Time' component
		concept Is_Time
		{
			begin_requirements_list(none);
			requires_concept(none,Is_Polaris_Component);
			requires_typename_state(Is_Polaris_Component, Time_Type, true_type,"This class has no TimeType = true_type defined");
			end_requirements_list(Time_Type);
		};
		concept Is_Time_Prototype
		{
			begin_requirements_list(none);
			requires_concept(none,Is_Polaris_Prototype);
			requires_typename_state_interface(Is_Polaris_Prototype, Time_Type, true_type,"This class has no TimeType = true_type defined");
			end_requirements_list(Time_Type);
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
			tag_polaris_prototype;

			define_get_set_exists_check(Value);
			feature TargetType Value(call_requirements(requires(ComponentType,get_exists_check(Value)) && requires(TargetType,Concepts::Is_Length_Value)))
			{
				return TargetType(cast_self_to_component().Value<ComponentType,CallerType,Value_Type>() * Conversion_Factor<TargetType>());
			}
			feature void Value(TargetType value, call_requirements(requires(ComponentType,set_exists_check(Value)) && requires(TargetType,Concepts::Is_Length_Value)))
			{
				cast_self_to_component().Value<ComponentType,CallerType,Value_Type>(value / Conversion_Factor<TargetType>());
			}
			
			feature static typename TargetType::ReturnType Convert_Value(typename TargetType::ParamType input_value, call_requirements(requires(typename TargetType,Is_Target_Type_Struct) && requires(typename TargetType::ParamType,Concepts::Is_Length_Value) && requires(typename TargetType::ReturnType,Concepts::Is_Length_Value)))
			{
				Value_Type convert_component_value_to_param = Conversion_Factor<TargetType::ParamType>();
				Value_Type convert_component_value_to_return = Conversion_Factor<TargetType::ReturnType>();
				return TargetType::ReturnType((Value_Type)(input_value.Value) * convert_component_value_to_return / convert_component_value_to_param);
			}
			feature static typename TargetType::ReturnType Convert_Value(typename TargetType::ParamType input_value, call_requirements(requires(typename TargetType,!Is_Target_Type_Struct) || requires(typename TargetType::ParamType,!Concepts::Is_Length_Value) || requires(typename TargetType::ReturnType,!Concepts::Is_Length_Value)))
			{
				assert_requirements(TargetType,Is_Target_Type_Struct, "TargetType is not a valid Target_Type_Struct structure.");
				assert_requirements(typename TargetType::ReturnType,Concepts::Is_Length_Value, "TargetTyp::ReturnType is not a valid length value structure.");
				assert_requirements(typename TargetType::ParamType,Concepts::Is_Length_Value, "TargetType::ParamType is not a valid length value structure.");
			}

			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Centimeters) && requires(ComponentType,Concepts::Measure_in_Centimeters)))
			{
				return (Value_Type)1.0;
			};
			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Feet) && requires(ComponentType,Concepts::Measure_in_Centimeters)))
			{		
				return (Value_Type)0.0328084;
			};
			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Inches) && requires(ComponentType,Concepts::Measure_in_Centimeters)))
			{
				return (Value_Type)0.393701;				
			};
			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Kilometers) && requires(ComponentType,Concepts::Measure_in_Centimeters)))
			{
				return (Value_Type)1.0 / (Value_Type)100000.0;
			};
			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Meters) && requires(ComponentType,Concepts::Measure_in_Centimeters)))
			{
				return (Value_Type)1.0 / (Value_Type)100.0;
			};
			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Miles) && requires(ComponentType,Concepts::Measure_in_Centimeters)))
			{
				return (Value_Type)(1.0 / 160934.0);
			};

			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Centimeters) && requires(ComponentType,Concepts::Measure_in_Feet)))
			{
				return (Value_Type)(30.48);
			};
			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Feet) && requires(ComponentType,Concepts::Measure_in_Feet)))
			{		
				return (Value_Type)(1.0);
			};
			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Inches) && requires(ComponentType,Concepts::Measure_in_Feet)))
			{
				return (Value_Type)(12.0);				
			};
			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Kilometers) && requires(ComponentType,Concepts::Measure_in_Feet)))
			{
				return (Value_Type)(1.0 / 3280.84);
			};
			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Meters) && requires(ComponentType,Concepts::Measure_in_Feet)))
			{
				return (Value_Type)(0.3048);
			};
			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Miles) && requires(ComponentType,Concepts::Measure_in_Feet)))
			{
				return (Value_Type)(1.0 / 5280.0);
			};

			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Centimeters) && requires(ComponentType,Concepts::Measure_in_Inches)))
			{
				return (Value_Type)(2.54);
			};
			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Feet) && requires(ComponentType,Concepts::Measure_in_Inches)))
			{		
				return (Value_Type)(1.0/12.0);
			};
			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Inches) && requires(ComponentType,Concepts::Measure_in_Inches)))
			{
				return (Value_Type)(1.0);				
			};
			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Kilometers) && requires(ComponentType,Concepts::Measure_in_Inches)))
			{
				return (Value_Type)(2.54/100000.0);
			};
			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Meters) && requires(ComponentType,Concepts::Measure_in_Inches)))
			{
				return (Value_Type)(0.0254);
			};
			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Miles) && requires(ComponentType,Concepts::Measure_in_Inches)))
			{
				return (Value_Type)(0.0833333 / 5280.0);
			};

			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Centimeters) && requires(ComponentType,Concepts::Measure_in_Kilometers)))
			{
				return (Value_Type)(100000.0);
			};
			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Feet) && requires(ComponentType,Concepts::Measure_in_Kilometers)))
			{		
				return (Value_Type)(3280.84);
			};
			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Inches) && requires(ComponentType,Concepts::Measure_in_Kilometers)))
			{
				return (Value_Type)(39370.1);				
			};
			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Kilometers) && requires(ComponentType,Concepts::Measure_in_Kilometers)))
			{
				return (Value_Type)(1.0);
			};
			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Meters) && requires(ComponentType,Concepts::Measure_in_Kilometers)))
			{
				return (Value_Type)(1000.0);
			};
			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Miles) && requires(ComponentType,Concepts::Measure_in_Kilometers)))
			{
				return (Value_Type)(0.621371);
			};
				
			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Centimeters) && requires(ComponentType,Concepts::Measure_in_Meters)))
			{
				return (Value_Type)(100.0);
			};
			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Feet) && requires(ComponentType,Concepts::Measure_in_Meters)))
			{		
				return (Value_Type)(3.28084);
			};
			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Inches) && requires(ComponentType,Concepts::Measure_in_Meters)))
			{
				return (Value_Type)(39.3701);				
			};
			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Kilometers) && requires(ComponentType,Concepts::Measure_in_Meters)))
			{
				return (Value_Type)(0.001);
			};
			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Meters) && requires(ComponentType,Concepts::Measure_in_Meters)))
			{
				return (Value_Type)(1.0);
			};
			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Miles) && requires(ComponentType,Concepts::Measure_in_Meters)))
			{
				return (Value_Type)(0.000621371);
			};
					
			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Centimeters) && requires(ComponentType,Concepts::Measure_in_Miles)))
			{
				return (Value_Type)(160934.0);
			};
			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Feet) && requires(ComponentType,Concepts::Measure_in_Miles)))
			{		
				return (Value_Type)(5280.0);
			};
			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Inches) && requires(ComponentType,Concepts::Measure_in_Miles)))
			{
				return (Value_Type)(63360.0);				
			};
			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Kilometers) && requires(ComponentType,Concepts::Measure_in_Miles)))
			{
				return (Value_Type)(1.60934);
			};
			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Meters) && requires(ComponentType,Concepts::Measure_in_Miles)))
			{
				return (Value_Type)(1609.34);
			};
			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Miles) && requires(ComponentType,Concepts::Measure_in_Miles)))
			{
				return (Value_Type)(1.0);
			};

			feature static Value_Type Conversion_Factor(call_requirements(!(
				(requires(TargetType,Concepts::Measure_in_Centimeters) || requires(TargetType,Concepts::Measure_in_Feet) || requires(TargetType,Concepts::Measure_in_Inches) || 
				requires(TargetType,Concepts::Measure_in_Kilometers) || requires(TargetType,Concepts::Measure_in_Meters) || requires(TargetType,Concepts::Measure_in_Miles)) &&
				(requires(ComponentType,Concepts::Measure_in_Centimeters) || requires(ComponentType,Concepts::Measure_in_Feet) || requires(ComponentType,Concepts::Measure_in_Inches) || 
				requires(ComponentType,Concepts::Measure_in_Kilometers) || requires(ComponentType,Concepts::Measure_in_Meters) || requires(ComponentType,Concepts::Measure_in_Miles))) ))
			{
				assert_requirements(TargetType,Concepts::Measure_in_Centimeters,"The specified TargetType is not a valid spatial measurement Data Structure.");
			}
		};

		prototype struct Width_Prototype : public Length_Prototype<ComponentType,CallerType>
		{

			feature TargetType Value()
			{
				return ((Length_Prototype<ComponentType,CallerType>*)this)->Value<TargetType>();
			}

			feature void Value(TargetType value)
			{
				((Length_Prototype<ComponentType,CallerType>*)this)->Value<TargetType>(value);
			}
		};

		prototype struct Height_Prototype : public Length_Prototype<ComponentType,CallerType>
		{
			feature TargetType Value()
			{
				return ((Length_Prototype<ComponentType,CallerType>*)this)->Value<TargetType>();
			}

			feature void Value(TargetType value)
			{
				((Length_Prototype<ComponentType,CallerType>*)this)->Value<TargetType>(value);
			}
		};

		prototype struct Area_Prototype : public Length_Prototype<ComponentType, CallerType>
		{
			feature TargetType Value(call_requires(TargetType,Concepts::Is_Area_Value))
			{		
				return ((Length_Prototype<ComponentType,CallerType>*)this)->Value<TargetType>()*((Length_Prototype<ComponentType,CallerType>*)this)->Conversion_Factor<TargetType>();
			}
			feature TargetType Value(call_requires(TargetType,!Concepts::Is_Area_Value))
			{
				assert_requirements(TargetType,Concepts::Is_Area_Measure,"Your target type is not identified as an area measure.")
			}

			feature void Value(TargetType value, call_requires(TargetType,Concepts::Is_Area_Value))
			{
				((Length_Prototype<ComponentType,CallerType>*)this)->Value<TargetType>(value / ((Length_Prototype<ComponentType,CallerType>*)this)->Conversion_Factor<TargetType>());
			}
			feature void Value(TargetType value, call_requires(TargetType,!Concepts::Is_Area_Value))
			{
				assert_requirements(TargetType,Concepts::Is_Area_Measure,"Your target type is not identified as an area measure.")
			}

			feature static typename TargetType::ReturnType Convert_Value(typename TargetType::ParamType input_value, call_requirements(requires(TargetType,Is_Target_Type_Struct) && requires(typename TargetType::ParamType,Concepts::Is_Area_Value) && requires(typename TargetType::ReturnType,Concepts::Is_Area_Value)))
			{
				Value_Type convert_component_value_to_param = Conversion_Factor<TargetType::ParamType>();
				Value_Type convert_component_value_to_return = Conversion_Factor<TargetType::ReturnType>();
				Value_Type conversion = convert_component_value_to_return / convert_component_value_to_param;
				return TargetType::ReturnType((Value_Type)(input_value.Value) * pow(conversion,2.0));
			}
			feature static typename TargetType::ReturnType Convert_Value(typename TargetType::ParamType input_value, call_requirements(requires(TargetType,!Is_Target_Type_Struct) || requires(typename TargetType::ParamType,!Concepts::Is_Area_Value) || requires(typename TargetType::ReturnType,!Concepts::Is_Area_Value)))
			{
				assert_requirements(TargetType,Is_Target_Type_Struct, "TargetType is not a valid Target_Type_Struct structure.");
				assert_requirements(typename TargetType::ReturnType,Concepts::Is_Area_Value, "TargetType::ReturnType is not a valid area value structure.");
				assert_requirements(typename TargetType::ParamType,Concepts::Is_Area_Value, "TargetType::ParamType is not a valid area value structure.");
			}
		};

		prototype struct Volume_Prototype : protected  Length_Prototype<ComponentType, CallerType>
		{
			feature TargetType Value(call_requires(TargetType,Concepts::Is_Volume_Value))
			{		
				return ((Length_Prototype<ComponentType,CallerType>*)this)->Value<TargetType>()*((Length_Prototype<ComponentType,CallerType>*)this)->Conversion_Factor<TargetType>()*((Length_Prototype<ComponentType,CallerType>*)this)->Conversion_Factor<TargetType>();
			}
			feature TargetType Value(call_requires(TargetType,!Concepts::Is_Volume_Value))
			{
				assert_requirements(TargetType,Concepts::Is_Volume_Measure,"Your target type is not identified as a volume measure.")
			}

			feature void Value(TargetType value,call_requires(TargetType,Concepts::Is_Volume_Value))
			{
				((Length_Prototype<ComponentType,CallerType>*)this)->Value<TargetType>(value / ((Length_Prototype<ComponentType,CallerType>*)this)->Conversion_Factor<TargetType>() / ((Length_Prototype<ComponentType,CallerType>*)this)->Conversion_Factor<TargetType>());
			}
			feature void Value(TargetType value, call_requires(TargetType,!Concepts::Is_Volume_Value))
			{
				assert_requirements(TargetType,Concepts::Is_Volume_Measure,"Your target type is not identified as a volume measure.")
			}

			feature static typename TargetType::ReturnType Convert_Value(typename TargetType::ParamType input_value, call_requirements(requires(TargetType,Is_Target_Type_Struct) && requires(typename TargetType::ParamType,Concepts::Is_Volume_Value) && requires(typename TargetType::ReturnType,Concepts::Is_Volume_Value)))
			{
				Value_Type convert_component_value_to_param = Conversion_Factor<TargetType::ParamType>();
				Value_Type convert_component_value_to_return = Conversion_Factor<TargetType::ReturnType>();
				Value_Type conversion = convert_component_value_to_return / convert_component_value_to_param;
				return TargetType::ReturnType((Value_Type)(input_value.Value) * pow(conversion,3.0));
			}
			feature static typename TargetType::ReturnType Convert_Value(typename TargetType::ParamType input_value, call_requirements(requires(TargetType,!Is_Target_Type_Struct) || requires(typename TargetType::ParamType,!Concepts::Is_Volume_Value) || requires(typename TargetType::ParamType,!Concepts::Is_Volume_Value)))
			{
				assert_requirements(TargetType,Is_Target_Type_Struct, "TargetType is not a valid Target_Type_Struct structure.");
				assert_requirements(typename TargetType::ReturnType,Concepts::Is_Volume_Value, "TargetType::ReturnType is not a valid volume value structure.");
				assert_requirements(typename TargetType::ParamType,Concepts::Is_Volume_Value, "TargetType::ParamType is not a valid volume value structure.");
			}
		};

		prototype struct Time_Prototype
		{
			tag_polaris_prototype;

			define_get_set_exists_check(Value);
			feature TargetType Value(call_requirements(requires(ComponentType,get_exists_check(Value)) && requires(TargetType,Concepts::Is_Time_Value)))
			{
				return TargetType(cast_self_to_component().Value<ComponentType,CallerType,Value_Type>() * Conversion_Factor<TargetType>());
			}
			feature void Value(TargetType value, call_requires(ComponentType,set_exists_check(Value)))
			{
				cast_self_to_component().Value<ComponentType,CallerType,Value_Type>(value / Conversion_Factor<TargetType>());
			}
			
			feature static typename TargetType::ReturnType Convert_Value(typename TargetType::ParamType input_value, call_requirements(requires(TargetType,Is_Target_Type_Struct) && requires(typename TargetType::ParamType,Concepts::Is_Time_Value) && requires(typename TargetType::ReturnType,Concepts::Is_Time_Value)))
			{
				Value_Type convert_component_value_to_param = Time_Prototype::Conversion_Factor<TargetType::ParamType>();
				Value_Type convert_component_value_to_return = Time_Prototype::Conversion_Factor<TargetType::ReturnType>();
				return TargetType::ReturnType((Value_Type)(input_value.Value) * convert_component_value_to_return / convert_component_value_to_param);
			}
			feature static typename TargetType::ReturnType Convert_Value(typename TargetType::ParamType input_value, call_requirements(requires(TargetType,!Is_Target_Type_Struct) || requires(typename TargetType::ParamType,!Concepts::Is_Time_Value) || requires(typename TargetType::ReturnType,!Concepts::Is_Time_Value)))
			{
				assert_requirements(TargetType,Is_Target_Type_Struct,"TargetType is not a valid target type struct.");
				assert_requirements(typename TargetType::ParamType,Concepts::Is_Time_Value,"TargetType::ParamType is not a valid Time type.");
				assert_requirements(typename TargetType::ReturnType,Concepts::Is_Time_Value,"TargetType::ReturnType is not a valid Time type.");
			}

			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Time_In_Days) && requires(ComponentType,Concepts::Time_In_Days)))
			{
				return (Value_Type)(1.0);
			};
			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Time_In_DRSeconds) && requires(ComponentType,Concepts::Time_In_Days)))
			{
				return (Value_Type)(864000.0);
			};
			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Time_In_Hours) && requires(ComponentType,Concepts::Time_In_Days)))
			{
				return (Value_Type)(24.0);
			};
			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Time_In_Minutes) && requires(ComponentType,Concepts::Time_In_Days)))
			{
				return (Value_Type)(1440.0);
			};
			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Time_In_Seconds) && requires(ComponentType,Concepts::Time_In_Days)))
			{
				return (Value_Type)(86400.0);
			};

			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Time_In_Days) && requires(ComponentType,Concepts::Time_In_DRSeconds)))
			{
				return (Value_Type)(1.0 / 864000.0);
			};
			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Time_In_DRSeconds) && requires(ComponentType,Concepts::Time_In_DRSeconds)))
			{
				return (Value_Type)(1.0);
			};
			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Time_In_Hours) && requires(ComponentType,Concepts::Time_In_DRSeconds)))
			{
				return (Value_Type)(1.0/36000.0);
			};
			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Time_In_Minutes) && requires(ComponentType,Concepts::Time_In_DRSeconds)))
			{
				return (Value_Type)(1.0/600.0);
			};
			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Time_In_Seconds) && requires(ComponentType,Concepts::Time_In_DRSeconds)))
			{
				return (Value_Type)(0.1);
			};

			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Time_In_Days) && requires(ComponentType,Concepts::Time_In_Hours)))
			{
				return (Value_Type)(1.0/24.0);
			};
			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Time_In_DRSeconds) && requires(ComponentType,Concepts::Time_In_Hours)))
			{
				return (Value_Type)(36000.0);
			};
			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Time_In_Hours) && requires(ComponentType,Concepts::Time_In_Hours)))
			{
				return (Value_Type)(1.0);
			};
			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Time_In_Minutes) && requires(ComponentType,Concepts::Time_In_Hours)))
			{
				return (Value_Type)(60.0);
			};
			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Time_In_Seconds) && requires(ComponentType,Concepts::Time_In_Hours)))
			{
				return (Value_Type)(3600.0);
			};

			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Time_In_Days) && requires(ComponentType,Concepts::Time_In_Minutes)))
			{
				return (Value_Type)(1.0/1440.0);
			};
			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Time_In_DRSeconds) && requires(ComponentType,Concepts::Time_In_Minutes)))
			{
				return (Value_Type)(600.0);
			};
			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Time_In_Hours) && requires(ComponentType,Concepts::Time_In_Minutes)))
			{
				return (Value_Type)(1.0/60.0);
			};
			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Time_In_Minutes) && requires(ComponentType,Concepts::Time_In_Minutes)))
			{
				return (Value_Type)(1.0);
			};
			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Time_In_Seconds) && requires(ComponentType,Concepts::Time_In_Minutes)))
			{
				return (Value_Type)(60.0);
			};

			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Time_In_Days) && requires(ComponentType,Concepts::Time_In_Seconds)))
			{
				return (Value_Type)(1.0/ 86400.0);
			};
			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Time_In_DRSeconds) && requires(ComponentType,Concepts::Time_In_Seconds)))
			{
				return (Value_Type)(10.0);
			};
			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Time_In_Hours) && requires(ComponentType,Concepts::Time_In_Seconds)))
			{
				return (Value_Type)(1.0/3600.0);
			};
			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Time_In_Minutes) && requires(ComponentType,Concepts::Time_In_Seconds)))
			{
				return (Value_Type)(1.0/60.0);
			};
			feature static Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Time_In_Seconds) && requires(ComponentType,Concepts::Time_In_Seconds)))
			{
				return (Value_Type)(1.0);
			};
			//================================================================================================
			// Time value accessors for:
			//------------------------------------------------------------------------------------------------
			/// Time component accessor - returns the requested portion of the current time
			feature_accessor(Time_Component);
			/// Display the time
			feature void Write()
			{
				//cout <<"Total Seconds: "<<pthis->Total_Seconds<ComponentType, CallerType, TargetType>()<<endl;
				cout << "Day "<< this->Time_Component<Data_Structures::Time_Days>()<<":  ";
				cout << this->Time_Component<Data_Structures::Time_Hours>()<<":"<<this->Time_Component<Data_Structures::Time_Minutes>()<<":"<<this->Time_Component<Data_Structures::Time_Seconds>()<<"."<< this->Time_Component<Data_Structures::Time_DRSeconds>();
			}
		};

		prototype struct Rate_Prototype : protected Time_Prototype<ComponentType,CallerType>
		{
			tag_polaris_prototype;

			feature TargetType Length()
			{
				return TargetType(cast_self_to_component().length<ComponentType,CallerType,Value_Type>() * Conversion_Factor<TargetType>());
			}
			feature void Length(TargetType value)
			{
				cast_self_to_component().length<ComponentType,CallerType,Value_Type>(value / Conversion_Factor<TargetType>());
			}

			feature Value_Type Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Centimeters) && requires(ComponentType,Concepts::Measure_in_Centimeters)))
			{
				return 1.0f
			};
		};
	}

	namespace Data_Structures
	{
		typedef Value_Type length_data_type;
		typedef Value_Type time_data_type;
		typedef Value_Type rate_data_type;

		Basic_Data_Struct(Inches, length_data_type, Length_Type, Inches_Type);
		Basic_Data_Struct(Feet, length_data_type, Length_Type, Feet_Type);
		Basic_Data_Struct(Miles, length_data_type, Length_Type, Miles_Type);
		Basic_Data_Struct(Centimeters, length_data_type, Length_Type, Centimeters_Type);
		Basic_Data_Struct(Meters, length_data_type, Length_Type, Meters_Type);
		Basic_Data_Struct(Kilometers, length_data_type, Length_Type, Kilometers_Type);

		Basic_Data_Struct(Square_Inches, length_data_type, Length_Type, Area_Type, Inches_Type);
		Basic_Data_Struct(Square_Feet, length_data_type, Length_Type, Area_Type, Feet_Type);
		Basic_Data_Struct(Square_Miles, length_data_type, Length_Type, Area_Type, Miles_Type);
		Basic_Data_Struct(Square_Centimeters, length_data_type, Length_Type, Area_Type, Centimeters_Type);
		Basic_Data_Struct(Square_Meters, length_data_type, Length_Type, Area_Type,  Meters_Type);
		Basic_Data_Struct(Square_Kilometers, length_data_type, Length_Type, Area_Type, Kilometers_Type);

		Basic_Data_Struct(Cubic_Inches, length_data_type, Length_Type, Volume_Type, Inches_Type);
		Basic_Data_Struct(Cubic_Feet, length_data_type, Length_Type, Volume_Type, Feet_Type);
		Basic_Data_Struct(Cubic_Miles, length_data_type, Length_Type, Volume_Type, Miles_Type);
		Basic_Data_Struct(Cubic_Centimeters, length_data_type, Length_Type, Volume_Type, Centimeters_Type);
		Basic_Data_Struct(Cubic_Meters, length_data_type, Length_Type, Volume_Type, Meters_Type);
		Basic_Data_Struct(Cubic_Kilometers, length_data_type, Length_Type, Volume_Type, Kilometers_Type);

		Basic_Data_Struct(Time_DRSeconds,time_data_type,Time_Type,DRSeconds_Type);
		Basic_Data_Struct(Time_Seconds,time_data_type,Time_Type,Seconds_Type);
		Basic_Data_Struct(Time_Minutes,time_data_type,Time_Type,Minutes_Type);
		Basic_Data_Struct(Time_Hours,time_data_type,Time_Type,Hours_Type);
		Basic_Data_Struct(Time_Days,time_data_type,Time_Type,Days_Type);

		Basic_Data_Struct(Units_Per_Second,rate_data_type,Time_Type,Seconds_Type);
		Basic_Data_Struct(Units_Per_Minute,rate_data_type,Time_Type,Minutes_Type);
		Basic_Data_Struct(Units_Per_Hour,rate_data_type,Time_Type,Hours_Type);
		Basic_Data_Struct(Units_Per_Day,rate_data_type,Time_Type,Days_Type);

	}

}