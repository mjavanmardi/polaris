#pragma once
#include "RNG_Prototype.h"
#include "RngStream.h"

namespace Polaris
{
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
		implementation struct MT_Probability_Double : public Polaris_Component<MasterType,INHERIT(MT_Probability_Double),Data_Object>
		{
			MT_Probability_Double()
			{
				_seed = time(NULL);
				_generator.seed(_seed);
			}
			void Initialize()
			{
				_seed = time(NULL);
				_generator.seed(_seed);
			}

			template<typename ValueType> void Initialize(ValueType seed_value, ValueType min = (ValueType)0, ValueType max = (ValueType)1, ValueType location = (ValueType)1, ValueType scale = (ValueType)1, ValueType shape = (ValueType)1, requires(check(ValueType,is_arithmetic)))
			{
				_seed = seed_value;
				_generator.seed(_seed);
			}
			//tag_feature_as_available(Initialize);

			template<typename ValueType> ValueType Next_Rand()
			{
				return (ValueType) _distribution(_generator);
			}
			//tag_feature_as_available(Next_Rand);

			m_data(unsigned long, seed, NONE, NONE);
			m_data(std::mt19937, generator, NONE, NONE);
			m_data(std::uniform_real_distribution<double>, distribution, NONE, NONE);
		};

		implementation struct MT_Uniform_Double : public MT_Probability_Double<MasterType,INHERIT(MT_Uniform_Double)>
		{
			typedef MT_Probability_Double<MasterType,INHERIT(MT_Uniform_Double)> BaseType;

			MT_Uniform_Double ()
			{
				BaseType();
				_minimum = 0.0;
				_maximum = 1.0;
			}

			void Initialize()
			{		
				BaseType::_generator.seed(BaseType::_seed);
				BaseType::_distribution = std::uniform_real_distribution<double>(_minimum,_maximum);
			}

			template<typename ValueType> void Initialize(ValueType seed_value, ValueType min = (ValueType)0, ValueType max = (ValueType)1, ValueType location = (ValueType)0, ValueType scale = (ValueType)1, ValueType shape = (ValueType)1, requires(check(ValueType,is_arithmetic)))
			{
				BaseType::_generator.seed((unsigned long)seed_value);
				this->_minimum = (double)min;
				this->_maximum = (double)max;

				BaseType::_distribution = std::uniform_real_distribution<double>(_minimum,_maximum);
			}
			//tag_feature_as_available(Initialize);

			m_data(double, maximum, NONE, NONE);
			m_data(double, minimum, NONE, NONE);
		};

		implementation struct MT_Normal_Double : public MT_Uniform_Double<MasterType,INHERIT(MT_Normal_Double)>
		{
			typedef MT_Uniform_Double<MasterType,INHERIT(MT_Normal_Double)> BaseType;
			typedef typename BaseType::BaseType GrandBaseType;

			MT_Normal_Double()
			{
				BaseType();
				_location = 0.0;
				_scale = 1.0;
			}

			void Initialize()
			{		
				assert(_scale > 0);
				GrandBaseType::_generator.seed(GrandBaseType::_seed);
				_distribution = std::normal_distribution<double>(_location,_scale);
			}

			template<typename ValueType> void Initialize(ValueType seed_value, ValueType min = (ValueType)0, ValueType max = (ValueType)1, ValueType location = (ValueType)0, ValueType scale = (ValueType)1, ValueType shape = (ValueType)1, requires(check(ValueType,is_arithmetic)))
			{
				GrandBaseType::_generator.seed((unsigned long)seed_value);
				this->_location = location;
				this->_scale = scale;

				_distribution = std::normal_distribution<double>(_location,_scale);
			}
			//tag_feature_as_available(Initialize);

			template<typename ValueType> ValueType Next_Rand()
			{
				return (ValueType) _distribution(GrandBaseType::_generator);
			}
			//tag_feature_as_available(Next_Rand);

			m_data(std::normal_distribution<double>, distribution, NONE, NONE);
			m_data(double, location, NONE, NONE);
			m_data(double, scale, NONE, NONE);
		};

		implementation struct RngStream_Implementation : public Polaris_Component<MasterType,INHERIT(RngStream_Implementation),Data_Object>
		{
			RngStream_Implementation()
			{
				_seed = time(NULL);
				_distribution.SetSeed(_seed);
				_maximum = 1.0;
				_minimum = 0.0;
				
			}
			void Initialize()
			{
				_distribution.SetSeed(_seed);
			}
			template<typename ValueType> void Initialize(	ValueType seed_value,
												ValueType min = (ValueType)0,
												ValueType max = (ValueType)1,
												ValueType location = (ValueType)0,
												ValueType scale = (ValueType)1,
												ValueType shape = (ValueType)1,
												requires(check(ValueType,is_arithmetic)))
			{
				_seed = seed_value;
				_distribution.SetSeed(_seed);
			}
			//tag_feature_as_available(Initialize);

			template<typename ValueType> ValueType Next_Rand()
			{
				return (ValueType) _distribution.RandU01();
			}
			//tag_feature_as_available(Next_Rand);

			m_data(unsigned long, seed, check(TargetType,is_arithmetic), check(TargetType,is_arithmetic));
			m_data(double, maximum, check(TargetType,is_arithmetic), NONE);
			m_data(double, minimum, check(TargetType,is_arithmetic), NONE);
			m_data(RNG_Components::RngStream, distribution, NONE, NONE);
		};
	}

}

