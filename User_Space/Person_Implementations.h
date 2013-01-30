#pragma once

#include "User_Space\Person_Prototype.h"
#include "Polaris_Movement_Plan_Implementation.h"
#include "Activity_Implementations.h"

namespace Person_Components
{
	namespace Implementations
	{
		implementation struct Person_Implementation : public Polaris_Component_Class<Person_Implementation,MasterType,Execution_Object,ParentType>
		{
			feature_implementation void Initialize(TargetType id)
			{	
				// Set the initial iteration to process
				this->First_Iteration<ComponentType,CallerType,Time_Minutes>(15.0);

				// Create and Initialize the Properties faculty
				define_component_interface(properties_itf,type_of(Properties),Prototypes::Person_Properties,ComponentType);
				properties_itf* properties = (properties_itf*)Allocate<type_of(Properties)>();
				properties->Initialize<NULLTYPE>();
				properties->Parent_Person<ComponentType*>(this);
				this->Properties<ComponentType,ComponentType,properties_itf*>(properties);

				// Create and Initialize the Planner faculty
				define_component_interface(planner_itf,type_of(Planning_Faculty),Prototypes::Person_Planner,ComponentType);
				planner_itf* planner = (planner_itf*)Allocate<type_of(Planning_Faculty)>();	
				planner->Parent_Person<ComponentType*>(this);
				planner->Initialize<NULLTYPE>();
				this->Planning_Faculty<ComponentType,ComponentType,planner_itf*>(planner);

				// Create and Initialize the routing faculty
				define_component_interface(_Routing_Interface, type_of(router), Routing_Components::Prototypes::Routing_Prototype, ComponentType);
				define_component_interface(_Network_Interface, type_of(network_reference), Network_Components::Prototypes::Network_Prototype, ComponentType);
				_Routing_Interface* router=(_Routing_Interface*)Allocate<typename _Routing_Interface::Component_Type>();
				router->template traveler<ComponentType*>(this);
				router->template network<_Network_Interface*>(this->network_reference<ComponentType,CallerType,_Network_Interface*>());	

				// Create and Initialize the vehicle
				define_component_interface(_Vehicle_Interface, type_of(vehicle), Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);
				_Vehicle_Interface* vehicle = (_Vehicle_Interface*)Allocate<typename _Vehicle_Interface::Component_Type>(); 	
				vehicle->template uuid<int>(id);
				vehicle->template internal_id<int>(id);
				vehicle->template traveler<ComponentType*>(this);

				// Seed the RNG with the agent ID
				define_component_interface(rng_itf,type_of(RNG),RNG_Components::Prototypes::RNG_Prototype,ComponentType);
				rng_itf* rng = this->RNG<ComponentType,ComponentType,rng_itf*>();		
				rng->Initialize<TargetType>((sin((double)id)+1.0) * 1000000.0);

				// Add basic traveler properties							
				this->template uuid<ComponentType,ComponentType,int>(id);
				this->template internal_id<ComponentType,ComponentType,int>(id);
				this->template router<ComponentType,ComponentType,_Routing_Interface*>(router);
				this->template vehicle<ComponentType,ComponentType,_Vehicle_Interface*>(vehicle);
			}
			feature_implementation void Initialize(typename TargetType::ParamType id, typename TargetType::Param2Type trip)
			{	
				// Set the initial iteration to process
				this->First_Iteration<Time_Minutes>(15.0);

				// Set the agent ID
				this->uuid<ComponentType,CallerType,TargetType::ParamType>(id);
				this->internal_id<ComponentType,CallerType,TargetType::ParamType>(id);

				// Create and Initialize the Properties faculty
				define_component_interface(properties_itf,type_of(Properties),Prototypes::Person_Properties,ComponentType);
				properties_itf* properties = (properties_itf*)Allocate<type_of(Properties)>();
				properties->Initialize<NULLTYPE>();
				properties->Parent_Person<ComponentType*>(this);
				this->Properties<ComponentType,ComponentType,properties_itf*>(properties);

				// Create and Initialize the Planner faculty
				define_component_interface(planner_itf,type_of(Planning_Faculty),Prototypes::Person_Planner,ComponentType);
				planner_itf* planner = (planner_itf*)Allocate<type_of(Planning_Faculty)>();
				planner->Initialize<TargetType::Param2Type>(trip);
				planner->Parent_Person<ComponentType*>(this);
				this->Planning_Faculty<ComponentType,ComponentType,planner_itf*>(planner);		

				// Seed the RNG with the agent ID
				define_component_interface(rng_itf,type_of(RNG),RNG_Components::Prototypes::RNG_Prototype,ComponentType);
				rng_itf* rng = this->RNG<ComponentType,ComponentType,rng_itf*>();	
				rng->Initialize<TargetType::ParamType>(id);
			}
			tag_feature_as_available(Initialize);	

			member_component(typename MasterType::vehicle_type,vehicle,none,none/*check_2(ComponentType,CallerType, Is_Same_Entity)*/);
			member_component(typename MasterType::routing_type,router,none,none/*check_2(ComponentType,CallerType, Is_Same_Entity)*/);
			member_component(typename MasterType::person_properties_type,Properties,none,check_2(ComponentType,CallerType, Is_Same_Entity));
			member_component(typename MasterType::person_planner_type,Planning_Faculty,none,check_2(ComponentType,CallerType, Is_Same_Entity));

			member_component(typename MasterType::scenario_type, scenario_reference, none, none);
			member_component(typename MasterType::network_type, network_reference, none, none);
			
			// Random number facility - accessed using Next_Rand feature_accessor
			member_data_component(typename MasterType::RNG, RNG, check(ReturnValueType,RNG_Components::Concepts::Is_RNG), check(SetValueType,RNG_Components::Concepts::Is_RNG) && check_2(ComponentType,CallerType, Is_Same_Entity));
			member_component_feature(Next_Rand, RNG, Next_Rand, RNG_Components::Prototypes::RNG_Prototype);

			// Agent ID
			member_data(long,uuid,check(ReturnValueType,is_arithmetic),check(SetValueType,is_arithmetic));
			member_data(long,internal_id,check(ReturnValueType,is_arithmetic),check(SetValueType,is_arithmetic));

			// First iteration  - sets the next iteration after all planning is completed
			member_data_component(typename Basic_Units::Implementations::Time_Implementation<MasterType>,_First_Iteration,none,none);
			member_component_feature(First_Iteration, _First_Iteration, Value, Basic_Units::Prototypes::Time_Prototype);

		};

