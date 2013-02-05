#pragma once
#include "User_Space.h"

namespace Network_Skimming_Components
{
	namespace Types
	{
		enum CALCULATION_STATUS
		{
			PROCESSING,
			COMPLETE
		};
		enum SUB_ITERATIONS
		{
			INITIALIZE = 19,
			PROCESS = 20,
			UPDATE = 21
		};
	}

	namespace Concepts
	{
	}
	
	namespace Prototypes
	{
		prototype struct Network_Skimming_Prototype : ComponentType
		{
			tag_as_prototype;

			//=============================================
			// Primary events
			//---------------------------------------------
			declare_feature_conditional(Skim_Table_Update_Conditional)
			{
				typedef Network_Skimming_Prototype<ComponentType, CallerType> _Skim_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_Skim_Interface* this_ptr=(_Skim_Interface*)_pthis;

				if (_sub_iteration == 0)
				{
					response.next._iteration = _iteration;
					response.next._sub_iteration = Types::SUB_ITERATIONS::INITIALIZE;
					response.result = false;
				}
				else
				{
					response.next._iteration = Simulation_Time.Future_Time<Simulation_Timestep_Increment,Simulation_Timestep_Increment>(this_ptr->update_increment<Simulation_Timestep_Increment>());
					response.next._sub_iteration = 0;
					response.result = true;
				}
			}
			declare_feature_event(Update_Skims_Event)
			{
				typedef Network_Skimming_Prototype<ComponentType, CallerType> _Skim_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_Skim_Interface* this_ptr=(_Skim_Interface*)_pthis;

				this_ptr->Update_Skims<NULLTYPE>();
			}


			//=============================================
			// Primary data accessors
			//---------------------------------------------
			// links back to the network to be skimmed
			feature_accessor(network_reference, check(ReturnValueType,Network_Components::Concepts::Is_Transportation_Network_Prototype), check(SetValueType,Network_Components::Concepts::Is_Transportation_Network_Prototype) || check(SetValueType,Network_Components::Concepts::Is_Transportation_Network_Prototype));
			// contains a map of skim_tables, one for each specified time slice
			feature_accessor(skim_tables_container, none, none);
			// reference to the current, completed skim table from skim_tables_container, based on simulation time, where skim_table
			feature_accessor(current_skim_table, none, none);
			// time increment at which skim tables are updated - set in the initializer
			feature_accessor(update_increment,check(ReturnValueType,Basic_Units::Concepts::Is_Time_Value),check(SetValueType,Basic_Units::Concepts::Is_Time_Value));
			// scheduled time at which skim tables are updated - set in the initializer
			feature_accessor(scheduled_update_time,check(ReturnValueType,Basic_Units::Concepts::Is_Time_Value),check(SetValueType,Basic_Units::Concepts::Is_Time_Value));
			// number of nodes selected per zone used to estimate zonal travel times
			feature_accessor(nodes_per_zone,none,none);

			//=============================================
			// Primary function accessors - used to pass through to the specific skimm table based on time-key
			//---------------------------------------------
			feature_prototype void Initialize()
			{
				load_event(ComponentType,Skim_Table_Update_Conditional,Update_Skims_Event,0,Types::SUB_ITERATIONS::PROCESS,NULLTYPE);
			}
			feature_prototype void Initialize(TargetType network_reference, requires(check_as_given(TargetType, is_pointer) && check(TargetType, Network_Components::Concepts::Is_Transportation_Network_Prototype)))
			{
				// this sets the implementation specific data members
				this_component()->Initialize<ComponentType,CallerType,TargetType>(network_reference);

				// set the network references
				this->network_reference<TargetType>(network_reference);

				load_event(ComponentType,Skim_Table_Update_Conditional,Update_Skims_Event,0,Types::SUB_ITERATIONS::INITIALIZE,NULLTYPE);

			}			
			feature_prototype void Initialize(TargetType network_reference, requires(!check_as_given(TargetType, is_pointer) || !check(TargetType, Network_Components::Concepts::Is_Transportation_Network_Prototype)))
			{
				assert_check_as_given(TargetType, is_pointer,"TargetType is not a pointer");
				assert_check(TargetType, Network_Components::Concepts::Is_Transportation_Network_Prototype, "TargetType is not a valid Transportation_Network interface");
				assert_sub_check(TargetType, Network_Components::Concepts::Is_Transportation_Network_Prototype, is_basic_network, "TargetType is not a basic network");
				assert_sub_check(TargetType, Network_Components::Concepts::Is_Transportation_Network_Prototype, has_turns, "TargetType does not have turns accessor");
				assert_sub_check(TargetType, Network_Components::Concepts::Is_Transportation_Network_Prototype, has_locations, "TargetType does not have locations accessor");
				assert_sub_check(TargetType, Network_Components::Concepts::Is_Transportation_Network_Prototype, has_zones, "TargetType does not have zones accessor");
			}			
			feature_prototype void Initialize(typename TargetType::ParamType input_file, typename TargetType::Param2Type, requires(check_as_given(typename TargetType::Param2Type, is_pointer) && check(typename TargetType::Param2Type, Network_Components::Concepts::Is_Transportation_Network_Prototype)))
			{
				this_component()->Initialize<ComponentType,CallerType,TargetType>(input_file, network_reference);
				load_event(ComponentType,Skim_Table_Update_Conditional,Update_Skims_Event,0,Types::SUB_ITERATIONS::PROCESS,NULLTYPE);
			}
			feature_prototype void Initialize(typename TargetType::ParamType input_file, typename TargetType::Param2Type, requires(!check_as_given(typename TargetType::Param2Type, is_pointer) || !check(typename TargetType::Param2Type, Network_Components::Concepts::Is_Transportation_Network_Prototype)))
			{
				assert_check_as_given(TargetType, is_pointer,"TargetType is not a pointer");
				assert_check(TargetType, Network_Components::Concepts::Is_Transportation_Network_Prototype, "TargetType is not a valid Transportation_Network interface");
			}
			
			feature_prototype bool Update_Skims()
			{
				return this_component()->Update_Skims<ComponentType,CallerType,TargetType>();
			}

			//=============================================
			// Primary function accessors - used to pass through to the specific skimm table based on time-key
			//---------------------------------------------
			// This returns the travel time based on free-flow speed
			feature_prototype typename TargetType::ReturnType Get_LOS(typename TargetType::ParamType Origin_ID, typename TargetType::ParamType Destination_ID, typename TargetType::Param2Type Mode_Indicator, requires(check(typename TargetType::ReturnType, Basic_Units::Concepts::Is_Time_Value)))
			{
			}
			// This returns the travel time based on the current simulation time
			feature_prototype typename TargetType::ReturnType Get_Current_LOS(typename TargetType::ParamType Origin_ID, typename TargetType::ParamType Destination_ID, typename TargetType::Param2Type Mode_Indicator, requires(check(typename TargetType::ReturnType, Basic_Units::Concepts::Is_Time_Value)))
			{
			}
			// This returns the travel time during a specific time interval
			feature_prototype typename TargetType::ReturnType Get_LOS(typename TargetType::ParamType Origin_ID, typename TargetType::ParamType Destination_ID, typename TargetType::Param2Type Mode_Indicator, typename TargetType::Param3Type Start_Time, requires(check(typename TargetType::ReturnType, Basic_Units::Concepts::Is_Time_Value)))
			{
				// example fo a possible function to direct to appropriate skim_table (needs to be updated with interfaces, etc.
				//this->skim_tables_container<skim_tables_container::iterator&>().find(Intervalized_Start_Time)->second->Get_LOS<TargetType>(Origin_ID, Destination_ID,Mode_Indicator);
			}

		};