namespace GLOBALS
{
	implementation struct _Global_RNG : public Polaris_Component<MasterType, INHERIT(_Global_RNG)>
	{
		typedef polaris::RNG_Components::Implementations::MT_Probability_Double<NULLTYPE> RNG_type;
		_Global_RNG()
		{
			for (int i=0; i < num_sim_threads(); i++)
			{
				typedef RNG_Components::Prototypes::RNG_Prototype<RNG_type> rng_itf;
				rng_itf* rng = (rng_itf*)&this->thread_rng[i];
				rng->Initialize<int>(std::abs(std::sin((float)i+1)*(float)INT_MAX));
			}
		}

		template <typename TargetType>
		void Set_Seed(TargetType random_seed)
		{
			for (int i=0; i < _num_threads; i++)
			{
				typedef RNG_Components::Prototypes::RNG_Prototype<RNG_type> rng_itf;
				rng_itf* rng = (rng_itf*)&this->thread_rng[i];
				rng->Initialize<TargetType>((TargetType)(std::abs(std::sin((float)(i+1)*random_seed)*(float)INT_MAX)) + random_seed);
			}
		}
		template <typename TargetType>
		void Set_Seed()
		{
			for (int i=0; i < _num_threads; i++)
			{
				TargetType random_seed = time(NULL);
				typedef RNG_Components::Prototypes::RNG_Prototype<RNG_type> rng_itf;
				rng_itf* rng = (rng_itf*)&this->thread_rng[i];
				rng->Initialize<TargetType>((TargetType)(std::abs(std::sin((float)(i+1)*random_seed)*(float)INT_MAX)) + random_seed);
			}
		}


		template <typename ReturnValueType>
		ReturnValueType Next_Rand()
		{
			typedef RNG_Components::Prototypes::RNG_Prototype<RNG_type> rng_itf;
			rng_itf* rng = (rng_itf*)&this->thread_rng[_thread_id];

			return rng->Next_Rand<double>();
		}

		template <typename TargetType>
		TargetType triangular_random_variate(TargetType u, TargetType a, TargetType b, TargetType mean)
		{
			if (mean == 0.0) return 0.0;

			double x = 0.0;
			double c = 3.0*mean - a - b;
			double fc = (c - a) / (b - a);
	
			if (u < fc)
			{
				x = a + sqrt(u*(b - a)*(c - a));
			}
			else
			{
				x = b - sqrt((1 - u)*(b - a)*(b - c));
			}

			return x;
		}

	private:
		 RNG_type thread_rng[_num_threads];
	};
	static _Global_RNG<NULLTYPE> Uniform_RNG;

	implementation struct _Global_Normal_RNG : public Polaris_Component<APPEND_CHILD(_Global_Normal_RNG),MasterType,NULLTYPE>
	{
		typedef RNG_Components::Implementations::MT_Normal_Double<NULLTYPE> RNG_type;

		_Global_Normal_RNG()
		{
			for (int i=0; i < _num_threads; i++)
			{
				typedef RNG_Components::Prototypes::RNG_Prototype<RNG_type> rng_itf;
				rng_itf* rng = (rng_itf*)&this->thread_rng[i];
				int seed = std::sin((float)i+1)*(float)INT_MAX;
				rng->Initialize<int>(seed);
			}
		}

		template <typename TargetType>
		void Set_Seed(TargetType random_seed)
		{
			for (int i=0; i < _num_threads; i++)
			{
				typedef RNG_Components::Prototypes::RNG_Prototype<RNG_type> rng_itf;
				rng_itf* rng = (rng_itf*)&this->thread_rng[i];
				rng->Initialize<TargetType>((TargetType)(std::sin((float)(i+1)*random_seed)*(float)INT_MAX) + random_seed);
			}
		}
		template <typename TargetType>
		void Set_Seed()
		{
			for (int i=0; i < _num_threads; i++)
			{
				TargetType random_seed = time(NULL);
				typedef RNG_Components::Prototypes::RNG_Prototype<RNG_type> rng_itf;
				rng_itf* rng = (rng_itf*)&this->thread_rng[i];
				rng->Initialize<TargetType>((TargetType)(std::abs(std::sin((float)(i+1)*random_seed)*(float)INT_MAX)) + random_seed);
			}
		}

		template <typename TargetType>
		TargetType Next_Rand(TargetType mu=0, TargetType sigma=1)
		{
			typedef RNG_Components::Prototypes::RNG_Prototype<RNG_type> rng_itf;
			rng_itf* rng = (rng_itf*)&this->thread_rng[_thread_id];
			TargetType r = rng->Next_Rand<TargetType>();
			return r*sigma + mu;
		}



		template <typename TargetType>
		void Correlated_Rands(vector<TargetType>& correlated_random_values, matrix<TargetType>& Sigma)
		{
			correlated_random_values.clear();

			// factor the covariance matrix
			matrix<TargetType> L;
			Sigma.cholesky(L);

			// create vector of uncorrelated normals
			matrix<TargetType> norm = matrix<TargetType>(typename matrix<TargetType>::index_type(Sigma.num_rows(),1),0);
			for (uint i = 0; i < Sigma.num_rows(); ++i) norm(i,0) = this->Next_Rand<TargetType>();

			// correlate the normals and populate the return vector
			matrix<TargetType> corr_norm = L*norm;
			for (uint i = 0; i < Sigma.num_rows(); ++i) correlated_random_values.push_back(corr_norm(i,0));
		}
	private:
		 RNG_type thread_rng[_num_threads];
	};

	static _Global_Normal_RNG<NULLTYPE> Normal_RNG;
}

using namespace GLOBALS;

}