		implementation struct General_Person_Planner_Implementation
		{
			// Pointer to the Parent class
			member_component(typename MasterType::person_type, Parent_Person, none, none);

			// Next Activity Generation Time member - used to schedule the next activity generation
			member_data_component(typename Basic_Units::Implementations::Time_Implementation<MasterType>,_Generation_Time,none,none);
			member_component_feature(Next_Activity_Generation_Time, _Generation_Time, Value, Basic_Units::Prototypes::Time_Prototype);
			// Planning Time Increment member - sets the next iteration after all planning is completed
			member_data_component(typename Basic_Units::Implementations::Time_Implementation<MasterType>,_Planning_Time_Increment,none,none);
			member_component_feature(Planning_Time_Increment, _Planning_Time_Increment, Value, Basic_Units::Prototypes::Time_Prototype);
			// Generation Time Increment member - sets the next Generation iteration 
			member_data_component(typename Basic_Units::Implementations::Time_Implementation<MasterType>,_Generation_Time_Increment,none,none);
			member_component_feature(Generation_Time_Increment, _Generation_Time_Increment, Value, Basic_Units::Prototypes::Time_Prototype);
			//member_data(Simulation_Timestep_Increment,Generation_Time_Increment,none,none);

			// Containers for activity planning events and movement planning events
			member_associative_container(concat(hash_multimap<long,typename MasterType::activity_plan_type*>),Activity_Plans_Container,none,none);
			member_associative_container(concat(hash_multimap<long,typename MasterType::movement_plan_type*>),Movement_Plans_Container,none,none);

			// Activity generation functionality
			feature_implementation void Activity_Generation()
			{
			}
			tag_feature_as_available(Activity_Generation);
			feature_implementation TargetType current_movement_plan(requires(check(TargetType,is_pointer) && check(TargetType,Movement_Plan_Components::Concepts::Is_Movement_Plan_Prototype)))
			{
				// Define interfaces to the container members of the class			
				define_container_and_value_interface(Movement_Plans_List,Movement_Plan,type_of(Movement_Plans_Container),Associative_Container_Prototype,Movement_Plan_Prototype,ComponentType);		
				Movement_Plans_List* movement_plans = this_ptr->Movement_Plans_Container<ComponentType, CallerType, Movement_Plans_List*>();
				Movement_Plans_List::iterator itr;
				if ((itr = movement_plans->find(_iteration)) != movement_plans->end()) return (TargetType)*itr;
				else return NULL;
			}
			tag_feature_as_available(current_movement_plan);
			feature_implementation TargetType current_activity_plan(requires(check(TargetType,is_pointer) && check(TargetType,Activity_Components::Concepts::Is_Activity_Plan_Prototype)))
			{
				// Define interfaces to the container members of the class
				define_container_and_value_interface(Activity_Plans_List,Activity_Plan,type_of(Activity_Plans_Container),Associative_Container_Prototype,Activity_Plan_Prototype,ComponentType);
				Activity_Plans_List* activity_plans = this_ptr->Activity_Plans_Container<Activity_Plans_List*>();
				Activity_Plans_List::iterator itr;
				if ((itr = activity_plans->find(_iteration)) != activity_plans->end()) return (TargetType)*itr;
				else return NULL;
			}
			tag_feature_as_available(current_activity_plan);

			// Adding activities and movements to the planning schedules
			feature_implementation void Add_Movement_Plan(TargetType movement_plan, requires(check_as_given(TargetType,is_pointer) && check(TargetType,Movement_Plan_Components::Concepts::Is_Movement_Plan_Prototype)))
			{
				define_container_and_value_interface(Movement_Plans,Movement_Plan,type_of(Movement_Plans_Container),Associative_Container_Prototype,Movement_Plan_Components::Prototypes::Movement_Plan_Prototype,ComponentType);
				Movement_Plan* move = (Movement_Plan*)movement_plan;
				// key the movement plan on the planning timestep just prior to departure
				long t1 = move->departed_time<Simulation_Timestep_Increment>();
				long t2 = this->Planning_Time_Increment<ComponentType,CallerType,Simulation_Timestep_Increment>();
				long remain = (long)(t1 / t2);
				Simulation_Timestep_Increment departure_time = remain * this->Planning_Time_Increment<ComponentType,CallerType,Simulation_Timestep_Increment>();
				Movement_Plans* movements = this->Movement_Plans_Container<ComponentType,CallerType,Movement_Plans*>();
				movements->insert(departure_time,move);
			}
			feature_implementation void Add_Movement_Plan(TargetType movement_plan, requires(!check_as_given(TargetType,is_pointer) || !check(TargetType,Movement_Plan_Components::Concepts::Is_Movement_Plan_Prototype)))
			{
				assert_check_as_given(TargetType,is_pointer, "Error, TargetType must be passed as a pointer");
				assert_check(TargetType, Movement_Plan_Components::Concepts::Is_Movement_Plan_Prototype, "Error, Function requires TargetType to be a Movement_Plan_Prototype.");
			}
			tag_feature_as_available(Add_Movement_Plan);
			feature_implementation void Add_Activity_Plan(TargetType activity_plan, requires(check_as_given(TargetType,is_pointer) && check(TargetType,Activity_Components::Concepts::Is_Activity_Plan_Prototype)))
			{
				define_container_and_value_interface(Activity_Plans,Activity_Plan,type_of(Activity_Plans_Container),Associative_Container_Prototype,Activity_Components::Prototypes::Activity_Plan_Prototype,ComponentType);	
				Activity_Plan* act = (Activity_Plan*)activity_plan;
				// key the movement plan on the planning timestep just prior to departure
				Activity_Plans* activities = this_ptr->Activity_Plans_Container<Activity_Plans*>();
				activities->insert(departure_time,move);
			}
		};

