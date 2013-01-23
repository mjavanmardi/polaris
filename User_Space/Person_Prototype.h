#pragma once

#include "User_Space_Includes.h"

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

}

namespace Variables
{
}

namespace Prototypes
{
	prototype struct Person_Prototype : ComponentType
	{
		// Event handling
		declare_feature_conditional(Agent_Conditional)
		{
			response.next._iteration = Simulation_Time.Future_Time<Time_Minutes>(5);
			response.next._sub_iteration = 0;
			response.result = true;
		}
		declare_feature_event(Agent_Event)
		{
			cout << endl << "Current time in minutes: "<<Simulation_Time.Current_Time<Time_Minutes>();
		}

		// Initializer
		feature_prototype void Initialize(TargetType id, requires(check(ComponentType,Concepts::Has_Initialize)))
		{
			//load_event(ComponentType,Agent_Conditional,Agent_Event,0,NULLTYPE);
			((ComponentType*)this)->Load_Register<ComponentType>((&Agent_Conditional<NULLTYPE>),(&Agent_Event<NULLTYPE>),0,0);
			this_component()->Initialize<ComponentType, CallerType, TargetType>(id);
		}
		feature_prototype void Initialize(TargetType id, requires(!check(ComponentType,Concepts::Has_Initialize)))
		{
			assert_check(ComponentType,Concepts::Has_Initialize,"This ComponentType is not a valid Agent, does not have an initializer.   Did you forget to use tag_feature_as_available macro?");
		}
		feature_prototype void Initialize(typename TargetType::ParamType id, typename TargetType::Param2Type trip, requires(check(ComponentType,Concepts::Has_Initialize)))
		{
			//load_event(ComponentType,Agent_Conditional,Agent_Event,0,NULLTYPE);
			((ComponentType*)this)->Load_Register<ComponentType>((&Agent_Conditional<NULLTYPE>),(&Agent_Event<NULLTYPE>),0,0);
			this_component()->Initialize<ComponentType, CallerType, TargetType>(id, trip);
		}
		feature_prototype void Initialize(typename TargetType::ParamType id, typename TargetType::Param2Type trip, requires(!check(ComponentType,Concepts::Has_Initialize)))
		{
			assert_check(ComponentType,Concepts::Has_Initialize,"This ComponentType is not a valid Agent, does not have an initializer.   Did you forget to use tag_feature_as_available macro?");
		}

		// Primary faculty accessors
		feature_accessor(ID,none,none);
		feature_accessor(Planning_Faculty,none,none);
		feature_accessor(Moving_Faculty,none,none);
		feature_accessor(Properties,none,none);
		feature_accessor(Next_Rand,none,none);
	};


	prototype struct Person_Properties : ComponentType
	{
		// Initializer
		define_feature_exists_check(Initialize,Has_Initialize);
		feature_prototype void Initialize(requires(check(ComponentType,Has_Initialize)))
		{
			this_component()->Initialize<ComponentType, CallerType, TargetType>();
		}
		feature_prototype void Initialize(requires(!check(ComponentType,Has_Initialize)))
		{
			assert_check(ComponentType,Has_Initialize,"This ComponentType is not a valid Agent, does not have an initializer.   Did you forget to use tag_feature_as_available macro?");
		}

		feature_accessor(My_Length,none,none);
		feature_accessor(My_Area,none,none);
		feature_accessor(My_Time,none,none);
		feature_accessor(My_Speed,none,none);
		feature_accessor(HH_Income,none,none);
	};


