#pragma once

#include "Household_Properties_Prototype.h"


namespace Household_Components
{
	namespace Implementations
	{
		//==================================================================================
		/// Properties classes
		//----------------------------------------------------------------------------------
		implementation struct ADAPTS_Household_Properties_Implementation : public Polaris_Component<MasterType,INHERIT(ADAPTS_Household_Properties_Implementation),Data_Object>
		{
			// Tag as Implementation
			typedef typename Polaris_Component<MasterType,INHERIT(ADAPTS_Household_Properties_Implementation),Data_Object>::Component_Type ComponentType;


			// local data members
			m_prototype(Prototypes::Household,typename MasterType::household_type, Parent_Household, NONE, NONE);
			m_data(int, home_location_id, NONE, NONE);
		
			// Methods
			template<typename TargetType> void Initialize()
			{
				
			}
			template<typename TargetType> void Initialize(TargetType home_synthesis_zone, requires(TargetType,check(strip_modifiers(TargetType), PopSyn::Concepts::Is_Synthesis_Zone) && check(typename TargetType, is_pointer)))
			{	
				//===============================================================================================================
				// INITIALIZE HOME / WORK / SCHOOL LOCATIONS
				//---------------------------------------------------------------------------------------------------------------
				// get an interface to the given home zone;
				typedef PopSyn::Prototypes::Synthesis_Zone<typename MasterType::synthesis_zone_type> zone_itf;
				typedef Prototypes::Household_Properties<typename type_of(Parent_Household)::get_type_of(Static_Properties)> pop_unit_itf;
				
				// useful interfaces
				typedef Random_Access_Sequence<typename zone_itf::get_type_of(Activity_Locations_Container),int> activity_location_ids_itf;
				typedef Network_Components::Prototypes::Network<typename type_of(Parent_Household)::get_type_of(network_reference)> network_itf;
				
				typedef Random_Access_Sequence<typename network_itf::get_type_of(activity_locations_container)> activity_locations_itf;
				typedef Activity_Location_Components::Prototypes::Activity_Location<typename get_component_type(activity_locations_itf)>  activity_location_itf;
				
				typedef Pair_Associative_Container<typename network_itf::get_type_of(zones_container)> _Zone_Container_Interface;
				typedef Zone_Components::Prototypes::Zone<typename get_mapped_component_type(_Zone_Container_Interface)>  _Zone_Interface;
				
			
				zone_itf* zone = (zone_itf*)home_synthesis_zone;
				network_itf* network = _Parent_Household->template network_reference<network_itf*>();
				activity_locations_itf* activity_locations = network->template activity_locations_container<activity_locations_itf*>();

				// initialize location indices
				_home_location_id = -1;

				
				// Available locations
				activity_location_ids_itf* loc_indices = zone->template Activity_Locations_Container<activity_location_ids_itf*>();

				// assign person to a random activity location in the zone				
				if (loc_indices->size() == 0)
				{
					_home_location_id= (int)((GLOBALS::Uniform_RNG.template Next_Rand<float>()*0.9999999) * activity_locations->size());
				}
				else
				{
					int index = -1;
					index = (int)((GLOBALS::Uniform_RNG.template Next_Rand<float>()*0.9999999) * loc_indices->size());
					_home_location_id = loc_indices->at(index);
				}

				// get the polaris zone of the synthesized person and increment its population counter;
				_Zone_Interface* pzone = _Parent_Household->template Home_Location<_Zone_Interface*>();
				pzone->template pop_households<int&>()++;

			}	
		};

	}
}
