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
		#pragma endregion

		#pragma region // Area Concepts
		concept Is_Area_Measure
		{
			begin_requirements_list(none);
			requires_typename_state(none, Area_Type, true_type,"Type is not tagged as a valid area type measure");
			requires_typename_state(Area_Type, Length_Type, true_type,"Type does not derive from a valid length type");
			end_requirements_list(Length_Type);
		};
		#pragma endregion

		#pragma region // Volume Concepts
		concept Is_Volume_Measure
		{
			begin_requirements_list(none);
			requires_typename_state(none, Volume_Type, true_type,"Type is not a valid volume type measure");
			requires_typename_state(Volume_Type, Length_Type, true_type,"Type does not derive from a valid length type");
			end_requirements_list(Length_Type);
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
			end_requirements_list(DRSeconds);
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
		/// Check if a type meets the requirements of being a basic 'Time' class
		concept Is_Time
		{
			begin_requirements_list(none);
			requires_typename_state(none, Time_Type, true_type,"This class has no TimeType = true_type defined");
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

			define_get_set_exists_check(Length);
			feature TargetType Length(call_requires(ComponentType,get_exists_check(Length)))
			{
				return TargetType(cast_self_to_component().Length<ComponentType,CallerType,float>() * Conversion_Factor<TargetType>());
			}
			feature void Length(TargetType value, call_requires(ComponentType,set_exists_check(Length)))
			{
				cast_self_to_component().Length<ComponentType,CallerType,float>(value / Conversion_Factor<TargetType>());
			}

			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Centimeters) && requires(ComponentType,Concepts::Measure_in_Centimeters)))
			{
				return 1.0f
			};
			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Feet) && requires(ComponentType,Concepts::Measure_in_Centimeters)))
			{		
				return 0.0328084f;
			};
			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Inches) && requires(ComponentType,Concepts::Measure_in_Centimeters)))
			{
				return 0.393701f;				
			};
			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Kilometers) && requires(ComponentType,Concepts::Measure_in_Centimeters)))
			{
				return 1.0f / 100000.0f;
			};
			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Meters) && requires(ComponentType,Concepts::Measure_in_Centimeters)))
			{
				return 1.0f / 100.0f;
			};
			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Miles) && requires(ComponentType,Concepts::Measure_in_Centimeters)))
			{
				return 1.0f / 160934.0f;
			};

			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Centimeters) && requires(ComponentType,Concepts::Measure_in_Feet)))
			{
				return 30.48f;
			};
			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Feet) && requires(ComponentType,Concepts::Measure_in_Feet)))
			{		
				return 1.0f;
			};
			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Inches) && requires(ComponentType,Concepts::Measure_in_Feet)))
			{
				return 12.0f;				
			};
			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Kilometers) && requires(ComponentType,Concepts::Measure_in_Feet)))
			{
				return 1.0f / 3280.84f;
			};
			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Meters) && requires(ComponentType,Concepts::Measure_in_Feet)))
			{
				return 0.3048f;
			};
			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Miles) && requires(ComponentType,Concepts::Measure_in_Feet)))
			{
				return 1.0f / 5280.0f;
			};

			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Centimeters) && requires(ComponentType,Concepts::Measure_in_Inches)))
			{
				return 2.54f;
			};
			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Feet) && requires(ComponentType,Concepts::Measure_in_Inches)))
			{		
				return 1.0f/12.0f;
			};
			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Inches) && requires(ComponentType,Concepts::Measure_in_Inches)))
			{
				return 1.0f;				
			};
			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Kilometers) && requires(ComponentType,Concepts::Measure_in_Inches)))
			{
				return 2.54/100000.0f;
			};
			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Meters) && requires(ComponentType,Concepts::Measure_in_Inches)))
			{
				return 0.0254f ;
			};
			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Miles) && requires(ComponentType,Concepts::Measure_in_Inches)))
			{
				return 0.0833333f / 5280.0f;
			};

			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Centimeters) && requires(ComponentType,Concepts::Measure_in_Kilometers)))
			{
				return 100000.0f;
			};
			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Feet) && requires(ComponentType,Concepts::Measure_in_Kilometers)))
			{		
				return 3280.84f;
			};
			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Inches) && requires(ComponentType,Concepts::Measure_in_Kilometers)))
			{
				return 39370.1;				
			};
			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Kilometers) && requires(ComponentType,Concepts::Measure_in_Kilometers)))
			{
				return 1.0f;
			};
			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Meters) && requires(ComponentType,Concepts::Measure_in_Kilometers)))
			{
				return 1000.0f;
			};
			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Miles) && requires(ComponentType,Concepts::Measure_in_Kilometers)))
			{
				return 0.621371f;
			};
				
			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Centimeters) && requires(ComponentType,Concepts::Measure_in_Meters)))
			{
				return 100.0f;
			};
			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Feet) && requires(ComponentType,Concepts::Measure_in_Meters)))
			{		
				return 3.28084f;
			};
			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Inches) && requires(ComponentType,Concepts::Measure_in_Meters)))
			{
				return 39.3701f;				
			};
			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Kilometers) && requires(ComponentType,Concepts::Measure_in_Meters)))
			{
				return 0.001f;
			};
			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Meters) && requires(ComponentType,Concepts::Measure_in_Meters)))
			{
				return 1.0f;
			};
			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Miles) && requires(ComponentType,Concepts::Measure_in_Meters)))
			{
				return 0.000621371f;
			};
					
			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Centimeters) && requires(ComponentType,Concepts::Measure_in_Miles)))
			{
				return 160934.0f;
			};
			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Feet) && requires(ComponentType,Concepts::Measure_in_Miles)))
			{		
				return 5280.0f;
			};
			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Inches) && requires(ComponentType,Concepts::Measure_in_Miles)))
			{
				return 63360.0f;				
			};
			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Kilometers) && requires(ComponentType,Concepts::Measure_in_Miles)))
			{
				return 1.60934f;
			};
			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Meters) && requires(ComponentType,Concepts::Measure_in_Miles)))
			{
				return 1609.34f;
			};
			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Miles) && requires(ComponentType,Concepts::Measure_in_Miles)))
			{
				return 1.0f;
			};

			feature float Conversion_Factor(call_requirements(!(
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

		prototype struct Area_Prototype : protected Length_Prototype<ComponentType, CallerType>
		{
			feature TargetType Area(call_requires(TargetType,Concepts::Is_Area_Measure))
			{
				
				return ((Length_Prototype<ComponentType,CallerType>*)this)->Length<TargetType>()*((Length_Prototype<ComponentType,CallerType>*)this)->Conversion_Factor<TargetType>();
			}
			feature TargetType Area(call_requires(TargetType,!Concepts::Is_Area_Measure))
			{
				assert_requirements(TargetType,Concepts::Is_Area_Measure,"Your target type is not identified as an area measure.")
			}

			feature void Area(TargetType value, call_requires(TargetType,Concepts::Is_Area_Measure))
			{
				((Length_Prototype<ComponentType,CallerType>*)this)->Length<TargetType>(value / ((Length_Prototype<ComponentType,CallerType>*)this)->Conversion_Factor<TargetType>());
			}
			feature void Area(TargetType value, call_requires(TargetType,!Concepts::Is_Area_Measure))
			{
				assert_requirements(TargetType,Concepts::Is_Area_Measure,"Your target type is not identified as an area measure.")
			}
		};

		prototype struct Volume_Prototype : protected  Length_Prototype<ComponentType, CallerType>
		{
			feature TargetType Volume(call_requires(TargetType,Concepts::Is_Volume_Measure))
			{		
				return ((Length_Prototype<ComponentType,CallerType>*)this)->Length<TargetType>()*((Length_Prototype<ComponentType,CallerType>*)this)->Conversion_Factor<TargetType>()*((Length_Prototype<ComponentType,CallerType>*)this)->Conversion_Factor<TargetType>();
			}
			feature TargetType Volume(call_requires(TargetType,!Concepts::Is_Volume_Measure))
			{
				assert_requirements(TargetType,Concepts::Is_Volume_Measure,"Your target type is not identified as a volume measure.")
			}

			feature void Volume(TargetType value,call_requires(TargetType,Concepts::Is_Volume_Measure))
			{
				((Length_Prototype<ComponentType,CallerType>*)this)->Length<TargetType>(value / ((Length_Prototype<ComponentType,CallerType>*)this)->Conversion_Factor<TargetType>() / ((Length_Prototype<ComponentType,CallerType>*)this)->Conversion_Factor<TargetType>());
			}
			feature void Volume(TargetType value, call_requires(TargetType,!Concepts::Is_Volume_Measure))
			{
				assert_requirements(TargetType,Concepts::Is_Volume_Measure,"Your target type is not identified as a volume measure.")
			}
		};


		prototype struct Time_Prototype_New
		{
			tag_polaris_prototype;

			define_get_set_exists_check(Time);
			feature TargetType Time(call_requires(ComponentType,get_exists_check(Time)))
			{
				return TargetType(cast_self_to_component().Time<ComponentType,CallerType,float>() * Conversion_Factor<TargetType>());
			}
			feature void Time(TargetType value, call_requires(ComponentType,set_exists_check(Time)))
			{
				cast_self_to_component().Time<ComponentType,CallerType,float>(value / Conversion_Factor<TargetType>());
			}

			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Time_In_Days) && requires(ComponentType,Concepts::Time_In_Days)))
			{
				return 1.0f;
			};
			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Time_In_DRSeconds) && requires(ComponentType,Concepts::Time_In_Days)))
			{
				return 864000.0f;
			};
			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Time_In_Hours) && requires(ComponentType,Concepts::Time_In_Days)))
			{
				return 24.0f;
			};
			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Time_In_Minutes) && requires(ComponentType,Concepts::Time_In_Days)))
			{
				return 1440.0f;
			};
			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Time_In_Seconds) && requires(ComponentType,Concepts::Time_In_Days)))
			{
				return 86400.0f;
			};

			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Time_In_Days) && requires(ComponentType,Concepts::Time_In_DRSeconds)))
			{
				return 1.0f / 864000.0f;
			};
			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Time_In_DRSeconds) && requires(ComponentType,Concepts::Time_In_DRSeconds)))
			{
				return 1.0f;
			};
			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Time_In_Hours) && requires(ComponentType,Concepts::Time_In_DRSeconds)))
			{
				return 1.0f/36000.0f;
			};
			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Time_In_Minutes) && requires(ComponentType,Concepts::Time_In_DRSeconds)))
			{
				return 0.001667f;
			};
			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Time_In_Seconds) && requires(ComponentType,Concepts::Time_In_DRSeconds)))
			{
				return 0.1f;
			};

			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Time_In_Days) && requires(ComponentType,Concepts::Time_In_Hours)))
			{
				return 0.041667f;
			};
			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Time_In_DRSeconds) && requires(ComponentType,Concepts::Time_In_Hours)))
			{
				return 36000.0f;
			};
			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Time_In_Hours) && requires(ComponentType,Concepts::Time_In_Hours)))
			{
				return 1.0f;
			};
			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Time_In_Minutes) && requires(ComponentType,Concepts::Time_In_Hours)))
			{
				return 60.0f;
			};
			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Time_In_Seconds) && requires(ComponentType,Concepts::Time_In_Hours)))
			{
				return 3600.0f;
			};

			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Time_In_Days) && requires(ComponentType,Concepts::Time_In_Minutes)))
			{
				return 0.000694f;
			};
			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Time_In_DRSeconds) && requires(ComponentType,Concepts::Time_In_Minutes)))
			{
				return 600.0f;
			};
			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Time_In_Hours) && requires(ComponentType,Concepts::Time_In_Minutes)))
			{
				return 0.01667f;
			};
			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Time_In_Minutes) && requires(ComponentType,Concepts::Time_In_Minutes)))
			{
				return 1.0f;
			};
			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Time_In_Seconds) && requires(ComponentType,Concepts::Time_In_Minutes)))
			{
				return 60.0f;
			};

			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Time_In_Days) && requires(ComponentType,Concepts::Time_In_Seconds)))
			{
				return 1.0f / 86400.0f;
			};
			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Time_In_DRSeconds) && requires(ComponentType,Concepts::Time_In_Seconds)))
			{
				return 10.0f;
			};
			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Time_In_Hours) && requires(ComponentType,Concepts::Time_In_Seconds)))
			{
				return 0.000278f;
			};
			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Time_In_Minutes) && requires(ComponentType,Concepts::Time_In_Seconds)))
			{
				return 0.01667f;
			};
			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Time_In_Seconds) && requires(ComponentType,Concepts::Time_In_Seconds)))
			{
				return 1.0f;
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

		prototype struct Rate_Prototype : protected Time_Prototype<ComponentType,CallerType>
		{
			tag_polaris_prototype;

			feature TargetType Length()
			{
				return TargetType(cast_self_to_component().length<ComponentType,CallerType,float>() * Conversion_Factor<TargetType>());
			}
			feature void Length(TargetType value)
			{
				cast_self_to_component().length<ComponentType,CallerType,float>(value / Conversion_Factor<TargetType>());
			}

			feature float Conversion_Factor(call_requirements(requires(TargetType,Concepts::Measure_in_Centimeters) && requires(ComponentType,Concepts::Measure_in_Centimeters)))
			{
				return 1.0f
			};
		};
	}

	namespace Data_Structures
	{
		typedef float length_data_type;
		typedef float time_data_type;
		typedef float rate_data_type;
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