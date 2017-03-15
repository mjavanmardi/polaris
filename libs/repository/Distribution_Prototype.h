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
		prototype struct Distribution ADD_DEBUG_INFO
		{
			tag_as_prototype;

			// Probability density function
			local_check_template_method_name(has_pdf, Probability_Density_Value);
			template<typename TargetType> TargetType Probability_Density_Value(TargetType x ,requires(TargetType,check(ComponentType,has_pdf) && check(strip_modifiers(TargetType), is_arithmetic)))
			{
				return this_component()->Probability_Density_Value<TargetType>(x);
			}
			template<typename TargetType> TargetType Probability_Density_Value(TargetType x, requires(TargetType,!check(ComponentType,has_pdf) || !check(strip_modifiers(TargetType), is_arithmetic)))
			{
				assert_check(ComponentType,has_pdf, "Error, the Distribution ComponentType does not have a PDF defined."); 
				assert_check(strip_modifiers(TargetType), is_arithmetic, "Error, TargetType for this function must be arithmetic.");
			}
			template<typename TargetType> TargetType Probability_Density_Value(TargetType x, TargetType location, requires(TargetType,check(ComponentType,has_pdf) && check(strip_modifiers(TargetType), is_arithmetic)))
			{
				return this_component()->Probability_Density_Value<TargetType>(x, location);
			}
			template<typename TargetType> TargetType Probability_Density_Value(TargetType x, TargetType location, requires(TargetType,!check(ComponentType,has_pdf) || !check(strip_modifiers(TargetType), is_arithmetic)))
			{
				assert_check(ComponentType,has_pdf, "Error, the Distribution ComponentType does not have a PDF defined."); 
				assert_check(strip_modifiers(TargetType), is_arithmetic, "Error, TargetType for this function must be arithmetic.");
			}
			template<typename TargetType> TargetType Probability_Density_Value(TargetType x, TargetType location, TargetType scale, requires(TargetType,check(ComponentType,has_pdf) && check(strip_modifiers(TargetType), is_arithmetic)))
			{
				return this_component()->Probability_Density_Value<TargetType>(x, location, scale);
			}
			template<typename TargetType> TargetType Probability_Density_Value(TargetType x, TargetType location, TargetType scale, requires(TargetType,!check(ComponentType,has_pdf) || !check(strip_modifiers(TargetType), is_arithmetic)))
			{
				assert_check(ComponentType,has_pdf, "Error, the Distribution ComponentType does not have a PDF defined."); 
				assert_check(strip_modifiers(TargetType), is_arithmetic, "Error, TargetType for this function must be arithmetic.");
			}	
			template<typename TargetType> TargetType Probability_Density_Value(TargetType x, TargetType location, TargetType scale, TargetType shape,requires(TargetType,check(ComponentType,has_pdf) && check(strip_modifiers(TargetType), is_arithmetic)))
			{
				return this_component()->Probability_Density_Value<TargetType>(x, location, scale, shape);
			}
			template<typename TargetType> TargetType Probability_Density_Value(TargetType x, TargetType location, TargetType scale, TargetType shape,requires(TargetType,!check(ComponentType,has_pdf) || !check(strip_modifiers(TargetType), is_arithmetic)))
			{
				assert_check(ComponentType,has_pdf, "Error, the Distribution ComponentType does not have a PDF defined."); 
				assert_check(strip_modifiers(TargetType), is_arithmetic, "Error, TargetType for this function must be arithmetic.");
			}
			
			// Cumulative distribution function
			local_check_template_method_name(has_cdf, Cumulative_Distribution_Value);
			template<typename TargetType> TargetType Cumulative_Distribution_Value(TargetType x ,requires(TargetType,check(ComponentType,has_cdf) && check(strip_modifiers(TargetType), is_arithmetic)))
			{
				return this_component()->Cumulative_Distribution_Value<TargetType>(x);
			}
			template<typename TargetType> TargetType Cumulative_Distribution_Value(TargetType x, requires(TargetType,!check(ComponentType,has_cdf) || !check(strip_modifiers(TargetType), is_arithmetic)))
			{
				assert_check(ComponentType,has_cdf, "Error, the Distribution ComponentType does not have a CDF defined."); 
				assert_check(strip_modifiers(TargetType), is_arithmetic, "Error, TargetType for this function must be arithmetic.");
			}
			template<typename TargetType> TargetType Cumulative_Distribution_Value(TargetType x, TargetType location, requires(TargetType,check(ComponentType,has_cdf) && check(strip_modifiers(TargetType), is_arithmetic)))
			{
				return this_component()->Cumulative_Distribution_Value<TargetType>(x, location);
			}
			template<typename TargetType> TargetType Cumulative_Distribution_Value(TargetType x, TargetType location, requires(TargetType,!check(ComponentType,has_cdf) || !check(strip_modifiers(TargetType), is_arithmetic)))
			{
				assert_check(ComponentType,has_cdf, "Error, the Distribution ComponentType does not have a CDF defined."); 
				assert_check(strip_modifiers(TargetType), is_arithmetic, "Error, TargetType for this function must be arithmetic.");
			}
			template<typename TargetType> TargetType Cumulative_Distribution_Value(TargetType x, TargetType location, TargetType scale, requires(TargetType,check(ComponentType,has_cdf) && check(strip_modifiers(TargetType), is_arithmetic)))
			{
				return this_component()->Cumulative_Distribution_Value<TargetType>(x, location, scale);
			}
			template<typename TargetType> TargetType Cumulative_Distribution_Value(TargetType x, TargetType location, TargetType scale, requires(TargetType,!check(ComponentType,has_cdf) || !check(strip_modifiers(TargetType), is_arithmetic)))
			{
				assert_check(ComponentType,has_cdf, "Error, the Distribution ComponentType does not have a CDF defined."); 
				assert_check(strip_modifiers(TargetType), is_arithmetic, "Error, TargetType for this function must be arithmetic.");
			}			
			template<typename TargetType> TargetType Cumulative_Distribution_Value(TargetType x, TargetType location, TargetType scale, TargetType shape, requires(TargetType,check(ComponentType,has_cdf) && check(strip_modifiers(TargetType), is_arithmetic)))
			{
				return this_component()->Cumulative_Distribution_Value<TargetType>(x, location, scale, shape);
			}
			template<typename TargetType> TargetType Cumulative_Distribution_Value(TargetType x, TargetType location, TargetType scale, TargetType shape, requires(TargetType,!check(ComponentType,has_cdf) || !check(strip_modifiers(TargetType), is_arithmetic)))
			{
				assert_check(ComponentType,has_cdf, "Error, the Distribution ComponentType does not have a CDF defined."); 
				assert_check(strip_modifiers(TargetType), is_arithmetic, "Error, TargetType for this function must be arithmetic.");
			}
		};

		prototype struct Bivariate_Distribution ADD_DEBUG_INFO
		{
			tag_as_prototype;

			// Probability density function
			local_check_template_method_name(has_pdf, Probability_Density_Value);
			template<typename TargetType> TargetType Probability_Density_Value(TargetType x, TargetType y, requires(TargetType, check(ComponentType, has_pdf) && check(strip_modifiers(TargetType), is_arithmetic)))
			{
				return this_component()->Probability_Density_Value<TargetType>(x,y);
			}
			template<typename TargetType> TargetType Probability_Density_Value(TargetType x, TargetType y, requires(TargetType, !check(ComponentType, has_pdf) || !check(strip_modifiers(TargetType), is_arithmetic)))
			{
				assert_check(ComponentType, has_pdf, "Error, the Distribution ComponentType does not have a PDF defined.");
				assert_check(strip_modifiers(TargetType), is_arithmetic, "Error, TargetType for this function must be arithmetic.");
			}
			template<typename TargetType> TargetType Probability_Density_Value(TargetType x, TargetType y, TargetType location, requires(TargetType, check(ComponentType, has_pdf) && check(strip_modifiers(TargetType), is_arithmetic)))
			{
				return this_component()->Probability_Density_Value<TargetType>(x, location);
			}
			template<typename TargetType> TargetType Probability_Density_Value(TargetType x, TargetType y, TargetType location, requires(TargetType, !check(ComponentType, has_pdf) || !check(strip_modifiers(TargetType), is_arithmetic)))
			{
				assert_check(ComponentType, has_pdf, "Error, the Distribution ComponentType does not have a PDF defined.");
				assert_check(strip_modifiers(TargetType), is_arithmetic, "Error, TargetType for this function must be arithmetic.");
			}
			template<typename TargetType> TargetType Probability_Density_Value(TargetType x, TargetType y, TargetType location, TargetType scale, requires(TargetType, check(ComponentType, has_pdf) && check(strip_modifiers(TargetType), is_arithmetic)))
			{
				return this_component()->Probability_Density_Value<TargetType>(x, location, scale);
			}
			template<typename TargetType> TargetType Probability_Density_Value(TargetType x, TargetType y, TargetType location, TargetType scale, requires(TargetType, !check(ComponentType, has_pdf) || !check(strip_modifiers(TargetType), is_arithmetic)))
			{
				assert_check(ComponentType, has_pdf, "Error, the Distribution ComponentType does not have a PDF defined.");
				assert_check(strip_modifiers(TargetType), is_arithmetic, "Error, TargetType for this function must be arithmetic.");
			}
			template<typename TargetType> TargetType Probability_Density_Value(TargetType x, TargetType y, TargetType location, TargetType scale, TargetType shape, requires(TargetType, check(ComponentType, has_pdf) && check(strip_modifiers(TargetType), is_arithmetic)))
			{
				return this_component()->Probability_Density_Value<TargetType>(x, location, scale, shape);
			}
			template<typename TargetType> TargetType Probability_Density_Value(TargetType x, TargetType y, TargetType location, TargetType scale, TargetType shape, requires(TargetType, !check(ComponentType, has_pdf) || !check(strip_modifiers(TargetType), is_arithmetic)))
			{
				assert_check(ComponentType, has_pdf, "Error, the Distribution ComponentType does not have a PDF defined.");
				assert_check(strip_modifiers(TargetType), is_arithmetic, "Error, TargetType for this function must be arithmetic.");
			}

			// Cumulative distribution function
			local_check_template_method_name(has_cdf, Cumulative_Distribution_Value);
			template<typename TargetType> TargetType Cumulative_Distribution_Value(TargetType x, TargetType y, TargetType rho, requires(TargetType, check(ComponentType, has_cdf) && check(strip_modifiers(TargetType), is_arithmetic)))
			{
				return this_component()->Cumulative_Distribution_Value<TargetType>(x,y,rho);
			}
			template<typename TargetType> TargetType Cumulative_Distribution_Value(TargetType x, TargetType y, TargetType rho, requires(TargetType, !check(ComponentType, has_cdf) || !check(strip_modifiers(TargetType), is_arithmetic)))
			{
				assert_check(ComponentType, has_cdf, "Error, the Distribution ComponentType does not have a CDF defined.");
				assert_check(strip_modifiers(TargetType), is_arithmetic, "Error, TargetType for this function must be arithmetic.");
			}
			template<typename TargetType> TargetType Cumulative_Distribution_Value(TargetType x, TargetType y, TargetType rho, TargetType location, requires(TargetType, check(ComponentType, has_cdf) && check(strip_modifiers(TargetType), is_arithmetic)))
			{
				return this_component()->Cumulative_Distribution_Value<TargetType>(x, y, rho, location);
			}
			template<typename TargetType> TargetType Cumulative_Distribution_Value(TargetType x, TargetType y, TargetType rho, TargetType location, requires(TargetType, !check(ComponentType, has_cdf) || !check(strip_modifiers(TargetType), is_arithmetic)))
			{
				assert_check(ComponentType, has_cdf, "Error, the Distribution ComponentType does not have a CDF defined.");
				assert_check(strip_modifiers(TargetType), is_arithmetic, "Error, TargetType for this function must be arithmetic.");
			}
			template<typename TargetType> TargetType Cumulative_Distribution_Value(TargetType x, TargetType y, TargetType rho, TargetType location, TargetType scale, requires(TargetType, check(ComponentType, has_cdf) && check(strip_modifiers(TargetType), is_arithmetic)))
			{
				return this_component()->Cumulative_Distribution_Value<TargetType>(x, y, rho, location, scale);
			}
			template<typename TargetType> TargetType Cumulative_Distribution_Value(TargetType x, TargetType y, TargetType rho, TargetType location, TargetType scale, requires(TargetType, !check(ComponentType, has_cdf) || !check(strip_modifiers(TargetType), is_arithmetic)))
			{
				assert_check(ComponentType, has_cdf, "Error, the Distribution ComponentType does not have a CDF defined.");
				assert_check(strip_modifiers(TargetType), is_arithmetic, "Error, TargetType for this function must be arithmetic.");
			}
			template<typename TargetType> TargetType Cumulative_Distribution_Value(TargetType x, TargetType y, TargetType rho, TargetType location, TargetType scale, TargetType shape, requires(TargetType, check(ComponentType, has_cdf) && check(strip_modifiers(TargetType), is_arithmetic)))
			{
				return this_component()->Cumulative_Distribution_Value<TargetType>(x, y, rho, location, scale, shape);
			}
			template<typename TargetType> TargetType Cumulative_Distribution_Value(TargetType x, TargetType y, TargetType rho, TargetType location, TargetType scale, TargetType shape, requires(TargetType, !check(ComponentType, has_cdf) || !check(strip_modifiers(TargetType), is_arithmetic)))
			{
				assert_check(ComponentType, has_cdf, "Error, the Distribution ComponentType does not have a CDF defined.");
				assert_check(strip_modifiers(TargetType), is_arithmetic, "Error, TargetType for this function must be arithmetic.");
			}
		};
	}

}

using namespace Distributions::Prototypes;
