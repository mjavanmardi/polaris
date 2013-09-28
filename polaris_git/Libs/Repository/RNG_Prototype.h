#pragma once
#include "Repository_Includes.h"
#include <time.h>

namespace RNG_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
		concept struct Is_RNG
		{
			check_feature(Has_Rand_Defined,Next_Rand);
			check_feature(Has_Rand_Defined_Prototype, Component_Type::Next_Rand);
			define_default_check(Has_Rand_Defined || Has_Rand_Defined_Prototype);
		};
	}
	
	namespace Prototypes
	{
		prototype struct RNG_Prototype ADD_DEBUG_INFO
		{
			tag_as_prototype;

			feature_prototype void Initialize()
			{
				this_component()->Initialize<ComponentType,CallerType,TargetType>();
			}
			feature_prototype void Initialize(	TargetType seed_value,
												TargetType min = (TargetType)0,
												TargetType max = (TargetType)1,
												TargetType location = (TargetType)0,
												TargetType scale = (TargetType)1,
												TargetType shape = (TargetType)1,
												requires(check(TargetType,is_arithmetic)))
			{
				this_component()->Initialize<ComponentType,CallerType,TargetType>(seed_value, min, max, location, scale, shape);
			}
			feature_prototype void Initialize(	TargetType seed_value,
												TargetType min = (TargetType)0,
												TargetType max = (TargetType)1,
												TargetType location = (TargetType)0,
												TargetType scale = (TargetType)1,
												TargetType shape = (TargetType)1,
												requires(check(TargetType,!is_arithmetic)))
			{
				assert_check(TargetType,is_arithmetic,"Error, the specified TargetType does not satisfy the 'is_arithmetic' concept");
			}

			feature_prototype TargetType Next_Rand()
			{
				return this_component()->Next_Rand<ComponentType,CallerType,TargetType>();
			}

			feature_accessor(seed, check(ReturnValueType,is_arithmetic),check(SetValueType,is_arithmetic));
			
			feature_accessor(minimum,check(ReturnValueType,is_arithmetic),check(SetValueType,is_arithmetic));

			feature_accessor(maximum,check(ReturnValueType,is_arithmetic),check(SetValueType,is_arithmetic));

			feature_accessor(location,check(ReturnValueType,is_arithmetic),check(SetValueType,is_arithmetic));

			feature_accessor(shape,check(ReturnValueType,is_arithmetic),check(SetValueType,is_arithmetic));

			feature_accessor(scale,check(ReturnValueType,is_arithmetic),check(SetValueType,is_arithmetic));
		};
	}

}

using namespace RNG_Components::Prototypes;