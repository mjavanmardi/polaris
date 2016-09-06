#pragma once

#include "Household_Properties_Prototype.h"
#include "Population_Synthesis_Concepts.h"


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
			template<typename TargetType> void Initialize(TargetType home_synthesis_zone, requires(TargetType,check(strip_modifiers(TargetType), PopSyn::Concepts::Is_Synthesis_Zone) && check(TargetType, is_pointer)))
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
				typedef Activity_Location_Components::Prototypes::Activity_Location<get_component_type(activity_locations_itf)>  activity_location_itf;
				
				typedef Pair_Associative_Container<typename network_itf::get_type_of(zones_container)> _Zone_Container_Interface;
				typedef Zone_Components::Prototypes::Zone<get_mapped_component_type(_Zone_Container_Interface)>  _Zone_Interface;
				
			
				zone_itf* zone = (zone_itf*)home_synthesis_zone;
				network_itf* network = _Parent_Household->template network_reference<network_itf*>();
				activity_locations_itf* activity_locations = network->template activity_locations_container<activity_locations_itf*>();

				// initialize location indices
				_home_location_id = -1;

				// Available locations
				activity_location_ids_itf* loc_indices = zone->template Activity_Locations_Container<activity_location_ids_itf*>();

				// if no activity locations exist in the current synthesis zone, distribute randomly throughout the region
				if (loc_indices->size() == 0)
				{
					_home_location_id = (int)((GLOBALS::Uniform_RNG.template Next_Rand<float>()*0.9999999) * activity_locations->size());
					return;
				}

				// Do probabilistic selection of final location, weighted by required population size of TAZ of each location (trying to match required TAZ totals)
				// get total population weight
				float pop_weight = 0;
				for (auto l = loc_indices->begin(); l != loc_indices->end(); ++l)
				{
					activity_location_itf* a = (activity_location_itf*)activity_locations->at(*l);
					_Zone_Interface* z = a->zone<_Zone_Interface*>();
					pop_weight += z->pop_households<float>();

				}
				// if none of the tazs for the current zones requires any population print error message and distribute randomly
				if (pop_weight == 0)
				{
					cout << "Warning: none of the TAZs corresponding to synthesis zone " << zone->ID<int>() << " require any population. Check zone and popsyn input values." << endl;
					_home_location_id = (int)((GLOBALS::Uniform_RNG.template Next_Rand<float>()*0.9999999) * activity_locations->size());
					return;
				}
				float p_cum = 0;
				float r = GLOBALS::Uniform_RNG.template Next_Rand<float>();
				for (auto l = loc_indices->begin(); l != loc_indices->end(); ++l)
				{
					activity_location_itf* a = (activity_location_itf*)activity_locations->at(*l);
					_Zone_Interface* z = a->zone<_Zone_Interface*>();
					p_cum += z->pop_households<float>() / pop_weight;

					if (r < p_cum)
					{
						_home_location_id = *l;
						return;
					}

				}

				// if nothing else, assign randomly
				_home_location_id = (int)((GLOBALS::Uniform_RNG.template Next_Rand<float>()*0.9999999) * activity_locations->size());
				return;


				//// assign person to a random activity location in the zone				
				//if (loc_indices->size() == 0)
				//{
				//	_home_location_id= (int)((GLOBALS::Uniform_RNG.template Next_Rand<float>()*0.9999999) * activity_locations->size());
				//}
				//else
				//{
				//	int index = -1;
				//	index = (int)((GLOBALS::Uniform_RNG.template Next_Rand<float>()*0.9999999) * loc_indices->size());
				//	_home_location_id = loc_indices->at(index);
				//}

				// get the polaris zone of the synthesized person and increment its population counter;
				//_Zone_Interface* pzone = _Parent_Household->template Home_Location<_Zone_Interface*>();
				//pzone->template pop_households<int&>()++;

			}	
		};

	}
}
