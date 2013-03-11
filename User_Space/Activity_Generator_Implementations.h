#pragma once

#include "Person_Prototype.h"

namespace Person_Components
{
	namespace Implementations
	{
		//==================================================================================
		/// Planning classes
		//----------------------------------------------------------------------------------
		implementation struct General_Activity_Generator_Implementation : public Polaris_Component<APPEND_CHILD(General_Activity_Generator_Implementation),MasterType,Data_Object,ParentType>
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
			define_container_and_value_interface_unqualified_container(Activity_Plans,Activity_Plan, typename type_of(Parent_Planner)::type_of(Activity_Plans_Container),Containers::Back_Insertion_Sequence_Prototype,Activity_Components::Prototypes::Activity_Plan_Prototype,ComponentType);
			define_container_and_value_interface_unqualified_container(Movement_Plans,Movement_Plan, typename type_of(Parent_Planner)::type_of(Movement_Plans_Container),Containers::Back_Insertion_Sequence_Prototype,Movement_Plan_Components::Prototypes::Movement_Plan_Prototype,ComponentType);
		
		};

		implementation struct ADAPTS_Activity_Generator_Implementation : public General_Activity_Generator_Implementation<MasterType, ParentType, APPEND_CHILD(ADAPTS_Activity_Generator_Implementation)>
		{
			typedef General_Activity_Generator_Implementation<MasterType, ParentType, APPEND_CHILD(ADAPTS_Activity_Generator_Implementation)> base_type;
			typedef Prototypes::Activity_Generator<base_type,base_type> base_itf;
			define_component_interface(person_itf,typename base_type::type_of(Parent_Planner)::type_of(Parent_Person), Prototypes::Person,ComponentType);

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

			feature_implementation void Activity_Generation()
			{
				// Create alias for this to use in conditional

				// Generate average of 4 activities per day

				define_container_and_value_interface(Activity_Plans,Activity_Plan,typename base_type::type_of(Activity_Plans_Container),Associative_Container_Prototype,Activity_Components::Prototypes::Activity_Plan_Prototype,ComponentType);
				Activity_Plan* act = (Activity_Plan*)Allocate<typename base_type::type_of(Activity_Plans_Container)::unqualified_value_type>();
				act->template Activity_Plan_ID<long>(_iteration);
				Activity_Plans* activities = this_ptr->template Activity_Plans_Container<Activity_Plans*>();
				activities->insert(Simulation_Time.Future_Time<Time_Minutes,Simulation_Timestep_Increment>(15),act);

				define_container_and_value_interface(Movement_Plans,Movement_Plan,typename base_type::type_of(Movement_Plans_Container),Associative_Container_Prototype,Movement_Plan_Components::Prototypes::Movement_Plan_Prototype,ComponentType);
				Movement_Plan* move = (Movement_Plan*)Allocate<typename base_type::type_of(Movement_Plans_Container)::unqualified_value_type>();
				move->template Movement_Plan_ID<long>(_iteration);
				Movement_Plans* movements = this_ptr->template Movement_Plans_Container<Movement_Plans*>();
				movements->insert(Simulation_Time.Future_Time<Time_Minutes,Simulation_Timestep_Increment>(30),move);
			}
			tag_feature_as_available(Activity_Generation);
		};

