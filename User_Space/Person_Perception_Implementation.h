#pragma once

#include "Person_Perception_Prototype.h"


namespace Person_Components
{
	namespace Implementations
	{
		//==================================================================================
		/// Planning classes
		//----------------------------------------------------------------------------------
		implementation struct General_Person_Perception_Implementation : public Polaris_Component<APPEND_CHILD(General_Person_Perception_Implementation),MasterType,Data_Object,ParentType>
		{
			// Pointer to the Parent class
			member_prototype(Person_Components::Prototypes::Person, Parent_Person, typename MasterType::person_type, none, none);

			member_prototype(Network_Components::Prototypes::Network_Prototype, Network, typename MasterType::network_type, none, none);
			member_prototype(Scenario_Components::Prototypes::Scenario_Prototype, Scenario, typename MasterType::scenario_type, none, none);

			// Interface definitions
			define_component_interface(_Skim_Interface, typename Network_interface::get_type_of(skimming_faculty),Network_Skimming_Components::Prototypes::Network_Skimming_Prototype,ComponentType);
			define_container_and_value_interface(_Activity_Locations_Container_Interface, _Activity_Location_Interface, typename Network_interface::get_type_of(activity_locations_container), Random_Access_Sequence_Prototype, Activity_Location_Components::Prototypes::Activity_Location_Prototype, ComponentType);
			define_container_and_value_interface(_Links_Container_Interface, _Link_Interface, typename _Activity_Location_Interface::get_type_of(origin_links), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
			define_container_and_value_interface(_Zones_Container_Interface, _Zone_Interface, typename Network_interface::get_type_of(zones_container), Associative_Container_Prototype, Zone_Components::Prototypes::Zone_Prototype, ComponentType);


			feature_implementation void Initialize()
			{

			}	tag_feature_as_available(Initialize);
			feature_implementation void Check_Network_State()
			{

			}	tag_feature_as_available(Check_Network_State);
		};

	}

}