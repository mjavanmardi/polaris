#pragma once
#include "RNG_Prototype.h"
#include "RngStream.h"

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
		implementation struct MT_Probability_Double : public Polaris_Component<APPEND_CHILD(MT_Probability_Double),MasterType,Data_Object,ParentType>
		{
			MT_Probability_Double()
			{
				_seed = time(NULL);
				_generator.seed(_seed);
			}
			feature_implementation void Initialize()
			{
				_seed = time(NULL);
				_generator.seed(_seed);
			}
			feature_implementation void Initialize(	TargetType seed_value, TargetType min = (TargetType)0, TargetType max = (TargetType)1, TargetType location = (TargetType)1, TargetType scale = (TargetType)1, TargetType shape = (TargetType)1, requires(check(TargetType,is_arithmetic)))
			{
				_seed = seed_value;
				_generator.seed(_seed);
			}
			tag_feature_as_available(Initialize);

			feature_implementation TargetType Next_Rand()
			{
				return (TargetType) _distribution(_generator);
			}
			tag_feature_as_available(Next_Rand);

			member_data(unsigned long, seed, none, none);
			member_data(std::mt19937, generator, none, none);
			member_data(std::tr1::uniform_real<double>, distribution, none, none);
		};

		implementation struct MT_Uniform_Double : public MT_Probability_Double<MasterType,ParentType,APPEND_CHILD(MT_Uniform_Double)>
		{
			typedef MT_Probability_Double<MasterType,ParentType,InheritanceList> BaseType;

			MT_Uniform_Double ()
			{
				BaseType();
				_minimum = 0.0;
				_maximum = 1.0;
			}

			feature_implementation void Initialize()
			{		
				BaseType::_generator.seed(BaseType::_seed);
				BaseType::_distribution = std::tr1::uniform_real<double>(_minimum,_maximum);
			}

			feature_implementation void Initialize(	TargetType seed_value, TargetType min = (TargetType)0, TargetType max = (TargetType)1, TargetType location = (TargetType)1, TargetType scale = (TargetType)1, TargetType shape = (TargetType)1, requires(check(TargetType,is_arithmetic)))
			{
				_generator.seed(MT_Probability_Double::_seed);
				this->template minimum<ComponentType, CallerType, TargetType>(min);
				this->template maximum<ComponentType, CallerType, TargetType>(max);

				MT_Probability_Double::_distribution = std::tr1::uniform_real<double>(_minimum,_maximum);
			}
			tag_feature_as_available(Initialize);

			member_data(double, maximum, none, none);
			member_data(double, minimum, none, none);
		};

		implementation struct MT_Normal_Double : public MT_Uniform_Double<MasterType,ParentType, APPEND_CHILD(MT_Normal_Double)>
		{
			MT_Normal_Double()
			{
				MT_Uniform_Double();
				_location = 0.0;
				_scale = 1.0;
			}

			feature_implementation void Initialize()
			{		
				assert(_scale > 0);
				MT_Probability_Double::_generator.seed(MT_Probability_Double::_seed);
				_distribution = std::tr1::normal_distribution<double>(_location,_scale);
			}

			feature_implementation void Initialize(	TargetType seed_value, TargetType min = (TargetType)0, TargetType max = (TargetType)1, TargetType location = (TargetType)1, TargetType scale = (TargetType)1, TargetType shape = (TargetType)1, requires(check(TargetType,is_arithmetic)))
			{
				//state_check(Is_Positive)(this,_scale);
				MT_Probability_Double::_generator.seed(MT_Probability_Double::_seed);
				this->template location<ComponentType, CallerType, TargetType>(location);
				this->template scale<ComponentType, CallerType, TargetType>(scale);

				_distribution = std::tr1::normal_distribution<double>(_location,_scale);
				//_distribution = std::tr1::uniform_real<double>(MT_Uniform_Double::_minimum,MT_Uniform_Double::_maximum);
			}
			tag_feature_as_available(Initialize);

			feature_implementation TargetType Next_Rand()
			{
				return (TargetType) _distribution(MT_Probability_Double::_generator);
			}
			tag_feature_as_available(Next_Rand);

			member_data(normal_distribution<double>, distribution, none, none);
			member_data(double, location, none, none);
			member_data(double, scale, none, none);
		};

		implementation struct RngStream_Implementation : public Polaris_Component<APPEND_CHILD(RngStream_Implementation),MasterType,Data_Object,ParentType>
		{
			RngStream_Implementation()
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
				_seed = seed_value;
				_distribution.SetSeed(_seed);
			}
			tag_feature_as_available(Initialize);

			feature_implementation TargetType Next_Rand()
			{
				return (TargetType) _distribution.RandU01();
			}
			tag_feature_as_available(Next_Rand);

			member_data(unsigned long, seed, check(ReturnValueType,is_arithmetic), check(SetValueType,is_arithmetic));
			member_data(double, maximum, check(ReturnValueType,is_arithmetic), not_available);
			member_data(double, minimum, check(ReturnValueType,is_arithmetic), not_available);
			member_data(User_Space::RngStream, distribution, none, not_available);
		};
	}

}