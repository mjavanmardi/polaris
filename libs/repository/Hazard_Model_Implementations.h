#pragma once

#include "Hazard_Model_Prototypes.h"


namespace Hazard_Model_Components
{
	//==================================================================================================================
	/// Namespace of Base classes related to the Interface(s) from the component namespace.
	/// The bases are combined with the interfaces to create components by adding data.
	//------------------------------------------------------------------------------------------------------------------
	namespace Implementations
	{
		implementation struct Hazard_Model_Implementation : public Polaris_Component<MT,INHERIT(Hazard_Model_Implementation),Data_Object>
		{
			typedef typename Polaris_Component<MT,INHERIT(Hazard_Model_Implementation),Data_Object>::ComponentType ComponentType;

			typedef Prototypes::Hazard_Model<ComponentType> this_itf;

			template<typename TargetType, typename TimeType> TargetType Evaluate_Failure_Probability(TimeType t1, TimeType t2, TargetType BX)
			{	
				this_itf* pthis = (this_itf*)this;
				double S0 = pthis->Evaluate_Survival(t1,BX);
				double S1 = pthis->Evaluate_Survival(t2,BX);
				return (S0-S1) / S1;
			}

			template<typename TargetType, typename TimeType> TimeType Evaluate_Inverse_Survival(TargetType p, TargetType BX)
			{
				this_itf* pthis = (this_itf*)this;

				TimeType t_l = 0.0;
				TimeType t_r = 20.0;
				TimeType t_mid;
				double TOL = 1.0 / 1440.0;
				int iter = 0;
				int Maxiter = 200;

				// Get endpoint function values
				double F_l, F_r, F_mid;
				F_l = pthis->Evaluate_Survival(t_l,BX) - p;
				F_r = pthis->Evaluate_Survival(t_r,BX) - p;

				// move negative endpoint further if function doesnt cross zero
				while (F_r > 0)
				{
					t_r = t_r * 2.0;
					F_r = pthis->Evaluate_Survival(t_r,BX) - p;
				}

				while (t_r - t_l > TOL)
				{
					// Get endpoint function values
					F_l = pthis->Evaluate_Survival(t_l,BX) - p;
					F_r = pthis->Evaluate_Survival(t_r,BX) - p;

					// calculate function value at bisection point
					t_mid = (t_r - t_l) / 2.0 + t_l;
					F_mid = pthis->Evaluate_Survival(t_mid,BX) - p;
					if (F_mid > 0) t_l = t_mid;
					else t_r = t_mid;
					
					// Throw exception if no root was found within limit
					if (iter >= Maxiter) THROW_EXCEPTION("ERROR: root-finder failed during initialization of last activity times.");
					iter++;
				}
				return t_mid;
			}
		};

		implementation struct Weibull_Baseline_Hazard_Implementation : public Hazard_Model_Implementation<MT,INHERIT(Weibull_Baseline_Hazard_Implementation)>
		{
			typedef true_type Weibull_Hazard_tag;

			m_data(double, gamma, NONE, NONE);
			m_data(double, constant, NONE, NONE);

			Weibull_Baseline_Hazard_Implementation()
			{
				this->_gamma=1.0;
				this->_constant=0.0;
			}

			template<typename TargetType> void Initialize(TargetType gamma, TargetType constant)
			{	
				if (gamma<=0) THROW_EXCEPTION("Error: gamma value '"<<gamma<<"' not supported for Weibull hazard function.  Must be greater than 0.")
				this->_gamma=gamma;
				this->_constant=constant;
			}	

			template<typename TargetType, typename TimeType> TargetType Evaluate_Baseline_Hazard(TimeType t)
			{	
				return std::exp(-1.0*_constant) * _gamma * std::pow(t,_gamma-1);
			}

			template<typename TargetType, typename TimeType> TargetType Evaluate_Survival(TimeType t, TargetType BX)
			{	
				//			exp (			-	C				*  exp(-Bx)			  * t^gamma  )
				return std::exp(-1.0*(std::exp(-1.0*_constant)) * std::exp(-1.0 * BX) * std::pow(t,_gamma) );
			}

		};

		implementation struct Additive_Weibull_Baseline_Hazard_Implementation : public Weibull_Baseline_Hazard_Implementation<MT,INHERIT(Additive_Weibull_Baseline_Hazard_Implementation)>
		{
			typedef Weibull_Baseline_Hazard_Implementation<MT,INHERIT(Additive_Weibull_Baseline_Hazard_Implementation)> Base_t;
			using Base_t::_constant;
			using Base_t::_gamma;

			typedef true_type Additive_Weibull_Hazard_tag;

			m_data(double, l_gamma, NONE, NONE);
			m_data(double, l_constant, NONE, NONE);

			typedef Weibull_Baseline_Hazard_Implementation<MasterType,INHERIT(Additive_Weibull_Baseline_Hazard_Implementation)> Base_Type;
			using Base_Type::Initialize;

			Additive_Weibull_Baseline_Hazard_Implementation()
			{
				Base_Type();
				this->_l_gamma=1.0;
				this->_l_constant=0.0;
			}

			template<typename TargetType> void Initialize(TargetType gamma_e, TargetType constant_e, TargetType gamma_l, TargetType constant_l)
			{	
				if (gamma_l<=0) THROW_EXCEPTION("Error: gamma_l value '"<<gamma_l<<"' not supported for Weibull hazard function.  Must be greater than 0.")
				Base_Type::Initialize(gamma_e, constant_e);
				this->_l_gamma=gamma_l;
				this->_l_constant=constant_l;
			}	

			template<typename TargetType, typename TimeType> TargetType Evaluate_Baseline_Hazard(TimeType t)
			{	
				TargetType val = Base_Type::template Evaluate_Baseline_Hazard<TargetType,TimeType>(t);
				return val + std::exp(-1.0*_l_constant) * _l_gamma * std::pow(t,_l_gamma-1);
			}

			template<typename TargetType, typename TimeType> TargetType Evaluate_Survival(TimeType t, TargetType BX)
			{	
				return std::exp((-1.0*(std::exp(-1.0*_constant)) * std::pow(t,_gamma) - (std::exp(-1.0*_l_constant)) * std::pow(t,_l_gamma))* std::exp(-1.0 * BX));
			}
		};

	}
}
