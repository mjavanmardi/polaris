#pragma once

//#include "User_Space_Includes.h"
#include "Activity_Prototype.h"
#include "Activity_Location_Prototype.h"
#include "Network_Prototype.h"
#include "Zone_Prototype.h"
#include "Movement_Plan_Prototype.h"
#include "Network_Skimming_Prototype.h"
#include "Population_Unit_Prototype.h"


namespace Person_Components
{

namespace Concepts
{
	concept struct Is_Person
	{
		check_feature(Has_Initialize_Defined,Initialize);
		check_getter(Has_Properties_Defined,Properties);
		check_getter(Has_Planner_Defined,Planning_Faculty);
		define_default_check(Has_Initialize_Defined && Has_Properties_Defined && Has_Planner_Defined);
	};

	concept struct Is_Transims_Person
	{
		check_concept(Is_Person_Check, Is_Person);
		check_typename_state(Is_Transims_Check,Is_Transims,true_type);
		define_default_check(Is_Person_Check && Is_Transims_Check);
	};
	concept struct Is_CTRAMP_Person
	{
		check_concept(Is_Person_Check, Is_Person);
		check_typename_state(Is_CTRAMP_Check,Is_Transims,true_type);
		define_default_check(Is_Person_Check && Is_CTRAMP_Check);
	};

	concept struct Has_Initialize
	{
		check_feature(Has_Initialize_Defined,Initialize);
		define_default_check(Has_Initialize_Defined);
	};

}

namespace Types
{
	enum PLANNING_ITERATION_STEP_KEYS
	{
		ACTIVITY_GENERATION=1,
		ACTIVITY_PLANNING=2,
		MOVEMENT_PLANNING=3,
	};

	namespace Census_Person_Characteristics
	{

