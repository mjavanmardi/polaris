#pragma once
#include "RNG_Prototype.h"
#include "RngStream.h"

namespace RNG_Components
{
	namespace Types
	{
		typedef float value_type;
	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{
		implementation struct MT_Probability : public Polaris_Component<MasterType,INHERIT(MT_Probability),Data_Object>
		{
			MT_Probability()
			{
				_seed = time(NULL);
				_generator.seed(_seed);
			}
			template<typename TargetType> void Initialize()
			{
				_seed = time(NULL);
				_generator.seed(_seed);
			}
			template<typename TargetType> void Initialize(	TargetType seed_value, TargetType min = (TargetType)0, TargetType max = (TargetType)1, TargetType location = (TargetType)1, TargetType scale = (TargetType)1, TargetType shape = (TargetType)1, requires(TargetType,check(strip_modifiers(TargetType),is_arithmetic)))
			{
				_seed = seed_value;
				_generator.seed(_seed);
			}

			template<typename TargetType> TargetType Next_Rand()
			{
				return (TargetType) _distribution(_generator);
			}

			m_data(unsigned long, seed, NONE, NONE);
			m_data(mt19937, generator, NONE, NONE);
			m_data(uniform_real_distribution<Types::value_type>, distribution, NONE, NONE);
		};

		implementation struct MT_Uniform : public MT_Probability<MasterType,INHERIT(MT_Uniform)>
		{
			typedef MT_Probability<MasterType,INHERIT(MT_Uniform)> BaseType;

			MT_Uniform()
			{
				BaseType();
				_minimum = 0.0;
				_maximum = 1.0;
			}

			template<typename TargetType> void Initialize()
			{		
				BaseType::_generator.seed(BaseType::_seed);
				BaseType::_distribution = uniform_real_distribution<Types::value_type>(_minimum,_maximum);
			}

			template<typename TargetType> void Initialize(	TargetType seed_value, TargetType min = (TargetType)0, TargetType max = (TargetType)1, TargetType location = (TargetType)0, TargetType scale = (TargetType)1, TargetType shape = (TargetType)1, requires(TargetType,check(strip_modifiers(TargetType),is_arithmetic)))
			{
				BaseType::_generator.seed((unsigned long)seed_value/*BaseType::_seed*/);
				this->template minimum< TargetType>(min);
				this->template maximum< TargetType>(max);

				BaseType::_distribution = uniform_real_distribution<double>(_minimum,_maximum);
			}

			m_data(Types::value_type, maximum, NONE, NONE);
			m_data(Types::value_type, minimum, NONE, NONE);
		};

		implementation struct MT_Normal : public MT_Uniform<MasterType, INHERIT(MT_Normal)>
		{
			typedef MT_Uniform<MasterType,INHERIT(MT_Normal)> BaseType;
			typedef typename BaseType::BaseType GrandBaseType;

			MT_Normal()
			{
				BaseType();
				_location = 0.0;
				_scale = 1.0;
			}

			template<typename TargetType> void Initialize()
			{		
				assert(_scale > 0);
				GrandBaseType::_generator.seed(GrandBaseType::_seed);
				_distribution = normal_distribution<double>(_location,_scale);
			}

			template<typename TargetType> void Initialize(	TargetType seed_value, TargetType min = (TargetType)0, TargetType max = (TargetType)1, TargetType location = (TargetType)0, TargetType scale = (TargetType)1, TargetType shape = (TargetType)1, requires(TargetType,check(strip_modifiers(TargetType),is_arithmetic)))
			{
				//state_check(Is_Positive)(this,_scale);
				GrandBaseType::_generator.seed((unsigned long)seed_value/* GrandBaseType::_seed*/);
				this->template location< TargetType>(location);
				this->template scale< TargetType>(scale);

				_distribution = normal_distribution<Types::value_type>(_location,_scale);
				//_distribution = tr1::uniform_real_distribution<double>(MT_Uniform_Double::_minimum,MT_Uniform_Double::_maximum);
			}

			template<typename TargetType> TargetType Next_Rand()
			{
				return (TargetType) _distribution(GrandBaseType::_generator);
			}

			template<typename TargetType> TargetType Cumulative_Distribution(TargetType x)
			{
				return Phi(x, _location, _scale);
			}

			template<typename TargetType> static TargetType Cumulative_Distribution(TargetType x, TargetType Mu, TargetType S)
			{
				return Phi(x, Mu, S);
			}

			m_data(normal_distribution<Types::value_type>, distribution, NONE, NONE);
			m_data(Types::value_type, location, NONE, NONE);
			m_data(Types::value_type, scale, NONE, NONE);

		private:
			template<typename TargetType> static TargetType erf(TargetType z)
			{
				TargetType t = 1.0 / (1.0 + 0.5 * abs(z));

				// use Horner's method
				TargetType ans = 1 - t * exp(-z * z - 1.26551223 +
													t * (1.00002368 +
													t * (0.37409196 +
													t * (0.09678418 +
													t * (-0.18628806 +
													t * (0.27886807 +
													t * (-1.13520398 +
													t * (1.48851587 +
													t * (-0.82215223 +
													t * (0.17087277))))))))));
				if (z >= 0) return ans;
				else return -ans;
			}

			// cumulative normal distribution
			template<typename TargetType> static TargetType Phi(TargetType z)
			{
				return 0.5 * (1.0 + erf(z / (sqrt(2.0))));
			}

			// cumulative normal distribution with mean mu and std deviation sigma
			template<typename TargetType> static TargetType Phi(TargetType z, TargetType mu, TargetType sigma)
			{
				return Phi((z - mu) / sigma);
			}
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
			template<typename TargetType> void Initialize()
			{
				_distribution.SetSeed(_seed);
			}
			template<typename TargetType> void Initialize(	TargetType seed_value,
												TargetType min = (TargetType)0,
												TargetType max = (TargetType)1,
												TargetType location = (TargetType)0,
												TargetType scale = (TargetType)1,
												TargetType shape = (TargetType)1,
												requires(TargetType,check(strip_modifiers(TargetType),is_arithmetic)))
			{
				_seed = seed_value;
				_distribution.SetSeed(_seed);
			}

			template<typename TargetType> TargetType Next_Rand()
			{
				return (TargetType) _distribution.RandU01();
			}

			m_data(unsigned long, seed, NONE, NONE);
			m_data(double, maximum, NONE, NONE);
			m_data(double, minimum, NONE, NONE);
			m_data(RNG_Components::RngStream, distribution, NONE, NONE);
		};
	}

}

namespace GLOBALS
{
    implementation struct _Global_Normal_RNG : public Polaris_Component<MasterType,INHERIT(_Global_Normal_RNG),NULLTYPE>
    {
        typedef RNG_Components::Implementations::MT_Normal<NULLTYPE> RNG_type;

        _Global_Normal_RNG()
        {

        }
        void Initialize()
        {
            thread_rng = new RNG_type[num_sim_threads()+1];

            for (unsigned int i=0; i < num_sim_threads()+1; i++)
            {
                typedef RNG_Components::Prototypes::RNG<RNG_type> rng_itf;
                rng_itf* rng = (rng_itf*)&this->thread_rng[i];
                int seed = sin((float)i+1)*(float)INT_MAX;
                rng->Initialize<int>(seed);
            }
        }
        void Initialize_Single_Threaded()
        {
            //TODO: Initialize after start up
            thread_rng = new RNG_type[1];

            typedef RNG_Components::Prototypes::RNG<RNG_type> rng_itf;
            rng_itf* rng = (rng_itf*)&this->thread_rng[0];
            // TODO i was used here before, is 1 correct?
            rng->Initialize<int>(abs(std::sin((float)1+1)*(float)INT_MAX));
        }


        template <typename TargetType>
        void Set_Seed(TargetType random_seed)
        {
            for (unsigned int i=0; i < num_sim_threads()+1; i++)
            {
                typedef RNG_Components::Prototypes::RNG<RNG_type> rng_itf;
                rng_itf* rng = (rng_itf*)&this->thread_rng[i];
                rng->Initialize<TargetType>((TargetType)(sin((float)(i+1)*random_seed)*(float)INT_MAX) + random_seed);
            }
        }
        template <typename TargetType>
        void Set_Seed()
        {
            for (unsigned int i=0; i < num_sim_threads()+1; i++)
            {
                TargetType random_seed = time(NULL);
                typedef RNG_Components::Prototypes::RNG<RNG_type> rng_itf;
                rng_itf* rng = (rng_itf*)&this->thread_rng[i];
                // TODO i was used here before, is 1 correct?
                rng->Initialize<TargetType>((TargetType)(abs(std::sin((float)(1+1)*random_seed)*(float)INT_MAX)) + random_seed);
            }
        }

        template <typename TargetType>
        TargetType Next_Rand(TargetType mu=0, TargetType sigma=1)
        {
            typedef RNG_Components::Prototypes::RNG<RNG_type> rng_itf;
            rng_itf* rng = (rng_itf*)&this->thread_rng[__thread_id];
            TargetType r = rng->Next_Rand<TargetType>();
            return r*sigma + mu;
        }



        template <typename TargetType>
        void Correlated_Norms(std::vector<TargetType>& correlated_random_values, matrix<TargetType>& Sigma)
        {
            correlated_random_values.clear();

            // factor the covariance matrix
            matrix<TargetType> L;
            Sigma.cholesky(L);

            // create std::vector of uncorrelated normals
            matrix<TargetType> norm = matrix<TargetType>(typename matrix<TargetType>::index_type(Sigma.num_rows(),1),0);
            for (uint i = 0; i < Sigma.num_rows(); ++i) norm(i,0) = this->Next_Rand<TargetType>();

            // correlate the normals and populate the return std::vector
            matrix<TargetType> corr_norm = L*norm;
            for (uint i = 0; i < Sigma.num_rows(); ++i) correlated_random_values.push_back(corr_norm(i,0));
        }
    private:
         RNG_type* thread_rng;
    };