		prototype struct Skim_Table_Prototype
		{
			tag_as_prototype;

			// Associative Container of skim matrices, keyed on Mode Indicator values
			feature_accessor(skim_tables_container,none,none);
			// links back to the network to be skimmed
			feature_accessor(network_reference, none, none);
			// links back to the skimming faculty
			feature_accessor(skim_reference, none, none);
			// number of nodes selected per zone used to estimate zonal travel times
			feature_accessor(nodes_per_zone,none,none);

			//=============================================
			// Primary function accessors - used to calculate/return LOS values for OD pairs
			//---------------------------------------------
			feature_prototype void Initialize()
			{
			}
			feature_prototype void Initialize(TargetType network_reference, requires(check_as_given(TargetType, is_pointer) && check(TargetType, Network_Components::Concepts::Is_Transportation_Network_Prototype)))
			{
				this_component()->Initialize<ComponentType,CallerType,TargetType>(network_reference);
			}			
			feature_prototype void Initialize(TargetType network_reference, requires(!check_as_given(TargetType, is_pointer) || !check(TargetType, Network_Components::Concepts::Is_Transportation_Network_Prototype)))
			{
				assert_check_as_given(TargetType, is_pointer,"TargetType is not a pointer");
				assert_check(TargetType, Network_Components::Concepts::Is_Transportation_Network_Prototype, "TargetType is not a valid Transportation_Network interface");
				assert_sub_check(TargetType, Network_Components::Concepts::Is_Transportation_Network_Prototype, is_basic_network, "TargetType is not a basic network");
				assert_sub_check(TargetType, Network_Components::Concepts::Is_Transportation_Network_Prototype, has_turns, "TargetType does not have turns accessor");
				assert_sub_check(TargetType, Network_Components::Concepts::Is_Transportation_Network_Prototype, has_locations, "TargetType does not have locations accessor");
				assert_sub_check(TargetType, Network_Components::Concepts::Is_Transportation_Network_Prototype, has_zones, "TargetType does not have zones accessor");
			}			
			feature_prototype void Initialize(typename TargetType::ParamType input_file, typename TargetType::Param2Type, requires(check_as_given(typename TargetType::Param2Type, is_pointer) && check(typename TargetType::Param2Type, Network_Components::Concepts::Is_Transportation_Network_Prototype)))
			{
				this_component()->Initialize<ComponentType,CallerType,TargetType>(input_file, network_reference);
			}
			feature_prototype void Initialize(typename TargetType::ParamType input_file, typename TargetType::Param2Type, requires(!check_as_given(typename TargetType::Param2Type, is_pointer) || !check(typename TargetType::Param2Type, Network_Components::Concepts::Is_Transportation_Network_Prototype)))
			{
				assert_check_as_given(TargetType, is_pointer,"TargetType is not a pointer");
				assert_check(TargetType, Network_Components::Concepts::Is_Transportation_Network_Prototype, "TargetType is not a valid Transportation_Network interface");
			}
			
			feature_prototype bool Update_LOS()
			{
				return this_component()->Update_LOS<ComponentType,CallerType,TargetType>();
			}
			feature_prototype typename TargetType::ReturnType Get_LOS(typename TargetType::ParamType Origin_ID, typename TargetType::ParamType Destination_ID, typename TargetType::Param2Type Mode_Indicator)
			{

			}
		};

