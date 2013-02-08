#pragma once

#include "User_Space.h"

//===============================================================
#pragma region FORWARD DECLARATIONS
namespace Activity_Components {
	namespace Prototypes
	{
		forward_declare_prototype struct Activity_Plan_Prototype;
	}
};

namespace Movement_Plan_Components {
	namespace Prototypes
	{
		forward_declare_prototype struct Movement_Plan_Prototype;
	}
};
#pragma endregion
//===============================================================


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
			response.next._iteration = Simulation_Time.Future_Time<Time_Minutes,Simulation_Timestep_Increment>(15);
			response.next._sub_iteration = 0;
			response.result = true;
		}
		declare_feature_event(Agent_Event)
		{
			typedef Person_Prototype<ComponentType, ComponentType> _Person_Interface;
			ComponentType* _pthis = (ComponentType*)_this;
			_Person_Interface* pthis =(_Person_Interface*)_pthis;

			define_component_interface(_network_itf,get_type_of(network_reference),Network_Components::Prototypes::Network_Prototype,CallerType);
			_network_itf* network = pthis->network_reference<_network_itf*>();

			if (pthis->internal_id<int>() == 4090)
			{
				cout << endl << "At time in minutes: "<<Simulation_Time.Current_Time<Time_Minutes>();
				cout << endl << "Agent "<<pthis->internal_id<int>()<<"'s travel time from zone 1 to zone 3 is :" << network->Get_LOS<Target_Type<Time_Minutes,int,int>>(1,3,Vehicle_Components::Types::Vehicle_Type_Keys::SOV);

			}
		}

		// Initializer
		feature_prototype void Initialize(TargetType id, requires(check(ComponentType,Concepts::Has_Initialize)))
		{
			this_component()->Initialize<ComponentType, CallerType, TargetType>(id);
			load_event(ComponentType,Agent_Conditional,Agent_Event,this->First_Iteration<Simulation_Timestep_Increment>(),0,NULLTYPE);
		}
		feature_prototype void Initialize(TargetType id, requires(!check(ComponentType,Concepts::Has_Initialize)))
		{
			assert_check(ComponentType,Concepts::Has_Initialize,"This ComponentType is not a valid Agent, does not have an initializer.   Did you forget to use tag_feature_as_available macro?");
		}
		feature_prototype void Initialize(typename TargetType::ParamType id, typename TargetType::Param2Type trip, requires(check(ComponentType,Concepts::Has_Initialize)))
		{
			this_component()->Initialize<ComponentType, CallerType, TargetType>(id, trip);
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
		feature_accessor(vehicle,none,none);

		// External knowledge accessor
		feature_accessor(network_reference,none,none);
		feature_accessor(scenario_reference,none,none);

		// Basic property members
		feature_accessor(uuid,none,none);
		feature_accessor(internal_id,none,none);
		feature_accessor(Next_Rand,none,none);
		feature_accessor(First_Iteration,none,none);
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

		// accessor to parent class
		feature_accessor(Parent_Person,none,none);
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
					response.next._iteration = Round<long,double>(Simulation_Time.Future_Time<Simulation_Timestep_Increment,Simulation_Timestep_Increment>(this_ptr->Planning_Time_Increment<Simulation_Timestep_Increment>()));
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
					response.next._iteration = Round<long,double>(Simulation_Time.Future_Time<Simulation_Timestep_Increment,Simulation_Timestep_Increment>(this_ptr->Planning_Time_Increment<Simulation_Timestep_Increment>()));
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
					response.next._iteration = Round<long,double>(Simulation_Time.Future_Time<Simulation_Timestep_Increment,Simulation_Timestep_Increment>(this_ptr->Planning_Time_Increment<Simulation_Timestep_Increment>()));
					response.next._sub_iteration = 0;
					response.result = true;
				}
			}
			// during the Activity Generation sub_iteration, swap in the movement planning event and set up future sub_iteration schedule
			else if (_sub_iteration == Types::PLANNING_ITERATION_STEP_KEYS::MOVEMENT_PLANNING)
			{
				// swap in movement event
				_pthis->Swap_Event((Event)&Person_Planner::Movement_Planning_Event<NULLTYPE>);
				response.next._iteration = Round<long,double>(Simulation_Time.Future_Time<Simulation_Timestep_Increment,Simulation_Timestep_Increment>(this_ptr->Planning_Time_Increment<Simulation_Timestep_Increment>()));
				response.next._sub_iteration = 0;
				response.result = true;
			}
			else
			{
				response.next._iteration = Round<long,double>(Simulation_Time.Future_Time<Simulation_Timestep_Increment,Simulation_Timestep_Increment>(this_ptr->Planning_Time_Increment<Simulation_Timestep_Increment>()));
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
			this_ptr->Next_Activity_Generation_Time<Simulation_Timestep_Increment>(Round<long,double>(Simulation_Time.Future_Time<Simulation_Timestep_Increment,Simulation_Timestep_Increment>(this_ptr->Generation_Time_Increment<Simulation_Timestep_Increment>())));
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

			// Get reference to movement plans
			define_container_and_value_interface(Movement_Plans,Movement_Plan,type_of(Movement_Plans_Container),Associative_Container_Prototype,Movement_Plan_Components::Prototypes::Movement_Plan_Prototype,ComponentType);
			Movement_Plans* movements = this_ptr->Movement_Plans_Container<Movement_Plans*>();
			
			// Get all movement plans scheduled for current iteration
			pair<Movement_Plans::iterator,Movement_Plans::iterator> range = movements->equal_range(_iteration);

			// Get reference to vehicle, and add current movement plan to it for routing
			define_component_interface(parent_itf,type_of(Parent_Person),Person_Components::Prototypes::Person_Prototype,ComponentType);
			define_component_interface(vehicle_itf,type_of(Parent_Person)::type_of(vehicle),Vehicle_Components::Prototypes::Vehicle_Prototype,ComponentType);
			parent_itf* parent = this_ptr->Parent_Person<parent_itf*>();
			vehicle_itf* vehicle = parent->vehicle<vehicle_itf*>();
			
			// Execute movement plans and remove from schedule
			for (Movement_Plans::iterator move_itr = range.first; move_itr != range.second; ++move_itr)
			{		
				Movement_Plan* move = move_itr->second;
				// make sure vehicle is not already being simulated, skip movement if it is
				if (vehicle->simulation_status<Vehicle_Components::Types::Vehicle_Status_Keys>() != Vehicle_Components::Types::Vehicle_Status_Keys::IN_NETWORK)
				{
					vehicle->movement_plan<Movement_Plan*>(move);
					this_ptr->Schedule_New_Departure<NULLTYPE>(move->departed_time<Simulation_Timestep_Increment>());
				}

				//TODO: CHANGE SO THAT MULTIPLE MOVES CAN BE PLANNED PER PLANNING TIMESTEP - currently we are only simulating the first planned move, then throwing out the rest
				break;

			}
			movements->erase(range.first, range.second);
		}

		feature_prototype void Initialize(requires(check(ComponentType,Concepts::Has_Initialize)))
		{
			define_component_interface(parent_itf,get_type_of(Parent_Person),Prototypes::Person_Prototype,ComponentType);
			parent_itf* parent = this->Parent_Person<parent_itf*>();
			long first_iter = parent->First_Iteration<Simulation_Timestep_Increment>();
			this_component()->Initialize<ComponentType, CallerType, TargetType>();
			load_event(ComponentType,Planning_Conditional,Activity_Generation_Event,first_iter,0,NULLTYPE);
		}
		feature_prototype void Initialize(requires(!check(ComponentType,Concepts::Has_Initialize)))
		{
			assert_check(ComponentType,Concepts::Has_Initialize,"This ComponentType is not a valid Agent, does not have an initializer.   Did you forget to use tag_feature_as_available macro?");
		}
		feature_prototype void Initialize(TargetType initializer, requires(check(ComponentType,Concepts::Has_Initialize)))
		{
			define_component_interface(parent_itf,get_type_of(Parent_Person),Prototypes::Person_Prototype,ComponentType);
			parent_itf* parent = this->Parent_Person<parent_itf*>();
			this_component()->Initialize<ComponentType, CallerType, TargetType>(initializer);
			this_component()->Load_Register<ComponentType>((&Planning_Conditional<NULLTYPE>),(&Movement_Planning_Event<NULLTYPE>),parent->First_Iteration<Simulation_Timestep_Increment>(),0);
		}
		feature_prototype void Initialize(TargetType initializer, requires(!check(ComponentType,Concepts::Has_Initialize)))
		{
			assert_check(ComponentType,Concepts::Has_Initialize,"This ComponentType is not a valid Agent, does not have an initializer.   Did you forget to use tag_feature_as_available macro?");
		}

		feature_prototype bool Schedule_New_Departure(int departed_time)
		{
			// schedule routing		
			define_component_interface(Parent_Person_Itf, get_type_of(Parent_Person), Person_Components::Prototypes::Person_Prototype, ComponentType);
			define_component_interface(Vehicle_Itf, get_type_of(Parent_Person)::get_type_of(vehicle), Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);
			define_component_interface(Routing_Itf, get_type_of(Parent_Person)::get_type_of(router), Routing_Components::Prototypes::Routing_Prototype, ComponentType);

			Parent_Person_Itf* person_itf = this->Parent_Person<Parent_Person_Itf*>();
			Routing_Itf* itf= person_itf->router<Routing_Itf*>();	
			Vehicle_Itf* vehicle_itf = person_itf->vehicle<Vehicle_Itf*>();
			
			// Schedule the routing if the vehicle is not already in the network, otherwise return false
			if (vehicle_itf->simulation_status<Vehicle_Components::Types::Vehicle_Status_Keys>() != Vehicle_Components::Types::Vehicle_Status_Keys::IN_NETWORK)
			{
				itf->template Schedule_Route_Computation<NULLTYPE>(departed_time);
				return true;
			}
			return false;
		}

	
		// Activity Plans and Movement plans, stored in a hashmap keyed based on next required plan time (updated in the plan class after plan completion)
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

}


}