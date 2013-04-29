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
			define_container_and_value_interface_unqualified_container(Activity_Plans,Activity_Plan, typename type_of(Parent_Planner)::type_of(Activity_Container),Containers::Back_Insertion_Sequence_Prototype,Activity_Components::Prototypes::Activity_Planner,ComponentType);
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
				static_assert(false,"Do not use");

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

			// static discretionary activity generation model
			static float eat_out_activity_freq[8];
			static float errands_activity_freq[8];
			static float healthcare_activity_freq[8];
			static float leisure_activity_freq[8];
			static float major_shopping_activity_freq[8];
			static float other_activity_freq[8];
			static float other_shopping_activity_freq[8];
			static float personal_business_activity_freq[8];
			static float religious_or_civic_activity_freq[8];
			static float service_vehicle_activity_freq[8];
			static float social_activity_freq[8];

			static int Generator_Count_Array[_num_threads];
			static int Generator_Count;

			
			// Interface definitions
			define_component_interface(_planner_itf,typename type_of(Parent_Planner),Prototypes::Person_Planner,ComponentType);
			define_component_interface(_Destination_Choice_Itf, typename type_of(Parent_Planner)::type_of(Destination_Chooser), Prototypes::Destination_Chooser, ComponentType);
			define_component_interface(person_itf,typename base_type::type_of(Parent_Planner)::type_of(Parent_Person), Prototypes::Person,ComponentType);
			define_component_interface(_static_properties_itf,typename base_type::type_of(Parent_Planner)::type_of(Parent_Person)::type_of(Static_Properties), Prototypes::Person_Properties,ComponentType);
			define_component_interface(_properties_itf,typename base_type::type_of(Parent_Planner)::type_of(Parent_Person)::type_of(Properties), Prototypes::Person_Properties,ComponentType);
			define_component_interface(_Scenario_Interface, typename type_of(Parent_Planner)::type_of(Parent_Person)::type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
			define_component_interface(_Network_Interface, typename type_of(Parent_Planner)::type_of(Parent_Person)::type_of(network_reference), Network_Components::Prototypes::Network_Prototype, ComponentType);	
			define_component_interface(_Skim_Interface, typename _Network_Interface::get_type_of(skimming_faculty),Network_Skimming_Components::Prototypes::Network_Skimming_Prototype,ComponentType);
			define_container_and_value_interface(_Activity_Locations_Container_Interface, _Activity_Location_Interface, typename _Network_Interface::get_type_of(activity_locations_container), Random_Access_Sequence_Prototype, Activity_Location_Components::Prototypes::Activity_Location_Prototype, ComponentType);
			define_container_and_value_interface(_Links_Container_Interface, _Link_Interface, typename _Activity_Location_Interface::get_type_of(origin_links), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
			define_container_and_value_interface(_Zones_Container_Interface, _Zone_Interface, typename _Network_Interface::get_type_of(zones_container), Associative_Container_Prototype, Zone_Components::Prototypes::Zone_Prototype, ComponentType);
			define_container_and_value_interface_unqualified_container(Activities, Activity, typename type_of(Parent_Planner)::type_of(Activity_Container),Containers::Back_Insertion_Sequence_Prototype,Activity_Components::Prototypes::Activity_Planner,ComponentType);
			typedef Activity_Components::Prototypes::Activity_Planner<typename MasterType::routine_activity_plan_type,ComponentType> Routine_Activity_Plan;
			typedef Activity_Components::Prototypes::Activity_Planner<typename MasterType::activity_plan_type,ComponentType> Activity_Plan;
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
				// updates for counters
				this->Generator_Count_Array[_thread_id]++;
				if (this->Generator_Count_Array[_thread_id] % 10000 == 0)  
				{
					//LOCK(this->_update_lock);
					this->Generator_Count+=10000;
					cout << '\r' << "Activity Generation: " << this->Generator_Count << "                                 ";
					//UNLOCK(this->_update_lock);
				}
				
				person_itf* _Parent_Person = base_type::_Parent_Planner->Parent_Person<person_itf*>();
				_static_properties_itf* static_properties = _Parent_Person->Static_Properties<_static_properties_itf*>();
				_properties_itf* properties = _Parent_Person->Properties<_properties_itf*>();
				_Destination_Choice_Itf* destination_chooser = base_type::_Parent_Planner->Destination_Chooser<_Destination_Choice_Itf*>();
				
				// get references to the plan containers
				Activities* activities = typename base_type::_Parent_Planner->template Activity_Container<Activities*>();
				Movement_Plans* movements = typename base_type::_Parent_Planner->template Movement_Plans_Container<Movement_Plans*>();	

				// external knowledge references
				_Network_Interface* network = _Parent_Person->template network_reference<_Network_Interface*>();
				_Scenario_Interface* scenario = _Parent_Person->template scenario_reference<_Scenario_Interface*>();

				int act_count = 1;

				//=========================================================================================================================
				// Get random start plan time inthe first minute for generation
				int start_plan_time = (int)(GLOBALS::Uniform_RNG.Next_Rand<float>()*60.0f) + _iteration;
				

				//=========================================================================================================================
				// Generate work activity
				EMPLOYMENT_STATUS work_status = static_properties->Employment_Status<EMPLOYMENT_STATUS>();
				if (work_status == EMPLOYMENT_STATUS::EMPLOYMENT_STATUS_CIVILIAN_AT_WORK || work_status == EMPLOYMENT_STATUS::EMPLOYMENT_STATUS_ARMED_FORCES_AT_WORK) Create_Routine_Activity<ComponentType,CallerType,ACTIVITY_TYPES>(PRIMARY_WORK_ACTIVITY,act_count, start_plan_time);
				//-------------------------------------------------------------------------------------------------------------------------


				//=========================================================================================================================
				// Generate school activity
				Person_Components::Types::SCHOOL_ENROLLMENT sch_status = static_properties->School_Enrollment<SCHOOL_ENROLLMENT>();
				//if (sch_status == SCHOOL_ENROLLMENT::ENROLLMENT_PUBLIC || sch_status == SCHOOL_ENROLLMENT::ENROLLMENT_PRIVATE) Create_Routine_Activity<ComponentType,CallerType,ACTIVITY_TYPES>(SCHOOL_ACTIVITY,act_count, start_plan_time);
				//-------------------------------------------------------------------------------------------------------------------------

				int person_index = this->Person_Type_index<ComponentType,CallerType,NT>();

				//=========================================================================================================================
				// Get frequency of each activity type
				float num_eat_out = eat_out_activity_freq[person_index];
				float num_errand = errands_activity_freq[person_index];
				float num_healthcare = healthcare_activity_freq[person_index];
				float num_leisure = leisure_activity_freq[person_index];
				float num_maj_shop = major_shopping_activity_freq[person_index];
				float num_other = other_activity_freq[person_index];
				float num_other_shop = other_shopping_activity_freq[person_index];
				float num_pb = personal_business_activity_freq[person_index];
				float num_civic = religious_or_civic_activity_freq[person_index];
				float num_service = service_vehicle_activity_freq[person_index];
				float num_social = social_activity_freq[person_index];

				//if (GLOBALS::Uniform_RNG.Next_Rand<float>() < num_eat_out ) Create_Activity<ComponentType,CallerType,ACTIVITY_TYPES>(EAT_OUT_ACTIVITY,act_count, start_plan_time);
				//if (GLOBALS::Uniform_RNG.Next_Rand<float>() < num_errand ) Create_Activity<ComponentType,CallerType,ACTIVITY_TYPES>(ERRANDS_ACTIVITY,act_count, start_plan_time);
				//if (GLOBALS::Uniform_RNG.Next_Rand<float>() < num_healthcare ) Create_Activity<ComponentType,CallerType,ACTIVITY_TYPES>(HEALTHCARE_ACTIVITY,act_count, start_plan_time);
				//if (GLOBALS::Uniform_RNG.Next_Rand<float>() < num_leisure ) Create_Activity<ComponentType,CallerType,ACTIVITY_TYPES>(LEISURE_ACTIVITY,act_count, start_plan_time);
				//if (GLOBALS::Uniform_RNG.Next_Rand<float>() < num_maj_shop ) Create_Activity<ComponentType,CallerType,ACTIVITY_TYPES>(MAJOR_SHOPPING_ACTIVITY,act_count, start_plan_time);
				////if (GLOBALS::Uniform_RNG.Next_Rand<float>() < num_other ) Create_Activity<ComponentType,CallerType,ACTIVITY_TYPES>(OTHER_ACTIVITY,act_count);
				//if (GLOBALS::Uniform_RNG.Next_Rand<float>() < num_other_shop ) Create_Activity<ComponentType,CallerType,ACTIVITY_TYPES>(OTHER_SHOPPING_ACTIVITY,act_count, start_plan_time);
				//if (GLOBALS::Uniform_RNG.Next_Rand<float>() < num_pb ) Create_Activity<ComponentType,CallerType,ACTIVITY_TYPES>(PERSONAL_BUSINESS_ACTIVITY,act_count, start_plan_time);
				//if (GLOBALS::Uniform_RNG.Next_Rand<float>() < num_civic ) Create_Activity<ComponentType,CallerType,ACTIVITY_TYPES>(RELIGIOUS_OR_CIVIC_ACTIVITY,act_count, start_plan_time);
				//if (GLOBALS::Uniform_RNG.Next_Rand<float>() < num_service ) Create_Activity<ComponentType,CallerType,ACTIVITY_TYPES>(SERVICE_VEHICLE_ACTIVITY,act_count, start_plan_time);
				//if (GLOBALS::Uniform_RNG.Next_Rand<float>() < num_social ) Create_Activity<ComponentType,CallerType,ACTIVITY_TYPES>(SOCIAL_ACTIVITY,act_count, start_plan_time);

			}
			tag_feature_as_available(Activity_Generation);

			feature_implementation int Person_Type_index()
			{
				person_itf* _Parent_Person = base_type::_Parent_Planner->Parent_Person<person_itf*>();
				_static_properties_itf* per = _Parent_Person->Static_Properties<_static_properties_itf*>();

				int age = per->Age<int>();
				Time_Hours hours = per->Work_Hours<Time_Hours>();
				bool student = per->Is_Student<bool>();
				bool employed = per->Is_Employed<bool>();

				int index = -1;
				if (employed && hours >= 30) index = 3;		// full time
				else if (employed) index = 4;				// part time
				else if (student && age > 18) index = 7;	// adult student
				else if (age >= 65) index = 6;				// senior
				else if (age < 65 && age > 18) index = 5;	// adult non-worker
				else if (age >= 16 && age <= 18) index = 0;	// driving age school child
				else if (age < 16 && age >= 5) index = 1;	// school child
				else if (age < 5) index = 2;				// pre- school age child
				else index = 5;

				return index;
			}

			feature_implementation void Create_Routine_Activity(TargetType act_type, int& activity_count, int start_plan_time)
			{
				Routine_Activity_Plan* activity = (Routine_Activity_Plan*)Allocate<typename MasterType::routine_activity_plan_type>();
				activity->Parent_Planner<_planner_itf*>(_Parent_Planner);
				activity->Activity_Plan_ID<int>(activity_count);
	
				// Activity planning time
				Simulation_Timestep_Increment plan_time = start_plan_time + activity_count;
				activity->Initialize<Target_Type<NT,NT, ACTIVITY_TYPES, Simulation_Timestep_Increment> >(act_type, plan_time);

				((base_type*)this)->_Parent_Planner->Add_Activity_Plan<Routine_Activity_Plan*>(activity);
				//Activities* activities = typename base_type::_Parent_Planner->template Activity_Container<Activities*>();
				//activities->push_back((Activity*)activity);
				activity_count++;
			}
			feature_implementation void Create_Activity(TargetType act_type, int& activity_count, int start_plan_time)
			{
				Activity_Plan* activity = (Activity_Plan*)Allocate<typename MasterType::activity_plan_type>();
				activity->Parent_Planner<_planner_itf*>(_Parent_Planner);
				activity->Activity_Plan_ID<int>(activity_count);


				Simulation_Timestep_Increment _plan_time = start_plan_time + activity_count;
				activity->Initialize<Target_Type<NT,NT, ACTIVITY_TYPES, Simulation_Timestep_Increment> >(act_type, _plan_time);

				((base_type*)this)->_Parent_Planner->Add_Activity_Plan<Activity_Plan*>(activity);
				//Activities* activities = typename base_type::_Parent_Planner->template Activity_Container<Activities*>();
				//activities->push_back((Activity*)activity);


				
				activity_count++;
			}

		};
		template<typename MasterType,typename ParentType, typename InheritanceList> float CTRAMP_Activity_Generator_Implementation<MasterType, ParentType, InheritanceList>::eat_out_activity_freq[]= {0.134,0.125,0.158,0.260,0.236,0.234,0.248,0.178};
		template<typename MasterType,typename ParentType, typename InheritanceList> float CTRAMP_Activity_Generator_Implementation<MasterType, ParentType, InheritanceList>::errands_activity_freq[]= {0.033,0.052,0.093,0.108,0.156,0.198,0.181,0.070};
		template<typename MasterType,typename ParentType, typename InheritanceList> float CTRAMP_Activity_Generator_Implementation<MasterType, ParentType, InheritanceList>::healthcare_activity_freq[]= {0.030,0.034,0.053,0.051,0.070,0.110,0.142,0.046};
		template<typename MasterType,typename ParentType, typename InheritanceList> float CTRAMP_Activity_Generator_Implementation<MasterType, ParentType, InheritanceList>::leisure_activity_freq[]= {0.198,0.271,0.234,0.161,0.213,0.236,0.184,0.206};
		template<typename MasterType,typename ParentType, typename InheritanceList> float CTRAMP_Activity_Generator_Implementation<MasterType, ParentType, InheritanceList>::major_shopping_activity_freq[]= {0.007,0.014,0.022,0.028,0.039,0.041,0.041,0.015};
		template<typename MasterType,typename ParentType, typename InheritanceList> float CTRAMP_Activity_Generator_Implementation<MasterType, ParentType, InheritanceList>::other_activity_freq[]= {0.008,0.016,0.021,0.027,0.023,0.034,0.031,0.028};
		template<typename MasterType,typename ParentType, typename InheritanceList> float CTRAMP_Activity_Generator_Implementation<MasterType, ParentType, InheritanceList>::other_shopping_activity_freq[]= {0.174,0.203,0.320,0.359,0.484,0.658,0.598,0.302};
		template<typename MasterType,typename ParentType, typename InheritanceList> float CTRAMP_Activity_Generator_Implementation<MasterType, ParentType, InheritanceList>::personal_business_activity_freq[]= {0.117,0.082,0.110,0.097,0.133,0.206,0.167,0.197};
		template<typename MasterType,typename ParentType, typename InheritanceList> float CTRAMP_Activity_Generator_Implementation<MasterType, ParentType, InheritanceList>::religious_or_civic_activity_freq[]= {0.059,0.069,0.033,0.049,0.074,0.113,0.103,0.043};
		template<typename MasterType,typename ParentType, typename InheritanceList> float CTRAMP_Activity_Generator_Implementation<MasterType, ParentType, InheritanceList>::service_vehicle_activity_freq[]= {0.021,0.023,0.043,0.080,0.076,0.077,0.076,0.061};
		template<typename MasterType,typename ParentType, typename InheritanceList> float CTRAMP_Activity_Generator_Implementation<MasterType, ParentType, InheritanceList>::social_activity_freq[]= {0.227,0.188,0.215,0.129,0.211,0.225,0.189,0.200};
		template<typename MasterType,typename ParentType, typename InheritanceList> int CTRAMP_Activity_Generator_Implementation<MasterType, ParentType, InheritanceList>::Generator_Count_Array[];
		template<typename MasterType,typename ParentType, typename InheritanceList> int CTRAMP_Activity_Generator_Implementation<MasterType, ParentType, InheritanceList>::Generator_Count;
	}
}