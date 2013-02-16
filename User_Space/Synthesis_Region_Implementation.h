#pragma once

#include "Synthesis_Region_Prototype.h"

namespace PopSyn
{

	namespace Implementations
	{
		implementation struct Synthesis_Region_Implementation : public Polaris_Component<APPEND_CHILD(Synthesis_Region_Implementation),MasterType,Execution_Object,ParentType>, _Synthesis_Zone_Implementation<MasterType,ParentType>
		{
			typedef dense_hash_map<long long, typename MasterType::zone*> zone_map_type;
			member_associative_container(zone_map_type, Synthesis_Zone_Collection,none,none);
			member_pointer(ostream,Output_Stream,none,none);
			member_component(typename MasterType::popsyn_solver, parent_reference, none,none);

			feature_implementation void Initialize()
			{
				this->_Synthesis_Zone_Collection.set_empty_key(-1);
				this->_Synthesis_Zone_Collection.set_deleted_key(-2);
			}
			feature_implementation TargetType scenario_reference()
			{
				return this->parent_reference<CallerType,type_of(parent_reference)&>().scenario_reference<CallerType,TargetType>();
			}tag_getter_as_available(scenario_reference);
			feature_implementation TargetType network_reference()
			{
				return this->parent_reference<CallerType,type_of(parent_reference)&>().network_reference<CallerType,TargetType>();
			}tag_getter_as_available(network_reference);
		};
	}
}
