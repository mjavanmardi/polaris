#pragma once
#include "Repository_Base_Includes.h"

namespace Basic_Units
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

	namespace Prototypes
	{
		//------------------------------------------------------------------------------------------------------------------
		/// RENAME THE Inteface struct below.  This is the inteface to a POLARIS component
		//------------------------------------------------------------------------------------------------------------------
		prototype struct Length_Prototype
		{
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Centimeters) && requires(ComponentType,Concepts::Length_in_Centimeters)))
			{
				return Data_Structures::Length_Centimeters(cast_self_to_component().length<ComponentType,CallerType,float>());
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Feet) && requires(ComponentType,Concepts::Length_in_Centimeters)))
			{		
				return Data_Structures::Length_Feet(cast_self_to_component().length<ComponentType,CallerType,float>() * 0.0328084);
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Inches) && requires(ComponentType,Concepts::Length_in_Centimeters)))
			{
				return Data_Structures::Length_Inches(cast_self_to_component().length<ComponentType,CallerType,float>() * 0.393701);				
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Kilometers) && requires(ComponentType,Concepts::Length_in_Centimeters)))
			{
				return Data_Structures::Length_Kilometers(cast_self_to_component().length<ComponentType,CallerType,float>() / 100000.0);
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Meters) && requires(ComponentType,Concepts::Length_in_Centimeters)))
			{
				return Data_Structures::Length_Meters(cast_self_to_component().length<ComponentType,CallerType,float>() / 100.0);
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Miles) && requires(ComponentType,Concepts::Length_in_Centimeters)))
			{
				return Data_Structures::Length_Miles(cast_self_to_component().length<ComponentType,CallerType,float>() / 160934.0);
			};

			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Centimeters) && requires(ComponentType,Concepts::Length_in_Feet)))
			{
				return Data_Structures::Length_Centimeters(cast_self_to_component().length<ComponentType,CallerType,float>() * 30.48);
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Feet) && requires(ComponentType,Concepts::Length_in_Feet)))
			{		
				return Data_Structures::Length_Feet(cast_self_to_component().length<ComponentType,CallerType,float>());
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Inches) && requires(ComponentType,Concepts::Length_in_Feet)))
			{
				return Data_Structures::Length_Inches(cast_self_to_component().length<ComponentType,CallerType,float>() * 12.0);				
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Kilometers) && requires(ComponentType,Concepts::Length_in_Feet)))
			{
				return Data_Structures::Length_Kilometers(cast_self_to_component().length<ComponentType,CallerType,float>() / 3280.84);
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Meters) && requires(ComponentType,Concepts::Length_in_Feet)))
			{
				return Data_Structures::Length_Meters(cast_self_to_component().length<ComponentType,CallerType,float>() / 3.28084);
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Miles) && requires(ComponentType,Concepts::Length_in_Feet)))
			{
				return Data_Structures::Length_Miles(cast_self_to_component().length<ComponentType,CallerType,float>() / 5280.0);
			};

			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Centimeters) && requires(ComponentType,Concepts::Length_in_Inches)))
			{
				return Data_Structures::Length_Centimeters(cast_self_to_component().length<ComponentType,CallerType,float>() * 30.48 / 12.0);
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Feet) && requires(ComponentType,Concepts::Length_in_Inches)))
			{		
				return Data_Structures::Length_Feet(cast_self_to_component().length<ComponentType,CallerType,float>() / 12.0);
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Inches) && requires(ComponentType,Concepts::Length_in_Inches)))
			{
				return Data_Structures::Length_Inches(cast_self_to_component().length<ComponentType,CallerType,float>());				
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Kilometers) && requires(ComponentType,Concepts::Length_in_Inches)))
			{
				return Data_Structures::Length_Kilometers(cast_self_to_component().length<ComponentType,CallerType,float>() / 3280.84 / 12.0);
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Meters) && requires(ComponentType,Concepts::Length_in_Inches)))
			{
				return Data_Structures::Length_Meters(cast_self_to_component().length<ComponentType,CallerType,float>() / 3.28084 / 12.0);
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Miles) && requires(ComponentType,Concepts::Length_in_Inches)))
			{
				return Data_Structures::Length_Miles(cast_self_to_component().length<ComponentType,CallerType,float>() / 5280.0 / 12.0);
			};

			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Centimeters) && requires(ComponentType,Concepts::Length_in_Kilometers)))
			{
				return Data_Structures::Length_Centimeters(cast_self_to_component().length<ComponentType,CallerType,float>() * 100000.0);
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Feet) && requires(ComponentType,Concepts::Length_in_Kilometers)))
			{		
				return Data_Structures::Length_Feet(cast_self_to_component().length<ComponentType,CallerType,float>() * 3280.84);
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Inches) && requires(ComponentType,Concepts::Length_in_Kilometers)))
			{
				return Data_Structures::Length_Inches(cast_self_to_component().length<ComponentType,CallerType,float>() * 3280.84 * 12.0);				
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Kilometers) && requires(ComponentType,Concepts::Length_in_Kilometers)))
			{
				return Data_Structures::Length_Kilometers(cast_self_to_component().length<ComponentType,CallerType,float>());
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Meters) && requires(ComponentType,Concepts::Length_in_Kilometers)))
			{
				return Data_Structures::Length_Meters(cast_self_to_component().length<ComponentType,CallerType,float>() * 1000.0);
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Miles) && requires(ComponentType,Concepts::Length_in_Kilometers)))
			{
				return Data_Structures::Length_Miles(cast_self_to_component().length<ComponentType,CallerType,float>() * 0.621371);
			};
				
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Centimeters) && requires(ComponentType,Concepts::Length_in_Meters)))
			{
				return Data_Structures::Length_Centimeters(cast_self_to_component().length<ComponentType,CallerType,float>() * 100.0);
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Feet) && requires(ComponentType,Concepts::Length_in_Meters)))
			{		
				return Data_Structures::Length_Feet(cast_self_to_component().length<ComponentType,CallerType,float>() * 3.28084);
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Inches) && requires(ComponentType,Concepts::Length_in_Meters)))
			{
				return Data_Structures::Length_Inches(cast_self_to_component().length<ComponentType,CallerType,float>() * 3.28084 * 12.0);				
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Kilometers) && requires(ComponentType,Concepts::Length_in_Meters)))
			{
				return Data_Structures::Length_Kilometers(cast_self_to_component().length<ComponentType,CallerType,float>() / 1000.0);
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Meters) && requires(ComponentType,Concepts::Length_in_Meters)))
			{
				return Data_Structures::Length_Meters(cast_self_to_component().length<ComponentType,CallerType,float>());
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Miles) && requires(ComponentType,Concepts::Length_in_Meters)))
			{
				return Data_Structures::Length_Miles(cast_self_to_component().length<ComponentType,CallerType,float>() * 0.000621371);
			};
					
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Centimeters) && requires(ComponentType,Concepts::Length_in_Miles)))
			{
				return Data_Structures::Length_Centimeters(cast_self_to_component().length<ComponentType,CallerType,float>() * 160934.0);
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Feet) && requires(ComponentType,Concepts::Length_in_Miles)))
			{		
				return Data_Structures::Length_Feet(cast_self_to_component().length<ComponentType,CallerType,float>() * 5280.0);
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Inches) && requires(ComponentType,Concepts::Length_in_Miles)))
			{
				return Data_Structures::Length_Inches(cast_self_to_component().length<ComponentType,CallerType,float>() * 5280.0 * 12.0);				
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Kilometers) && requires(ComponentType,Concepts::Length_in_Miles)))
			{
				return Data_Structures::Length_Kilometers(cast_self_to_component().length<ComponentType,CallerType,float>() * 1.60934);
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Meters) && requires(ComponentType,Concepts::Length_in_Miles)))
			{
				return Data_Structures::Length_Meters(cast_self_to_component().length<ComponentType,CallerType,float>() * 1609.34);
			};
			feature TargetType Length(call_requirements(requires(TargetType,Concepts::Length_in_Miles) && requires(ComponentType,Concepts::Length_in_Miles)))
			{
				return Data_Structures::Length_Miles(cast_self_to_component().length<ComponentType,CallerType,float>());
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
	}

	namespace Implementations
	{
		implementation struct Length
		{
			// LENGTH - stored in feet
			typedef true_type Length_Type;
			member_data_basic(float,length);
		};
		implementation struct Length_in_inches : Length<MasterType>
		{
			typedef true_type Inches;
		};
		implementation struct Length_in_feet : Length<MasterType>
		{
			typedef true_type Feet;
		};
		implementation struct Length_in_miles : Length<MasterType>
		{
			typedef true_type Miles;
		};
		implementation struct Length_in_centimeters : Length<MasterType>
		{
			typedef true_type Centimeters;
		};
		implementation struct Length_in_meters : Length<MasterType>
		{
			typedef true_type Meters;
		};
		implementation struct Length_in_kilometers : Length<MasterType>
		{
			typedef true_type Kilometers;
		};
	}

	namespace Components
	{
		implementation struct Length_in_inches
		{
			typedef Polaris_Component<Implementations::Length_in_inches, MasterType> type;
		};
		implementation struct Length_in_feet
		{
			typedef Polaris_Component<Implementations::Length_in_feet,MasterType> type;
		};
		implementation struct Length_in_miles
		{
			typedef Polaris_Component<Implementations::Length_in_miles,MasterType> type;
		};
		implementation struct Length_in_centimeters
		{
			typedef Polaris_Component<Implementations::Length_in_centimeters,MasterType> type;
		};
		implementation struct Length_in_meters
		{
			typedef Polaris_Component<Implementations::Length_in_meters,MasterType> type;
		};
		implementation struct Length_in_kilometers
		{
			typedef Polaris_Component<Implementations::Length_in_kilometers,MasterType> type;
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