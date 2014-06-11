#pragma once

#include "Synthesis_Zone_Prototypes.h"

namespace PopSyn
{

	namespace Implementations
	{
		//==========================================================================================
		// Base zone class for population synthesizer, this should be inherited but not modified
		//------------------------------------------------------------------------------------------
		implementation struct _Synthesis_Zone_Base_Implementation
		{			
			m_data(long long,ID,check(strip_modifiers(TargetType),is_arithmetic),check(strip_modifiers(TargetType),is_arithmetic));
			
			m_prototype(PopSyn::Prototypes::Solver_Settings,typename MasterType::solver_settings_type,Solver_Settings, NONE, NONE);

			m_prototype(PopSyn::Prototypes::Synthesis_Region,typename MasterType::synthesis_region_type, parent_reference, NONE, NONE);
	
			typedef boost::unordered_multimap<uint, Household_Components::Prototypes::Household_Properties<typename MasterType::household_static_properties_type>*> __sample_map_type;		
			m_container(__sample_map_type, Sample_Data, NONE, NONE); 

			template<typename TargetType> void Initialize()
			{
			}

			// pass through functions to access the Top-level class scenario/network
			template<typename TargetType> TargetType scenario_reference()
			{
				return this->parent_reference<type_of(parent_reference)&>().scenario_reference<TargetType>();
			}
			template<typename TargetType> TargetType network_reference()
			{
				return this->parent_reference<type_of(parent_reference)&>().network_reference<type_of(parent_reference),TargetType>();
			}	
		};


