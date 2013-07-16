#pragma once
#include "Activity_Prototype.h"


namespace Activity_Components
{
	namespace Types
	{

	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{
		//=======================================================================
		// Activity Classes
		//-----------------------------------------------------------------------
		implementation struct Basic_Activity_Plan_Implementation : public Polaris_Component<APPEND_CHILD(Basic_Activity_Plan_Implementation),MasterType,Execution_Object,ParentType>
		{
			// Tag As Implementation
			typedef typename Polaris_Component<APPEND_CHILD(Basic_Activity_Plan_Implementation),MasterType,Execution_Object>::Component_Type ComponentType;


			//=================================================================
			// Pointer back to planner
			member_prototype(Person_Components::Prototypes::Person_Planner, Parent_Planner, typename MasterType::person_planner_type, none, none);

			// pointer to movement plan associated with activity
			member_prototype(Movement_Plan_Components::Prototypes::Movement_Plan_Prototype, movement_plan, typename MasterType::movement_plan_type,none,none);
			//member_prototype(Movement_Plan_Components::Prototypes::Movement_Plan_Prototype, movement_record, typename MasterType::movement_plan_record_type,none,none);

			static int Route_Planning_Counter[_num_threads];
			static member_data(int, Route_Planning_Count,none,none);
			static member_data(_lock, update_lock,none,none);

			//================================================================================================================================================================================================
			//================================================================================================================================================================================================
			// Interfaces
			typedef Prototypes::Activity_Planner<ComponentType,ComponentType> this_itf;
			define_component_interface(_person_itf, typename type_of(Parent_Planner)::type_of(Parent_Person), Person_Components::Prototypes::Person, ComponentType);
			define_component_interface(_scheduler_itf, typename type_of(Parent_Planner)::type_of(Parent_Person)::type_of(Scheduling_Faculty), Person_Components::Prototypes::Person_Scheduler, ComponentType);
			define_component_interface(_properties_itf, typename type_of(Parent_Planner)::type_of(Parent_Person)::type_of(Properties), Person_Components::Prototypes::Person_Properties, ComponentType);
			define_component_interface(_planning_itf,type_of(Parent_Planner),Person_Components::Prototypes::Person_Planner,ComponentType);
			define_component_interface(_dest_choice_itf,typename _planning_itf::get_type_of(Destination_Choice_Faculty),Person_Components::Prototypes::Destination_Chooser,ComponentType);
			define_component_interface(_scenario_itf, typename type_of(Parent_Planner)::type_of(Parent_Person)::type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
			define_component_interface(_network_itf, typename type_of(Parent_Planner)::type_of(Parent_Person)::type_of(network_reference), Network_Components::Prototypes::Network_Prototype, ComponentType);	
			define_component_interface(_skim_itf, typename _network_itf::get_type_of(skimming_faculty),Network_Skimming_Components::Prototypes::Network_Skimming_Prototype,ComponentType);
			define_container_and_value_interface(_activity_locations_container_itf, _activity_location_itf, typename _network_itf::get_type_of(activity_locations_container), Random_Access_Sequence_Prototype, Activity_Location_Components::Prototypes::Activity_Location_Prototype, ComponentType);
			define_container_and_value_interface(_links_container_itf, _link_itf, typename _activity_location_itf::get_type_of(origin_links), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
			define_container_and_value_interface(_turns_container_itf, _turn_itf, typename _link_itf::get_type_of(outbound_turn_movements), Random_Access_Sequence_Prototype, Turn_Movement_Components::Prototypes::Movement_Prototype, ComponentType);
			define_container_and_value_interface(_zones_container_itf, _zone_itf, typename _network_itf::get_type_of(zones_container), Associative_Container_Prototype, Zone_Components::Prototypes::Zone_Prototype, ComponentType);
			define_container_and_value_interface(_activity_plans_container_itf,_activity_plan_itf, typename _scheduler_itf::get_type_of(Activity_Container),Containers::Back_Insertion_Sequence_Prototype,Activity_Components::Prototypes::Activity_Planner,ComponentType);
			define_container_and_value_interface(_movement_plans_container_itf,_movement_plan_itf, typename _scheduler_itf::get_type_of(Movement_Plans_Container),Containers::Back_Insertion_Sequence_Prototype,Movement_Plan_Components::Prototypes::Movement_Plan_Prototype,ComponentType);
			//================================================================================================================================================================================================
			//================================================================================================================================================================================================

		
			// Fundamental activity properties
			member_data(int, Activity_Plan_ID, check(ReturnValueType,is_arithmetic), check(SetValueType,is_arithmetic));
			member_data(Types::ACTIVITY_TYPES, Activity_Type,none,none);
			feature_implementation TargetType Parent_ID()
			{
				return this->_Parent_Planner->template Parent_Person<_person_itf*>()->template uuid<TargetType>();
			}	tag_getter_as_available(Parent_ID);
			
			member_data(Types::FLEXIBILITY_VALUES, Location_Flexibility, none, none);
			member_data(Types::FLEXIBILITY_VALUES, Mode_Flexibility, none, none);
			member_data(Types::FLEXIBILITY_VALUES, Start_Time_Flexibility,none,none);
			member_data(Types::FLEXIBILITY_VALUES, Duration_Flexibility,none,none);
			member_data(Types::FLEXIBILITY_VALUES, Involved_Persons_Flexibility,none,none);

			// Activity attributes
			member_prototype(Activity_Location_Components::Prototypes::Activity_Location_Prototype, Location, typename MasterType::activity_location_type, check(ReturnValueType,Activity_Location_Components::Concepts::Is_Activity_Location), check(SetValueType,Activity_Location_Components::Concepts::Is_Activity_Location));
			member_data(Vehicle_Components::Types::Vehicle_Type_Keys, Mode, none, none);
			member_component_and_feature_accessor(Start_Time, Value, Basic_Units::Prototypes::Time_Prototype,Basic_Units::Implementations::Time_Implementation<NT>)
			member_component_and_feature_accessor(Duration, Value, Basic_Units::Prototypes::Time_Prototype,Basic_Units::Implementations::Time_Implementation<NT>)
			member_component_and_feature_accessor(Expected_Travel_Time, Value, Basic_Units::Prototypes::Time_Prototype, Basic_Units::Implementations::Time_Implementation<NT>);
			member_component_and_feature_accessor(Actual_Travel_Time, Value, Basic_Units::Prototypes::Time_Prototype, Basic_Units::Implementations::Time_Implementation<NT>);
			member_container(vector<typename MasterType::person_type*>, Involved_Persons_Container, none, none);
			feature_implementation TargetType End_Time()
			{
				this_itf* pthis = (this_itf*)this;
				return pthis->template Start_Time<TargetType>() + pthis->template Duration<TargetType>();
			}	tag_getter_as_available(End_Time);
			feature_implementation TargetType End_Time(TargetType value, bool maintain_duration)
			{
				this_itf* pthis = (this_itf*)this;

				if (maintain_duration) pthis->template Start_Time<TargetType>(value - pthis->template Duration<TargetType>());
				else
				{
					if (value < pthis->template Start_Time<TargetType>()) 
					{
						THROW_WARNING("WARNING: new end time less than start time is not possible if start time is fixed (maintain_duration=false)");
					}
					else pthis->template Duration<TargetType>(value - pthis->template Start_Time<TargetType>());
				}
			}	tag_setter_as_available(End_Time);

			// Planning event time members
			member_data(Revision,Location_Planning_Time,check_2(strip_modifiers(ReturnValueType),Revision,is_same), check_2(strip_modifiers(SetValueType),Revision,is_same));
			member_data(Revision,Mode_Planning_Time,check_2(strip_modifiers(ReturnValueType),Revision,is_same), check_2(strip_modifiers(SetValueType),Revision,is_same));
			member_data(Revision,Start_Time_Planning_Time,check_2(strip_modifiers(ReturnValueType),Revision,is_same), check_2(strip_modifiers(SetValueType),Revision,is_same));
			member_data(Revision,Duration_Planning_Time,check_2(strip_modifiers(ReturnValueType),Revision,is_same), check_2(strip_modifiers(SetValueType),Revision,is_same));
			member_data(Revision,Involved_Persons_Planning_Time,check_2(strip_modifiers(ReturnValueType),Revision,is_same), check_2(strip_modifiers(SetValueType),Revision,is_same));
			member_data(Revision,Route_Planning_Time,check_2(strip_modifiers(ReturnValueType),Revision,is_same), check_2(strip_modifiers(SetValueType),Revision,is_same));

			member_data(Revision,Stored_Location_Planning_Time,check_2(strip_modifiers(ReturnValueType),Revision,is_same), check_2(strip_modifiers(SetValueType),Revision,is_same));
			member_data(Revision,Stored_Mode_Planning_Time,check_2(strip_modifiers(ReturnValueType),Revision,is_same), check_2(strip_modifiers(SetValueType),Revision,is_same));
			member_data(Revision,Stored_Start_Time_Planning_Time,check_2(strip_modifiers(ReturnValueType),Revision,is_same), check_2(strip_modifiers(SetValueType),Revision,is_same));
			member_data(Revision,Stored_Duration_Planning_Time,check_2(strip_modifiers(ReturnValueType),Revision,is_same), check_2(strip_modifiers(SetValueType),Revision,is_same));
			member_data(Revision,Stored_Involved_Persons_Planning_Time,check_2(strip_modifiers(ReturnValueType),Revision,is_same), check_2(strip_modifiers(SetValueType),Revision,is_same));
			member_data(Revision,Stored_Route_Planning_Time,check_2(strip_modifiers(ReturnValueType),Revision,is_same), check_2(strip_modifiers(SetValueType),Revision,is_same));

			feature_implementation bool Location_Is_Planned(){return (_Location_Planning_Time._iteration >= (int)END && _Location_Planning_Time._sub_iteration >= (int)END);} tag_feature_as_available(Location_Is_Planned);
			feature_implementation bool Mode_Is_Planned(){return (_Mode_Planning_Time._iteration >= (int)END && _Mode_Planning_Time._sub_iteration >= (int)END);}	tag_feature_as_available(Mode_Is_Planned);
			feature_implementation bool Duration_Is_Planned(){return (_Duration_Planning_Time._iteration >= (int)END && _Duration_Planning_Time._sub_iteration >= (int)END);}	tag_feature_as_available(Duration_Is_Planned);
			feature_implementation bool Involved_Persons_Is_Planned(){return (_Involved_Persons_Planning_Time._iteration >= (int)END && _Involved_Persons_Planning_Time._sub_iteration >= (int)END);}	tag_feature_as_available(Involved_Persons_Is_Planned);
			feature_implementation bool Start_Is_Planned(){return (_Start_Time_Planning_Time._iteration >= (int)END && _Start_Time_Planning_Time._sub_iteration >= (int)END);} tag_feature_as_available(Start_Is_Planned);
			feature_implementation bool Route_Is_Planned(){return (_Route_Planning_Time._iteration >=(int)END && _Route_Planning_Time._sub_iteration >= (int)END);} tag_feature_as_available(Route_Is_Planned);

			// Basic Activity Events - Plan route and add to schedule
			feature_implementation void Initialize(TargetType act_type)
			{
				UNLOCK(this->_update_lock);
				this_itf* pthis = (this_itf*)this;
				pthis->Activity_Type<TargetType>(act_type);
				pthis->Duration<Time_Seconds>(END);
				pthis->Start_Time<Time_Seconds>(END);
				pthis->Location<_activity_location_itf*>(nullptr);
				pthis->Mode<Vehicle_Components::Types::Vehicle_Type_Keys>(Vehicle_Components::Types::Vehicle_Type_Keys::SOV);
			}
			tag_feature_as_available(Initialize);

			feature_implementation void Set_Attribute_Planning_Times(TargetType planning_time, requires(check_2(TargetType, Simulation_Timestep_Increment, is_same)))
			{
				Revision &start = this->Start_Time_Planning_Time<ComponentType, CallerType, Revision&>();
				Revision &dur = this->Duration_Planning_Time<ComponentType, CallerType, Revision&>();
				Revision &loc = this->Location_Planning_Time<ComponentType, CallerType, Revision&>();
				Revision & mode = this->Mode_Planning_Time<ComponentType, CallerType, Revision&>();
				Revision & persons = this->Involved_Persons_Planning_Time<ComponentType, CallerType, Revision&>();
				Revision &route = this->Route_Planning_Time<ComponentType, CallerType, Revision&>();
				
				start._iteration = END;				
				dur._iteration = END;				
				loc._iteration = END;				
				mode._iteration = END;				
				persons._iteration = END;			
				route._iteration = std::min(_iteration+1, (int)planning_time);

				start._sub_iteration = 0;	
				dur._sub_iteration = 0;
				loc._sub_iteration = 0;
				mode._sub_iteration = 0;
				persons._sub_iteration = 0;
				route._sub_iteration = 5;	
			}
			feature_implementation void Set_Attribute_Planning_Times(TargetType planning_time, requires(!check_2(TargetType, Simulation_Timestep_Increment, is_same)))
			{
				assert_check_2(TargetType, Simulation_Timestep_Increment, is_same, "Error: planning_time must be passed as a Simulation_Timestep_Increment type when using this function.");
			}
			tag_feature_as_available(Set_Attribute_Planning_Times);

			feature_implementation void Route_Planning_Event_Handler()
			{			
				// Create movement plan and give it an ID
				_movement_plan_itf* move = (_movement_plan_itf*)Allocate<typename _scheduler_itf::get_type_of(Movement_Plans_Container)::unqualified_value_type>();
				move->template initialize_trajectory<NULLTYPE>();
				move->template destination_activity_reference<ComponentType*>((ComponentType*)this);

				// General interfaces, to parent and global classes
				_planning_itf* planner = this->Parent_Planner<ComponentType,CallerType,_planning_itf*>();
				_person_itf* person = planner->template Parent_Person<_person_itf*>();
				_network_itf* network = person->template network_reference<_network_itf*>();
				_scheduler_itf* scheduler = person->template Scheduling_Faculty<_scheduler_itf*>();
				_scenario_itf* scenario = (_scenario_itf*)_global_scenario;

				
				// Get the origin and destination locations
				_activity_location_itf* orig;
				_activity_location_itf* dest = this->Location<ComponentType,CallerType,_activity_location_itf*>();
				Time_Seconds start = this->Start_Time<ComponentType,CallerType,Time_Seconds>();
				this_itf* prev_act = scheduler->template previous_activity_plan<Target_Type<NT,this_itf*, Time_Seconds>>(this->Start_Time<ComponentType,CallerType,Time_Seconds>());		
				if (prev_act == nullptr)  orig = person->template Home_Location<_activity_location_itf*>();
				else
				{
					orig = prev_act->template Location<_activity_location_itf*>();
					// check if a stop at home will fit prior to activity
					Time_Seconds time_before = start - (prev_act->template Start_Time<Time_Seconds>() + prev_act->template Duration<Time_Seconds>());
					Time_Seconds ttime_prev_to_home = network->template Get_LOS<Target_Type<NT,Time_Seconds,int,Vehicle_Type_Keys, Time_Seconds>>(orig->template zone<_zone_itf*>()->template uuid<int>(), person->template Home_Location<_zone_itf*>()->template uuid<int>(),SOV,start);
					Time_Seconds ttime_home_to_this = network->template Get_LOS<Target_Type<NT,Time_Seconds,int,Vehicle_Type_Keys, Time_Seconds>>(person->template Home_Location<_zone_itf*>()->template uuid<int>(),dest->template zone<_zone_itf*>()->template uuid<int>(), SOV,start);
					Time_Seconds ttime_prev_to_this = network->template Get_LOS<Target_Type<NT,Time_Seconds,int,Vehicle_Type_Keys, Time_Seconds>>(orig->template zone<_zone_itf*>()->template uuid<int>(), dest->template zone<_zone_itf*>()->template uuid<int>(),SOV,start);
					// enough time between previous activity and this activity to go home, stay there for a minimimum amount of time (equal to the shortest leg of the return home trip) and get to this activity
					float min_home_time = std::min((float)ttime_prev_to_home,(float)ttime_home_to_this);			
					if (ttime_prev_to_home + ttime_home_to_this < time_before - min_home_time) orig = person->template Home_Location<_activity_location_itf*>();
				}

				// set the reference to the movement plan
				this->movement_plan<ComponentType,CallerType,_movement_plan_itf*>(move);
				
				if (orig == nullptr || dest == nullptr) {THROW_WARNING("Null origin or destination values specified"); return;}

				// update the movement plan with the specified origin/destination
				this->Update_Movement_Plan<ComponentType,CallerType,_activity_location_itf*>(orig,dest,_iteration);

				// exit if no trip is needed
				if (orig->template internal_id<int>() == dest->template internal_id<int>()) return;

				// route if planning router specified
				if(scenario->template do_planner_routing<bool>()) planner->template Schedule_New_Routing<_movement_plan_itf*>(_iteration+1, move);

				// check that origin and destination are valid
				//if (orig != nullptr && dest != nullptr) 
				//{
				//	// If the trip is valid, assign to a movement plan and add to the schedule
				//	if (orig->template origin_links<_links_container_itf&>().size() != 0 && dest->template origin_links<_links_container_itf&>().size() != 0)
				//	{		
				//		// add attributes to plan
				//		move->template origin<_activity_location_itf*>(orig);
				//		move->template destination<_activity_location_itf*>(dest);
				//		move->template origin<_link_itf*>(orig->template origin_links<_links_container_itf&>().at(0));
				//		move->template destination<_link_itf*>(dest->template origin_links<_links_container_itf&>().at(0));

				//		if (move->template origin<_link_itf*>()->template outbound_turn_movements<_turns_container_itf*>()->size() == 0 || move->template destination<_link_itf*>()->template outbound_turn_movements<_turns_container_itf*>()->size() == 0)
				//		{
				//			_link_itf* o_link =move->template origin<_link_itf*>();
				//			_link_itf* d_link =move->template destination<_link_itf*>();
				//			//THROW_WARNING("WARNING: cannot route trip as orig or dest links do not have valid turn movements: [Perid.actid,acttype,orig_link,dest_link,orig_zone,dest_zone]: "<<concat(this->Parent_ID<ComponentType,CallerType,int>()) << "." << concat(this->Activity_Plan_ID<ComponentType, CallerType,int>()) <<", " << concat(this->Activity_Type<ComponentType, CallerType,ACTIVITY_TYPES>()) << ", " <<o_link->uuid<int>() << ", " << d_link->uuid<int>() << ", "  << orig->zone<_zone_itf*>()->uuid<int>() << ", " << dest->zone<_zone_itf*>()->uuid<int>());
				//			return;
				//		}
				//		
				//		// Add to plan to router schedule only if the trip involves movement, otherwise return
				//		if (orig->template internal_id<int>() == dest->template internal_id<int>()) 
				//		{
				//			return;
				//		}

				//		// shift departure time by estimated travel time, and make sure that it does not occur before next iteration
				//		_skim_itf* skim = person->template network_reference<_network_itf*>()->template skimming_faculty<_skim_itf*>();	
				//		Simulation_Timestep_Increment ttime = skim->template Get_LOS<Target_Type<NT,Simulation_Timestep_Increment,int,Vehicle_Components::Types::Vehicle_Type_Keys>>(orig->template zone<_zone_itf*>()->template uuid<int>(),dest->template zone<_zone_itf*>()->template uuid<int>(),Vehicle_Components::Types::Vehicle_Type_Keys::SOV);
				//		this->Expected_Travel_Time<ComponentType,CallerType,Simulation_Timestep_Increment>(ttime);
				//		Simulation_Timestep_Increment depart = this->Start_Time<ComponentType,CallerType,Simulation_Timestep_Increment>() - ttime;
				//		if (depart < _iteration+1)
				//		{
				//			depart = _iteration+1;
				//			this->Start_Time<ComponentType,CallerType,Simulation_Timestep_Increment>(depart + ttime);
				//		}

				//		// schedule the routing and do routin if requested through scenario, otherwise move to the activity scheduling phase
				//		move->template departed_time<Simulation_Timestep_Increment>(depart);
				//		if(scenario->template do_planner_routing<bool>()) planner->template Schedule_New_Routing<_movement_plan_itf*>(_iteration+1, move);
				//	}
				//	else
				//	{
				//		move->template origin<_activity_location_itf*>(orig);
				//		move->template destination<_activity_location_itf*>(dest);
				//		THROW_WARNING("WARNING: movement from " << orig->template uuid<int>() << " to " << dest->template uuid<int>() << ", can not happen as no origin / destination links are available for the locations.");
				//	}
				//}
				//else
				//{
				//	//----------------------------------------------------------------
				//	// Print to log file
				//	stringstream s;
				//	s <<"ACTIVITY NOT SCHEDULED, null origin or destination: "<< person->template uuid<int>();
				//	s << "," <<orig << ", " <<dest<<endl;
				//	//planner->template Write_To_Log<stringstream&>(s);
				//	//----------------------------------------------------------------
				//}
			}
			feature_implementation void Add_Activity_To_Schedule_Event_Handler()
			{
				// General interfaces
				_person_itf* person = this->_Parent_Planner->template Parent_Person<_person_itf*>();
				_scheduler_itf* scheduler = person->template Scheduling_Faculty<_scheduler_itf*>();
				_movement_plan_itf* move = this->movement_plan<ComponentType,CallerType,_movement_plan_itf*>();
				_scenario_itf* scenario = (_scenario_itf*)_global_scenario;

				// exit if movement plan origin/destination not set
				if (move->template origin<_activity_location_itf*>() == nullptr || move->template destination<_activity_location_itf*>() == nullptr) return;

				if (scenario->template do_planner_routing<bool>())
				{
					// Assign the movement plan to the persons activity schedule, if null movement, leave valid_trajectory to false
					if (move->template valid_trajectory<bool>() || move->template origin<_activity_location_itf*>() == move->template destination<_activity_location_itf*>())
					{			
						Simulation_Timestep_Increment ttime = move->template routed_travel_time<Simulation_Timestep_Increment>();
						Simulation_Timestep_Increment depart = this->Start_Time<ComponentType,CallerType,Simulation_Timestep_Increment>() - ttime;
						if (depart < _Parent_Planner->template Next_Planning_Time<Simulation_Timestep_Increment>())
						{
							depart = _Parent_Planner->template Next_Planning_Time<Simulation_Timestep_Increment>();
							this->Start_Time<ComponentType,CallerType,Simulation_Timestep_Increment>(depart + ttime);
						}

						this->Expected_Travel_Time<ComponentType,CallerType,Simulation_Timestep_Increment>(move->template routed_travel_time<Simulation_Timestep_Increment>());
						move->template departed_time<Simulation_Timestep_Increment>(depart);
						scheduler->template Add_Movement_Plan<_movement_plan_itf*>(move);

					}
					else THROW_WARNING("WARNING: ACTIVITY NOT SCHEDULED, no valid route found from origin to destination. (PERID,ACTID,O,D) "<< person->template uuid<int>() <<","<< concat(this->Activity_Plan_ID<ComponentType,CallerType,int>()) << "," <<concat(move->template origin<_activity_location_itf*>()->template uuid<int>()) << ", " <<concat(move->template destination<_activity_location_itf*>()->template uuid<int>())<< ",at iteration " << _iteration << "." << _sub_iteration<<". Scheduled for route planning @ " << move->template planning_time<Simulation_Timestep_Increment>() << ", and departure @ " << move->template departed_time<Simulation_Timestep_Increment>());		
				}
				else
				{
					Simulation_Timestep_Increment ttime = this->Expected_Travel_Time<ComponentType,CallerType,Simulation_Timestep_Increment>();
					Simulation_Timestep_Increment depart = move->template departed_time<Simulation_Timestep_Increment>();
					if (depart < _Parent_Planner->template Next_Planning_Time<Simulation_Timestep_Increment>())
					{
						depart = _Parent_Planner->template Next_Planning_Time<Simulation_Timestep_Increment>();
						this->Start_Time<ComponentType,CallerType,Simulation_Timestep_Increment>(depart + ttime);
					}
					scheduler->template Add_Movement_Plan<_movement_plan_itf*>(move);
				}
			}		
			feature_implementation void Update_Movement_Plan(TargetType origin, TargetType destination, Simulation_Timestep_Increment min_departure)
			{
				_movement_plan_itf* move = this->movement_plan<ComponentType,CallerType,_movement_plan_itf*>();
				_activity_location_itf* orig = (_activity_location_itf*)origin;
				_activity_location_itf* dest = (_activity_location_itf*)destination;

				// General interfaces, to parent and global classes
				_person_itf* person = this->_Parent_Planner->template Parent_Person<_person_itf*>();
				_network_itf* network = person->template network_reference<_network_itf*>();

				if (orig != nullptr && dest != nullptr) 
				{
					// If the trip is valid, assign to a movement plan and add to the schedule
					if (orig->template origin_links<_links_container_itf&>().size() != 0 && dest->template origin_links<_links_container_itf&>().size() != 0)
					{		
						// add attributes to plan
						move->template origin<_activity_location_itf*>(orig);
						move->template destination<_activity_location_itf*>(dest);
						move->template origin<_link_itf*>(orig->template origin_links<_links_container_itf&>().at(0));
						move->template destination<_link_itf*>(dest->template origin_links<_links_container_itf&>().at(0));

						if (move->template origin<_link_itf*>()->template outbound_turn_movements<_turns_container_itf*>()->size() == 0 || move->template destination<_link_itf*>()->template outbound_turn_movements<_turns_container_itf*>()->size() == 0)
						{
							_link_itf* o_link =move->template origin<_link_itf*>();
							_link_itf* d_link =move->template destination<_link_itf*>();
							//THROW_WARNING("WARNING: cannot route trip as orig or dest links do not have valid turn movements: [Perid.actid,acttype,orig_link,dest_link,orig_zone,dest_zone]: "<<concat(this->Parent_ID<ComponentType,CallerType,int>()) << "." << concat(this->Activity_Plan_ID<ComponentType, CallerType,int>()) <<", " << concat(this->Activity_Type<ComponentType, CallerType,ACTIVITY_TYPES>()) << ", " <<o_link->uuid<int>() << ", " << d_link->uuid<int>() << ", "  << orig->zone<_zone_itf*>()->uuid<int>() << ", " << dest->zone<_zone_itf*>()->uuid<int>());
							return;
						}
						
						// shift departure time by estimated travel time, and make sure that it does not occur before next iteration
						Simulation_Timestep_Increment start = this->Start_Time<ComponentType,CallerType,Simulation_Timestep_Increment>();
						Simulation_Timestep_Increment ttime = network->template Get_LOS<Target_Type<NT,Simulation_Timestep_Increment,int,Vehicle_Components::Types::Vehicle_Type_Keys,Simulation_Timestep_Increment>>(orig->template zone<_zone_itf*>()->template uuid<int>(),dest->template zone<_zone_itf*>()->template uuid<int>(),Vehicle_Components::Types::Vehicle_Type_Keys::SOV,start);
						this->Expected_Travel_Time<ComponentType,CallerType,Simulation_Timestep_Increment>(ttime);
						Simulation_Timestep_Increment depart =  start - ttime;
						if (depart < min_departure)
						{
							depart = min_departure+1;
							this->Start_Time<ComponentType,CallerType,Simulation_Timestep_Increment>(depart + ttime);
						}

						// schedule the routing and do routin if requested through scenario, otherwise move to the activity scheduling phase
						move->template departed_time<Simulation_Timestep_Increment>(depart);
					}
					else
					{
						move->template origin<_activity_location_itf*>(orig);
						move->template destination<_activity_location_itf*>(dest);
						THROW_WARNING("WARNING: movement from " << orig->template uuid<int>() << " to " << dest->template uuid<int>() << ", can not happen as no origin / destination links are available for the locations.");
					}
				}
				else
				{
					THROW_WARNING("Null origin or destination values specified");
				}
			}
			feature_implementation void Arrive_At_Activity()
			{
				define_component_interface(_Logger_Interface, typename MasterType::person_data_logger_type, Person_Components::Prototypes::Person_Data_Logger, NULLTYPE);	
				((_Logger_Interface*)_global_person_logger)->template Add_Record<Activity_Planner<ComponentType,CallerType>*>((Activity_Planner<ComponentType,CallerType>*)this,true);

				// Person has arrived at activity destination
				_Parent_Planner->template Parent_Person<_person_itf*>()->template Arrive_At_Destination<NT>();
			}
			feature_implementation void Depart_From_Activity()
			{

			}
		
		};



		// Specialization for ADAPTS-style activities with plan horizon info and more detailed set_plan_times function
		implementation struct ADAPTS_Activity_Plan_Implementation : public Basic_Activity_Plan_Implementation<MasterType,ParentType,APPEND_CHILD(ADAPTS_Activity_Plan_Implementation)>
		{
			typedef Basic_Activity_Plan_Implementation<MasterType,ParentType,APPEND_CHILD(ADAPTS_Activity_Plan_Implementation)> base_type;
			typedef typename Basic_Activity_Plan_Implementation<MasterType,ParentType,APPEND_CHILD(ADAPTS_Activity_Plan_Implementation)>::Component_Type ComponentType;

			//================================================================================================================================================================================================
			//================================================================================================================================================================================================
			// Interfaces
			typedef Prototypes::Activity_Planner<ComponentType,ComponentType> this_itf;
			define_component_interface(_properties_itf, typename type_of(base_type::Parent_Planner)::type_of(Parent_Person)::type_of(Properties), Person_Components::Prototypes::Person_Properties, ComponentType);
			define_component_interface(_scheduler_itf, typename type_of(base_type::Parent_Planner)::type_of(Parent_Person)::type_of(Scheduling_Faculty), Person_Components::Prototypes::Person_Scheduler, ComponentType);
			define_component_interface(_static_properties_itf, typename type_of(base_type::Parent_Planner)::type_of(Parent_Person)::type_of(Static_Properties), Person_Components::Prototypes::Person_Properties, ComponentType);
			define_component_interface(_planning_itf,typename type_of(base_type::Parent_Planner),Person_Components::Prototypes::Person_Planner,ComponentType);
			define_component_interface(_person_itf,typename _planning_itf::get_type_of(Parent_Person),Person_Components::Prototypes::Person,ComponentType);
			define_component_interface(_dest_choice_itf,typename _planning_itf::get_type_of(Destination_Choice_Faculty),Person_Components::Prototypes::Destination_Chooser,ComponentType);
			define_component_interface(_timing_choice_itf,typename _planning_itf::get_type_of(Timing_Chooser),Person_Components::Prototypes::Activity_Timing_Chooser,ComponentType);
			define_component_interface(_scenario_itf, typename type_of(base_type::Parent_Planner)::type_of(Parent_Person)::type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
			define_component_interface(_network_itf, typename type_of(base_type::Parent_Planner)::type_of(Parent_Person)::type_of(network_reference), Network_Components::Prototypes::Network_Prototype, ComponentType);	
			define_component_interface(_skim_itf, typename _network_itf::get_type_of(skimming_faculty),Network_Skimming_Components::Prototypes::Network_Skimming_Prototype,ComponentType);
			define_container_and_value_interface(_activity_locations_container_itf, _activity_location_itf, typename _network_itf::get_type_of(activity_locations_container), Random_Access_Sequence_Prototype, Activity_Location_Components::Prototypes::Activity_Location_Prototype, ComponentType);
			define_container_and_value_interface(_links_container_itf, _link_itf, typename _activity_location_itf::get_type_of(origin_links), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
			define_container_and_value_interface(_zones_container_itf, _zone_itf, typename _network_itf::get_type_of(zones_container), Associative_Container_Prototype, Zone_Components::Prototypes::Zone_Prototype, ComponentType);
			//define_container_and_value_interface(_activity_plans_container_itf,_activity_plan_itf, typename type_of(base_type::Parent_Planner)::type_of(Activity_Container),Containers::Back_Insertion_Sequence_Prototype,Activity_Components::Prototypes::Activity_Planner,ComponentType);
			//define_container_and_value_interface(_movement_plans_container_itf,_movement_plan_itf, typename type_of(base_type::Parent_Planner)::type_of(Movement_Plans_Container),Containers::Back_Insertion_Sequence_Prototype,Movement_Plan_Components::Prototypes::Movement_Plan_Prototype,ComponentType);
			//================================================================================================================================================================================================
			//================================================================================================================================================================================================
		
			member_data_component(Basic_Units::Implementations::Time_Implementation<NT>,_Activity_Planning_Time,none,none);
			member_component_feature(Activity_Planning_Time, _Activity_Planning_Time, Value, Basic_Units::Prototypes::Time_Prototype)

			// Activity attribute planning properties		
			member_data(Types::PLAN_HORIZON_VALUES, Activity_Plan_Horizon,none,none);
			member_data(Types::PLAN_HORIZON_VALUES, Location_Plan_Horizon, none, none);
			//member_data(Types::FLEXIBILITY_VALUES, Location_Flexibility, none, none);
			member_data(Types::PLAN_HORIZON_VALUES, Mode_Plan_Horizon, none, none);
			//member_data(Types::FLEXIBILITY_VALUES, Mode_Flexibility, none, none);
			member_data(Types::PLAN_HORIZON_VALUES, Start_Time_Plan_Horizon,none,none);
			//member_data(Types::FLEXIBILITY_VALUES, Start_Time_Flexibility,none,none);
			member_data(Types::PLAN_HORIZON_VALUES, Duration_Plan_Horizon,none,none);
			//member_data(Types::FLEXIBILITY_VALUES, Duration_Flexibility,none,none);
			member_data(Types::PLAN_HORIZON_VALUES, Involved_Persons_Plan_Horizon,none,none);
			//member_data(Types::FLEXIBILITY_VALUES, Involved_Persons_Flexibility,none,none);

			// Activity methods
			feature_implementation void Initialize(TargetType act_type)
			{
				//UNLOCK(this->_update_lock);
				this_itf* pthis = (this_itf*)this;
				pthis->template Activity_Type<TargetType>(act_type);
				pthis->template Duration<Time_Seconds>(END);
				pthis->template Start_Time<Time_Seconds>(END);
				pthis->template Location<_activity_location_itf*>(nullptr);
				pthis->template Mode<Vehicle_Components::Types::Vehicle_Type_Keys>(Vehicle_Components::Types::Vehicle_Type_Keys::SOV);
			}

			feature_implementation void Set_Meta_Attributes()
			{
				//===========================================================================================================================
				// Initialize local variables for use
				//---------------------------------------------------------------------------------------------------------------------------

				// get references to use in model
				base_type* base_this = (base_type*)this;
				this_itf* pthis = (this_itf*)this;
				_planning_itf* planner = pthis->template Parent_Planner<_planning_itf*>();
				_person_itf* person = planner->template Parent_Person<_person_itf*>();
				_properties_itf* properties = person->template Properties<_properties_itf*>();
				_static_properties_itf* static_props = person->template Static_Properties<_static_properties_itf*>();

				//Create local variables
				int ACT1, ACT2, ACT3, ACT4, ACT5, Employed, Student, Male, Senior, TELEWORK, ICT_USE;
				ACT1 = 0; ACT2 = 0; ACT3 = 0; ACT4 = 0; ACT5 = 0; Employed = 0; Student = 0; Male = 0; Senior = 0; TELEWORK = 0; ICT_USE = 0;
				float AvgDur, AvgFreq;

				if (base_this->_Activity_Type == WORK_AT_HOME_ACTIVITY || base_this->_Activity_Type == PRIMARY_WORK_ACTIVITY || base_this->_Activity_Type == SCHOOL_ACTIVITY || base_this->_Activity_Type == OTHER_WORK_ACTIVITY) ACT1 = 1;
				else if (base_this->_Activity_Type == HEALTHCARE_ACTIVITY || base_this->_Activity_Type == RELIGIOUS_OR_CIVIC_ACTIVITY || base_this->_Activity_Type == PERSONAL_BUSINESS_ACTIVITY) ACT2 = 1;
				else if (base_this->_Activity_Type == Types::ERRANDS_ACTIVITY || base_this->_Activity_Type == PICK_UP_OR_DROP_OFF_ACTIVITY || base_this->_Activity_Type == SERVICE_VEHICLE_ACTIVITY) ACT3 = 1;
				else if (base_this->_Activity_Type == Types::LEISURE_ACTIVITY|| base_this->_Activity_Type == Types::EAT_OUT_ACTIVITY || base_this->_Activity_Type == Types::RECREATION_ACTIVITY || base_this->_Activity_Type == Types::SOCIAL_ACTIVITY) ACT4 = 1;
				else if (base_this->_Activity_Type == Types::OTHER_SHOPPING_ACTIVITY ||base_this-> _Activity_Type == Types::MAJOR_SHOPPING_ACTIVITY) ACT5 = 1;
				if (static_props->template Is_Employed<bool>()) Employed = 1;
				if (static_props->template Is_Student<bool>()) Student = 1;
				if (static_props->template Gender<GENDER>() == GENDER::MALE) Male = 1;
				if (static_props->template Age<int>() >= 65) Senior = 1;
				//if (PER.PersonData.ICT_Use != IctType.NULL || PER.PersonData.ICT_Use != IctType.UseLow) ICT_USE = 1;
				if (static_props->template Journey_To_Work_Mode<Person_Components::Types::JOURNEY_TO_WORK_MODE>() == JOURNEY_TO_WORK_MODE::WORKMODE_WORK_AT_HOME) TELEWORK = 1;
				AvgFreq = properties->template Average_Activity_Frequency<Target_Type<NT,float,ACTIVITY_TYPES>>(base_this->_Activity_Type);
				AvgDur = properties->template Average_Activity_Duration<Target_Type<NT,Time_Minutes,ACTIVITY_TYPES>>(base_this->_Activity_Type);


				//===========================================================================================================================
				// FLEXIBILITY CALCULATIONS
				//---------------------------------------------------------------------------------------------------------------------------
				matrix<double> alpha = matrix<double>(matrix<double>::index_type(3,5)); // limits matrix
				alpha(0,0)=-99999999;	alpha(0,1)=-99999999;	alpha(0,2)=-99999999;	alpha(0,3)=-99999999;	alpha(0,4)=-99999999;
				alpha(1,0)=0;			alpha(1,1)=0;			alpha(1,2)=0;			alpha(1,3)=0;			alpha(1,4)=0;
				alpha(2,0)=99999999;	alpha(2,1)=99999999;	alpha(2,2)=99999999;	alpha(2,3)=99999999;	alpha(2,4)=99999999;

				matrix<double> Sigma = matrix<double>(matrix<double>::index_type(5,5)); // covariance matrix
				Sigma(0,0)=1;			Sigma(0,1)=0;			Sigma(0,2)=0;			Sigma(0,3)=0;			Sigma(0,4)=0;
				Sigma(1,0)=0.099646;	Sigma(1,1)=1;			Sigma(1,2)=0;			Sigma(1,3)=0;			Sigma(1,4)=0;
				Sigma(2,0)=-0.053507;	Sigma(2,1)=-0.03372;	Sigma(2,2)=1;			Sigma(2,3)=0;			Sigma(2,4)=0;
				Sigma(3,0)=0.095299;	Sigma(3,1)=0.305332;	Sigma(3,2)=-0.113887;	Sigma(3,3)=1;			Sigma(3,4)=0;
				Sigma(4,0)=0.124233;	Sigma(4,1)=0.168151;	Sigma(4,2)=0.025553;	Sigma(4,3)=-0.099794;	Sigma(4,4)=1;

				double P_per[2]; // Holds probability for each level (index by i) of each response (index by j)
				double P_mod[2];
				double P_loc[2];
				double P_tim[2];
				double P_dur[2];

				//Calculate xB values for each response
				double xB_mod = -0.383692 + 0.362839 * Employed + 0.4215 * Student - 0.217904 * Male - 0.267344 * Senior + 0.691999 * Male * Senior + 0.231177 * AvgFreq + 0.568976 * ACT1 * ICT_USE - 1.039075 * ACT1 * AvgFreq;
				double xB_per = -0.338294 + 0.347752 * Student - 0.536749 * Male + 0.259776 * Senior + 0.478152 * Male * Senior + 0.532325 * ICT_USE + 1.508163 * TELEWORK - 1.655547 * ICT_USE * TELEWORK - 1.390818 * ACT1 * +1.352461 * ACT1 * TELEWORK - 0.598393 * ACT2 + 11.034344 * ACT2 * AvgDur - 0.617948 * ACT3 + 1.727868 * AvgFreq * ACT3 + 0.437761 * AvgFreq * ACT4 + 6.708181 * ACT4 * AvgDur - 0.606538 * ACT5 + 0.325567 * AvgFreq * ACT5;
				double xB_loc = -0.368786 - 1.174171 * Male - 0.1914 * Senior + 1.346607 * Male * Senior - 0.3674 * ICT_USE + 0.99392 * TELEWORK - 0.815509 * ICT_USE * TELEWORK + 1.023933 * ACT1 * TELEWORK + 2.325112 * ACT1 * AvgFreq - 8.966545 * ACT2 * AvgDur - 1.02207 * AvgFreq * ACT3 + 0.904036 * ACT4 + 0.637477 * AvgFreq * ACT4 - 8.32642 * ACT4 * AvgDur + 2.382859 * ACT5;
				double xB_tim = 1.326259 - 0.38711 * Male - 0.226637 * Senior + 0.484623 * Male * Senior - 0.302255 * ICT_USE + 0.196488 * TELEWORK + 3.3758 * ACT2 * AvgDur - 0.400564 * ACT3 + 1.301273 * AvgFreq * ACT3 - 0.601686 * ACT5 + 0.192867 * AvgFreq * ACT5;
				double xB_dur = -1.239259 + 1.294471 * Employed + 1.461113 * Student - 0.246144 * Senior - 0.374409 * Male * Senior - 0.23527 * ICT_USE * TELEWORK + 1.30007 * AvgDur + 0.621008 * ACT1 * +0.994951 * ACT1 * ICT_USE - 1.124459 * ACT1 * TELEWORK - 0.56222 * ACT1 * AvgFreq - 2.070631 * ACT1 * AvgDur - 0.167278 * ICT_USE * ACT4 + 0.769745 * AvgFreq * ACT4 + 0.487925 * AvgFreq * ACT5;			

				// Get probabilities of each response level
				for (int i = 0; i < 2; i++)
				{
					P_mod[i] = Normal_Distribution->template Cumulative_Distribution_Value<float>(alpha(i + 1, 0) - xB_mod);
					P_per[i] = Normal_Distribution->template Cumulative_Distribution_Value<float>(alpha(i + 1, 1) - xB_per);
					P_loc[i] = Normal_Distribution->template Cumulative_Distribution_Value<float>(alpha(i + 1, 2) - xB_loc);
					P_tim[i] = Normal_Distribution->template Cumulative_Distribution_Value<float>(alpha(i + 1, 3) - xB_tim);
					P_dur[i] = Normal_Distribution->template Cumulative_Distribution_Value<float>(alpha(i + 1, 4) - xB_dur);
				}

				// Set correlated random variables
				vector<double> rand;
				GLOBALS::Normal_RNG.template Correlated_Rands<double>(rand, Sigma);

				// Set flexibility values
				for (int i = 0; i < 2; i++)
				{
					if (rand[0] < P_mod[i]) {base_this->_Mode_Flexibility = (Types::FLEXIBILITY_VALUES)i;				rand[0] = 999;}
					if (rand[1] < P_per[i]) {base_this->_Involved_Persons_Flexibility = (Types::FLEXIBILITY_VALUES)i;	rand[1] = 999; }
					if (rand[2] < P_loc[i]) {base_this->_Location_Flexibility = (Types::FLEXIBILITY_VALUES)i;			rand[2] = 999; }
					if (rand[3] < P_tim[i]) {base_this->_Start_Time_Flexibility = (Types::FLEXIBILITY_VALUES)i;			rand[3] = 999; }
					if (rand[4] < P_dur[i]) {base_this->_Duration_Flexibility = (Types::FLEXIBILITY_VALUES)i;			rand[4] = 999; }
				}
            
				// Set results of flexiblity model to use in next models
				double IFLOC, IFTIM, IFDUR, IFMOD;
				IFMOD = P_mod[0];
				IFLOC = P_loc[0];
				IFTIM = P_tim[0];
				IFDUR = P_dur[0];




				//===========================================================================================================================
				// Activity Plan Horizon Calculations
				//---------------------------------------------------------------------------------------------------------------------------
				double xB = 0.130 + 0.640303 * IFLOC - 0.63621 * IFTIM - 1.498554 * IFDUR + 0.767701 * Employed + 0.485362 * ICT_USE
					- 0.560652 * TELEWORK + 1.049907 * ACT1 - 0.446779 * ACT1 * AvgFreq - 0.789474 * ACT1 * IFLOC + 2.241387 * ACT1 * IFDUR
					- 0.892963 * ACT1 * Employed + 1.717603 * ACT1 * Student + 15.248565 * ACT2 * AvgDur + 1.497258 * IFDUR * ACT2
					- 0.613958 * ICT_USE * ACT2 + 1.573672 * AvgFreq * ACT3 + 0.633234 * IFDUR * ACT3 - 0.745918 * Employed * ACT3
					- 1.115487 * Student * ACT3 + 1.049812 * ACT3 * Senior - 0.593425 * ACT3 * Male + 0.519961 * AvgFreq * ACT4 + 2.200494 * AvgDur * ACT4
					+ 1.403807 * IFDUR * ACT4 + 0.837724 * Student * ACT4 + 0.681377 * Senior * ACT4 - 0.793118 * Male * ACT4 - 0.432584 * ICT_USE * ACT4
					+ 0.289143 * AvgFreq * ACT5 + 1.128966 * IFDUR * ACT5 - 0.610606 * Employed * ACT5 + 0.415577 * Senior * ACT5
					- 0.752066 * ICT_USE * ACT5;
				double Pimp, Pday, Pweek;
				double a1 = 1.646398;
				double a2 = 3.505000;
				double r = Uniform_RNG.template Next_Rand<double>();

				// calculate probabilities
				Pimp = 1.0 / (1.0 + exp(xB));
				Pday = 1.0 / (1.0 + exp(-a1 + xB));
				Pweek = 1.0 / (1.0 + exp(-a2 + xB));

				// Set activity plan horizon
				if (r < Pimp) this->_Activity_Plan_Horizon = Types::PLAN_HORIZON_VALUES::IMPULSIVE;
				else if (r < Pday) this->_Activity_Plan_Horizon = Types::PLAN_HORIZON_VALUES::PREPLANNED_SAME_DAY;
				else if (r < Pweek) this->_Activity_Plan_Horizon = Types::PLAN_HORIZON_VALUES::PREPLANNED_2_TO_7_DAYS;
				else this->_Activity_Plan_Horizon = Types::PLAN_HORIZON_VALUES::PREPLANNED_OVER_7_DAYS;

				


				//===========================================================================================================================
				// ATTRIBUTE Plan Horizon Calculations
				//---------------------------------------------------------------------------------------------------------------------------
				double PLANIMP = Pimp, PLANSAME = Pday-Pimp, PLANWEEK = Pweek-Pday, PLANONTOUR;
				PLANONTOUR = PLANIMP * 1;

				matrix<double> alpha2 = matrix<double>(matrix<double>::index_type(6,5));
				alpha2(0,0)=-99999999;	alpha2(0,1)=-99999999;	alpha2(0,2)=-99999999;	alpha2(0,3)=-99999999;	alpha2(0,4)=-99999999;
				alpha2(1,0)=0;			alpha2(1,1)=0;			alpha2(1,2)=0;			alpha2(1,3)=0;			alpha2(1,4)=0;
				alpha2(2,0)=0.835415;	alpha2(2,1)=2.029384;	alpha2(2,2)=1.305499;	alpha2(2,3)=1.151026;	alpha2(2,4)=0.434582;
				alpha2(3,0)=1.486357;	alpha2(3,1)=3.614181;	alpha2(3,2)=2.316088;	alpha2(3,3)=2.026512;	alpha2(3,4)=0.879016;
				alpha2(4,0)=1.794494;	alpha2(4,1)=4.401609;	alpha2(4,2)=2.796206;	alpha2(4,3)=2.643064;	alpha2(4,4)=1.095505;
				alpha2(5,0)=99999999;	alpha2(5,1)=99999999;	alpha2(5,2)=99999999;	alpha2(5,3)=99999999;	alpha2(5,4)=99999999;

				matrix<double> Sigma2 = matrix<double>(matrix<double>::index_type(5,5));
				Sigma2(0,0)=1;			Sigma2(0,1)=0;			Sigma2(0,2)=0;			Sigma2(0,3)=0;			Sigma2(0,4)=0;
				Sigma2(1,0)=0.14308;	Sigma2(1,1)=1;			Sigma2(1,2)=0;			Sigma2(1,3)=0;			Sigma2(1,4)=0;
				Sigma2(2,0)=0.148218;	Sigma2(2,1)=0.104424;	Sigma2(2,2)=1;			Sigma2(2,3)=0;			Sigma2(2,4)=0;
				Sigma2(3,0)=0.159259;	Sigma2(3,1)=0.180953;	Sigma2(3,2)=0.352088;	Sigma2(3,3)=1;			Sigma2(3,4)=0;
				Sigma2(4,0)=0.186676;	Sigma2(4,1)=0.177994;	Sigma2(4,2)=0.218223;	Sigma2(4,3)=0.538872;	Sigma2(4,4)=1;

				double P_pmod[5];
				double P_pper[5]; // Holds probability for each level (index by i) of each response (index by j)
				double P_ploc[5];
				double P_ptim[5];
				double P_pdur[5];

				//Calculate xB values for each response
				double xB_pmod = 0.618 + 0.262288 * ACT2 + 0.376779 * ACT4 + 0.195715 * ACT5 + 0.575386 * PLANONTOUR - 1.527302 * PLANIMP 
								 - 0.492435 * PLANSAME - 0.271618 * PLANWEEK + 0.633625 * Employed + 1.043977 * Student - 0.115081 * Senior
								 + 0.106084 * IFDUR + 0.785746 * ACT1 * AvgDur + 0.353502 * PLANWEEK * ACT3 - 3.92232 * ACT4 * AvgDur
								 + 0.456054 * ACT1 * TELEWORK + 0.130093 * ICT_USE;
				double xB_pper = 4.096 - 5.532536 * PLANIMP - 2.912268 * PLANSAME - 1.231383 * PLANWEEK + 0.825496 * ACT2 * Student
								 - 5.690886 * ACT4 * AvgDur + 0.358763 * ACT4 * ICT_USE;
				double xB_ploc = 2.079 - 3.2086 * PLANIMP - 1.40061 * PLANSAME - 0.581665 * PLANWEEK + 0.202875 * Employed + 0.350806 * Student
								 + 1.846201 * AvgDur + 0.951409 * ACT1 * IFLOC + 0.278635 * ACT4 * IFLOC + 0.839625 * ICT_USE
								 - 0.736206 * ACT2 * ICT_USE - 0.692824 * ACT3 * ICT_USE - 1.033742 * ACT4 * ICT_USE - 1.166372 * ACT5 * ICT_USE;
				double xB_ptim = 1.612 + 0.206278 * ACT5 - 3.178382 * PLANIMP - 1.570305 * PLANSAME - 0.932677 * PLANWEEK + 0.359391 * Employed
								 + 0.163188 * IFDUR + 0.616433 * ACT3 * IFTIM + 0.756115 * ACT1 * TELEWORK - 0.145507 * ICT_USE
								 + 0.185875 * ACT4 * ICT_USE - 0.497667 * ACT5 * ICT_USE;
				double xB_pdur = -1.264 + 0.323092 * ACT2 - 1.466871 * PLANIMP - 0.518569 * PLANSAME - 0.35598 * PLANWEEK + 0.742175 * Employed
								 + 0.157659 * Senior + 0.130234 * Male + 0.297442 * IFTIM + 0.835193 * IFDUR + 0.266187 * AvgFreq
								 - 0.395776 * ACT2 * IFDUR - 0.282278 * ACT5 * IFDUR - 0.338128 * ACT5 * IFTIM + 0.879233 * ACT1 * TELEWORK;

				// Get probabilities of each response level
				for (int i = 0; i < 5; i++)
				{
					P_pmod[i] = Normal_Distribution->template Cumulative_Distribution_Value<float>(alpha2[i + 1, 0] - xB_pmod, 0, 1);
					P_pper[i] = Normal_Distribution->template Cumulative_Distribution_Value<float>(alpha2[i + 1, 1] - xB_pper, 0, 1);
					P_ploc[i] = Normal_Distribution->template Cumulative_Distribution_Value<float>(alpha2[i + 1, 2] - xB_ploc, 0, 1);
					P_ptim[i] = Normal_Distribution->template Cumulative_Distribution_Value<float>(alpha2[i + 1, 3] - xB_ptim, 0, 1);
					P_pdur[i] = Normal_Distribution->template Cumulative_Distribution_Value<float>(alpha2[i + 1, 4] - xB_pdur, 0, 1);
				}

				// Set correlated random variables
				vector<double> rand2;
				GLOBALS::Normal_RNG.template Correlated_Rands<double>(rand2, Sigma2);

				// Set attribute plan horizon values
				for (int i = 0; i < 5; i++)
				{
					// convert from i to planhorizon values (skip 'previous day')
					int x = i + 1; if (i > 1) x = x + 1;
					PLAN_HORIZON_VALUES value;
					switch (i)
					{
					case 0 : value = PLAN_HORIZON_VALUES::IMPULSIVE; break;
					case 1 : value = PLAN_HORIZON_VALUES::PREPLANNED_SAME_DAY; break;
					case 2 : value = PLAN_HORIZON_VALUES::PREPLANNED_2_TO_7_DAYS; break;
					case 3 : value = PLAN_HORIZON_VALUES::PREPLANNED_OVER_7_DAYS; break;
					default : value = ROUTINE;
					}

					if (rand2[0] < P_pmod[i]) { this->_Mode_Plan_Horizon = value;				rand2[0] = 999; }
					if (rand2[1] < P_pper[i]) { this->_Involved_Persons_Plan_Horizon = value;	rand2[1] = 999; }
					if (rand2[2] < P_ploc[i]) { this->_Location_Plan_Horizon = value;			rand2[2] = 999; }
					if (rand2[3] < P_ptim[i]) { this->_Start_Time_Plan_Horizon = value;			rand2[3] = 999; }
					if (rand2[4] < P_pdur[i]) { this->_Duration_Plan_Horizon = value;			rand2[4] = 999; }
				}
			}
			tag_feature_as_available(Set_Meta_Attributes);

			feature_implementation void Set_Attribute_Planning_Times(TargetType planning_time, requires(check_2(TargetType, Simulation_Timestep_Increment, is_same)))
			{
				// get references to use in model
				base_type* base_this = (base_type*)this;

				Revision &start	= base_this->template Start_Time_Planning_Time<ComponentType, CallerType, Revision&>();
				Revision &dur		= base_this->template Duration_Planning_Time<ComponentType, CallerType, Revision&>();
				Revision &loc		= base_this->template Location_Planning_Time<ComponentType, CallerType, Revision&>();
				Revision &mode	= base_this->template Mode_Planning_Time<ComponentType, CallerType, Revision&>();
				Revision &persons	= base_this->template Involved_Persons_Planning_Time<ComponentType, CallerType, Revision&>();
				Revision &route	= base_this->template Route_Planning_Time<ComponentType, CallerType, Revision&>();


				//========================================================================
				//TODO: remove stored versions of the planning times after testing
				Revision &Stored_start	= base_this->template Stored_Start_Time_Planning_Time<ComponentType, CallerType, Revision&>();
				Revision &Stored_dur		= base_this->template Stored_Duration_Planning_Time<ComponentType, CallerType, Revision&>();
				Revision &Stored_loc		= base_this->template Stored_Location_Planning_Time<ComponentType, CallerType, Revision&>();
				Revision &Stored_mode	= base_this->template Stored_Mode_Planning_Time<ComponentType, CallerType, Revision&>();
				Revision &Stored_persons	= base_this->template Stored_Involved_Persons_Planning_Time<ComponentType, CallerType, Revision&>();
				Revision &Stored_route	= base_this->template Stored_Route_Planning_Time<ComponentType, CallerType, Revision&>();
				//========================================================================


				// Set revisions based on the plan-horizons for each attribute
				int dur_sub = (int)(this->_Duration_Plan_Horizon);
				int loc_sub = (int)(this->_Location_Plan_Horizon);
				int mode_sub = (int)(this->_Mode_Plan_Horizon);
				int inv_sub = (int)(this->_Involved_Persons_Plan_Horizon);
				int start_sub = (int)(this->_Start_Time_Plan_Horizon);
			
				// give intrinsic order when simultaneous plan horizons are specified
				if (start_sub == dur_sub) dur_sub++;
				if (start_sub == loc_sub) loc_sub++;
				if (start_sub == mode_sub) mode_sub++;
				if (start_sub == inv_sub) inv_sub++;
				if (dur_sub == loc_sub) loc_sub++;
				if (dur_sub == mode_sub) mode_sub++;
				if (dur_sub == inv_sub) inv_sub++;
				if (loc_sub == mode_sub) mode_sub++;
				if (loc_sub == inv_sub) inv_sub++;
				if (mode_sub == inv_sub) inv_sub++;

				int route_sub = max(start_sub,max(dur_sub,max(loc_sub,max(mode_sub,inv_sub)))) + 1;

				start._iteration = max(_iteration+1, (int)planning_time);
				dur._iteration = END; //dur._iteration = max(_iteration+1, (int)planning_time);
				loc._iteration = max(_iteration+1,(int)planning_time);
				mode._iteration = max(_iteration+1, (int)planning_time);
				persons._iteration = max(_iteration+1, (int)planning_time);
				route._iteration = max(_iteration+1, (int)planning_time);
				
				start._sub_iteration = start_sub;
				dur._sub_iteration = END; //dur._sub_iteration = dur_sub;
				loc._sub_iteration = loc_sub;
				mode._sub_iteration = mode_sub;
				persons._sub_iteration = inv_sub;
				route._sub_iteration = route_sub;


				//========================================================================
				//TODO: remove stored versions of the planning times after testing
				Stored_start._iteration = start._iteration;
				Stored_dur._iteration = dur._iteration;
				Stored_loc._iteration = loc._iteration;
				Stored_mode._iteration = mode._iteration;
				Stored_persons._iteration = persons._iteration;
				Stored_route._iteration = route._iteration;
				
				Stored_start._sub_iteration = start._sub_iteration;
				Stored_dur._sub_iteration = dur._sub_iteration;
				Stored_loc._sub_iteration = loc._sub_iteration;
				Stored_mode._sub_iteration = mode._sub_iteration;
				Stored_persons._sub_iteration = persons._sub_iteration;
				Stored_route._sub_iteration = route._sub_iteration;
				//========================================================================

			
				//cout << endl << ((base_type*)this)->_Parent_Planner->Parent_Person<_person_itf*>()->uuid<int>() <<",  ";
				//cout << "Start: "	<<start._iteration <<"."<<start._sub_iteration;
				//cout << ",  Dur: "	<<dur._iteration <<"."<<dur._sub_iteration;
				//cout << ",  loc: "	<<loc._iteration <<"."<<loc._sub_iteration;
				//cout << ",  mode: "	<<mode._iteration <<"."<<mode._sub_iteration;
				//cout << ",  persons: "	<<persons._iteration <<"."<<persons._sub_iteration;
				//cout << ",  route: "	<<route._iteration <<"."<<route._sub_iteration;
			}
			feature_implementation void Set_Attribute_Planning_Times(TargetType planning_time, requires(!check_2(TargetType, Simulation_Timestep_Increment, is_same)))
			{
				assert_check_2(TargetType, Simulation_Timestep_Increment, is_same, "Error: planning_time must be passed as a Simulation_Timestep_Increment type when using this function.");
			}
			tag_feature_as_available(Set_Attribute_Planning_Times);

			// Attribute planning events
			feature_implementation void Location_Planning_Event_Handler()
			{
				base_type* bthis = (base_type*)this;
				this_itf* pthis = (this_itf*)this;

				// references to external agents
				_planning_itf* planner = bthis->template Parent_Planner<ComponentType,CallerType,_planning_itf*>();
				_person_itf* person = planner->template Parent_Person<_person_itf*>();
				_scheduler_itf* scheduler = person->template Scheduling_Faculty<_scheduler_itf*>();
				_dest_choice_itf* dest_chooser = planner->template Destination_Choice_Faculty<_dest_choice_itf*>();
				_network_itf* network = person->template network_reference<_network_itf*>();


				// timing variables
				Time_Minutes start, end, prev_end, next_start, time_before, time_after, ttime_prev_to_home, ttime_home_to_next;
				start = pthis->template Start_Time<Time_Minutes>();
				end = pthis->template Start_Time<Time_Minutes>() + pthis->template Duration<Time_Minutes>();

				// Get half-tour origin from previous planned activity and destination from next planned activity (or from home if no preceeding activities)
				_activity_location_itf *prev_loc, *next_loc, *home_loc;
				home_loc = person->template Home_Location<_activity_location_itf*>();

				if (bthis->template Start_Is_Planned<ComponentType,CallerType,bool>())
				{
					// set previous activity location, =home if no previous activity or if person can go home before current activity
					this_itf* previous = person->template previous_activity_plan<Target_Type<NT,this_itf*,Time_Seconds>>(bthis->template Start_Time<ComponentType,CallerType,Time_Seconds>());
					if (previous == nullptr) prev_loc = home_loc;
					else prev_loc = scheduler->template previous_location<Target_Type<NT,_activity_location_itf*, ComponentType*>>(this);

					// set next activity location, =home if no next activity
					this_itf* next = person->template previous_activity_plan<Target_Type<NT,this_itf*,Time_Seconds>>(bthis->template Start_Time<ComponentType,CallerType,Time_Seconds>());
					if (next == nullptr) next_loc = home_loc;
					else next_loc = scheduler->template next_location<Target_Type<NT,_activity_location_itf*, ComponentType*>>(this);
				}
				// if the start time is not known, treat the tour as originating/destined for home
				else 
				{
					prev_loc = home_loc;
					next_loc = home_loc;
				}

				// call destination choice - set to home of no location could be chosen
				_activity_location_itf* dest = dest_chooser->template Choose_Destination<_activity_location_itf*>(prev_loc, next_loc);
				if (dest == nullptr) dest = home_loc;

				// check that origin and destination are valid
				if (prev_loc != nullptr && dest != nullptr) 
				{
					bthis->template Location<ComponentType,CallerType,_activity_location_itf*>(dest);
				}
				else
				{
					//----------------------------------------------------------------
					// Print to log file
					stringstream s;
					s <<"ACTIVITY NOT GENERATED, null origin or destination: "<< person->template uuid<int>();
					s << "," <<prev_loc << ", " <<dest<<endl;
					//planner->template Write_To_Log<stringstream&>(s);
					//----------------------------------------------------------------
				}
			}
			feature_implementation void Mode_Planning_Event_Handler()
			{
				//====================================================================
				// Simplified mode choice model, based only on the expected travel time by auto
				// And 3 activity types.  Distributions of auto share by travel time were
				// estimated from CMAP travel tracker survey and are applied here.
				// If the time to travel for the activity is not known, 12pm is assumed (for skim time lookup)
				// If the locaion has not been planned we cannot estimate an approximate time and the average mode share is used
				//--------------------------------------------------------------------
				base_type* bthis = (base_type*)this;
				this_itf* pthis = (this_itf*)this;

				// references to external agents
				_planning_itf* planner = bthis->template Parent_Planner<ComponentType,CallerType,_planning_itf*>();
				_person_itf* person = planner->template Parent_Person<_person_itf*>();
				_scheduler_itf* scheduler = person->template Scheduling_Faculty<_scheduler_itf*>();
				_dest_choice_itf* dest_chooser = planner->template Destination_Choice_Faculty<_dest_choice_itf*>();
				_network_itf* network = person->template network_reference<_network_itf*>();

				// expected travel time, based on whether start time is planned or not and location is planned or not
				float p_auto = 0.0f;

				// if start time planned use the planned start time, otherwise use 12pm.
				Time_Seconds start;
				if (pthis->template Start_Is_Planned<bool>()) start = pthis->template Start_Time<Time_Seconds>();
				else start = 12.0*60.0*60.0;

				// if location planned, get the travel time to location at start time
				Time_Minutes ttime;
				if (pthis->template Location_Is_Planned<bool>()) 
				{
					// get the previous activity
					_activity_location_itf* orig = scheduler->template previous_location<Target_Type<NT,_activity_location_itf*,ComponentType*>>(this);
					_activity_location_itf* dest = pthis->template Location<_activity_location_itf*>();
					int o_id = orig->template zone<_zone_itf*>()->template uuid<int>();
					int d_id = dest->template zone<_zone_itf*>()->template uuid<int>();

					// get expected auto travel time
					ttime = network->template Get_LOS<Target_Type<NT,Time_Minutes,int,Vehicle_Components::Types::Vehicle_Type_Keys,Time_Seconds>>(o_id,d_id,Vehicle_Components::Types::Vehicle_Type_Keys::SOV,start);
					if (ttime <10.0f) p_auto=0.831;
					else if (ttime <20.0f) p_auto=0.855;
					else if (ttime <30.0f) p_auto=0.844;
					else if (ttime <40.0f) p_auto=0.8;
					else if (ttime <50.0f) p_auto=0.762;
					else if (ttime <60.0f) p_auto=0.726;
					else if (ttime <70.0f) p_auto=0.687;
					else if (ttime <80.0f) p_auto=0.619;
					else if (ttime <90.0f) p_auto=0.531;
					else if (ttime <100.0f) p_auto=0.501;
					else p_auto=0.547;
				}
				// else if location is not planned, use the average auto mode choice percentage
				else p_auto = 0.818;

			
				if (GLOBALS::Uniform_RNG.Next_Rand<float>() < p_auto) pthis->template Mode<Vehicle_Components::Types::Vehicle_Type_Keys>(Vehicle_Components::Types::Vehicle_Type_Keys::SOV);
				else pthis->template Mode<Vehicle_Components::Types::Vehicle_Type_Keys>(Vehicle_Components::Types::Vehicle_Type_Keys::BUS);
			}
			feature_implementation void Duration_Planning_Event_Handler()
			{
				
			}
			feature_implementation void Start_Time_Planning_Event_Handler()
			{
				this_itf* pthis = (this_itf*)this;
				base_type* bthis = (base_type*)this;

				// interfaces
				_planning_itf* planner = pthis->template Parent_Planner<_planning_itf*>();
				_person_itf* person = planner->template Parent_Person<_person_itf*>();
				_scheduler_itf* scheduler = person->template Scheduling_Faculty<_scheduler_itf*>();
				_scenario_itf* scenario = (_scenario_itf*)_global_scenario;

				// get the combined start time and duration
				_timing_choice_itf* timing_planner = planner->template Timing_Chooser<_timing_choice_itf*>();
				pair<Time_Seconds,Time_Seconds> start_and_duration = timing_planner->template Get_Start_Time_and_Duration<Target_Type<NT,Time_Seconds,ComponentType*>>(this);

				// make sure start time is not prior to current iteration
				Time_Seconds time_min = Simulation_Time.template Future_Time<Time_Seconds,Time_Seconds>(planner->template Planning_Time_Increment<Time_Seconds>());
				pthis->template Start_Time<Time_Seconds>(std::max<int>(start_and_duration.first,time_min.Value));

				// set the duration, making sure it fits into current schedule slot
				pthis->template Duration<Time_Seconds>(start_and_duration.second);

				//========================================================================
				// Resolve timing conflicts when timing is known
				bool is_scheduled =	scheduler->template Resolve_Timing_Conflict<this_itf*>(pthis,false);
				// if conflict not resolved remove activity from schedule and modify routing response time so no further planning is done
				if (!is_scheduled) 
				{
					pthis->template Unschedule_Activity_Events<NT>();
					scheduler->template Remove_Activity_Plan<this_itf*>(pthis);
				}


				//========================================================================
				// END HERE if no planner routing is requested through scenario
				if (!scenario->template do_planner_routing<bool>()) return;
				//------------------------------------------------------------------------




				//========================================================================
				// start routing on the planning timestep at 2 times the estimated travel time from skims prior to departure - rounded to nearest minute
				Time_Minutes exp_ttime=0.0f;
				if (bthis->template Location_Is_Planned<ComponentType,CallerType,NT>())
				{
					_network_itf* network = person->template network_reference<_network_itf*>();
					_zone_itf* o_zone = person->template Home_Location<_zone_itf*>();
					_zone_itf* d_zone = pthis->template Location<_activity_location_itf*>()->template zone<_zone_itf*>();
					exp_ttime = network->template Get_LOS<Target_Type<NT,Time_Minutes,int,Vehicle_Components::Types::Vehicle_Type_Keys, Time_Minutes> >(o_zone->template uuid<int>(),d_zone->template uuid<int>(),Vehicle_Components::Types::Vehicle_Type_Keys::SOV, pthis->template Start_Time<Time_Minutes>());
				}			
				exp_ttime = std::max<float>(exp_ttime,30.0f);


				//==============================================================================================
				// Reset the route planning time based on the expected departure time, and aggregate as needed
				//---------------------------
				// Aggregate plan routing
				if (scenario->template aggregate_routing<bool>())
				{
					Time_Minutes start_minutes = (int)(pthis->template Start_Time<Time_Minutes>() - (exp_ttime * 2.0));
					int start_increment = std::max<int>(Simulation_Time.template Convert_Time_To_Simulation_Timestep<Time_Minutes>(start_minutes), _iteration);
					pthis->template Route_Planning_Time<Revision&>()._iteration = start_increment;
				}
				//---------------------------
				// Disaggregate plan routing
				else
				{
					Simulation_Timestep_Increment start_seconds = pthis->template Start_Time<Simulation_Timestep_Increment>() - Simulation_Time.template Convert_Time_To_Simulation_Timestep<Time_Minutes>(exp_ttime * 2.0);
					int start_increment = std::max<int>(start_seconds, _iteration);
					pthis->template Route_Planning_Time<Revision&>()._iteration = start_increment;
				}	
			}
			feature_implementation void Involved_Persons_Planning_Event_Handler()
			{
				
			}
			
		};
		


		// Specialization for ADAPTS-style routine activities
		implementation struct ADAPTS_Routine_Activity_Plan_Implementation : public Basic_Activity_Plan_Implementation<MasterType,ParentType,APPEND_CHILD(ADAPTS_Routine_Activity_Plan_Implementation)>
		{
			typedef Basic_Activity_Plan_Implementation<MasterType,ParentType,APPEND_CHILD(ADAPTS_Routine_Activity_Plan_Implementation)> base_type;
			typedef typename Basic_Activity_Plan_Implementation<MasterType,ParentType,APPEND_CHILD(ADAPTS_Routine_Activity_Plan_Implementation)>::Component_Type ComponentType;
			typedef Prototypes::Activity_Planner<ComponentType,ComponentType> this_itf;
		
			//================================================================================================================================================================================================
			//================================================================================================================================================================================================
			// Interfaces
			define_component_interface(_properties_itf, typename type_of(base_type::Parent_Planner)::type_of(Parent_Person)::type_of(Properties), Person_Components::Prototypes::Person_Properties, ComponentType);
			define_component_interface(_static_properties_itf, typename type_of(base_type::Parent_Planner)::type_of(Parent_Person)::type_of(Static_Properties), Person_Components::Prototypes::Person_Properties, ComponentType);
			define_component_interface(_planning_itf,typename type_of(base_type::Parent_Planner),Person_Components::Prototypes::Person_Planner,ComponentType);
			define_component_interface(_scheduler_itf, typename type_of(base_type::Parent_Planner)::type_of(Parent_Person)::type_of(Scheduling_Faculty), Person_Components::Prototypes::Person_Scheduler, ComponentType);
			define_component_interface(_person_itf,typename _planning_itf::get_type_of(Parent_Person),Person_Components::Prototypes::Person,ComponentType);
			define_component_interface(_dest_choice_itf,typename _planning_itf::get_type_of(Destination_Choice_Faculty),Person_Components::Prototypes::Destination_Chooser,ComponentType);
			define_component_interface(_scenario_itf, typename type_of(base_type::Parent_Planner)::type_of(Parent_Person)::type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
			define_component_interface(_network_itf, typename type_of(base_type::Parent_Planner)::type_of(Parent_Person)::type_of(network_reference), Network_Components::Prototypes::Network_Prototype, ComponentType);	
			define_component_interface(_skim_itf, typename _network_itf::get_type_of(skimming_faculty),Network_Skimming_Components::Prototypes::Network_Skimming_Prototype,ComponentType);
			define_component_interface(_timing_choice_itf,typename _planning_itf::get_type_of(Timing_Chooser),Person_Components::Prototypes::Activity_Timing_Chooser,ComponentType);
			define_container_and_value_interface(_activity_locations_container_itf, _activity_location_itf, typename _network_itf::get_type_of(activity_locations_container), Random_Access_Sequence_Prototype, Activity_Location_Components::Prototypes::Activity_Location_Prototype, ComponentType);
			define_container_and_value_interface(_links_container_itf, _link_itf, typename _activity_location_itf::get_type_of(origin_links), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
			define_container_and_value_interface(_zones_container_itf, _zone_itf, typename _network_itf::get_type_of(zones_container), Associative_Container_Prototype, Zone_Components::Prototypes::Zone_Prototype, ComponentType);
		/*	define_container_and_value_interface(_activity_plans_container_itf,_activity_plan_itf, typename type_of(base_type::Parent_Planner)::type_of(Activity_Container),Containers::Back_Insertion_Sequence_Prototype,Activity_Components::Prototypes::Activity_Planner,ComponentType);
			define_container_and_value_interface(_movement_plans_container_itf,_movement_plan_itf, typename type_of(base_type::Parent_Planner)::type_of(Movement_Plans_Container),Containers::Back_Insertion_Sequence_Prototype,Movement_Plan_Components::Prototypes::Movement_Plan_Prototype,ComponentType);*/
			//================================================================================================================================================================================================
			//================================================================================================================================================================================================

			// Fundamental activity properties
			feature_implementation Types::PLAN_HORIZON_VALUES Activity_Plan_Horizon()
			{
				return Types::PLAN_HORIZON_VALUES::ROUTINE;
			}
			tag_getter_as_available(Activity_Plan_Horizon);

			// Activity attribute planning properties
			feature_implementation Types::PLAN_HORIZON_VALUES Location_Plan_Horizon()
			{
				return Types::PLAN_HORIZON_VALUES::ROUTINE;
			}
			tag_getter_as_available(Location_Plan_Horizon);
			feature_implementation Types::PLAN_HORIZON_VALUES Mode_Plan_Horizon()
			{
				return Types::PLAN_HORIZON_VALUES::ROUTINE;
			}
			tag_getter_as_available(Mode_Plan_Horizon);
			feature_implementation Types::PLAN_HORIZON_VALUES Start_Time_Plan_Horizon()
			{
				return Types::PLAN_HORIZON_VALUES::ROUTINE;
			}
			tag_getter_as_available(Start_Time_Plan_Horizon);
			feature_implementation Types::PLAN_HORIZON_VALUES Duration_Plan_Horizon()
			{
				return Types::PLAN_HORIZON_VALUES::ROUTINE;
			}
			tag_getter_as_available(Duration_Plan_Horizon);
			feature_implementation Types::PLAN_HORIZON_VALUES Involved_Persons_Plan_Horizon()
			{
				return Types::PLAN_HORIZON_VALUES::ROUTINE;
			}
			tag_getter_as_available(Involved_Persons_Plan_Horizon);


			// Activity methods
			feature_implementation void Initialize(TargetType act_type)
			{
				//UNLOCK(this->_update_lock);
				this_itf* pthis = (this_itf*)this;
				pthis->template Activity_Type<TargetType>(act_type);
				pthis->template Duration<Time_Seconds>(END);
				pthis->template Start_Time<Time_Seconds>(END);
				pthis->template Location<_activity_location_itf*>(nullptr);
				pthis->template Mode<Vehicle_Components::Types::Vehicle_Type_Keys>(Vehicle_Components::Types::Vehicle_Type_Keys::SOV);
			}

			feature_implementation void Set_Attribute_Planning_Times(TargetType planning_time, requires(check_2(TargetType, Simulation_Timestep_Increment, is_same)))
			{
				base_type* base_this = (base_type*)this;

				base_this->template Start_Time_Planning_Time<ComponentType, CallerType, Revision&>()._iteration = min(_iteration+1, (int)planning_time);
				base_this->template Start_Time_Planning_Time<ComponentType, CallerType, Revision&>()._sub_iteration = 0;
				base_this->template Duration_Planning_Time<ComponentType, CallerType, Revision&>()._iteration = min(_iteration+1, (int)planning_time);
				base_this->template Duration_Planning_Time<ComponentType, CallerType, Revision&>()._sub_iteration = 1;
				base_this->template Location_Planning_Time<ComponentType, CallerType, Revision&>()._iteration = min(_iteration+1, (int)planning_time);
				base_this->template Location_Planning_Time<ComponentType, CallerType, Revision&>()._sub_iteration = 2;
				base_this->template Mode_Planning_Time<ComponentType, CallerType, Revision&>()._iteration = min(_iteration+1, (int)planning_time);
				base_this->template Mode_Planning_Time<ComponentType, CallerType, Revision&>()._sub_iteration = 3;
				base_this->template Involved_Persons_Planning_Time<ComponentType, CallerType, Revision&>()._iteration = min(_iteration+1, (int)planning_time);
				base_this->template Involved_Persons_Planning_Time<ComponentType, CallerType, Revision&>()._sub_iteration = 4;
				base_this->template Route_Planning_Time<ComponentType, CallerType, Revision&>()._iteration = min(_iteration+1, (int)planning_time);
				base_this->template Route_Planning_Time<ComponentType, CallerType, Revision&>()._sub_iteration = 5;

				//========================================================================
				//TODO: remove stored versions of the planning times after testing
				Revision &start	= base_this->template Start_Time_Planning_Time<ComponentType, CallerType, Revision&>();
				Revision &dur		= base_this->template Duration_Planning_Time<ComponentType, CallerType, Revision&>();
				Revision &loc		= base_this->template Location_Planning_Time<ComponentType, CallerType, Revision&>();
				Revision &mode	= base_this->template Mode_Planning_Time<ComponentType, CallerType, Revision&>();
				Revision &persons	= base_this->template Involved_Persons_Planning_Time<ComponentType, CallerType, Revision&>();
				Revision &route	= base_this->template Route_Planning_Time<ComponentType, CallerType, Revision&>();
				Revision &Stored_start	= base_this->template Stored_Start_Time_Planning_Time<ComponentType, CallerType, Revision&>();
				Revision &Stored_dur		= base_this->template Stored_Duration_Planning_Time<ComponentType, CallerType, Revision&>();
				Revision &Stored_loc		= base_this->template Stored_Location_Planning_Time<ComponentType, CallerType, Revision&>();
				Revision &Stored_mode	= base_this->template Stored_Mode_Planning_Time<ComponentType, CallerType, Revision&>();
				Revision &Stored_persons	= base_this->template Stored_Involved_Persons_Planning_Time<ComponentType, CallerType, Revision&>();
				Revision &Stored_route	= base_this->template Stored_Route_Planning_Time<ComponentType, CallerType, Revision&>();
				//========================================================================

				//========================================================================
				//TODO: remove stored versions of the planning times after testing
				Stored_start._iteration = start._iteration;
				Stored_dur._iteration = dur._iteration;
				Stored_loc._iteration = loc._iteration;
				Stored_mode._iteration = mode._iteration;
				Stored_persons._iteration = persons._iteration;
				Stored_route._iteration = route._iteration;
				
				Stored_start._sub_iteration = start._sub_iteration;
				Stored_dur._sub_iteration = dur._sub_iteration;
				Stored_loc._sub_iteration = loc._sub_iteration;
				Stored_mode._sub_iteration = mode._sub_iteration;
				Stored_persons._sub_iteration = persons._sub_iteration;
				Stored_route._sub_iteration = route._sub_iteration;
				//========================================================================
			}
			feature_implementation void Set_Attribute_Planning_Times(TargetType planning_time, requires(!check_2(TargetType, Simulation_Timestep_Increment, is_same)))
			{
				assert_check_2(TargetType, Simulation_Timestep_Increment, is_same, "Error: planning_time must be passed as a Simulation_Timestep_Increment type when using this function.");
			}
			tag_feature_as_available(Set_Attribute_Planning_Times);

			feature_implementation void Location_Planning_Event_Handler()
			{
				this_itf* pthis = (this_itf*)this;
				base_type* base_this = (base_type*)this;
				_person_itf* person = base_this->_Parent_Planner->template Parent_Person<_person_itf*>();
				_activity_location_itf* orig = person->template Home_Location<_activity_location_itf*>();


				// Select the location based on the activity type
				_activity_location_itf* dest = nullptr;		
				if (pthis->template Activity_Type<Types::ACTIVITY_TYPES>() == Types::AT_HOME_ACTIVITY)
				{
					dest = person->template Home_Location<_activity_location_itf*>();
				}
				else if (pthis->template Activity_Type<Types::ACTIVITY_TYPES>() == Types::ACTIVITY_TYPES::PRIMARY_WORK_ACTIVITY)
				{
					dest = person->template Work_Location<_activity_location_itf*>();
				}
				else if (pthis->template Activity_Type<Types::ACTIVITY_TYPES>() == Types::ACTIVITY_TYPES::SCHOOL_ACTIVITY)
				{
					dest = person->template School_Location<_activity_location_itf*>();
				}
				else if (pthis->template Activity_Type<Types::ACTIVITY_TYPES>() == Types::ACTIVITY_TYPES::WORK_AT_HOME_ACTIVITY)
				{
					dest = person->template Home_Location<_activity_location_itf*>();
				}
				else
				{
					//----------------------------------------------------------------
					// Print to log file
					stringstream s;
					s <<"ACTIVITY NOT GENERATED, only work, home, school and work at home activities can be routine at this point: "<< person->template uuid<int>();
					s << "," <<orig << ", " <<dest<<endl;
					//base_this->_Parent_Planner->template Write_To_Log<stringstream&>(s);
					//----------------------------------------------------------------
					return;
				}

				// check that origin and destination are valid
				if (orig != nullptr && dest != nullptr) 
				{
					pthis->template Location<_activity_location_itf*>(dest);
				}
				else
				{
					//----------------------------------------------------------------
					// Print to log file
					stringstream s;
					s <<"ACTIVITY NOT GENERATED, null origin or destination: "<< person->template uuid<int>();
					s << "," <<orig << ", " <<dest<<endl;
					//base_this->_Parent_Planner->template Write_To_Log<stringstream&>(s);
					//----------------------------------------------------------------
				}
			}

			feature_implementation void Mode_Planning_Event_Handler()
			{
				base_type* bthis = (base_type*)this;
				this_itf* pthis = (this_itf*)this;

				// references to external agents
				_planning_itf* planner = bthis->template Parent_Planner<ComponentType,CallerType,_planning_itf*>();
				_person_itf* person = planner->template Parent_Person<_person_itf*>();
				_scheduler_itf* scheduler = person->template Scheduling_Faculty<_scheduler_itf*>();
				_dest_choice_itf* dest_chooser = planner->template Destination_Choice_Faculty<_dest_choice_itf*>();
				_network_itf* network = person->template network_reference<_network_itf*>();

				// expected travel time, based on whether start time is planned or not and location is planned or not
				float p_auto = 0.0f;

				// if start time planned use the planned start time, otherwise use 12pm.
				Time_Seconds start;
				if (pthis->template Start_Is_Planned<bool>()) start = pthis->template Start_Time<Time_Seconds>();
				else start = 12.0*60.0*60.0;

				// if location planned, get the travel time to location at start time
				Time_Minutes ttime;
				if (pthis->template Location_Is_Planned<bool>()) 
				{
					// get the previous activity
					_activity_location_itf* orig = scheduler->template previous_location<Target_Type<NT,_activity_location_itf*,this_itf*>>(pthis);
					_activity_location_itf* dest = pthis->template Location<_activity_location_itf*>();
					int o_id = orig->template zone<_zone_itf*>()->template uuid<int>();
					int d_id = dest->template zone<_zone_itf*>()->template uuid<int>();

					// get expected auto travel time
					ttime = network->template Get_LOS<Target_Type<NT,Time_Minutes,int,Vehicle_Components::Types::Vehicle_Type_Keys,Time_Seconds>>(o_id,d_id,Vehicle_Components::Types::Vehicle_Type_Keys::SOV,start);

					if (pthis->template Activity_Type<ACTIVITY_TYPES>() == ACTIVITY_TYPES::PRIMARY_WORK_ACTIVITY)
					{
						if (ttime <10.0f) p_auto=0.737;
						else if (ttime <20.0f) p_auto=0.871;
						else if (ttime <30.0f) p_auto=0.887;
						else if (ttime <40.0f) p_auto=0.889;
						else if (ttime <50.0f) p_auto=0.813;
						else if (ttime <60.0f) p_auto=0.629;
						else if (ttime <70.0f) p_auto=0.687;
						else if (ttime <80.0f) p_auto=0.468;
						else if (ttime <90.0f) p_auto=0.344;
						else if (ttime <100.0f) p_auto=0.501;
						else p_auto=0.39;
					}
					else if (pthis->template Activity_Type<ACTIVITY_TYPES>() == ACTIVITY_TYPES::SCHOOL_ACTIVITY)
					{
						if (ttime <10.0f) p_auto=0.717;
						else if (ttime <20.0f) p_auto=0.491;
						else if (ttime <30.0f) p_auto=0.356;
						else if (ttime <40.0f) p_auto=0.365;
						else if (ttime <50.0f) p_auto=0.276;
						else if (ttime <60.0f) p_auto=0.312;
						else if (ttime <70.0f) p_auto=0.303;
						else if (ttime <80.0f) p_auto=0.312;
						else if (ttime <90.0f) p_auto=0.24;
						else if (ttime <100.0f) p_auto=0.068;
						else p_auto=0.24;
					}
					else THROW_WARNING("Warning, non-work/school activity should not be created as routine");
				}
				// else if location is not planned, use the average auto mode choice percentage
				else
				{
					if (pthis->template Activity_Type<ACTIVITY_TYPES>() == ACTIVITY_TYPES::PRIMARY_WORK_ACTIVITY) p_auto = 0.79f;
					else if (pthis->template Activity_Type<ACTIVITY_TYPES>() == ACTIVITY_TYPES::SCHOOL_ACTIVITY) p_auto = 0.5f;
					else THROW_WARNING("Warning, non-work/school activity should not be created as routine");
				}
			
				if (GLOBALS::Uniform_RNG.Next_Rand<float>() < p_auto) pthis->template Mode<Vehicle_Components::Types::Vehicle_Type_Keys>(Vehicle_Components::Types::Vehicle_Type_Keys::SOV);
				else pthis->template Mode<Vehicle_Components::Types::Vehicle_Type_Keys>(Vehicle_Components::Types::Vehicle_Type_Keys::BUS);
			}

			feature_implementation void Duration_Planning_Event_Handler()
			{
				this_itf* pthis = (this_itf*)this;
				base_type* bthis = (base_type*)this;

				_person_itf* person = bthis->_Parent_Planner->template Parent_Person<_person_itf*>();
				_static_properties_itf* static_properties = person->template Static_Properties<_static_properties_itf*>();

				Time_Minutes duration; 	
	
				if (pthis->template Activity_Type<ACTIVITY_TYPES>() == PRIMARY_WORK_ACTIVITY || pthis->template Activity_Type<ACTIVITY_TYPES>() == WORK_AT_HOME_ACTIVITY)
				{
					duration = static_properties->template Work_Hours<Time_Minutes>()/5.0;
				}
				else if (pthis->template Activity_Type<Types::ACTIVITY_TYPES>() == Types::ACTIVITY_TYPES::SCHOOL_ACTIVITY)
				{
					duration = 60.0 * 7.0;
				}
				else
				{
					THROW_EXCEPTION("ERROR: only work and school activities are currently allowed to be routine.");
				}
				pthis->template Duration<Time_Minutes>(duration);

			}

			feature_implementation void Start_Time_Planning_Event_Handler()
			{

				this_itf* pthis = (this_itf*)this;
				base_type* bthis = (base_type*)this;

				_person_itf* person = bthis->_Parent_Planner->template Parent_Person<_person_itf*>();
				_static_properties_itf* static_properties = person->template Static_Properties<_static_properties_itf*>();
				_scheduler_itf* scheduler = person->template Scheduling_Faculty<_scheduler_itf*>();
				_scenario_itf* scenario = (_scenario_itf*)_global_scenario;

				ACTIVITY_TYPES act_type = pthis->template Activity_Type<ACTIVITY_TYPES>();

				// School Activity start time (randomly between 7 and 9AM)
				if (act_type == ACTIVITY_TYPES::SCHOOL_ACTIVITY || act_type == ACTIVITY_TYPES::PRIMARY_WORK_ACTIVITY)
				{
					// interfaces
					_planning_itf* planner = pthis->template Parent_Planner<_planning_itf*>();
					_person_itf* person = planner->template Parent_Person<_person_itf*>();

					// get the combined start time and duration
					_timing_choice_itf* timing_planner = planner->template Timing_Chooser<_timing_choice_itf*>();
					pair<Time_Seconds,Time_Seconds> start_and_duration = timing_planner->template Get_Start_Time_and_Duration<Target_Type<NT,Time_Seconds,ComponentType*>>(this);

					// make sure start time is not prior to current iteration
					Time_Seconds time_min = Simulation_Time.template Future_Time<Time_Seconds,Time_Seconds>(planner->template Planning_Time_Increment<Time_Seconds>());
					pthis->template Start_Time<Time_Seconds>(std::max<int>(start_and_duration.first,time_min.Value));
					pthis->template Duration<Time_Seconds>(start_and_duration.second);
				}
				else THROW_EXCEPTION("ERROR: only work and school activities are currently allowed to be routine.");


				//========================================================================
				// Resolve timing conflicts when timing is known
				//------------------------------------------------------------------------
				bool is_scheduled =	scheduler->template Resolve_Timing_Conflict<this_itf*>(pthis,false);
				// if conflict not resolved remove activity from schedule and modify routing response time so no further planning is done
				if (!is_scheduled) 
				{
					pthis->template Unschedule_Activity_Events<NT>();
					scheduler->template Remove_Activity_Plan<this_itf*>(pthis);
				}


				//========================================================================
				// END HERE if no planner routing is requested through scenario
				if (!scenario->template do_planner_routing<bool>()) return;
				//------------------------------------------------------------------------



				//==============================================================================================
				// OTHERWISE, reset the event time for the planning router based on expected departure time
				//------------------------------------------------------------------------
				// start routing on the planning timestep at 1.5 times the estimated travel time from skims prior to departure - rounded to nearest minute
				Time_Minutes exp_ttime=0.0f;
				if (bthis->template Location_Is_Planned<ComponentType,CallerType,NT>())
				{
					_network_itf* network = person->template network_reference<_network_itf*>();
					_zone_itf* o_zone = person->template Home_Location<_zone_itf*>();
					_zone_itf* d_zone = bthis->template Location<ComponentType,CallerType,_activity_location_itf*>()->template zone<_zone_itf*>();

					exp_ttime = network->template Get_LOS<Target_Type<NT,Time_Minutes,int,Vehicle_Components::Types::Vehicle_Type_Keys, Time_Hours> >(o_zone->template uuid<int>(),d_zone->template uuid<int>(),Vehicle_Components::Types::Vehicle_Type_Keys::SOV, pthis->template Start_Time<Time_Hours>());
				}
				exp_ttime = std::max<float>(exp_ttime,30.0f);
				
				//---------------------------
				// Aggregate plan routing
				if (scenario->template aggregate_routing<bool>())
				{
					Time_Minutes start_minutes = (int)(pthis->template Start_Time<Time_Minutes>() - (exp_ttime * 2.0));
					int start_increment = std::max<int>(Simulation_Time.template Convert_Time_To_Simulation_Timestep<Time_Minutes>(start_minutes), _iteration);
					pthis->template Route_Planning_Time<Revision&>()._iteration = start_increment;
				}
				//---------------------------
				// Disaggregate plan routing
				else
				{
					Simulation_Timestep_Increment start_seconds = pthis->template Start_Time<Simulation_Timestep_Increment>() - Simulation_Time.template Convert_Time_To_Simulation_Timestep<Time_Minutes>(exp_ttime * 2.0);
					int start_increment = std::max<int>(start_seconds, _iteration);
					pthis->template Route_Planning_Time<Revision&>()._iteration = start_increment;
				}
			}

			feature_implementation void Involved_Persons_Planning_Event_Handler()
			{
				
			}

			feature_implementation void Set_Meta_Attributes()
			{
				//===========================================================================================================================
				// Initialize local variables for use
				//---------------------------------------------------------------------------------------------------------------------------

				// get references to use in model
				base_type* base_this = (base_type*)this;
				this_itf* pthis = (this_itf*)this;
				_planning_itf* planner = pthis->template Parent_Planner<_planning_itf*>();
				_person_itf* person = planner->template Parent_Person<_person_itf*>();
				_properties_itf* properties = person->template Properties<_properties_itf*>();
				_static_properties_itf* static_props = person->template Static_Properties<_static_properties_itf*>();

				//Create local variables
				int ACT1, ACT2, ACT3, ACT4, ACT5, Employed, Student, Male, Senior, TELEWORK, ICT_USE;
				ACT1 = 0; ACT2 = 0; ACT3 = 0; ACT4 = 0; ACT5 = 0; Employed = 0; Student = 0; Male = 0; Senior = 0; TELEWORK = 0; ICT_USE = 0;
				float AvgDur, AvgFreq;

				if (base_this->_Activity_Type == WORK_AT_HOME_ACTIVITY || base_this->_Activity_Type == PRIMARY_WORK_ACTIVITY || base_this->_Activity_Type == SCHOOL_ACTIVITY || base_this->_Activity_Type == OTHER_WORK_ACTIVITY) ACT1 = 1;
				else if (base_this->_Activity_Type == HEALTHCARE_ACTIVITY || base_this->_Activity_Type == RELIGIOUS_OR_CIVIC_ACTIVITY || base_this->_Activity_Type == PERSONAL_BUSINESS_ACTIVITY) ACT2 = 1;
				else if (base_this->_Activity_Type == Types::ERRANDS_ACTIVITY || base_this->_Activity_Type == PICK_UP_OR_DROP_OFF_ACTIVITY || base_this->_Activity_Type == SERVICE_VEHICLE_ACTIVITY) ACT3 = 1;
				else if (base_this->_Activity_Type == Types::LEISURE_ACTIVITY|| base_this->_Activity_Type == Types::EAT_OUT_ACTIVITY || base_this->_Activity_Type == Types::RECREATION_ACTIVITY || base_this->_Activity_Type == Types::SOCIAL_ACTIVITY) ACT4 = 1;
				else if (base_this->_Activity_Type == Types::OTHER_SHOPPING_ACTIVITY ||base_this-> _Activity_Type == Types::MAJOR_SHOPPING_ACTIVITY) ACT5 = 1;
				if (static_props->template Is_Employed<bool>()) Employed = 1;
				if (static_props->template Is_Student<bool>()) Student = 1;
				if (static_props->template Gender<GENDER>() == GENDER::MALE) Male = 1;
				if (static_props->template Age<int>() >= 65) Senior = 1;
				//if (PER.PersonData.ICT_Use != IctType.NULL || PER.PersonData.ICT_Use != IctType.UseLow) ICT_USE = 1;
				if (static_props->template Journey_To_Work_Mode<Person_Components::Types::JOURNEY_TO_WORK_MODE>() == JOURNEY_TO_WORK_MODE::WORKMODE_WORK_AT_HOME) TELEWORK = 1;
				AvgFreq = properties->template Average_Activity_Frequency<Target_Type<NT,float,ACTIVITY_TYPES>>(base_this->_Activity_Type);
				AvgDur = properties->template Average_Activity_Duration<Target_Type<NT,Time_Minutes,ACTIVITY_TYPES>>(base_this->_Activity_Type);


				//===========================================================================================================================
				// FLEXIBILITY CALCULATIONS
				//---------------------------------------------------------------------------------------------------------------------------
				matrix<double> alpha = matrix<double>(matrix<double>::index_type(3,5));
				alpha(0,0)=-99999999;	alpha(0,1)=-99999999;	alpha(0,2)=-99999999;	alpha(0,3)=-99999999;	alpha(0,4)=-99999999;
				alpha(1,0)=0;			alpha(1,1)=0;			alpha(1,2)=0;			alpha(1,3)=0;			alpha(1,4)=0;
				alpha(2,0)=99999999;	alpha(2,1)=99999999;	alpha(2,2)=99999999;	alpha(2,3)=99999999;	alpha(2,4)=99999999;

				matrix<double> Sigma = matrix<double>(matrix<double>::index_type(5,5)); // covariance matrix
				Sigma(0,0)=1;			Sigma(0,1)=0;			Sigma(0,2)=0;			Sigma(0,3)=0;			Sigma(0,4)=0;
				Sigma(1,0)=0.099646;	Sigma(1,1)=1;			Sigma(1,2)=0;			Sigma(1,3)=0;			Sigma(1,4)=0;
				Sigma(2,0)=-0.053507;	Sigma(2,1)=-0.03372;	Sigma(2,2)=1;			Sigma(2,3)=0;			Sigma(2,4)=0;
				Sigma(3,0)=0.095299;	Sigma(3,1)=0.305332;	Sigma(3,2)=-0.113887;	Sigma(3,3)=1;			Sigma(3,4)=0;
				Sigma(4,0)=0.124233;	Sigma(4,1)=0.168151;	Sigma(4,2)=0.025553;	Sigma(4,3)=-0.099794;	Sigma(4,4)=1;

				double P_per[2]; // Holds probability for each level (index by i) of each response (index by j)
				double P_mod[2];
				double P_loc[2];
				double P_tim[2];
				double P_dur[2];

				//Calculate xB values for each response
				double xB_mod = -0.383692 + 0.362839 * Employed + 0.4215 * Student - 0.217904 * Male - 0.267344 * Senior + 0.691999 * Male * Senior + 0.231177 * AvgFreq + 0.568976 * ACT1 * ICT_USE - 1.039075 * ACT1 * AvgFreq;
				double xB_per = -0.338294 + 0.347752 * Student - 0.536749 * Male + 0.259776 * Senior + 0.478152 * Male * Senior + 0.532325 * ICT_USE + 1.508163 * TELEWORK - 1.655547 * ICT_USE * TELEWORK - 1.390818 * ACT1 * +1.352461 * ACT1 * TELEWORK - 0.598393 * ACT2 + 11.034344 * ACT2 * AvgDur - 0.617948 * ACT3 + 1.727868 * AvgFreq * ACT3 + 0.437761 * AvgFreq * ACT4 + 6.708181 * ACT4 * AvgDur - 0.606538 * ACT5 + 0.325567 * AvgFreq * ACT5;
				double xB_loc = -0.368786 - 1.174171 * Male - 0.1914 * Senior + 1.346607 * Male * Senior - 0.3674 * ICT_USE + 0.99392 * TELEWORK - 0.815509 * ICT_USE * TELEWORK + 1.023933 * ACT1 * TELEWORK + 2.325112 * ACT1 * AvgFreq - 8.966545 * ACT2 * AvgDur - 1.02207 * AvgFreq * ACT3 + 0.904036 * ACT4 + 0.637477 * AvgFreq * ACT4 - 8.32642 * ACT4 * AvgDur + 2.382859 * ACT5;
				double xB_tim = 1.326259 - 0.38711 * Male - 0.226637 * Senior + 0.484623 * Male * Senior - 0.302255 * ICT_USE + 0.196488 * TELEWORK + 3.3758 * ACT2 * AvgDur - 0.400564 * ACT3 + 1.301273 * AvgFreq * ACT3 - 0.601686 * ACT5 + 0.192867 * AvgFreq * ACT5;
				double xB_dur = -1.239259 + 1.294471 * Employed + 1.461113 * Student - 0.246144 * Senior - 0.374409 * Male * Senior - 0.23527 * ICT_USE * TELEWORK + 1.30007 * AvgDur + 0.621008 * ACT1 * +0.994951 * ACT1 * ICT_USE - 1.124459 * ACT1 * TELEWORK - 0.56222 * ACT1 * AvgFreq - 2.070631 * ACT1 * AvgDur - 0.167278 * ICT_USE * ACT4 + 0.769745 * AvgFreq * ACT4 + 0.487925 * AvgFreq * ACT5;			

				// Get probabilities of each response level
				for (int i = 0; i < 2; i++)
				{
					P_mod[i] = Normal_Distribution->template Cumulative_Distribution_Value<float>(alpha(i + 1, 0) - xB_mod);
					P_per[i] = Normal_Distribution->template Cumulative_Distribution_Value<float>(alpha(i + 1, 1) - xB_per);
					P_loc[i] = Normal_Distribution->template Cumulative_Distribution_Value<float>(alpha(i + 1, 2) - xB_loc);
					P_tim[i] = Normal_Distribution->template Cumulative_Distribution_Value<float>(alpha(i + 1, 3) - xB_tim);
					P_dur[i] = Normal_Distribution->template Cumulative_Distribution_Value<float>(alpha(i + 1, 4) - xB_dur);
				}

				// Set correlated random variables
				vector<double> rand;
				GLOBALS::Normal_RNG.template Correlated_Rands<double>(rand, Sigma);

				// Set flexibility values
				for (int i = 0; i < 2; i++)
				{
					if (rand[0] < P_mod[i]) {base_this->_Mode_Flexibility = (Types::FLEXIBILITY_VALUES)i;				rand[0] = 999;}
					if (rand[1] < P_per[i]) {base_this->_Involved_Persons_Flexibility = (Types::FLEXIBILITY_VALUES)i;	rand[1] = 999; }
					if (rand[2] < P_loc[i]) {base_this->_Location_Flexibility = (Types::FLEXIBILITY_VALUES)i;			rand[2] = 999; }
					if (rand[3] < P_tim[i]) {base_this->_Start_Time_Flexibility = (Types::FLEXIBILITY_VALUES)i;			rand[3] = 999; }
					if (rand[4] < P_dur[i]) {base_this->_Duration_Flexibility = (Types::FLEXIBILITY_VALUES)i;			rand[4] = 999; }
				}
            
				// Set results of flexiblity model to use in next models
				double IFLOC, IFTIM, IFDUR, IFMOD;
				IFMOD = P_mod[0];
				IFLOC = P_loc[0];
				IFTIM = P_tim[0];
				IFDUR = P_dur[0];

				pthis->Location_Flexibility<Types::FLEXIBILITY_VALUES>(Types::FLEXIBILITY_VALUES::LOW_FLEXIBILITY);
			}
			tag_feature_as_available(Set_Meta_Attributes);
		};



		// Specialization for ADAPTS-style at-home activities
		implementation struct ADAPTS_At_Home_Activity_Plan_Implementation : public Basic_Activity_Plan_Implementation<MasterType,ParentType,APPEND_CHILD(ADAPTS_At_Home_Activity_Plan_Implementation)>
		{
			typedef Basic_Activity_Plan_Implementation<MasterType,ParentType,APPEND_CHILD(ADAPTS_At_Home_Activity_Plan_Implementation)> base_type;
			typedef typename Basic_Activity_Plan_Implementation<MasterType,ParentType,APPEND_CHILD(ADAPTS_At_Home_Activity_Plan_Implementation)>::Component_Type ComponentType;
			typedef Prototypes::Activity_Planner<ComponentType,ComponentType> this_itf;
		
			//================================================================================================================================================================================================
			//================================================================================================================================================================================================
			// Interfaces
			define_component_interface(_properties_itf, typename type_of(base_type::Parent_Planner)::type_of(Parent_Person)::type_of(Properties), Person_Components::Prototypes::Person_Properties, ComponentType);
			define_component_interface(_static_properties_itf, typename type_of(base_type::Parent_Planner)::type_of(Parent_Person)::type_of(Static_Properties), Person_Components::Prototypes::Person_Properties, ComponentType);
			define_component_interface(_planning_itf,typename type_of(base_type::Parent_Planner),Person_Components::Prototypes::Person_Planner,ComponentType);
			define_component_interface(_person_itf,typename _planning_itf::get_type_of(Parent_Person),Person_Components::Prototypes::Person,ComponentType);
			define_component_interface(_dest_choice_itf,typename _planning_itf::get_type_of(Destination_Choice_Faculty),Person_Components::Prototypes::Destination_Chooser,ComponentType);
			define_component_interface(_scenario_itf, typename type_of(base_type::Parent_Planner)::type_of(Parent_Person)::type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
			define_component_interface(_network_itf, typename type_of(base_type::Parent_Planner)::type_of(Parent_Person)::type_of(network_reference), Network_Components::Prototypes::Network_Prototype, ComponentType);	
			define_component_interface(_skim_itf, typename _network_itf::get_type_of(skimming_faculty),Network_Skimming_Components::Prototypes::Network_Skimming_Prototype,ComponentType);
			define_component_interface(_timing_choice_itf,typename _planning_itf::get_type_of(Timing_Chooser),Person_Components::Prototypes::Activity_Timing_Chooser,ComponentType);
			define_container_and_value_interface(_activity_locations_container_itf, _activity_location_itf, typename _network_itf::get_type_of(activity_locations_container), Random_Access_Sequence_Prototype, Activity_Location_Components::Prototypes::Activity_Location_Prototype, ComponentType);
			define_container_and_value_interface(_links_container_itf, _link_itf, typename _activity_location_itf::get_type_of(origin_links), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
			define_container_and_value_interface(_zones_container_itf, _zone_itf, typename _network_itf::get_type_of(zones_container), Associative_Container_Prototype, Zone_Components::Prototypes::Zone_Prototype, ComponentType);
		/*	define_container_and_value_interface(_activity_plans_container_itf,_activity_plan_itf, typename type_of(base_type::Parent_Planner)::type_of(Activity_Container),Containers::Back_Insertion_Sequence_Prototype,Activity_Components::Prototypes::Activity_Planner,ComponentType);
			define_container_and_value_interface(_movement_plans_container_itf,_movement_plan_itf, typename type_of(base_type::Parent_Planner)::type_of(Movement_Plans_Container),Containers::Back_Insertion_Sequence_Prototype,Movement_Plan_Components::Prototypes::Movement_Plan_Prototype,ComponentType);*/
			//================================================================================================================================================================================================
			//================================================================================================================================================================================================

			// Fundamental activity properties
			feature_implementation Types::PLAN_HORIZON_VALUES Activity_Plan_Horizon()
			{
				return Types::PLAN_HORIZON_VALUES::ROUTINE;
			}
			tag_getter_as_available(Activity_Plan_Horizon);

			// Activity attribute planning properties
			feature_implementation Types::PLAN_HORIZON_VALUES Location_Plan_Horizon()
			{
				return Types::PLAN_HORIZON_VALUES::ROUTINE;
			}
			tag_getter_as_available(Location_Plan_Horizon);
			feature_implementation Types::PLAN_HORIZON_VALUES Mode_Plan_Horizon()
			{
				return Types::PLAN_HORIZON_VALUES::ROUTINE;
			}
			tag_getter_as_available(Mode_Plan_Horizon);
			feature_implementation Types::PLAN_HORIZON_VALUES Start_Time_Plan_Horizon()
			{
				return Types::PLAN_HORIZON_VALUES::ROUTINE;
			}
			tag_getter_as_available(Start_Time_Plan_Horizon);
			feature_implementation Types::PLAN_HORIZON_VALUES Duration_Plan_Horizon()
			{
				return Types::PLAN_HORIZON_VALUES::ROUTINE;
			}
			tag_getter_as_available(Duration_Plan_Horizon);
			feature_implementation Types::PLAN_HORIZON_VALUES Involved_Persons_Plan_Horizon()
			{
				return Types::PLAN_HORIZON_VALUES::ROUTINE;
			}
			tag_getter_as_available(Involved_Persons_Plan_Horizon);
			/*member_data(Types::FLEXIBILITY_VALUES, Location_Flexibility, none, none);
			member_data(Types::FLEXIBILITY_VALUES, Mode_Flexibility, none, none);
			member_data(Types::FLEXIBILITY_VALUES, Start_Time_Flexibility,none,none);
			member_data(Types::FLEXIBILITY_VALUES, Duration_Flexibility,none,none);
			member_data(Types::FLEXIBILITY_VALUES, Involved_Persons_Flexibility,none,none);*/


			// Activity methods
			feature_implementation void Initialize(typename TargetType::ParamType start_time, typename TargetType::ParamType duration, typename TargetType::Param2Type mode)
			{
				
				//UNLOCK(this->_update_lock);
				this_itf* pthis = (this_itf*)this;

				_planning_itf* planner = pthis->template Parent_Planner<_planning_itf*>();
				_person_itf* person = planner->template Parent_Person<_person_itf*>();
				
				pthis->template Activity_Type<Types::ACTIVITY_TYPES>(Types::ACTIVITY_TYPES::AT_HOME_ACTIVITY);
				pthis->template Duration<typename TargetType::ParamType>(duration);
				pthis->template Start_Time<typename TargetType::ParamType>(start_time);
				pthis->template Location<_activity_location_itf*>(person->template Home_Location<_activity_location_itf*>());
				pthis->template Mode<Vehicle_Components::Types::Vehicle_Type_Keys>(mode);
			}

			feature_implementation void Set_Attribute_Planning_Times(TargetType planning_time, requires(check_2(TargetType, Simulation_Timestep_Increment, is_same)))
			{
				base_type* base_this = (base_type*)this;

				base_this->template Start_Time_Planning_Time<ComponentType, CallerType, Revision&>()._iteration = END+1;
				base_this->template Start_Time_Planning_Time<ComponentType, CallerType, Revision&>()._sub_iteration = END+1;
				base_this->template Duration_Planning_Time<ComponentType, CallerType, Revision&>()._iteration = END+1;
				base_this->template Duration_Planning_Time<ComponentType, CallerType, Revision&>()._sub_iteration = END+1;
				base_this->template Location_Planning_Time<ComponentType, CallerType, Revision&>()._iteration = END+1;
				base_this->template Location_Planning_Time<ComponentType, CallerType, Revision&>()._sub_iteration = END+1;
				base_this->template Mode_Planning_Time<ComponentType, CallerType, Revision&>()._iteration = END+1;
				base_this->template Mode_Planning_Time<ComponentType, CallerType, Revision&>()._sub_iteration = END+1;
				base_this->template Involved_Persons_Planning_Time<ComponentType, CallerType, Revision&>()._iteration = END+1;
				base_this->template Involved_Persons_Planning_Time<ComponentType, CallerType, Revision&>()._sub_iteration = END+1;
				base_this->template Route_Planning_Time<ComponentType, CallerType, Revision&>()._iteration = _iteration+1;
				base_this->template Route_Planning_Time<ComponentType, CallerType, Revision&>()._sub_iteration = 0;

				//========================================================================
				//TODO: remove stored versions of the planning times after testing
				Revision &start	= base_this->template Start_Time_Planning_Time<ComponentType, CallerType, Revision&>();
				Revision &dur		= base_this->template Duration_Planning_Time<ComponentType, CallerType, Revision&>();
				Revision &loc		= base_this->template Location_Planning_Time<ComponentType, CallerType, Revision&>();
				Revision &mode	= base_this->template Mode_Planning_Time<ComponentType, CallerType, Revision&>();
				Revision &persons	= base_this->template Involved_Persons_Planning_Time<ComponentType, CallerType, Revision&>();
				Revision &route	= base_this->template Route_Planning_Time<ComponentType, CallerType, Revision&>();
				Revision &Stored_start	= base_this->template Stored_Start_Time_Planning_Time<ComponentType, CallerType, Revision&>();
				Revision &Stored_dur		= base_this->template Stored_Duration_Planning_Time<ComponentType, CallerType, Revision&>();
				Revision &Stored_loc		= base_this->template Stored_Location_Planning_Time<ComponentType, CallerType, Revision&>();
				Revision &Stored_mode	= base_this->template Stored_Mode_Planning_Time<ComponentType, CallerType, Revision&>();
				Revision &Stored_persons	= base_this->template Stored_Involved_Persons_Planning_Time<ComponentType, CallerType, Revision&>();
				Revision &Stored_route	= base_this->template Stored_Route_Planning_Time<ComponentType, CallerType, Revision&>();
				//========================================================================

				//========================================================================
				//TODO: remove stored versions of the planning times after testing
				Stored_start._iteration = start._iteration;
				Stored_dur._iteration = dur._iteration;
				Stored_loc._iteration = loc._iteration;
				Stored_mode._iteration = mode._iteration;
				Stored_persons._iteration = persons._iteration;
				Stored_route._iteration = route._iteration;
				
				Stored_start._sub_iteration = start._sub_iteration;
				Stored_dur._sub_iteration = dur._sub_iteration;
				Stored_loc._sub_iteration = loc._sub_iteration;
				Stored_mode._sub_iteration = mode._sub_iteration;
				Stored_persons._sub_iteration = persons._sub_iteration;
				Stored_route._sub_iteration = route._sub_iteration;
				//========================================================================
			}
			feature_implementation void Set_Attribute_Planning_Times(TargetType planning_time, requires(!check_2(TargetType, Simulation_Timestep_Increment, is_same)))
			{
				assert_check_2(TargetType, Simulation_Timestep_Increment, is_same, "Error: planning_time must be passed as a Simulation_Timestep_Increment type when using this function.");
			}
			tag_feature_as_available(Set_Attribute_Planning_Times);

			feature_implementation void Location_Planning_Event_Handler(){} tag_feature_as_available(Location_Planning_Event_Handler);
			feature_implementation void Mode_Planning_Event_Handler(){} tag_feature_as_available(Mode_Planning_Event_Handler);
			feature_implementation void Duration_Planning_Event_Handler(){} tag_feature_as_available(Duration_Planning_Event_Handler);
			feature_implementation void Start_Time_Planning_Event_Handler(){} tag_feature_as_available(Start_Time_Planning_Event_Handler);
			feature_implementation void Involved_Persons_Planning_Event_Handler(){} tag_feature_as_available(Involved_Persons_Planning_Event_Handler);

			feature_implementation void Set_Meta_Attributes()
			{
				base_type* base_this = (base_type*)this;
				base_this->_Duration_Flexibility = Types::FLEXIBILITY_VALUES::HIGH_FLEXIBILITY;
				base_this->_Location_Flexibility = Types::FLEXIBILITY_VALUES::HIGH_FLEXIBILITY;
				base_this->_Mode_Flexibility = Types::FLEXIBILITY_VALUES::HIGH_FLEXIBILITY;
				base_this->_Start_Time_Flexibility = Types::FLEXIBILITY_VALUES::HIGH_FLEXIBILITY;
				base_this->_Involved_Persons_Flexibility = Types::FLEXIBILITY_VALUES::HIGH_FLEXIBILITY;
		
				this->_Duration_Planning_Time = Types::PLAN_HORIZON_VALUES::IMPULSIVE;
				this->_Location_Planning_Time = Types::PLAN_HORIZON_VALUES::IMPULSIVE;
				this->_Mode_Planning_Time = Types::PLAN_HORIZON_VALUES::IMPULSIVE;
				this->_Start_Time_Planning_Time = Types::PLAN_HORIZON_VALUES::IMPULSIVE;
				this->_Involved_Persons_Planning_Time = Types::PLAN_HORIZON_VALUES::IMPULSIVE;
			}
			tag_feature_as_available(Set_Meta_Attributes);
		};



		// Stripped down activity record with minimal memory usage (used for storing and printing completed activities)
		implementation struct Activity_Record : public Polaris_Component<APPEND_CHILD(Activity_Record), MasterType, Data_Object, ParentType>
		{
			// Tag as implementation
			typedef typename Polaris_Component<APPEND_CHILD(Activity_Record),MasterType,Data_Object>::Component_Type ComponentType;

			feature_implementation void Initialize(TargetType object/*,requires(check(TargetType, Concepts::Is_Activity_Plan_Prototype))*/)
			{
				typedef Activity_Location_Components::Prototypes::Activity_Location_Prototype<typename MasterType::activity_location_type> location_itf;
				typedef Zone_Components::Prototypes::Zone_Prototype<typename MasterType::zone_type> zone_itf;
				typedef Prototypes::Activity_Planner<typename MasterType::activity_type> object_itf;
				object_itf* obj = (object_itf*)object;

				this->_Activity_Plan_ID = obj->Activity_Plan_ID<char>();
				this->_Activity_Type = (char)obj->Activity_Type<Types::ACTIVITY_TYPES>();
				this->Location<ComponentType,CallerType,location_itf*>(obj->Location<location_itf*>());
				//this->Duration<ComponentType,CallerType,Time_Minutes>(obj->Duration<Time_Minutes>());
				//this->Start_Time<ComponentType,CallerType,Time_Minutes>(obj->Start_Time<Time_Minutes>());
				//this->Travel_Time<ComponentType,CallerType,Time_Minutes>(obj->Expected_Travel_Time<Time_Minutes>());
			}

			// Fundamental activity properties
			member_data(char, Activity_Plan_ID, check(ReturnValueType,is_arithmetic), check(SetValueType,is_arithmetic));
			member_data(char, Activity_Type,none,none);
			member_prototype(Activity_Location_Components::Prototypes::Activity_Location_Prototype, Location, typename MasterType::activity_location_type, check(ReturnValueType,Activity_Location_Components::Concepts::Is_Activity_Location), check(SetValueType,Activity_Location_Components::Concepts::Is_Activity_Location));
			member_data(Time_Minutes, Start_Time, check_2(ReturnValueType,Time_Minutes, is_same),check_2(SetValueType,Time_Minutes, is_same));
			member_data(Time_Minutes, Duration, check_2(ReturnValueType,Time_Minutes, is_same),check_2(SetValueType,Time_Minutes, is_same));
			member_data(Time_Minutes, Travel_Time, check_2(ReturnValueType,Time_Minutes, is_same),check_2(SetValueType,Time_Minutes, is_same));
		};



		//=======================================================================
		// Static activity plan member initialization
		//-----------------------------------------------------------------------
		//static_array_definition(Basic_Activity_Plan_Implementation, Location_Planning_Counter, int);
		static_array_definition(Basic_Activity_Plan_Implementation, Route_Planning_Counter, int);
		//static_array_definition(Basic_Activity_Plan_Implementation, Scheduling_Counter, int);
		//static_member_initialization(Basic_Activity_Plan_Implementation, Location_Planning_Count, 0);
		static_member_initialization(Basic_Activity_Plan_Implementation, Route_Planning_Count, 0);
		//static_member_initialization(Basic_Activity_Plan_Implementation, Scheduling_Count, 0);
		template<typename MasterType,typename ParentType, typename InheritanceList> _lock Basic_Activity_Plan_Implementation<MasterType, ParentType, InheritanceList>::_update_lock = 0;
	}
}
