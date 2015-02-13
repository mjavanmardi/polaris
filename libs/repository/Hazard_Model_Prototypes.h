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

			//===================================
			// Interfaces to implementation specific methods
			template<typename TargetType> void Initialize(TargetType gamma, TargetType constant, requires(TargetType, check(ComponentType,Concepts::Is_Weibull_Hazard) && check(TargetType, is_arithmetic)))
			{
				this_component()->Initialize(gamma, constant);
			}
			template<typename TargetType> void Initialize(TargetType gamma, TargetType constant, requires(TargetType, !check(ComponentType,Concepts::Is_Weibull_Hazard) || !check(TargetType, is_arithmetic)))
			{
				assert_check(ComponentType,Concepts::Is_Weibull_Hazard,"Error: model implementation does not have Weibull_Hazard_tag.");
				assert_check(TargetType,is_arithmetic,"Error, TargetType must be an arithmetic type.");
			}

			template<typename TargetType> void Initialize(TargetType gamma_e, TargetType constant_e, TargetType gamma_l, TargetType constant_l, requires(TargetType, check(ComponentType,Concepts::Is_Additive_Weibull_Hazard) && check(TargetType, is_arithmetic)))
			{
				this_component()->Initialize(gamma_e, constant_e, gamma_l, constant_l);
			}
			template<typename TargetType> void Initialize(TargetType gamma_e, TargetType constant_e, TargetType gamma_l, TargetType constant_l, requires(TargetType, !check(ComponentType,Concepts::Is_Additive_Weibull_Hazard) || !check(TargetType, is_arithmetic)))
			{
				assert_check(ComponentType,Concepts::Is_Additive_Weibull_Hazard,"Error: model implementation does not have Additive_Weibull_Hazard_tag.");
				assert_check(TargetType,is_arithmetic,"Error, TargetType must be an arithmetic type.");
			}

			//===================================
			// GENERAL HAZARD MODEL FUNCTIONALITY
			template<typename TargetType, typename TimeType> TargetType Evaluate_Baseline_Hazard(TimeType t, requires(TargetType, check(TimeType, Basic_Units::Concepts::Is_Time_Value) && check(TargetType, is_arithmetic)))
			{	
				if (t<0) THROW_EXCEPTION("Error: time value '"<<t<<"' not supported for survival function.  Must be greater than 0.")
				return this_component()->Evaluate_Baseline_Hazard<TargetType,TimeType>(t);
			}	
			template<typename TargetType, typename TimeType> TargetType Evaluate_Baseline_Hazard(TimeType t, requires(TargetType, !check(TimeType, Basic_Units::Concepts::Is_Time_Value) || !check(TargetType, is_arithmetic)))
			{
				assert_check(TimeType,Basic_Units::Concepts::Is_Time_Value,"Error: invalid time type specified.");
				assert_check(TargetType,is_arithmetic,"Error, TargetType must be an arithmetic type.");
			}

			template<typename TargetType, typename TimeType> TargetType Evaluate_Survival(TimeType t, TargetType BX, requires(TargetType, check(TimeType, Basic_Units::Concepts::Is_Time_Value) && check(TargetType, is_arithmetic)))
			{
				if (t<0) THROW_EXCEPTION("Error: time value '"<<t<<"' not supported for survival function.  Must be greater than 0.")
				return this_component()->Evaluate_Survival<TargetType,TimeType>(t, BX);
			}
			template<typename TargetType, typename TimeType> TargetType Evaluate_Survival(TimeType t, TargetType BX, requires(TargetType, !check(TimeType, Basic_Units::Concepts::Is_Time_Value) || !check(TargetType, is_arithmetic)))
			{
				assert_check(TimeType,Basic_Units::Concepts::Is_Time_Value,"Error: invalid time type specified.");
				assert_check(TargetType,is_arithmetic,"Error, TargetType must be an arithmetic type.");
			}

			template<typename TargetType, typename TimeType> TargetType Evaluate_Failure_Probability(TimeType t1, TimeType t2, TargetType BetaX, requires(TargetType, check(TimeType, Basic_Units::Concepts::Is_Time_Value) && check(TargetType, is_arithmetic)))
			{	
				if (t1<0) THROW_EXCEPTION("Error: time value '"<<t1<<"' not supported for survival function.  Must be greater than 0.")
				if (t2<0) THROW_EXCEPTION("Error: time value '"<<t1<<"' not supported for survival function.  Must be greater than 0.")
				if (t2<t1) THROW_EXCEPTION("Error: time value t2="<<t2<<" must be greater than t1="<<t1)
				return this_component()->Evaluate_Failure_Probability<TargetType,TimeType>(t1,t2,BetaX);
			}
			template<typename TargetType, typename TimeType> TargetType Evaluate_Failure_Probability(TimeType t1, TimeType t2, TargetType BetaX, requires(TargetType, !check(TimeType, Basic_Units::Concepts::Is_Time_Value) || !check(TargetType, is_arithmetic)))
			{	
				assert_check(TimeType,Basic_Units::Concepts::Is_Time_Value,"Error: invalid time type specified.");
				assert_check(TargetType,is_arithmetic,"Error, TargetType must be an arithmetic type.");
			}

			template<typename TargetType, typename TimeType> TimeType Evaluate_Inverse_Survival(TargetType p, TargetType BetaX, requires(TargetType, check(TimeType, Basic_Units::Concepts::Is_Time_Value) && check(TargetType, is_arithmetic)))
			{	
				if (p<0 || p>=1) THROW_EXCEPTION("Error: 'p='"<<p<<"' out of range. Probability value for inverse survival must be in the rang 0 <= p < 1")
				return this_component()->Evaluate_Inverse_Survival<TargetType,TimeType>(p,BetaX);
			}
			template<typename TargetType, typename TimeType> TimeType Evaluate_Inverse_Survival(TargetType p, TargetType BetaX, requires(TargetType, !check(TimeType, Basic_Units::Concepts::Is_Time_Value) || !check(TargetType, is_arithmetic)))
			{	
				assert_check(TimeType,Basic_Units::Concepts::Is_Time_Value,"Error: invalid time type specified.");
				assert_check(TargetType,is_arithmetic,"Error, TargetType must be an arithmetic type.");
			}
		};
	}
}
