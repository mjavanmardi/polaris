#pragma once

#include "Household_Properties_Prototype.h"
#include "Person_Properties_Prototype.h"
#include "Person_Prototype.h"
#include "Movement_Plan_Prototype.h"
#include "Network_Skimming_Prototype.h"
#include "Activity_Prototype.h"


namespace Household_Components
{
	namespace Implementations
	{
		//==================================================================================
		/// Properties classes
		//----------------------------------------------------------------------------------
		implementation struct ADAPTS_Household_Properties_Implementation : public Polaris_Component<APPEND_CHILD(ADAPTS_Household_Properties_Implementation),MasterType,Data_Object,ParentType>
		{
			// Tag as Implementation
			typedef typename Polaris_Component<APPEND_CHILD(ADAPTS_Household_Properties_Implementation),MasterType,Data_Object>::Component_Type ComponentType;


			// local data members
			member_prototype(Prototypes::Household, Parent_Household, typename MasterType::household_type, none, none);
			member_data(int, home_location_id, none, none);
		
			// Methods
			feature_implementation void Initialize()
			{
				
			}
			tag_feature_as_available(Initialize);
			feature_implementation void Initialize(typename TargetType::ParamType home_synthesis_zone/*, requires(check(typename TargetType::ParamType, PopSyn::Concepts::Is_Synthesis_Zone) && check_as_given(typename TargetType::ParamType, is_pointer))*/)
			{	
				//===============================================================================================================
				// INITIALIZE HOME / WORK / SCHOOL LOCATIONS
				//---------------------------------------------------------------------------------------------------------------
				// get an interface to the given home zone;
				typedef PopSyn::Prototypes::Synthesis_Zone_Prototype<typename MasterType::zone> zone_itf;
				typedef Prototypes::Household_Properties<typename Parent_Household_interface::get_type_of(Static_Properties)> pop_unit_itf;
				
				// useful interfaces
				define_simple_container_interface(activity_location_ids_itf,typename zone_itf::get_type_of(Activity_Locations_Container),Containers::Random_Access_Sequence_Prototype,int,ComponentType);				
				define_component_interface(network_itf,typename Parent_Household_interface::get_type_of(network_reference),Network_Components::Prototypes::Network_Prototype,ComponentType);
				define_container_and_value_interface(activity_locations_itf, activity_location_itf,typename network_itf::get_type_of(activity_locations_container),Containers::Random_Access_Sequence_Prototype, Activity_Location_Components::Prototypes::Activity_Location_Prototype,ComponentType);
				define_container_and_value_interface(_Zone_Container_Interface, _Zone_Interface,typename network_itf::get_type_of(zones_container),Containers::Associative_Container_Prototype, Zone_Components::Prototypes::Zone_Prototype,ComponentType);
			
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
				pzone->template households<int&>()++;

			}	
			tag_feature_signature_as_available(Initialize,1);

		};


		implementation struct ACS_Household_Static_Properties_Implementation : public Polaris_Component<APPEND_CHILD(ACS_Household_Static_Properties_Implementation), MasterType, Data_Object, ParentType>
		{
			// Tag as Implementation
			typedef typename Polaris_Component<APPEND_CHILD(ACS_Household_Static_Properties_Implementation),MasterType,Data_Object>::Component_Type ComponentType;

			//=================================================================
			// Basic Person Characteristics Used in Popsyn algorithms
			member_data(double,ID,none,none);
			member_data(double,Weight,none,none);
			member_data(uint,Index,none,none);				 //index into the joint-distribution matrix of the region (convert using region.get_index())

			//=================================================================
			// Census specific household data, used in ABM routines
			member_data(Types::HHTYPE, Household_type,none,none);
			member_data(int, Household_size,check(ReturnValueType, is_integral),check(SetValueType,is_integral));
			member_data(int, Number_of_workers,check(ReturnValueType, is_integral),check(SetValueType,is_integral));
			member_data(int, Number_of_vehicles, check(ReturnValueType, is_integral),check(SetValueType,is_integral));		
			member_component_and_feature_accessor(Income, Value, Basic_Units::Prototypes::Currency_Prototype, Basic_Units::Implementations::Currency_Implementation<NT>);

			//=================================================================
			// Container which holds linked persons in ACS File for this household info unit
			member_container(vector<typename MasterType::person_static_properties_type*>, Persons_Container,none,none);

			// Characteristics setter
			feature_implementation void Characteristics(vector<double>* data)
			{
				// these setters correspond exactly to the ACS-PUMS definitions and layout as given in pums_file.txt.  if pumsfile changes change these functions
				typedef Prototypes::Household_Properties<ComponentType> this_itf;
				this_itf* pthis = (this_itf*)this;
						
				pthis->Household_type<Types::HHTYPE>((Types::HHTYPE)(int)(*data)[3]);
				pthis->Household_size<int>((*data)[4]);
				pthis->Number_of_vehicles<int>((*data)[8]);
				pthis->Number_of_workers<int>((*data)[25]);
				pthis->Income<Basic_Units::Currency_Variables::Dollars>((*data)[14]);

			}
			tag_feature_signature_as_available(Characteristics, 1);
		};

	}
}