    //TODO: should be extern
    static _Global_Normal_RNG<NULLTYPE> Normal_RNG;

    implementation struct _Global_RNG : public Polaris_Component<MasterType,INHERIT(_Global_RNG),NULLTYPE>
	{
		typedef RNG_Components::Implementations::MT_Probability<NULLTYPE> RNG_type;
		_Global_RNG()
		{
			
		}
		void Initialize()
		{
			//TODO: Initialize after start up
			thread_rng = new RNG_type[num_sim_threads()+1];
		
			for (unsigned int i=0; i < num_sim_threads()+1; i++)
			{
				typedef RNG_Components::Prototypes::RNG<RNG_type> rng_itf;
				rng_itf* rng = (rng_itf*)&this->thread_rng[i];
				rng->Initialize<int>(abs(std::sin((float)i+1)*(float)INT_MAX));
			}
		}
		void Initialize_Single_Threaded()
		{
			//TODO: Initialize after start up
			thread_rng = new RNG_type[1];
		
			typedef RNG_Components::Prototypes::RNG<RNG_type> rng_itf;
			rng_itf* rng = (rng_itf*)&this->thread_rng[0];
			// TODO i was used here before, is 1 correct?
			rng->Initialize<int>(abs(std::sin((float)1+1)*(float)INT_MAX));
		}

		template <typename TargetType>
		void Set_Seed(TargetType random_seed)
		{
			for (unsigned int i=0; i < num_sim_threads()+1; i++)
			{
				typedef RNG_Components::Prototypes::RNG<RNG_type> rng_itf;
				rng_itf* rng = (rng_itf*)&this->thread_rng[i];
				rng->Initialize<TargetType>((TargetType)(abs(std::sin((float)(i+1)*random_seed)*(float)INT_MAX)) + random_seed);
			}
		}
		template <typename TargetType>
		void Set_Seed()
		{
			for (unsigned int i=0; i < num_sim_threads()+1; i++)
			{
				TargetType random_seed = time(NULL);
				typedef RNG_Components::Prototypes::RNG<RNG_type> rng_itf;
				rng_itf* rng = (rng_itf*)&this->thread_rng[i];
				rng->Initialize<TargetType>((TargetType)(abs(std::sin((float)(i+1)*random_seed)*(float)INT_MAX)) + random_seed);
			}
		}


		template <typename TargetType>
		TargetType Next_Rand()
		{
			typedef RNG_Components::Prototypes::RNG<RNG_type> rng_itf;
			rng_itf* rng = (rng_itf*)&this->thread_rng[__thread_id];

			int local_sim_threads =	num_sim_threads()+1;
			int local_tid = __thread_id;
			bool pause = true;

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

		template <typename TargetType>
		void Correlated_Rands(std::vector<TargetType>& correlated_random_values, matrix<TargetType>& Sigma)
		{
			// TODO Normal_RNG not a member of GLOBALS
			GLOBALS::Normal_RNG.template Correlated_Norms<TargetType>(correlated_random_values, Sigma);
			
			for (uint i=0; i<Sigma.num_rows();++i)
			{
				correlated_random_values[i] = RNG_Components::Implementations::MT_Normal<NT>::Cumulative_Distribution<TargetType>(correlated_random_values[i],0,1);
			}
		}

	private:
		 RNG_type* thread_rng;
	};
	static _Global_RNG<NULLTYPE> Uniform_RNG;
}

using namespace GLOBALS;
