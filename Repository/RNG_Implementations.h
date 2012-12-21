#pragma once
#include "RNG_Prototype.h"
#include "User_Space\RngStream.h"

namespace RNG_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{
		implementation struct MT_Probability_Double
		{
			MT_Probability_Double<MasterType>()
			{
				_seed = time(NULL);
				_generator.seed(_seed);
			}
			feature_implementation void Initialize()
			{
				_generator.seed(_seed);
			}
			feature_implementation void Initialize(	TargetType seed_value,
												TargetType min = (TargetType)0,
												TargetType max = (TargetType)1,
												TargetType location = (TargetType)1,
												TargetType scale = (TargetType)1,
												TargetType shape = (TargetType)1,
												requires(check(TargetType,is_arithmetic)))
			{
				_generator.seed(_seed);
			}
			feature_implementation TargetType Next_Rand()
			{
				return (TargetType) _distribution(_generator);
			}

			member_data(unsigned long, seed, none, none);
			member_data(mt19937, generator, none, none);
			member_data(uniform_real<double>, distribution, none, none);
		};

		implementation struct MT_Uniform_Double : public MT_Probability_Double<MasterType>
		{
			MT_Uniform_Double<MasterType>() : MT_Probability_Double<MasterType>()
			{
				_minimum = 0.0;
				_maximum = 1.0;
			}

			feature_implementation void Initialize()
			{		
				_generator.seed(_seed);
				_distribution = uniform_real<double>(_minimum,_maximum);
			}

			feature_implementation void Initialize(	TargetType seed_value,
												TargetType min = (TargetType)0,
												TargetType max = (TargetType)1,
												TargetType location = (TargetType)1,
												TargetType scale = (TargetType)1,
												TargetType shape = (TargetType)1,
												requires(check(TargetType,is_arithmetic)))
			{
				_generator.seed(_seed);
				this->minimum<ComponentType, CallerType, TargetType>(min);
				this->maximum<ComponentType, CallerType, TargetType>(max);

				_distribution = uniform_real<double>(_minimum,_maximum);
			}
			member_data(double, maximum, none, none);
			member_data(double, minimum, none, none);
		};

		implementation struct MT_Normal_Double : public MT_Uniform_Double<MasterType>
		{
			MT_Normal_Double<MasterType>() : MT_Uniform_Double<MasterType>()
			{
				_location = 0.0;
				_scale = 1.0;
			}

			feature_implementation void Initialize()
			{		
				assert(_scale > 0);
				_generator.seed(_seed);
				_distribution = std::tr1::normal_distribution<double>(_location,_scale);
			}

			feature_implementation void Initialize(	TargetType seed_value,
												TargetType min = (TargetType)0,
												TargetType max = (TargetType)1,
												TargetType location = (TargetType)1,
												TargetType scale = (TargetType)1,
												TargetType shape = (TargetType)1,
												requires(check(TargetType,is_arithmetic)))
			{
				state_check(Is_Positive)(this,_scale);
				_generator.seed(_seed);
				this->location<ComponentType, CallerType, TargetType>(location);
				this->scale<ComponentType, CallerType, TargetType>(scale);

				_distribution = uniform_real<double>(_minimum,_maximum);
			}

			feature_implementation TargetType Next_Rand()
			{
				return (TargetType) _distribution(_generator);
			}

			member_data(normal_distribution<double>, distribution, none, none);
			member_data(double, location, none, none);
			member_data(double, scale, none, none);
		};

		implementation struct RngStream_Implementation
		{
			RngStream_Implementation<MasterType>()
			{
				_seed = time(NULL);
				_distribution.SetSeed(_seed);
				_maximum = 1.0;
				_minimum = 0.0;
				
			}
			feature_implementation void Initialize()
			{
				_distribution.SetSeed(_seed);
			}
			feature_implementation void Initialize(	TargetType seed_value,
												TargetType min = (TargetType)0,
												TargetType max = (TargetType)1,
												TargetType location = (TargetType)1,
												TargetType scale = (TargetType)1,
												TargetType shape = (TargetType)1,
												requires(check(TargetType,is_arithmetic)))
			{
				_distribution.SetSeed(_seed);
			}
			feature_implementation TargetType Next_Rand()
			{
				return (TargetType) _distribution.RandU01();
			}

			member_data(unsigned long, seed, check(ReturnValueType,is_arithmetic), check(SetValueType,is_arithmetic));
			member_data(double, maximum, check(ReturnValueType,is_arithmetic), not_available);
			member_data(double, minimum, check(ReturnValueType,is_arithmetic), not_available);
			member_data(User_Space::RngStream, distribution, none, not_available);
		};
	}

}