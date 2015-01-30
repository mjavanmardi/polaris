#pragma once
#include "Repository_Includes.h"
#include "RNG_Implementations.h"
///


namespace Hazard_Model_Components
{
	//==================================================================================================================
	/// Namespace of useful types.  Add any types that convey semantic infomration regarding inputs, outpute, requirements, etc. 
	/// for components in this namespace.  Use well defined typenames that convey the purpose of the Tag
	//------------------------------------------------------------------------------------------------------------------
	namespace Types
	{
		typedef true_type Weibull_Hazard_tag;
		typedef true_type Additive_Weibull_Hazard_tag;
	}


	//==================================================================================================================
	/// Namespace which holds all the concepts which enforce requirements and restrictions on template members, including components 
	/// List all concepts that may be used by this namespace here
	//------------------------------------------------------------------------------------------------------------------
	namespace Concepts
	{
		concept struct Is_Weibull_Hazard
		{
			check_typedef_name(Is_Weibull, Weibull_Hazard_tag);
			define_default_check(Is_Weibull);
		};
		concept struct Is_Additive_Weibull_Hazard
		{
			check_typedef_name(Is_Weibull, Additive_Weibull_Hazard_tag);
			define_default_check(Is_Weibull);
		};

	}


	//==================================================================================================================
	/// Namespace which includes the interface(or interfaces if necessary) which links to the various Bases regarding the transportation object
	//------------------------------------------------------------------------------------------------------------------
	namespace Prototypes
	{
		prototype struct Hazard_Model ADD_DEBUG_INFO
		{
			tag_as_prototype;

			template<typename TargetType> void Initialize(TargetType gamma, TargetType constant, requires(TargetType, check(ComponentType,Concepts::Is_Weibull_Hazard)))
			{
				this_component()->Initialize(gamma, constant);
			}
			template<typename TargetType> void Initialize(TargetType gamma, TargetType constant, requires(TargetType, !check(ComponentType,Concepts::Is_Weibull_Hazard)))
			{
				assert_check(ComponentType,Concepts::Is_Weibull_Hazard,"Error");
			}
			template<typename TargetType> void Initialize(TargetType gamma_e, TargetType constant_e, TargetType gamma_l, TargetType constant_l, requires(TargetType, check(ComponentType,Concepts::Is_Additive_Weibull_Hazard)))
			{
				this_component()->Initialize(gamma_e, constant_e, gamma_l, constant_l);
			}
			template<typename TargetType> void Initialize(TargetType gamma_e, TargetType constant_e, TargetType gamma_l, TargetType constant_l, requires(TargetType, !check(ComponentType,Concepts::Is_Additive_Weibull_Hazard)))
			{
				assert_check(ComponentType,Concepts::Is_Additive_Weibull_Hazard,"Error");
			}

			template<typename TargetType, typename TimeType> TargetType Evaluate_Baseline_Hazard(TimeType t)
			{	
				return this_component()->Evaluate_Baseline_Hazard<TargetType,TimeType>(t);
			}	
			template<typename TargetType, typename TimeType> TargetType Evaluate_Survival(TimeType t, TargetType BX)
			{
				return this_component()->Evaluate_Survival<TargetType,TimeType>(t, BX);
			}
			template<typename TargetType, typename TimeType> TargetType Evaluate_Failure_Probability(TimeType t1, TimeType t2, TargetType BetaX)
			{	
				return this_component()->Evaluate_Failure_Probability<TargetType,TimeType>(t1,t2,BetaX);
			}

			template<typename TargetType, typename TimeType> TimeType Evaluate_Inverse_Survival(TargetType p, TargetType BetaX)
			{	
				return this_component()->Evaluate_Inverse_Survival<TargetType,TimeType>(p,BetaX);
			}



			/// SELECT FROM THE AVAILABLE CHOICES FOR SIMULATION
			//template<typename TargetType> TargetType Choose(int& selected_index, requires(TargetType,check(ComponentType, Concepts::Is_Probabilistic)))
			//{
			//	typedef Random_Access_Sequence<typename get_type_of(choice_options)> choice_options_itf;
			//	typedef Choice_Option<Implementations::Choice_Option_Base<NT>> choice_option_itf;
			//	typedef Random_Access_Sequence<typename get_type_of(choice_probabilities),double> probabilities_itf;

			//	// Local type definition option
			//	choice_options_itf* choices = this->choice_options<choice_options_itf*>();
			//	probabilities_itf* probs = this->choice_probabilities<probabilities_itf*>();
			//	typename probabilities_itf::iterator p_itr = probs->begin(); 

			//	double cumulative_probability = 0;
			//	
			//	double rand = Uniform_RNG.Next_Rand<double>();

			//	int i = 0;
			//	for (typename choice_options_itf::iterator itr = choices->begin(); itr != choices->end(); ++itr, ++p_itr)
			//	{
			//		cumulative_probability += *p_itr;
			//		if (ISNAN(cumulative_probability)) THROW_WARNING("WARNING: p is not a number. p=" << *p_itr << ", cum_p="<<cumulative_probability << ", index="<<i);
			//		if (rand < cumulative_probability) 
			//		{
			//			selected_index = i;
			//			return (TargetType)(*itr);
			//		}
			//		i++;
			//	}
			//	
			//	THROW_WARNING("WARNING: No choice selected from choice model (prob, cumulative_prob, selected_index) "<<rand << ", " << cumulative_probability << ", " << selected_index);
			//	return nullptr;
			//}
			//template<typename TargetType> TargetType Choose(int& selected_index,requires(TargetType,!check(ComponentType, Concepts::Is_Probabilistic)))
			//{
			//	assert_check(ComponentType, Concepts::Is_Probabilistic, "ComponentType does not specify if Choice is Deterministic or Probabilistic");
			//}
		 //
			//template<typename TargetType> TargetType Choice_At(int selected_index)
			//{
			//	typedef Random_Access_Sequence<typename get_type_of(choice_options),void*> choice_options_itf;

			//	// Local type definition option
			//	choice_options_itf* choices = this->choice_options<choice_options_itf*>();

			//	return (TargetType)choices->at(selected_index);
			//}

			///// ACCESSORS
			//accessor(choice_options, NONE, NONE);
			//accessor(choice_utilities, NONE, NONE);
			//accessor(choice_probabilities, NONE, NONE);
		};
	}
}
