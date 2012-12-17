#pragma once

#include "User_Space\Population_Synthesis\Synthesis_Zone_Prototypes.h"

namespace PopSyn
{

	namespace Implementations
	{
		implementation struct Synthesis_Zone_Implementation
		{
			typedef true_type Has_Marginals_In_Distribution;
			typedef true_type Probabilistic_Selection_Type;
			typedef double Value_Type;

			member_data(ulong,ID,check(ReturnValueType,is_arithmetic),check(SetValueType,is_arithmetic));
			member_container(m_array<double>,Target_Joint_Distribution,none,not_available);
			member_container(s_array<double>,Target_Marginal_Distribution, none, not_available);
			//member_container(vector<typename MasterType::pop_unit*>, Sample_Data,none,none);

			member_component(typename MasterType::IPF_Solver_Settings,Solver_Settings,none,none);
			member_component(typename MasterType::RNG, Rand,none,none);
	
			typedef hash_multimap<uint, typename MasterType::pop_unit*> __sample_map_type;		
			member_associative_container(__sample_map_type, Sample_Data, none, none); 
		};

		implementation struct IPF_Solver_Settings_Implementation
		{
			feature_implementation void initialize(typename TargetType::Param1Type tolerance, typename TargetType::Param2Type iterations)
			{
				this->Tolerance<ComponentType, CallerType, TargetType::Param1Type>(tolerance);
				this->Iterations<ComponentType, CallerType, TargetType::Param2Type>(iterations);
			}
			member_data(double, Tolerance, check(ReturnValueType,is_arithmetic),check(SetValueType, is_arithmetic));
			member_data(int, Iterations, check(ReturnValueType,is_arithmetic),check(SetValueType, is_arithmetic));
		};
	}
}


