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
			template<typename ParamType, typename ReturnType> ReturnType Get_1D_Index(ParamType& multi_dimensional_index_vector)
			{
				return this->_Target_Joint_Distribution.get_index(multi_dimensional_index_vector);
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

			m_prototype(Null_Prototype<typename MasterType::ipf_solver_settings_type>,Solver_Settings, NONE, NONE);
			m_prototype(Null_Prototype<typename MasterType::rng_type>, Rand, NONE, NONE);
			m_prototype(Null_Prototype<typename MasterType::synthesis_region_type>, parent_reference, NONE, NONE);
	
			typedef boost::unordered_multimap<uint, Household_Components::Prototypes::Household_Properties<typename MasterType::household_static_properties_type>*> __sample_map_type;		
			m_container(__sample_map_type, Sample_Data, NONE, NONE); 

			
		};

		implementation struct Synthesis_Zone_Implementation_Simple : public Polaris_Component< MasterType,INHERIT(Synthesis_Zone_Implementation_Simple), Data_Object>, _Synthesis_Zone_Implementation<MasterType>
		{
			// Tag as implementation
			typedef typename Polaris_Component<MasterType,INHERIT(Synthesis_Zone_Implementation_Simple),Data_Object>::Component_Type ComponentType;

			// Static versions of the agent containers
			m_container(boost::container::vector<Household_Components::Prototypes::Household_Properties<typename MasterType::household_static_properties_type>*>, Synthetic_Households_Container, NONE, NONE);
			m_container(boost::container::vector<Person_Components::Prototypes::Person_Properties<typename MasterType::person_static_properties_type>*>, Synthetic_Persons_Container, NONE, NONE);
		};

		implementation struct Synthesis_Zone_Implementation_Full : public Polaris_Component< MasterType,INHERIT(Synthesis_Zone_Implementation_Full), Data_Object>, _Synthesis_Zone_Implementation<MasterType>
		{
			// Tag as implementation
			typedef typename Polaris_Component<MasterType,INHERIT(Synthesis_Zone_Implementation_Full),Data_Object>::Component_Type ComponentType;

			// Full versions of the agent containers
			m_container(boost::container::vector<Household_Components::Prototypes::Household_Properties<typename MasterType::household_type>*>, Synthetic_Households_Container, NONE, NONE);
			m_container(boost::container::vector<int>, Activity_Locations_Container, NONE, NONE);
		};



		implementation struct IPF_Solver_Settings_Implementation : public Polaris_Component< MasterType,INHERIT(IPF_Solver_Settings_Implementation), Data_Object>
		{
			// Tag as implementation
			typedef typename Polaris_Component<MasterType,INHERIT(IPF_Solver_Settings_Implementation),Data_Object>::Component_Type ComponentType;

			template<typename ParamType> void Initialize(ParamType tolerance, ParamType percent_to_synthesize, ParamType iterations)
			{
				this->Tolerance< ParamType>(tolerance);
				this->Percentage_to_synthesize< ParamType>(percent_to_synthesize);
				this->Iterations< ParamType>(iterations);
			}
			m_data(double, Tolerance, check(strip_modifiers(TargetType),is_arithmetic),check(strip_modifiers(TargetType), is_arithmetic));
			m_data(int, Iterations, check(strip_modifiers(TargetType),is_arithmetic),check(strip_modifiers(TargetType), is_arithmetic));
			m_data(double, Percentage_to_synthesize, check(strip_modifiers(TargetType),is_arithmetic),check(strip_modifiers(TargetType), is_arithmetic));
		};
	}
}