		implementation struct ADAPTS_Person_Planner_Implementation : public Polaris_Component_Class<ADAPTS_Person_Planner_Implementation,MasterType,Execution_Object,ParentType>, public General_Person_Planner_Implementation<MasterType, ParentType>
		{
			feature_implementation void Initialize(requires(check(typename ComponentType::Parent_Type,Concepts::Is_Person)))
			{	
				this->Generation_Time_Increment<ComponentType,CallerType,Time_Minutes>(15);
				this->Planning_Time_Increment<ComponentType,CallerType,Time_Minutes>(15);

				// get reference to the parent pointer and set the first activity generation time to be the parent first iteration
				define_component_interface(parent_itf,get_type_of(Parent_Person),Prototypes::Person_Prototype,ComponentType);
				parent_itf* parent = this->Parent_Person<ComponentType,CallerType,parent_itf*>();
				this->Next_Activity_Generation_Time<ComponentType,CallerType,Time_Minutes>(parent->First_Iteration<Time_Minutes>());	
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
				typedef Prototypes::Person_Planner<ComponentType, ComponentType> _Planning_Interface;
				_Planning_Interface* this_ptr=(_Planning_Interface*)this;

				// get reference to the parent pointer
				define_component_interface(parent_itf,get_type_of(Parent_Person),Prototypes::Person_Prototype,ComponentType);
				parent_itf* parent = this->Parent_Person<ComponentType,CallerType,parent_itf*>();

				// Generate average of 4 activities per day

				define_container_and_value_interface(Activity_Plans,Activity_Plan,type_of(Activity_Plans_Container),Associative_Container_Prototype,Prototypes::Activity_Plan_Prototype,ComponentType);
				Activity_Plan* act = (Activity_Plan*)Allocate<type_of(Activity_Plans_Container)::unqualified_value_type>();
				act->Activity_Plan_ID<long>(_iteration);
				Activity_Plans* activities = this_ptr->Activity_Plans_Container<Activity_Plans*>();
				activities->insert(Simulation_Time.Future_Time<Time_Minutes,Simulation_Timestep_Increment>(15),act);

				define_container_and_value_interface(Movement_Plans,Movement_Plan,type_of(Movement_Plans_Container),Associative_Container_Prototype,Prototypes::Movement_Plan_Prototype,ComponentType);
				Movement_Plan* move = (Movement_Plan*)Allocate<type_of(Movement_Plans_Container)::unqualified_value_type>();
				move->Movement_Plan_ID<long>(_iteration);
				Movement_Plans* movements = this_ptr->Movement_Plans_Container<Movement_Plans*>();
				movements->insert(Simulation_Time.Future_Time<Time_Minutes,Simulation_Timestep_Increment>(30),move);
			}
			tag_feature_as_available(Activity_Generation);
		};

