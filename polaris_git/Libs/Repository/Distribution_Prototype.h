#pragma once
#include "Repository_Includes.h"


namespace Distributions
{
	namespace Types
	{
	}

	namespace Concepts
	{

	}
	
	namespace Prototypes
	{
		prototype struct Distribution
		{
			tag_as_prototype;

			// Probability density function
			define_feature_exists_check(Probability_Density_Value, has_pdf);
			feature_prototype TargetType Probability_Density_Value(TargetType x ,requires(check(ComponentType,has_pdf) && check(TargetType, is_arithmetic)))
			{
				return this_component()->Probability_Density_Value<ComponentType,CallerType,TargetType>(x);
			}
			feature_prototype TargetType Probability_Density_Value(TargetType x, requires(!check(ComponentType,has_pdf) || !check(TargetType, is_arithmetic)))
			{
				assert_check(ComponentType,has_pdf, "Error, the Distribution ComponentType does not have a PDF defined."); 
				assert_check(TargetType, is_arithmetic, "Error, TargetType for this function must be arithmetic.");
			}
			feature_prototype TargetType Probability_Density_Value(TargetType x, TargetType location, requires(check(ComponentType,has_pdf) && check(TargetType, is_arithmetic)))
			{
				return this_component()->Probability_Density_Value<ComponentType,CallerType,TargetType>(x, location);
			}
			feature_prototype TargetType Probability_Density_Value(TargetType x, TargetType location, requires(!check(ComponentType,has_pdf) || !check(TargetType, is_arithmetic)))
			{
				assert_check(ComponentType,has_pdf, "Error, the Distribution ComponentType does not have a PDF defined."); 
				assert_check(TargetType, is_arithmetic, "Error, TargetType for this function must be arithmetic.");
			}
			feature_prototype TargetType Probability_Density_Value(TargetType x, TargetType location, TargetType scale, requires(check(ComponentType,has_pdf) && check(TargetType, is_arithmetic)))
			{
				return this_component()->Probability_Density_Value<ComponentType,CallerType,TargetType>(x, location, scale);
			}
			feature_prototype TargetType Probability_Density_Value(TargetType x, TargetType location, TargetType scale, requires(!check(ComponentType,has_pdf) || !check(TargetType, is_arithmetic)))
			{
				assert_check(ComponentType,has_pdf, "Error, the Distribution ComponentType does not have a PDF defined."); 
				assert_check(TargetType, is_arithmetic, "Error, TargetType for this function must be arithmetic.");
			}	
			feature_prototype TargetType Probability_Density_Value(TargetType x, TargetType location, TargetType scale, TargetType shape,requires(check(ComponentType,has_pdf) && check(TargetType, is_arithmetic)))
			{
				return this_component()->Probability_Density_Value<ComponentType,CallerType,TargetType>(x, location, scale, shape);
			}
			feature_prototype TargetType Probability_Density_Value(TargetType x, TargetType location, TargetType scale, TargetType shape,requires(!check(ComponentType,has_pdf) || !check(TargetType, is_arithmetic)))
			{
				assert_check(ComponentType,has_pdf, "Error, the Distribution ComponentType does not have a PDF defined."); 
				assert_check(TargetType, is_arithmetic, "Error, TargetType for this function must be arithmetic.");
			}
			
			// Cumulative distribution function
			define_feature_exists_check(Cumulative_Distribution_Value, has_cdf);
			feature_prototype TargetType Cumulative_Distribution_Value(TargetType x ,requires(check(ComponentType,has_cdf) && check(TargetType, is_arithmetic)))
			{
				return this_component()->Cumulative_Distribution_Value<ComponentType,CallerType,TargetType>(x);
			}
			feature_prototype TargetType Cumulative_Distribution_Value(TargetType x, requires(!check(ComponentType,has_cdf) || !check(TargetType, is_arithmetic)))
			{
				assert_check(ComponentType,has_cdf, "Error, the Distribution ComponentType does not have a CDF defined."); 
				assert_check(TargetType, is_arithmetic, "Error, TargetType for this function must be arithmetic.");
			}
			feature_prototype TargetType Cumulative_Distribution_Value(TargetType x, TargetType location, requires(check(ComponentType,has_cdf) && check(TargetType, is_arithmetic)))
			{
				return this_component()->Cumulative_Distribution_Value<ComponentType,CallerType,TargetType>(x, location);
			}
			feature_prototype TargetType Cumulative_Distribution_Value(TargetType x, TargetType location, requires(!check(ComponentType,has_cdf) || !check(TargetType, is_arithmetic)))
			{
				assert_check(ComponentType,has_cdf, "Error, the Distribution ComponentType does not have a CDF defined."); 
				assert_check(TargetType, is_arithmetic, "Error, TargetType for this function must be arithmetic.");
			}
			feature_prototype TargetType Cumulative_Distribution_Value(TargetType x, TargetType location, TargetType scale, requires(check(ComponentType,has_cdf) && check(TargetType, is_arithmetic)))
			{
				return this_component()->Cumulative_Distribution_Value<ComponentType,CallerType,TargetType>(x, location, scale);
			}
			feature_prototype TargetType Cumulative_Distribution_Value(TargetType x, TargetType location, TargetType scale, requires(!check(ComponentType,has_cdf) || !check(TargetType, is_arithmetic)))
			{
				assert_check(ComponentType,has_cdf, "Error, the Distribution ComponentType does not have a CDF defined."); 
				assert_check(TargetType, is_arithmetic, "Error, TargetType for this function must be arithmetic.");
			}			
			feature_prototype TargetType Cumulative_Distribution_Value(TargetType x, TargetType location, TargetType scale, TargetType shape, requires(check(ComponentType,has_cdf) && check(TargetType, is_arithmetic)))
			{
				return this_component()->Cumulative_Distribution_Value<ComponentType,CallerType,TargetType>(x, location, scale, shape);
			}
			feature_prototype TargetType Cumulative_Distribution_Value(TargetType x, TargetType location, TargetType scale, TargetType shape, requires(!check(ComponentType,has_cdf) || !check(TargetType, is_arithmetic)))
			{
				assert_check(ComponentType,has_cdf, "Error, the Distribution ComponentType does not have a CDF defined."); 
				assert_check(TargetType, is_arithmetic, "Error, TargetType for this function must be arithmetic.");
			}
		};
	}

}

using namespace Distributions::Prototypes;