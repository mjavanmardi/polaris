#pragma once
#include "Distribution_Prototype.h"
#include "RngStream.h"

namespace Distributions
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{
		implementation struct Normal_Distribution_Implementation : public Polaris_Component<MasterType,INHERIT(Normal_Distribution_Implementation)>
		{
			template<typename TargetType> TargetType Probability_Density_Value(TargetType x, TargetType mean=0.0, TargetType std=1.0, TargetType shape=0.0)
			{
				double x_std = ( (double)x - (double)mean ) / (double)std;

				const double INVSQRT2PI	= 0.39894228040143268;

				double p = INVSQRT2PI * exp(- 0.5 * x_std * x_std);

				return (TargetType)p;
			}

			template<typename TargetType> TargetType Cumulative_Distribution_Value(TargetType x, TargetType mean=0.0, TargetType std=1.0, TargetType shape=0.0)
			{
				double x_std = ( (double)x - (double)mean ) / (double)std;
				const double A1	= 0.31938153;
				const double A2	= -0.356563782;
				const double A3	= 1.781477937;
				const double A4	= -1.821255978;
				const double A5	= 1.330274429;
				const double INVSQRT2PI	= 0.39894228040143268;

				double K = 1.0 / (1.0 + 0.2316419 * abs(x_std));

				double cnd = INVSQRT2PI * exp(- 0.5 * x_std * x_std) * (K * (A1 + K * (A2 + K * (A3 + K * (A4 + K * A5)))));

				if (x_std > 0)	cnd = 1.0 - cnd;
				return (TargetType)cnd;
			}
		};
	}

}

namespace GLOBALS
{
	Distributions::Implementations::Normal_Distribution_Implementation<NT> _Normal_Distribution;
	Distributions::Prototypes::Distribution<Distributions::Implementations::Normal_Distribution_Implementation<NT>>* const Normal_Distribution = (Distributions::Prototypes::Distribution<Distributions::Implementations::Normal_Distribution_Implementation<NT>>*)&_Normal_Distribution;
}
using namespace GLOBALS;
