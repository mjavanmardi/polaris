#pragma once
#include "Repository_Base_Includes.h"

namespace Basic_Units
{
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

		namespace Time_Types
		{
			typedef true_type Time_Type;
			typedef true_type Days;
			typedef true_type Hours;
			typedef true_type Minutes;
			typedef true_type Seconds;
			typedef true_type DRSeconds; 
		}
	}

	namespace Concepts
	{
		#pragma region // Length Concepts
		concept Length_in_Inches
		{
			begin_requirements_list(none);
			requires_typename_state(none, Inches_Type, true_type,"Length is not in Inches");
			requires_typename_state(Inches_Type, Length_Type, true_type,"Type is not a length type");
			end_requirements_list(Length_Type);
		};
		concept Length_in_Feet
		{
			begin_requirements_list(none);
			requires_typename_state(none, Feet_Type, true_type,"Length is not in feet");
			requires_typename_state(Feet_Type, Length_Type, true_type,"Type is not a length type");
			end_requirements_list(Length_Type);
		};
		concept Length_in_Miles
		{
			begin_requirements_list(none);
			requires_typename_state(none, Miles_Type, true_type,"Length is not in Miles");
			requires_typename_state(Miles_Type, Length_Type, true_type,"Type is not a length type");
			end_requirements_list(Length_Type);
		};
		concept Length_in_Centimeters
		{
			begin_requirements_list(none);
			requires_typename_state(none, Centimeters_Type, true_type,"Length is not in Centimeters");
			requires_typename_state(Centimeters_Type, Length_Type, true_type,"Type is not a length type");
			end_requirements_list(Length_Type);
		};
		concept Length_in_Meters
		{
			begin_requirements_list(none);
			requires_typename_state(none, Meters_Type, true_type,"Length is not in Meters");
			requires_typename_state(Meters_Type, Length_Type, true_type,"Type is not a length type");
			end_requirements_list(Length_Type);
		};
		concept Length_in_Kilometers
		{
			begin_requirements_list(none);
			requires_typename_state(none, Kilometers_Type, true_type,"Length is not in Kilometers");
			requires_typename_state(Kilometers_Type, Length_Type, true_type,"Type is not a length type");
			end_requirements_list(Length_Type);
		};
		#pragma endregion

		#pragma region // Time Concepts
		/// Check if the class has an arithmetic 'Time' member defined
		concept Has_Time
		{
			begin_requirements_list(none);
			requires_typename_state(none,TimeType,true_type,"Type is not a Time data structure");
			//requires_concept_checked_member(TimeType, value, is_arithmetic,"An arithmetic 'Time' data member has not been defined.");
			end_requirements_list(TimeType);
		};
		/// Check if initializing struct is tagged as DRSeconds (i.e. 1/10th of a second)
		concept Time_In_DRSeconds
		{
			begin_requirements_list(none);
			requires_concept(none,Has_Time);
			requires_typename_state(Has_Time,DRSeconds,true_type,"Type is not in DRSeconds");
			end_requirements_list(DRSeconds);
		};
		/// Check if initializing struct is tagged as Seconds
		concept Time_In_Seconds
		{
			begin_requirements_list(none);
			requires_concept(none,Has_Time);
			requires_typename_state(Has_Time,Seconds,true_type,"Type is not in Seconds");
			end_requirements_list(Seconds);
		};
		/// Check if initializing struct is tagged as Minutes
		concept Time_In_Minutes
		{
			begin_requirements_list(none);
			requires_concept(none,Has_Time);
			requires_typename_state(Has_Time,Minutes,true_type,"Type is not in Minutes");
			end_requirements_list(Minutes);
		};
		/// Check if initializing struct is tagged as Hours
		concept Time_In_Hours
		{
			begin_requirements_list(none);
			requires_concept(none,Has_Time);
			requires_typename_state(Has_Time,Hours,true_type,"Type is not in Hours");
			end_requirements_list(Hours);
		};
		/// Check if initializing struct is tagged as Days
		concept Time_In_Days
		{
			begin_requirements_list(none);
			requires_concept(none,Has_Time);
			requires_typename_state(Has_Time,Days,true_type,"Type is not in Hours");
			end_requirements_list(Days);
		};
		/// Check if a type meets the requirements of being a basic 'Time' class
		concept Is_Time
		{
			begin_requirements_list(none);
			requires_typename_state(none, TimeType, true_type,"This class has no TimeType = true_type defined");
			end_requirements_list(TimeType);
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

			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Centimeters) && requires(ComponentType,Concepts::Length_in_Centimeters)))
			{
				return Data_Structures::Centimeters(cast_self_to_component().length<ComponentType,CallerType,float>());
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Feet) && requires(ComponentType,Concepts::Length_in_Centimeters)))
			{		
				return Data_Structures::Feet(cast_self_to_component().length<ComponentType,CallerType,float>() * 0.0328084);
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Inches) && requires(ComponentType,Concepts::Length_in_Centimeters)))
			{
				return Data_Structures::Inches(cast_self_to_component().length<ComponentType,CallerType,float>() * 0.393701);				
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Kilometers) && requires(ComponentType,Concepts::Length_in_Centimeters)))
			{
				return Data_Structures::Kilometers(cast_self_to_component().length<ComponentType,CallerType,float>() / 100000.0);
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Meters) && requires(ComponentType,Concepts::Length_in_Centimeters)))
			{
				return Data_Structures::Meters(cast_self_to_component().length<ComponentType,CallerType,float>() / 100.0);
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Miles) && requires(ComponentType,Concepts::Length_in_Centimeters)))
			{
				return Data_Structures::Miles(cast_self_to_component().length<ComponentType,CallerType,float>() / 160934.0);
			};

			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Centimeters) && requires(ComponentType,Concepts::Length_in_Feet)))
			{
				return Data_Structures::Centimeters(cast_self_to_component().length<ComponentType,CallerType,float>() * 30.48);
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Feet) && requires(ComponentType,Concepts::Length_in_Feet)))
			{		
				return Data_Structures::Feet(cast_self_to_component().length<ComponentType,CallerType,float>());
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Inches) && requires(ComponentType,Concepts::Length_in_Feet)))
			{
				return Data_Structures::Inches(cast_self_to_component().length<ComponentType,CallerType,float>() * 12.0);				
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Kilometers) && requires(ComponentType,Concepts::Length_in_Feet)))
			{
				return Data_Structures::Kilometers(cast_self_to_component().length<ComponentType,CallerType,float>() / 3280.84);
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Meters) && requires(ComponentType,Concepts::Length_in_Feet)))
			{
				return Data_Structures::Meters(cast_self_to_component().length<ComponentType,CallerType,float>() / 3.28084);
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Miles) && requires(ComponentType,Concepts::Length_in_Feet)))
			{
				return Data_Structures::Miles(cast_self_to_component().length<ComponentType,CallerType,float>() / 5280.0);
			};

			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Centimeters) && requires(ComponentType,Concepts::Length_in_Inches)))
			{
				return Data_Structures::Centimeters(cast_self_to_component().length<ComponentType,CallerType,float>() * 30.48 / 12.0);
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Feet) && requires(ComponentType,Concepts::Length_in_Inches)))
			{		
				return Data_Structures::Feet(cast_self_to_component().length<ComponentType,CallerType,float>() / 12.0);
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Inches) && requires(ComponentType,Concepts::Length_in_Inches)))
			{
				return Data_Structures::Inches(cast_self_to_component().length<ComponentType,CallerType,float>());				
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Kilometers) && requires(ComponentType,Concepts::Length_in_Inches)))
			{
				return Data_Structures::Kilometers(cast_self_to_component().length<ComponentType,CallerType,float>() / 3280.84 / 12.0);
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Meters) && requires(ComponentType,Concepts::Length_in_Inches)))
			{
				return Data_Structures::Meters(cast_self_to_component().length<ComponentType,CallerType,float>() / 3.28084 / 12.0);
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Miles) && requires(ComponentType,Concepts::Length_in_Inches)))
			{
				return Data_Structures::Miles(cast_self_to_component().length<ComponentType,CallerType,float>() / 5280.0 / 12.0);
			};

			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Centimeters) && requires(ComponentType,Concepts::Length_in_Kilometers)))
			{
				return Data_Structures::Centimeters(cast_self_to_component().length<ComponentType,CallerType,float>() * 100000.0);
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Feet) && requires(ComponentType,Concepts::Length_in_Kilometers)))
			{		
				return Data_Structures::Feet(cast_self_to_component().length<ComponentType,CallerType,float>() * 3280.84);
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Inches) && requires(ComponentType,Concepts::Length_in_Kilometers)))
			{
				return Data_Structures::Inches(cast_self_to_component().length<ComponentType,CallerType,float>() * 3280.84 * 12.0);				
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Kilometers) && requires(ComponentType,Concepts::Length_in_Kilometers)))
			{
				return Data_Structures::Kilometers(cast_self_to_component().length<ComponentType,CallerType,float>());
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Meters) && requires(ComponentType,Concepts::Length_in_Kilometers)))
			{
				return Data_Structures::Meters(cast_self_to_component().length<ComponentType,CallerType,float>() * 1000.0);
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Miles) && requires(ComponentType,Concepts::Length_in_Kilometers)))
			{
				return Data_Structures::Miles(cast_self_to_component().length<ComponentType,CallerType,float>() * 0.621371);
			};
				
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Centimeters) && requires(ComponentType,Concepts::Length_in_Meters)))
			{
				return Data_Structures::Centimeters(cast_self_to_component().length<ComponentType,CallerType,float>() * 100.0);
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Feet) && requires(ComponentType,Concepts::Length_in_Meters)))
			{		
				return Data_Structures::Feet(cast_self_to_component().length<ComponentType,CallerType,float>() * 3.28084);
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Inches) && requires(ComponentType,Concepts::Length_in_Meters)))
			{
				return Data_Structures::Inches(cast_self_to_component().length<ComponentType,CallerType,float>() * 3.28084 * 12.0);				
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Kilometers) && requires(ComponentType,Concepts::Length_in_Meters)))
			{
				return Data_Structures::Kilometers(cast_self_to_component().length<ComponentType,CallerType,float>() / 1000.0);
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Meters) && requires(ComponentType,Concepts::Length_in_Meters)))
			{
				return Data_Structures::Meters(cast_self_to_component().length<ComponentType,CallerType,float>());
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Miles) && requires(ComponentType,Concepts::Length_in_Meters)))
			{
				return Data_Structures::Miles(cast_self_to_component().length<ComponentType,CallerType,float>() * 0.000621371);
			};
					
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Centimeters) && requires(ComponentType,Concepts::Length_in_Miles)))
			{
				return Data_Structures::Centimeters(cast_self_to_component().length<ComponentType,CallerType,float>() * 160934.0);
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Feet) && requires(ComponentType,Concepts::Length_in_Miles)))
			{		
				return Data_Structures::Feet(cast_self_to_component().length<ComponentType,CallerType,float>() * 5280.0);
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Inches) && requires(ComponentType,Concepts::Length_in_Miles)))
			{
				return Data_Structures::Inches(cast_self_to_component().length<ComponentType,CallerType,float>() * 5280.0 * 12.0);				
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Kilometers) && requires(ComponentType,Concepts::Length_in_Miles)))
			{
				return Data_Structures::Kilometers(cast_self_to_component().length<ComponentType,CallerType,float>() * 1.60934);
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Meters) && requires(ComponentType,Concepts::Length_in_Miles)))
			{
				return Data_Structures::Meters(cast_self_to_component().length<ComponentType,CallerType,float>() * 1609.34);
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Miles) && requires(ComponentType,Concepts::Length_in_Miles)))
			{
				return Data_Structures::Miles(cast_self_to_component().length<ComponentType,CallerType,float>());
			};

			feature TargetType Length(call_requirements(!(
				(requires(TargetType,Concepts::Length_in_Centimeters) || requires(TargetType,Concepts::Length_in_Feet) || requires(TargetType,Concepts::Length_in_Inches) || 
				requires(TargetType,Concepts::Length_in_Kilometers) || requires(TargetType,Concepts::Length_in_Meters) || requires(TargetType,Concepts::Length_in_Miles)) &&
				(requires(ComponentType,Concepts::Length_in_Centimeters) || requires(ComponentType,Concepts::Length_in_Feet) || requires(ComponentType,Concepts::Length_in_Inches) || 
				requires(ComponentType,Concepts::Length_in_Kilometers) || requires(ComponentType,Concepts::Length_in_Meters) || requires(ComponentType,Concepts::Length_in_Miles))) ))
			{
				assert_requirements(ComponentType,Concepts::Length_in_Centimeters,"The specified ComponentType is not a valid length component.");
				assert_requirements(TargetType,Concepts::Length_in_Centimeters,"The specified TargetType is not a valid Length Data Structure.");
			}

			feature void Length(TargetType value,call_requirements(requires(TargetType,Concepts::Length_in_Centimeters) && requires(ComponentType,Concepts::Length_in_Centimeters)))
			{
				cast_self_to_component().length<ComponentType,CallerType,float>(value);
			};
			feature void Length(TargetType value,call_requirements(requires(TargetType,Concepts::Length_in_Feet) && requires(ComponentType,Concepts::Length_in_Centimeters)))
			{		
				cast_self_to_component().length<ComponentType,CallerType,float>(value * 30.48);
			};
			feature void Length(TargetType value,call_requirements(requires(TargetType,Concepts::Length_in_Inches) && requires(ComponentType,Concepts::Length_in_Centimeters)))
			{
				cast_self_to_component().length<ComponentType,CallerType,float>(value * 2.54);				
			};
			feature void Length(TargetType value,call_requirements(requires(TargetType,Concepts::Length_in_Kilometers) && requires(ComponentType,Concepts::Length_in_Centimeters)))
			{
				cast_self_to_component().length<ComponentType,CallerType,float>(value * 100000.0);
			};
			feature void Length(TargetType value,call_requirements(requires(TargetType,Concepts::Length_in_Meters) && requires(ComponentType,Concepts::Length_in_Centimeters)))
			{
				cast_self_to_component().length<ComponentType,CallerType,float>(value * 100.0);
			};
			feature void Length(TargetType value,call_requirements(requires(TargetType,Concepts::Length_in_Miles) && requires(ComponentType,Concepts::Length_in_Centimeters)))
			{
				cast_self_to_component().length<ComponentType,CallerType,float>(value * 160934.0);
			};

			feature void Length(TargetType value,call_requirements(requires(TargetType,Concepts::Length_in_Centimeters) && requires(ComponentType,Concepts::Length_in_Feet)))
			{
				cast_self_to_component().length<ComponentType,CallerType,float>(value / 30.48);
			};
			feature void Length(TargetType value,call_requirements(requires(TargetType,Concepts::Length_in_Feet) && requires(ComponentType,Concepts::Length_in_Feet)))
			{		
				cast_self_to_component().length<ComponentType,CallerType,float>(value);
			};
			feature void Length(TargetType value,call_requirements(requires(TargetType,Concepts::Length_in_Inches) && requires(ComponentType,Concepts::Length_in_Feet)))
			{
				cast_self_to_component().length<ComponentType,CallerType,float>(value / 12.0);				
			};
			feature void Length(TargetType value,call_requirements(requires(TargetType,Concepts::Length_in_Kilometers) && requires(ComponentType,Concepts::Length_in_Feet)))
			{
				cast_self_to_component().length<ComponentType,CallerType,float>(value * 3280.84);
			};
			feature void Length(TargetType value,call_requirements(requires(TargetType,Concepts::Length_in_Meters) && requires(ComponentType,Concepts::Length_in_Feet)))
			{
				cast_self_to_component().length<ComponentType,CallerType,float>(value * 3.28084);
			};
			feature void Length(TargetType value,call_requirements(requires(TargetType,Concepts::Length_in_Miles) && requires(ComponentType,Concepts::Length_in_Feet)))
			{
				cast_self_to_component().length<ComponentType,CallerType,float>(value * 5280.0);
			};

			feature void Length(TargetType value,call_requirements(requires(TargetType,Concepts::Length_in_Centimeters) && requires(ComponentType,Concepts::Length_in_Inches)))
			{
				cast_self_to_component().length<ComponentType,CallerType,float>(value / 2.54);
			};
			feature void Length(TargetType value,call_requirements(requires(TargetType,Concepts::Length_in_Feet) && requires(ComponentType,Concepts::Length_in_Inches)))
			{		
				cast_self_to_component().length<ComponentType,CallerType,float>(value * 12.0);
			};
			feature void Length(TargetType value,call_requirements(requires(TargetType,Concepts::Length_in_Inches) && requires(ComponentType,Concepts::Length_in_Inches)))
			{
				cast_self_to_component().length<ComponentType,CallerType,float>(value);				
			};
			feature void Length(TargetType value,call_requirements(requires(TargetType,Concepts::Length_in_Kilometers) && requires(ComponentType,Concepts::Length_in_Inches)))
			{
				cast_self_to_component().length<ComponentType,CallerType,float>(value * 3280.84 * 12.0);
			};
			feature void Length(TargetType value,call_requirements(requires(TargetType,Concepts::Length_in_Meters) && requires(ComponentType,Concepts::Length_in_Inches)))
			{
				cast_self_to_component().length<ComponentType,CallerType,float>(value * 3.28084 * 12.0);
			};
			feature void Length(TargetType value,call_requirements(requires(TargetType,Concepts::Length_in_Miles) && requires(ComponentType,Concepts::Length_in_Inches)))
			{
				cast_self_to_component().length<ComponentType,CallerType,float>(value * 5280.0 * 12.0);
			};

			feature void Length(TargetType value,call_requirements(requires(TargetType,Concepts::Length_in_Centimeters) && requires(ComponentType,Concepts::Length_in_Kilometers)))
			{
				cast_self_to_component().length<ComponentType,CallerType,float>(value / 100000.0);
			};
			feature void Length(TargetType value,call_requirements(requires(TargetType,Concepts::Length_in_Feet) && requires(ComponentType,Concepts::Length_in_Kilometers)))
			{		
				cast_self_to_component().length<ComponentType,CallerType,float>(value / 3280.84);
			};
			feature void Length(TargetType value,call_requirements(requires(TargetType,Concepts::Length_in_Inches) && requires(ComponentType,Concepts::Length_in_Kilometers)))
			{
				cast_self_to_component().length<ComponentType,CallerType,float>(value / 3280.84 / 12.0);				
			};
			feature void Length(TargetType value,call_requirements(requires(TargetType,Concepts::Length_in_Kilometers) && requires(ComponentType,Concepts::Length_in_Kilometers)))
			{
				cast_self_to_component().length<ComponentType,CallerType,float>(value);
			};
			feature void Length(TargetType value,call_requirements(requires(TargetType,Concepts::Length_in_Meters) && requires(ComponentType,Concepts::Length_in_Kilometers)))
			{
				cast_self_to_component().length<ComponentType,CallerType,float>(value / 1000.0);
			};
			feature void Length(TargetType value,call_requirements(requires(TargetType,Concepts::Length_in_Miles) && requires(ComponentType,Concepts::Length_in_Kilometers)))
			{
				cast_self_to_component().length<ComponentType,CallerType,float>(value / 0.621371);
			};
				
			feature void Length(TargetType value,call_requirements(requires(TargetType,Concepts::Length_in_Centimeters) && requires(ComponentType,Concepts::Length_in_Meters)))
			{
				cast_self_to_component().length<ComponentType,CallerType,float>(value / 100.0);
			};
			feature void Length(TargetType value,call_requirements(requires(TargetType,Concepts::Length_in_Feet) && requires(ComponentType,Concepts::Length_in_Meters)))
			{		
				cast_self_to_component().length<ComponentType,CallerType,float>(value / 3.28084);
			};
			feature void Length(TargetType value,call_requirements(requires(TargetType,Concepts::Length_in_Inches) && requires(ComponentType,Concepts::Length_in_Meters)))
			{
				cast_self_to_component().length<ComponentType,CallerType,float>(value / 3.28084 / 12.0);				
			};
			feature void Length(TargetType value,call_requirements(requires(TargetType,Concepts::Length_in_Kilometers) && requires(ComponentType,Concepts::Length_in_Meters)))
			{
				cast_self_to_component().length<ComponentType,CallerType,float>(value * 1000.0);
			};
			feature void Length(TargetType value,call_requirements(requires(TargetType,Concepts::Length_in_Meters) && requires(ComponentType,Concepts::Length_in_Meters)))
			{
				cast_self_to_component().length<ComponentType,CallerType,float>(value);
			};
			feature void Length(TargetType value,call_requirements(requires(TargetType,Concepts::Length_in_Miles) && requires(ComponentType,Concepts::Length_in_Meters)))
			{
				cast_self_to_component().length<ComponentType,CallerType,float>(value * 1609.34);
			};
					
			feature void Length(TargetType value,call_requirements(requires(TargetType,Concepts::Length_in_Centimeters) && requires(ComponentType,Concepts::Length_in_Miles)))
			{
				cast_self_to_component().length<ComponentType,CallerType,float>(value / 160934.0);
			};
			feature void Length(TargetType value,call_requirements(requires(TargetType,Concepts::Length_in_Feet) && requires(ComponentType,Concepts::Length_in_Miles)))
			{		
				cast_self_to_component().length<ComponentType,CallerType,float>(value / 5280.0);
			};
			feature void Length(TargetType value,call_requirements(requires(TargetType,Concepts::Length_in_Inches) && requires(ComponentType,Concepts::Length_in_Miles)))
			{
				cast_self_to_component().length<ComponentType,CallerType,float>(value / 5280.0 / 12.0);				
			};
			feature void Length(TargetType value,call_requirements(requires(TargetType,Concepts::Length_in_Kilometers) && requires(ComponentType,Concepts::Length_in_Miles)))
			{
				cast_self_to_component().length<ComponentType,CallerType,float>(value / 1.60934);
			};
			feature void Length(TargetType value,call_requirements(requires(TargetType,Concepts::Length_in_Meters) && requires(ComponentType,Concepts::Length_in_Miles)))
			{
				cast_self_to_component().length<ComponentType,CallerType,float>(value / 1609.34);
			};
			feature void Length(TargetType value,call_requirements(requires(TargetType,Concepts::Length_in_Miles) && requires(ComponentType,Concepts::Length_in_Miles)))
			{
				cast_self_to_component().length<ComponentType,CallerType,float>(value);
			};

			feature void Length(TargetType value,call_requirements(!(
				(requires(TargetType,Concepts::Length_in_Centimeters) || requires(TargetType,Concepts::Length_in_Feet) || requires(TargetType,Concepts::Length_in_Inches) || 
				requires(TargetType,Concepts::Length_in_Kilometers) || requires(TargetType,Concepts::Length_in_Meters) || requires(TargetType,Concepts::Length_in_Miles)) &&
				(requires(ComponentType,Concepts::Length_in_Centimeters) || requires(ComponentType,Concepts::Length_in_Feet) || requires(ComponentType,Concepts::Length_in_Inches) || 
				requires(ComponentType,Concepts::Length_in_Kilometers) || requires(ComponentType,Concepts::Length_in_Meters) || requires(ComponentType,Concepts::Length_in_Miles))) ))
			{
				assert_requirements(ComponentType,Concepts::Length_in_Centimeters,"The specified ComponentType is not a valid length component.");
				assert_requirements(TargetType,Concepts::Length_in_Centimeters,"The specified TargetType is not a valid Length Data Structure.");
			}
		};

		prototype struct Width_Prototype : public Length_Prototype<ComponentType,CallerType>
		{

			feature TargetType Width()
			{
				return ((Length_Prototype<ComponentType,CallerType>*)this)->Length<TargetType>();
			}

			feature void Width(TargetType value)
			{
				((Length_Prototype<ComponentType,CallerType>*)this)->Length<TargetType>(value);
			}
		};

		prototype struct Height_Prototype : public Length_Prototype<ComponentType,CallerType>
		{
			feature TargetType Height()
			{
				return ((Length_Prototype<ComponentType,CallerType>*)this)->Length<TargetType>();
			}

			feature void Height(TargetType value)
			{
				((Length_Prototype<ComponentType,CallerType>*)this)->Length<TargetType>(value);
			}
		};

		prototype struct Time_Prototype
		{
			tag_polaris_prototype;

			/// Initializer
			feature void Initialize(TargetType t)
			{
				cast_self_to_component().Initialize<ComponentType,CallerType,TargetType>(t);
			}

			/// Adding time
			feature void Add_Time(TargetType t)
			{
				cast_self_to_component().Add_Time<ComponentType,CallerType,TargetType>(t);
			}

			/// Display the time
			feature void Write()
			{
				//cout <<"Total Seconds: "<<pthis->Total_Seconds<ComponentType, CallerType, TargetType>()<<endl;
				cout << "Day "<< this->Time_Component<Data_Structures::Time_Days>()<<":  ";
				cout << this->Time_Component<Data_Structures::Time_Hours>()<<":"<<this->Time_Component<Data_Structures::Time_Minutes>()<<":"<<this->Time_Component<Data_Structures::Time_Seconds>()<<"."<< this->Time_Component<Data_Structures::Time_DRSeconds>();
			}

			//================================================================================================
			// Time value accessors for:
			//------------------------------------------------------------------------------------------------
			/// Time component accessor - returns the requested portion of the current time
			feature_accessor(Time_Component);
			/// Returns the value of the current time converted to the requested time units
			feature_accessor(Total_Time);
		};
	}

	namespace Data_Structures
	{
		typedef float length_data_type;
		typedef float time_data_type;
		Basic_Data_Struct(Inches, length_data_type, Length_Type, Inches_Type);
		Basic_Data_Struct(Feet, length_data_type, Length_Type, Feet_Type);
		Basic_Data_Struct(Miles, length_data_type, Length_Type, Miles_Type);
		Basic_Data_Struct(Centimeters, length_data_type, Length_Type, Centimeters_Type);
		Basic_Data_Struct(Meters, length_data_type, Length_Type, Meters_Type);
		Basic_Data_Struct(Kilometers, length_data_type, Length_Type, Kilometers_Type);

		Basic_Data_Struct(Time_DRSeconds,time_data_type,Time_Type,DRSeconds);
		Basic_Data_Struct(Time_Seconds,time_data_type,Time_Type,Seconds);
		Basic_Data_Struct(Time_Minutes,time_data_type,Time_Type,Minutes);
		Basic_Data_Struct(Time_Hours,time_data_type,Time_Type,Hours);
		Basic_Data_Struct(Time_Days,time_data_type,Time_Type,Days);
	}

}