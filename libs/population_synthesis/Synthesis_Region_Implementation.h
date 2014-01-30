#pragma once

#include "Synthesis_Region_Prototype.h"

namespace PopSyn
{

	namespace Implementations
	{
		implementation struct Synthesis_Region_Implementation : public Polaris_Component<MasterType,INHERIT(Synthesis_Region_Implementation),Execution_Object>, _Synthesis_Zone_Implementation<MasterType>
		{
			// Tag as implementation
			typedef typename Polaris_Component<MasterType,INHERIT(Synthesis_Region_Implementation),Execution_Object>::Component_Type ComponentType;

			typedef dense_hash_map<long long, typename MasterType::zone*> zone_map_type;
			m_container(zone_map_type, Synthesis_Zone_Collection, NONE, NONE);
			m_data(ostream*,Output_Stream, NONE, NONE);
			m_prototype(Null_Prototype<typename MasterType::popsyn_solver>, parent_reference, NONE, NONE);

			typedef dense_hash_map<double, typename MasterType::household_static_properties_type*> __temp_sample_map_type;		
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
			}tag_getter_as_available(scenario_reference);
			template<typename TargetType> TargetType network_reference()
			{
				return this->parent_reference<type_of(parent_reference)&>().network_reference<TargetType>();
			}tag_getter_as_available(network_reference);
		};
	}
}

