#pragma once

#include "User_Space\User_Space_includes.h"
#include "User_Space\Population_Synthesis\Population_Unit_Implementations.h"

using namespace std;

//---------------------------------------------------------
//	SUMMARY FILE CLASS
//---------------------------------------------------------
namespace PopSyn
{
	namespace Concepts
	{
		concept struct Is_IPF_Capable
		{
			check_typename_defined(Has_Value_Type, Value_Type);
			check_typename_state(Has_Marginals, Has_Marginals_In_Distribution, true_type);

			define_default_check(Has_Marginals && Has_Value_Type);
		};
	}

	namespace Prototypes
	{
		prototype struct Synthesis_Zone_Prototype
		{
			tag_as_prototype;

			feature_prototype void Initialize()
			{
				this_component()->Initialize<ComponentType,CallerType,TargetType>();
			}

			feature_prototype void Fit_Joint_Distribution_To_Marginal_Data(requires(check(ComponentType,Concepts::Is_IPF_Capable)))
			{
				// IPF version of fitting the joint distribution to marginal distribution
				define_simple_container_interface(distribution, get_type_of(Target_Joint_Distribution),Multidimensional_Random_Access_Array_Prototype,int,NULLTYPE);

				distribution::iterator itr;
				distribution::size_type num_dim;
				distribution::index_type dimensions;

				// get the distribution
				distribution& mway = Target_Joint_Distribution<distribution&>();
				dimensions = mway.dimensions();
				num_dim = dimensions.size();

				// loop over each dimension, and each index within each dimensions and fit to the marginal
				
				
			}

			feature_prototype void Fit_Joint_Distribution_To_Marginal_Data(requires(check(ComponentType,!Concepts::Is_IPF_Capable)))
			{
				assert_check(ComponentType,Concepts::Is_IPF_Capable,"Not IPF Capable");
				assert_sub_check(ComponentType,Concepts::Is_IPF_Capable,Has_Joint_Distribution_Double,"doesn't have a double joint distribution");
				assert_sub_check(ComponentType,Concepts::Is_IPF_Capable,Has_Joint_Distribution_Float,"doesn't have a float joint distribution");
				assert_sub_check(ComponentType,Concepts::Is_IPF_Capable,Has_Value_Type,"doesn't have a value_type");
				assert_sub_check(ComponentType,Concepts::Is_IPF_Capable,Has_Marginals,"doesn't have marginals");
				
			}

			feature_accessor(Target_Joint_Distribution,none,none);

			feature_accessor(Sample_Data,none,none);

			feature_accessor(ID,none,none);

			feature_accessor(Marginal_Fit_Tolerance,check(ReturnValueType,is_arithmetic),check(SetValueType,is_arithmetic));

			feature_accessor(Maximum_Fit_Iterations,check(ReturnValueType,is_arithmetic),check(SetValueType,is_arithmetic));

			feature_accessor(Maximum_Selection_Iterations,check(ReturnValueType,is_arithmetic),check(SetValueType,is_arithmetic));
		};
	}



//class Zone : public m_array<double>
//{
//public:
//	// constructors
//	Zone (){}
//	Zone (double ID, vector<int> &dim_sizes);
//	Zone (const Zone& obj);
//
//	// Processing methods
//	void initialize(m_array Data);
//	bool IPF(double TOL, int MAX_ITER);
//	bool Select_HH(int MAX_ITER, const hash_map<uint,vector<Pop_Unit>> &sample, Prob_Generator& Rand);
//	void Add_Sample(const Pop_Unit& P);
//
//	// Property access methods
//	double& marginal(int dim, int index) {return _marginals[dim][index];} //marginals get/set
//	const double& Id(void){return _id;} // ID get
//
//	// I/O methods
//	bool write(void);
//	bool write(File_Writer &fw);
//	bool write_sample(File_Writer &fw);
//
//protected:
//	// Marginal totals across categories
//	vector<vector<double>> _marginals;
//	// List of households in zone
//	hash_map<uint, vector<Pop_Unit>> _sample;
//	// Unique zone ID
//	double _id;
//	
//
//private:
//
//};
//

}