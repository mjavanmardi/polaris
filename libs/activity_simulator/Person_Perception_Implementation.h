#pragma once

#include "Person_Perception_Prototype.h"


namespace Person_Components
{
	namespace Implementations
	{
		//==================================================================================
		/// Planning classes
		//----------------------------------------------------------------------------------
		implementation struct General_Person_Perception_Implementation : public Polaris_Component<MasterType,INHERIT(General_Person_Perception_Implementation),Data_Object>
		{
			// Tag as implementation
			typedef typename Polaris_Component<MasterType,INHERIT(General_Person_Perception_Implementation),Data_Object>::Component_Type ComponentType;

			// Pointer to the Parent class
			m_prototype(Prototypes::Person, typename MasterType::person_type, Parent_Person, NONE, NONE);

			// Physical world elements
			m_prototype(Network_Components::Prototypes::Network, typename MasterType::network_type, Network, NONE, NONE);
			m_prototype(Scenario_Components::Prototypes::Scenario, typename MasterType::scenario_type, Scenario, NONE, NONE);

			// Interface definitions
			typedef Network_Components::Prototypes::Network< type_of(Network)> _Network_Interface;
			typedef Network_Skimming_Components::Prototypes::Network_Skimming< typename _Network_Interface::get_type_of(skimming_faculty)> _Skim_Interface;
			
			typedef Random_Access_Sequence< typename _Network_Interface::get_type_of(activity_locations_container)> _Activity_Locations_Container_Interface;
			typedef Activity_Location_Components::Prototypes::Activity_Location<get_component_type(_Activity_Locations_Container_Interface)>  _Activity_Location_Interface;
			
			typedef Random_Access_Sequence< typename _Activity_Location_Interface::get_type_of(origin_links)> _Links_Container_Interface;
			typedef Link_Components::Prototypes::Link<get_component_type(_Links_Container_Interface)>  _Link_Interface;
			
			typedef Pair_Associative_Container< typename _Network_Interface::get_type_of(zones_container)> _Zones_Container_Interface;
			typedef Zone_Components::Prototypes::Zone<get_mapped_component_type(_Zones_Container_Interface)>  _Zone_Interface;
			
			template<typename TargetType> void Initialize()
			{

			}
			template<typename TargetType> void Check_Network_State()
			{

			}
		};

	}

}