	enum GENDER
	{
		NONE,MALE=1,FEMALE
	};
	enum RACE
	{
		WHITE=1,BLACK,INDIAN,ASIAN,PACIFIC_ISLANDER,OTHER_RACE,TWO_OR_MORE_RACES,HISPANIC
	};
	enum WORKER_CLASS
	{
		EMPLOYEE_PRIVATE=1,EMPLOYEE_NON_PROFIT,LOCAL,STATE,FEDERAL,SELF_EMPLOYED,SELF_EMPLOYED_INCORPORATED,FAMILY_BUSINESS_UNPAID,UNEMPLOYED
	};
	enum WORK_TRAVEL_MODE
	{
		CAR=1,BUS,STREETCAR,SUBWAY,RAILROAD,FERRY,TAXI,MOTORCYCLE,BIKE,WALK,WORK_AT_HOME,OTHER
	};
	enum MARITAL_STATUS
	{
		MARRIED=1,WIDOWED,DIVORCED,SEPARATED,NEVER_MARRIED
	};
	enum HH_RELATIONSHIP
	{
		REFERENCE_PERSON,SPOUSE,CHILD,ADOPTED_CHILD,STEPCHILD,SIBLING,PARENT,GRANDCHILD,PARENT_IN_LAW,CHILD_IN_LAW,OTHER_RELATIVE,BOARDER,ROOMATE,PARTNER,FOSTER_CHILD,OTHER_UNRELATED,GQ_INSTITUTIONAL,GQ_NONINSTITUTIONAL
	};
	enum SCHOOL_ATTENDANCE
	{
		NO,YES_PUBLIC,YES_PRIVATE
	};
	enum SCHOOL_GRADE
	{
		NO_SCHOOL,PRESCHOOL,KINDERGARTEN,GRADE_1,GRADE_2,GRADE_3,GRADE_4,GRADE_5,GRADE_6,GRADE_7,GRADE_8,GRADE_9,GRADE_10,GRADE_11,GRADE_12,UNDERGRADUATE,GRADUATE_SCHOOL
	};
	enum EDUCATIONAL_ATTAINMENT
	{
		EDUC_NA,EDUC_NONE,EDUC_PRESCHOOL,EDUC_KINDERGARTEN,EDUC_GRADE_1,EDUC_GRADE_2,EDUC_GRADE_3,EDUC_GRADE_4,EDUC_GRADE_5,EDUC_GRADE_6,EDUC_GRADE_7,EDUC_GRADE_8,EDUC_GRADE_9,EDUC_GRADE_10,EDUC_GRADE_11,EDUC_GRADE_12_NO_DIPLOMA,
		EDUC_HIGH_SCHOOL_DIPLOMA,EDUC_GED,EDUC_SOME_COLLEGE_UNDER_1YR,EDUC_SOME_COLLEGE_OVER_1YR,EDUC_ASSOCIATE_DEGREE,EDUC_BACHELOR_DEGREE,EDUC_MASTERS_DEGREE,EDUC_PROFESSIONAL_DEGREE,EDUC_DOCTORATE
	};


	}
}

namespace Variables
{

}

namespace Prototypes
{
	prototype struct Person
	{
		tag_as_prototype;

		// Event handling
		declare_feature_conditional(Agent_Conditional)
		{
			response.next._iteration = Simulation_Time.Future_Time<Time_Minutes,Simulation_Timestep_Increment>(15);
			response.next._sub_iteration = 0;
			response.result = true;
		}
		declare_feature_event(Agent_Event)
		{
			typedef Person<ComponentType, ComponentType> _Person_Interface;
			ComponentType* _pthis = (ComponentType*)_this;
			_Person_Interface* pthis =(_Person_Interface*)_pthis;
			
			define_component_interface(_network_itf,typename get_type_of(network_reference),Network_Components::Prototypes::Network_Prototype,CallerType);
			_network_itf* network = pthis->network_reference<_network_itf*>();
		}

		// Initializer
		feature_prototype void Initialize(TargetType id, requires(check(ComponentType,Concepts::Has_Initialize)))
		{
			this_component()->Initialize<ComponentType,CallerType, TargetType>(id);
			load_event(ComponentType,Agent_Conditional,Agent_Event,this->First_Iteration<Simulation_Timestep_Increment>(),0,NULLTYPE);
		}
		feature_prototype void Initialize(TargetType id, requires(!check(ComponentType,Concepts::Has_Initialize)))
		{
			assert_check(ComponentType,Concepts::Has_Initialize,"This ComponentType is not a valid Agent, does not have an initializer.   Did you forget to use tag_feature_as_available macro?");
		}
		feature_prototype void Initialize(typename TargetType::ParamType id, typename TargetType::Param2Type trip, requires(check(ComponentType,Concepts::Has_Initialize)))
		{
			this_component()->Initialize<ComponentType,CallerType, TargetType>(id, trip);
			load_event(ComponentType,Agent_Conditional,Agent_Event,this->First_Iteration<Simulation_Timestep_Increment>(),0,NULLTYPE);
		}
		feature_prototype void Initialize(typename TargetType::ParamType id, typename TargetType::Param2Type trip, requires(!check(ComponentType,Concepts::Has_Initialize)))
		{
			assert_check(ComponentType,Concepts::Has_Initialize,"This ComponentType is not a valid Agent, does not have an initializer.   Did you forget to use tag_feature_as_available macro?");
		}

		// Sub-component accessors	
		feature_accessor(Planning_Faculty,none,none);
		feature_accessor(router,none,none);
		feature_accessor(Moving_Faculty,none,none);
		feature_accessor(Properties,none,none);
		feature_accessor(Static_Properties,none,none);
		feature_accessor(vehicle,none,none);

		// External knowledge accessor
		feature_accessor(network_reference,none,none);
		feature_accessor(scenario_reference,none,none);

		// Basic property members
		feature_accessor(uuid,none,none);
		feature_accessor(internal_id,none,none);
		feature_accessor(Next_Rand,none,none);
		feature_accessor(First_Iteration,none,none);
		feature_accessor(Write_Activity_Files,none,none);

		// Accessors for setting the home/work locations (stores only an index into the network_reference::activity_locations_container) - overloaded to return either th loc_index, the location interface or the zone interface
		feature_prototype TargetType Home_Location(requires(check(TargetType, Activity_Location_Components::Concepts::Is_Activity_Location) && check_as_given(TargetType,is_pointer)))
		{
			define_component_interface(properties_itf,typename get_type_of(Properties),Person_Properties,ComponentType);
			define_component_interface(network_itf, typename get_type_of(network_reference),Network_Components::Prototypes::Network_Prototype,ComponentType);
			define_container_and_value_interface(activity_locations_container_itf, activity_location_itf, typename network_itf::get_type_of(activity_locations_container), Containers::Random_Access_Sequence_Prototype,Activity_Location_Components::Prototypes::Activity_Location_Prototype,ComponentType);
			properties_itf* properties = this->Properties<properties_itf*>();
			network_itf* network = this->network_reference<network_itf*>();
			activity_locations_container_itf* locations = network->template activity_locations_container<activity_locations_container_itf*>();
			
			int loc_id = properties->home_location_id<int>();
			return (TargetType)(*locations)[loc_id];						
		}
		feature_prototype TargetType Home_Location(requires(check(TargetType, Zone_Components::Concepts::Is_Zone) && check_as_given(TargetType,is_pointer)))
		{
			define_component_interface(properties_itf,typename get_type_of(Properties),Person_Properties,ComponentType);
			define_component_interface(network_itf, typename get_type_of(network_reference),Network_Components::Prototypes::Network_Prototype,ComponentType);
			define_container_and_value_interface(activity_locations_container_itf, activity_location_itf, typename network_itf::get_type_of(activity_locations_container), Containers::Random_Access_Sequence_Prototype,Activity_Location_Components::Prototypes::Activity_Location_Prototype,ComponentType);
			//define_container_and_value_interface(zone_container_itf, zone_itf, typename network_itf::get_type_of(zones_container), Containers::Random_Access_Sequence_Prototype,Zone_Components::Prototypes::Zone_Prototype,ComponentType);
			properties_itf* properties = this->Properties<properties_itf*>();
			network_itf* network = this->network_reference<network_itf*>();
			activity_locations_container_itf* locations = network->template activity_locations_container<activity_locations_container_itf*>();
			//zone_container_itf* zones = network->template zones_container<zone_container_itf*>();
			int loc_id = properties->template home_location_id<int>();
			activity_location_itf* loc = (*locations)[loc_id];	
			return loc->zone<TargetType>();
		}
		feature_prototype TargetType Home_Location(requires(check(TargetType, is_integral)))
		{
			define_component_interface(properties_itf,typename get_type_of(Properties),Person_Properties,ComponentType);
			properties_itf* properties = this->Properties<properties_itf*>();
			return properties->home_location_id<TargetType>();
		}
		feature_prototype TargetType Home_Location(requires(check(TargetType, !is_integral) && ( (check(TargetType,!Activity_Location_Components::Concepts::Is_Activity_Location) && check(TargetType,!Zone_Components::Concepts::Is_Zone) ) || check_as_given(TargetType,!is_pointer) ) ) )
		{
			assert_check(TargetType,is_integral,"Error, Home_Location can only be requested as an Integral type - which returns location index, or as an Activity_Location refernence type, which returns the actual location.");
		}
		feature_prototype void Home_Location(TargetType location_index, requires(check(TargetType, is_integral)))
		{
			define_component_interface(properties_itf,typename get_type_of(Properties),Person_Properties,ComponentType);
			define_component_interface(network_itf, typename get_type_of(network_reference),Network_Components::Prototypes::Network_Prototype,ComponentType);
			define_container_and_value_interface(activity_locations_container_itf, activity_location_itf, typename network_itf::get_type_of(activity_locations_container), Containers::Random_Access_Sequence_Prototype,Activity_Location_Components::Prototypes::Activity_Location_Prototype,ComponentType);
			properties_itf* properties = this->Properties<properties_itf*>();
			network_itf* network = this->network_reference<network_itf*>();
			activity_locations_container_itf* locations = network->template activity_locations_container<activity_locations_container_itf*>();

			if (location_index < 0 || location_index >= locations->size()) THROW_EXCEPTION("Error: location index "<<location_index<<" does not exist in network locations container.  Index out of range (0,"<<locations->size()<<").");
			properties->home_location_id<TargetType>(location_index);
		}
		feature_prototype void Home_Location(TargetType location_index, requires(check(TargetType, !is_integral)))
		{
			assert_check(TargetType, is_integral, "Error, Home_Location can only be set by passing an integral index from network::activity_locations_container");
		}
		feature_prototype TargetType Work_Location(requires(check(TargetType, Activity_Location_Components::Concepts::Is_Activity_Location) && check_as_given(TargetType,is_pointer)))
		{
			define_component_interface(properties_itf,typename get_type_of(Properties),Person_Properties,ComponentType);
			define_component_interface(network_itf, typename get_type_of(network_reference),Network_Components::Prototypes::Network_Prototype,ComponentType);
			define_container_and_value_interface(activity_locations_container_itf, activity_location_itf, typename network_itf::get_type_of(activity_locations_container), Containers::Random_Access_Sequence_Prototype,Activity_Location_Components::Prototypes::Activity_Location_Prototype,ComponentType);
			properties_itf* properties = this->Properties<properties_itf*>();
			network_itf* network = this->network_reference<network_itf*>();
			activity_locations_container_itf* locations = network->template activity_locations_container<activity_locations_container_itf*>();

			int loc_id = properties->home_location_id<TargetType>();
			return (TargetType)(*locations[loc_id]);						
		}
		feature_prototype TargetType Work_Location(requires(check(TargetType, Zone_Components::Concepts::Is_Zone) && check_as_given(TargetType,is_pointer)))
		{
			define_component_interface(properties_itf,typename get_type_of(Properties),Person_Properties,ComponentType);
			define_component_interface(network_itf, typename get_type_of(network_reference),Network_Components::Prototypes::Network_Prototype,ComponentType);
			define_container_and_value_interface(activity_locations_container_itf, activity_location_itf, typename network_itf::get_type_of(activity_locations_container), Containers::Random_Access_Sequence_Prototype,Activity_Location_Components::Prototypes::Activity_Location_Prototype,ComponentType);
			define_container_and_value_interface(zone_container_itf, zone_itf, typename network_itf::get_type_of(zones_container), Containers::Random_Access_Sequence_Prototype,Zone_Components::Prototypes::Zone_Prototype,ComponentType);
			properties_itf* properties = this->Properties<properties_itf*>();
			network_itf* network = this->network_reference<network_itf*>();
			activity_locations_container_itf* locations = network->template activity_locations_container<activity_locations_container_itf*>();
			zone_container_itf* zones = network->template zones_container<zone_container_itf*>();
			int loc_id = properties->template work_location_id<TargetType>();
			activity_location_itf* loc = locations[loc_id];	
			return loc->zone<zone_itf*>();
		}
		feature_prototype TargetType Work_Location(requires(check(TargetType, is_integral)))
		{
			define_component_interface(properties_itf,typename get_type_of(Properties),Person_Properties,ComponentType);
			properties_itf* properties = this->Properties<properties_itf*>();
			return properties->home_location_id<TargetType>();
		}
		feature_prototype TargetType Work_Location(requires(check(TargetType, !is_integral) && ( check(TargetType,!Activity_Location_Components::Concepts::Is_Activity_Location) || check_as_given(TargetType,!is_pointer) ) ) )
		{
			assert_check(TargetType,is_integral,"Error, Home_Location can only be requested as an Integral type - which returns location index, or as an Activity_Location refernence type, which returns the actual location.");
		}
		feature_prototype void Work_Location(TargetType location_index, requires(check(TargetType, is_integral)))
		{
			define_component_interface(properties_itf,typename get_type_of(Properties),Person_Properties,ComponentType);
			define_component_interface(network_itf, typename get_type_of(network_reference),Network_Components::Prototypes::Network_Prototype,ComponentType);
			define_container_and_value_interface(activity_locations_container_itf, activity_location_itf, typename network_itf::get_type_of(activity_locations_container), Containers::Random_Access_Sequence_Prototype,Activity_Location_Components::Prototypes::Activity_Location_Prototype,ComponentType);
			properties_itf* properties = this->Properties<properties_itf*>();
			network_itf* network = this->network_reference<network_itf*>();
			activity_locations_container_itf* locations = network->template activity_locations_container<activity_locations_container_itf*>();

			if (location_index < 0 || location_index >= locations->size()) THROW_EXCEPTION("Error: location index "<<location_index<<" does not exist in network locations container.");
			properties->home_location_id<TargetType>(location_index);
		}
		feature_prototype void Work_Location(TargetType location_index, requires(check(TargetType, !is_integral)))
		{
			assert_check(TargetType, is_integral, "Error, work location can only be set by passing an integral index from network::activity_locations_container");
		}
	};