		implementation struct CTRAMP_Person_Planner_Implementation : public Polaris_Component_Class<CTRAMP_Person_Planner_Implementation,MasterType,Execution_Object,ParentType>, public General_Person_Planner_Implementation<MasterType, ParentType>
		{
			feature_implementation void Initialize(requires(check(typename ComponentType::Parent_Type,Concepts::Is_Person)))
			{	
				this->Generation_Time_Increment<ComponentType,CallerType,Time_Minutes>(END);
				this->Planning_Time_Increment<ComponentType,CallerType,Time_Minutes>(15);
				this->Next_Activity_Generation_Time<ComponentType,CallerType,Time_Minutes>(0);	
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
				typedef Prototypes::Person_Planner<ComponentType, ComponentType> _Planning_Interface;
				_Planning_Interface* this_ptr=(_Planning_Interface*)this;

				// get reference to the parent pointer
				define_component_interface(parent_itf,get_type_of(Parent_Person),Prototypes::Person_Prototype,ComponentType);
				parent_itf* parent = this->Parent_Person<ComponentType,CallerType,parent_itf*>();

				// get references to the plan containers
				define_container_and_value_interface(Activity_Plans,Activity_Plan,type_of(Activity_Plans_Container),Associative_Container_Prototype,Activity_Components::Prototypes::Activity_Plan_Prototype,ComponentType);
				define_container_and_value_interface(Movement_Plans,Movement_Plan,type_of(Movement_Plans_Container),Associative_Container_Prototype,Movement_Plan_Components::Prototypes::Movement_Plan_Prototype,ComponentType);
				Activity_Plans* activities = this_ptr->Activity_Plans_Container<Activity_Plans*>();
				Movement_Plans* movements = this_ptr->Movement_Plans_Container<Movement_Plans*>();	

				// external knowledge references
				define_component_interface(_Scenario_Interface, get_type_of(Parent_Person)::type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
				define_component_interface(_Network_Interface, get_type_of(Parent_Person)::type_of(network_reference), Network_Components::Prototypes::Network_Prototype, ComponentType);	
				define_container_and_value_interface(_Activity_Locations_Container_Interface, _Activity_Location_Interface, _Network_Interface::get_type_of(activity_locations_container), Random_Access_Sequence_Prototype, Activity_Location_Components::Prototypes::Activity_Location_Prototype, ComponentType);
				define_container_and_value_interface(_Links_Container_Interface, _Link_Interface, _Activity_Location_Interface::get_type_of(origin_links), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_container_and_value_interface(_Zones_Container_Interface, _Zone_Interface, _Network_Interface::get_type_of(zones_container), Random_Access_Sequence_Prototype, Zone_Components::Prototypes::Zone_Prototype, ComponentType);
				_Network_Interface* network = parent->network_reference<_Network_Interface*>();
				_Scenario_Interface* scenario = parent->scenario_reference<_Scenario_Interface*>();

				// Generate average of 3 activities per day
				double r = parent->Next_Rand<double>();
				int num_activities = r *6.0 + 1;

				double t = 0;

				for (int i = 0; i < num_activities; i++)
				{
					// Create movement plan and give it an ID
					Movement_Plan* move = (Movement_Plan*)Allocate<type_of(Movement_Plans_Container)::unqualified_value_type>();

					// Get a random origin and destination and departure time
					int size = network->links_container<_Links_Container_Interface&>().size();
					int O = (parent->Next_Rand<double>()-0.000001) * size;
					int D = (parent->Next_Rand<double>()-0.000001) * size;
					// departure time from current time, in minutes
					t += parent->Next_Rand<double>()*6.0*60.0;

					Basic_Units::Time_Variables::Time_Minutes time = Simulation_Time.Future_Time<Basic_Units::Time_Variables::Time_Minutes, Basic_Units::Time_Variables::Time_Minutes>(t);
		
					if (O != D)
					{
						move->template origin<_Link_Interface*>(network->links_container<_Links_Container_Interface&>().at(O));
						move->template destination<_Link_Interface*>(network->links_container<_Links_Container_Interface&>().at(D));
						move->template departed_time<Basic_Units::Time_Variables::Time_Minutes>(time);
					}

					// Add to plans
					this_ptr->Add_Movement_Plan<Movement_Plan*>(move);
				}
			}
			tag_feature_as_available(Activity_Generation);
		};
		
		implementation struct TRANSIMS_Person_Planner_Implementation : public Polaris_Component_Class<TRANSIMS_Person_Planner_Implementation,MasterType,Execution_Object,ParentType>, public General_Person_Planner_Implementation<MasterType, ParentType>
		{
			feature_implementation void Initialize(TargetType trip, requires(check(typename ComponentType::Parent_Type,Concepts::Is_Person)))
			{	
				// Set the event schedule parameters
				this->Generation_Time_Increment<ComponentType,CallerType,Time_Minutes>(END);
				this->Planning_Time_Increment<ComponentType,CallerType,Time_Minutes>(15);
				this->Next_Activity_Generation_Time<ComponentType,CallerType,Time_Minutes>(END);

				// Create alias for this to use in conditional
				typedef Prototypes::Person_Planner<ComponentType, ComponentType> _Planning_Interface;
				_Planning_Interface* this_ptr=(_Planning_Interface*)this;

				// Add the trip input parameter to the movement schedule
				define_container_and_value_interface(Movement_Plans,Movement_Plan,type_of(Movement_Plans_Container),Associative_Container_Prototype,Movement_Plan_Components::Prototypes::Movement_Plan_Prototype,ComponentType);
				Movement_Plans* movements = this_ptr->Movement_Plans_Container<Movement_Plans*>();
				Movement_Plan* move = (Movement_Plan*)trip;
				this->Add_Movement_Plan<ComponentType,CallerType,Movement_Plan*>(move);
			
			} 
			feature_implementation void Initialize(TargetType trip, requires(check(typename ComponentType::Parent_Type,!Concepts::Is_Person)))
			{	
				assert_check(typename ComponentType::Parent_Type,Concepts::Is_Person, "The specified ParentType is not a valid Person type.");
			}
			feature_implementation void Initialize()
			{	
				static_assert(false, "\n\n\n[--------- Error, a TRANSIMS Planner Initialization must be called with an input trip parameter. ---------]\n\n");
			}
			tag_feature_as_available(Initialize);
		};



		implementation struct ADAPTS_Person_Properties_Implementation : public Polaris_Component_Class<ADAPTS_Person_Properties_Implementation,MasterType,Data_Object,ParentType>
		{
			member_component(typename MasterType::person_type, Parent_Person, none, none);

			feature_implementation void Initialize(requires(check_2(ComponentType,CallerType,Is_Same_Entity)))
			{	
			}	tag_feature_as_available(Initialize);

			// Length member
			member_data_component(Basic_Units::Implementations::Length_Implementation<MasterType>,Length,none,none);
			member_component_feature(My_Length, Length, Value, Basic_Units::Prototypes::Length_Prototype);
			// Volume member
			member_data_component(typename Basic_Units::Implementations::Area_Implementation<MasterType>,Area,none,none);
			member_component_feature(My_Area, Area, Value, Basic_Units::Prototypes::Area_Prototype);
			// Time member
			member_data_component(typename Basic_Units::Implementations::Time_Implementation<MasterType>,Time,none,none);
			member_component_feature(My_Time, Time, Value, Basic_Units::Prototypes::Time_Prototype);
			// Speed member
			member_data_component(typename Basic_Units::Implementations::Speed_Implementation<MasterType>,Speed,none,none);
			member_component_feature(My_Speed, Speed, Value, Basic_Units::Prototypes::Speed_Prototype);
			// Income member
			member_data_component(typename Basic_Units::Implementations::Currency_Implementation<MasterType>,Income,none,none);
			member_component_feature(HH_Income, Income, Value, Basic_Units::Prototypes::Currency_Prototype);
		};

		implementation struct TRANSIMS_Person_Properties_Implementation : public Polaris_Component_Class<TRANSIMS_Person_Properties_Implementation,MasterType,Data_Object,ParentType>
		{
			member_component(typename MasterType::person_type, Parent_Person, none, none);

			feature_implementation void Initialize(requires(check_2(ComponentType,CallerType,Is_Same_Entity)))
			{	
				//this->Length<ComponentType, CallerType, type_of(Length)*>(Allocate<type_of(Length)>());
				//this->Area<ComponentType, CallerType, type_of(Area)*>(Allocate<type_of(Area)>());
				//this->Time<ComponentType, CallerType, type_of(Time)*>(Allocate<type_of(Time)>());
				//this->Speed<ComponentType, CallerType,type_of(Speed)*>(Allocate<type_of(Speed)>());
				//this->Income<ComponentType, CallerType, type_of(Income)*>(Allocate<type_of(Income)>());
			}	tag_feature_as_available(Initialize);

			// Length member
			member_data_component(Basic_Units::Implementations::Length_Implementation<MasterType>,Length,none,none);
			member_component_feature(My_Length, Length, Value, Basic_Units::Prototypes::Length_Prototype);
			// Volume member
			member_data_component(typename Basic_Units::Implementations::Area_Implementation<MasterType>,Area,none,none);
			member_component_feature(My_Area, Area, Value, Basic_Units::Prototypes::Area_Prototype);
			// Time member
			member_data_component(typename Basic_Units::Implementations::Time_Implementation<MasterType>,Time,none,none);
			member_component_feature(My_Time, Time, Value, Basic_Units::Prototypes::Time_Prototype);
			// Speed member
			member_data_component(typename Basic_Units::Implementations::Speed_Implementation<MasterType>,Speed,none,none);
			member_component_feature(My_Speed, Speed, Value, Basic_Units::Prototypes::Speed_Prototype);
			// Income member
			member_data_component(typename Basic_Units::Implementations::Currency_Implementation<MasterType>,Income,none,none);
			member_component_feature(HH_Income, Income, Value, Basic_Units::Prototypes::Currency_Prototype);
		};

	}
}