	prototype struct Person_Planner : ComponentType
	{
		// Event handling
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
			define_container_and_value_interface(Activity_Plans_List,Activity_Plan,type_of(Activity_Plans_Container),Associative_Container_Prototype,Activity_Plan_Prototype,ComponentType);
			define_container_and_value_interface(Movement_Plans_List,Movement_Plan,type_of(Movement_Plans_Container),Associative_Container_Prototype,Movement_Plan_Prototype,ComponentType);
			Activity_Plans_List* activity_plans = this_ptr->Activity_Plans_Container<Activity_Plans_List*>();
			Movement_Plans_List* movement_plans = this_ptr->Movement_Plans_Container<Movement_Plans_List*>();

			// during the 0th sub_iteration, set up future sub_iteration schedule
			if (_sub_iteration == 0)
			{
				// If activity generation is to be performed, do that next
				if(this_ptr->Next_Activity_Generation_Time<Time_Seconds>() == Simulation_Time.Current_Time<Time_Seconds>())
				{			
					response.next._iteration = _iteration;
					response.next._sub_iteration = Types::PLANNING_ITERATION_STEP_KEYS::ACTIVITY_GENERATION;
					response.result = false;
				}
				// check if activity planning needs to occur, if so do this
				else if (activity_plans->find(_iteration) != activity_plans->end())
				{
					response.next._iteration = _iteration;
					response.next._sub_iteration = Types::PLANNING_ITERATION_STEP_KEYS::ACTIVITY_PLANNING;
					response.result = false;
				}
				// check if movement planning needs to occur, if so do this
				else if (movement_plans->find(_iteration) != movement_plans->end())
				{
					response.next._iteration = _iteration;
					response.next._sub_iteration = Types::PLANNING_ITERATION_STEP_KEYS::MOVEMENT_PLANNING;
					response.result = false;
				}
				// otherwise move on to next main iteration
				else
				{
					response.next._iteration = Round<long,double>(Simulation_Time.Future_Time<Time_Minutes>(this_ptr->Planning_Time_Increment<Time_Minutes>()));
					response.next._sub_iteration = 0;
					response.result = false;
				}
			}
			// during the Activity Generation sub_iteration, swap in the activity-generation event and set up future sub_iteration schedule
			else if (_sub_iteration == Types::PLANNING_ITERATION_STEP_KEYS::ACTIVITY_GENERATION)
			{
				_pthis->Swap_Event((Event)&Person_Planner::Activity_Generation_Event<NULLTYPE>);

				// check if activity planning needs to occur, if so do this
				if (activity_plans->find(_iteration) != activity_plans->end())
				{
					response.next._iteration = _iteration;
					response.next._sub_iteration = Types::PLANNING_ITERATION_STEP_KEYS::ACTIVITY_PLANNING;
					response.result = true;
				}
				// check if movement planning needs to occur, if so do this
				else if (movement_plans->find(_iteration) != movement_plans->end())
				{
					response.next._iteration = _iteration;
					response.next._sub_iteration = Types::PLANNING_ITERATION_STEP_KEYS::MOVEMENT_PLANNING;
					response.result = true;
				}
				// otherwise, finish activity generation and move on to next main iteration
				else
				{
					response.next._iteration = Round<long,double>(Simulation_Time.Future_Time<Time_Minutes>(this_ptr->Planning_Time_Increment<Time_Minutes>()));
					response.next._sub_iteration = 0;
					response.result = true;
				}
			}
			// during the Activity Planning sub_iteration, swap in the activity-planning event and set up future sub_iteration schedule
			else if (_sub_iteration == Types::PLANNING_ITERATION_STEP_KEYS::ACTIVITY_PLANNING)
			{
				_pthis->Swap_Event((Event)&Person_Planner::Activity_Planning_Event<NULLTYPE>);

				// check if movement planning needs to occur, if so do this next
				if (movement_plans->find(_iteration) != movement_plans->end())
				{
					response.next._iteration = _iteration;
					response.next._sub_iteration = Types::PLANNING_ITERATION_STEP_KEYS::MOVEMENT_PLANNING;
					response.result = true;
				}
				// otherwise, finish activity planning and move on to next main iteration
				else
				{
					response.next._iteration = Round<long,double>(Simulation_Time.Future_Time<Time_Minutes>(this_ptr->Planning_Time_Increment<Time_Minutes>()));
					response.next._sub_iteration = 0;
					response.result = true;
				}
			}
			// during the Activity Generation sub_iteration, swap in the movement planning event and set up future sub_iteration schedule
			else if (_sub_iteration == Types::PLANNING_ITERATION_STEP_KEYS::MOVEMENT_PLANNING)
			{
				// swap in movement event
				_pthis->Swap_Event((Event)&Person_Planner::Movement_Planning_Event<NULLTYPE>);
				response.next._iteration = Round<long,double>(Simulation_Time.Future_Time<Time_Minutes>(this_ptr->Planning_Time_Increment<Time_Minutes>()));
				response.next._sub_iteration = 0;
				response.result = true;
			}
			else
			{
				response.next._iteration = Round<long,double>(Simulation_Time.Future_Time<Time_Minutes>(this_ptr->Planning_Time_Increment<Time_Minutes>()));
				response.next._sub_iteration = 0;
				response.result = false;
			}
		}
		
