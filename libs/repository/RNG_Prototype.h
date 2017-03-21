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
			check_accessor_name(Has_Rand_Defined, Next_Rand);
			check_component_accessor_name(Has_Rand_Defined_Prototype, Next_Rand);
			define_default_check(Has_Rand_Defined/* || Has_Rand_Defined_Prototype*/);
		};
	}
	
	namespace Prototypes
	{
		prototype struct RNG ADD_DEBUG_INFO
		{
			tag_as_prototype;

			template<typename TargetType> void Initialize()
			{
				this_component()->Initialize<TargetType>();
			}
			template<typename TargetType> void Initialize(	TargetType seed_value,
												TargetType min = (TargetType)0,
												TargetType max = (TargetType)1,
												TargetType location = (TargetType)0,
												TargetType scale = (TargetType)1,
												TargetType shape = (TargetType)1,
												requires(TargetType,check(strip_modifiers(TargetType),is_arithmetic)))
			{
				this_component()->Initialize<TargetType>(seed_value, min, max, location, scale, shape);
			}
			template<typename TargetType> void Initialize(	TargetType seed_value,
												TargetType min = (TargetType)0,
												TargetType max = (TargetType)1,
												TargetType location = (TargetType)0,
												TargetType scale = (TargetType)1,
												TargetType shape = (TargetType)1,
												requires(TargetType,check(strip_modifiers(TargetType),!is_arithmetic)))
			{
				assert_check(strip_modifiers(TargetType),is_arithmetic,"Error, the specified TargetType does not satisfy the 'is_arithmetic' concept");
			}

			template<typename TargetType> TargetType Next_Rand()
			{
				return this_component()->Next_Rand<TargetType>();
			}

			accessor(seed, NONE, NONE);
			
			accessor(minimum, NONE, NONE);

			accessor(maximum, NONE, NONE);

			accessor(location, NONE, NONE);

			accessor(shape, NONE, NONE);

			accessor(scale, NONE, NONE);
		};
	}

}
using namespace RNG_Components::Prototypes;