		implementation struct CTRAMP_Activity_Generator_Implementation : public General_Activity_Generator_Implementation<MasterType, ParentType, APPEND_CHILD(CTRAMP_Activity_Generator_Implementation)>
		{
			// IMPLEMENTATION TYPEDEFS AND INTERFACES
			typedef General_Activity_Generator_Implementation<MasterType, ParentType, APPEND_CHILD(CTRAMP_Activity_Generator_Implementation)> base_type;
			typedef base_type base;
			typedef Prototypes::Activity_Generator<base_type,base_type> base_itf;
			
			// Interface definitions
			define_component_interface(person_itf,typename base_type::type_of(Parent_Planner)::type_of(Parent_Person), Prototypes::Person,ComponentType);
			define_component_interface(_Scenario_Interface, typename type_of(Parent_Planner)::type_of(Parent_Person)::type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
			define_component_interface(_Network_Interface, typename type_of(Parent_Planner)::type_of(Parent_Person)::type_of(network_reference), Network_Components::Prototypes::Network_Prototype, ComponentType);	
			define_component_interface(_Skim_Interface, typename _Network_Interface::get_type_of(skimming_faculty),Network_Skimming_Components::Prototypes::Network_Skimming_Prototype,ComponentType);
			define_container_and_value_interface(_Activity_Locations_Container_Interface, _Activity_Location_Interface, typename _Network_Interface::get_type_of(activity_locations_container), Random_Access_Sequence_Prototype, Activity_Location_Components::Prototypes::Activity_Location_Prototype, ComponentType);
			define_container_and_value_interface(_Links_Container_Interface, _Link_Interface, typename _Activity_Location_Interface::get_type_of(origin_links), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
			define_container_and_value_interface(_Zones_Container_Interface, _Zone_Interface, typename _Network_Interface::get_type_of(zones_container), Associative_Container_Prototype, Zone_Components::Prototypes::Zone_Prototype, ComponentType);
			define_container_and_value_interface_unqualified_container(Activity_Plans,Activity_Plan, typename type_of(Parent_Planner)::type_of(Activity_Plans_Container),Containers::Back_Insertion_Sequence_Prototype,Activity_Components::Prototypes::Activity_Plan_Prototype,ComponentType);
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

			feature_implementation void Activity_Generation()
			{
				person_itf* _Parent_Person = base_type::_Parent_Planner->Parent_Person<person_itf*>();

				// get references to the plan containers
				Activity_Plans* activities = typename base_type::_Parent_Planner->template Activity_Plans_Container<Activity_Plans*>();
				Movement_Plans* movements = typename base_type::_Parent_Planner->template Movement_Plans_Container<Movement_Plans*>();	

				// external knowledge references
				_Network_Interface* network = _Parent_Person->template network_reference<_Network_Interface*>();
				_Scenario_Interface* scenario = _Parent_Person->template scenario_reference<_Scenario_Interface*>();
				_Activity_Location_Interface *orig, *dest;

				// Generate average of 2 activities per day
				int num_activities = 2;
				for (int i = 0; i < num_activities; i++)
				{
					// Create movement plan and give it an ID
					Movement_Plan* move = (Movement_Plan*)Allocate<typename base_type::type_of(Parent_Planner)::type_of(Movement_Plans_Container)::unqualified_value_type>();
					move->template initialize_trajectory<NULLTYPE>();

					// Get the origin and destination locations
					orig = _Parent_Person->Home_Location<_Activity_Location_Interface*>();
					dest = this->Destination_Choice<ComponentType,CallerType,_Activity_Location_Interface*>(orig);

					// check that origin and destination are valid
					if (orig != nullptr && dest != nullptr) 
					{
						// If the trip is valid, assign to a movement plan and add to the schedule
						if (orig->internal_id<int>() != dest->internal_id<int>())
						{
							// Get the departure time from current time, in minutes, normally distributed around 7AM and 5pm
							double t = Normal_RNG.Next_Rand<double>(7.0*(i+1) + 3.0*i, 3.0+i);					
							Basic_Units::Time_Variables::Time_Seconds time = Simulation_Time.Future_Time<Basic_Units::Time_Variables::Time_Hours, Basic_Units::Time_Variables::Time_Seconds>(t);

							// add attributes to plan
							move->template origin<_Activity_Location_Interface*>(orig);
							move->template destination<_Activity_Location_Interface*>(dest);
							move->template origin<_Link_Interface*>(orig->origin_links<_Links_Container_Interface&>().at(0));
							move->template destination<_Link_Interface*>(dest->origin_links<_Links_Container_Interface&>().at(0));
							move->template departed_time<Basic_Units::Time_Variables::Time_Seconds>(time);

							// Add to plans schedule
							_Parent_Planner->template Add_Movement_Plan<Movement_Plan*>(move);
						}
					}
				}
			}
			tag_feature_as_available(Activity_Generation);

			feature_implementation TargetType Destination_Choice(TargetType origin, requires(check_as_given(TargetType,is_pointer) && check(TargetType,Activity_Location_Components::Concepts::Is_Activity_Location)))
			{
				person_itf* _Parent_Person = base_type::_Parent_Planner->Parent_Person<person_itf*>();

				const int set_size=50;

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
				float ttime, pop, emp, u;
				float utility_sum = 0;
				float prob_sum = 0;
				_Zone_Interface* zone;

				// select zones to choose from and estimate utility
				for (int i=0; i<set_size; i++)
				{
					loc_id = (int)((Uniform_RNG.Next_Rand<float>()*0.999999)*size);
					_Activity_Location_Interface* loc = locations->at(loc_id);
					loc_options.push_back(loc);
					zone = loc->zone<_Zone_Interface*>();

					ttime = LOS->Get_LOS<Target_Type<NULLTYPE,Time_Minutes,int,Vehicle_Components::Types::Vehicle_Type_Keys>>(orig->zone<_Zone_Interface*>()->uuid<int>(),zone->uuid<int>(),Vehicle_Components::Types::Vehicle_Type_Keys::SOV);
					pop = zone->population<float>();
					emp = zone->employment<float>();

					u = exp(0.00005*pop + 0.0002*emp - 0.1*ttime);
					utility.push_back(u);
					utility_sum += u;
				}
				if (utility_sum == 0) return nullptr;
				
				// convert to cumulative probability
				for (int i=0; i<set_size; i++)
				{
					prob_sum += utility[i] / utility_sum;
					cum_probability.push_back(prob_sum);
				}

				// select one of the zones through monte-carlo
				float r = Uniform_RNG.Next_Rand<float>();
				for (int i=0; i<set_size; i++)
				{
					if (r <= cum_probability[i])
					{
						return loc_options[i];
					}
				}
				return nullptr;

			}
			tag_feature_as_available(Destination_Choice);

		};
	

	}
}