		declare_feature_event(Activity_Generation_Event)
		{
			// Create alias for this to use in conditional
			typedef Person_Planner<ComponentType, ComponentType> _Planning_Interface;
			ComponentType* _pthis = (ComponentType*)_this;
			_Planning_Interface* this_ptr=(_Planning_Interface*)_pthis;

			// Call specific implementation of the activity generation routine
			_pthis->Activity_Generation<ComponentType,CallerType,TargetType>();

			// set next activity generation occurence
			this_ptr->Next_Activity_Generation_Time<Simulation_Timestep_Increment>(Round<long,double>(Simulation_Time.Future_Time<Time_Minutes>(this_ptr->Generation_Time_Increment<Time_Minutes>())));
			cout << endl << "Doing Activity Generation for iteration " << _iteration;
		}
		declare_feature_event(Activity_Planning_Event)
		{
			// Create alias for this to use in conditional
			typedef Person_Planner<ComponentType, ComponentType> _Planning_Interface;
			ComponentType* _pthis = (ComponentType*)_this;
			_Planning_Interface* this_ptr=(_Planning_Interface*)_pthis;

			define_container_and_value_interface(Activity_Plans,Activity_Plan,type_of(Activity_Plans_Container),Associative_Container_Prototype,Activity_Plan_Prototype,ComponentType);
			Activity_Plans* activities = this_ptr->Activity_Plans_Container<Activity_Plans*>();
			pair<Activity_Plans::iterator,Activity_Plans::iterator> range = activities->equal_range(_iteration);
			
			for (Activity_Plans::iterator act_itr = range.first; act_itr != range.second; ++act_itr)
			{
				Activity_Plan* act = act_itr->second;
				act->Do_Activity_Planning<NULLTYPE>();	
			}
			activities->erase(range.first, range.second);
		}
		declare_feature_event(Movement_Planning_Event)
		{
			// Create alias for this to use in conditional
			typedef Person_Planner<ComponentType, ComponentType> _Planning_Interface;
			ComponentType* _pthis = (ComponentType*)_this;
			_Planning_Interface* this_ptr=(_Planning_Interface*)_pthis;

			// Get referecne to movement plans
			define_container_and_value_interface(Movement_Plans,Movement_Plan,type_of(Movement_Plans_Container),Associative_Container_Prototype,Movement_Plan_Prototype,ComponentType);
			Movement_Plans* movements = this_ptr->Movement_Plans_Container<Movement_Plans*>();
			
			// Get all movement plans scheduled for current iteration
			pair<Movement_Plans::iterator,Movement_Plans::iterator> range = movements->equal_range(_iteration);
			
			// Execute movement plans and remove from schedule
			for (Movement_Plans::iterator move_itr = range.first; move_itr != range.second; ++move_itr)
			{
				Movement_Plan* move = move_itr->second;
				move->Do_Movement_Planning<NULLTYPE>();
			}
			movements->erase(range.first, range.second);
		}
		
		feature_prototype void Initialize(requires(check(ComponentType,Concepts::Has_Initialize)))
		{
			this_component()->Load_Register<ComponentType>((&Planning_Conditional<NULLTYPE>),(&Activity_Generation_Event<NULLTYPE>),0,0);
			this_component()->Initialize<ComponentType, CallerType, TargetType>();
		}
		feature_prototype void Initialize(requires(!check(ComponentType,Concepts::Has_Initialize)))
		{
			assert_check(ComponentType,Concepts::Has_Initialize,"This ComponentType is not a valid Agent, does not have an initializer.   Did you forget to use tag_feature_as_available macro?");
		}
		feature_prototype void Initialize(TargetType initializer, requires(check(ComponentType,Concepts::Has_Initialize)))
		{
			this_component()->Load_Register<ComponentType>((&Planning_Conditional<NULLTYPE>),(&Movement_Planning_Event<NULLTYPE>),0,0);
			this_component()->Initialize<ComponentType, CallerType, TargetType>(initializer);
		}
		feature_prototype void Initialize(TargetType initializer, requires(!check(ComponentType,Concepts::Has_Initialize)))
		{
			assert_check(ComponentType,Concepts::Has_Initialize,"This ComponentType is not a valid Agent, does not have an initializer.   Did you forget to use tag_feature_as_available macro?");
		}

	
		// Activity Plans and Movement plans, stored in a hashmap keyed based on next required plan time (updated in the plan class after plan completion)
		feature_accessor(Activity_Plans_Container,none,none);
		feature_accessor(Movement_Plans_Container,none,none);	

		feature_accessor(Next_Activity_Generation_Time,none,none);
		feature_accessor(Planning_Time_Increment,none,none);
		feature_accessor(Generation_Time_Increment,none,none);
	};


	prototype struct Activity_Plan_Prototype
	{
		feature_prototype void Initialize()
		{
			this_component()->Initialize<ComponentType, ComponentType, TargetType>();
		}
		feature_prototype void Do_Activity_Planning()
		{
			cout <<endl<<"Doing Activity Planning for Activity "<<this->Activity_Plan_ID<int>();
		}

		feature_accessor(Activity_Plan_ID, check(ReturnValueType,is_arithmetic), check(SetValueType,is_arithmetic));
		feature_accessor(Activity_Type,none,none);
		feature_accessor(Activity_Plan_Horizon,none,none);

		feature_accessor(Location_Type, none, none);
		feature_accessor(Location_ID, check(ReturnValueType,is_arithmetic), check(SetValueType,is_arithmetic));
		feature_accessor(Location_Plan_Horizon, none, none);
		feature_accessor(Location_Flexibility, none, none);

		feature_accessor(Mode_Type, none, none);
		feature_accessor(Mode_ID, none, none);
		feature_accessor(Mode_Plan_Horizon, none, none);
		feature_accessor(Mode_Flexibility, none, none);
	};


	prototype struct Movement_Plan_Prototype
	{
		feature_accessor(Movement_Plan_ID,none,none);
		feature_prototype void Do_Movement_Planning()
		{
			cout <<endl<<"Doing Movement Planning for Movement "<<this->Movement_Plan_ID<int>();
		}
	};
}


}