		//==========================================================================================
		// Recreate the three classes below for each population synthesis algorithm
		//------------------------------------------------------------------------------------------
		implementation struct _Synthesis_Zone_Polaris_Implementation : public _Synthesis_Zone_Base_Implementation<MasterType>
		{
			typedef true_type Has_Marginal_Distribution;
			typedef true_type Has_Joint_Distribution;
			typedef true_type Probabilistic_Selection_Type;
			typedef double Value_Type;

			// Containers for holding the joint and marginal distributions
			m_container(m_array<double>,Target_Joint_Distribution, NONE,NONE);
			m_container(s_array<double>,Target_Marginal_Distribution, NONE, NONE);
			m_container(m_array<double>,Target_Person_Joint_Distribution, NONE,NONE);
			m_container(s_array<double>,Target_Person_Marginal_Distribution, NONE, NONE);

			m_container(m_array<double>,Synthesized_Joint_Distribution, NONE,NONE);
			m_container(s_array<double>,Synthesized_Marginal_Distribution, NONE, NONE);
			m_container(m_array<double>,Synthesized_Person_Joint_Distribution, NONE,NONE);
			m_container(s_array<double>,Synthesized_Person_Marginal_Distribution, NONE, NONE);

			template<typename ContainerType> void Initialize(ContainerType dims_hh, ContainerType dims_per)
			{
				// Create the dimension vectors for the hh and person distributions from linker
				typename Target_Joint_Distribution_type::index_type dimensions_hh;
				typename Target_Person_Joint_Distribution_type::index_type dimensions_per;

				for (typename strip_modifiers(ContainerType)::iterator i = dims_hh.begin(); i != dims_hh.end(); ++i) 
				{
					dimensions_hh.push_back(*i);
				}
				for (typename strip_modifiers(ContainerType)::iterator i = dims_per.begin(); i != dims_per.end(); ++i) 
				{
					dimensions_per.push_back(*i);
				}

				//-----------------------------------------------------------------------------------------
				// Initialize the distribution and marginals
				this->_Target_Joint_Distribution.resize(dimensions_hh,0.0);
				this->_Target_Marginal_Distribution.resize(dimensions_hh,0.0);
				this->_Target_Person_Joint_Distribution.resize(dimensions_per,0.0);
				this->_Target_Person_Marginal_Distribution.resize(dimensions_per,0.0);
				this->_Synthesized_Joint_Distribution.resize(dimensions_hh,0.0);
				this->_Synthesized_Marginal_Distribution.resize(dimensions_hh,0.0);
				this->_Synthesized_Person_Joint_Distribution.resize(dimensions_per,0.0);
				this->_Synthesized_Person_Marginal_Distribution.resize(dimensions_per,0.0);
			}

			template<typename TargetType> void Add_Marginal_Data(File_IO::File_Reader& fr, TargetType linker)
			{
				typedef get_mapped_component_type(Sample_Data_type) sample_type;
				typedef Household_Components::Prototypes::Household_Properties<sample_type> pop_unit_itf;
				typedef Multidimensional_Random_Access_Array<Target_Marginal_Distribution_type> marginal_itf;
				marginal_itf* regional_marg, *regional_person_marg;
				if (_parent_reference != nullptr) regional_marg = _parent_reference->Target_Marginal_Distribution<marginal_itf*>();
				if (_parent_reference != nullptr) regional_person_marg = _parent_reference->Target_Person_Marginal_Distribution<marginal_itf*>();

				double x;

				Target_Marginal_Distribution_type::const_dimensional_type dimensions_hh = _Target_Marginal_Distribution.dimensions();
				for (typename Target_Marginal_Distribution_type::size_type i=0; i<dimensions_hh.size(); i++)
				{
					for (typename Target_Marginal_Distribution_type::size_type j=0; j<dimensions_hh[i]; j++)
					{
						if (!fr.Get_Data<double>(x,linker->get_sf3_column(i,j))) break;
						_Target_Marginal_Distribution[pair<typename marginal_itf::size_type,typename marginal_itf::size_type>(i,j)] = x;
						if (_parent_reference != nullptr) (*regional_marg)[pair<typename marginal_itf::size_type,typename marginal_itf::size_type>(i,j)] += x;
					}

				}
				Target_Person_Marginal_Distribution_type::const_dimensional_type dimensions_per = _Target_Person_Marginal_Distribution.dimensions();
				for (typename Target_Person_Marginal_Distribution_type::size_type i=0; i<dimensions_per.size(); i++)
				{
					for (typename Target_Person_Marginal_Distribution_type::size_type j=0; j<dimensions_per[i]; j++)
					{
						if (!fr.Get_Data<double>(x,linker->get_sf3_column(i,j,false))) break;
						_Target_Person_Marginal_Distribution[pair<typename marginal_itf::size_type,typename marginal_itf::size_type>(i,j)] = x;
						if (_parent_reference != nullptr) (*regional_person_marg)[pair<typename marginal_itf::size_type,typename marginal_itf::size_type>(i,j)] += x;
					}

				}
			}

			template<typename InterfaceType> void Add_To_Synthetic_Distributions(InterfaceType unit, int count, requires(InterfaceType,check_2(strip_modifiers(InterfaceType)::Component_Type,typename MasterType::household_static_properties_type,is_same)))
			{
				// Track the updates to the person distributions
				_Synthesized_Joint_Distribution[unit->Index<int>()]+=count;
				Synthesized_Joint_Distribution_type::index_type index = _Synthesized_Joint_Distribution.get_index(unit->Index<int>());
				for (int i=0; i< index.size(); i++)
				{
					_Synthesized_Marginal_Distribution(i,index[i])+=count;	
				}
			}
			template<typename InterfaceType> void Add_To_Synthetic_Distributions(InterfaceType unit, int count, requires(InterfaceType,check_2(strip_modifiers(InterfaceType)::Component_Type,typename MasterType::person_static_properties_type,is_same)))
			{
				// Track the updates to the person distributions
				_Synthesized_Person_Joint_Distribution[unit->Index<int>()]+=count;
				Synthesized_Person_Joint_Distribution_type::index_type index = _Synthesized_Person_Joint_Distribution.get_index(unit->Index<int>());
				for (int i=0; i< index.size(); i++)
				{
					_Synthesized_Person_Marginal_Distribution(i,index[i])+=count;	
				}
			}
			template<typename InterfaceType> void Add_To_Synthetic_Distributions(InterfaceType unit, int count, 
				requires(InterfaceType,!check_2(strip_modifiers(InterfaceType)::Component_Type,typename MasterType::person_static_properties_type,is_same) && !check_2(type_of(strip_modifiers(InterfaceType)),typename MasterType::household_static_properties_type,is_same)))
			{
				assert_check_2(strip_modifiers(InterfaceType)::Component_Type,typename MasterType::person_static_properties_type,is_same,"The object type is not the same as defined in MasterType::person_static_properties_type.");
				assert_check_2(strip_modifiers(InterfaceType)::Component_Type,typename MasterType::household_static_properties_type,is_same,"The object type is not the same as defined in MasterType::household_static_properties_type.");
				
			}

		};
		implementation struct Synthesis_Zone_Implementation_Simple : public Polaris_Component< MasterType,INHERIT(Synthesis_Zone_Implementation_Simple), Data_Object>, _Synthesis_Zone_Polaris_Implementation<MasterType>
		{
			// Tag as implementation
			typedef typename Polaris_Component<MasterType,INHERIT(Synthesis_Zone_Implementation_Simple),Data_Object>::Component_Type ComponentType;		

			// Static versions of the agent containers
			m_container(std::vector<Household_Components::Prototypes::Household_Properties<typename MasterType::household_static_properties_type>*>, Synthetic_Households_Container, NONE, NONE);
			m_container(std::vector<Person_Components::Prototypes::Person_Properties<typename MasterType::person_static_properties_type>*>, Synthetic_Persons_Container, NONE, NONE);

		};
		implementation struct Synthesis_Zone_Implementation_Full : public Synthesis_Zone_Implementation_Simple<MasterType,INHERIT(Synthesis_Zone_Implementation_Full)>
		{
			// Tag as implementation
			typedef typename Synthesis_Zone_Implementation_Simple<MasterType,INHERIT(Synthesis_Zone_Implementation_Full)>::Component_Type ComponentType;

			// Full versions of the agent containers
			m_container(std::vector<Household_Components::Prototypes::Household<typename MasterType::household_type>*>, Synthetic_Households_Container, NONE, NONE);
			m_container(std::vector<int>, Activity_Locations_Container, NONE, NONE);
		};
	}
}