		prototype struct Network_Path_Tree_Builder_Prototype
		{
			tag_as_prototype;

			feature_accessor(mode_type_id,none,none);
			// number of nodes selected per zone used to estimate zonal travel times
			feature_accessor(origin_node_zone_pair,none,none);
			// Associative Container of skim matrices, keyed on Mode Indicator values
			feature_accessor(destination_node_zone_list,none,none);
			// links back to the network to be skimmed
			feature_accessor(network_reference, none, none);
			// links back to the skimming faculty
			feature_accessor(skim_reference, none, none);
			feature_accessor(event_time,none,none);


			//=============================================
			// Primary events
			//---------------------------------------------
			declare_feature_conditional(tree_builder_conditional)
			{
				typedef Network_Path_Tree_Builder_Prototype<ComponentType, CallerType> _Path_Tree_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_Path_Tree_Interface* this_ptr=(_Path_Tree_Interface*)_pthis;
	
				define_component_interface(skim_itf, get_type_of(skim_reference),Prototypes::Network_Skimming_Prototype,ComponentType);
				skim_itf* skimmer = this_ptr->skim_reference<skim_itf*>();

				if (_sub_iteration == Types::PROCESS)
				{
					response.next._iteration = Simulation_Time.Future_Time<Simulation_Timestep_Increment,Simulation_Timestep_Increment>(skimmer->update_increment<Simulation_Timestep_Increment>());
					response.next._sub_iteration = Types::PROCESS;
					response.result = true;
				}
				else
				{
					static_assert(false);
				}

			}

			declare_feature_event(tree_builder_event)
			{
				typedef Network_Path_Tree_Builder_Prototype<ComponentType, CallerType> _Path_Tree_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_Path_Tree_Interface* this_ptr=(_Path_Tree_Interface*)_pthis;

				define_container_and_value_interface(pair_list_itf, pair_itf,get_type_of(destination_node_zone_list),Random_Access_Sequence_Prototype, Node_Zone_Pair_Prototype, ComponentType);
				pair_itf* orig = this_ptr->origin_node_zone_pair<pair_itf*>();
				cout << endl<<"building tree from : " << orig->node_index;
				
				pair_list_itf* dest_list = this_ptr->destination_node_zone_list<pair_list_itf*>();
				pair_list_itf::iterator itr = dest_list->begin();

				for (;itr != dest_list->end(); ++itr)
				{
					cout << endl<<"to: "<<*itr->node_index;
				}
			}
			
			feature_prototype void Initialize()
			{
				load_event(ComponentType,tree_builder_conditional,tree_builder_event,this->event_time<Simulation_Timestep_Increment>(),Types::SUB_ITERATIONS::PROCESS,NULLTYPE);
			}
		};

		prototype struct Node_Zone_Pair_Prototype
		{
			tag_as_prototype;
			feature_accessor(node_index,none,none);
			feature_accessor(zone_index,none,none);
		};
	}
}
