#pragma once

#include "Synthesis_Zone_Prototypes.h"

namespace PopSyn
{

	namespace Implementations
	{
		implementation struct _Synthesis_Zone_Implementation
		{
			typedef true_type Has_Marginals_In_Distribution;
			typedef true_type Probabilistic_Selection_Type;
			typedef double Value_Type;
			
			feature_implementation void Initialize()
			{

			}
			feature_implementation typename TargetType::ReturnType Get_1D_Index(typename TargetType::ParamType& multi_dimensional_index_vector)
			{
				return this->_Target_Joint_Distribution.get_index(multi_dimensional_index_vector);
			}

			feature_implementation TargetType scenario_reference()
			{
				return this->parent_reference<ComponentType,CallerType,type_of(parent_reference)&>().scenario_reference<type_of(parent_reference),CallerType,TargetType>();
			}tag_getter_as_available(scenario_reference);
			feature_implementation TargetType network_reference()
			{
				return this->parent_reference<ComponentType,CallerType,type_of(parent_reference)&>().network_reference<type_of(parent_reference),CallerType,TargetType>();
			}tag_getter_as_available(network_reference);

			member_data(long long,ID,check(ReturnValueType,is_arithmetic),check(SetValueType,is_arithmetic));
			member_container(m_array<double>,Target_Joint_Distribution,none,not_available);
			member_container(s_array<double>,Target_Marginal_Distribution, none, not_available);

			member_component(typename MasterType::IPF_Solver_Settings,Solver_Settings,none,none);
			member_component(typename MasterType::RNG, Rand,none,none);
			member_component(typename MasterType::region, parent_reference, none,none);
	
			typedef hash_multimap<uint, typename MasterType::person_static_properties_type*> __sample_map_type;		
			member_associative_container(__sample_map_type, Sample_Data, none, none); 
			member_container(vector<typename MasterType::person_type*>, Synthetic_Persons_Container,none,none);
			member_container(vector<int>, Activity_Locations_Container,none,none);
		};

		implementation struct Synthesis_Zone_Implementation : public Polaris_Component<APPEND_CHILD(Synthesis_Zone_Implementation), MasterType, Data_Object, ParentType>, _Synthesis_Zone_Implementation<MasterType,ParentType>
		{
			// Tag as implementation
			typedef typename Polaris_Component<APPEND_CHILD(Synthesis_Zone_Implementation),MasterType,Data_Object>::Component_Type ComponentType;
		};

		implementation struct IPF_Solver_Settings_Implementation : public Polaris_Component<APPEND_CHILD(IPF_Solver_Settings_Implementation), MasterType, Data_Object, ParentType>
		{
			// Tag as implementation
			typedef typename Polaris_Component<APPEND_CHILD(IPF_Solver_Settings_Implementation),MasterType,Data_Object>::Component_Type ComponentType;

			feature_implementation void Initialize(typename TargetType::ParamType tolerance, typename TargetType::ParamType percent_to_synthesize, typename TargetType::Param2Type iterations)
			{
				this->Tolerance<ComponentType,CallerType, typename TargetType::ParamType>(tolerance);
				this->Percentage_to_synthesize<ComponentType,CallerType, typename TargetType::ParamType>(percent_to_synthesize);
				this->Iterations<ComponentType,CallerType, typename TargetType::Param2Type>(iterations);
			}
			member_data(double, Tolerance, check(ReturnValueType,is_arithmetic),check(SetValueType, is_arithmetic));
			member_data(int, Iterations, check(ReturnValueType,is_arithmetic),check(SetValueType, is_arithmetic));
			member_data(double, Percentage_to_synthesize, check(ReturnValueType,is_arithmetic),check(SetValueType, is_arithmetic));
		};
	}
}


