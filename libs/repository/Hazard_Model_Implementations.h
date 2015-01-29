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
			template<typename TargetType, typename TimeType> TargetType Evaluate_Failure_Probability(TimeType t1, TimeType t2, TargetType BX)
			{	
				typedef Prototypes::Hazard_Model<ComponentType> this_itf;
				this_itf* pthis = (this_itf*)this;
				double S0 = pthis->Evaluate_Survival(t1,BX);
				double S1 = pthis->Evaluate_Survival(t2,BX);
				return (S0-S1) / S1;
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
				Base_Type::Initialize(gamma_e, constant_e);
				this->_l_gamma=gamma_l;
				this->_l_constant=constant_l;
			}	

			template<typename TargetType, typename TimeType> TargetType Evaluate_Baseline_Hazard(TimeType t)
			{	
				TargetType val = Base_Type::Evaluate_Baseline_Hazard<TargetType,TimeType>(t);
				return val + std::exp(-1.0*_l_constant) * _l_gamma * std::pow(t,_l_gamma-1);
			}

			template<typename TargetType, typename TimeType> TargetType Evaluate_Survival(TimeType t, TargetType BX)
			{	
				return std::exp((-1.0*(std::exp(-1.0*_constant)) * std::pow(t,_gamma) - (std::exp(-1.0*_l_constant)) * std::pow(t,_l_gamma))* std::exp(-1.0 * BX));
			}
		};

	}
}
