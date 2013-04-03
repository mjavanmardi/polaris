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
			// Pointer to the Parent class
			member_prototype(Person_Components::Prototypes::Person_Planner, Parent_Planner, typename MasterType::person_planner_type, none, none);

			// PARAMETER DECLARATIONS
			static member_data(float, B_TTIME, none,none);
			static member_data(float, B_EMPLOYMENT, none,none);
			static member_data(float, B_POPULATION, none,none);

			member_prototype(Activity_Location_Components::Prototypes::Activity_Location_Prototype, origin, typename MasterType::activity_location_type, none,none );
			member_prototype(Activity_Location_Components::Prototypes::Activity_Location_Prototype, destination, typename MasterType::activity_location_type, none,none );
			
			// Interface definitions
			define_component_interface(person_itf,typename type_of(Parent_Planner)::type_of(Parent_Person), Prototypes::Person,ComponentType);
			define_component_interface(_Scenario_Interface, typename type_of(Parent_Planner)::type_of(Parent_Person)::type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
			define_component_interface(_Network_Interface, typename type_of(Parent_Planner)::type_of(Parent_Person)::type_of(network_reference), Network_Components::Prototypes::Network_Prototype, ComponentType);	
			define_component_interface(_Skim_Interface, typename _Network_Interface::get_type_of(skimming_faculty),Network_Skimming_Components::Prototypes::Network_Skimming_Prototype,ComponentType);
			define_container_and_value_interface(_Activity_Locations_Container_Interface, _Activity_Location_Interface, typename _Network_Interface::get_type_of(activity_locations_container), Random_Access_Sequence_Prototype, Activity_Location_Components::Prototypes::Activity_Location_Prototype, ComponentType);
			define_container_and_value_interface(_Links_Container_Interface, _Link_Interface, typename _Activity_Location_Interface::get_type_of(origin_links), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
			define_container_and_value_interface(_Zones_Container_Interface, _Zone_Interface, typename _Network_Interface::get_type_of(zones_container), Associative_Container_Prototype, Zone_Components::Prototypes::Zone_Prototype, ComponentType);
			define_container_and_value_interface_unqualified_container(Activity_Plans,Activity_Plan, typename type_of(Parent_Planner)::type_of(Activity_Plans_Container),Containers::Back_Insertion_Sequence_Prototype,Activity_Components::Prototypes::Activity_Planner,ComponentType);
			define_container_and_value_interface_unqualified_container(Movement_Plans,Movement_Plan, typename type_of(Parent_Planner)::type_of(Movement_Plans_Container),Containers::Back_Insertion_Sequence_Prototype,Movement_Plan_Components::Prototypes::Movement_Plan_Prototype,ComponentType);

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
				person_itf* _Parent_Person = _Parent_Planner->Parent_Person<person_itf*>();

				// external knowledge references
				_Network_Interface* network = _Parent_Person->template network_reference<_Network_Interface*>();
				_Zones_Container_Interface* zones = network->zones_container<_Zones_Container_Interface*>();
				_Activity_Locations_Container_Interface* locations = network->activity_locations_container<_Activity_Locations_Container_Interface*>();
				_Skim_Interface* LOS = network->skimming_faculty<_Skim_Interface*>();

				// variables used for utility calculation
				float ttime, pop, emp, u;
				float utility_sum = 0;
				float prob_sum = 0;
				_Zone_Interface* zone;

				// select zones to choose from and estimate utility
				zone = _destination->template zone<_Zone_Interface*>();

				ttime = LOS->Get_LOS<Target_Type<NULLTYPE,Time_Minutes,int,Vehicle_Components::Types::Vehicle_Type_Keys>>(_origin->zone<_Zone_Interface*>()->uuid<int>(),zone->uuid<int>(),Vehicle_Components::Types::Vehicle_Type_Keys::SOV);
				pop = zone->population<float>();
				emp = zone->employment<float>();

				u = exp(_B_POPULATION * pop + _B_EMPLOYMENT * emp - _B_TTIME * ttime);
				return (typename TargetType)u;				
			}
			tag_feature_as_available(Calculate_Utility);


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
			typedef TypeList<Prototypes::Destination_Choice_Option<typename MasterType::person_destination_choice_option_type >> TList;
		};



		implementation struct CTRAMP_Destination_Chooser_Implementation : public Polaris_Component<APPEND_CHILD(CTRAMP_Destination_Chooser_Implementation),MasterType,Data_Object,ParentType>
		{
			// Pointer to the Parent class
			member_prototype(Person_Components::Prototypes::Person_Planner, Parent_Planner, typename MasterType::person_planner_type, none, none);
			member_prototype(Choice_Model_Components::Prototypes::Choice_Model, Choice_Model, Destination_Choice_Model_Implementation<MasterType>,none,none);
			
			static member_data(int, choice_set_size,none,none);

			// Interface definitions
			typedef Choice_Model_Components::Prototypes::Choice_Model<Destination_Choice_Model_Implementation<MasterType> > _Choice_Model_Interface;
			typedef Prototypes::Destination_Choice_Option<typename MasterType::person_destination_choice_option_type> _Choice_Option_Interface;
			define_component_interface(person_itf,typename type_of(Parent_Planner)::type_of(Parent_Person), Prototypes::Person,ComponentType);
			define_component_interface(_Scenario_Interface, typename type_of(Parent_Planner)::type_of(Parent_Person)::type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
			define_component_interface(_Network_Interface, typename type_of(Parent_Planner)::type_of(Parent_Person)::type_of(network_reference), Network_Components::Prototypes::Network_Prototype, ComponentType);	
			define_component_interface(_Skim_Interface, typename _Network_Interface::get_type_of(skimming_faculty),Network_Skimming_Components::Prototypes::Network_Skimming_Prototype,ComponentType);
			define_container_and_value_interface(_Activity_Locations_Container_Interface, _Activity_Location_Interface, typename _Network_Interface::get_type_of(activity_locations_container), Random_Access_Sequence_Prototype, Activity_Location_Components::Prototypes::Activity_Location_Prototype, ComponentType);
			define_container_and_value_interface(_Links_Container_Interface, _Link_Interface, typename _Activity_Location_Interface::get_type_of(origin_links), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
			define_container_and_value_interface(_Zones_Container_Interface, _Zone_Interface, typename _Network_Interface::get_type_of(zones_container), Associative_Container_Prototype, Zone_Components::Prototypes::Zone_Prototype, ComponentType);
			define_container_and_value_interface_unqualified_container(Activity_Plans,Activity_Plan, typename type_of(Parent_Planner)::type_of(Activity_Plans_Container),Containers::Back_Insertion_Sequence_Prototype,Activity_Components::Prototypes::Activity_Planner,ComponentType);
			define_container_and_value_interface_unqualified_container(Movement_Plans,Movement_Plan, typename type_of(Parent_Planner)::type_of(Movement_Plans_Container),Containers::Back_Insertion_Sequence_Prototype,Movement_Plan_Components::Prototypes::Movement_Plan_Prototype,ComponentType);

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

			feature_implementation TargetType Choose_Destination(TargetType origin, requires(check_as_given(TargetType,is_pointer) && check(TargetType,Activity_Location_Components::Concepts::Is_Activity_Location)))
			{
				person_itf* _Parent_Person = _Parent_Planner->Parent_Person<person_itf*>();
				
				// create local choice model
				Destination_Choice_Model_Implementation<MasterType> a;
				_Choice_Model_Interface* choice_model = (_Choice_Model_Interface*)&a;

				// get references to the plan containers
				Activity_Plans* activities = typename _Parent_Planner->template Activity_Plans_Container<Activity_Plans*>();
				Movement_Plans* movements = typename _Parent_Planner->template Movement_Plans_Container<Movement_Plans*>();	

				// external knowledge references
				_Network_Interface* network = _Parent_Person->template network_reference<_Network_Interface*>();
				_Zones_Container_Interface* zones = network->zones_container<_Zones_Container_Interface*>();
				_Activity_Locations_Container_Interface* locations = network->activity_locations_container<_Activity_Locations_Container_Interface*>();
				_Skim_Interface* LOS = network->skimming_faculty<_Skim_Interface*>();

				// get reference to origin zone
				_Activity_Location_Interface* orig = (_Activity_Location_Interface*)origin;

				// variables used for utility calculation
				const int size = (int)locations->size();
				int loc_id;
				vector<_Choice_Option_Interface*> loc_options;

				// select zones to choose from and estimate utility
				for (int i=0; i<_choice_set_size; i++)
				{
					loc_id = (int)((Uniform_RNG.Next_Rand<float>()*0.999999)*size);
					_Activity_Location_Interface* loc = locations->at(loc_id);

					_Choice_Option_Interface* choice = (_Choice_Option_Interface*)Allocate<typename MasterType::person_destination_choice_option_type>();
					choice->origin<_Activity_Location_Interface*>(orig);
					choice->destination<_Activity_Location_Interface*>(loc);
					choice->Parent_Planner<Parent_Planner_interface*>(_Parent_Planner);
					choice_model->Add_Choice_Option<_Choice_Option_Interface*>(choice);
					loc_options.push_back(choice);

					//cout << endl << "location id: " << choice_model->Choice_At<_Choice_Option_Interface*>(i)->destination<_Activity_Location_Interface*>()->uuid<int>() << ", or: " << loc->uuid<int>() << ", at loc_index="<<loc_id;
				}

				// Make choice
				int selected_index = 0;
				choice_model->Evaluate_Choices<NT>();

				//cout << endl << "location id at index 0 is: " << choice_model->Choice_At<_Choice_Option_Interface*>(0)->destination<_Activity_Location_Interface*>()->uuid<int>();

				_Choice_Option_Interface* selected = choice_model->Choose<_Choice_Option_Interface*>(selected_index);

				//cout << endl << "selected location id is: " << choice_model->Choice_At<_Choice_Option_Interface*>(selected_index)->destination<_Activity_Location_Interface*>()->uuid<int>();

				//cout << endl << "selected location id: " << selected->destination<_Activity_Location_Interface*>()->uuid<int>();

				TargetType return_ptr = choice_model->Choice_At<_Choice_Option_Interface*>(selected_index)->destination<TargetType>();

				// free memory allocated locally
				for (int i = 0; i < loc_options.size(); i++)
				{
					Free<_Choice_Option_Interface::Component_Type>((_Choice_Option_Interface::Component_Type*)loc_options[i]);
				}

									
				//cout << ", destination:" <<return_ptr->uuid<int>() <<endl;

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