	prototype struct Person_Properties
	{
		tag_as_prototype;

		// Initializer
		define_feature_exists_check(Initialize,Has_Initialize);
		feature_prototype void Initialize(requires(check(ComponentType,Has_Initialize)))
		{
			this_component()->Initialize<ComponentType,CallerType, TargetType>();
		}
		feature_prototype void Initialize(requires(!check(ComponentType,Has_Initialize)))
		{
			assert_check(ComponentType,Has_Initialize,"This ComponentType is not a valid Agent, does not have an initializer.   Did you forget to use tag_feature_as_available macro?");
		}

		feature_accessor(home_location_id,none,none);
		feature_accessor(work_location_id,none,none);
		feature_accessor(My_Length,none,none);
		feature_accessor(My_Area,none,none);
		feature_accessor(My_Time,none,none);
		feature_accessor(My_Speed,none,none);
		feature_accessor(HH_Income,none,none);

		// accessor to parent class
		feature_accessor(Parent_Person,none,none);
	};


	prototype struct Person_Planner
	{
		tag_as_prototype;
		 
		// Conditonal handling and helper functions
		declare_feature_conditional(Planning_Conditional)
		{
			//----------------------------------------------
			// CONDITIONALS FOR BASIC AGENT SCHEDULING
			// 1.) Activity Generation (Occurs 
			// Create alias for this to use in conditional
			typedef Person_Planner<ComponentType, ComponentType> _Planning_Interface;
			ComponentType* _pthis = (ComponentType*)_this;
			_Planning_Interface* this_ptr=(_Planning_Interface*)_pthis;

			// Define interfaces to the container members of the class
			define_container_and_value_interface(Activity_Plans_List,Activity_Plan,typename get_type_of(Activity_Plans_Container),Containers::Back_Insertion_Sequence_Prototype,Activity_Components::Prototypes::Activity_Plan_Prototype,ComponentType);
			define_container_and_value_interface(Movement_Plans_List,Movement_Plan,typename get_type_of(Movement_Plans_Container),Containers::Back_Insertion_Sequence_Prototype,Movement_Plan_Components::Prototypes::Movement_Plan_Prototype,ComponentType);
			Activity_Plans_List* activity_plans = this_ptr->Activity_Plans_Container<Activity_Plans_List*>();
			Movement_Plans_List* movement_plans = this_ptr->Movement_Plans_Container<Movement_Plans_List*>();
			typename Activity_Plans_List::iterator act_itr = activity_plans->begin();
			typename Movement_Plans_List::iterator move_itr = movement_plans->begin();
			Activity_Plan* activity;
			Movement_Plan* movement;			
			if (act_itr != activity_plans->end()) activity = *act_itr;
			if (move_itr != movement_plans->end()) movement = *move_itr;

			//------------------------------------------------------------------------------------------------------------------------------
			// SETUP SUBITERATION, set up future sub_iteration schedule
			if (_sub_iteration == 0)
			{
				// If activity generation is to be performed, do that next
				if(this_ptr->Next_Activity_Generation_Time<Time_Seconds>() == Simulation_Time.Current_Time<Time_Seconds>()) 
				{	
					this_ptr->Go_To_Subiteration<NT>(Types::PLANNING_ITERATION_STEP_KEYS::ACTIVITY_GENERATION,false,response);
				}
				// check if activity planning needs to occur, if so do this
				else if (act_itr != activity_plans->end())
				{
					if (activity->planning_time<Simulation_Timestep_Increment>() < Simulation_Time.Future_Time<Simulation_Timestep_Increment, Simulation_Timestep_Increment>(this_ptr->Planning_Time_Increment<Simulation_Timestep_Increment>()))
						this_ptr->Go_To_Subiteration<NT>(Types::PLANNING_ITERATION_STEP_KEYS::ACTIVITY_PLANNING,false,response);
					else
						this_ptr->Go_To_Subiteration<NT>(Types::PLANNING_ITERATION_STEP_KEYS::MOVEMENT_PLANNING,false,response);
				}
				// check if movement planning needs to occur, if so do this
				else if (move_itr != movement_plans->end())
				{
					if (movement->departed_time<Simulation_Timestep_Increment>() < Simulation_Time.Future_Time<Simulation_Timestep_Increment, Simulation_Timestep_Increment>(this_ptr->Planning_Time_Increment<Simulation_Timestep_Increment>()))
						this_ptr->Go_To_Subiteration<NT>(Types::PLANNING_ITERATION_STEP_KEYS::MOVEMENT_PLANNING,false,response);
					else
						this_ptr->Go_To_Next_Iteration<NT>(false,response);
				}
				// otherwise move on to next main iteration
				else
				{
					this_ptr->Go_To_Next_Iteration<NT>(false,response);
				}
			}

			//------------------------------------------------------------------------------------------------------------------------------
			// ACTIVITY GENERATION SUBITERATION, swap in the activity-generation event and set up future sub_iteration schedule
			else if (_sub_iteration == Types::PLANNING_ITERATION_STEP_KEYS::ACTIVITY_GENERATION)
			{
				_pthis->Swap_Event((Event)&Person_Planner::Activity_Generation_Event<NULLTYPE>);

				// check if activity planning needs to occur, if so do this otherwise, goto movement plans
				if (act_itr != activity_plans->end())
				{
					if (activity->planning_time<Simulation_Timestep_Increment>() < Simulation_Time.Future_Time<Simulation_Timestep_Increment, Simulation_Timestep_Increment>(this_ptr->Planning_Time_Increment<Simulation_Timestep_Increment>()))
						this_ptr->Go_To_Subiteration<NT>(Types::PLANNING_ITERATION_STEP_KEYS::ACTIVITY_PLANNING,true,response);
					else
						this_ptr->Go_To_Subiteration<NT>(Types::PLANNING_ITERATION_STEP_KEYS::MOVEMENT_PLANNING,true,response);
				}
				// check if movement planning needs to occur, if so do this
				else
				{
					this_ptr->Go_To_Subiteration<NT>(Types::PLANNING_ITERATION_STEP_KEYS::MOVEMENT_PLANNING,true,response);
				}
			}

			//------------------------------------------------------------------------------------------------------------------------------
			// ACTIVITY PLANNING SUBITERATION Planning sub_iteration, swap in the activity-planning event and set up future sub_iteration schedule
			else if (_sub_iteration == Types::PLANNING_ITERATION_STEP_KEYS::ACTIVITY_PLANNING)
			{
				_pthis->Swap_Event((Event)&Person_Planner::Activity_Planning_Event<NULLTYPE>);
				this_ptr->Go_To_Subiteration<NT>(Types::PLANNING_ITERATION_STEP_KEYS::MOVEMENT_PLANNING,true,response);
			}

			//------------------------------------------------------------------------------------------------------------------------------
			// MOVEMENT PLANNING SUBITERATION, swap in the movement planning event and set up future sub_iteration schedule
			else if (_sub_iteration == Types::PLANNING_ITERATION_STEP_KEYS::MOVEMENT_PLANNING)
			{
				_pthis->Swap_Event((Event)&Person_Planner::Movement_Planning_Event<NULLTYPE>);
				this_ptr->Go_To_Next_Iteration<NT>(true,response);
			}
			//------------------------------------------------------------------------------------------------------------------------------
			// No valid events scheduled - skip to next iteration
			else
			{
				this_ptr->Go_To_Next_Iteration<NT>(false,response);
			}
		}
		feature_prototype void Go_To_Subiteration(Types::PLANNING_ITERATION_STEP_KEYS sub_iteration, bool do_current_event, Conditional_Response& response)
		{
			response.next._iteration = _iteration;
			response.next._sub_iteration = sub_iteration;
			response.result = do_current_event;
		}
		feature_prototype void Go_To_Next_Iteration(bool do_current_event, Conditional_Response& response)
		{
			response.next._iteration = Round<long,double>(Simulation_Time.Future_Time<Simulation_Timestep_Increment,Simulation_Timestep_Increment>(this->Planning_Time_Increment<Simulation_Timestep_Increment>()));
			response.next._sub_iteration = 0;
			response.result = do_current_event;
		}
		

		// Event handling
		declare_feature_event(Activity_Generation_Event)
		{
			// Create alias for this to use in conditional
			typedef Person_Planner<ComponentType, ComponentType> _Planning_Interface;
			ComponentType* _pthis = (ComponentType*)_this;
			_Planning_Interface* this_ptr=(_Planning_Interface*)_pthis;

			// Call specific implementation of the activity generation routine
			define_component_interface(activity_generator_itf,typename get_type_of(Activity_Generator),Prototypes::Activity_Generator,ComponentType);
			activity_generator_itf* generator = this_ptr->Activity_Generator<activity_generator_itf*>();
			generator->template Activity_Generation<TargetType>();

			// set next activity generation occurence
			this_ptr->template Next_Activity_Generation_Time<Simulation_Timestep_Increment>(Round<long,double>(Simulation_Time.Future_Time<Simulation_Timestep_Increment,Simulation_Timestep_Increment>(this_ptr->template Generation_Time_Increment<Simulation_Timestep_Increment>())));
		}
		declare_feature_event(Activity_Planning_Event)
		{
			// Create alias for this to use in conditional
			typedef Person_Planner<ComponentType, ComponentType> _Planning_Interface;
			ComponentType* _pthis = (ComponentType*)_this;
			_Planning_Interface* this_ptr=(_Planning_Interface*)_pthis;

			define_container_and_value_interface(Activity_Plans,Activity_Plan,typename get_type_of(Activity_Plans_Container),Containers::Back_Insertion_Sequence_Prototype,Activity_Plan_Prototype,ComponentType);
			Activity_Plans* activities = this_ptr->Activity_Plans_Container<Activity_Plans*>();
			typename Activity_Plans::iterator act_itr = activities->begin();

			while (act_itr != activities->end())
			{
				Activity_Plan* act = *act_itr;

				// if movement happens in the current planning increment, execute movement
				if (act->template planning_time<Simulation_Timestep_Increment>() >= Simulation_Time.Current_Time<Simulation_Timestep_Increment>() &&
					act->template planning_time<Simulation_Timestep_Increment>() < Simulation_Time.Future_Time<Simulation_Timestep_Increment,Simulation_Timestep_Increment>(this_ptr->Planning_Time_Increment<Simulation_Timestep_Increment>()))
				{
					// Do activity planning
					act->template Do_Activity_Planning<NULLTYPE>();	
	
					//TODO: CHANGE SO THAT MULTIPLE MOVES CAN BE PLANNED PER PLANNING TIMESTEP - currently we are only simulating the first planned move, then throwing out the rest
					typename Activity_Plans::iterator prev = act_itr++;
					activities->erase(prev);
				}

				// remove movements which have already been skipped
				else if (act->template planning_time<Simulation_Timestep_Increment>() < Simulation_Time.Current_Time<Simulation_Timestep_Increment>())
				{
					typename Activity_Plans::iterator prev = act_itr++;
					activities->erase(prev);
				}
				// exit if no movements in current timestep
				else
				{
					break;
				}
			}
		}
		declare_feature_event(Movement_Planning_Event)
		{
			// Create alias for this to use in conditional
			typedef Person_Planner<ComponentType, ComponentType> _Planning_Interface;
			ComponentType* _pthis = (ComponentType*)_this;
			_Planning_Interface* this_ptr=(_Planning_Interface*)_pthis;

			// create aliases for network components from parent
			define_component_interface(parent_itf,typename get_type_of(Parent_Person),Prototypes::Person,ComponentType);
			define_component_interface(_Network_Interface, typename parent_itf::get_type_of(network_reference), Network_Components::Prototypes::Network_Prototype, ComponentType);	
			define_container_and_value_interface(_Activity_Locations_Container_Interface, _Activity_Location_Interface, typename _Network_Interface::get_type_of(activity_locations_container), Random_Access_Sequence_Prototype, Activity_Location_Components::Prototypes::Activity_Location_Prototype, ComponentType);
			define_container_and_value_interface(_Links_Container_Interface, _Link_Interface, typename _Activity_Location_Interface::get_type_of(origin_links), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
			define_container_and_value_interface(_Zones_Container_Interface, _Zone_Interface, typename _Network_Interface::get_type_of(zones_container), Random_Access_Sequence_Prototype, Zone_Components::Prototypes::Zone_Prototype, ComponentType);
			parent_itf* parent = this_ptr->template Parent_Person<parent_itf*>();
			_Network_Interface* network = parent->template network_reference<_Network_Interface*>();

			// Get reference to vehicle, and add current movement plan to it for routing
			define_component_interface(vehicle_itf,typename parent_itf::get_type_of(vehicle),Vehicle_Components::Prototypes::Vehicle_Prototype,ComponentType);
			vehicle_itf* vehicle = parent->template vehicle<vehicle_itf*>();

			// Get reference to movement plans
			define_container_and_value_interface(Movement_Plans,Movement_Plan,typename get_type_of(Movement_Plans_Container),Containers::Back_Insertion_Sequence_Prototype,Movement_Plan_Components::Prototypes::Movement_Plan_Prototype,ComponentType);
			Movement_Plans* movements = this_ptr->Movement_Plans_Container<Movement_Plans*>();
			typename Movement_Plans::iterator move_itr = movements->begin();


			if (typename ComponentType::_write_activity_files) typename ComponentType::logs[_thread_id]<<" >= "<< Simulation_Time.Current_Time<Simulation_Timestep_Increment>() << " && < " <<Simulation_Time.Future_Time<Simulation_Timestep_Increment,Simulation_Timestep_Increment>(this_ptr->Planning_Time_Increment<Simulation_Timestep_Increment>())<<endl;


			while (move_itr != movements->end())
			{
				Movement_Plan* move = *move_itr;

				//if (typename ComponentType::_write_activity_files) typename ComponentType::logs[_thread_id]<<"MOVE_EVENT:," << parent->uuid<int>() << ","<<move->template departed_time<Simulation_Timestep_Increment>()<<",";
				//if (typename ComponentType::_write_activity_files) typename ComponentType::logs[_thread_id]<<" >= "<< Simulation_Time.Current_Time<Simulation_Timestep_Increment>() << " && < " <<Simulation_Time.Future_Time<Simulation_Timestep_Increment,Simulation_Timestep_Increment>(this_ptr->Planning_Time_Increment<Simulation_Timestep_Increment>())<<endl;

				// if movement happens in the current planning increment, execute movement
				if (move->template departed_time<Simulation_Timestep_Increment>() >= Simulation_Time.Current_Time<Simulation_Timestep_Increment>() &&
					move->template departed_time<Simulation_Timestep_Increment>() < Simulation_Time.Future_Time<Simulation_Timestep_Increment,Simulation_Timestep_Increment>(this_ptr->Planning_Time_Increment<Simulation_Timestep_Increment>()))
				{

					if (typename ComponentType::_write_activity_files) typename ComponentType::logs[_thread_id]<<"MOVE_EVENT:," << parent->uuid<int>() << ", PASSED."<<endl;
				
					// make sure vehicle is not already being simulated, skip movement if it is
					if (vehicle->template simulation_status<Vehicle_Components::Types::Vehicle_Status_Keys>() == Vehicle_Components::Types::Vehicle_Status_Keys::UNLOADED)
					{
						// increment the zone origin/destination counters based on movement plan
						_Activity_Location_Interface* orig = move->origin<_Activity_Location_Interface*>();
						_Activity_Location_Interface* dest = move->destination<_Activity_Location_Interface*>();
						_Zone_Interface* orig_zone = orig->zone<_Zone_Interface*>();
						_Zone_Interface* dest_zone = dest->zone<_Zone_Interface*>();

						// if origin or destination zones are unavailable, abort trip
						if (orig_zone->zone_is_available<bool>()== false || dest_zone->zone_is_available<bool>() == false)
						{
							cout << endl << "zone is unavailable";
							typename Movement_Plans::iterator prev = move_itr++;
							movements->erase(prev);
							return;
						}

						// increment counters for display
						orig_zone->production_count<int&>()++;
						dest_zone->attraction_count<int&>()++;

						// add movement plan to the person's vehicle and schedule the departure
						vehicle->template movement_plan<Movement_Plan*>(move);

						this_ptr->template Schedule_New_Departure<NULLTYPE>(move->template departed_time<Simulation_Timestep_Increment>());
					}
	
					//TODO: CHANGE SO THAT MULTIPLE MOVES CAN BE PLANNED PER PLANNING TIMESTEP - currently we are only simulating the first planned move, then throwing out the rest
					typename Movement_Plans::iterator prev = move_itr++;
					movements->erase(prev);
				}

				// remove movements which have already been skipped
				else if (move->template departed_time<Simulation_Timestep_Increment>() < Simulation_Time.Current_Time<Simulation_Timestep_Increment>())
				{
					typename Movement_Plans::iterator prev = move_itr++;
					movements->erase(prev);
				}
				// exit if no movements in current timestep
				else
				{
					return;
				}
			}

			//// Get all movement plans scheduled for current iteration
			//pair<typename Movement_Plans::iterator, typename Movement_Plans::iterator> range = movements->equal_range(_iteration);

			//// Execute movement plans and remove from schedule
			//for (typename Movement_Plans::iterator move_itr = range.first; move_itr != range.second; ++move_itr)
			//{		
			//	Movement_Plan* move = move_itr->second;
			//	// make sure vehicle is not already being simulated, skip movement if it is
			//	if (vehicle->template simulation_status<Vehicle_Components::Types::Vehicle_Status_Keys>() == Vehicle_Components::Types::Vehicle_Status_Keys::UNLOADED)
			//	{
			//		vehicle->template movement_plan<Movement_Plan*>(move);
			//		this_ptr->template Schedule_New_Departure<NULLTYPE>(move->template departed_time<Simulation_Timestep_Increment>());

			//		// increment the zone origin/destination counters based on movement plan
			//		_Activity_Location_Interface* orig = move->origin<_Activity_Location_Interface*>();
			//		_Activity_Location_Interface* dest = move->destination<_Activity_Location_Interface*>();
			//		_Zone_Interface* orig_zone = orig->zone<_Zone_Interface*>();
			//		_Zone_Interface* dest_zone = dest->zone<_Zone_Interface*>();
			//		orig_zone->production_count<int&>()++;
			//		dest_zone->attraction_count<int&>()++;
			//	}
	
			//	//TODO: CHANGE SO THAT MULTIPLE MOVES CAN BE PLANNED PER PLANNING TIMESTEP - currently we are only simulating the first planned move, then throwing out the rest
			//	break;

			//}
			//movements->erase(_iteration);
			////movements->erase(range.first, range.second);
		}


		feature_prototype void Initialize(requires(check(ComponentType,Concepts::Has_Initialize)))
		{
			define_component_interface(parent_itf,typename get_type_of(Parent_Person),Prototypes::Person,ComponentType);
			parent_itf* parent = this->Parent_Person<parent_itf*>();
			long first_iter = parent->template First_Iteration<Simulation_Timestep_Increment>();
			this_component()->Initialize<ComponentType,CallerType, TargetType>();
			load_event(ComponentType,Planning_Conditional,Activity_Generation_Event,first_iter,0,NULLTYPE);
		}
		feature_prototype void Initialize(requires(!check(ComponentType,Concepts::Has_Initialize)))
		{
			assert_check(ComponentType,Concepts::Has_Initialize,"This ComponentType is not a valid Agent, does not have an initializer.   Did you forget to use tag_feature_as_available macro?");
		}
		feature_prototype void Initialize(TargetType initializer, requires(check(ComponentType,Concepts::Has_Initialize)))
		{
			define_component_interface(parent_itf,typename get_type_of(Parent_Person),Prototypes::Person,ComponentType);
			parent_itf* parent = this->Parent_Person<parent_itf*>();
			this_component()->Initialize<ComponentType,CallerType, TargetType>(initializer);
			load_event(ComponentType,Planning_Conditional,Movement_Planning_Event,parent->template First_Iteration<Simulation_Timestep_Increment>(),0,NULLTYPE);
		}
		feature_prototype void Initialize(TargetType initializer, requires(!check(ComponentType,Concepts::Has_Initialize)))
		{
			assert_check(ComponentType,Concepts::Has_Initialize,"This ComponentType is not a valid Agent, does not have an initializer.   Did you forget to use tag_feature_as_available macro?");
		}

		feature_prototype bool Schedule_New_Departure(int departed_time)
		{
			// schedule routing		
			define_component_interface(Parent_Person_Itf, typename get_type_of(Parent_Person), Person_Components::Prototypes::Person, ComponentType);
			define_component_interface(Vehicle_Itf, typename get_type_of(Parent_Person)::get_type_of(vehicle), Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);
			define_component_interface(Routing_Itf, typename get_type_of(Parent_Person)::get_type_of(router), Routing_Components::Prototypes::Routing_Prototype, ComponentType);

			Parent_Person_Itf* person_itf = this->Parent_Person<Parent_Person_Itf*>();
			Routing_Itf* itf= person_itf->template router<Routing_Itf*>();	
			Vehicle_Itf* vehicle_itf = person_itf->template vehicle<Vehicle_Itf*>();

		
			// Schedule the routing if the vehicle is not already in the network, otherwise return false
			if (vehicle_itf->template simulation_status<Vehicle_Components::Types::Vehicle_Status_Keys>() == Vehicle_Components::Types::Vehicle_Status_Keys::UNLOADED)
			{
				if (typename ComponentType::_write_activity_files) typename ComponentType::logs[_thread_id]<<"SCHEDULE_ROUTER:," << person_itf->uuid<int>() << ","<<departed_time<<endl;
				itf->template Schedule_Route_Computation<NULLTYPE>(departed_time);
				return true;
			}
			return false;
		}

	
		// Activity Plans and Movement plans, stored in a hashmap keyed based on next required plan time (updated in the plan class after plan completion)
		feature_accessor(Activity_Generator,none,none);
		feature_accessor(Activity_Plans_Container,none,none);
		feature_accessor(Movement_Plans_Container,none,none);
		feature_accessor(Schedule_Container,none,none);	
		feature_accessor(current_movement_plan,none,none);
		feature_accessor(current_activity_plan,none,none);
		feature_prototype void Add_Movement_Plan(TargetType movement_plan)
		{
			this_component()->Add_Movement_Plan<ComponentType,CallerType,TargetType>(movement_plan);
		}
		feature_prototype void Add_Activity_Plan(TargetType activity_plan)
		{
			this_component()->Add_Activity_Plan<ComponentType,CallerType,TargetType>(activity_plan);
		}

		// indicators used to determine next iteration for generation/planning events
		feature_accessor(Next_Activity_Generation_Time,none,none);
		feature_accessor(Planning_Time_Increment,none,none);
		feature_accessor(Generation_Time_Increment,none,none);

		// accessor to parent class
		feature_accessor(Parent_Person,none,none);
	};


	prototype struct Activity_Generator
	{
		tag_as_prototype;

		// accessor to parent class
		feature_accessor(Parent_Planner,none,none);

		feature_prototype void Activity_Generation()
		{
			this_component()->Activity_Generation<ComponentType,CallerType,TargetType>();
		}
	};
}


}