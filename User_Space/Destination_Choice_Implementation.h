#pragma once

#include "Person_Prototype.h"

namespace Person_Components
{
	namespace Implementations
	{
		//==================================================================================
		/// Planning classes
		//----------------------------------------------------------------------------------

		implementation struct CTRAMP_Destination_Choice_Option : public Polaris_Component<APPEND_CHILD(CTRAMP_Destination_Choice_Option),MasterType,Data_Object,ParentType,true>
		{
			// Tag as Implementation
			typedef typename Polaris_Component<APPEND_CHILD(CTRAMP_Destination_Choice_Option),MasterType,Data_Object>::Component_Type ComponentType;


			// Pointer to the Parent class
			member_prototype(Person_Components::Prototypes::Person_Planner, Parent_Planner, typename MasterType::person_planner_type, none, none);

			// PARAMETER DECLARATIONS
			static member_data(float, B_TTIME, none,none);
			static member_data(float, B_EMPLOYMENT, none,none);
			static member_data(float, B_POPULATION, none,none);

			member_prototype(Activity_Location_Components::Prototypes::Activity_Location_Prototype, previous, typename MasterType::activity_location_type, none,none );
			member_prototype(Activity_Location_Components::Prototypes::Activity_Location_Prototype, destination, typename MasterType::activity_location_type, none,none );
			member_prototype(Activity_Location_Components::Prototypes::Activity_Location_Prototype, next, typename MasterType::activity_location_type, none,none );
			
			//====================================================================================================================================
			// Interface definitions
			define_component_interface(person_itf,typename type_of(Parent_Planner)::type_of(Parent_Person), Prototypes::Person,ComponentType);
			define_component_interface(scheduler_itf,typename person_itf::get_type_of(Scheduling_Faculty), Prototypes::Person_Scheduler,ComponentType);
			define_component_interface(_Scenario_Interface, typename type_of(Parent_Planner)::type_of(Parent_Person)::type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
			define_component_interface(_Network_Interface, typename type_of(Parent_Planner)::type_of(Parent_Person)::type_of(network_reference), Network_Components::Prototypes::Network_Prototype, ComponentType);	
			define_component_interface(_Skim_Interface, typename _Network_Interface::get_type_of(skimming_faculty),Network_Skimming_Components::Prototypes::Network_Skimming_Prototype,ComponentType);
			define_container_and_value_interface(_Activity_Locations_Container_Interface, _Activity_Location_Interface, typename _Network_Interface::get_type_of(activity_locations_container), Random_Access_Sequence_Prototype, Activity_Location_Components::Prototypes::Activity_Location_Prototype, ComponentType);
			define_container_and_value_interface(_Links_Container_Interface, _Link_Interface, typename _Activity_Location_Interface::get_type_of(origin_links), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
			define_container_and_value_interface(_Zones_Container_Interface, _Zone_Interface, typename _Network_Interface::get_type_of(zones_container), Associative_Container_Prototype, Zone_Components::Prototypes::Zone_Prototype, ComponentType);
			define_container_and_value_interface(Activity_Plans,Activity_Plan, typename scheduler_itf::get_type_of(Activity_Container),Containers::Back_Insertion_Sequence_Prototype,Activity_Components::Prototypes::Activity_Planner,ComponentType);
			define_container_and_value_interface(Movement_Plans,Movement_Plan, typename scheduler_itf::get_type_of(Movement_Plans_Container),Containers::Back_Insertion_Sequence_Prototype,Movement_Plan_Components::Prototypes::Movement_Plan_Prototype,ComponentType);
			//------------------------------------------------------------------------------------------------------------------------------------

			feature_implementation void Initialize(requires(check(typename ComponentType::Parent_Type,Concepts::Is_Person)))
			{	
			}
			feature_implementation void Initialize(requires(check(typename ComponentType::Parent_Type,!Concepts::Is_Person)))
			{	
				assert_sub_check(typename ComponentType::Parent_Type,Concepts::Is_Person,Has_Initialize_Defined, "The specified ParentType is not a valid Person type.");
				assert_sub_check(typename ComponentType::Parent_Type,Concepts::Is_Person,Has_Properties_Defined, "The specified ParentType does not have the required Properties member defined.");
				assert_sub_check(typename ComponentType::Parent_Type,Concepts::Is_Person,Has_Planner_Defined, "The specified ParentType does not have the required Planner member defined.");
			}
			tag_feature_as_available(Initialize);

			feature_implementation TargetType Calculate_Utility()
			{
				person_itf* _Parent_Person = _Parent_Planner->template Parent_Person<person_itf*>();

				// external knowledge references
				_Network_Interface* network = _Parent_Person->template network_reference<_Network_Interface*>();
				_Zones_Container_Interface* zones = network->template zones_container<_Zones_Container_Interface*>();
				_Activity_Locations_Container_Interface* locations = network->template activity_locations_container<_Activity_Locations_Container_Interface*>();
				_Skim_Interface* LOS = network->template skimming_faculty<_Skim_Interface*>();

				// variables used for utility calculation
				TargetType ttime_before, ttime_after, ttime_without, ttime_deflected, pop, emp, u;
				TargetType utility_sum = 0;
				TargetType prob_sum = 0;
				_Zone_Interface* zone;

				// select zones to choose from and estimate utility
				zone = _destination->template zone<_Zone_Interface*>();

				// get the deflected travel time due to the addition of the current activity
				ttime_before = LOS->template Get_LOS<Target_Type<NULLTYPE,Time_Minutes,int,Vehicle_Components::Types::Vehicle_Type_Keys>>(_previous->template zone<_Zone_Interface*>()->template uuid<int>(),zone->template uuid<int>(),Vehicle_Components::Types::Vehicle_Type_Keys::SOV);
				ttime_after = LOS->template Get_LOS<Target_Type<NULLTYPE,Time_Minutes,int,Vehicle_Components::Types::Vehicle_Type_Keys>>(zone->template uuid<int>(),_next->template zone<_Zone_Interface*>()->template uuid<int>(),Vehicle_Components::Types::Vehicle_Type_Keys::SOV);
				ttime_without = LOS->template Get_LOS<Target_Type<NULLTYPE,Time_Minutes,int,Vehicle_Components::Types::Vehicle_Type_Keys>>(_previous->template zone<_Zone_Interface*>()->template uuid<int>(),_next->template zone<_Zone_Interface*>()->template uuid<int>(),Vehicle_Components::Types::Vehicle_Type_Keys::SOV);
				ttime_deflected = ttime_before + ttime_after - ttime_without;
				
				pop = zone->template population<TargetType>();
				emp = zone->template employment<TargetType>();

				u = (_B_POPULATION * log(pop+1.0) + _B_EMPLOYMENT * log(emp+1.0) + _B_TTIME * ttime_deflected);
				if (u > 100.0) THROW_WARNING("WARNING: utility > 200.0 will cause numeric overflow, possible misspecification in utility function for destination choice. [Pop,emp,ttime]="<<pop << ", " << emp << ", " << ttime_deflected);

				return (TargetType)u;				
			}
			tag_feature_as_available(Calculate_Utility);

			feature_implementation TargetType Print_Utility()
			{
				person_itf* _Parent_Person = _Parent_Planner->template Parent_Person<person_itf*>();

				// external knowledge references
				_Network_Interface* network = _Parent_Person->template network_reference<_Network_Interface*>();
				_Zones_Container_Interface* zones = network->template zones_container<_Zones_Container_Interface*>();
				_Activity_Locations_Container_Interface* locations = network->template activity_locations_container<_Activity_Locations_Container_Interface*>();
				_Skim_Interface* LOS = network->template skimming_faculty<_Skim_Interface*>();

				// variables used for utility calculation
				TargetType ttime, pop, emp, u;
				TargetType utility_sum = 0;
				TargetType prob_sum = 0;
				_Zone_Interface* zone;

				// select zones to choose from and estimate utility
				zone = _destination->template zone<_Zone_Interface*>();

				ttime = LOS->template Get_LOS<Target_Type<NULLTYPE,Time_Minutes,int,Vehicle_Components::Types::Vehicle_Type_Keys>>(_previous->template zone<_Zone_Interface*>()->template uuid<int>(),zone->template uuid<int>(),Vehicle_Components::Types::Vehicle_Type_Keys::SOV);
				pop = zone->template population<TargetType>();
				emp = zone->template employment<TargetType>();

				u = (_B_POPULATION * pop + _B_EMPLOYMENT * emp + _B_TTIME * ttime);
				if (u > 100.0) THROW_WARNING("WARNING: utility > 100.0 will cause numeric overflow, possible misspecification in utility function for destination choice. [Pop,emp,ttime]="<<pop << ", " << emp << ", " << ttime);

				cout << zone->template uuid<int>() << ": _BPOP["<<_B_POPULATION<<"] * " << pop << " + _B_EMP[" << _B_EMPLOYMENT<<"] * " << emp << " + B_TTIME["<<_B_TTIME << "] * " << ttime << " = " << u;
				return (TargetType)u;				
			}
			tag_feature_as_available(Print_Utility);
		};
		#pragma region Choice option parameters
		// INITIALIZE DESTINATION MODEL STATIC PARAMETERS
		static_member_definition(Person_Components::Implementations::CTRAMP_Destination_Choice_Option,B_TTIME);
		static_member_definition(Person_Components::Implementations::CTRAMP_Destination_Choice_Option,B_EMPLOYMENT);
		static_member_definition(Person_Components::Implementations::CTRAMP_Destination_Choice_Option,B_POPULATION);
		#pragma endregion


		implementation struct Destination_Choice_Model_Implementation : public Choice_Model_Components::Implementations::MNL_Model_Implementation<MasterType,ParentType, APPEND_CHILD(Destination_Choice_Model_Implementation)>
		{
			typedef Choice_Model_Components::Implementations::MNL_Model_Implementation<MasterType,ParentType, APPEND_CHILD(Destination_Choice_Model_Implementation)> BaseType;
			typedef typename BaseType::Component_Type ComponentType;
			typedef TypeList<Prototypes::Destination_Choice_Option<typename MasterType::person_destination_choice_option_type >> TList;
		};



		implementation struct CTRAMP_Destination_Chooser_Implementation : public Polaris_Component<APPEND_CHILD(CTRAMP_Destination_Chooser_Implementation),MasterType,Data_Object,ParentType>
		{
			// Tag as Implementation
			typedef typename Polaris_Component<APPEND_CHILD(CTRAMP_Destination_Chooser_Implementation),MasterType,Data_Object>::Component_Type ComponentType;

			// Pointer to the Parent class
			member_prototype(Person_Components::Prototypes::Person_Planner, Parent_Planner, typename MasterType::person_planner_type, none, none);
			member_prototype(Choice_Model_Components::Prototypes::Choice_Model, Choice_Model, Destination_Choice_Model_Implementation<MasterType>,none,none);
			
			static member_data(int, choice_set_size,none,none);

			// Interface definitions
			typedef Choice_Model_Components::Prototypes::Choice_Model<Destination_Choice_Model_Implementation<MasterType> > _Choice_Model_Interface;
			typedef Prototypes::Destination_Choice_Option<typename MasterType::person_destination_choice_option_type> _Choice_Option_Interface;

			define_component_interface(person_itf,typename type_of(Parent_Planner)::type_of(Parent_Person), Prototypes::Person,ComponentType);
			define_component_interface(scheduler_itf,typename person_itf::get_type_of(Scheduling_Faculty),Prototypes::Person_Scheduler,ComponentType);
			define_component_interface(_Scenario_Interface, typename type_of(Parent_Planner)::type_of(Parent_Person)::type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
			define_component_interface(_Network_Interface, typename type_of(Parent_Planner)::type_of(Parent_Person)::type_of(network_reference), Network_Components::Prototypes::Network_Prototype, ComponentType);	
			define_component_interface(_Skim_Interface, typename _Network_Interface::get_type_of(skimming_faculty),Network_Skimming_Components::Prototypes::Network_Skimming_Prototype,ComponentType);
			define_container_and_value_interface(_Activity_Locations_Container_Interface, _Activity_Location_Interface, typename _Network_Interface::get_type_of(activity_locations_container), Random_Access_Sequence_Prototype, Activity_Location_Components::Prototypes::Activity_Location_Prototype, ComponentType);
			define_container_and_value_interface(_Links_Container_Interface, _Link_Interface, typename _Activity_Location_Interface::get_type_of(origin_links), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
			define_container_and_value_interface(_Zones_Container_Interface, _Zone_Interface, typename _Network_Interface::get_type_of(zones_container), Associative_Container_Prototype, Zone_Components::Prototypes::Zone_Prototype, ComponentType);
			define_container_and_value_interface(Activity_Plans,Activity_Plan, typename scheduler_itf::get_type_of(Activity_Container),Containers::Back_Insertion_Sequence_Prototype,Activity_Components::Prototypes::Activity_Planner,ComponentType);
			define_container_and_value_interface(Movement_Plans,Movement_Plan, typename scheduler_itf::get_type_of(Movement_Plans_Container),Containers::Back_Insertion_Sequence_Prototype,Movement_Plan_Components::Prototypes::Movement_Plan_Prototype,ComponentType);

			feature_implementation void Initialize(requires(check(typename ComponentType::Parent_Type,Concepts::Is_Person)))
			{	
			}
			feature_implementation void Initialize(requires(check(typename ComponentType::Parent_Type,!Concepts::Is_Person)))
			{	
				assert_sub_check(typename ComponentType::Parent_Type,Concepts::Is_Person,Has_Initialize_Defined, "The specified ParentType is not a valid Person type.");
				assert_sub_check(typename ComponentType::Parent_Type,Concepts::Is_Person,Has_Properties_Defined, "The specified ParentType does not have the required Properties member defined.");
				assert_sub_check(typename ComponentType::Parent_Type,Concepts::Is_Person,Has_Planner_Defined, "The specified ParentType does not have the required Planner member defined.");
			}
			tag_feature_as_available(Initialize);

			feature_implementation TargetType Choose_Destination(TargetType previous_location, TargetType next_location, vector<TargetType>* destinations_to_use=nullptr, requires(check_as_given(TargetType,is_pointer) && check(TargetType,Activity_Location_Components::Concepts::Is_Activity_Location)))
			{
				person_itf* _Parent_Person = _Parent_Planner->template Parent_Person<person_itf*>();
				
				// create local choice model
				Destination_Choice_Model_Implementation<MasterType> a;
				_Choice_Model_Interface* choice_model = (_Choice_Model_Interface*)&a;

				// get references to the plan containers
				//Activity_Plans* activities = _Parent_Person->template Activity_Container<Activity_Plans*>();
				//Movement_Plans* movements = _Parent_Person->template Movement_Plans_Container<Movement_Plans*>();	

				// external knowledge references
				_Network_Interface* network = _Parent_Person->template network_reference<_Network_Interface*>();
				_Zones_Container_Interface* zones = network->template zones_container<_Zones_Container_Interface*>();

				// selecte locations to choose from - use all if destinations to use not specified
				_Activity_Locations_Container_Interface* locations;
				if (destinations_to_use == nullptr) locations= network->template activity_locations_container<_Activity_Locations_Container_Interface*>();
				else locations = (_Activity_Locations_Container_Interface*)destinations_to_use;

				_Skim_Interface* LOS = network->template skimming_faculty<_Skim_Interface*>();

				// get reference to origin zone
				_Activity_Location_Interface* prev = (_Activity_Location_Interface*)previous_location;
				_Activity_Location_Interface* next = (_Activity_Location_Interface*)next_location;

				// variables used for utility calculation
				const int size = (int)locations->size();
				int loc_id;
				vector<_Choice_Option_Interface*> loc_options;

				// select zones to choose from and estimate utility
				for (int i=0; i<_choice_set_size; i++)
				{
					loc_id = (int)((Uniform_RNG.template Next_Rand<float>()*0.999999)*size);
					_Activity_Location_Interface* loc = locations->at(loc_id);

					_Choice_Option_Interface* choice = (_Choice_Option_Interface*)Allocate<typename MasterType::person_destination_choice_option_type>();
					choice->template previous<_Activity_Location_Interface*>(prev);
					choice->template destination<_Activity_Location_Interface*>(loc);
					choice->template next<_Activity_Location_Interface*>(next);
					choice->template Parent_Planner<Parent_Planner_interface*>(_Parent_Planner);
					choice_model->template Add_Choice_Option<_Choice_Option_Interface*>(choice);
					loc_options.push_back(choice);
				}

				// Make choice
				int selected_index = 0;
				choice_model->template Evaluate_Choices<NT>();


				_Choice_Option_Interface* selected = choice_model->template Choose<_Choice_Option_Interface*>(selected_index);

				if (selected == nullptr )
				{
					THROW_WARNING("WARNING: selected is null - no destination choice made, index = " << selected_index);
					return nullptr;
				}

				TargetType return_ptr = choice_model->template Choice_At<_Choice_Option_Interface*>(selected_index)->template destination<TargetType>();

				// free memory allocated locally
				for (int i = 0; i < loc_options.size(); i++)
				{
					Free<typename _Choice_Option_Interface::Component_Type>((typename _Choice_Option_Interface::Component_Type*)loc_options[i]);
				}

				return return_ptr;
			}
			tag_feature_as_available(Choose_Destination);
		};
		#pragma region Choice option parameters
		// INITIALIZE DESTINATION MODEL STATIC PARAMETERS
		static_member_definition(Person_Components::Implementations::CTRAMP_Destination_Chooser_Implementation,choice_set_size);
		#pragma endregion

	}
}
