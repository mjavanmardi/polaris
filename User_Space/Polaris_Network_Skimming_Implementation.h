#pragma once

#include "Network_Skimming_Prototype.h"
#include "Polaris_Network_Implementation.h"
#include "Polaris_Zone_Implementation.h"

namespace Network_Skimming_Components
{
	namespace Types
	{

	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{
		//======================================================================================
		/// Common elements to all network skimming components
		//--------------------------------------------------------------------------------------
		implementation struct _Basic_Network_Skimming_Elements
		{
			// reference to the transportation network
			member_component(typename MasterType::network_type, network_reference, none, none);
			// time increment at which skim tables are updated - set in the initializer
			member_component(Basic_Units::Implementations::Time_Implementation<MasterType>,_update_increment,check(ReturnValueType,Basic_Units::Concepts::Is_Time_Prototype),check(SetValueType,Basic_Units::Concepts::Is_Time_Prototype));
			member_component_feature(_update_increment,update_increment,check(ReturnValueType,Basic_Units::Concepts::Is_Time_Value),check(SetValueType,Basic_Units::Concepts::Is_Time_Value));
		};


		//======================================================================================
		/// Skim table which holds the values for LOS
		//--------------------------------------------------------------------------------------
		implementation struct Skim_Table_Implementation : public Polaris_Component_Class<Skim_Table_Implementation,MasterType,Execution_Object,ParentType>
		{
			member_container(concat(hash_map<long,m_array<double>>), skim_tables_container,none,none);

			// links back to the network to be skimmed
			member_component(typename MasterType::network_type, network_reference, none, none);

			//=============================================
			// Primary function accessors - used to calculate/return LOS values for OD pairs
			//---------------------------------------------
			feature_implementation void Initialize(TargetType network_ref, requires(check_as_given(TargetType, is_pointer) && check(TargetType, Is_Polaris_Prototype)))
			{
				// set the network reference
				define_component_interface(network_itf,type_of(network_reference),Network_Components::Prototypes::Network_Prototype,ComponentType);
				this->network_reference<network_itf*>(network_ref);
				network_itf* network = (network_itf*)network_ref;

				// create the LOS table
				define_container_and_value_interface(zones_itf,zone_itf,get_type_of(network_itf::zones_container),Random_Access_Sequence_Prototype,Zone_Components::Prototypes::Zone_Prototype,ComponentType);
				zones_itf* zones_container = network->zones_container<zones_itf*>();
				zones_itf::iterator zone_itr = zones_container.begin();

				m_array<double> vehicle_los = m_array<double>(zones_container.size(), 0);
				for (;zone_itr != zones_container.end(); ++zones_itr)
				{
					zone_itf zone = *zone_itr;
					zone->internal_id<long>
				}


			}
			//feature_prototype bool Update_LOS()
			//{
			//	// For each Mode :
			//		// For each Origin :
			//			// For each Destination :
			//				// Do routing for Origin to Destination
			//				// Get travel time for route
			//				// Store travel time in Skim_Table [Origin.Index , Destination.Index]
			//}
			//feature_prototype typename TargetType::ReturnType Get_LOS(TargetType::ParamType Origin_ID, TargetType::ParamType Destination_ID, TargetType::Param2Type Mode_Indicator);
		};


		//======================================================================================
		/// Basic skimming implementation with only the current LOS being stored
		//--------------------------------------------------------------------------------------
		implementation struct Basic_Network_Skimming_Implementation : public _Basic_Network_Skimming_Elements<MasterType>, public Polaris_Component_Class<Basic_Network_Skimming_Implementation,MasterType,Execution_Object,ParentType>
		{
			member_component(typename MasterType::network_type, network_reference, none, none);
			member_component(Skim_Table_Implementation<MasterType>, current_skim_table, none, none);
			feature_implementation typename TargetType::ReturnType Get_Current_LOS(typename TargetType::ParamType Origin_ID, typename TargetType::ParamType Destination_ID, typename TargetType::Param2Type Mode_Indicator)
			{
				define_component_interface(_skim_interface,type_of(current_skim_table),Prototypes::Skim_Table_Prototype,ComponentType);
				_skim_interface* skim = this->current_skim_table<ComponentType,CallerType,_skim_interface*>();
				return skim->Get_LOS<TargetType>(Origin_ID, Destination_ID, Mode_Indicator);
			}
		
			feature_implementation void Initialize(TargetType network, requires(check_as_given(TargetType, is_pointer) && check(TargetType, Is_Polaris_Prototype)))
			{
				define_component_interface(network_itf,type_of(network_reference),Network_Components::Prototypes::Network_Prototype,ComponentType);
				this->network_reference<network_itf*>(network);

			}
		};



		implementation struct Advanced_Network_Skimming_Implementation : public _Basic_Network_Skimming_Elements<MasterType>, public Polaris_Component_Class<Advanced_Network_Skimming_Implementation,MasterType,Execution_Object,ParentType>
		{

		};
	}

}