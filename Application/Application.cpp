
#include "Polaris_PCH.h"


concept struct Has_Type_Check
{
	check_typename_defined(check1,ReturnType);
	define_default_check(check1);
};

prototype struct Test1
{
	feature_prototype void Test_Func(requires(check(TargetType,Has_Type_Check))) {}
	feature_prototype void Test_Func(requires(!check(TargetType,Has_Type_Check))) {assert_check(TargetType, Has_Type_Check, "check typename defined fail");}
};

struct Tester_Struct {typedef double& ReturnType;};

int main()
{
	
	matrix<double> Sigma;
	matrix<double> lu;

	Sigma.resize(5,5,2.0);
	Sigma(0,0)=1;	Sigma(0,1)=0.099646;	Sigma(0,2)=-0.053507;	Sigma(0,3)=0.095299;	Sigma(0,4)=0.124233;
	Sigma(1,0)=0.099646;	Sigma(1,1)=1.000000095845;	Sigma(1,2)=-0.0337199637166;	Sigma(1,3)=0.305332422422;	Sigma(1,4)=0.168151167191;
	Sigma(2,0)=-0.053507;	Sigma(2,1)=-0.0337199637166;	Sigma(2,2)=1.00000035696104;	Sigma(2,3)=-0.1138867469362;	Sigma(2,4)=0.0255530422328;
	Sigma(3,0)=0.095299;	Sigma(3,1)=0.305332422422;	Sigma(3,2)=-0.1138867469362;	Sigma(3,3)=1.000001024481;	Sigma(3,4)=-0.099793723579;
	Sigma(4,0)=0.124233;	Sigma(4,1)=0.168151167191;	Sigma(4,2)=0.0255530422328;	Sigma(4,3)=-0.099793723579;	Sigma(4,4)=1.00000077957836;

	
	vector<double> corr;

	Normal_RNG.Next_Rand<float>();
	float i = -5.0;
	while (i <= 5.0)
	{
		GLOBALS::Normal_RNG.Correlated_Rands<double>(corr, Sigma);
		cout << endl;
		for (int j = 0; j < corr.size(); j++) cout << corr[j] << ", ";
		//cout << endl << i << ", " << GLOBALS::Normal_Distribution->Cumulative_Distribution_Value<float>(i,5,2) << ", " << GLOBALS::Normal_Distribution->Probability_Density_Value<float>(i,5,2);
		i+=0.001;
	}
	

	int test;

	cin >> test;
}