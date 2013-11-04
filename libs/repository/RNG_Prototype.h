#pragma once
#include "Dependencies.h"
#include <time.h>

namespace polaris{
namespace RNG_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
		concept struct Is_RNG
		{
			check_typedef_name(Has_Rand_Defined,Next_Rand);
			check_typedef_name(Has_Rand_Defined_Prototype, Component_Type::Next_Rand);
			define_default_check(Has_Rand_Defined || Has_Rand_Defined_Prototype);
		};
	}
	
	namespace Prototypes
	{
		prototype struct RNG
		{
			tag_as_prototype;

			void Initialize()
			{
				this_component()->Initialize();
			}
			template<typename ValueType> void Initialize(	TargetType seed_value,
												TargetType min = (TargetType)0,
												TargetType max = (TargetType)1,
												TargetType location = (TargetType)0,
												TargetType scale = (TargetType)1,
												TargetType shape = (TargetType)1,
												requires(check(TargetType,is_arithmetic)))
			{
				this_component()->Initialize<ValueType>(seed_value, min, max, location, scale, shape);
			}
			template<typename ValueType> void Initialize(	ValueType seed_value,
												ValueType min = (TargetType)0,
												ValueType max = (TargetType)1,
												ValueType location = (TargetType)0,
												ValueType scale = (TargetType)1,
												ValueType shape = (TargetType)1,
												requires(check(ValueType,!is_arithmetic)))
			{
				assert_default_check(ValueType,is_arithmetic,"Error, the specified TargetType does not satisfy the 'is_arithmetic' concept");
			}

			template<typename ValueType> ValueType Next_Rand()
			{
				return this_component()->Next_Rand<ValueType>();
			}

			accessor(seed, check(ReturnValueType,is_arithmetic),check(SetValueType,is_arithmetic));
			
			accessor(minimum,check(ReturnValueType,is_arithmetic),check(SetValueType,is_arithmetic));

			accessor(maximum,check(ReturnValueType,is_arithmetic),check(SetValueType,is_arithmetic));

			accessor(location,check(ReturnValueType,is_arithmetic),check(SetValueType,is_arithmetic));

			accessor(shape,check(ReturnValueType,is_arithmetic),check(SetValueType,is_arithmetic));

			accessor(scale,check(ReturnValueType,is_arithmetic),check(SetValueType,is_arithmetic));
		};
	}

}

using namespace RNG_Components;

}