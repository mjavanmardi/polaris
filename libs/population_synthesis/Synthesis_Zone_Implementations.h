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
			
			template<typename TargetType> void Initialize()
			{

			}
			template<typename TargetType> typename TargetType::ReturnType Get_1D_Index(typename TargetType::ParamType& multi_dimensional_index_boost::container::vector)
			{
				return this->_Target_Joint_Distribution.get_index(multi_dimensional_index_boost::container::vector);
			}

			template<typename TargetType> TargetType scenario_reference()
			{
				return this->parent_reference<type_of(parent_reference)&>().scenario_reference<type_of(parent_reference),TargetType>();
			}tag_getter_as_available(scenario_reference);
			template<typename TargetType> TargetType network_reference()
			{
				return this->parent_reference<type_of(parent_reference)&>().network_reference<type_of(parent_reference),TargetType>();
			}tag_getter_as_available(network_reference);

			m_data(long long,ID,check(strip_modifiers(TargetType),is_arithmetic),check(strip_modifiers(TargetType),is_arithmetic));
			m_container(m_array<double>,Target_Joint_Distribution, NONE,NONE);
			m_container(s_array<double>,Target_Marginal_Distribution, NONE, NONE);

			m_prototype(Null_Prototype<typename MasterType::IPF_Solver_Settings>,Solver_Settings, NONE, NONE);
			m_prototype(Null_Prototype<typename MasterType::RNG>, Rand, NONE, NONE);
			m_prototype(Null_Prototype<typename MasterType::region>, parent_reference, NONE, NONE);
	
			typedef unordered_multimap<uint, typename MasterType::household_static_properties_type*> __sample_map_type;		
			m_container(__sample_map_type, Sample_Data, NONE, NONE); 
			m_container(boost::container::vector<typename MasterType::household_type*>, Synthetic_Households_Container, NONE, NONE);
			m_container(boost::container::vector<int>, Activity_Locations_Container, NONE, NONE);
		};

		implementation struct Synthesis_Zone_Implementation : public Polaris_Component< MasterType,INHERIT(Synthesis_Zone_Implementation), Data_Object, ParentType>, _Synthesis_Zone_Implementation<MasterType>
		{
			// Tag as implementation
			typedef typename Polaris_Component<MasterType,INHERIT(Synthesis_Zone_Implementation),Data_Object>::Component_Type ComponentType;
		};

		implementation struct IPF_Solver_Settings_Implementation : public Polaris_Component< MasterType,INHERIT(IPF_Solver_Settings_Implementation), Data_Object, ParentType>
		{
			// Tag as implementation
			typedef typename Polaris_Component<MasterType,INHERIT(IPF_Solver_Settings_Implementation),Data_Object>::Component_Type ComponentType;

			template<typename TargetType> void Initialize(typename TargetType::ParamType tolerance, typename TargetType::ParamType percent_to_synthesize, typename TargetType::Param2Type iterations)
			{
				this->Tolerance< typename TargetType::ParamType>(tolerance);
				this->Percentage_to_synthesize< typename TargetType::ParamType>(percent_to_synthesize);
				this->Iterations< typename TargetType::Param2Type>(iterations);
			}
			m_data(double, Tolerance, check(strip_modifiers(TargetType),is_arithmetic),check(strip_modifiers(TargetType), is_arithmetic));
			m_data(int, Iterations, check(strip_modifiers(TargetType),is_arithmetic),check(strip_modifiers(TargetType), is_arithmetic));
			m_data(double, Percentage_to_synthesize, check(strip_modifiers(TargetType),is_arithmetic),check(strip_modifiers(TargetType), is_arithmetic));
		};
	}
}


