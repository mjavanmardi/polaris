#pragma once

#include "Person_Prototype.h"

namespace Person_Components
{
	namespace Implementations
	{
		//==================================================================================
		/// Planning classes
		//----------------------------------------------------------------------------------
		implementation struct General_Destination_Choice_Implementation : public Polaris_Component<APPEND_CHILD(General_Destination_Choice_Implementation),MasterType,Data_Object,ParentType>
		{
			// Pointer to the Parent class
			member_prototype(Person_Components::Prototypes::Person_Planner, Parent_Planner, typename MasterType::person_planner_type, none, none);

			// Interface definitions
			define_component_interface(_Scenario_Interface, typename type_of(Parent_Planner)::type_of(Parent_Person)::type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
			define_component_interface(_Network_Interface, typename type_of(Parent_Planner)::type_of(Parent_Person)::type_of(network_reference), Network_Components::Prototypes::Network_Prototype, ComponentType);	
			define_component_interface(_Skim_Interface, typename _Network_Interface::get_type_of(skimming_faculty),Network_Skimming_Components::Prototypes::Network_Skimming_Prototype,ComponentType);
			define_container_and_value_interface(_Activity_Locations_Container_Interface, _Activity_Location_Interface, typename _Network_Interface::get_type_of(activity_locations_container), Random_Access_Sequence_Prototype, Activity_Location_Components::Prototypes::Activity_Location_Prototype, ComponentType);
			define_container_and_value_interface(_Links_Container_Interface, _Link_Interface, typename _Activity_Location_Interface::get_type_of(origin_links), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
			define_container_and_value_interface(_Zones_Container_Interface, _Zone_Interface, typename _Network_Interface::get_type_of(zones_container), Associative_Container_Prototype, Zone_Components::Prototypes::Zone_Prototype, ComponentType);
			define_container_and_value_interface_unqualified_container(Activity_Plans,Activity_Plan, typename type_of(Parent_Planner)::type_of(Activity_Plans_Container),Containers::Back_Insertion_Sequence_Prototype,Activity_Components::Prototypes::Activity_Planner,ComponentType);
			define_container_and_value_interface_unqualified_container(Movement_Plans,Movement_Plan, typename type_of(Parent_Planner)::type_of(Movement_Plans_Container),Containers::Back_Insertion_Sequence_Prototype,Movement_Plan_Components::Prototypes::Movement_Plan_Prototype,ComponentType);
		
		};

		implementation struct CTRAMP_Destination_Choice_Implementation : public General_Destination_Choice_Implementation<MasterType, ParentType, APPEND_CHILD(CTRAMP_Destination_Choice_Implementation)>
		{
			// IMPLEMENTATION TYPEDEFS AND INTERFACES
			typedef General_Destination_Choice_Implementation<MasterType, ParentType, APPEND_CHILD(CTRAMP_Destination_Choice_Implementation)> base_type;
			typedef base_type base;
			typedef Prototypes::Activity_Generator<base_type,base_type> base_itf;


			// PARAMETER DECLARATIONS
			static member_data(int, choice_set_size,none,none);
			static member_data(float, B_TTIME, none,none);
			static member_data(float, B_EMPLOYMENT, none,none);
			static member_data(float, B_POPULATION, none,none);

			
			// Interface definitions
			define_component_interface(person_itf,typename base_type::type_of(Parent_Planner)::type_of(Parent_Person), Prototypes::Person,ComponentType);
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
				person_itf* _Parent_Person = base_type::_Parent_Planner->Parent_Person<person_itf*>();

				// get references to the plan containers
				Activity_Plans* activities = typename base_type::_Parent_Planner->template Activity_Plans_Container<Activity_Plans*>();
				Movement_Plans* movements = typename base_type::_Parent_Planner->template Movement_Plans_Container<Movement_Plans*>();	

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
				vector<_Activity_Location_Interface*> loc_options;
				vector<float> utility;
				vector<float> cum_probability;
				float u;
				float utility_sum = 0;
				float prob_sum = 0;

				// select zones to choose from and estimate utility
				for (int i=0; i<_choice_set_size; i++)
				{
					loc_id = (int)((Uniform_RNG.Next_Rand<float>()*0.999999)*size);
					_Activity_Location_Interface* loc = locations->at(loc_id);
					loc_options.push_back(loc);

					u = this->Calculate_Utility<ComponentType,CallerType,Target_Type<NULLTYPE,float,_Activity_Location_Interface*>>(orig, loc);
					utility.push_back(u);
					utility_sum += u;
				}
				if (utility_sum == 0) return nullptr;
				
				// convert to cumulative probability
				for (int i=0; i<_choice_set_size; i++)
				{
					prob_sum += utility[i] / utility_sum;
					cum_probability.push_back(prob_sum);
				}

				// select one of the zones through monte-carlo
				float r = Uniform_RNG.Next_Rand<float>();
				for (int i=0; i<_choice_set_size; i++)
				{
					if (r <= cum_probability[i])
					{
						return (TargetType)loc_options[i];
					}
				}
				return nullptr;

			}
			tag_feature_as_available(Choose_Destination);

			feature_implementation typename TargetType::ReturnType Calculate_Utility(typename TargetType::ParamType origin, typename TargetType::ParamType destination, requires(check_as_given(typename TargetType::ParamType,is_pointer) /*&& check(typename TargetType::ParamType,Activity_Location_Components::Concepts::Is_Activity_Location)*/))
			{
				person_itf* _Parent_Person = base_type::_Parent_Planner->Parent_Person<person_itf*>();

				// external knowledge references
				_Network_Interface* network = _Parent_Person->template network_reference<_Network_Interface*>();
				_Zones_Container_Interface* zones = network->zones_container<_Zones_Container_Interface*>();
				_Activity_Locations_Container_Interface* locations = network->activity_locations_container<_Activity_Locations_Container_Interface*>();
				_Skim_Interface* LOS = network->skimming_faculty<_Skim_Interface*>();

				// get reference to origin zone
				_Activity_Location_Interface* orig = (_Activity_Location_Interface*)origin;
				_Activity_Location_Interface* dest = (_Activity_Location_Interface*)destination;

				// variables used for utility calculation
				float ttime, pop, emp, u;
				float utility_sum = 0;
				float prob_sum = 0;
				_Zone_Interface* zone;

				// select zones to choose from and estimate utility
				zone = dest->template zone<_Zone_Interface*>();

				ttime = LOS->Get_LOS<Target_Type<NULLTYPE,Time_Minutes,int,Vehicle_Components::Types::Vehicle_Type_Keys>>(orig->zone<_Zone_Interface*>()->uuid<int>(),zone->uuid<int>(),Vehicle_Components::Types::Vehicle_Type_Keys::SOV);
				pop = zone->population<float>();
				emp = zone->employment<float>();

				u = exp(_B_POPULATION * pop + _B_EMPLOYMENT * emp - _B_TTIME * ttime);
				return (typename TargetType::ReturnType)u;
				return 0;				
			}
			tag_feature_as_available(Calculate_Utility);


		};
		// INITIALIZE DESTINATION MODEL STATIC PARAMETERS
		static_member_definition(Person_Components::Implementations::CTRAMP_Destination_Choice_Implementation,choice_set_size);
		static_member_definition(Person_Components::Implementations::CTRAMP_Destination_Choice_Implementation,B_TTIME);
		static_member_definition(Person_Components::Implementations::CTRAMP_Destination_Choice_Implementation,B_EMPLOYMENT);
		static_member_definition(Person_Components::Implementations::CTRAMP_Destination_Choice_Implementation,B_POPULATION);
	}
}

