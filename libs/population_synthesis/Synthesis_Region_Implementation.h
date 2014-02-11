#pragma once

#include "Synthesis_Region_Prototype.h"

namespace PopSyn
{

	namespace Implementations
	{
		implementation struct _Synthesis_Region_Implementation
		{
			// Tag as implementation
			//typedef typename Polaris_Component<MasterType,INHERIT(Synthesis_Region_Implementation_Simple),Execution_Object>::Component_Type ComponentType;

			typedef dense_hash_map<long long, Prototypes::Synthesis_Zone<typename MasterType::synthesis_zone_type>*> zone_map_type;
			m_container(zone_map_type, Synthesis_Zone_Collection, NONE, NONE);
			m_data(ostream*,Output_Stream, NONE, NONE);
			m_prototype(Null_Prototype<typename MasterType::population_synthesis_type>, parent_reference, NONE, NONE);

			typedef dense_hash_map<double, Household_Components::Prototypes::Household_Properties<typename MasterType::household_static_properties_type>*> __temp_sample_map_type;		
			m_container(__temp_sample_map_type, Temporary_Sample_Data, NONE, NONE); 

			template<typename TargetType> void Initialize()
			{
				this->_Synthesis_Zone_Collection.set_empty_key(-1);
				this->_Synthesis_Zone_Collection.set_deleted_key(-2);

				this->_Temporary_Sample_Data.set_empty_key(-1);
				this->_Temporary_Sample_Data.set_deleted_key(-2);
			}
			template<typename TargetType> TargetType scenario_reference()
			{
				return this->parent_reference<type_of(parent_reference)&>().scenario_reference<TargetType>();
			}
			tag_getter_as_available(scenario_reference);
			template<typename TargetType> TargetType network_reference()
			{
				return this->parent_reference<type_of(parent_reference)&>().network_reference<TargetType>();
			}
			tag_getter_as_available(network_reference);
		};

		implementation struct Synthesis_Region_Implementation_Simple : public Polaris_Component<MasterType,INHERIT(Synthesis_Region_Implementation_Simple),Execution_Object>, _Synthesis_Region_Implementation<MasterType>, _Synthesis_Zone_Implementation<MasterType>
		{
		};

		implementation struct Synthesis_Region_Implementation_Full : public Polaris_Component<MasterType,INHERIT(Synthesis_Region_Implementation_Full),Execution_Object>,  _Synthesis_Region_Implementation<MasterType>, _Synthesis_Zone_Implementation_With_Simulation<MasterType>
		{
		};
	}
}