//Zone::Zone(double ID, vector<int> &dim_sizes) : m_array(dim_sizes, 0)
//{
//	_id = ID;
//
//	// initialize marginal data structure
//	for (int i=0; i<dim_sizes.size(); i++)
//	{
//		vector<double> marg;
//		for (int j=0; j<dim_sizes[i]; j++)
//		{
//			marg.push_back(0.0);
//		}
//		_marginals.push_back(marg);
//	}
//}
//
//Zone::Zone(const Zone& obj) : m_array(obj)
//{
//	_id = obj._id;
//
//	// initialize marginal data structure
//	for (int i=0; i<obj._dim_sizes.size(); i++)
//	{
//		vector<double> marg;
//		for (int j=0; j<obj._dim_sizes[i]; j++)
//		{
//			marg.push_back(obj._marginals[i][j]);
//		}
//		_marginals.push_back(marg);
//	}
//
//	_sample.clear();
//	for (hash_map<uint,vector<Pop_Unit>>::const_iterator p = obj._sample.begin(); p!= obj._sample.end(); p++)
//	{
//		vector<Pop_Unit> v;
//		for (vector<Pop_Unit>::const_iterator i=p->second.begin(); i!=p->second.end(); i++)
//		{
//			v.push_back(*i);
//		}
//		_sample.insert(pair<uint,vector<Pop_Unit>>(p->first,v));
//	}
//}
//
//bool Zone::write(void)
//{
//	cout<<"Zone: "<<fixed<<_id<<endl;
//	for (int d=0; d<_ndim; d++)
//	{
//		cout<<"Marginal "<<d<<":\t";
//		for (int i=0; i<_marginals[d].size(); i++) cout<<_marginals[d][i]<<"\t";
//		cout<<endl;
//	}
//	cout<<endl;
//
//	print();
//	return true;
//}
//
//bool Zone::write(File_Writer &fw)
//{
//	stringstream ss;
//	ss <<"Zone: "<<fixed<<_id;
//	fw.Write_Line(ss);
//
//	for (int d=0; d<_ndim; d++)
//	{
//		ss<<"Marginal "<<d<<":\t";
//		for (int i=0; i<_marginals[d].size(); i++) ss<<_marginals[d][i]<<"\t";
//		fw.Write_Line(ss);
//	}
//	fw.Write_Line();
//
//	print(fw);
//	return true;
//}
//
//bool Zone::write_sample(File_Writer &fw)
//{
//	stringstream ss;
//
//	hash_map<uint,vector<Pop_Unit>>::iterator itr;
//	vector<Pop_Unit>::iterator p_itr;
//
//	for (itr = _sample.begin(); itr!=_sample.end(); itr++)
//	{
//		for (p_itr = itr->second.begin(); p_itr!=itr->second.end(); p_itr++) 
//		{
//			ss <<fixed<<_id<<fw.DELIM;
//			p_itr->write(ss,fw.DELIM);
//			fw.Write_Line(ss);
//		}
//	}
//
//
//	return true;
//}
//
//void Zone::Add_Sample(const Pop_Unit& Pop)
//{
//	Pop_Unit P = Pop_Unit(Pop);
//
//	hash_map<uint, vector<Pop_Unit>>::iterator i;
//	if ((i = _sample.find(P.Index())) != _sample.end())
//	{
//		i->second.push_back(P);
//	}
//	else
//	{
//		vector<Pop_Unit> v;
//		v.push_back(P);
//		_sample.insert(pair<uint,vector<Pop_Unit>>(P.Index(),v));
//	}
//}
//
//bool Zone::IPF(double TOL, int MAX_ITER)
//{
//	bool cont = true;
//	int iter = 0;
//	double max_error = 0.0;
//
//	while (iter<MAX_ITER)
//	{
//		// cycle over each dimension
//		for (int d=0; d<_ndim; d++)
//		{
//
//			// fit to each marginal value in dimension
//			for (int i=0; i<_dim_sizes[d]; i++)
//			{
//				double sum=0.0;
//
//				// get current sum of joint distribution for marginal
//				while (iterate(d,i))
//				{
//					sum += getCurrentCell();
//				}
//
//				double err;
//				if (sum > 0.0) err = marginal(d,i)/sum;
//				else err = 0.0;
//				if (abs(err-1.0) > max_error) max_error = abs(err-1.0);
//
//				// get current sum of joint distribution for marginal
//				while (iterate(d,i))
//				{
//					getCurrentCell()*=err;
//				}
//			}
//		}
//
//		if (max_error < TOL) return true;
//		iter++;
//	}
//	return true;
//}
//
//bool Zone::Select_HH(int MAX_ITER, const hash_map<uint, vector<Pop_Unit>>& sample, Prob_Generator& Rand)
//{
//	bool cont = true;
//
//	hash_map<uint,vector<Pop_Unit>>::const_iterator itr;
//
//	// Loop through all non-zero cells
//	for (itr = sample.begin(); itr != sample.end(); itr++)
//	{
//		double cur_weight=0;
//		double n_req = _data[itr->first];
//
//		// check each household in sample.  Make n_req attempts to add household with prob=hh weight
//		vector<Pop_Unit>::const_iterator p_itr;
//		for (p_itr = itr->second.begin(); p_itr != itr->second.end(); p_itr++)
//		{
//			for (double attempts = n_req; attempts > 0; attempts--)
//			{
//				double p_adj = 1;
//				if (n_req<1) p_adj = n_req;
//				cur_weight += p_itr->Weight();
//
//				// check generation probability against weight of sample element amongst remaining unchecked elements
//				// if p<prob, add household to sample, decrement num_required
//				double p = Rand.Get_Probability();
//				if (p*p_adj < cur_weight)
//				{
//					n_req--;
//					Add_Sample(*p_itr);
//				}
//			}
//			
//			if (n_req < 0) break;
//		}
//	}
//
//	cout<<"Zone "<<fixed<<_id<<" is done."<<endl;
//	return true;
//}
//
//
